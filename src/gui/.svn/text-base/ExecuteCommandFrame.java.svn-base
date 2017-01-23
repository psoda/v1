package gui;

import java.awt.Dimension;
import java.awt.GridLayout;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JLabel;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;

import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;
import java.util.Set;
import java.util.TreeMap;
import java.util.Map;
import java.util.TreeSet;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.File;

public class ExecuteCommandFrame extends JFrame {

	private static final long serialVersionUID = 6432824127997262081L;

	/**
	 * A reference to this panel
	 */
	private PsodaPanel psodaPanel;

	/**
	 * A data structure for a command node and an interface to the C++ command
	 * description
	 */
	private GuiCommandNode model;

	/**
	 * The panel for the gui components
	 */
	private JPanel optionsPanel;

	/**
	 * The parameter names
	 */
	private Map<String, JLabel> labels;

	/**
	 * A view of the PsodaScript source code representation of the command
	 */
	private CodeView commandCode;

	/**
	 * The restore defaults and start buttons
	 */
	private JPanel buttons;

	private JButton restoreDefaultsButton;

	private JButton startButton;

	public ExecuteCommandFrame(String commandName, PsodaButtons buttonPanel,
			PsodaPanel newPsodaTabby) {
		super(commandName + " Options");
		psodaPanel = newPsodaTabby;
		model = new GuiCommandNode();
		model.populateNodeWithCommandDefaults(commandName);
		labels = new TreeMap<String, JLabel>();
		String[] paramNames = model.getParamNames();
		String[] paramDescriptions = model.getParamDescriptions();
		String[] defaultValues = model.getDefaultValues();
		String[] fileParamNames = model.getFileParamNames();
		Set<String> fileParamsSet = new TreeSet<String>();
		for (String thisFileParamName : fileParamNames) {
			fileParamsSet.add(thisFileParamName);
		}
		
		ArrayList<String[]> options = model.getParamOptions();
		// assert(paramNames.length = defaultValues.length = options.size());
		//begin new stuff

//		GridLayout gl = new GridLayout(

		//BoxLayout bl = new BoxLayout(getContentPane(), BoxLayout.Y_AXIS);
		//setLayout(bl);
		
//		GridLayout gl = new GridLayout(4,4);
//		setLayout(gl);
		
//		/*old stuff:
		setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
//		*/
		setupCommandParamComponents(paramNames, paramDescriptions,
				defaultValues, fileParamsSet, options);
		//setupCommandCodeView();
		setupButtons(commandName);

		setVisible(true);
	} // end SetupOptionsFrame constructor

	private void setupButtons(String commandName) {
		buttons = new JPanel(new GridLayout(1, 2));
		restoreDefaultsButton = new JButton("Restore Defaults");
		restoreDefaultsButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				model.populateNodeWithCommandDefaults();
			}
		});
		buttons.add(restoreDefaultsButton);

		startButton = new JButton("Run \"" + commandName + "\"");
		startButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				psodaPanel.setSelectedTab(0);
                dispose(); // Close the window when you hit the run button
				model.execute();
			}
		});

		buttons.add(startButton);
		buttons.setVisible(true);
		add(buttons);
	}

	private void setupCommandCodeView() {
		commandCode = new CodeView(model.toCode());
		commandCode.setBackground(this.getBackground());
		model.addObserver(commandCode);
		add(commandCode);
	}

	private void setupCommandParamComponents(String[] paramNames,
			String[] paramDescriptions, String[] defaultValues,
			Set<String> fileParamsSet, ArrayList<String[]> options) {
		optionsPanel = new JPanel();
		int numParams = paramNames.length;
		optionsPanel.setLayout(new GridLayout(numParams + 1, 2));
		double lineHeight = 25;
		int prefWidth = 350;
		if (getGraphics() != null) {
			lineHeight = getGraphics().getFontMetrics().getHeight() * 1.3;
		}
		int height = (int) ((numParams + 6) * lineHeight);
		Dimension preferredDim = new Dimension(prefWidth, height);
		setPreferredSize(preferredDim);
		setSize(preferredDim);

		for (int i = 0; i < numParams; i++) {

			String thisParamName = paramNames[i];
			String thisDefaultValue = defaultValues[i];
			String[] theseOptionStrings = options.get(i);

			JLabel thisParamLabel = new JLabel("   "+thisParamName);
			labels.put(thisParamName, thisParamLabel);

			JComponent thisValue;
			// First check for a file type
			if (fileParamsSet.contains(thisParamName)) {
				FileParameterValueComponent newValue = new FileParameterValueComponent(thisParamName);
				model.addObserver(newValue);
				thisValue = newValue;
			} else if (theseOptionStrings.length > 1) {
				// make a drop down box
				OptionsParameterValueComponent newValue = new OptionsParameterValueComponent(
						thisParamName, theseOptionStrings, thisDefaultValue);
				model.addObserver(newValue);
				thisValue = newValue;
			} else {
				SimpleParameterValueComponent newValue = new SimpleParameterValueComponent(
						thisParamName, thisDefaultValue);
				model.addObserver(newValue);
				thisValue = newValue;
			}

			thisParamLabel.setToolTipText(paramDescriptions[i]);
			optionsPanel.add(thisParamLabel);

			thisValue.setToolTipText(paramDescriptions[i]);
			optionsPanel.add(thisValue);
		}

		// Add a space at the bottom (this also keeps the two columns even when there is only one param)
		optionsPanel.add(new JLabel());
		optionsPanel.add(new JLabel());
		optionsPanel.setVisible(true);
		add(optionsPanel);
	}

	private class CodeView extends JTextArea implements Observer {

		private static final long serialVersionUID = -7150410193647936299L;

		public CodeView(String origText) {
			super(origText);
			setEditable(false);
			setBorder(null);
			setLineWrap(true);
			setRows(3);
		}

		public void update(Observable observable, Object arg) {
			setText(model.toCode());
			repaint();
		}

	}

	private class FileParameterValueComponent extends JPanel implements Observer {

		private static final long serialVersionUID = -8385995514712281786L;
		private String paramName;
		private JTextField currentValue;
		private JButton chooseFileButton;
		
		public FileParameterValueComponent(String _paramName) {
			setLayout(new GridLayout(1, 2));
			paramName = _paramName;
			currentValue = new JTextField(paramName);
			currentValue.addCaretListener(new CaretListener() {

				public void caretUpdate(CaretEvent arg0) {
					if (!model.getParameter(paramName).equals(currentValue.getText())) {
						model.setParameter(paramName, currentValue.getText());
					}
				}
				
			});
			
			chooseFileButton = new JButton("File");
			chooseFileButton.addActionListener(new ActionListener() {

				public void actionPerformed(ActionEvent event) {
					PSODA gui = PSODA.getInstance();
					if (gui.getFileChooser().showOpenDialog(optionsPanel) == JFileChooser.APPROVE_OPTION) {
						File file = gui.getFileChooser().getSelectedFile();
						model.setParameter(paramName, "\"" + file.getAbsolutePath() + "\"");
					}
				}
				
			});
			
			add(currentValue);
			add(chooseFileButton);
			update(null, null);
		}
		
		public void update(Observable arg0, Object arg1) {
			if (!model.getParameter(paramName).equals(
					currentValue.getText())) {
				currentValue.setText(model.getParameter(paramName));
			}
		}

	}
	
	
	private class OptionsParameterValueComponent extends JComboBox implements Observer {

		private static final long serialVersionUID = -2284712001722469905L;

		private String paramName;

		public OptionsParameterValueComponent(String _paramName,
				String[] options, String initialSelection) {
			super(options);
			paramName = _paramName;
			setSelectedItem(initialSelection);
			setEditable(true);
			addItemListener(new ItemListener() {

				public void itemStateChanged(ItemEvent event) {
					if (!model.getParameter(paramName).equals(
							(String) getSelectedItem())) {
						model.setParameter(paramName,
								(String) getSelectedItem());
					}
				}

			});
		}

		public void update(Observable observable, Object arg) {
			if (!model.getParameter(paramName).equals(
					(String) getSelectedItem())) {
				setSelectedItem(model.getParameter(paramName));
			}
		}

	}

	private class SimpleParameterValueComponent extends JTextField implements Observer {

		private static final long serialVersionUID = -6054949509535173138L;

		private String paramName;

		public SimpleParameterValueComponent(String _paramName,
				String initialValue) {
			super(initialValue);
			paramName = _paramName;
			setEditable(true);
			addCaretListener(new CaretListener() {

				public void caretUpdate(CaretEvent event) {
					if (!model.getParameter(paramName).equals(getText())) {
						model.setParameter(paramName, getText());
					}
				}

			});
		}

		public void update(Observable observable, Object arg) {
			if (!model.getParameter(paramName).equals(getText())) {
				setText(model.getParameter(paramName));
			}
		}

	}

}
