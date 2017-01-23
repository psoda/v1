package gui;

import java.util.Date;
import java.lang.Runtime;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Rectangle;
import java.io.File;
import java.io.IOException;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.filechooser.FileFilter;
import javax.swing.text.BadLocationException;

import forester.atv.ATVjframe;
import forester.tree.Tree;

/**
 * PSODA November 11, 2006
 * 
 */
public class PSODA {

	public native void initInterpreter(long vmsize);
	public native void interpretFile(String fileName);
	public native void deleteInterpreter();
	public native void pausePSODA();
	public native void resumePSODA();
	public native void killPSODA();
	public native void writeTreeSAAPPSODA(String fileName);
	public native boolean saveTrees(String fileName, String format);
	public native boolean saveAlign(String fileName, String format);
	public native String[] getTrees();
	public native String[] getTaxaNames();
	public native static String[] getDefinedCommandNames();
	public native static String[] getPopularCommandNames();

	/**
	 * A reference to the singleton instance of the gui
	 */
	private static PSODA psodaInstance;

	/**
	 * The main gui Frame
	 */
	private JFrame psodaFrame;

	/**
	 * The file chooser that should be used when selecting a file from the file
	 * system
	 */
	private JFileChooser fileChooser;

	/**
	 * The time that psoda was started
	 */
	private Date date;

	/**
	 * Is true while C++ psoda has a thread running
	 */
	private boolean running = false;

	private MainMenu menuBar;
	private MainTabs mainTabs;

	public static PSODA getInstance() {
		if (psodaInstance == null) {
			psodaInstance = new PSODA();
		}
		return psodaInstance;
	}

	private PSODA() {
		date = new Date();
		setupFileChooser();

		psodaFrame = new JFrame(
				"PSODA - Open Source Genomic Analysis");
		psodaFrame.setPreferredSize(new Dimension(1045, 800));
		psodaFrame.setDefaultCloseOperation(3); // EXIT_ON_CLOSE

		/* Create the CaretListenerLabel */
		JPanel statusPanel = new JPanel(new GridLayout(1, 1));
		CaretListenerLabel caretListenerLabel = new CaretListenerLabel("");
		statusPanel.add(caretListenerLabel);

		/* Create the Tabs */
		mainTabs = new MainTabs(this, caretListenerLabel);

		/* Create the Menu Bar and Menus */
		menuBar = new MainMenu(this, mainTabs);

		/* Pack and open GUI */
		psodaFrame.setJMenuBar(menuBar);
		psodaFrame.add(mainTabs, BorderLayout.CENTER);
		psodaFrame.add(caretListenerLabel, BorderLayout.PAGE_END);
		psodaFrame.pack();
		psodaFrame.setVisible(true);
	}

	private void setupFileChooser() {
		fileChooser = new JFileChooser("Choose a file to open");

        /* Comment this out. Don't automatically open in the data directory */
		File dataDirectory = new File("../data");
		if (dataDirectory.exists() && dataDirectory.isDirectory()) {
			fileChooser.setCurrentDirectory(dataDirectory);
		}

		FileFilter treeFilter = new FileFilter() {

			public boolean accept(File file) {
				return (file != null && (file.isDirectory() || file.getName()
						.endsWith(".tre")));
			}

			public String getDescription() {
				return "Tree files";
			}

		};
		FileFilter nexusFilter = new FileFilter() {

			public boolean accept(File file) {
				return (file != null && (file.isDirectory() || file.getName()
						.endsWith(".nex")));
			}

			public String getDescription() {
				return "NEXUS files";
			}

		};
		FileFilter fastaFilter = new FileFilter() {

			public boolean accept(File file) {
				return (file != null && (file.isDirectory() || file.getName()
						.endsWith(".fasta")));
			}

			public String getDescription() {
				return "FASTA files";
			}

		};

		fileChooser.addChoosableFileFilter(treeFilter);
		fileChooser.addChoosableFileFilter(nexusFilter);
		fileChooser.addChoosableFileFilter(fastaFilter);
	}

	public String guiFindFile() {
		JFileChooser myChooser = getFileChooser();
		if (myChooser.showOpenDialog(getPsodaFrame()) == JFileChooser.APPROVE_OPTION) {
			File file = myChooser.getSelectedFile();
			if (file != null) {
				try {
					return file.getCanonicalPath();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return "";
	}

	public boolean runInterpreterWithFile(String fileName) {
		if (fileName != null && !fileName.equals("")) {
			date = new Date();
// If it isnt a nexus file, just run loaddata
			if(!fileName.substring(fileName.length()-4,fileName.length()).equals(".nex")) {
				GuiCommandNode model;
				model = new GuiCommandNode();
				model.populateNodeWithCommandDefaults("loaddata");
				model.setParameter("file", fileName);
				model.execute();
			} else {
				long vmsize = Runtime.getRuntime().maxMemory();
				PSODA.getInstance().initInterpreter(vmsize);
				PSODA.getInstance().interpretFile(fileName);
			}
			return true;
		} else {
			return false;
		}
	}

	/**
	 * format determines which file format to save the trees in Newick
	 * Nexus/PAUP
	 */
	public boolean saveCurrentTrees(String fileName, String format) {
		if (!saveTrees(fileName, format)) {
			return false;
		}
		return true;
	}

	/**
	 * format determines which file format to save the trees in Newick
	 * Nexus/PAUP, but Nexus is the only one currently implemented
	 */
	public boolean saveCurrentAlign(String fileName, String format) {
		if (!saveAlign(fileName, format)) {
			return false;
		}
		return true;
	}

	/**
	 * Will write the output text from PSODA to the "PSODA Output" tab Also
	 * updates the tree list while it is at it
	 */
	public void writeStringToGui(String s) {
		mainTabs.writePSODAOutput(s);
		String[] treeList = getTreesInRepository();
		setTreeList(treeList);
	}

	/**
	 * Will open the consensus tree in ATV
	 * 
	 * Parameter: A String representing the consensus tree Return Value: NONE
	 */
	public void openConsensusTreeInATV(String treeString) {
		try {
			String newTreeString = getTaxaNames(treeString);
			Tree tree = new Tree(newTreeString);
			ATVjframe atvframe = new ATVjframe(tree);
			atvframe.showWhole();
		} catch (Exception exp) {
			exp.printStackTrace();
			System.err.println("Could not read tree. Terminating.");
		}
	}

	public void openDataConvertFile(String path) {
		mainTabs.openDataConvertFile(path);
	}

	/**
	 * Writes open tree to a file which can be read in by TreeSAAP
	 */
	public void writeTreeSAAP(String filename) {
		writeTreeSAAPPSODA(filename);
	}

	/**
	 * Will set the running boolean to the value passed in
	 * 
	 * Parameter: value -- true if psoda is running, false if not Return Value:
	 * NONE
	 */
	public void setIsRunning(boolean value) {
		running = value;
	}

	/**
	 * Will update the treeList with current tree repository
	 */
	public void setTreeList(String[] trees) {
		mainTabs.setTreeList(trees);
	}

	/**
	 * Will return whether psoda is running
	 * 
	 * Parameter: NONE Return Value: A boolean
	 */
	public boolean getIsRunning() {
		return running;
	}

	public int getRunLength() {
		Date tmpDate = new Date();
		int length = (int) ((tmpDate.getTime() - date.getTime()) / 1000);
		return length;
	}

	public String[] getTreesInRepository() {
		String[] treeList = getTrees();
		if (treeList == null) {
			return null;
		} else if (treeList.length == 0) {
			return null;
		}
		return treeList;
	}

	public JFileChooser getFileChooser() {
		return fileChooser;
	}

	public MainMenu getMenuBar() {
		return menuBar;
	}

	public JFrame getPsodaFrame() {
		return psodaFrame;
	}

	public MainTabs getPsodaTabs() {
		return mainTabs;
	}

	/**
	 * This method will take the newick tree string (represented by numbers) and
	 * replace the numbers with the actual taxa names
	 * 
	 * Parameter: A String reprensenting the tree in newick format Return Value:
	 * A String representing the tree in newick with taxa names
	 */
	public String getTaxaNames(String newick) {
		String[] taxaNames = getTaxaNames();
		String treeString = new String();
		char ch;

		for (int i = 0; i < newick.length(); i++) {
			ch = newick.charAt(i);
			if (Character.isDigit(ch)) {
				String tmpString = new String();
				while (Character.isDigit(ch)) {
					tmpString += Character.toString(ch);
					ch = newick.charAt(++i);
				}
				i--;
				int taxaNum = Integer.parseInt(tmpString);
				treeString += taxaNames[taxaNum - 1];
			} else if (ch == ':') {
				String branchLength = "";
				while (Character.isDigit(ch) || ch == ':' || ch == '.') {
					branchLength += Character.toString(ch);
					ch = newick.charAt(++i);
				}
				i--;
				treeString += branchLength;
			} else {
				treeString += Character.toString(ch);
			}
		}
		// System.out.println("**treeString**: " + treeString);
		return treeString += ";";
	}

	/**
	 * Listens for and reports caret movements.
	 */
	protected class CaretListenerLabel extends JLabel implements CaretListener {

		private static final long serialVersionUID = -7010655128826618290L;

		public CaretListenerLabel(String label) {
			super(label);
		}

		// Might not be invoked from the event dispatch thread.
		public void caretUpdate(CaretEvent e) {
			displaySelectionInfo(e.getDot(), e.getMark());
		}

		// This method can be invoked from any thread. It
		// invokes the setText and modelToView methods, which
		// must run on the event dispatch thread. We use
		// invokeLater to schedule the code for execution
		// on the event dispatch thread.
		protected void displaySelectionInfo(final int dot, final int mark) {
			SwingUtilities.invokeLater(new Runnable() {
				public void run() {
					if (dot == mark) { // no selection
						try {
							JScrollPane tmpPane = mainTabs
									.getInputTabsSelectedComponent();
							JTextArea textPane = (JTextArea) tmpPane
									.getViewport().getView();
							Rectangle caretCoords = textPane.modelToView(dot);
							// Convert it to view coordinates.
							setText("text position: " + dot
									+ ", [row, column] = ["
									+ ((caretCoords.y / 11) + 1) + ", "
									+ ((caretCoords.x / 6) + 1) + "]" + "\n");
						} catch (BadLocationException ble) {
							setText("text position: " + dot + "\n");
						}
					} else if (dot < mark) {
						setText("selection from: " + dot + " to " + mark + "\n");
					} else {
						setText("selection from: " + mark + " to " + dot + "\n");
					}
				}
			});
		}
	}

	/**
	 * Will create a new PSODA object Parameters: NONE
	 */
	public static void main(String[] args) {
		System.loadLibrary("PSODA");
		long vmsize = Runtime.getRuntime().maxMemory();
		PSODA.getInstance().initInterpreter(vmsize);

		String homeDir = System.getProperty("user.home");
		File registrationFile = new File(homeDir + "/.psodaRegistered");
		if (!registrationFile.exists()) {
			new Registration(homeDir);
		}
	}

}
