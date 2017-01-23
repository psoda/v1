/**
 *	RunTreePanel.class is an instantiated object that creates
 *	a JPanel containing choice of trees
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.util.Vector;
import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JTextArea;
import javax.swing.JCheckBox;

import treesaap.Driver.Driver;
import treesaap.GeneralDNAFileParser.TreeBean;
import treesaap.GeneralDNAFileParser.GeneralDNAFileParser;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class RunTreePanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public RunTreePanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public RunTreePanel(Object driver, ActionControl act)
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
		setBackground(Color.white);

		//Method Vars
		String name;
		TreeBean tree;
		ToolTipButton button;
		GeneralDNAFileParser fileParser = ((Driver)getDriver()).getGDFP();
	
		//Check for Valid data
		Vector names = fileParser.getTreeNames();
		if(names == null || names.size() == 0)
		{
			addItem(new JLabel("  There are no trees open to run"), endLine, 1, endLine);
		}
		
		//Cycle through names
		else
		{
			//TCS
			addItem(new JLabel(" Tree Options:"), endLine, 1, endLine);
			addItem(new JCheckBox(" Use Selected Edges from TCS", false), endLine, 1, cont);
			addItem(new JTextArea("",1,5), endLine);
	
			//Run through trees
			for(int i=0;i<names.size();i++)
			{
				//get tree
				name = (String)names.elementAt(i);
				tree = (TreeBean)fileParser.getTrees().get(name);
				
				//validate tree
				if(tree.getTaxaNames().size() != 0)
				{
					//add button label
					button = new ToolTipButton("Run", "Run Tree");	
					button.addActionListener(getAction());
					button.setActionName("run("+ name +")");
					addItem(button, cont);
					addItem(new JLabel("  "+ name), endLine);
				}			
			}
		}
	}//create
	
	
	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		//Method Vars
		Vector panelObjects = getPanelObjects();
		Driver driver = (Driver)getDriver();
		
		if(panelObjects.size() > 1)
			((JCheckBox)getItem(1)).setSelected(driver.getTree().getBean().getSelectedTCSEdges());
		
		
	}//setValues

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars
		Vector panelObjects = getPanelObjects();
		Driver driver = (Driver)getDriver();
			
		if(panelObjects.size() > 1)
			driver.getTree().getBean().setSelectedTCSEdges(((JCheckBox)getItem(1)).isSelected());
		
	}//getValues

}//RunTreePanel