/**
 *	OperationPanel.class is an instantiated object that creates
 *	a JPanel containing all the options of this program
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
import treesaap.Driver.DriverUsageBean;
import treesaap.Driver.DriverPrintUsageBean;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class OperationPanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public OperationPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public OperationPanel(Object driver, ActionControl act)
	{
		setAction(act);
		setDriver(driver);
		createObjects();
	}
	
	/**
	 * Creates all the objects to be placed on this panel, places them in panelObjects
	 */
	public void create()
	{
		//set Background
		setBorder(new BevelBorder(BevelBorder.RAISED,Color.lightGray,Color.black));
		setBackground(Color.white);
		setTabName("Operational");
		setIconName("operationIcon.gif");
		setToolTip("Program Operation Options");
		
		//Method Vars
		JTextField fileName;

		//Output Directory & Button
		addItem(new JLabel("  Output Directory :"), endLine, 1, endLine);
		addItem(new JTextField("./Output", 45), cont);		
		ToolTipButton browse = new ToolTipButton("Browse", "Browse for folder");
		browse.addActionListener(getAction());
		browse.setActionName("browse");
		addItem(browse, endLine);
		
		//Tree Folder
		addItem(new JCheckBox(" Create a Tree Folder for output at direcory above", true), endLine);
	
		//Tree File Options
		addItem(new JLabel(" Print Tree Files:"), endLine, 1, endLine);
		addItem(new JCheckBox(" Tree Information File", true), cont);
		fileName = new JTextField("(TreeName).txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);		
		addItem(new JCheckBox(" TCS Tree File", true), cont);
		fileName = new JTextField("tree.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		
		//Data File Options
		addItem(new JLabel(" Print Data Files:"), endLine, 1, endLine);
		addItem(new JCheckBox(" Genetic Codes", false), cont);
		fileName = new JTextField("Data/GeneticCodes.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);		
		addItem(new JCheckBox(" Properties", false), cont);
		fileName = new JTextField("Data/Properties.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Z-Scores", false), cont);
		fileName = new JTextField("Data/ZScores.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" GF-Scores", false), cont);
		fileName = new JTextField("Data/GFScores.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Pathways", false), cont);
		fileName = new JTextField("Data/Pathways.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);
		
		//Substs File Options
		addItem(new JLabel(" Print Substs Files:"), endLine, 1, endLine);
		addItem(new JCheckBox(" Synonymous Substitutions", true), cont);
		fileName = new JTextField("Substs/SynSubs.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Nonsynonymous Substitutions", true), cont);
		fileName = new JTextField("Substs/NSynSubs.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Ambiguous Codons", true), cont);
		fileName = new JTextField("Substs/Ambig.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);
		addItem(new JCheckBox(" Stop Codons", true), cont);
		fileName = new JTextField("Substs/Stop.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);
		
		//Evpthwy File Options
		addItem(new JLabel(" Print Evpthwy Files:"), endLine, 1, endLine);
		addItem(new JCheckBox(" Category Totals, Individual Files in Directory", false), cont);
		fileName = new JTextField("Evpthwy/Totals/",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Category Totals File", true), cont);
		fileName = new JTextField("Evpthwy/CatTotals.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Significant Property/Category by Z-Score", true), cont);
		fileName = new JTextField("Evpthwy/Z-SigProp.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Significant Properties by G-Score", true), cont);
		fileName = new JTextField("Evpthwy/G-SigProp.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Significant Properties by GF-Score", true), cont);
		fileName = new JTextField("Evpthwy/GF-SigProp.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Significant Property/Category by Substitution", true), cont);
		fileName = new JTextField("Evpthwy/Subs-SigProp.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Significant Codons", true), cont);
		fileName = new JTextField("Evpthwy/SigCodons.txt",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	
		addItem(new JCheckBox(" Sliding Window Property/Category Z-Score Totals in Directory", true), cont);
		fileName = new JTextField("Evpthwy/SlidingWindow/",20);
		fileName.setEditable(false);
		addItem(fileName, endLine);	

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
		DriverUsageBean driverBean = ((Driver)getDriver()).getBean();
		DriverPrintUsageBean printBean = ((Driver)getDriver()).getPrint().getPrintBean();
		
		try
		{
			//Check Boxes
			((JCheckBox)getItem(3)).setSelected(driverBean.getCreateTreeFolder());
			
			//Text Fields
			((JTextField)getItem(1)).setText(driverBean.getResultsDirectory());
			
			//Print Check Boxes
			((JCheckBox)getItem(5)).setSelected(printBean.getWriteTree());
			((JCheckBox)getItem(7)).setSelected(printBean.getWriteTCSTree());
			((JCheckBox)getItem(10)).setSelected(printBean.getWriteGeneticCodes());
			((JCheckBox)getItem(12)).setSelected(printBean.getWriteProperties());
			((JCheckBox)getItem(14)).setSelected(printBean.getWriteZScores());
			((JCheckBox)getItem(16)).setSelected(printBean.getWriteGFScores());
			((JCheckBox)getItem(18)).setSelected(printBean.getWritePathways());
			((JCheckBox)getItem(21)).setSelected(printBean.getWriteSynonymous());
			((JCheckBox)getItem(23)).setSelected(printBean.getWriteNonsynonymous());
			((JCheckBox)getItem(25)).setSelected(printBean.getWriteAmbiguous());
			((JCheckBox)getItem(27)).setSelected(printBean.getWriteStop());
			((JCheckBox)getItem(30)).setSelected(printBean.getCategoryTotalsFolder());
			((JCheckBox)getItem(32)).setSelected(printBean.getCategoryTotalsFile());
			((JCheckBox)getItem(34)).setSelected(printBean.getSignificantByZScore());
			((JCheckBox)getItem(36)).setSelected(printBean.getSignificantByGScore());
			((JCheckBox)getItem(38)).setSelected(printBean.getSignificantByGFScore());
			((JCheckBox)getItem(40)).setSelected(printBean.getSignificantBySubs());
			((JCheckBox)getItem(42)).setSelected(printBean.getSignificantByCodon());
			((JCheckBox)getItem(44)).setSelected(printBean.getSlidingCatTotals());
	
		}
		catch(Exception e)
		{
			driverBean.errorMessage("\nThere was a problem while placing user-settings on the Operational Panel.");
		}
		
	}//setValues		

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars			
		DriverUsageBean driverBean = ((Driver)getDriver()).getBean();
		DriverPrintUsageBean printBean = ((Driver)getDriver()).getPrint().getPrintBean();
		
		try
		{
			//Check Boxes
			driverBean.setCreateTreeFolder(((JCheckBox)getItem(3)).isSelected());

			//Text Fields
			String value = ((JTextField)getItem(1)).getText();
			driverBean.setResultsDirectory(value);
			if(driverBean.getResultsDirectory().equals(""))
			{
				driverBean.setResultsDirectory("./Output");
				((JTextField)getItem(1)).setText("./Output");
			}
			//set gdfp verbose output directory
			((Driver)getDriver()).getGDFP().getBean().setVerboseFile(driverBean.getResultsDirectory());

			//Print Check Boxes
			printBean.setWriteTree(((JCheckBox)getItem(5)).isSelected());
			printBean.setWriteTCSTree(((JCheckBox)getItem(7)).isSelected());
			printBean.setWriteGeneticCodes(((JCheckBox)getItem(10)).isSelected());
			printBean.setWriteProperties(((JCheckBox)getItem(12)).isSelected());
			printBean.setWriteZScores(((JCheckBox)getItem(14)).isSelected());
			printBean.setWriteGFScores(((JCheckBox)getItem(16)).isSelected());
			printBean.setWritePathways(((JCheckBox)getItem(18)).isSelected());
			printBean.setWriteSynonymous(((JCheckBox)getItem(21)).isSelected());
			printBean.setWriteNonsynonymous(((JCheckBox)getItem(23)).isSelected());
			printBean.setWriteAmbiguous(((JCheckBox)getItem(25)).isSelected());
			printBean.setWriteStop(((JCheckBox)getItem(27)).isSelected());
			printBean.setCategoryTotalsFolder(((JCheckBox)getItem(30)).isSelected());
			printBean.setCategoryTotalsFile(((JCheckBox)getItem(32)).isSelected());
			printBean.setSignificantByZScore(((JCheckBox)getItem(34)).isSelected());
			printBean.setSignificantByGScore(((JCheckBox)getItem(36)).isSelected());
			printBean.setSignificantByGFScore(((JCheckBox)getItem(38)).isSelected());
			printBean.setSignificantBySubs(((JCheckBox)getItem(40)).isSelected());
			printBean.setSignificantByCodon(((JCheckBox)getItem(42)).isSelected());
			printBean.setSlidingCatTotals(((JCheckBox)getItem(44)).isSelected());
		}
		catch(Exception e)
		{
			driverBean.errorMessage("\nThere was a problem while reading values from Operational Panel.");
		}
		
	}//getValues

}//OperationPanel