/**
 *	ActionControl.class is an instantiated object that determines
 *	what actions are to be taken depending on the developer and the 
 *	given super class.
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface;

import DataConvert.GraphicInterface.Objects.ActionMenuItem;
import DataConvert.GraphicInterface.Objects.ToolTipButton;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ActionControl implements ActionListener
{
	//Class Variables - Created Objects
	private GraphicInterface gui;		//The Controller GUI

	
	/**
	 *	ActionControl Constructor - Instantiates Class Objects.	
	 *
	 *	@param Object Driver - the driver class, must have access
	 *	@param GraphicInterfaceUsageBean bean - class created with this bean
	 */
	public ActionControl(GraphicInterface GUI)
	{
		gui = GUI;
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
		
		//get source of action
		Object source = event.getSource();	
		
		//Try each type of object until find correct one, to get the name of it
		try{
			actionName = ((ActionMenuItem)source).getActionName();
		}catch(Exception e){}
		try{
			actionName = ((ToolTipButton)source).getActionName();
		}catch(Exception e){}

		gui.command(actionName);
	}

}//ActionControl
