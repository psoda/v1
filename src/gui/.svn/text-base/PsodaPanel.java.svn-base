package gui;

import forester.atv.ATVjframe;
import forester.tree.Tree;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.BoxLayout;
import javax.swing.JComboBox;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.plaf.basic.BasicComboBoxEditor;
import javax.swing.text.Document;

import org.math.plot.Plot3DPanel;

import java.util.*;
import java.io.*;

public class PsodaPanel extends JPanel {

	private static final long serialVersionUID = 6549098954354458787L;

	/**
	 * The index of the trees tab
	 */
	public static final int TREES = 1;

	private boolean graphEnabled;

	/**
	 * Reference to gui
	 */
	private PSODA gui;


	/* Tabs */
	private static JTabbedPane tabbedPane;

	/* Interactive Tab */
	private JPanel psodaInteractionPanel;
	private JScrollPane outputScrollPane;
	private JTextArea outputText;
	private JComboBox inputField;
	
/* Search Visualization Tab */
  private static Plot3DPanel graph;

	/* Trees Tab */
	private JList treeList;
	private PsodaButtons buttonPanel;

	public PsodaPanel(PSODA inGui, MainTabs inTabs) {
		addMouseListener(new MouseListener() {

			public void mouseClicked(MouseEvent arg0) {
				giveFocusToInputField();
			}

			public void mouseEntered(MouseEvent arg0) {	}
			public void mouseExited(MouseEvent arg0) { }
			public void mousePressed(MouseEvent arg0) {	}
			public void mouseReleased(MouseEvent arg0) { }
			
		});
		gui = inGui;
		buttonPanel = new PsodaButtons(inTabs);
		graphEnabled = true;


		setLayout(new BorderLayout());
		add(buttonPanel, BorderLayout.EAST);
		setupTabs();

		giveFocusToInputField();
	}

  /**
   * Will create/reset the tree space graph when "Run" is pressed
   * 
   * Parameter: NONE Return Value: NONE
   */
  public void createNewGraph() {
    graph = new Plot3DPanel();
    tabbedPane.setComponentAt(1, graph);
  }

	public PsodaButtons getButtonPanel() {
		return buttonPanel;
	}

	public Document getDocument() {
		return outputText.getDocument();
	}

	/**
	 * This will return the selected value of the tree list which will be the
	 * newick tree
	 * 
	 * Parameter: NONE Return Value: A String representing the newick tree
	 */
	public String getTreeListsSelectedValue() {
		return (String) treeList.getSelectedValue();
	}

	public void giveFocusToInputField() {
		inputField.requestFocus();
	}

 /**
   * Will set the graphs enable status This is used for determining when new
   * points will be added to the graph
   */
  public void setGraphEnabled(boolean bool) {
    graphEnabled = bool;
  }

	public static void setSelectedTab(int index) {
		tabbedPane.setSelectedIndex(index);
	}

	/**
	 * Will update the tree list will all trees in the repository
	 * 
	 * Parameter: A String array of newick trees Return Value: NONE
	 */
	public void setTreeList(String[] trees) {
		if (trees != null) {
			int score = Integer.parseInt(trees[trees.length - 1]);
			if (score == 0) {
				tabbedPane.setTitleAt(TREES, "Trees (Unscored)");
			} else {
				tabbedPane.setTitleAt(TREES, "Trees (" + score + ")");
			}

			// Remove the score from the list
			trees[trees.length - 1] = null;
			treeList.setListData(trees);
		}
	}

	/**
	 * Will write the output text from PSODA to the "PSODA Output" tab
	 * 
	 * Parameter: A String to be print Return Value: NONE
	 */
	public void writePSODAOutput(String outputString) {
		JScrollBar bar = outputScrollPane.getVerticalScrollBar();
		if (bar != null) {
			bar.setValue(bar.getMaximum());
		}
		outputText.append(outputString);
	}

 /**
   * Will add points to the tree space graph
   * 
   * Parameter: An array of trees Return Value: NONE
   */
  public static void addScatterPlot() {
    int redRGB = Color.RED.getRGB();
    Color red = new Color(redRGB);
		Vector<String> lines = new Vector<String>();


		// Read in the file "Terminal.dat"
		// It should have columns X Y Z Tree
		try {
      BufferedReader in = new BufferedReader(new FileReader("Terminal.dat"));
			String token = new String();
      while ((token = in.readLine()) != null) {
//				System.out.println("\nRead a line =>"+token);
        lines.add(token);
				token = new String();
      }
//			System.out.println("\nsize of lines=>"+lines.size());
			double[][] tmpPlot = new double[lines.size()][3];
			double xval, yval, zval;
			
			for(int i = 0; i < lines.size(); i++) {
				String curline = lines.get(i);
				String[] tokens = curline.split("\\s");
//				System.out.println("\ntoken 0  =>"+tokens[0]);
//				System.out.println("\ntoken 1  =>"+tokens[1]);
//				System.out.println("\ntoken 1  =>"+tokens[2]);
				try {
					xval = Double.valueOf(tokens[0].trim()).doubleValue();
				} catch (NumberFormatException e) {
					xval = 0;
				}
				try {
					yval = Double.valueOf(tokens[1].trim()).doubleValue();
				} catch (NumberFormatException e) {
					yval = 0;
				}
				try {
					zval = Double.valueOf(tokens[2].trim()).doubleValue();
				} catch (NumberFormatException e) {
					zval = 0;
				}
				tmpPlot[i][0] = xval;
				tmpPlot[i][1] = yval;
				tmpPlot[i][2] = zval;
//			  System.out.println("\nadding to scatterplot "+i+"="+xval+" "+yval+" "+zval);
			}
			graph.addScatterPlot("Tree Space", red, tmpPlot);

    } catch (FileNotFoundException e) {
			System.out.println("\nTerminal.dat visualization file not found.  Generate data before visualizing.");
		} catch (IOException e) {
			System.out.println("\nIO Exception.");
    }
  }


	private void setupTabs() {
		tabbedPane = new JTabbedPane(JTabbedPane.BOTTOM);

		setupIteractionTab();
		setupGraphTab();
		setupTreesTab();

		tabbedPane.addChangeListener(new ChangeListener() {

			public void stateChanged(ChangeEvent arg0) {
				if (tabbedPane.getSelectedIndex() == 0) {
					giveFocusToInputField();
				}
			}

		});

		add(tabbedPane, BorderLayout.CENTER);

	}

	private class CommandCode {

		private String command;
		
		public CommandCode(String newCommand) {
			command = newCommand;
		}
		
		public String toString() {
			return command;
		}
	}
	
	private void setupIteractionTab() {
		psodaInteractionPanel = new JPanel();
		psodaInteractionPanel.setLayout(new BoxLayout(psodaInteractionPanel,
				BoxLayout.Y_AXIS));
		outputText = new JTextArea();
		Font mono = new Font("Monospaced", Font.PLAIN, 10);
		outputText.setEditable(false);
		outputText.setFont(mono);
		outputScrollPane = new JScrollPane(outputText);
		psodaInteractionPanel.add(outputScrollPane);
		inputField = new JComboBox();
		inputField.setBackground(Color.white);
		inputField.setEditable(true);
		inputField.setMaximumSize(new Dimension(5000, 25));
		BasicComboBoxEditor commandHistoryEditor = new BasicComboBoxEditor();
		inputField.setEditor(commandHistoryEditor);
		inputField.insertItemAt(new CommandCode(""), 0);
		commandHistoryEditor.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent event) {
				String codeToExecute = inputField.getEditor().getItem().toString(); 
				ExecutePsodaCodeThread runningThread = new ExecutePsodaCodeThread(codeToExecute);
				inputField.insertItemAt(new CommandCode(codeToExecute), 1);
				inputField.setSelectedIndex(0);
				runningThread.start();
			}
			
		});
		
		psodaInteractionPanel.add(inputField);
		tabbedPane.add("Interaction", psodaInteractionPanel);
	}

/**
   * Search Visualization Tab
   */
  private void setupGraphTab() {
    graph = new Plot3DPanel();
    tabbedPane.add("Search Visualization", graph);
  }

	private void setupTreesTab() {
		treeList = new JList();
		JScrollPane treeScrollPane = new JScrollPane(treeList);
		treeList.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {

				/*
				 * If there is a double click on the list, get the newick tree
				 * out and create a TreeViewer
				 */
				if (e.getClickCount() >= 2) {
					int index = treeList.locationToIndex(e.getPoint());
					if (index >= 0) {
						String newickString = getTreeListsSelectedValue();
						String treeString = gui.getTaxaNames(newickString);
						try {
							Tree tree = new Tree(treeString);
							ATVjframe atvframe = new ATVjframe(tree);
							atvframe.showWhole();
						} catch (Exception exp) {
							System.err
									.println("Could not read Tree. Terminating.");
						}
					}
				}
			}
		});
		treeList.addListSelectionListener(new ListSelectionListener() {

			public void valueChanged(ListSelectionEvent arg0) {
				if (treeList != null && treeList.getSelectedValues() != null
						&& treeList.getSelectedValues().length == 1
						&& treeList.getSelectedValue() != null) {
					buttonPanel.setEnableExportToTreeSaap(true);
				} else {
					buttonPanel.setEnableExportToTreeSaap(false);
				}
			}

		});
		tabbedPane.add("Trees", treeScrollPane);
	}

}
