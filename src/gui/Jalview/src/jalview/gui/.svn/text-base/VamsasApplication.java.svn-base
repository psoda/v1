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
package jalview.gui;

import jalview.bin.Cache;
import jalview.datamodel.SequenceI;
import jalview.io.VamsasAppDatastore;
import jalview.structure.StructureSelectionManager;
import jalview.structure.VamsasListener;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;
import java.util.Hashtable;
import java.util.IdentityHashMap;

import javax.swing.JInternalFrame;
import javax.swing.JOptionPane;

import uk.ac.vamsas.client.ClientHandle;
import uk.ac.vamsas.client.IClient;
import uk.ac.vamsas.client.IClientDocument;
import uk.ac.vamsas.client.InvalidSessionDocumentException;
import uk.ac.vamsas.client.NoDefaultSessionException;
import uk.ac.vamsas.client.UserHandle;
import uk.ac.vamsas.client.VorbaId;
import uk.ac.vamsas.client.picking.IMessageHandler;
import uk.ac.vamsas.client.picking.IPickManager;
import uk.ac.vamsas.client.picking.Message;
import uk.ac.vamsas.client.picking.MouseOverMessage;
import uk.ac.vamsas.objects.core.Entry;

/**
 * @author jimp
 * 
 */
public class VamsasApplication
{
  IClient vclient = null;

  ClientHandle app = null;

  UserHandle user = null;

  Desktop jdesktop = null; // our jalview desktop reference

  // Cache.preferences for vamsas client session arena
  // preferences for check for default session at startup.
  // user and organisation stuff.
  public VamsasApplication(Desktop jdesktop, File sessionPath)
  {
    // JBPNote:
    // we should create a session URI from the sessionPath and pass it to
    // the clientFactory - but the vamsas api doesn't cope with that yet.
    this.jdesktop = jdesktop;
    initClientSession(null, sessionPath);
  }

  private static uk.ac.vamsas.client.IClientFactory getClientFactory()
          throws IOException
  {
    return new uk.ac.vamsas.client.simpleclient.SimpleClientFactory();
  }

  /**
   * Start a new vamsas session
   * 
   * @param jdesktop
   */
  public VamsasApplication(Desktop jdesktop)
  {
    this.jdesktop = jdesktop;
    initClientSession(null, null);
  }

  /**
   * init a connection to the session at the given url
   * 
   * @param jdesktop
   * @param sessionUrl
   */
  public VamsasApplication(Desktop jdesktop, String sessionUrl)
  {
    this.jdesktop = jdesktop;
    initClientSession(sessionUrl, null);
  }

  /**
   * @throws IOException
   *                 or other if clientfactory instantiation failed.
   * @return list of current sessions or null if no session exists.
   */
  public static String[] getSessionList() throws Exception
  {
    return getClientFactory().getCurrentSessions();
  }

  /**
   * initialise, possibly with either a valid session url or a file for a new
   * session
   * 
   * @param sess
   *                null or a valid session url
   * @param vamsasDocument
   *                null or a valid vamsas document file
   * @return false if no vamsas connection was made
   */
  private boolean initClientSession(String sess, File vamsasDocument)
  {
    try
    {
      // Only need to tell the library what the application is here
      app = getJalviewHandle();
      uk.ac.vamsas.client.IClientFactory clientfactory = getClientFactory();
      if (vamsasDocument != null)
      {
        if (sess != null)
        {
          throw new Error(
                  "Implementation Error - cannot import existing vamsas document into an existing session, Yet!");
        }
        try
        {
          vclient = clientfactory.openAsNewSessionIClient(app,
                  vamsasDocument);
        } catch (InvalidSessionDocumentException e)
        {
          JOptionPane
                  .showInternalMessageDialog(
                          Desktop.desktop,

                          "VAMSAS Document could not be opened as a new session - please choose another",
                          "VAMSAS Document Import Failed",
                          JOptionPane.ERROR_MESSAGE);

        }
      }
      else
      {
        // join existing or create a new session
        if (sess == null)
        {
          vclient = clientfactory.getNewSessionIClient(app);
        }
        else
        {
          vclient = clientfactory.getIClient(app, sess);
        }
      }
      // set some properties for our VAMSAS interaction
      setVclientConfig();
      user = vclient.getUserHandle();

    } catch (Exception e)
    {
      jalview.bin.Cache.log
              .error("Couldn't instantiate vamsas client !", e);
      return false;
    }
    return true;
  }

  private void setVclientConfig()
  {
    if (vclient == null)
    {
      return;
    }
    try
    {
      if (vclient instanceof uk.ac.vamsas.client.simpleclient.SimpleClient)
      {
        uk.ac.vamsas.client.simpleclient.SimpleClientConfig cfg = ((uk.ac.vamsas.client.simpleclient.SimpleClient) vclient)
                .getSimpleClientConfig();
        cfg._validatemergedroots = false;
        cfg._validateupdatedroots = true; // we may write rubbish otherwise.
      }
    } catch (Error e)
    {
      Cache.log
              .warn(
                      "Probable SERIOUS VAMSAS client incompatibility - carrying on regardless",
                      e);
    } catch (Exception e)
    {
      Cache.log
              .warn(
                      "Probable VAMSAS client incompatibility - carrying on regardless",
                      e);
    }
  }

  /**
   * make the appHandle for Jalview
   * 
   * @return
   */
  private ClientHandle getJalviewHandle()
  {
    return new ClientHandle("jalview.bin.Jalview", jalview.bin.Cache
            .getProperty("VERSION"));
  }

  /**
   * 
   * @return true if we are registered in a vamsas session
   */
  public boolean inSession()
  {
    return (vclient != null);
  }

  /**
   * called to connect to session inits handlers, does an initial document
   * update.
   */
  public void initial_update()
  {
    if (!inSession())
    {
      throw new Error(
              "Impementation error! Vamsas Operations when client not initialised and connected.");
    }
    addDocumentUpdateHandler();
    addStoreDocumentHandler();
    startSession();
    Cache.log
            .debug("Jalview loading the Vamsas Session for the first time.");
    dealWithDocumentUpdate(false); // we don't push an update out to the
    // document yet.
    Cache.log.debug("... finished update for the first time.");
  }

  /**
   * Update all windows after a vamsas datamodel change. this could go on the
   * desktop object!
   * 
   */
  protected void updateJalviewGui()
  {
    JInternalFrame[] frames = jdesktop.getAllFrames();

    if (frames == null)
    {
      return;
    }

    try
    {
      // REVERSE ORDER
      for (int i = frames.length - 1; i > -1; i--)
      {
        if (frames[i] instanceof AlignFrame)
        {
          AlignFrame af = (AlignFrame) frames[i];
          af.alignPanel.alignmentChanged();
        }
      }
    } catch (Exception e)
    {
      Cache.log
              .warn(
                      "Exception whilst refreshing jalview windows after a vamsas document update.",
                      e);
    }
  }

  public void push_update()
  {
    Thread udthread = new Thread(new Runnable()
    {

      public void run()
      {
        Cache.log.info("Jalview updating to the Vamsas Session.");

        dealWithDocumentUpdate(true);
        /*
         * IClientDocument cdoc=null; try { cdoc = vclient.getClientDocument(); }
         * catch (Exception e) { Cache.log.error("Failed to get client document
         * for update."); // RAISE A WARNING DIALOG disableGui(false); return; }
         * updateVamsasDocument(cdoc); updateJalviewGui();
         * cdoc.setVamsasRoots(cdoc.getVamsasRoots()); // propagate update flags
         * back vclient.updateDocument(cdoc);
         */
        Cache.log.info("Jalview finished updating to the Vamsas Session.");
        // TODO Auto-generated method stub
      }

    });
    udthread.start();
  }

  public void end_session()
  {
    if (!inSession())
      throw new Error("Jalview not connected to Vamsas session.");
    Cache.log.info("Jalview disconnecting from the Vamsas Session.");
    try
    {
      if (joinedSession)
      {
        vclient.finalizeClient();
        Cache.log.info("Jalview has left the session.");
      }
      else
      {
        Cache.log
                .warn("JV Client leaving a session that's its not joined yet.");
      }
      joinedSession = false;
      vclient = null;
      app = null;
      user = null;
      jv2vobj = null;
      vobj2jv = null;
    } catch (Exception e)
    {
      Cache.log.error("Vamsas Session finalization threw exceptions!", e);
    }
  }

  public void updateJalview(IClientDocument cdoc)
  {
    Cache.log.debug("Jalview updating from sesion document ..");
    ensureJvVamsas();
    VamsasAppDatastore vds = new VamsasAppDatastore(cdoc, vobj2jv, jv2vobj,
            baseProvEntry(), alRedoState);
    vds.updateToJalview();
    Cache.log.debug(".. finished updating from sesion document.");

  }

  private void ensureJvVamsas()
  {
    if (jv2vobj == null)
    {
      jv2vobj = new IdentityHashMap();
      vobj2jv = new Hashtable();
      alRedoState = new Hashtable();
    }
  }

  /**
   * jalview object binding to VorbaIds
   */
  IdentityHashMap jv2vobj = null;

  Hashtable vobj2jv = null;

  Hashtable alRedoState = null;

  public void updateVamsasDocument(IClientDocument doc)
  {
    ensureJvVamsas();
    VamsasAppDatastore vds = new VamsasAppDatastore(doc, vobj2jv, jv2vobj,
            baseProvEntry(), alRedoState);
    // wander through frames
    JInternalFrame[] frames = Desktop.desktop.getAllFrames();

    if (frames == null)
    {
      return;
    }

    try
    {
      // REVERSE ORDER
      for (int i = frames.length - 1; i > -1; i--)
      {
        if (frames[i] instanceof AlignFrame)
        {
          AlignFrame af = (AlignFrame) frames[i];

          // update alignment and root from frame.
          vds.storeVAMSAS(af.getViewport(), af.getTitle());
        }
      }
      // REVERSE ORDER
      for (int i = frames.length - 1; i > -1; i--)
      {
        if (frames[i] instanceof AlignFrame)
        {
          AlignFrame af = (AlignFrame) frames[i];

          // add any AlignedCodonFrame mappings on this alignment to any other.
          vds.storeSequenceMappings(af.getViewport(), af.getTitle());
        }
      }
    } catch (Exception e)
    {
      Cache.log.error("Vamsas Document store exception", e);
    }
  }

  private Entry baseProvEntry()
  {
    uk.ac.vamsas.objects.core.Entry pentry = new uk.ac.vamsas.objects.core.Entry();
    pentry.setUser(user.getFullName());
    pentry.setApp(app.getClientUrn());
    pentry.setDate(new java.util.Date());
    pentry.setAction("created");
    return pentry;
  }

  /**
   * do a vamsas document update or update jalview from the vamsas document
   * 
   * @param fromJalview
   *                true to update from jalview to the vamsas document
   */
  protected void dealWithDocumentUpdate(boolean fromJalview)
  {
    // called by update handler for document update.
    Cache.log.debug("Updating jalview from changed vamsas document.");
    disableGui(true);
    try
    {
      long time = System.currentTimeMillis();
      IClientDocument cdoc = vclient.getClientDocument();
      if (Cache.log.isDebugEnabled())
      {
        Cache.log.debug("Time taken to get ClientDocument = "
                + (System.currentTimeMillis() - time));
        time = System.currentTimeMillis();
      }
      if (fromJalview)
      {
        this.updateVamsasDocument(cdoc);
        if (Cache.log.isDebugEnabled())
        {
          Cache.log
                  .debug("Time taken to update Vamsas Document from jalview\t= "
                          + (System.currentTimeMillis() - time));
          time = System.currentTimeMillis();
        }
        cdoc.setVamsasRoots(cdoc.getVamsasRoots());
        if (Cache.log.isDebugEnabled())
        {
          Cache.log.debug("Time taken to set Document Roots\t\t= "
                  + (System.currentTimeMillis() - time));
          time = System.currentTimeMillis();
        }
      }
      else
      {
        updateJalview(cdoc);
        if (Cache.log.isDebugEnabled())
        {
          Cache.log
                  .debug("Time taken to update Jalview from vamsas document Roots\t= "
                          + (System.currentTimeMillis() - time));
          time = System.currentTimeMillis();
        }

      }
      vclient.updateDocument(cdoc);
      if (Cache.log.isDebugEnabled())
      {
        Cache.log.debug("Time taken to update Session Document\t= "
                + (System.currentTimeMillis() - time));
        time = System.currentTimeMillis();
      }
      cdoc = null;
    } catch (Exception ee)
    {
      System.err.println("Exception whilst updating :");
      ee.printStackTrace(System.err);
    }
    Cache.log.debug("Finished updating from document change.");
    disableGui(false);
  }

  private void addDocumentUpdateHandler()
  {
    final VamsasApplication client = this;
    vclient.addDocumentUpdateHandler(new PropertyChangeListener()
    {
      public void propertyChange(PropertyChangeEvent evt)
      {
        Cache.log.debug("Dealing with document update event.");
        client.dealWithDocumentUpdate(false);
        Cache.log.debug("finished dealing with event.");
      }
    });
    Cache.log.debug("Added Jalview handler for vamsas document updates.");
  }

  private void addStoreDocumentHandler()
  {
    final VamsasApplication client = this;
    vclient.addVorbaEventHandler(
            uk.ac.vamsas.client.Events.DOCUMENT_REQUESTTOCLOSE,
            new PropertyChangeListener()
            {
              public void propertyChange(PropertyChangeEvent evt)
              {
                Cache.log
                        .debug("Asking user if the vamsas session should be stored.");
                int reply = JOptionPane
                        .showInternalConfirmDialog(
                                Desktop.desktop,
                                "The current VAMSAS session has unsaved data - do you want to save it ?",
                                "VAMSAS Session Shutdown",
                                JOptionPane.YES_NO_OPTION,
                                JOptionPane.QUESTION_MESSAGE);

                if (reply == JOptionPane.YES_OPTION)
                {
                  Cache.log.debug("Prompting for vamsas store filename.");
                  Desktop.instance.vamsasSave_actionPerformed(null);
                  Cache.log.debug("Finished attempt at storing document.");
                }
                Cache.log
                        .debug("finished dealing with REQUESTTOCLOSE event.");
              }
            });
    Cache.log.debug("Added Jalview handler for vamsas document updates.");
  }

  public void disableGui(boolean b)
  {
    Desktop.instance.setVamsasUpdate(b);
  }

  private boolean joinedSession = false;

  private VamsasListener picker = null;

  private void startSession()
  {
    if (inSession())
    {
      try
      {
        vclient.joinSession();
        joinedSession = true;
      } catch (Exception e)
      {
        // Complain to GUI
        Cache.log.error("Failed to join vamsas session.", e);
        vclient = null;
      }
      try
      {
        final IPickManager pm = vclient.getPickManager();
        final StructureSelectionManager ssm = StructureSelectionManager
                .getStructureSelectionManager();
        pm.registerMessageHandler(new IMessageHandler()
        {
          String last = null;

          public void handleMessage(Message message)
          {
            if (message instanceof MouseOverMessage && vobj2jv != null)
            {
              MouseOverMessage mm = (MouseOverMessage) message;
              String mstring = mm.getVorbaID() + " " + mm.getPosition();
              if (last != null && mstring.equals(last))
              {
                return;
              }
              // if (Cache.log.isDebugEnabled())
              // {
              // Cache.log.debug("Received MouseOverMessage "+mm.getVorbaID()+"
              // "+mm.getPosition());
              // }
              Object jvobj = vobj2jv.get(mm.getVorbaID());
              if (jvobj != null && jvobj instanceof SequenceI)
              {
                last = mstring;
                // Cache.log.debug("Handling Mouse over "+mm.getVorbaID()+"
                // bound to "+jvobj+" at "+mm.getPosition());
                // position is character position in aligned sequence
                ssm.mouseOverVamsasSequence((SequenceI) jvobj, mm
                        .getPosition());
              }
            }
          }
        });
        picker = new VamsasListener()
        {
          SequenceI last = null;

          int i = -1;

          public void mouseOver(SequenceI seq, int index)
          {
            if (jv2vobj == null)
              return;
            if (seq != last || i != index)
            {
              VorbaId v = (VorbaId) jv2vobj.get(seq);
              if (v != null)
              {
                Cache.log.debug("Mouse over " + v.getId() + " bound to "
                        + seq + " at " + index);
                last = seq;
                i = index;
                MouseOverMessage message = new MouseOverMessage(v.getId(),
                        index);
                pm.sendMessage(message);
              }
            }
          }
        };
        ssm.addStructureViewerListener(picker); // better method here
      } catch (Exception e)
      {
        Cache.log.error("Failed to init Vamsas Picking", e);
      }
    }
  }
}
