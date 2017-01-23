/*
 * Jalview - A Sequence Alignment Editor and Viewer (Version 2.4)
 * Copyright (C) 2008 AM Waterhouse, J Procter, G Barton, M Clamp, S Searle
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */
package jalview.ws;

import javax.swing.*;

import jalview.bin.*;
import jalview.datamodel.*;
import jalview.gui.*;
import jalview.gui.FeatureRenderer.FeatureRendererSettings;

public abstract class WSThread extends Thread
{
  /**
   * Generic properties for Web Service Client threads.
   */
  /**
   * view that this job was associated with
   */
  AlignmentI currentView = null;

  /**
   * feature settings from view that job was associated with
   */
  FeatureRendererSettings featureSettings = null;

  /**
   * metadata about this web service
   */
  WebserviceInfo wsInfo = null;

  /**
   * original input data for this job
   */
  AlignmentView input = null;

  /**
   * dataset sequence relationships to be propagated onto new results
   */
  AlignedCodonFrame[] codonframe = null;

  /**
   * are there jobs still running in this thread.
   */
  boolean jobComplete = false;

  abstract class WSJob
  {
    /**
     * Generic properties for an individual job within a Web Service Client
     * thread
     */
    int jobnum = 0; // WebServiceInfo pane for this job

    String jobId; // ws job ticket

    /**
     * has job been cancelled
     */
    boolean cancelled = false;

    /**
     * number of exceptions left before job dies
     */
    int allowedServerExceptions = 3;

    /**
     * has job been submitted
     */
    boolean submitted = false;

    /**
     * are all sub-jobs complete
     */
    boolean subjobComplete = false;

    /**
     * 
     * @return true if job has completed and valid results are available
     */
    abstract boolean hasResults();

    /**
     * 
     * @return boolean true if job can be submitted.
     */
    abstract boolean hasValidInput();

    /**
     * The last result object returned by the service.
     */
    vamsas.objects.simple.Result result;
  }

  class JobStateSummary
  {
    /**
     * number of jobs running
     */
    int running = 0;

    /**
     * number of jobs queued
     */
    int queuing = 0;

    /**
     * number of jobs finished
     */
    int finished = 0;

    /**
     * number of jobs failed
     */
    int error = 0;

    /**
     * number of jobs stopped due to server error
     */
    int serror = 0;

    /**
     * number of jobs cancelled
     */
    int cancelled = 0;

    /**
     * number of jobs finished with results
     */
    int results = 0;

    /**
     * processes WSJob and updates job status counters and WebService status
     * displays
     * 
     * @param wsInfo
     * @param OutputHeader
     * @param j
     */
    void updateJobPanelState(WebserviceInfo wsInfo, String OutputHeader,
            WSJob j)
    {
      if (j.result != null)
      {
        String progheader = "";
        // Parse state of job[j]
        if (j.result.isRunning())
        {
          running++;
          wsInfo.setStatus(j.jobnum, WebserviceInfo.STATE_RUNNING);
        }
        else if (j.result.isQueued())
        {
          queuing++;
          wsInfo.setStatus(j.jobnum, WebserviceInfo.STATE_QUEUING);
        }
        else if (j.result.isFinished())
        {
          finished++;
          j.subjobComplete = true;
          if (j.hasResults())
          {
            results++;
          }
          wsInfo.setStatus(j.jobnum, WebserviceInfo.STATE_STOPPED_OK);
        }
        else if (j.result.isFailed())
        {
          progheader += "Job failed.\n";
          j.subjobComplete = true;
          wsInfo.setStatus(j.jobnum, WebserviceInfo.STATE_STOPPED_ERROR);
          error++;
        }
        else if (j.result.isServerError())
        {
          serror++;
          j.subjobComplete = true;
          wsInfo.setStatus(j.jobnum,
                  WebserviceInfo.STATE_STOPPED_SERVERERROR);
        }
        else if (j.result.isBroken() || j.result.isFailed())
        {
          error++;
          j.subjobComplete = true;
          wsInfo.setStatus(j.jobnum, WebserviceInfo.STATE_STOPPED_ERROR);
        }
        // and pass on any sub-job messages to the user
        wsInfo.setProgressText(j.jobnum, OutputHeader);
        wsInfo.appendProgressText(j.jobnum, progheader);
        if (j.result.getStatus() != null)
        {
          wsInfo.appendProgressText(j.jobnum, j.result.getStatus());
        }
      }
      else
      {
        if (j.submitted && j.subjobComplete)
        {
          if (j.allowedServerExceptions == 0)
          {
            serror++;
          }
          else if (j.result == null)
          {
            error++;
          }
        }
      }
    }
  }

  /**
   * one or more jobs being managed by this thread.
   */
  WSJob jobs[] = null;

  /**
   * full name of service
   */
  String WebServiceName = null;

  String OutputHeader;

  String WsUrl = null;

  /**
   * query web service for status of job. on return, job.result must not be null -
   * if it is then it will be assumed that the job status query timed out and a
   * server exception will be logged.
   * 
   * @param job
   * @throws Exception
   *                 will be logged as a server exception for this job
   */
  abstract void pollJob(WSJob job) throws Exception;

  public void run()
  {
    JobStateSummary jstate = null;
    if (jobs == null)
    {
      jobComplete = true;
    }
    while (!jobComplete)
    {
      jstate = new JobStateSummary();
      for (int j = 0; j < jobs.length; j++)
      {

        if (!jobs[j].submitted && jobs[j].hasValidInput())
        {
          StartJob(jobs[j]);
        }

        if (jobs[j].submitted && !jobs[j].subjobComplete)
        {
          try
          {
            pollJob(jobs[j]);
            if (jobs[j].result == null)
            {
              throw (new Exception(
                      "Timed out when communicating with server\nTry again later.\n"));
            }
            jalview.bin.Cache.log.debug("Job " + j + " Result state "
                    + jobs[j].result.getState() + "(ServerError="
                    + jobs[j].result.isServerError() + ")");
          } catch (Exception ex)
          {
            // Deal with Transaction exceptions
            wsInfo.appendProgressText(jobs[j].jobnum, "\n" + WebServiceName
                    + " Server exception!\n" + ex.getMessage());
            Cache.log.warn(WebServiceName + " job(" + jobs[j].jobnum
                    + ") Server exception: " + ex.getMessage());

            if (jobs[j].allowedServerExceptions > 0)
            {
              jobs[j].allowedServerExceptions--;
              Cache.log.debug("Sleeping after a server exception.");
              try
              {
                Thread.sleep(5000);
              } catch (InterruptedException ex1)
              {
              }
            }
            else
            {
              Cache.log.warn("Dropping job " + j + " " + jobs[j].jobId);
              jobs[j].subjobComplete = true;
              wsInfo.setStatus(jobs[j].jobnum,
                      WebserviceInfo.STATE_STOPPED_SERVERERROR);
            }
          } catch (OutOfMemoryError er)
          {
            jobComplete = true;
            jobs[j].subjobComplete = true;
            jobs[j].result = null; // may contain out of date result object
            wsInfo.setStatus(jobs[j].jobnum,
                    WebserviceInfo.STATE_STOPPED_ERROR);
            Cache.log.error("Out of memory when retrieving Job " + j
                    + " id:" + WsUrl + "/" + jobs[j].jobId, er);
            new jalview.gui.OOMWarning("retrieving result for "
                    + WebServiceName, er);
            System.gc();
          }
        }
        jstate.updateJobPanelState(wsInfo, OutputHeader, jobs[j]);
      }
      // Decide on overall state based on collected jobs[] states
      if (jstate.running > 0)
      {
        wsInfo.setStatus(WebserviceInfo.STATE_RUNNING);
      }
      else if (jstate.queuing > 0)
      {
        wsInfo.setStatus(WebserviceInfo.STATE_QUEUING);
      }
      else
      {
        jobComplete = true;
        if (jstate.finished > 0)
        {
          wsInfo.setStatus(WebserviceInfo.STATE_STOPPED_OK);
        }
        else if (jstate.error > 0)
        {
          wsInfo.setStatus(WebserviceInfo.STATE_STOPPED_ERROR);
        }
        else if (jstate.serror > 0)
        {
          wsInfo.setStatus(WebserviceInfo.STATE_STOPPED_SERVERERROR);
        }
      }
      if (!jobComplete)
      {
        try
        {
          Thread.sleep(5000);
        } catch (InterruptedException e)
        {
          Cache.log
                  .debug("Interrupted sleep waiting for next job poll.", e);
        }
        // System.out.println("I'm alive "+alTitle);
      }
    }
    if (jobComplete && jobs != null)
    {
      parseResult(); // tidy up and make results available to user
    }
    else
    {
      Cache.log
              .debug("WebServiceJob poll loop finished with no jobs created.");
      wsInfo.setFinishedNoResults();
    }
  }

  /**
   * submit job to web service
   * 
   * @param job
   */
  abstract void StartJob(WSJob job);

  /**
   * process the set of WSJob objects into a set of results, and tidy up.
   */
  abstract void parseResult();

  /**
   * helper function to conserve dataset references to sequence objects returned
   * from web services 1. Propagates AlCodonFrame data from
   * <code>codonframe</code> to <code>al</code>
   * 
   * @param al
   */
  protected void propagateDatasetMappings(Alignment al)
  {
    if (codonframe != null)
    {
      SequenceI[] alignment = al.getSequencesArray();
      for (int sq = 0; sq < alignment.length; sq++)
      {
        for (int i = 0; i < codonframe.length; i++)
        {
          if (codonframe[i] != null
                  && codonframe[i].involvesSequence(alignment[sq]))
          {
            al.addCodonFrame(codonframe[i]);
            codonframe[i] = null;
            break;
          }
        }
      }
    }
  }

  /**
   * 
   * @param alignFrame
   *                reference for copying mappings across
   * @param wsInfo
   *                gui attachment point
   * @param input
   *                input data for the calculation
   * @param webServiceName
   *                name of service
   * @param wsUrl
   *                url of the service being invoked
   */
  public WSThread(AlignFrame alignFrame, WebserviceInfo wsinfo,
          AlignmentView input, String webServiceName, String wsUrl)
  {
    this(alignFrame, wsinfo, input, wsUrl);
    WebServiceName = webServiceName;
  }

  char defGapChar = '-';

  /**
   * 
   * @return gap character to use for any alignment generation
   */
  public char getGapChar()
  {
    return defGapChar;
  }

  /**
   * 
   * @param alframe -
   *                reference for copying mappings and display styles across
   * @param wsinfo2 -
   *                gui attachment point
   * @param alview -
   *                input data for the calculation
   * @param wsurl2 -
   *                url of the service being invoked
   */
  public WSThread(AlignFrame alframe, WebserviceInfo wsinfo2,
          AlignmentView alview, String wsurl2)
  {
    super();
    // this.alignFrame = alframe;
    currentView = alframe.getCurrentView().getAlignment();
    featureSettings = alframe.getFeatureRenderer().getSettings();
    defGapChar = alframe.getViewport().getGapCharacter();
    this.wsInfo = wsinfo2;
    this.input = alview;
    WsUrl = wsurl2;
    if (alframe != null)
    {
      AlignedCodonFrame[] cf = alframe.getViewport().getAlignment()
              .getCodonFrames();
      if (cf != null)
      {
        codonframe = new AlignedCodonFrame[cf.length];
        System.arraycopy(cf, 0, codonframe, 0, cf.length);
      }
    }
  }
}
