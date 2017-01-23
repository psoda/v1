/**
 *	ParserPanel.class is an instantiated object that creates
 *	a JPanel containing all the options of the GeneralDNAFileParser
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JCheckBox;
import javax.swing.JTextField;
import javax.swing.border.BevelBorder;

import treesaap.Driver.Driver;
import treesaap.Driver.DriverPrintUsageBean;
import treesaap.GeneralDNAFileParser.DNAFileParserUsageBean;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class ParserPanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public ParserPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public ParserPanel(Object driver, ActionControl act)
	{
		setAction(act);
		setDriver(driver);
		createObjects();
	}
	
	/**
	 * Creates all the objects to be placed on this panel, places them in panelObjects
	 */
    //@Override
	public void create()
	{
		//set Background
		setBorder(new BevelBorder(BevelBorder.RAISED,Color.lightGray,Color.black));
		setBackground(Color.white);
		setTabName("Parser");
		setIconName("inputIcon.gif");
		setToolTip("Open File Options");
		
		//objects
		addItem(new JCheckBox(" Limit DNA Data to Capitals", true), endLine, 1, endLine);
		addItem(new JCheckBox(" Display Progress Window for Opening Files", true), endLine);
		addItem(new JLabel("  Percentage of Ambiguous Codons may be over Valid Codons (between 0,1) :"), endLine, 1, endLine);
		addItem(new JTextField("0.5",5), endLine, 1, cont);		
		addItem(new JLabel("  Depth of data outputted when reading in a file - see documentation (between 0,7) :"), endLine, 1, endLine);
		addItem(new JLabel("  Location :"), cont);
		JTextField tf = new JTextField("verbose.txt", 35);
		tf.setEditable(false);
		addItem(tf, endLine);
		addItem(new JLabel("  Depth :"), cont);
		addItem(new JTextField("7",5), endLine);
		
		//Category-Totals File
		addItem(new JLabel("  Tree File Output Options :"), endLine, 1, endLine);
		addItem(new JCheckBox(" Print Tree Header to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print Tree's Taxa to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print Ancestral Data to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print Tree's Nodes to file", true), endLine, 1, cont);

		//Buttons
		ToolTipButton save = new ToolTipButton("Save", "Save Settings to File");
		save.addActionListener(getAction());
		save.setActionName("save");
		addItem(save, cont, 2, endLine);
		
		ToolTipButton reset = new ToolTipButton("Reset", "Reset Settings");
		reset.addActionListener(getAction());
		reset.setActionName("reset");
		addItem(reset, endLine);

	}//create

	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		//Method Vars
		DNAFileParserUsageBean parserBean = ((Driver)getDriver()).getGDFP().getBean();
		DriverPrintUsageBean printBean = ((Driver)getDriver()).getPrint().getPrintBean();
		
		try
		{
			//Check Boxes
			((JCheckBox)getItem(0)).setSelected(parserBean.getAllCaps());
			((JCheckBox)getItem(1)).setSelected(parserBean.getProgressWindow());
			((JCheckBox)getItem(10)).setSelected(printBean.getTreeHeader());
			((JCheckBox)getItem(11)).setSelected(printBean.getTreeTaxa());
			((JCheckBox)getItem(12)).setSelected(printBean.getAncestralTree());
			((JCheckBox)getItem(13)).setSelected(printBean.getTreeNodes());

			//Text Fields
			((JTextField)getItem(3)).setText(parserBean.getEvenHanded()+"");
			((JTextField)getItem(6)).setText(parserBean.getVerboseFile());
			((JTextField)getItem(8)).setText(parserBean.getGdfpVerbose()+"");
		}
		catch(Exception e)
		{
			parserBean.errorMessage("\nThere was a problem while placing user-settings on the Parser Panel.");
		}
		
	}//setValues		

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars	
		DNAFileParserUsageBean parserBean = ((Driver)getDriver()).getGDFP().getBean();
		DriverPrintUsageBean printBean = ((Driver)getDriver()).getPrint().getPrintBean();
		
		try
		{
			//Check Boxes
			parserBean.setAllCaps(((JCheckBox)getItem(0)).isSelected());
			parserBean.setProgressWindow(((JCheckBox)getItem(1)).isSelected());
			printBean.setTreeHeader(((JCheckBox)getItem(10)).isSelected());
			printBean.setTreeTaxa(((JCheckBox)getItem(11)).isSelected());
			printBean.setAncestralTree(((JCheckBox)getItem(12)).isSelected());
			printBean.setTreeNodes(((JCheckBox)getItem(13)).isSelected());
			
			//Text Fields
			String value;
			try{
				value = ((JTextField)getItem(3)).getText();
				double num = Double.valueOf(value).doubleValue();
				if(num >= 0 && num <= 1)
					parserBean.setEvenHanded(num);
				else
					throw new Exception();
			}catch(Exception e){
				parserBean.setEvenHanded(.5);
				((JTextField)getItem(3)).setText(.5+"");
			}
			try{
				value = ((JTextField)getItem(8)).getText();
				int num = Integer.valueOf(value).intValue();
				if(num >= 0 && num <= 7)
					parserBean.setGdfpVerbose(num);
				else
					throw new Exception();
			}catch(Exception e){
				parserBean.setGdfpVerbose(7);
				((JTextField)getItem(8)).setText(7+"");
			}
		}
		catch(Exception e)
		{
			parserBean.errorMessage("\nThere was a problem while reading values from Parser Panel.");
		}
		
	}//getValues
	
}//ParserPanel