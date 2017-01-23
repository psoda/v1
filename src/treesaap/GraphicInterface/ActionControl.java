/**
 *	ActionControl.class is an instantiated object that determines
 *	what actions are to be taken depending on the developer and the 
 *	given super class.
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface;

import treesaap.Driver.Driver;
import treesaap.GraphicInterface.Objects.ActionMenuItem;
import treesaap.GraphicInterface.Objects.ToolTipButton;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ActionControl implements ActionListener
{
	//Class Variables - Created Objects
	private Object driver;						//Object of the driver class
	private GraphicInterfaceUsageBean bean;		//Bean created and populated by this class

	
	/**
	 *	ActionControl Constructor - Instantiates Class Objects.	
	 *
	 *	@param Object Driver - the driver class, must have access
	 *	@param GraphicInterfaceUsageBean bean - class created with this bean
	 */
	public ActionControl(Object Driver, GraphicInterfaceUsageBean Bean)
	{
		bean = Bean;	
		driver = Driver;
	}//constructor
	
	/**
	 *	Only called when an action is performed on the JFrame
	 *	Developers should insert code here
	 *
	 * @param ActionEvent event - the event that has occured on the GUI
	 */
	public void actionPerformed(ActionEvent event) 
	{
		//Method Objects
		String actionName = "";
		Driver main = (Driver)driver;
		
		//get source of action
		Object source = event.getSource();	
		
		//Try each type of object until find correct one, to get the name of it
		try{
			actionName = ((ActionMenuItem)source).getActionName();
		}catch(Exception e){}
		try{
			actionName = ((ToolTipButton)source).getActionName();
		}catch(Exception e){}

		//Determine action depending on actionName
		if(actionName.indexOf("display") != -1)
		{
			main.displayObject(actionName.substring(actionName.indexOf("(")+1, actionName.lastIndexOf(")")));
		}
		else if(actionName.indexOf("run") != -1)
		{
			main.run(actionName.substring(actionName.indexOf("(")+1, actionName.lastIndexOf(")")));
		}
		else if(actionName.indexOf("branches") != -1)
		{
			//switch on inputted action
			actionName = actionName.substring(actionName.indexOf("(")+1, actionName.lastIndexOf(")"));
			if(actionName.equals("all"))
				main.pairWiseComparison(true);
			else
				main.pairWiseComparison(false);
		}
		else if(actionName.equals("save"))
		{
			main.saveSettings();
		}
		else if(actionName.equals("reset"))
		{
			main.resetSettings();
		}
		else if(actionName.equals("reload"))
		{
			main.reload();
		}
		else if(actionName.equals("deletePropLists"))
		{
			main.deletePropLists();
		}
		else if(actionName.equals("browse"))
		{
			main.getFolder();
		}
		else if(actionName.equals("openTree"))
		{
			main.openTree();
		}
		else if(actionName.equals("openFile"))
		{
			main.open(false);
		}
		else if(actionName.equals("openNewFile"))
		{
			main.open(true);
		}
		else if(actionName.equals("close"))
		{
			main.close();
		}		
		else if(actionName.equals("exit"))
		{
			System.exit(0);
		}		
		
		
		else
			System.out.println(actionName);
	}

}//ActionControl
