package gui;

import java.awt.*;
import javax.swing.*;
import javax.swing.Action;
import javax.swing.event.CaretListener;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.ImageIcon;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.text.*;

import treesaap.GraphicInterface.Panels.MainPanel5;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Font;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import java.util.HashMap;
import java.util.Map;

import jalview.gui.Desktop;
import jalview.bin.Cache;

public class MainTabs extends JTabbedPane {

	private static final long serialVersionUID = 573713039863877451L;

	private static MyUndoableEditListener undoableListener;

	private static String inputTabbyTitle = "File Editor";

	public static final int INPUT_FILE = 1;

	public static final int TREESAAP = 3;
	public static final int JALVIEW = 4;

	/**
	 * A reference to the entire gui
	 */
	private PSODA gui;
	
	private UndoRedo undoRedo;

	private HashMap<Object, Action> actions;

	private CaretListener caretListenerLabel;
	
	/**
	 * A reference to this main set of tabs
	 */
	private JTabbedPane mainTabs;

	private JPanel aboutPanel;
	private JTabbedPane fileEditor;
	private PsodaPanel psodaPanel;
	private MainPanel5 treesaapPanel;
	private Desktop jalviewPanel;
	//public Desktop GetJalviewPanel()
	//{
	//	return jalviewPanel;
	//}

	public MainTabs(PSODA inGui, CaretListener caretListenerLabel) {
		gui = inGui;
		mainTabs = this;
		this.caretListenerLabel = caretListenerLabel;
		this.setSize(400,400);

		/* Create Components for tabs */
		fileEditor = new JTabbedPane(JTabbedPane.BOTTOM);
		JTextArea inputFileText = new JTextArea();
		JScrollPane inputScrollPane = new JScrollPane(inputFileText);

		ImageIcon soda = new ImageIcon(getClass().getResource(
				"Psoda.jpg"));
		JLabel background = new JLabel(soda);
		aboutPanel = new JPanel(new BorderLayout());
		undoRedo = new UndoRedo(this);

		addTabToInputTab("Untitled*", inputScrollPane);
		aboutPanel.add(background);
		aboutPanel.setPreferredSize(new Dimension(400,400));

		this.add("About", aboutPanel);
		this.add(inputTabbyTitle + " ("
				+ fileEditor.getTitleAt(fileEditor.getSelectedIndex()) + ")",
				fileEditor);
		fileEditor.addChangeListener(new ChangeListener() {

			public void stateChanged(ChangeEvent arg0) {
				int selectedIndex = fileEditor.getSelectedIndex();
				if (selectedIndex >= 0
						&& selectedIndex < fileEditor.getTabCount()) {
					mainTabs.setTitleAt(INPUT_FILE, inputTabbyTitle
							+ " ("
							+ fileEditor.getTitleAt(fileEditor
									.getSelectedIndex()) + ")");
				} else {
					mainTabs.setTitleAt(INPUT_FILE, inputTabbyTitle
							+ " (<no file>)");
				}

			}

		});

		psodaPanel = new PsodaPanel(gui, this);
		this.add("PSODA", psodaPanel);
		try
		{
			Cache.initLogger();
		} catch (java.lang.NoClassDefFoundError error)
		{
			error.printStackTrace();
			System.out.println("\nEssential logging libraries not found: Error on Daniel Wilcox' part.");
			System.exit(0);
		}
		jalviewPanel = new Desktop();
		jalviewPanel.discoverer.start();
		//jalviewPanel.setLayout(new BorderLayout());
		jalviewPanel.add(jalviewPanel.GetDesktopMenuBar(), BorderLayout.NORTH);
		//JalviewPanelAndMenu.add(jalviewPanel);
		//jalviewPanel.add(jalviewPanel.GetDesktopMenuBar());
		//jalviewPanel.setVisible(true);
		treesaapPanel = new MainPanel5();
		this.add("TreeSAAP", treesaapPanel);
		this.add("Jalview", jalviewPanel);

		undoableListener = new MyUndoableEditListener(this, undoRedo);
		setupDocument(inputFileText, "Untitled*", "false", "false");
		createActionTable(inputFileText);

		// Create individual listeners
		this.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				JTabbedPane mainTabby = (JTabbedPane) e.getSource();
				setMainTabsSelectedIndex(mainTabby.getSelectedIndex());
				MainMenu menuBar = gui.getMenuBar();
				if (mainTabby.getSelectedIndex() == INPUT_FILE
						&& getInputTabsTabCount() != 0) {
					menuBar.setSaveFileMenuItemEnabled(true);
					menuBar.setSaveAsFileMenuItemEnabled(true);
					menuBar.setCloseFileMenuItemEnabled(true);
				} else if (mainTabby.getSelectedIndex() == TREESAAP
						&& getInputTabsTabCount() != 0) {
					menuBar.setSaveFileMenuItemEnabled(false);
					menuBar.setSaveAsFileMenuItemEnabled(false);
					menuBar.setCloseFileMenuItemEnabled(false);
				} else if (mainTabby.getSelectedComponent() == psodaPanel) {
					psodaPanel.giveFocusToInputField();
				} else {
					menuBar.setSaveFileMenuItemEnabled(false);
					menuBar.setSaveAsFileMenuItemEnabled(false);
					menuBar.setCloseFileMenuItemEnabled(false);
				}
			}
		});
		fileEditor.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				// JTabbedPane inputTabby = (JTabbedPane)e.getSource();
				int inputTabbysSelectedTab = getInputTabsSelectedIndex();
				setInputTabsSelectedIndex(inputTabbysSelectedTab);
				if (inputTabbysSelectedTab != -1) { // If the selected tab is
													// NOT -1, update the Edit
													// Menu's Undo and Redo
													// states.
					undoRedo.updateUndoState(); // Must check for -1 in the case
												// where the last tab is closed.
					undoRedo.updateRedoState();
				}
			}
		});

	}

	// ////////////////////////////////////
	// ////////// GET METHODS /////////////
	// ////////////////////////////////////

	public Action getActionByName(String name) {
		return actions.get(name);
	}

	/*
	 * Will return the UndoableEditListener
	 */
	public MyUndoableEditListener getUndoableEditListener() {
		return undoableListener;
	}

	/*
	 * This will return the UndoRedo object
	 * 
	 * Return Value: The UndoRedo object
	 */
	public UndoRedo getUndoRedo() {
		return undoRedo;
	}

	/*
	 * This will return the title of a given tab within inputTabby
	 * 
	 * Parameter: The tab number of interest Return Value: A String representing
	 * the tab name
	 */
	public String getInputTabsTitleAt(int tabNum) {
		return fileEditor.getTitleAt(tabNum);
	}

	/*
	 * This will return the numerical index of which tab is currently selected
	 * for inputTabby
	 * 
	 * Parameter: NONE Return Value: An int representing which tab is selected
	 */
	public int getInputTabsSelectedIndex() {
		return fileEditor.getSelectedIndex();
	}

	/*
	 * This will return the total number of tabs for inputTabby
	 * 
	 * Parameter: NONE Return Value: An int representing the total tab number
	 */
	public int getInputTabsTabCount() {
		return fileEditor.getTabCount();
	}

	/*
	 * This will return the component of the currently selected tab for
	 * inputTabby. It will always be a JScrollPane
	 * 
	 * Parameter: NONE Return Value: A JScrollPane
	 */
	public JScrollPane getInputTabsSelectedComponent() {
		return (JScrollPane) fileEditor.getSelectedComponent();
	}

	/*
	 * This will return the component of a given tab for inputTabby. It will
	 * always be a JScrollPane
	 * 
	 * Parameter: An int representing the desired tab Return Value: A
	 * JScrollPane
	 */
	private JScrollPane getInputTabsSelectedComponentAt(int i) {
		return (JScrollPane) fileEditor.getComponentAt(i);
	}

	/*
	 * This will return the numerical value representing the tab index matching
	 * a given title in inputTabby.
	 * 
	 * Parameter: NONE Return Value: An int representing the desired tab
	 */
	private int getInputTabsIndexOf(String title) {
		return fileEditor.indexOfTab(title);
	}

	/*
	 * Will get the document of the currently selected tab
	 * 
	 * Return value: The Document of the current tab
	 */
	public Document getInputTabsCurrentDocument() {
		JScrollPane tmpPane = getInputTabsSelectedComponent();
		JTextArea textArea = (JTextArea) tmpPane.getViewport().getView();
		Document doc = textArea.getDocument();
		return doc;
	}

	/*
	 * Will get the path property of the currently selected document (the
	 * dacument in the currently selected tab)
	 * 
	 * Return value: a String of the path property for the current document
	 */
	public String getPathPropertyOfCurrentDoc() {
		Document doc = getInputTabsCurrentDocument();
		return (String) doc.getProperty("path");
	}

	/*
	 * Will return the numerical value of the currently selected index for the
	 * main tab (this)
	 * 
	 * Parameter: NONE Return Value: An int representing the currently selected
	 * tab
	 */
	public int getMainTabsSelectedIndex() {
		return this.getSelectedIndex();
	}

	// ////////////////////////////////////
	// ////////// SET METHODS /////////////
	// ////////////////////////////////////

	/*
	 * Will set the title for a given tab. Currently this is used for adding the
	 * '*' to an inputTabby tab when the file has been modified
	 * 
	 * Parameter: An int representing the tab, and a String title Return Value:
	 * NONE
	 */
	private void setTitleOfInputTabAt(int index, String title) {
		fileEditor.setTitleAt(index, title);
	}

	/*
	 * Will set the title for a given tab. Currently this is used for adding the
	 * current score for the tree repository to the 'Trees' tab.
	 * 
	 * Parameter: An int representing the tab, and a String title Return Value:
	 * NONE
	 */
	protected void setTitleOfMainTabAt(int index, String title) {
		this.setTitleAt(index, title);
	}

	/*
	 * Will set the selected index for inputTabby
	 * 
	 * Parameter: An int representing the tab Return Value: NONE
	 */
	private void setInputTabsSelectedIndex(int index) {
		fileEditor.setSelectedIndex(index);
	}

	/*
	 * Will set the selected index for the main tab (this)
	 * 
	 * Parameter: An int representing the tab Return Value: NONE
	 */
	public void setMainTabsSelectedIndex(int index) {
		this.setSelectedIndex(index);
	}

	/*
	 * Will set a file as "not saved" visually by putting an astrisk (*) in the
	 * tab title
	 * 
	 * Parameter: The Document that has been modified Return Value: NONE
	 */
	public void setFileNotSaved(Document doc) {
		String fileSaved = (String) doc.getProperty("fileSaved");
		if (fileSaved == "true") {
			doc.putProperty("fileSaved", "false");
			int current = getInputTabsSelectedIndex();
			setTitleOfInputTabAt(current, getInputTabsTitleAt(current) + "*");
		}
	}

	/*
	 * Will set the component for a given tab. Currently this is used for
	 * resetting the graph when 'Run' is clicked
	 * 
	 * Parameter: An int representing the tab, and the Component Return Value:
	 * NONE
	 */
	protected void setMainTabsComponentAt(int index, Component comp) {
		this.setComponentAt(index, comp);
	}

	/*
	 * Will set a file saved by removing the astrisk in the tab title
	 * 
	 * Parameter: The File to be saved Return Value: NONE
	 */
	public void setFileSaved(File fileToSave) {
		Document doc = getInputTabsCurrentDocument();
		// String fileSaved = (String)doc.getProperty("fileSaved");
		// if(fileSaved != "false"){
		doc.putProperty("fileSaved", "true");
		doc.putProperty("hasHardFile", "true");
		setDocPathForContainer(fileToSave.getPath());
		setPathPropertyOfCurrentDoc(fileToSave.getPath());
		setTitleOfInputTabAt(getInputTabsSelectedIndex(), fileToSave.getName());
		// }
	}

	/*
	 * Will set the path property of the currently selected document (the
	 * dacument in the currently selected tab) to the path passed in
	 * 
	 * Parameter: A String of the path to the file Return Value: NONE
	 */
	private void setPathPropertyOfCurrentDoc(String path) {
		Document doc = getInputTabsCurrentDocument();
		doc.putProperty("path", path);
	}

	/*
	 * Will set the path property of the Containter for the currently selected
	 * document (the dacument in the currently selected tab) to the path passed
	 * in
	 * 
	 * Parameter: A String of the path to the file Return Value: NONE
	 */
	private void setDocPathForContainer(String newPath) {
		undoableListener.getContainer(getPathPropertyOfCurrentDoc()).setPath(
				newPath);
	}

	/*
	 * Will update the tree list will all trees in the repository
	 * 
	 * Parameter: A String array of newick trees Return Value: NONE
	 */
	public void setTreeList(String[] trees) {
		psodaPanel.setTreeList(trees);
	}

	// ////////////////////////////////////
	// ////// MISCELLANEOUS METHODS ///////
	// ////////////////////////////////////

	/*
	 * This method will setup any new Document by setting properties.
	 * 
	 * Parameters: (1) The TextPane of interest (which holds the Document) (2)
	 * The file path to the document. In the case that it has not been saved, it
	 * will be given the format 'Untitled' + uniqueNumber. (3) A boolean in
	 * String format saying if the file has been saved. This is is important for
	 * behavior when 'Save' or 'File,' 'Save' are clicked. (4) A boolean in
	 * String format saying if the Document as EVER been saved. This is
	 * important for determining behavior when 'Save' or 'File,' 'Save' are
	 * clicked. When these are clicked, if there is no hardFile, PSODA will
	 * force them to pick a name to save it under. Otherwise, it will not.
	 */
	private void setupDocument(JTextArea inputFileTextArea, String path,
			String fileSaved, String hasHardFile) {
		Font mono = new Font("Monospaced", Font.PLAIN, 10);
		inputFileTextArea.setFont(mono);
		inputFileTextArea.getDocument().addDocumentListener(
				new MyDocumentListener(this));
		inputFileTextArea.getDocument().putProperty("path", path);
		inputFileTextArea.getDocument().putProperty("fileSaved", fileSaved);
		inputFileTextArea.getDocument().putProperty("hasHardFile", hasHardFile);
		inputFileTextArea.getDocument().addUndoableEditListener(
				undoableListener);
		UndoableContainer container = new UndoableContainer(path);
		undoableListener.addContainer(container);
		// addBindings(inputFileTextArea);
		inputFileTextArea.addCaretListener(caretListenerLabel);
	}

	/*
	 * private void addBindings(JTextArea textArea){ InputMap inputMap =
	 * textArea.getInputMap();
	 */
	/*
	 * I took these out because they conflict with other key bindings on Linux
	 * and Windows //Ctrl-b to go backward one character KeyStroke key =
	 * KeyStroke.getKeyStroke(KeyEvent.VK_B, Event.CTRL_MASK); inputMap.put(key,
	 * DefaultEditorKit.backwardAction);
	 * 
	 * //Ctrl-f to go forward one character key =
	 * KeyStroke.getKeyStroke(KeyEvent.VK_F, Event.CTRL_MASK); inputMap.put(key,
	 * DefaultEditorKit.forwardAction);
	 * 
	 * //Ctrl-p to go up one line key = KeyStroke.getKeyStroke(KeyEvent.VK_P,
	 * Event.CTRL_MASK); inputMap.put(key, DefaultEditorKit.upAction);
	 * 
	 * //Ctrl-n to go down one line key = KeyStroke.getKeyStroke(KeyEvent.VK_N,
	 * Event.CTRL_MASK); inputMap.put(key, DefaultEditorKit.downAction);
	 * 
	 * //Ctrl-a to go to beginning of line key =
	 * KeyStroke.getKeyStroke(KeyEvent.VK_A, Event.CTRL_MASK); inputMap.put(key,
	 * DefaultEditorKit.beginLineAction);
	 * 
	 * //Ctrl-e to go to beginning of line key =
	 * KeyStroke.getKeyStroke(KeyEvent.VK_E, Event.CTRL_MASK); inputMap.put(key,
	 * DefaultEditorKit.endLineAction);
	 * 
	 * //Ctrl-Shift-f to go to beginning of next word key =
	 * KeyStroke.getKeyStroke(KeyEvent.VK_F, Event.CTRL_MASK |
	 * Event.SHIFT_MASK); inputMap.put(key, DefaultEditorKit.nextWordAction);
	 * 
	 * //Ctrl-Shift-b to go to beginning of previous word key =
	 * KeyStroke.getKeyStroke(KeyEvent.VK_B, Event.CTRL_MASK |
	 * Event.SHIFT_MASK); inputMap.put(key,
	 * DefaultEditorKit.previousWordAction);
	 */
	/*
	 * }
	 */
	private void createActionTable(JTextComponent textComponent) {
		actions = new HashMap<Object, Action>();
		Action[] actionsArray = textComponent.getActions();
		for (int i = 0; i < actionsArray.length; i++) {
			Action a = actionsArray[i];
			actions.put(a.getValue(Action.NAME), a);
		}
	}

	/*
	 * This method will add a tab to inputTabby
	 * 
	 * Parameters: A string determining the title, and a JScrollPane Return
	 * Value: NONE
	 */
	private void addTabToInputTab(String title, JScrollPane sPane) {
		fileEditor.add(title, sPane);
	}

	/*
	 * Will open the file created by DataConvert
	 * 
	 * Parameter: A String representing the path Return Value: NONE
	 */
	public void openDataConvertFile(String path) {
		File[] fileList = new File[1];
		File file;

		if (path != null) {
			file = new File(path);
			fileList[0] = file;
			openFile(fileList);
		}

	}

	/*
	 * Will determine if the Document within the currently selected tab has been
	 * saved
	 * 
	 * Parameter: NONE Return Value: A boolean
	 */
	public boolean currentDocIsSaved() {
		if (getInputTabsCurrentDocument().getProperty("fileSaved").equals(
				"true")) {
			return true;
		}
		return false;
	}

	/*
	 * Will determine if the Document within the currently selected tab has an
	 * actual file saved on disk
	 * 
	 * Parameter: NONE Return Value: A boolean
	 */
	public boolean currentDocHasHardFile() {
		if (getInputTabsCurrentDocument().getProperty("hasHardFile").equals(
				"true")) {
			return true;
		}
		return false;
	}

	/*
	 * Will check if a tab exists by title
	 * 
	 * Parameter: A File object Return value: A Boolean -- true if file is open
	 * false if not
	 */
	private boolean fileIsOpen(File file) {
		for (int i = 0; i < getInputTabsTabCount(); i++) {
			JScrollPane tmpPane = getInputTabsSelectedComponentAt(i);
			JTextArea textArea = (JTextArea) tmpPane.getViewport().getView();
			Document doc = textArea.getDocument();
			if (doc.getProperty("path").equals(file.getPath())) {
				return true;
			}
		}
		return false;
	}

	/*
	 * Will close the current tab within the "Input" tab if the "Input" tab is
	 * selected. Called when "Close" is pressed.
	 */
	public void closeInputTab(int tab) {
		String path = getPathPropertyOfCurrentDoc();
		// removeInputTabsTabAt(tab);
		fileEditor.remove(tab);
		undoableListener.removeContainer(undoableListener.getContainer(path));
	}

	/*
	 * Will write the output text from PSODA to the "PSODA Output" tab
	 * 
	 * Parameter: A String to be print Return Value: NONE
	 */
	public void writePSODAOutput(String outputString) {
		psodaPanel.writePSODAOutput(outputString);
	}

	static int newCount = 1;

	/*
	 * Will create a new file when "File" "New" is selected
	 * 
	 * Parameter: NONE Return Value: NONE
	 */
	public void createNewFile() {
		createNewFile("");
	}

	/*
	 * Will create a new file with the given text
	 * 
	 * Parameter: A String to be printed to the new file Return Value: NONE
	 */
	public void createNewFile(String text) {
		Font mono = new Font("Monospaced", Font.PLAIN, 10);
		JTextArea inputText = new JTextArea();
		JScrollPane inputPane = new JScrollPane(inputText);
		inputText.setFont(mono);

		setupDocument(inputText, "Untitled" + newCount, "false", "false");

		/*
		 * StyledDocument doc = inputText.getStyledDocument(); try{
		 * doc.insertString(doc.getLength(), text, doc.getStyle("regular")); }
		 * catch (BadLocationException ble) { System.err.println("Couldn't
		 * insert initial text into text pane."); }
		 */

		inputText.append(text);

		int inputTabbyCount = getInputTabsTabCount(); // Count before
														// inserting new tab.
		addTabToInputTab("Untitled-" + newCount + "*", inputPane);
		setMainTabsSelectedIndex(1); // Will set the main Tab's selected
										// index to "Input File"
		setInputTabsSelectedIndex(inputTabbyCount);
		newCount++;
	}

	/*
	 * Will save the file of the currently selected tab if there is a file to
	 * save
	 * 
	 * Parameter: The File to be saved Return Value: NONE
	 */
	public void saveFile(File fileToSave) {
		if (fileToSave.getName().toLowerCase().contains("untitled")
				|| fileToSave.getName().toLowerCase().contains("untitled*")) {
			JOptionPane
					.showMessageDialog(
							null,
							"ERROR: \"Untitled\" is a reserved file name and cannot be used.",
							"Error", JOptionPane.ERROR_MESSAGE);
		} else {
			try {
				BufferedWriter out = new BufferedWriter(new FileWriter(
						fileToSave));
				JScrollPane tmpPane = getInputTabsSelectedComponent();
				JTextArea textArea = (JTextArea) tmpPane.getViewport()
						.getView();
				String text = textArea.getText();
				out.write(text, 0, text.length());
				out.close();
				setFileSaved(fileToSave);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public JTextArea readInFile(File fileToOpen) {

		JTextArea inputText = new JTextArea();

		Font mono = new Font("Monospaced", Font.PLAIN, 10);
		inputText.setFont(mono);

		try {
			BufferedReader in = new BufferedReader(new FileReader(fileToOpen));
			// current = (char)in.read();
			gui.getMenuBar().setCloseFileMenuItemEnabled(true);
			String token = new String();
			// while((token = getNextToken()) != null){
			while ((token = in.readLine()) != null) {
				inputText.append(token + "\n");
			}
		} catch (FileNotFoundException e) {
			gui.getMenuBar().setCloseFileMenuItemEnabled(false);
			e.printStackTrace();
		} catch (IOException e) {
			gui.getMenuBar().setCloseFileMenuItemEnabled(false);
			e.printStackTrace();
		}

		return inputText;
	}

	/*
	 * Will open a file when "File" "Open" or "Open File..." is clicked
	 * 
	 * Parameter: An array of Files to be opened. Currently it is limited to
	 * opening one file at a time because of the choice to use FileDialog Return
	 * Value: NONE
	 */
	public void openFile(File[] filesToOpen) {
		for (int i = 0; i < filesToOpen.length; i++) {
			if (!fileIsOpen(filesToOpen[i])) {
				// TextPane inputText = new TextPane();
				JTextArea inputText = readInFile(filesToOpen[i]);
				JScrollPane inputPane = new JScrollPane(inputText);

				if (getInputTabsTabCount() == 1
						&& getInputTabsIndexOf("Untitled*") != -1) {
					closeInputTab(getInputTabsIndexOf("Untitled*"));
				}

				setupDocument(inputText, filesToOpen[i].getPath(), "true",
						"true");

				addTabToInputTab(filesToOpen[i].getName(), inputPane);
				setMainTabsSelectedIndex(INPUT_FILE); // Will set the main
														// Tab's selected index
														// to "Input File"
				setInputTabsSelectedIndex(getInputTabsTabCount() - 1); // will
																		// set
																		// the
																		// input
																		// tab's
																		// selected
																		// index
																		// to
																		// the
																		// new
																		// file
				gui.getMenuBar().setSaveFileMenuItemEnabled(true);
				gui.getMenuBar().setSaveAsFileMenuItemEnabled(true);
				gui.getMenuBar().setCloseFileMenuItemEnabled(true);
			} else {
				JOptionPane.showMessageDialog(null, "WARNING: "
						+ filesToOpen[i].getPath() + " is already open!",
						"Warning", JOptionPane.WARNING_MESSAGE);
				for (int thisIndex = 0; thisIndex < getInputTabsTabCount(); thisIndex++) {
					if (fileEditor.getTitleAt(thisIndex).equals(
							filesToOpen[i].getName())
							|| fileEditor.getTitleAt(thisIndex).equals(
									filesToOpen[i].getName() + "*")) {
						setMainTabsSelectedIndex(INPUT_FILE); // Will set the
																// main Tab's
																// selected
																// index to
																// "Input File"
						setInputTabsSelectedIndex(i); // will set the input
														// tab's selected index
														// to the new file
					}
				}
			}
		}
	}

	public void loadTreesaapFile(String treeString) {
		Map<String, String> env = System.getenv();
		// First look in psoda_home, then home directory, then current tdirectory
		String psodahome = env.get("PSODA_HOME");
		if (psodahome == null) {
			psodahome = System.getProperty("user.home");
		}
		String psodafile;
		if (psodahome != null) {
			psodafile = psodahome + System.getProperty("file.separator")+"psodaFile.nex";
		} else {
			psodafile = "psodaFile.nex";
		}
		System.out.format("TreeSAAP File %s%n",psodafile);
		File file = new File(psodafile);
		try {
//			System.out.println("Desired tree: " + treeString);

			PSODA.getInstance().writeTreeSAAP(file.getAbsolutePath());
		System.out.format("Calling writeTreeSAAP File %s%n",file.getAbsolutePath());
			BufferedWriter out = new BufferedWriter(new FileWriter(file, true));
			String treeText = "\ntree PSODAtree = [&U] " + treeString + ";";
			treeText += "\nEnd;";
			out.write(treeText);
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.format("Calling TreeSAAP File %s%n",file.getAbsolutePath());
		treesaapPanel.loadFile(file.getAbsolutePath());
	}

	public PsodaPanel getPsodaTabby() {
		return psodaPanel;
	}

	public JTabbedPane getFileEditorTabby() {
		return fileEditor;
	}
}
