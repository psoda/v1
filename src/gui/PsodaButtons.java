package gui;

import javax.swing.ButtonGroup;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JOptionPane;
import javax.swing.JRadioButton;
import javax.swing.JTabbedPane;
import javax.swing.SpringLayout;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FileDialog;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import java.io.File;

public class PsodaButtons extends JPanel {

	private static final long serialVersionUID = -9039861353970122988L;

	/**
	 * The string to appear at the top of the commands menu
	 */
	private static final String commandListTitle = "Commands";

	/**
	 * The string to appear as a diveder in the commands menu
	 */
	private static final String commandListDivider = "____________";

	/**
	 * A reference to main tabs of the gui
	 */
	private MainTabs tabs;

	/**
	 * A reference to this button panel
	 */
	private PsodaButtons thisPanel;

	/* Buttons */
	private RunOpenFileButton runOpenFileButton;
	private JButton stopButton;
	private JButton pauseButton;
	private JButton saveTreesButton;
	private JButton saveAlignButton;
	private JButton visualizeButton;
	private JButton exportAlignmentButton;
	private JButton exportTreesaapButton;
	private JComboBox commandsMenu;

	public PsodaButtons(MainTabs inTabs) {
		thisPanel = this;
		tabs = inTabs;

		setLayout(new GridBagLayout());

		setupRunOpenFileButton(); 
		setupStopButton();
		setupPauseButton();
		setupVisualizeButton();
		setupExportToTreeSaapButton();
		setupExportToJalviewButton();
		setupSaveCurrentTreesButton();
		setupSaveCurrentAlignButton();
		setupCommandsMenu();

		Dimension biggestSize = saveAlignButton.getPreferredSize();
		Dimension buttonSize = new Dimension(biggestSize.width + 25,
				biggestSize.height);
		saveTreesButton.setPreferredSize(buttonSize);
		saveAlignButton.setPreferredSize(buttonSize);
		runOpenFileButton.setPreferredSize(buttonSize);
		stopButton.setPreferredSize(buttonSize);
		pauseButton.setPreferredSize(buttonSize);
		exportTreesaapButton.setPreferredSize(buttonSize);
		visualizeButton.setPreferredSize(buttonSize);
		exportAlignmentButton.setPreferredSize(buttonSize);
		commandsMenu.setPreferredSize(buttonSize);

		GridBagConstraints buttonConstraints = new GridBagConstraints();
		buttonConstraints.insets.set(10, 10, 10, 10);
		buttonConstraints.gridx = 0;
		buttonConstraints.gridy = GridBagConstraints.RELATIVE;

		add(runOpenFileButton, buttonConstraints); 

		add(commandsMenu, buttonConstraints);
/* Put these up here */
		add(stopButton, buttonConstraints);
		add(pauseButton, buttonConstraints);
		add(saveTreesButton, buttonConstraints);
		add(saveAlignButton, buttonConstraints);
		buttonConstraints.insets.set(100, 10, 10, 10);
		add(exportTreesaapButton, buttonConstraints);
		add(exportAlignmentButton, buttonConstraints);
		add(visualizeButton, buttonConstraints);
		buttonConstraints.insets.set(10, 10, 10, 10);
		add(commandsMenu, buttonConstraints);

		tabs.getFileEditorTabby().addChangeListener(runOpenFileButton);
		runOpenFileButton.updateButton();
	}

	public RunOpenFileButton getRunOpenFileButton() {
		return runOpenFileButton;
	}

// Perform the visualization
	private void setupVisualizeButton()
	{
		visualizeButton = new JButton("Visualize Trees");
		visualizeButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				MainTabs psodaTabs = PSODA.getInstance().getPsodaTabs();
				PsodaPanel psodaTabby = psodaTabs.getPsodaTabby();
				PsodaPanel.addScatterPlot();
				PsodaPanel.setSelectedTab(1);

				// System.out.println("Time: " + tmpPlot[0][1]);
				// System.out.println("Number of Trees: " + tmpPlot[0][0]);
				// System.out.println("Number of Plots: " + numberOfPlots);

			}
		});
	}
	private void setupExportToJalviewButton()
	{
		final String jalviewFileName = "jalViewFile.fasta";
		exportAlignmentButton = new JButton("Export to Jalview");
		exportAlignmentButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				final File jalViewFile = new File(jalviewFileName);
				if (!PSODA.getInstance().saveCurrentAlign(
						jalViewFile.getPath(), "fasta")) {
					JOptionPane
							.showMessageDialog(
									null,
									"IMPORTANT: There are no Alignments !",
									"IMPORTANT",
									JOptionPane.INFORMATION_MESSAGE);
					return;
				}
				jalview.gui.PsodaInterface.loadPsodaFile(jalViewFile.getAbsolutePath());
				if (tabs != null) {
						tabs.setSelectedIndex(MainTabs.JALVIEW);
				}
			}
		});
	}
	private void setupExportToTreeSaapButton() {
		exportTreesaapButton = new JButton("Export to TreeSAAP");
		setEnableExportToTreeSaap(false);
		exportTreesaapButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (tabs != null) {
					String tree = tabs.getPsodaTabby()
							.getTreeListsSelectedValue();
					
					if(tree != null) {
						tabs.loadTreesaapFile(tree);
						tabs.setSelectedIndex(MainTabs.TREESAAP);
					}
				}
			}
		});
	}

	private void setupSaveCurrentTreesButton() {
		saveTreesButton = new JButton("Save Current Trees");
		SpringLayout layout = new SpringLayout();
		final JFrame formatFrame = new JFrame("Format Options");
		final JPanel formatPanel = new JPanel(layout);
		final JLabel formatLabel = new JLabel(
				"Please choose the output format for the trees file:");
		final JRadioButton newickFormat = new JRadioButton("Newick Format",
				true);
		final JRadioButton nexusPaupFormat = new JRadioButton("Nexus/PAUP");
		final JButton saveTreesOKButton = new JButton("OK");
		final JButton saveTreesCancelButton = new JButton("Cancel");

		saveTreesOKButton.setSelected(true);
		ButtonGroup formatGroup = new ButtonGroup();
		formatGroup.add(newickFormat);
		formatGroup.add(nexusPaupFormat);

		formatFrame.setResizable(false);
		formatFrame.setPreferredSize(new Dimension(350, 150));
		formatPanel.add(formatLabel);
		formatPanel.add(newickFormat);
		formatPanel.add(nexusPaupFormat);
		formatPanel.add(saveTreesOKButton);
		formatPanel.add(saveTreesCancelButton);

		layout.putConstraint(SpringLayout.WEST, formatLabel, 5,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, formatLabel, 5,
				SpringLayout.NORTH, formatPanel);

		layout.putConstraint(SpringLayout.WEST, newickFormat, 15,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, newickFormat, 5,
				SpringLayout.SOUTH, formatLabel);

		layout.putConstraint(SpringLayout.WEST, nexusPaupFormat, 15,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, nexusPaupFormat, 5,
				SpringLayout.SOUTH, newickFormat);

		layout.putConstraint(SpringLayout.WEST, saveTreesCancelButton, 15,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, saveTreesCancelButton, 5,
				SpringLayout.SOUTH, nexusPaupFormat);

		layout.putConstraint(SpringLayout.WEST, saveTreesOKButton, 5,
				SpringLayout.EAST, saveTreesCancelButton);
		layout.putConstraint(SpringLayout.NORTH, saveTreesOKButton, 5,
				SpringLayout.SOUTH, nexusPaupFormat);

		formatFrame.add(formatPanel);
		formatFrame.pack();

		saveTreesButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				formatFrame.setVisible(true);
			}
		});
		saveTreesOKButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				PSODA gui = PSODA.getInstance();
				formatFrame.setVisible(false);
				FileDialog fd = new FileDialog(gui.getPsodaFrame(),
						"Save file", FileDialog.SAVE);
				fd.setVisible(true);
				File fileToSave = null;
				String fileName = fd.getFile();
				if (fileName != null) {
					fileName = fd.getDirectory() + fileName;
					fileToSave = new File(fileName);

					String format = "";
					if (newickFormat.isSelected()) {
						format = "newick";
					} else if (nexusPaupFormat.isSelected()) {
						format = "nexus";
					}
					if (!PSODA.getInstance().saveCurrentTrees(
							fileToSave.getPath(), format)) {
						JOptionPane
								.showMessageDialog(
										null,
										"IMPORTANT: There are no trees in the repository!",
										"IMPORTANT",
										JOptionPane.INFORMATION_MESSAGE);
					}
				}
			}
		});
		saveTreesCancelButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				formatFrame.setVisible(false);
			}
		});
	}

	private void setupSaveCurrentAlignButton() {
		saveAlignButton = new JButton("Save Current Alignment");
		SpringLayout layout = new SpringLayout();
		final JFrame formatFrame = new JFrame("Format Options");
		final JPanel formatPanel = new JPanel(layout);
		final JLabel formatLabel = new JLabel(
				"Please choose the output format for the Alignment file:");
		final JRadioButton fastaFormat = new JRadioButton("Fasta Format",
				true);
		final JRadioButton newickFormat = new JRadioButton("Newick Format");
		final JRadioButton nexusPaupFormat = new JRadioButton("Nexus/PAUP");
		final JButton saveAlignOKButton = new JButton("OK");
		final JButton saveAlignCancelButton = new JButton("Cancel");

		saveAlignOKButton.setSelected(true);
		ButtonGroup formatGroup = new ButtonGroup();
		formatGroup.add(newickFormat);
		formatGroup.add(nexusPaupFormat);

		formatFrame.setResizable(false);
		formatFrame.setPreferredSize(new Dimension(400, 250));
		formatPanel.add(formatLabel);
		formatPanel.add(fastaFormat);
		formatPanel.add(newickFormat);
		formatPanel.add(nexusPaupFormat);
		formatPanel.add(saveAlignOKButton);
		formatPanel.add(saveAlignCancelButton);

		layout.putConstraint(SpringLayout.WEST, formatLabel, 5,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, formatLabel, 5,
				SpringLayout.NORTH, formatPanel);

		layout.putConstraint(SpringLayout.WEST, fastaFormat, 15,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, fastaFormat, 5,
				SpringLayout.SOUTH, formatLabel);

		layout.putConstraint(SpringLayout.WEST, newickFormat, 15,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, newickFormat, 5,
				SpringLayout.SOUTH, fastaFormat);

		layout.putConstraint(SpringLayout.WEST, nexusPaupFormat, 15,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, nexusPaupFormat, 5,
				SpringLayout.SOUTH, newickFormat);

		layout.putConstraint(SpringLayout.WEST, saveAlignCancelButton, 15,
				SpringLayout.WEST, formatPanel);
		layout.putConstraint(SpringLayout.NORTH, saveAlignCancelButton, 5,
				SpringLayout.SOUTH, nexusPaupFormat);

		layout.putConstraint(SpringLayout.WEST, saveAlignOKButton, 5,
				SpringLayout.EAST, saveAlignCancelButton);
		layout.putConstraint(SpringLayout.NORTH, saveAlignOKButton, 5,
				SpringLayout.SOUTH, nexusPaupFormat);

		formatFrame.add(formatPanel);
		formatFrame.pack();

		saveAlignButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				formatFrame.setVisible(true);
			}
		});
		saveAlignOKButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				PSODA gui = PSODA.getInstance();
				formatFrame.setVisible(false);
				FileDialog fd = new FileDialog(gui.getPsodaFrame(),
						"Save file", FileDialog.SAVE);
				fd.setVisible(true);
				File fileToSave = null;
				String fileName = fd.getFile();
				if (fileName != null) {
					fileName = fd.getDirectory() + fileName;
					fileToSave = new File(fileName);

					String format = "";
					if (fastaFormat.isSelected()) {
						format = "fasta";
					} else if (newickFormat.isSelected()) {
						format = "newick";
					} else if (nexusPaupFormat.isSelected()) {
						format = "nexus";
					}
					if (!PSODA.getInstance().saveCurrentAlign(
							fileToSave.getPath(), format)) {
						JOptionPane
								.showMessageDialog(
										null,
										"IMPORTANT: There are no Alignments !",
										"IMPORTANT",
										JOptionPane.INFORMATION_MESSAGE);
					}
				}
			}
		});
		saveAlignCancelButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				formatFrame.setVisible(false);
			}
		});
	}

	private void setupPauseButton() {
		pauseButton = new JButton("Pause");
		pauseButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (PSODA.getInstance().getIsRunning()) {
					if (!pauseButton.isSelected()) {
						PSODA.getInstance().pausePSODA();
						setPauseButtonText("Continue");
						setPauseButtonSelected(true);
						setStopButtonEnabled(false);
					} else {
						PSODA.getInstance().resumePSODA();
						setPauseButtonText("Pause");
						setPauseButtonSelected(false);
						setStopButtonEnabled(true);
					}
				}
			}
		});
	}

	private void setupStopButton() {
		stopButton = new JButton("Stop");
		stopButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				PSODA.getInstance().killPSODA();
			}
		});
	}

	private void setupRunOpenFileButton() {
		runOpenFileButton = new RunOpenFileButton();
		runOpenFileButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				MainTabs psodaTabs = PSODA.getInstance().getPsodaTabs();
				PsodaPanel psodaTabby = psodaTabs.getPsodaTabby();
				if (!PSODA.getInstance().getIsRunning()) {
					PSODA.getInstance().setIsRunning(true);
					if (psodaTabs.getInputTabsTabCount() == 0) {
						JOptionPane.showMessageDialog(null,
								"WARNING: There is no open file!", "Warning",
								JOptionPane.WARNING_MESSAGE);
						PSODA.getInstance().setIsRunning(false);
					} else {
						// String tabTitle =
						// psodaTabs.getInputTabsTitleAt(psodaTabs.getInputTabsSelectedIndex());
						if (!psodaTabs.currentDocIsSaved()) {
							JOptionPane.showMessageDialog(null,
									"WARNING: Please save your file first!",
									"Warning", JOptionPane.WARNING_MESSAGE);
							PSODA.getInstance().setIsRunning(false);
						} else {
							String path = psodaTabs
									.getPathPropertyOfCurrentDoc();
							if (path != null && !path.equals("")) {
								psodaTabby.setSelectedTab(0);
								psodaTabby.createNewGraph(); // Will reset the tree space graph
								psodaTabby.setGraphEnabled(true);
								String pathWithQuotes = "\""+path+"\"";
								(new InterpretFileThread(pathWithQuotes)).start();
							} else {
								JOptionPane
										.showMessageDialog(
												null,
												"WARNING: You must choose a valid file!",
												"Warning",
												JOptionPane.WARNING_MESSAGE);
								PSODA.getInstance().setIsRunning(false);
							}
						}
					}
				} else {
					JOptionPane.showMessageDialog(null,
							"WARNING: Only one process can run at a time!",
							"Warning", JOptionPane.WARNING_MESSAGE);
				}
			}
		});
	}

	private void setupCommandsMenu() {
		String[] definedCommandNames = PSODA.getDefinedCommandNames();
		String[] popularCommandNames = PSODA.getPopularCommandNames();

		DefaultComboBoxModel comboBoxModel = new DefaultComboBoxModel();
		comboBoxModel.addElement(commandListTitle);
		comboBoxModel.addElement(commandListDivider);
		for (String thisCommandName : popularCommandNames) {
			comboBoxModel.addElement(thisCommandName);
		}
		comboBoxModel.addElement(commandListDivider);
		for (String thisCommandName : definedCommandNames) {
			comboBoxModel.addElement(thisCommandName);
		}

		commandsMenu = new JComboBox();
		commandsMenu.setModel(comboBoxModel);
		commandsMenu.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent event) {
				if (commandsMenu.getSelectedIndex() > 1) {
					ExecuteCommandFrame options = new ExecuteCommandFrame(
							(String) commandsMenu.getSelectedItem(), thisPanel,
							tabs.getPsodaTabby());
					options.setVisible(true);
					commandsMenu.setSelectedIndex(0);
				}
			}

			public void focusLost(FocusEvent arg0) {
				// do nothing
			}
			
		});
	}

	/**
	 * This method will set the stopButton's enabled status
	 * 
	 * Parameter: A boolean setting the value Return Value: NONE
	 */
	public void setStopButtonEnabled(boolean bool) {
		stopButton.setEnabled(bool);
	}

	/**
	 * This method will set the pauseButton's text. This is used for setting the
	 * pauseButton's text between 'Pause' and 'Continue'
	 * 
	 * Parameter: A boolean setting the value Return Value: NONE
	 */
	public void setPauseButtonText(String text) {
		pauseButton.setText(text);
	}

	/**
	 * This method will set the pauseButton's selected value
	 * 
	 * Parameter: A boolean setting the value Return Value: NONE
	 */
	public void setPauseButtonSelected(boolean bool) {
		pauseButton.setSelected(bool);
	}

	public void setEnableExportToTreeSaap(boolean enabled) {
		exportTreesaapButton.setEnabled(enabled);
		if (enabled) {
			exportTreesaapButton
					.setToolTipText("Exports the selected tree in the \"Trees\" tab to TreeSAAP");
		} else {
			exportTreesaapButton
					.setToolTipText("Disabled because a single tree is not selected on the Trees sub tab");
		}
	}

	/**
	 * CreateNewInterpreter Class December 13, 2006 Will be used to create the
	 * interpreter and extends Thread
	 */
	public class InterpretFileThread implements Runnable {

		/**
		 * The filename to interpret
		 */
		private String filename = "";

		/**
		 * The new thread
		 */
		private Thread thread;

		public void start() {
			thread = new Thread(this);
			thread.start();
		}

		public void run() {
			PSODA.getInstance().runInterpreterWithFile(filename);
			PSODA.getInstance().setIsRunning(false); // This will set
														// 'running' to false
														// when the thread ends
														// //
		}

		public InterpretFileThread(String fileName) {
			this.filename = fileName;
		}
	}


	private class RunOpenFileButton extends JButton implements ChangeListener {

		private static final long serialVersionUID = -7309699734643586288L;

		private static final String preText = "Run ";

		public RunOpenFileButton() {
			super();
		}

		public void updateButton() {
			JTabbedPane openFiles = tabs.getFileEditorTabby();
			if (openFiles != null && openFiles.getSelectedComponent() != null) {
				setEnabled(true);
				setToolTipText("");
				setText(preText + "\""
						+ openFiles.getTitleAt(openFiles.getSelectedIndex())
						+ "\"");
			} else {
				setEnabled(false);
				setToolTipText("Disabled because no input file is selected in the File Editor tab.");
				setText(preText + "<Input File>");
			}
		}

		public void stateChanged(ChangeEvent event) {
			updateButton();
		}

	}

}
