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

import jalview.io.*;

import java.awt.*;
import java.awt.datatransfer.*;
import java.awt.dnd.*;
import java.awt.event.*;
import java.lang.reflect.Constructor;
import java.util.*;

import javax.swing.*;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

/**
 * DOCUMENT ME!
 * 
 * @author $author$
 * @version $Revision: 1.116.2.4 $
 */
public class Desktop extends jalview.jbgui.GDesktop implements
        DropTargetListener, ClipboardOwner, IProgressIndicator
{
  /** DOCUMENT ME!! */
  public static Desktop instance;

  // Need to decide if the Memory Usage is to be included in
  // Next release or not.
  public static MyDesktopPane desktop;

  // public static JDesktopPane desktop;

  static int openFrameCount = 0;

  static final int xOffset = 30;

  static final int yOffset = 30;

  public static jalview.ws.Discoverer discoverer;

  public static Object[] jalviewClipboard;

  public static boolean internalCopy = false;

  static int fileLoadingCount = 0;

  /**
   * Creates a new Desktop object.
   */
  public Desktop()
  {
    /**
     * A note to implementors. It is ESSENTIAL that any activities that might
     * block are spawned off as threads rather than waited for during this
     * constructor.
     */
    instance = this;
    doVamsasClientCheck();
    doGroovyCheck();

    //setTitle("Jalview " + jalview.bin.Cache.getProperty("VERSION"));
    //setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    boolean selmemusage = jalview.bin.Cache.getDefault("SHOW_MEMUSAGE",
            false);
    desktop = new MyDesktopPane(selmemusage);
    showMemusage.setSelected(selmemusage);
    desktop.setBackground(Color.white);
    this.setLayout(new BorderLayout());
    this.add(desktop, BorderLayout.CENTER);
    desktop.setDragMode(JDesktopPane.OUTLINE_DRAG_MODE);

    // This line prevents Windows Look&Feel resizing all new windows to maximum
    // if previous window was maximised
    desktop.setDesktopManager(new DefaultDesktopManager());

    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    String x = jalview.bin.Cache.getProperty("SCREEN_X");
    String y = jalview.bin.Cache.getProperty("SCREEN_Y");
    String width = jalview.bin.Cache.getProperty("SCREEN_WIDTH");
    String height = jalview.bin.Cache.getProperty("SCREEN_HEIGHT");

    if ((x != null) && (y != null) && (width != null) && (height != null))
    {
      setBounds(Integer.parseInt(x), Integer.parseInt(y), Integer
              .parseInt(width), Integer.parseInt(height));
    }
    else
    {
      setBounds((int) (screenSize.width - 900) / 2,
              (int) (screenSize.height - 650) / 2, 900, 650);
    }

    /*this.addWindowListener(new WindowAdapter()
    {
      public void windowClosing(WindowEvent evt)
      {
        quit();
      }
    });*/

    this.addMouseListener(new MouseAdapter()
    {
      public void mousePressed(MouseEvent evt)
      {
        if (SwingUtilities.isRightMouseButton(evt))
        {
          showPasteMenu(evt.getX(), evt.getY());
        }
      }
    });

    this.setDropTarget(new java.awt.dnd.DropTarget(desktop, this));
    // Spawn a thread that shows the splashscreen
    new SplashScreen();

    discoverer = new jalview.ws.Discoverer(); // Only gets started if gui is
                                              // displayed.
  }

  private void doVamsasClientCheck()
  {
    if (jalview.bin.Cache.vamsasJarsPresent())
    {
      setupVamsasDisconnectedGui();
      VamsasMenu.setVisible(true);
      final Desktop us = this;
      VamsasMenu.addMenuListener(new MenuListener()
      {
        // this listener remembers when the menu was first selected, and
        // doesn't rebuild the session list until it has been cleared and
        // reselected again.
        boolean refresh = true;

        public void menuCanceled(MenuEvent e)
        {
          refresh = true;
        }

        public void menuDeselected(MenuEvent e)
        {
          refresh = true;
        }

        public void menuSelected(MenuEvent e)
        {
          if (refresh)
          {
            us.buildVamsasStMenu();
            refresh = false;
          }
        }
      });
      vamsasStart.setVisible(true);
    }
  }

  void showPasteMenu(int x, int y)
  {
    JPopupMenu popup = new JPopupMenu();
    JMenuItem item = new JMenuItem("Paste To New Window");
    item.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent evt)
      {
        paste();
      }
    });

    popup.add(item);
    popup.show(this, x, y);
  }

  public void paste()
  {
    try
    {
      Clipboard c = Toolkit.getDefaultToolkit().getSystemClipboard();
      Transferable contents = c.getContents(this);

      if (contents != null)
      {
        String file = (String) contents
                .getTransferData(DataFlavor.stringFlavor);

        String format = new IdentifyFile().Identify(file,
                FormatAdapter.PASTE);

        new FileLoader().LoadFile(file, FormatAdapter.PASTE, format);

      }
    } catch (Exception ex)
    {
      System.out
              .println("Unable to paste alignment from system clipboard:\n"
                      + ex);
    }
  }

  /**
   * DOCUMENT ME!
   * 
   * @param frame
   *                DOCUMENT ME!
   * @param title
   *                DOCUMENT ME!
   * @param w
   *                DOCUMENT ME!
   * @param h
   *                DOCUMENT ME!
   */
  public static synchronized void addInternalFrame(
          final JInternalFrame frame, String title, int w, int h)
  {
    addInternalFrame(frame, title, w, h, true);
  }

  /**
   * DOCUMENT ME!
   * 
   * @param frame
   *                DOCUMENT ME!
   * @param title
   *                DOCUMENT ME!
   * @param w
   *                DOCUMENT ME!
   * @param h
   *                DOCUMENT ME!
   * @param resizable
   *                DOCUMENT ME!
   */
  public static synchronized void addInternalFrame(
          final JInternalFrame frame, String title, int w, int h,
          boolean resizable)
  {

    // TODO: consider fixing method to update entries in the window submenu with
    // the current window title

    frame.setTitle(title);
    if (frame.getWidth() < 1 || frame.getHeight() < 1)
    {
      frame.setSize(w, h);
    }
    // THIS IS A PUBLIC STATIC METHOD, SO IT MAY BE CALLED EVEN IN
    // A HEADLESS STATE WHEN NO DESKTOP EXISTS. MUST RETURN
    // IF JALVIEW IS RUNNING HEADLESS
    // ///////////////////////////////////////////////
    if (System.getProperty("java.awt.headless") != null
            && System.getProperty("java.awt.headless").equals("true"))
    {
      return;
    }

    openFrameCount++;

    frame.setVisible(true);
    frame.setClosable(true);
    frame.setResizable(resizable);
    frame.setMaximizable(resizable);
    frame.setIconifiable(resizable);
    frame.setFrameIcon(null);

    if (frame.getX() < 1 && frame.getY() < 1)
    {
      frame.setLocation(xOffset * openFrameCount, yOffset
              * ((openFrameCount - 1) % 10) + yOffset);
    }

    final JMenuItem menuItem = new JMenuItem(title);
    frame
            .addInternalFrameListener(new javax.swing.event.InternalFrameAdapter()
            {
              public void internalFrameActivated(
                      javax.swing.event.InternalFrameEvent evt)
              {
                JInternalFrame itf = desktop.getSelectedFrame();
                if (itf != null)
                {
                  itf.requestFocus();
                }

              }

              public void internalFrameClosed(
                      javax.swing.event.InternalFrameEvent evt)
              {
                PaintRefresher.RemoveComponent(frame);
                openFrameCount--;
                windowMenu.remove(menuItem);
                JInternalFrame itf = desktop.getSelectedFrame();
                if (itf != null)
                {
                  itf.requestFocus();
                }
                System.gc();
              };
            });

    menuItem.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
        try
        {
          frame.setSelected(true);
          frame.setIcon(false);
        } catch (java.beans.PropertyVetoException ex)
        {

        }
      }
    });

    windowMenu.add(menuItem);

    desktop.add(frame);
    frame.toFront();
    try
    {
      frame.setSelected(true);
      frame.requestFocus();
    } catch (java.beans.PropertyVetoException ve)
    {
    }
  }

  public void lostOwnership(Clipboard clipboard, Transferable contents)
  {
    if (!internalCopy)
    {
      Desktop.jalviewClipboard = null;
    }

    internalCopy = false;
  }

  public void dragEnter(DropTargetDragEvent evt)
  {
  }

  public void dragExit(DropTargetEvent evt)
  {
  }

  public void dragOver(DropTargetDragEvent evt)
  {
  }

  public void dropActionChanged(DropTargetDragEvent evt)
  {
  }

  /**
   * DOCUMENT ME!
   * 
   * @param evt
   *                DOCUMENT ME!
   */
  public void drop(DropTargetDropEvent evt)
  {
    Transferable t = evt.getTransferable();
    java.util.List files = null;

    try
    {
      DataFlavor uriListFlavor = new DataFlavor(
              "text/uri-list;class=java.lang.String");
      if (t.isDataFlavorSupported(DataFlavor.javaFileListFlavor))
      {
        // Works on Windows and MacOSX
        evt.acceptDrop(DnDConstants.ACTION_COPY_OR_MOVE);
        files = (java.util.List) t
                .getTransferData(DataFlavor.javaFileListFlavor);
      }
      else if (t.isDataFlavorSupported(uriListFlavor))
      {
        // This is used by Unix drag system
        evt.acceptDrop(DnDConstants.ACTION_COPY_OR_MOVE);
        String data = (String) t.getTransferData(uriListFlavor);
        files = new java.util.ArrayList(1);
        for (java.util.StringTokenizer st = new java.util.StringTokenizer(
                data, "\r\n"); st.hasMoreTokens();)
        {
          String s = st.nextToken();
          if (s.startsWith("#"))
          {
            // the line is a comment (as per the RFC 2483)
            continue;
          }

          java.net.URI uri = new java.net.URI(s);
          java.io.File file = new java.io.File(uri);
          files.add(file);
        }
      }
    } catch (Exception e)
    {
    }

    if (files != null)
    {
      try
      {
        for (int i = 0; i < files.size(); i++)
        {
          String file = files.get(i).toString();
          String protocol = FormatAdapter.FILE;
          String format = null;

          if (file.endsWith(".jar"))
          {
            format = "Jalview";

          }
          else
          {
            format = new IdentifyFile().Identify(file, protocol);
          }

          new FileLoader().LoadFile(file, protocol, format);

        }
      } catch (Exception ex)
      {
      }
    }
  }

  /**
   * DOCUMENT ME!
   * 
   * @param e
   *                DOCUMENT ME!
   */
  public void inputLocalFileMenuItem_actionPerformed(AlignViewport viewport)
  {
    JalviewFileChooser chooser = new JalviewFileChooser(jalview.bin.Cache
            .getProperty("LAST_DIRECTORY"),
            jalview.io.AppletFormatAdapter.READABLE_EXTENSIONS,
            jalview.io.AppletFormatAdapter.READABLE_FNAMES,
            jalview.bin.Cache.getProperty("DEFAULT_FILE_FORMAT"));

    chooser.setFileView(new JalviewFileView());
    chooser.setDialogTitle("Open local file");
    chooser.setToolTipText("Open");

    int value = chooser.showOpenDialog(this);

    if (value == JalviewFileChooser.APPROVE_OPTION)
    {
      String choice = chooser.getSelectedFile().getPath();
      jalview.bin.Cache.setProperty("LAST_DIRECTORY", chooser
              .getSelectedFile().getParent());

      String format = null;
      if (chooser.getSelectedFormat().equals("Jalview"))
      {
        format = "Jalview";
      }
      else
      {
        format = new IdentifyFile().Identify(choice, FormatAdapter.FILE);
      }

      if (viewport != null)
      {
        new FileLoader().LoadFile(viewport, choice, FormatAdapter.FILE,
                format);
      }
      else
      {
        new FileLoader().LoadFile(choice, FormatAdapter.FILE, format);
      }
    }
  }

  /**
   * DOCUMENT ME!
   * 
   * @param e
   *                DOCUMENT ME!
   */
  public void inputURLMenuItem_actionPerformed(AlignViewport viewport)
  {
    // This construct allows us to have a wider textfield
    // for viewing
    JLabel label = new JLabel("Enter URL of Input File");
    final JComboBox history = new JComboBox();

    JPanel panel = new JPanel(new GridLayout(2, 1));
    panel.add(label);
    panel.add(history);
    history.setPreferredSize(new Dimension(400, 20));
    history.setEditable(true);
    history.addItem("http://www.");

    String historyItems = jalview.bin.Cache.getProperty("RECENT_URL");

    StringTokenizer st;

    if (historyItems != null)
    {
      st = new StringTokenizer(historyItems, "\t");

      while (st.hasMoreTokens())
      {
        history.addItem(st.nextElement());
      }
    }

    int reply = JOptionPane.showInternalConfirmDialog(desktop, panel,
            "Input Alignment From URL", JOptionPane.OK_CANCEL_OPTION);

    if (reply != JOptionPane.OK_OPTION)
    {
      return;
    }

    String url = history.getSelectedItem().toString();

    if (url.toLowerCase().endsWith(".jar"))
    {
      if (viewport != null)
      {
        new FileLoader().LoadFile(viewport, url, FormatAdapter.URL,
                "Jalview");
      }
      else
      {
        new FileLoader().LoadFile(url, FormatAdapter.URL, "Jalview");
      }
    }
    else
    {
      String format = new IdentifyFile().Identify(url, FormatAdapter.URL);

      if (format.equals("URL NOT FOUND"))
      {
        JOptionPane.showInternalMessageDialog(Desktop.desktop,
                "Couldn't locate " + url, "URL not found",
                JOptionPane.WARNING_MESSAGE);

        return;
      }

      if (viewport != null)
      {
        new FileLoader().LoadFile(viewport, url, FormatAdapter.URL, format);
      }
      else
      {
        new FileLoader().LoadFile(url, FormatAdapter.URL, format);
      }
    }
  }

  /**
   * DOCUMENT ME!
   * 
   * @param e
   *                DOCUMENT ME!
   */
  public void inputTextboxMenuItem_actionPerformed(AlignViewport viewport)
  {
    CutAndPasteTransfer cap = new CutAndPasteTransfer();
    cap.setForInput(viewport);
    Desktop.addInternalFrame(cap, "Cut & Paste Alignment File", 600, 500);
  }

  /*
   * Exit the program
   */
  public void quit()
  {
    jalview.bin.Cache.setProperty("SCREEN_X", getBounds().x + "");
    jalview.bin.Cache.setProperty("SCREEN_Y", getBounds().y + "");
    jalview.bin.Cache.setProperty("SCREEN_WIDTH", getWidth() + "");
    jalview.bin.Cache.setProperty("SCREEN_HEIGHT", getHeight() + "");
    System.exit(0);
  }

  /**
   * DOCUMENT ME!
   * 
   * @param e
   *                DOCUMENT ME!
   */
  public void aboutMenuItem_actionPerformed(ActionEvent e)
  {
    StringBuffer message = new StringBuffer("Jalview version "
            + jalview.bin.Cache.getProperty("VERSION") + "; last updated: "
            + jalview.bin.Cache.getDefault("BUILD_DATE", "unknown"));

    if (!jalview.bin.Cache.getProperty("LATEST_VERSION").equals(
            jalview.bin.Cache.getProperty("VERSION")))
    {
      message
              .append("\n\n!! Jalview version "
                      + jalview.bin.Cache.getProperty("LATEST_VERSION")
                      + " is available for download from http://www.jalview.org !!\n");

    }
    // TODO: update this text for each release or centrally store it for lite
    // and application
    message
            .append("\nAuthors:  Andrew Waterhouse, Jim Procter, Michele Clamp, James Cuff, Steve Searle,\n    David Martin & Geoff Barton."
                    + "\nDevelopment managed by The Barton Group, University of Dundee, Scotland, UK.\n"
                    + "\nFor help, see the FAQ at www.jalview.org and/or join the jalview-discuss@jalview.org mailing list\n"
                    + "\nIf  you use Jalview, please cite:"
                    + "\n\"Clamp, M., Cuff, J., Searle, S. M. and Barton, G. J. (2004), The Jalview Java Alignment Editor\""
                    + "\nBioinformatics,  2004 20;426-7.");

    JOptionPane.showInternalMessageDialog(Desktop.desktop,

    message.toString(), "About Jalview", JOptionPane.INFORMATION_MESSAGE);
  }

  /**
   * DOCUMENT ME!
   * 
   * @param e
   *                DOCUMENT ME!
   */
  public void documentationMenuItem_actionPerformed(ActionEvent e)
  {
    try
    {
      ClassLoader cl = jalview.gui.Desktop.class.getClassLoader();
      java.net.URL url = javax.help.HelpSet.findHelpSet(cl, "help/help");
      javax.help.HelpSet hs = new javax.help.HelpSet(cl, url);

      javax.help.HelpBroker hb = hs.createHelpBroker();
      hb.setCurrentID("home");
      hb.setDisplayed(true);
    } catch (Exception ex)
    {
    }
  }

  public void closeAll_actionPerformed(ActionEvent e)
  {
    JInternalFrame[] frames = desktop.getAllFrames();
    for (int i = 0; i < frames.length; i++)
    {
      try
      {
        frames[i].setClosed(true);
      } catch (java.beans.PropertyVetoException ex)
      {
      }
    }
    System.out.println("ALL CLOSED");

  }

  public void raiseRelated_actionPerformed(ActionEvent e)
  {
    reorderAssociatedWindows(false, false);
  }

  public void minimizeAssociated_actionPerformed(ActionEvent e)
  {
    reorderAssociatedWindows(true, false);
  }

  void closeAssociatedWindows()
  {
    reorderAssociatedWindows(false, true);
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.jbgui.GDesktop#garbageCollect_actionPerformed(java.awt.event.ActionEvent)
   */
  protected void garbageCollect_actionPerformed(ActionEvent e)
  {
    // We simply collect the garbage
    jalview.bin.Cache.log.debug("Collecting garbage...");
    System.gc();
    jalview.bin.Cache.log.debug("Finished garbage collection.");
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.jbgui.GDesktop#showMemusage_actionPerformed(java.awt.event.ActionEvent)
   */
  protected void showMemusage_actionPerformed(ActionEvent e)
  {
    desktop.showMemoryUsage(showMemusage.isSelected());
  }

  void reorderAssociatedWindows(boolean minimize, boolean close)
  {
    JInternalFrame[] frames = desktop.getAllFrames();
    if (frames == null || frames.length < 1)
    {
      return;
    }

    AlignViewport source = null, target = null;
    if (frames[0] instanceof AlignFrame)
    {
      source = ((AlignFrame) frames[0]).getCurrentView();
    }
    else if (frames[0] instanceof TreePanel)
    {
      source = ((TreePanel) frames[0]).getViewPort();
    }
    else if (frames[0] instanceof PCAPanel)
    {
      source = ((PCAPanel) frames[0]).av;
    }
    else if (frames[0].getContentPane() instanceof PairwiseAlignPanel)
    {
      source = ((PairwiseAlignPanel) frames[0].getContentPane()).av;
    }

    if (source != null)
    {
      for (int i = 0; i < frames.length; i++)
      {
        target = null;
        if (frames[i] == null)
        {
          continue;
        }
        if (frames[i] instanceof AlignFrame)
        {
          target = ((AlignFrame) frames[i]).getCurrentView();
        }
        else if (frames[i] instanceof TreePanel)
        {
          target = ((TreePanel) frames[i]).getViewPort();
        }
        else if (frames[i] instanceof PCAPanel)
        {
          target = ((PCAPanel) frames[i]).av;
        }
        else if (frames[i].getContentPane() instanceof PairwiseAlignPanel)
        {
          target = ((PairwiseAlignPanel) frames[i].getContentPane()).av;
        }

        if (source == target)
        {
          try
          {
            if (close)
            {
              frames[i].setClosed(true);
            }
            else
            {
              frames[i].setIcon(minimize);
              if (!minimize)
              {
                frames[i].toFront();
              }
            }

          } catch (java.beans.PropertyVetoException ex)
          {
          }
        }
      }
    }
  }

  /**
   * DOCUMENT ME!
   * 
   * @param e
   *                DOCUMENT ME!
   */
  protected void preferences_actionPerformed(ActionEvent e)
  {
    new Preferences();
  }

  /**
   * DOCUMENT ME!
   * 
   * @param e
   *                DOCUMENT ME!
   */
  public void saveState_actionPerformed(ActionEvent e)
  {
    JalviewFileChooser chooser = new JalviewFileChooser(jalview.bin.Cache
            .getProperty("LAST_DIRECTORY"), new String[]
    { "jar" }, new String[]
    { "Jalview Project" }, "Jalview Project");

    chooser.setFileView(new JalviewFileView());
    chooser.setDialogTitle("Save State");

    int value = chooser.showSaveDialog(this);

    if (value == JalviewFileChooser.APPROVE_OPTION)
    {
      java.io.File choice = chooser.getSelectedFile();
      jalview.bin.Cache.setProperty("LAST_DIRECTORY", choice.getParent());
      new Jalview2XML().SaveState(choice);
    }
  }

  /**
   * DOCUMENT ME!
   * 
   * @param e
   *                DOCUMENT ME!
   */
  public void loadState_actionPerformed(ActionEvent e)
  {
    JalviewFileChooser chooser = new JalviewFileChooser(jalview.bin.Cache
            .getProperty("LAST_DIRECTORY"), new String[]
    { "jar" }, new String[]
    { "Jalview Project" }, "Jalview Project");
    chooser.setFileView(new JalviewFileView());
    chooser.setDialogTitle("Restore state");

    int value = chooser.showOpenDialog(this);

    if (value == JalviewFileChooser.APPROVE_OPTION)
    {
      String choice = chooser.getSelectedFile().getAbsolutePath();
      jalview.bin.Cache.setProperty("LAST_DIRECTORY", chooser
              .getSelectedFile().getParent());
      new Jalview2XML().LoadJalviewAlign(choice);
    }
  }

  public void inputSequence_actionPerformed(ActionEvent e)
  {
    new SequenceFetcher(this);
  }

  JPanel progressPanel;

  public void startLoading(final String fileName)
  {
    if (fileLoadingCount == 0)
    {
      addProgressPanel("Loading File: " + fileName + "   ");

    }
    fileLoadingCount++;
  }

  private JProgressBar addProgressPanel(String string)
  {
    if (progressPanel == null)
    {
      progressPanel = new JPanel(new BorderLayout());
      totalProgressCount = 0;
    }
    JProgressBar progressBar = new JProgressBar();
    progressBar.setIndeterminate(true);

    progressPanel.add(new JLabel(string), BorderLayout.WEST);

    progressPanel.add(progressBar, BorderLayout.CENTER);

    this.add(progressPanel, BorderLayout.SOUTH);
    totalProgressCount++;
    validate();
    return progressBar;
  }

  int totalProgressCount = 0;

  private void removeProgressPanel(JProgressBar progbar)
  {
    if (progressPanel != null)
    {
      progressPanel.remove(progbar);
      if (--totalProgressCount < 1)
      {
        this.remove(progressPanel);
        progressPanel = null;
      }
    }
    validate();
  }

  public void stopLoading()
  {
    fileLoadingCount--;
    if (fileLoadingCount < 1)
    {
      if (progressPanel != null)
      {
        this.remove(progressPanel);
        progressPanel = null;
      }
      fileLoadingCount = 0;
    }
    validate();
  }

  public static int getViewCount(String viewId)
  {
    int count = 0;
    JInternalFrame[] frames = Desktop.desktop.getAllFrames();
    for (int t = 0; t < frames.length; t++)
    {
      if (frames[t] instanceof AlignFrame)
      {
        AlignFrame af = (AlignFrame) frames[t];
        for (int a = 0; a < af.alignPanels.size(); a++)
        {
          if (viewId
                  .equals(((AlignmentPanel) af.alignPanels.elementAt(a)).av
                          .getSequenceSetId()))
          {
            count++;
          }
        }
      }
    }

    return count;
  }

  public void explodeViews(AlignFrame af)
  {
    int size = af.alignPanels.size();
    if (size < 2)
    {
      return;
    }

    for (int i = 0; i < size; i++)
    {
      AlignmentPanel ap = (AlignmentPanel) af.alignPanels.elementAt(i);
      AlignFrame newaf = new AlignFrame(ap);
      if (ap.av.explodedPosition != null
              && !ap.av.explodedPosition.equals(af.getBounds()))
      {
        newaf.setBounds(ap.av.explodedPosition);
      }

      ap.av.gatherViewsHere = false;

      addInternalFrame(newaf, af.getTitle(), AlignFrame.DEFAULT_WIDTH,
              AlignFrame.DEFAULT_HEIGHT);
    }

    af.alignPanels.clear();
    af.closeMenuItem_actionPerformed(true);

  }

  public void gatherViews(AlignFrame source)
  {
    source.viewport.gatherViewsHere = true;
    source.viewport.explodedPosition = source.getBounds();
    JInternalFrame[] frames = desktop.getAllFrames();
    String viewId = source.viewport.sequenceSetID;

    for (int t = 0; t < frames.length; t++)
    {
      if (frames[t] instanceof AlignFrame && frames[t] != source)
      {
        AlignFrame af = (AlignFrame) frames[t];
        boolean gatherThis = false;
        for (int a = 0; a < af.alignPanels.size(); a++)
        {
          AlignmentPanel ap = (AlignmentPanel) af.alignPanels.elementAt(a);
          if (viewId.equals(ap.av.getSequenceSetId()))
          {
            gatherThis = true;
            ap.av.gatherViewsHere = false;
            ap.av.explodedPosition = af.getBounds();
            source.addAlignmentPanel(ap, false);
          }
        }

        if (gatherThis)
        {
          af.alignPanels.clear();
          af.closeMenuItem_actionPerformed(true);
        }
      }
    }

  }

  jalview.gui.VamsasApplication v_client = null;

  public void vamsasImport_actionPerformed(ActionEvent e)
  {
    if (v_client == null)
    {
      // Load and try to start a session.
      JalviewFileChooser chooser = new JalviewFileChooser(jalview.bin.Cache
              .getProperty("LAST_DIRECTORY"));

      chooser.setFileView(new JalviewFileView());
      chooser.setDialogTitle("Open a saved VAMSAS session");
      chooser
              .setToolTipText("select a vamsas session to be opened as a new vamsas session.");

      int value = chooser.showOpenDialog(this);

      if (value == JalviewFileChooser.APPROVE_OPTION)
      {
        try
        {
          v_client = new jalview.gui.VamsasApplication(this, chooser
                  .getSelectedFile());
        } catch (Exception ex)
        {
          jalview.bin.Cache.log.error(
                  "New vamsas session from existing session file failed:",
                  ex);
          return;
        }
        setupVamsasConnectedGui();
        v_client.initial_update(); // TODO: thread ?
      }
    }
    else
    {
      jalview.bin.Cache.log
              .error("Implementation error - load session from a running session is not supported.");
    }
  }

  public void vamsasStart_actionPerformed(ActionEvent e)
  {
    if (v_client == null)
    {
      // Start a session.
      // we just start a default session for moment.
      /*
       * JalviewFileChooser chooser = new JalviewFileChooser(jalview.bin.Cache.
       * getProperty("LAST_DIRECTORY"));
       * 
       * chooser.setFileView(new JalviewFileView());
       * chooser.setDialogTitle("Load Vamsas file");
       * chooser.setToolTipText("Import");
       * 
       * int value = chooser.showOpenDialog(this);
       * 
       * if (value == JalviewFileChooser.APPROVE_OPTION) { v_client = new
       * jalview.gui.VamsasApplication(this, chooser.getSelectedFile());
       * 
       */
      v_client = new VamsasApplication(this);
      setupVamsasConnectedGui();
      v_client.initial_update(); // TODO: thread ?
    }
    else
    {
      // store current data in session.
      v_client.push_update(); // TODO: thread
    }
  }

  protected void setupVamsasConnectedGui()
  {
    vamsasStart.setText("Session Update");
    vamsasSave.setVisible(true);
    vamsasStop.setVisible(true);
    vamsasImport.setVisible(false); // Document import to existing session is
                                    // not possible for vamsas-client-1.0.
  }

  protected void setupVamsasDisconnectedGui()
  {
    vamsasSave.setVisible(false);
    vamsasStop.setVisible(false);
    vamsasImport.setVisible(true);
    vamsasStart.setText("New Vamsas Session");
  }

  public void vamsasStop_actionPerformed(ActionEvent e)
  {
    if (v_client != null)
    {
      v_client.end_session();
      v_client = null;
      setupVamsasDisconnectedGui();
    }
  }

  protected void buildVamsasStMenu()
  {
    if (v_client == null)
    {
      String[] sess = null;
      try
      {
        sess = VamsasApplication.getSessionList();
      } catch (Exception e)
      {
        jalview.bin.Cache.log.warn(
                "Problem getting current sessions list.", e);
        sess = null;
      }
      if (sess != null)
      {
        jalview.bin.Cache.log.debug("Got current sessions list: "
                + sess.length + " entries.");
        VamsasStMenu.removeAll();
        for (int i = 0; i < sess.length; i++)
        {
          JMenuItem sessit = new JMenuItem();
          sessit.setText(sess[i]);
          sessit.setToolTipText("Connect to session " + sess[i]);
          final Desktop dsktp = this;
          final String mysesid = sess[i];
          sessit.addActionListener(new ActionListener()
          {

            public void actionPerformed(ActionEvent e)
            {
              if (dsktp.v_client == null)
              {
                Thread rthr = new Thread(new Runnable()
                {

                  public void run()
                  {
                    dsktp.v_client = new VamsasApplication(dsktp, mysesid);
                    dsktp.setupVamsasConnectedGui();
                    dsktp.v_client.initial_update();
                  }

                });
                rthr.start();
              }
            };
          });
          VamsasStMenu.add(sessit);
        }
        // don't show an empty menu.
        VamsasStMenu.setVisible(sess.length > 0);

      }
      else
      {
        jalview.bin.Cache.log.debug("No current vamsas sessions.");
        VamsasStMenu.removeAll();
        VamsasStMenu.setVisible(false);
      }
    }
    else
    {
      // Not interested in the content. Just hide ourselves.
      VamsasStMenu.setVisible(false);
    }
  }

  public void vamsasSave_actionPerformed(ActionEvent e)
  {
    if (v_client != null)
    {
      JalviewFileChooser chooser = new JalviewFileChooser(jalview.bin.Cache
              .getProperty("LAST_DIRECTORY"), new String[]
      { "vdj" }, // TODO: VAMSAS DOCUMENT EXTENSION is VDJ
              new String[]
              { "Vamsas Document" }, "Vamsas Document");

      chooser.setFileView(new JalviewFileView());
      chooser.setDialogTitle("Save Vamsas Document Archive");

      int value = chooser.showSaveDialog(this);

      if (value == JalviewFileChooser.APPROVE_OPTION)
      {
        java.io.File choice = chooser.getSelectedFile();
        jalview.bin.Cache.setProperty("LAST_DIRECTORY", choice.getParent());
        String warnmsg = null;
        String warnttl = null;
        try
        {
          v_client.vclient.storeDocument(choice);
        } catch (Error ex)
        {
          warnttl = "Serious Problem saving Vamsas Document";
          warnmsg = ex.toString();
          jalview.bin.Cache.log.error("Error Whilst saving document to "
                  + choice, ex);

        } catch (Exception ex)
        {
          warnttl = "Problem saving Vamsas Document.";
          warnmsg = ex.toString();
          jalview.bin.Cache.log.warn("Exception Whilst saving document to "
                  + choice, ex);

        }
        if (warnmsg != null)
        {
          JOptionPane.showInternalMessageDialog(Desktop.desktop,

          warnmsg, warnttl, JOptionPane.ERROR_MESSAGE);
        }
      }
    }
  }

  JProgressBar vamUpdate = null;

  /**
   * hide vamsas user gui bits when a vamsas document event is being handled.
   * 
   * @param b
   *                true to hide gui, false to reveal gui
   */
  public void setVamsasUpdate(boolean b)
  {
    jalview.bin.Cache.log.debug("Setting gui for Vamsas update "
            + (b ? "in progress" : "finished"));

    if (vamUpdate != null)
    {
      this.removeProgressPanel(vamUpdate);
    }
    if (b)
    {
      vamUpdate = this.addProgressPanel("Updating vamsas session");
    }
    vamsasStart.setVisible(!b);
    vamsasStop.setVisible(!b);
    vamsasSave.setVisible(!b);
  }

  public JInternalFrame[] getAllFrames()
  {
    return desktop.getAllFrames();
  }

  /**
   * Checks the given url to see if it gives a response indicating that the user
   * should be informed of a new questionnaire.
   * 
   * @param url
   */
  public void checkForQuestionnaire(String url)
  {
    UserQuestionnaireCheck jvq = new UserQuestionnaireCheck(url);
    // javax.swing.SwingUtilities.invokeLater(jvq);
    new Thread(jvq).start();
  }

  /**
   * Proxy class for JDesktopPane which optionally displays the current memory
   * usage and highlights the desktop area with a red bar if free memory runs
   * low.
   * 
   * @author AMW
   */
  public class MyDesktopPane extends JDesktopPane implements Runnable
  {

    boolean showMemoryUsage = false;

    Runtime runtime;

    java.text.NumberFormat df;

    float maxMemory, allocatedMemory, freeMemory, totalFreeMemory,
            percentUsage;

    public MyDesktopPane(boolean showMemoryUsage)
    {
      showMemoryUsage(showMemoryUsage);
    }

    public void showMemoryUsage(boolean showMemoryUsage)
    {
      this.showMemoryUsage = showMemoryUsage;
      if (showMemoryUsage)
      {
        Thread worker = new Thread(this);
        worker.start();
      }
    }

    public boolean isShowMemoryUsage()
    {
      return showMemoryUsage;
    }

    public void run()
    {
      df = java.text.NumberFormat.getNumberInstance();
      df.setMaximumFractionDigits(2);
      runtime = Runtime.getRuntime();

      while (showMemoryUsage)
      {
        try
        {
          Thread.sleep(3000);
          maxMemory = runtime.maxMemory() / 1048576f;
          allocatedMemory = runtime.totalMemory() / 1048576f;
          freeMemory = runtime.freeMemory() / 1048576f;
          totalFreeMemory = freeMemory + (maxMemory - allocatedMemory);

          percentUsage = (totalFreeMemory / maxMemory) * 100;

          // if (percentUsage < 20)
          {
            // border1 = BorderFactory.createMatteBorder(12, 12, 12, 12,
            // Color.red);
            // instance.set.setBorder(border1);
          }
          repaint();

        } catch (Exception ex)
        {
          ex.printStackTrace();
        }
      }
    }

    public void paintComponent(Graphics g)
    {
      if (showMemoryUsage)
      {
        if (percentUsage < 20)
          g.setColor(Color.red);

        g.drawString("Total Free Memory: " + df.format(totalFreeMemory)
                + "MB; Max Memory: " + df.format(maxMemory) + "MB; "
                + df.format(percentUsage) + "%", 10, getHeight()
                - g.getFontMetrics().getHeight());
      }
    }

  }

  protected JMenuItem groovyShell;

  public void doGroovyCheck()
  {
    if (jalview.bin.Cache.groovyJarsPresent())
    {
      groovyShell = new JMenuItem();
      groovyShell.setText("Groovy Console...");
      groovyShell.addActionListener(new ActionListener()
      {
        public void actionPerformed(ActionEvent e)
        {
          groovyShell_actionPerformed(e);
        }
      });
      toolsMenu.add(groovyShell);
      groovyShell.setVisible(true);
    }
  }

  /**
   * Accessor method to quickly get all the AlignmentFrames loaded.
   */
  public static AlignFrame[] getAlignframes()
  {
    JInternalFrame[] frames = Desktop.desktop.getAllFrames();

    if (frames == null)
    {
      return null;
    }
    Vector avp = new Vector();
    try
    {
      // REVERSE ORDER
      for (int i = frames.length - 1; i > -1; i--)
      {
        if (frames[i] instanceof AlignFrame)
        {
          AlignFrame af = (AlignFrame) frames[i];
          avp.addElement(af);
        }
      }
    } catch (Exception ex)
    {
      ex.printStackTrace();
    }
    if (avp.size() == 0)
    {
      return null;
    }
    AlignFrame afs[] = new AlignFrame[avp.size()];
    for (int i = 0, j = avp.size(); i < j; i++)
    {
      afs[i] = (AlignFrame) avp.elementAt(i);
    }
    avp.clear();
    return afs;
  }

  /**
   * Add Groovy Support to Jalview
   */
  public void groovyShell_actionPerformed(ActionEvent e)
  {
    // use reflection to avoid creating compilation dependency.
    if (!jalview.bin.Cache.groovyJarsPresent())
    {
      throw new Error(
              "Implementation Error. Cannot create groovyShell without Groovy on the classpath!");
    }
    try
    {
      Class gcClass = Desktop.class.getClassLoader().loadClass(
              "groovy.ui.Console");
      Constructor gccons = gcClass.getConstructor(null);
      java.lang.reflect.Method setvar = gcClass.getMethod("setVariable",
              new Class[]
              { String.class, Object.class });
      java.lang.reflect.Method run = gcClass.getMethod("run", null);
      Object gc = gccons.newInstance(null);
      setvar.invoke(gc, new Object[]
      { "Jalview", this });
      run.invoke(gc, null);
    } catch (Exception ex)
    {
      jalview.bin.Cache.log.error("Groovy Shell Creation failed.", ex);
      JOptionPane
              .showInternalMessageDialog(
                      Desktop.desktop,

                      "Couldn't create the groovy Shell. Check the error log for the details of what went wrong.",
                      "Jalview Groovy Support Failed",
                      JOptionPane.ERROR_MESSAGE);
    }
  }

  /**
   * Progress bars managed by the IProgressIndicator method.
   */
  private Hashtable progressBars;

  /*
   * (non-Javadoc)
   * 
   * @see jalview.gui.IProgressIndicator#setProgressBar(java.lang.String, long)
   */
  public void setProgressBar(String message, long id)
  {
    if (progressBars == null)
    {
      progressBars = new Hashtable();
    }

    if (progressBars.get(new Long(id)) != null)
    {
      JProgressBar progressPanel = (JProgressBar) progressBars
              .remove(new Long(id));
      removeProgressPanel(progressPanel);
    }
    else
    {
      progressBars.put(new Long(id), addProgressPanel(message));
    }
  }
}
