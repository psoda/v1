package gui;

import javax.swing.JFileChooser;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;
import javax.swing.text.DefaultEditorKit;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.Font;

import java.io.File;

import DataConvert.DataConvert.*;
import forester.atv.*;
import forester.tree.*;

/**
 * MenuBar
 * November 11, 2006
 * 
 */

/**
 * Will create the MenuBar for the GUI
 * 
 */
public class MainMenu extends JMenuBar {

	private static final long serialVersionUID = -94426911685495885L;

	/**
	 * A reference to this menu bar
	 */
	private MainMenu thisMenuBar;

	private JMenuItem fileNew;
	private JMenuItem fileOpen;
	private JMenuItem fileOpenInDataConvert;
	private JMenuItem fileSave;
	private JMenuItem fileSaveAs;
	private JMenuItem fileClose;

	public MainMenu(final PSODA gui, final MainTabs psodaTabs) {
		thisMenuBar = this;
		/* File Menu Items */
		fileNew = new JMenuItem("New...");
		fileOpen = new JMenuItem("Open...");
		fileOpenInDataConvert = new JMenuItem("Open in Data Convert...");
		fileClose = new JMenuItem("Close");
		fileSave = new JMenuItem("Save");
		fileSaveAs = new JMenuItem("Save as...");
		// JMenuItem filePrint = new JMenuItem("Print...");

		/* Edit Menu Items */
		JMenuItem undo = new JMenuItem(psodaTabs.getUndoRedo().getUndoAction());
		JMenuItem redo = new JMenuItem(psodaTabs.getUndoRedo().getRedoAction());

		/* Tree Menu Items */
		JMenuItem openATV = new JMenuItem("Open ATV (A Tree Viewer)");

		/* Tools Menu Items */
		JMenuItem dataConvert = new JMenuItem("Data Convert 1.0");

		/* Menus */
		JMenu fileMenu = new JMenu("File");
		fileMenu.add(fileNew);
		fileNew.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N,
				java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		fileNew.setFont(new Font("Helvetica", Font.PLAIN, 15));

		fileMenu.add(fileOpen);
		fileOpen.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O,
				java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		fileOpen.setFont(new Font("Helvetica", Font.PLAIN, 15));

		fileMenu.add(fileOpenInDataConvert);
		fileOpenInDataConvert.setAccelerator(KeyStroke.getKeyStroke(
				KeyEvent.VK_I, java.awt.Toolkit.getDefaultToolkit()
						.getMenuShortcutKeyMask()));
		fileOpenInDataConvert.setFont(new Font("Helvetica", Font.PLAIN, 15));

		fileMenu.addSeparator();

		fileMenu.add(fileSave);
		fileSave.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S,
				java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		fileSave.setFont(new Font("Helvetica", Font.PLAIN, 15));

		fileMenu.add(fileSaveAs);
		// fileSaveAs.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A,
		// java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		fileSaveAs.setFont(new Font("Helvetica", Font.PLAIN, 15));

		fileMenu.add(fileClose);
		fileClose.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q,
				java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		fileClose.setFont(new Font("Helvetica", Font.PLAIN, 15));

		JMenu editMenu = new JMenu("Edit");
		editMenu.add(undo);
		undo.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Z,
				java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		undo.setFont(new Font("Helvetica", Font.PLAIN, 15));

		editMenu.add(redo);
		redo.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Y,
				java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		redo.setFont(new Font("Helvetica", Font.PLAIN, 15));

		editMenu.addSeparator();
		editMenu.add(psodaTabs.getActionByName(DefaultEditorKit.cutAction));
		editMenu.add(psodaTabs.getActionByName(DefaultEditorKit.copyAction));
		editMenu.add(psodaTabs.getActionByName(DefaultEditorKit.pasteAction));
		editMenu.addSeparator();
		editMenu.add(psodaTabs
				.getActionByName(DefaultEditorKit.selectAllAction));

		JMenu treeMenu = new JMenu("Trees");
		treeMenu.add(openATV);
		openATV.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_T,
				java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		openATV.setFont(new Font("Helvetica", Font.PLAIN, 15));

		JMenu toolsMenu = new JMenu("Tools");
		toolsMenu.add(dataConvert);
		dataConvert.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_D,
				java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
		dataConvert.setFont(new Font("Helvetica", Font.PLAIN, 15));

		this.add(fileMenu);
		this.add(editMenu);
		this.add(treeMenu);
		this.add(toolsMenu);

		/* Create individual ActionListeners */
		fileNew.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				psodaTabs.createNewFile();
				fileClose.setEnabled(true);
				fileSave.setEnabled(true);
				fileSaveAs.setEnabled(true);
			}
		});
		fileOpen.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				File[] fileList = new File[1];

				if (gui.getFileChooser().showOpenDialog(gui.getPsodaFrame()) == JFileChooser.APPROVE_OPTION) {
					File file = gui.getFileChooser().getSelectedFile();

					if (file != null) {
						fileList[0] = file;
						psodaTabs.openFile(fileList);
					}
				}

			}
		});
		fileClose.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (psodaTabs.getMainTabsSelectedIndex() == 1) { // 1 ==
																	// "Input
																	// File" tab
					psodaTabs.closeInputTab(psodaTabs
							.getInputTabsSelectedIndex());
					if (psodaTabs.getInputTabsTabCount() == 0) {
						fileSave.setEnabled(false);
						fileSaveAs.setEnabled(false);
						fileClose.setEnabled(false);
					}
				}
			}
		});
		fileOpenInDataConvert.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// String tabTitle =
				// psodaTabs.getInputTabsTitleAt(psodaTabs.getInputTabsSelectedIndex());//inputTabby.getTitleAt(inputTabby.getSelectedIndex());
				if (!psodaTabs.currentDocIsSaved()) {
					JOptionPane.showMessageDialog(null,
							"WARNING: Please save your file first!", "Warning",
							JOptionPane.WARNING_MESSAGE);
				}

				else {
					String path = psodaTabs.getPathPropertyOfCurrentDoc();

					Driver dataDriver = new Driver();
					dataDriver.gui(null);
					dataDriver.setPsodaGui(gui);
					dataDriver.getGuiObject().openFile(path);
				}
			}
		});
		fileSave.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// String tabTitle =
				// psodaTabs.getInputTabsTitleAt(psodaTabs.getInputTabsSelectedIndex());
				File fileToSave = null;
				if (!psodaTabs.currentDocHasHardFile()) { // If a tab titled
															// "Untitled"
															// exists...
					thisMenuBar.doSaveAs();
				} else {
					fileToSave = new File(psodaTabs
							.getPathPropertyOfCurrentDoc()); // Will create
																// new file with
																// the title of
																// currently
																// selected tab
				}
				if (fileToSave != null) {
					psodaTabs.saveFile(fileToSave);
				}
			}
		});
		fileSaveAs.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				thisMenuBar.doSaveAs();
			}
		});
		openATV.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					Tree tree = null;
					ATVjframe atvframe = new ATVjframe(tree);
					atvframe.showWhole();
				} catch (Exception exp) {
					System.err.println("Could not read Tree. Terminating.");
				}
			}
		});
		dataConvert.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Driver dataDriver = new Driver();
				dataDriver.gui(null);
				dataDriver.setPsodaGui(gui);
			}
		});

	}

	protected void doSaveAs() {
		PSODA gui = PSODA.getInstance();
		MainTabs tabs = gui.getPsodaTabs();
		if (gui.getFileChooser().showSaveDialog(gui.getPsodaFrame()) == JFileChooser.APPROVE_OPTION) {
			File file = gui.getFileChooser().getSelectedFile();

			if (file != null) {
				tabs.saveFile(file);
			}
		}
	}

	public void setSaveFileMenuItemEnabled(boolean bool) {
		fileSave.setEnabled(bool);
	}

	public void setSaveAsFileMenuItemEnabled(boolean bool) {
		fileSaveAs.setEnabled(bool);
	}

	public void setCloseFileMenuItemEnabled(boolean bool) {
		fileClose.setEnabled(bool);
	}

}
