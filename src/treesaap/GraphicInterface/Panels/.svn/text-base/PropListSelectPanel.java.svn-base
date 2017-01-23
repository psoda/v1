/**
 *	PropListSelectPanel.class is an instantiated object that creates
 *	a JPanel containing all the options of property lists
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;
	
import java.util.Vector;
import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.ButtonGroup;
import javax.swing.JRadioButton;
import javax.swing.border.BevelBorder;

import treesaap.Data.Data;
import treesaap.Driver.Driver;
import treesaap.Driver.DriverUsageBean;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;

public class PropListSelectPanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public PropListSelectPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public PropListSelectPanel(Object driver, ActionControl act)
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
		setTabName("Select");
		setIconName("selectPropIcon.gif");
		setToolTip("Select Properties");

		//Method Vars
		Data data = ((Driver)getDriver()).getData();
		
		//Label for Radio Buttons
		addItem(new JLabel("Choose Property List :"), endLine, 1, endLine);
		
		//Radio Buttons
		String name;
		JRadioButton jb;
		ButtonGroup props = new ButtonGroup();
		
		//all properties radio button
		jb = new JRadioButton("Use all Amino Acid Properties");
		jb.setSelected(true);
		props.add(jb);
		addItem(jb, endLine, 1, cont);
		
		//go through all the open property lists
		for(int i=0;i<data.getPropertyListNames().size();i++)
		{
			name = (String)data.getPropertyListNames().elementAt(i);
			jb = new JRadioButton(name);
			props.add(jb);
			addItem(jb, endLine, 1, cont);
		}

	}//createObjects
	
	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		//Method Vars
		Vector panelObjects = getPanelObjects();
		Data data = ((Driver)getDriver()).getData();
		DriverUsageBean driverBean = ((Driver)getDriver()).getBean();
		
		try
		{
			//Lists
			JRadioButton jb;
			for(int i=1;i<panelObjects.size();i++)
			{
				jb = (JRadioButton)getItem(i);
				if(jb.getText().equals(data.getProperties()) || (jb.getText().equals("Use all Amino Acid Properties") && driverBean.getProperties().equals("all")))
					jb.setSelected(true);
			}
		}
		catch(Exception e)
		{
			driverBean.errorMessage("\nThere was a problem while placing user-settings on the Property Select Panel.");
		}
		
	}//setValues

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars
		Vector panelObjects = getPanelObjects();
		DriverUsageBean driverBean = ((Driver)getDriver()).getBean();

		try
		{
			//Property List
			JRadioButton jb;
			for(int i=1;i<panelObjects.size();i++)
			{
				jb = (JRadioButton)getItem(i);
				if(jb.isSelected())
					if(jb.getText().equals("Use all Amino Acid Properties"))
						driverBean.setProperties("all");
					else
						driverBean.setProperties(jb.getText());
			}
		}
		catch(Exception e)
		{
			driverBean.errorMessage("\nThere was a problem while reading values from Property Select Panel.");
		}
		
	}//getValues

}//PropListSelectPanel