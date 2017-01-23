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
package jalview.bin;

import java.applet.*;

import java.awt.*;
import java.awt.event.*;
import java.util.*;

import jalview.appletgui.*;
import jalview.datamodel.*;
import jalview.io.*;

/**
 * Jalview Applet. Runs in Java 1.18 runtime
 * 
 * @author $author$
 * @version $Revision: 1.62.2.3 $
 */
public class JalviewLite extends Applet
{

  // /////////////////////////////////////////
  // The following public methods maybe called
  // externally, eg via javascript in HTML page
  /**
   * @return String list of selected sequence IDs, each terminated by "¬"
   *         (&#172;)
   */
  public String getSelectedSequences()
  {
    return getSelectedSequencesFrom(getDefaultTargetFrame());
  }

  /**
   * @param sep
   *                separator string or null for default
   * @return String list of selected sequence IDs, each terminated by sep or
   *         ("¬" as default)
   */
  public String getSelectedSequences(String sep)
  {
    return getSelectedSequencesFrom(getDefaultTargetFrame(), sep);
  }

  /**
   * @param alf
   *                alignframe containing selection
   * @return String list of selected sequence IDs, each terminated by "¬"
   * 
   */
  public String getSelectedSequencesFrom(AlignFrame alf)
  {
    return getSelectedSequencesFrom(alf, "¬");
  }

  /**
   * get list of selected sequence IDs separated by given separator
   * 
   * @param alf
   *                window containing selection
   * @param sep
   *                separator string to use - default is "¬"
   * @return String list of selected sequence IDs, each terminated by the given
   *         separator
   */
  public String getSelectedSequencesFrom(AlignFrame alf, String sep)
  {
    StringBuffer result = new StringBuffer("");
    if (sep == null || sep.length() == 0)
    {
      sep = "¬";
    }
    if (alf.viewport.getSelectionGroup() != null)
    {
      SequenceI[] seqs = alf.viewport.getSelectionGroup()
              .getSequencesInOrder(alf.viewport.getAlignment());

      for (int i = 0; i < seqs.length; i++)
      {
        result.append(seqs[i].getName());
        result.append(sep);
      }
    }

    return result.toString();
  }

  /**
   * get sequences selected in current alignFrame and return their alignment in
   * format 'format' either with or without suffix
   * 
   * @param alf -
   *                where selection is
   * @param format -
   *                format of alignment file
   * @param suffix -
   *                "true" to append /start-end string to each sequence ID
   * @return selected sequences as flat file or empty string if there was no
   *         current selection
   */
  public String getSelectedSequencesAsAlignment(String format, String suffix)
  {
    return getSelectedSequencesAsAlignmentFrom(currentAlignFrame, format,
            suffix);
  }

  /**
   * get sequences selected in alf and return their alignment in format 'format'
   * either with or without suffix
   * 
   * @param alf -
   *                where selection is
   * @param format -
   *                format of alignment file
   * @param suffix -
   *                "true" to append /start-end string to each sequence ID
   * @return selected sequences as flat file or empty string if there was no
   *         current selection
   */
  public String getSelectedSequencesAsAlignmentFrom(AlignFrame alf,
          String format, String suffix)
  {
    try
    {
      boolean seqlimits = suffix.equalsIgnoreCase("true");
      if (alf.viewport.getSelectionGroup() != null)
      {
        String reply = new AppletFormatAdapter().formatSequences(format,
                new Alignment(alf.viewport.getSelectionAsNewSequence()),
                seqlimits);
        return reply;
      }
    } catch (Exception ex)
    {
      ex.printStackTrace();
      return "Error retrieving alignment in " + format + " format. ";
    }
    return "";
  }

  public String getAlignment(String format)
  {
    return getAlignmentFrom(getDefaultTargetFrame(), format, "true");
  }

  public String getAlignmentFrom(AlignFrame alf, String format)
  {
    return getAlignmentFrom(alf, format, "true");
  }

  public String getAlignment(String format, String suffix)
  {
    return getAlignmentFrom(getDefaultTargetFrame(), format, suffix);
  }

  public String getAlignmentFrom(AlignFrame alf, String format,
          String suffix)
  {
    try
    {
      boolean seqlimits = suffix.equalsIgnoreCase("true");

      String reply = new AppletFormatAdapter().formatSequences(format,
              alf.viewport.getAlignment(), seqlimits);
      return reply;
    } catch (Exception ex)
    {
      ex.printStackTrace();
      return "Error retrieving alignment in " + format + " format. ";
    }
  }

  public void loadAnnotation(String annotation)
  {
    loadAnnotationFrom(getDefaultTargetFrame(), annotation);
  }

  public void loadAnnotationFrom(AlignFrame alf, String annotation)
  {
    if (new AnnotationFile().readAnnotationFile(alf.getAlignViewport()
            .getAlignment(), annotation, AppletFormatAdapter.PASTE))
    {
      alf.alignPanel.fontChanged();
      alf.alignPanel.setScrollValues(0, 0);
    }
    else
    {
      alf.parseFeaturesFile(annotation, AppletFormatAdapter.PASTE);
    }
  }

  public String getFeatures(String format)
  {
    return getFeaturesFrom(getDefaultTargetFrame(), format);
  }

  public String getFeaturesFrom(AlignFrame alf, String format)
  {
    return alf.outputFeatures(false, format);
  }

  public String getAnnotation()
  {
    return getAnnotationFrom(getDefaultTargetFrame());
  }

  public String getAnnotationFrom(AlignFrame alf)
  {
    return alf.outputAnnotations(false);
  }

  public AlignFrame newView()
  {
    return newViewFrom(getDefaultTargetFrame());
  }

  public AlignFrame newView(String name)
  {
    return newViewFrom(getDefaultTargetFrame(), name);
  }

  public AlignFrame newViewFrom(AlignFrame alf)
  {
    return alf.newView(null);
  }

  public AlignFrame newViewFrom(AlignFrame alf, String name)
  {
    return alf.newView(name);
  }

  /**
   * 
   * @param text
   *                alignment file as a string
   * @param title
   *                window title
   * @return null or new alignment frame
   */
  public AlignFrame loadAlignment(String text, String title)
  {
    Alignment al = null;
    String format = new IdentifyFile().Identify(text,
            AppletFormatAdapter.PASTE);
    try
    {
      al = new AppletFormatAdapter().readFile(text,
              AppletFormatAdapter.PASTE, format);
      if (al.getHeight() > 0)
      {
        return new AlignFrame(al, this, title, false);
      }
    } catch (java.io.IOException ex)
    {
      ex.printStackTrace();
    }
    return null;
  }

  // //////////////////////////////////////////////
  // //////////////////////////////////////////////

  static int lastFrameX = 200;

  static int lastFrameY = 200;

  boolean fileFound = true;

  String file = "No file";

  Button launcher = new Button("Start Jalview");

  /**
   * The currentAlignFrame is static, it will change if and when the user
   * selects a new window. Note that it will *never* point back to the embedded
   * AlignFrame if the applet is started as embedded on the page and then
   * afterwards a new view is created.
   */
  public static AlignFrame currentAlignFrame;

  /**
   * This is the first frame to be displayed, and does not change. API calls
   * will default to this instance if currentAlignFrame is null.
   */
  AlignFrame initialAlignFrame;

  boolean embedded = false;

  private boolean checkForJmol = true;

  public boolean jmolAvailable = false;

  public static boolean debug;

  /**
   * init method for Jalview Applet
   */
  public void init()
  {
    /**
     * turn on extra applet debugging
     */
    String dbg = getParameter("debug");
    if (dbg != null)
    {
      debug = dbg.toLowerCase().equals("true");
    }
    /**
     * if true disable the check for jmol
     */
    String chkforJmol = getParameter("nojmol");
    if (chkforJmol != null)
    {
      checkForJmol = !chkforJmol.equals("true");
    }
    /**
     * get the separator parameter if present
     */
    String sep = getParameter("separator");
    if (sep != null)
    {
      if (sep.length() > 0)
      {
        separator = sep;
        if (debug)
        {
          System.err.println("Separator set to '" + separator + "'");
        }
      }
      else
      {
        throw new Error(
                "Invalid separator parameter - must be non-zero length");
      }
    }
    int r = 255;
    int g = 255;
    int b = 255;
    String param = getParameter("RGB");

    if (param != null)
    {
      try
      {
        r = Integer.parseInt(param.substring(0, 2), 16);
        g = Integer.parseInt(param.substring(2, 4), 16);
        b = Integer.parseInt(param.substring(4, 6), 16);
      } catch (Exception ex)
      {
        r = 255;
        g = 255;
        b = 255;
      }
    }

    param = getParameter("label");
    if (param != null)
    {
      launcher.setLabel(param);
    }

    this.setBackground(new Color(r, g, b));

    file = getParameter("file");

    if (file == null)
    {
      // Maybe the sequences are added as parameters
      StringBuffer data = new StringBuffer("PASTE");
      int i = 1;
      while ((file = getParameter("sequence" + i)) != null)
      {
        data.append(file.toString() + "\n");
        i++;
      }
      if (data.length() > 5)
      {
        file = data.toString();
      }
    }

    LoadJmolThread jmolAvailable = new LoadJmolThread();
    jmolAvailable.start();

    final JalviewLite applet = this;
    if (getParameter("embedded") != null
            && getParameter("embedded").equalsIgnoreCase("true"))
    {
      // Launch as embedded applet in page
      embedded = true;
      LoadingThread loader = new LoadingThread(file, applet);
      loader.start();
    }
    else if (file != null)
    {
      if (getParameter("showbutton") == null
              || !getParameter("showbutton").equalsIgnoreCase("false"))
      {
        // Add the JalviewLite 'Button' to the page
        add(launcher);
        launcher.addActionListener(new java.awt.event.ActionListener()
        {
          public void actionPerformed(ActionEvent e)
          {
            LoadingThread loader = new LoadingThread(file, applet);
            loader.start();
          }
        });
      }
      else
      {
        // Open jalviewLite immediately.
        LoadingThread loader = new LoadingThread(file, applet);
        loader.start();
      }
    }
    else
    {
      // jalview initialisation with no alignment. loadAlignment() method can
      // still be called to open new alignments.
      file = "NO FILE";
      fileFound = false;
    }
  }

  /**
   * Initialises and displays a new java.awt.Frame
   * 
   * @param frame
   *                java.awt.Frame to be displayed
   * @param title
   *                title of new frame
   * @param width
   *                width if new frame
   * @param height
   *                height of new frame
   */
  public static void addFrame(final Frame frame, String title, int width,
          int height)
  {
    frame.setLocation(lastFrameX, lastFrameY);
    lastFrameX += 40;
    lastFrameY += 40;
    frame.setSize(width, height);
    frame.setTitle(title);
    frame.addWindowListener(new WindowAdapter()
    {
      public void windowClosing(WindowEvent e)
      {
        if (frame instanceof AlignFrame)
        {
          ((AlignFrame) frame).closeMenuItem_actionPerformed();
        }
        if (currentAlignFrame == frame)
        {
          currentAlignFrame = null;
        }
        lastFrameX -= 40;
        lastFrameY -= 40;
        if (frame instanceof EmbmenuFrame)
        {
          ((EmbmenuFrame) frame).destroyMenus();
        }
        frame.setMenuBar(null);
        frame.dispose();
      }

      public void windowActivated(WindowEvent e)
      {
        if (frame instanceof AlignFrame)
        {
          currentAlignFrame = (AlignFrame) frame;
          if (debug)
          {
            System.err.println("Activated window " + frame);
          }
        }
        // be good.
        super.windowActivated(e);
      }
      /*
       * Probably not necessary to do this - see TODO above. (non-Javadoc)
       * 
       * @see java.awt.event.WindowAdapter#windowDeactivated(java.awt.event.WindowEvent)
       * 
       * public void windowDeactivated(WindowEvent e) { if (currentAlignFrame ==
       * frame) { currentAlignFrame = null; if (debug) {
       * System.err.println("Deactivated window "+frame); } }
       * super.windowDeactivated(e); }
       */
    });
    frame.setVisible(true);
  }

  /**
   * This paints the background surrounding the "Launch Jalview button" <br>
   * <br>
   * If file given in parameter not found, displays error message
   * 
   * @param g
   *                graphics context
   */
  public void paint(Graphics g)
  {
    if (!fileFound)
    {
      g.setColor(new Color(200, 200, 200));
      g.setColor(Color.cyan);
      g.fillRect(0, 0, getSize().width, getSize().height);
      g.setColor(Color.red);
      g.drawString("Jalview can't open file", 5, 15);
      g.drawString("\"" + file + "\"", 5, 30);
    }
    else if (embedded)
    {
      g.setColor(Color.black);
      g.setFont(new Font("Arial", Font.BOLD, 24));
      g.drawString("Jalview Applet", 50, this.getSize().height / 2 - 30);
      g.drawString("Loading Data...", 50, this.getSize().height / 2);
    }
  }

  class LoadJmolThread extends Thread
  {
    public void run()
    {
      if (checkForJmol)
      {
        try
        {
          if (!System.getProperty("java.version").startsWith("1.1"))
          {
            Class.forName("org.jmol.adapter.smarter.SmarterJmolAdapter");
            jmolAvailable = true;
          }
          if (!jmolAvailable)
          {
            System.out
                    .println("Jmol not available - Using MCview for structures");
          }
        } catch (java.lang.ClassNotFoundException ex)
        {
        }
      }
      else
      {
        jmolAvailable = false;
        if (debug)
        {
          System.err
                  .println("Skipping Jmol check. Will use MCView (probably)");
        }
      }
    }
  }

  class LoadingThread extends Thread
  {
    /**
     * State variable: File source
     */
    String file;

    /**
     * State variable: protocol for access to file source
     */
    String protocol;

    /**
     * State variable: format of file source
     */
    String format;

    JalviewLite applet;

    private void dbgMsg(String msg)
    {
      if (applet.debug)
      {
        System.err.println(msg);
      }
    }

    /**
     * update the protocol state variable for accessing the datasource located
     * by file.
     * 
     * @param file
     * @return possibly updated datasource string
     */
    public String setProtocolState(String file)
    {
      if (file.startsWith("PASTE"))
      {
        file = file.substring(5);
        protocol = AppletFormatAdapter.PASTE;
      }
      else if (inArchive(file))
      {
        protocol = AppletFormatAdapter.CLASSLOADER;
      }
      else
      {
        file = addProtocol(file);
        protocol = AppletFormatAdapter.URL;
      }
      dbgMsg("Protocol identified as '" + protocol + "'");
      return file;
    }

    public LoadingThread(String _file, JalviewLite _applet)
    {
      dbgMsg("Loading thread started with:\n>>file\n" + _file + ">>endfile");
      file = setProtocolState(_file);

      format = new jalview.io.IdentifyFile().Identify(file, protocol);
      dbgMsg("File identified as '" + format + "'");
      applet = _applet;
    }

    public void run()
    {
      startLoading();
    }

    private void startLoading()
    {
      dbgMsg("Loading started.");
      Alignment al = null;
      try
      {
        al = new AppletFormatAdapter().readFile(file, protocol, format);
      } catch (java.io.IOException ex)
      {
        dbgMsg("File load exception.");
        ex.printStackTrace();
      }
      if ((al != null) && (al.getHeight() > 0))
      {
        dbgMsg("Successfully loaded file.");
        initialAlignFrame = new AlignFrame(al, applet, file, embedded);
        // update the focus.
        currentAlignFrame = initialAlignFrame;

        if (protocol == jalview.io.AppletFormatAdapter.PASTE)
        {
          currentAlignFrame.setTitle("Sequences from " + getDocumentBase());
        }

        currentAlignFrame.statusBar.setText("Successfully loaded file "
                + file);

        String treeFile = applet.getParameter("tree");
        if (treeFile == null)
        {
          treeFile = applet.getParameter("treeFile");
        }

        if (treeFile != null)
        {
          try
          {
            treeFile = setProtocolState(treeFile);
            /*
             * if (inArchive(treeFile)) { protocol =
             * AppletFormatAdapter.CLASSLOADER; } else { protocol =
             * AppletFormatAdapter.URL; treeFile = addProtocol(treeFile); }
             */
            jalview.io.NewickFile fin = new jalview.io.NewickFile(treeFile,
                    protocol);

            fin.parse();

            if (fin.getTree() != null)
            {
              currentAlignFrame.loadTree(fin, treeFile);
              dbgMsg("Successfuly imported tree.");
            }
            else
            {
              dbgMsg("Tree parameter did not resolve to a valid tree.");
            }
          } catch (Exception ex)
          {
            ex.printStackTrace();
          }
        }

        String param = getParameter("features");
        if (param != null)
        {
          param = setProtocolState(param);

          currentAlignFrame.parseFeaturesFile(param, protocol);
        }

        param = getParameter("showFeatureSettings");
        if (param != null && param.equalsIgnoreCase("true"))
        {
          currentAlignFrame.viewport.showSequenceFeatures(true);
          new FeatureSettings(currentAlignFrame.alignPanel);
        }

        param = getParameter("annotations");
        if (param != null)
        {
          param = setProtocolState(param);

          if (new AnnotationFile().readAnnotationFile(
                  currentAlignFrame.viewport.getAlignment(), param,
                  protocol))
          {
            currentAlignFrame.alignPanel.fontChanged();
            currentAlignFrame.alignPanel.setScrollValues(0, 0);
          }
          else
          {
            System.err
                    .println("Annotations were not added from annotation file '"
                            + param + "'");
          }

        }

        param = getParameter("jnetfile");
        if (param != null)
        {
          try
          {
            param = setProtocolState(param);
            jalview.io.JPredFile predictions = new jalview.io.JPredFile(
                    param, protocol);
            JnetAnnotationMaker.add_annotation(predictions,
                    currentAlignFrame.viewport.getAlignment(), 0, false); // false==do
            // not
            // add
            // sequence
            // profile
            // from
            // concise
            // output
            currentAlignFrame.alignPanel.fontChanged();
            currentAlignFrame.alignPanel.setScrollValues(0, 0);
          } catch (Exception ex)
          {
            ex.printStackTrace();
          }
        }

        /*
         * <param name="PDBfile" value="1gaq.txt PDB|1GAQ|1GAQ|A PDB|1GAQ|1GAQ|B
         * PDB|1GAQ|1GAQ|C">
         * 
         * <param name="PDBfile2" value="1gaq.txt A=SEQA B=SEQB C=SEQB">
         * 
         * <param name="PDBfile3" value="1q0o Q45135_9MICO">
         */

        int pdbFileCount = 0;
        do
        {
          if (pdbFileCount > 0)
            param = getParameter("PDBFILE" + pdbFileCount);
          else
            param = getParameter("PDBFILE");

          if (param != null)
          {
            PDBEntry pdb = new PDBEntry();

            String seqstring;
            SequenceI[] seqs = null;
            String[] chains = null;

            StringTokenizer st = new StringTokenizer(param, " ");

            if (st.countTokens() < 2)
            {
              String sequence = applet.getParameter("PDBSEQ");
              if (sequence != null)
                seqs = new SequenceI[]
                { (Sequence) currentAlignFrame.getAlignViewport()
                        .getAlignment().findName(sequence) };

            }
            else
            {
              param = st.nextToken();
              Vector tmp = new Vector();
              Vector tmp2 = new Vector();

              while (st.hasMoreTokens())
              {
                seqstring = st.nextToken();
                StringTokenizer st2 = new StringTokenizer(seqstring, "=");
                if (st2.countTokens() > 1)
                {
                  // This is the chain
                  tmp2.addElement(st2.nextToken());
                  seqstring = st2.nextToken();
                }
                tmp.addElement((Sequence) currentAlignFrame
                        .getAlignViewport().getAlignment().findName(
                                seqstring));
              }

              seqs = new SequenceI[tmp.size()];
              tmp.copyInto(seqs);
              if (tmp2.size() == tmp.size())
              {
                chains = new String[tmp2.size()];
                tmp2.copyInto(chains);
              }
            }
            param = setProtocolState(param);

            if (// !jmolAvailable
            // &&
            protocol == AppletFormatAdapter.CLASSLOADER)
            {
              // TODO: verify this Re:
              // https://mantis.lifesci.dundee.ac.uk/view.php?id=36605
              // This exception preserves the current behaviour where, even if
              // the local pdb file was identified in the class loader
              protocol = AppletFormatAdapter.URL; // this is probably NOT
              // CORRECT!
              param = addProtocol(param); // 
            }

            pdb.setFile(param);

            if (seqs != null)
            {
              for (int i = 0; i < seqs.length; i++)
              {
                if (seqs[i] != null)
                {
                  ((Sequence) seqs[i]).addPDBId(pdb);
                }
                else
                {
                  if (JalviewLite.debug)
                  {
                    // this may not really be a problem but we give a warning
                    // anyway
                    System.err
                            .println("Warning: Possible input parsing error: Null sequence for attachment of PDB (sequence "
                                    + i + ")");
                  }
                }
              }

              if (jmolAvailable)
              {
                new jalview.appletgui.AppletJmol(pdb, seqs, chains,
                        currentAlignFrame.alignPanel, protocol);
                lastFrameX += 40;
                lastFrameY += 40;
              }
              else
                new MCview.AppletPDBViewer(pdb, seqs, chains,
                        currentAlignFrame.alignPanel, protocol);
            }
          }

          pdbFileCount++;
        } while (pdbFileCount < 10);

        // ///////////////////////////
        // modify display of features
        //
        // hide specific groups
        param = getParameter("hidefeaturegroups");
        if (param != null)
        {
          applet.setFeatureGroupState(param, false);
        }
        // show specific groups
        param = getParameter("showfeaturegroups");
        if (param != null)
        {
          applet.setFeatureGroupState(param, true);
        }
      }
      else
      {
        fileFound = false;
        remove(launcher);
        repaint();
      }
    }

    /**
     * Discovers whether the given file is in the Applet Archive
     * 
     * @param file
     *                String
     * @return boolean
     */
    boolean inArchive(String file)
    {
      // This might throw a security exception in certain browsers
      // Netscape Communicator for instance.
      try
      {
        boolean rtn = (getClass().getResourceAsStream("/" + file) != null);
        if (debug)
        {
          System.err.println("Resource '" + file + "' was "
                  + (rtn ? "" : "not") + " located by classloader.");
        }
        return rtn;
      } catch (Exception ex)
      {
        System.out.println("Exception checking resources: " + file + " "
                + ex);
        return false;
      }
    }

    String addProtocol(String file)
    {
      if (file.indexOf("://") == -1)
      {
        file = getCodeBase() + file;
        if (debug)
        {
          System.err.println("Prepended codebase for resource: '" + file
                  + "'");
        }
      }

      return file;
    }
  }

  /**
   * @return the default alignFrame acted on by the public applet methods. May
   *         return null with an error message on System.err indicating the
   *         fact.
   */
  protected AlignFrame getDefaultTargetFrame()
  {
    if (currentAlignFrame != null)
    {
      return currentAlignFrame;
    }
    if (initialAlignFrame != null)
    {
      return initialAlignFrame;
    }
    System.err
            .println("Implementation error: Jalview Applet API cannot work out which AlignFrame to use.");
    return null;
  }

  /**
   * separator used for separatorList
   */
  protected String separator = "|"; // this is a safe(ish) separator - tabs

  // don't work for firefox

  /**
   * parse the string into a list
   * 
   * @param list
   * @return elements separated by separator
   */
  public String[] separatorListToArray(String list)
  {
    int seplen = separator.length();
    if (list == null || list.equals(""))
      return null;
    java.util.Vector jv = new Vector();
    int cp = 0, pos;
    while ((pos = list.indexOf(separator, cp)) > cp)
    {
      jv.addElement(list.substring(cp, pos));
      cp = pos + seplen;
    }
    if (cp < list.length())
    {
      jv.addElement(list.substring(cp));
    }
    if (jv.size() > 0)
    {
      String[] v = new String[jv.size()];
      for (int i = 0; i < v.length; i++)
      {
        v[i] = (String) jv.elementAt(i);
      }
      jv.removeAllElements();
      if (debug)
      {
        System.err.println("Array from '" + separator
                + "' separated List:\n" + v.length);
        for (int i = 0; i < v.length; i++)
        {
          System.err.println("item " + i + " '" + v[i] + "'");
        }
      }
      return v;
    }
    if (debug)
    {
      System.err.println("Empty Array from '" + separator
              + "' separated List");
    }
    return null;
  }

  /**
   * concatenate the list with separator
   * 
   * @param list
   * @return concatenated string
   */
  public String arrayToSeparatorList(String[] list)
  {
    StringBuffer v = new StringBuffer();
    if (list != null)
    {
      for (int i = 0, iSize = list.length - 1; i < iSize; i++)
      {
        if (list[i] != null)
        {
          v.append(list[i]);
        }
        v.append(separator);
      }
      if (list[list.length - 1] != null)
      {
        v.append(list[list.length - 1]);
      }
      if (debug)
      {
        System.err.println("Returning '" + separator
                + "' separated List:\n");
        System.err.println(v);
      }
      return v.toString();
    }
    if (debug)
    {
      System.err.println("Returning empty '" + separator
              + "' separated List\n");
    }
    return "";
  }

  /**
   * @return
   * @see jalview.appletgui.AlignFrame#getFeatureGroups()
   */
  public String getFeatureGroups()
  {
    String lst = arrayToSeparatorList(getDefaultTargetFrame()
            .getFeatureGroups());
    return lst;
  }

  /**
   * @param alf
   *                alignframe to get feature groups on
   * @return
   * @see jalview.appletgui.AlignFrame#getFeatureGroups()
   */
  public String getFeatureGroupsOn(AlignFrame alf)
  {
    String lst = arrayToSeparatorList(alf.getFeatureGroups());
    return lst;
  }

  /**
   * @param visible
   * @return
   * @see jalview.appletgui.AlignFrame#getFeatureGroupsOfState(boolean)
   */
  public String getFeatureGroupsOfState(boolean visible)
  {
    return arrayToSeparatorList(getDefaultTargetFrame()
            .getFeatureGroupsOfState(visible));
  }

  /**
   * @param alf
   *                align frame to get groups of state visible
   * @param visible
   * @return
   * @see jalview.appletgui.AlignFrame#getFeatureGroupsOfState(boolean)
   */
  public String getFeatureGroupsOfStateOn(AlignFrame alf, boolean visible)
  {
    return arrayToSeparatorList(alf.getFeatureGroupsOfState(visible));
  }

  /**
   * @param groups
   *                tab separated list of group names
   * @param state
   *                true or false
   * @see jalview.appletgui.AlignFrame#setFeatureGroupState(java.lang.String[],
   *      boolean)
   */
  public void setFeatureGroupStateOn(AlignFrame alf, String groups,
          boolean state)
  {
    boolean st = state;// !(state==null || state.equals("") ||
    // state.toLowerCase().equals("false"));
    alf.setFeatureGroupState(separatorListToArray(groups), st);
  }

  public void setFeatureGroupState(String groups, boolean state)
  {
    setFeatureGroupStateOn(getDefaultTargetFrame(), groups, state);
  }

  /**
   * List separator string
   * 
   * @return the separator
   */
  public String getSeparator()
  {
    return separator;
  }

  /**
   * List separator string
   * 
   * @param separator
   *                the separator to set
   */
  public void setSeparator(String separator)
  {
    this.separator = separator;
  }
}
