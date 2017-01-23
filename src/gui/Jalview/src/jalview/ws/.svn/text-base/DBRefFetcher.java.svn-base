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

import java.io.*;
import java.util.*;

import org.exolab.castor.mapping.*;
import org.exolab.castor.xml.*;
import jalview.analysis.*;
import jalview.datamodel.*;
import jalview.datamodel.Mapping;
import jalview.gui.*;
import jalview.ws.dbsources.Uniprot;
import jalview.ws.ebi.EBIFetchClient;

/**
 * Implements a runnable for validating a sequence against external databases
 * and then propagating references and features onto the sequence(s)
 * 
 * @author $author$
 * @version $Revision: 1.8 $
 */
public class DBRefFetcher implements Runnable
{
  SequenceI[] dataset;

  IProgressIndicator af;

  CutAndPasteTransfer output = new CutAndPasteTransfer();

  StringBuffer sbuffer = new StringBuffer();

  boolean running = false;

  // /This will be a collection of Vectors of sequenceI refs.
  // The key will be the seq name or accession id of the seq
  Hashtable seqRefs;

  String[] dbSources;

  SequenceFetcher sfetcher;

  public DBRefFetcher()
  {
  }

  /**
   * Creates a new SequenceFeatureFetcher object.
   * 
   * @param seqs
   *                fetch references for these sequences
   * @param af
   *                the parent alignframe for progress bar monitoring.
   */
  public DBRefFetcher(SequenceI[] seqs, AlignFrame af)
  {
    this.af = af;
    SequenceI[] ds = new SequenceI[seqs.length];
    for (int i = 0; i < seqs.length; i++)
    {
      if (seqs[i].getDatasetSequence() != null)
        ds[i] = seqs[i].getDatasetSequence();
      else
        ds[i] = seqs[i];
    }
    this.dataset = ds;
    // TODO Jalview 2.5 lots of this code should be in the gui package!
    sfetcher = jalview.gui.SequenceFetcher.getSequenceFetcherSingleton(af);
    // select appropriate databases based on alignFrame context.
    if (af.getViewport().getAlignment().isNucleotide())
    {
      dbSources = DBRefSource.DNACODINGDBS;
    }
    else
    {
      dbSources = DBRefSource.PROTEINDBS;
    }
  }

  /**
   * start the fetcher thread
   * 
   * @param waitTillFinished
   *                true to block until the fetcher has finished
   */
  public void fetchDBRefs(boolean waitTillFinished)
  {
    Thread thread = new Thread(this);
    thread.start();
    running = true;

    if (waitTillFinished)
    {
      while (running)
      {
        try
        {
          Thread.sleep(500);
        } catch (Exception ex)
        {
        }
      }
    }
  }

  /**
   * The sequence will be added to a vector of sequences belonging to key which
   * could be either seq name or dbref id
   * 
   * @param seq
   *                SequenceI
   * @param key
   *                String
   */
  void addSeqId(SequenceI seq, String key)
  {
    key = key.toUpperCase();

    Vector seqs;
    if (seqRefs.containsKey(key))
    {
      seqs = (Vector) seqRefs.get(key);

      if (seqs != null && !seqs.contains(seq))
      {
        seqs.addElement(seq);
      }
      else if (seqs == null)
      {
        seqs = new Vector();
        seqs.addElement(seq);
      }

    }
    else
    {
      seqs = new Vector();
      seqs.addElement(seq);
    }

    seqRefs.put(key, seqs);
  }

  /**
   * DOCUMENT ME!
   */
  public void run()
  {
    if (dbSources == null)
    {
      throw new Error("Implementation error. Must initialise dbSources");
    }
    long startTime = System.currentTimeMillis();
    af.setProgressBar("Fetching db refs", startTime);
    running = true;
    int db = 0;
    Vector sdataset = new Vector();
    for (int s = 0; s < dataset.length; s++)
    {
      sdataset.addElement(dataset[s]);
    }
    while (sdataset.size() > 0 && db < dbSources.length)
    {
      int maxqlen = 1; // default number of queries made to at one time
      System.err.println("Verifying against " + dbSources[db]);
      jalview.ws.seqfetcher.DbSourceProxy dbsource = sfetcher
              .getSourceProxy(dbSources[db]);
      if (dbsource == null)
      {
        System.err.println("No proxy for " + dbSources[db]);
        db++;
        continue;
      }
      if (dbsource.getDbSourceProperties()
              .containsKey(DBRefSource.MULTIACC))
      {
        maxqlen = ((Integer) dbsource.getDbSourceProperties().get(
                DBRefSource.MULTIACC)).intValue();
      }
      else
      {
        maxqlen = 1;
      }
      // iterate through db for each remaining un-verified sequence
      SequenceI[] currSeqs = new SequenceI[sdataset.size()];
      sdataset.copyInto(currSeqs);// seqs that are to be validated against
      // dbSources[db]
      Vector queries = new Vector(); // generated queries curSeq
      seqRefs = new Hashtable();

      int seqIndex = 0;

      while (queries.size() > 0 || seqIndex < currSeqs.length)
      {
        if (queries.size() > 0)
        {
          // Still queries to make for current seqIndex
          StringBuffer queryString = new StringBuffer("");
          int nqSize = (maxqlen > queries.size()) ? queries.size()
                  : maxqlen;
          for (int nq = 0, numq = 0; nq < nqSize; nq++)
          {
            String query = (String) queries.elementAt(nq);
            if (dbsource.isValidReference(query))
            {
              queryString.append((nq == 0) ? "" : dbsource
                      .getAccessionSeparator());
              queryString.append(query);
              numq++;
            }
          }
          for (int nq = 0; nq < nqSize; nq++)
          {
            queries.removeElementAt(0);
          }
          // make the queries and process the response
          AlignmentI retrieved = null;
          try
          {
            retrieved = dbsource.getSequenceRecords(queryString.toString());
          } catch (Exception ex)
          {
            ex.printStackTrace();
          } catch (OutOfMemoryError err)
          {
            new OOMWarning("retrieving database references ("
                    + queryString.toString() + ")", err);
          }
          if (retrieved != null)
          {
            transferReferences(sdataset, dbSources[db], retrieved);
          }
        }
        else
        {
          // make some more strings for use as queries
          for (int i = 0; (seqIndex < dataset.length) && (i < 50); seqIndex++, i++)
          {
            SequenceI sequence = dataset[seqIndex];
            DBRefEntry[] uprefs = jalview.util.DBRefUtils.selectRefs(
                    sequence.getDBRef(), new String[]
                    { dbSources[db] }); // jalview.datamodel.DBRefSource.UNIPROT
            // });
            // check for existing dbrefs to use
            if (uprefs != null)
            {
              for (int j = 0; j < uprefs.length; j++)
              {
                addSeqId(sequence, uprefs[j].getAccessionId());
                queries
                        .addElement(uprefs[j].getAccessionId()
                                .toUpperCase());
              }
            }
            else
            {
              // generate queries from sequence ID string
              StringTokenizer st = new StringTokenizer(sequence.getName(),
                      "|");
              while (st.hasMoreTokens())
              {
                String token = st.nextToken();
                addSeqId(sequence, token);
                queries.addElement(token.toUpperCase());
              }
            }
          }
        }
      }
      // advance to next database
      db++;
    } // all databases have been queries.
    if (sbuffer.length() > 0)
    {
      output
              .setText("Your sequences have been verified against known sequence databases. Some of the ids have been\n"
                      + "altered, most likely the start/end residue will have been updated.\n"
                      + "Save your alignment to maintain the updated id.\n\n"
                      + sbuffer.toString());
      Desktop.addInternalFrame(output, "Sequence names updated ", 600, 300);
      // The above is the dataset, we must now find out the index
      // of the viewed sequence

    }

    af.setProgressBar("DBRef search completed", startTime);
    // promptBeforeBlast();

    running = false;

  }

  /**
   * Verify local sequences in seqRefs against the retrieved sequence database
   * records.
   * 
   */
  void transferReferences(Vector sdataset, String dbSource,
          AlignmentI retrievedAl) // File
  // file)
  {

    if (retrievedAl == null || retrievedAl.getHeight() == 0)
    {
      return;
    }
    SequenceI[] retrieved = retrievedAl.getSequencesArray();
    SequenceI sequence = null;

    // Vector entries = new Uniprot().getUniprotEntries(file);

    int i, iSize = retrieved.length; // entries == null ? 0 : entries.size();
    // UniprotEntry entry;
    for (i = 0; i < iSize; i++)
    {
      SequenceI entry = retrieved[i]; // (UniprotEntry) entries.elementAt(i);

      // Work out which sequences this sequence matches,
      // taking into account all accessionIds and names in the file
      Vector sequenceMatches = new Vector();
      // look for corresponding accession ids
      DBRefEntry[] entryRefs = jalview.util.DBRefUtils.selectRefs(entry
              .getDBRef(), new String[]
      { dbSource });
      for (int j = 0; j < entryRefs.length; j++)
      {
        String accessionId = entryRefs[j].getAccessionId(); // .getAccession().elementAt(j).toString();
        // match up on accessionId
        if (seqRefs.containsKey(accessionId.toUpperCase()))
        {
          Vector seqs = (Vector) seqRefs.get(accessionId);
          for (int jj = 0; jj < seqs.size(); jj++)
          {
            sequence = (SequenceI) seqs.elementAt(jj);
            if (!sequenceMatches.contains(sequence))
            {
              sequenceMatches.addElement(sequence);
            }
          }
        }
      }
      if (sequenceMatches.size() == 0)
      {
        // failed to match directly on accessionId==query so just compare all
        // sequences to entry
        Enumeration e = seqRefs.keys();
        while (e.hasMoreElements())
        {
          Vector sqs = (Vector) seqRefs.get(e.nextElement());
          if (sqs != null && sqs.size() > 0)
          {
            Enumeration sqe = sqs.elements();
            while (sqe.hasMoreElements())
            {
              sequenceMatches.addElement(sqe.nextElement());
            }
          }
        }
      }
      // look for corresponding names
      // this is uniprot specific ?
      // could be useful to extend this so we try to find any 'significant'
      // information in common between two sequence objects.
      /*
       * DBRefEntry[] entryRefs =
       * jalview.util.DBRefUtils.selectRefs(entry.getDBRef(), new String[] {
       * dbSource }); for (int j = 0; j < entry.getName().size(); j++) { String
       * name = entry.getName().elementAt(j).toString(); if
       * (seqRefs.containsKey(name)) { Vector seqs = (Vector) seqRefs.get(name);
       * for (int jj = 0; jj < seqs.size(); jj++) { sequence = (SequenceI)
       * seqs.elementAt(jj); if (!sequenceMatches.contains(sequence)) {
       * sequenceMatches.addElement(sequence); } } } }
       */
      // sequenceMatches now contains the set of all sequences associated with
      // the returned db record
      String entrySeq = entry.getSequenceAsString().toUpperCase();
      for (int m = 0; m < sequenceMatches.size(); m++)
      {
        sequence = (SequenceI) sequenceMatches.elementAt(m);
        // only update start and end positions and shift features if there are
        // no existing references
        // TODO: test for legacy where uniprot or EMBL refs exist but no
        // mappings are made (but content matches retrieved set)
        boolean updateRefFrame = sequence.getDBRef() == null
                || sequence.getDBRef().length == 0;
        // verify sequence against the entry sequence

        String nonGapped = AlignSeq.extractGaps("-. ",
                sequence.getSequenceAsString()).toUpperCase();

        int absStart = entrySeq.indexOf(nonGapped);
        int mapStart = entry.getStart();
        jalview.datamodel.Mapping mp;

        if (absStart == -1)
        {
          // Is local sequence contained in dataset sequence?
          absStart = nonGapped.indexOf(entrySeq);
          if (absStart == -1)
          { // verification failed.
            sbuffer.append(sequence.getName()
                    + " SEQUENCE NOT %100 MATCH \n");
            continue;
          }

          sbuffer.append(sequence.getName() + " HAS " + absStart
                  + " PREFIXED RESIDUES COMPARED TO " + dbSource + "\n");
          //
          // + " - ANY SEQUENCE FEATURES"
          // + " HAVE BEEN ADJUSTED ACCORDINGLY \n");
          // absStart = 0;
          // create valid mapping between matching region of local sequence and
          // the mapped sequence
          mp = new Mapping(null, new int[]
          { sequence.getStart() + absStart,
              sequence.getStart() + absStart + entrySeq.length() - 1 },
                  new int[]
                  { entry.getStart(),
                      entry.getStart() + entrySeq.length() - 1 }, 1, 1);
          updateRefFrame = false; // mapping is based on current start/end so
          // don't modify start and end
        }
        else
        {
          // update start and end of local sequence to place it in entry's
          // reference frame.
          // apply identity map map from whole of local sequence to matching
          // region of database
          // sequence
          mp = null; // Mapping.getIdentityMap();
          // new Mapping(null,
          // new int[] { absStart+sequence.getStart(),
          // absStart+sequence.getStart()+entrySeq.length()-1},
          // new int[] { entry.getStart(), entry.getEnd() }, 1, 1);
          // relocate local features for updated start
          if (updateRefFrame && sequence.getSequenceFeatures() != null)
          {
            SequenceFeature[] sf = sequence.getSequenceFeatures();
            int start = sequence.getStart();
            int end = sequence.getEnd();
            int startShift = 1 - absStart - start; // how much the features are
            // to be shifted by
            for (int sfi = 0; sfi < sf.length; sfi++)
            {
              if (sf[sfi].getBegin() >= start && sf[sfi].getEnd() <= end)
              {
                // shift feature along by absstart
                sf[sfi].setBegin(sf[sfi].getBegin() + startShift);
                sf[sfi].setEnd(sf[sfi].getEnd() + startShift);
              }
            }
          }
        }

        System.out.println("Adding dbrefs to " + sequence.getName()
                + " from " + dbSource + " sequence : " + entry.getName());
        sequence.transferAnnotation(entry, mp);
        // unknownSequences.remove(sequence);
        int absEnd = absStart + nonGapped.length();
        absStart += 1;
        if (updateRefFrame)
        {
          // finally, update local sequence reference frame if we're allowed
          sequence.setStart(absStart);
          sequence.setEnd(absEnd);
        }
        // and remove it from the rest
        // TODO: decide if we should remove annotated sequence from set
        sdataset.remove(sequence);
      }
    }
  }
}
