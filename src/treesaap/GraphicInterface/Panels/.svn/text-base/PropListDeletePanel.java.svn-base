/**
 *	PropListDeletePanel.class is an instantiated object that creates
 *	a JPanel containing all the delete options of property lists
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.util.Vector;
import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JCheckBox;
import javax.swing.border.BevelBorder;

import treesaap.Data.Data;
import treesaap.Driver.Driver;
import treesaap.Driver.DriverUsageBean;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class PropListDeletePanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public PropListDeletePanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public PropListDeletePanel(Object driver, ActionControl act)
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
		setTabName("Delete");
		setIconName("deletePropIcon.gif");
		setToolTip("Delete Property Lists");

		//Method Vars
		Data data = ((Driver)getDriver()).getData();
		
		//Label for Radio Buttons
		addItem(new JLabel("Choose Property Lists :"), endLine, 1, endLine);
 		
		//add lists
		Vector names = data.getPropertyListNames();
		for(int i=0;i<names.size();i++)
			addItem(new JCheckBox((String)names.elementAt(i), false), endLine);
	
		//Buttons
		ToolTipButton delete = new ToolTipButton("Delete", "Delete Property List");
		delete.addActionListener(getAction());
		delete.setActionName("deletePropLists");
		addItem(delete, cont, 2, endLine);

	}//create
	
	
	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{
		//Method Vars
		Vector panelObjects = getPanelObjects();
		DriverUsageBean driverBean = ((Driver)getDriver()).getBean();
		
		try
		{
			//reset all checkboxes
			for(int i=1;i<panelObjects.size()-1;i++)
				((JCheckBox)getItem(i)).setSelected(false);
		}
		catch(Exception e)
		{
			driverBean.errorMessage("\nThere was a problem while placing user-settings on the Property Delete List Panel.");
		}
	}//setValues

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
 		//Method Vars
		Vector panelObjects = getPanelObjects();
		Data data = ((Driver)getDriver()).getData();
		DriverUsageBean driverBean = ((Driver)getDriver()).getBean();
		
		//set a new property list
		data.getBean().setDeletePropLists(new Vector());
		
		try
		{
			//Check Boxes - go through vector of them
			for(int i=1;i<panelObjects.size()-1;i++)
			{
				if(((JCheckBox)getItem(i)).isSelected())
					data.getBean().getDeletePropLists().add(((JCheckBox)getItem(i)).getText());
			}	
		}
		catch(Exception e)
		{
			driverBean.errorMessage("\nThere was a problem while obtaining user-settings on the Property Delete List Panel.");
		}
	}//getValues

}//PropListDeletePanel