/**
 *	RunPanel.class is an instantiated object that creates
 *	a JPanel containing all the run options and choice of trees
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.awt.Color;
import java.awt.BorderLayout;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.border.BevelBorder;

import treesaap.Driver.Driver;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;

public class RunPanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public RunPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public RunPanel(Object driver, ActionControl act)
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
		setTabName("Run");
		setIconName("runIcon.gif");
		setToolTip("Execution Options");
		
		//Method Vars
		RunSettingsPanel left = new RunSettingsPanel(getDriver(), getAction());
		RunTreePanel right = new RunTreePanel(getDriver(), getAction());
		
		//set background
		left.setBackground(Color.white);
		right.setBackground(Color.white);
		
		//SplitPane
		addItem(new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, true, left, right), endLine);
		
		//place the objects on the panel
		place();

	}//create

	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		//Method Vars
		Driver driver = (Driver)getDriver();
		
		try
		{
			((ActionPanel)((JSplitPane)getItem(0)).getLeftComponent()).setValues();
			((ActionPanel)((JSplitPane)getItem(0)).getRightComponent()).setValues();
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the Run Panel.");
		}
		
	}//setValues

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars
		Driver driver = (Driver)getDriver();

		try
		{
			((ActionPanel)((JSplitPane)getItem(0)).getLeftComponent()).getValues();
			((ActionPanel)((JSplitPane)getItem(0)).getRightComponent()).getValues();
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while reading values from Run Panel.");
		}
		
	}//getValues
	
	/**
	 * Places all objects onto splitFrame
	 */
	public void place()
	{			
		//Method Vars
		Driver driver = (Driver)getDriver();
		
		try
		{
			RunTreePanel right = new RunTreePanel(getDriver(), getAction());
			right.setBackground(Color.white);
			
			((JSplitPane)getItem(0)).setRightComponent(right);
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the Run Panel.");
		}
		
	}//place

}//TreePanel