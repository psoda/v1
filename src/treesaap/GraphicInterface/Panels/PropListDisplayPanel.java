/**
 *	PropListDisplayPanel.class is an instantiated object that creates
 *	a JPanel containing all the propertyList as option
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.util.Vector;
import java.awt.Color;
import java.awt.BorderLayout;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JTextArea;
import javax.swing.JSplitPane;
import javax.swing.border.BevelBorder;

import treesaap.Data.Data;
import treesaap.Driver.Driver;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class PropListDisplayPanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public PropListDisplayPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public PropListDisplayPanel(Object driver, ActionControl act)
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
		setTabName("Display");
		setIconName("displayPropIcon.gif");
		setToolTip("Display Property Lists");
		
		//Method Vars
		Vector panelObjects = getPanelObjects();
		
		//SplitPane
		addItem(new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, true, new JPanel(), new JPanel()), endLine);
		
		//place the objects on the panel
		place();

	}//Constructor
	
		/**
		 * Places all objects onto splitFrame
		 */
	public void place()
	{			
		//Method Vars
		Driver driver = (Driver)getDriver();
		
		try
		{
			//Place on next Panel			
			JPanel backing = new JPanel();
			backing.setBackground(Color.white);
			backing.add(createLeftPanel(), BorderLayout.NORTH);
			
			//set them on the split pane
			((JSplitPane)getItem(0)).setLeftComponent(backing);
			((JSplitPane)getItem(0)).setRightComponent(new JPanel());
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the Property Display Panel.");
		}
		
	}//place
	
	/**
	 * Displays a given object on the Right Split
	 * @param String name - the name of the object to be displayed
	 */
	public void displayObject(String name)
	{		
		//Method Vars
		Driver driver = (Driver)getDriver();
		
		try
		{
			//Place on next Panel			
			JPanel backing = new JPanel();
			backing.setBackground(Color.white);
			backing.add(createRightPanel(name), BorderLayout.NORTH);
			
			//set them on the split pane
			((JSplitPane)getItem(0)).setRightComponent(backing);
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while displaying values from the Property List "+ name +".");
		}
		
	}//displayObject

	/**
	 * Creates and returns an ActionPanel of property lists containing buttons for each
	 */
	public ActionPanel createLeftPanel()
	{
		//Method Vars
		ToolTipButton button;
		Data data = ((Driver)getDriver()).getData();
		
		//set-up new ActionPanel
		ActionPanel left = new ActionPanel();
		left.setBackground(Color.white);
		left.createObjects();
		
 		//Check for Valid data
		Vector names = data.getPropertyListNames();
		if(names == null || names.size() == 0)
		{
			left.addItem(new JLabel("  There are no Property Lists open"), endLine, 1, endLine);
		}
		
		//add property lists
		else
		{
			//add space
			left.addItem(new JTextArea("",1,0), endLine);
			
			//Cycle through names
			for(int i=0;i<names.size();i++)
			{
				//add button label
				button = new ToolTipButton("Display", "Display Property List");
				button.addActionListener(getAction());
				button.setActionName("displayList("+ names.elementAt(i) +")");
				left.addItem(button, cont);
				
				//add tree label
				left.addItem(new JLabel((String)names.elementAt(i)), endLine);
			}
		} 
		
		//Create new action panel, set and return
		left.setPanel();
		return left;
		
	}//createLeftPanel
	
	/**
	 * Creates a display panel of the property list specified
	 * @param String name - the name of the object to be displayed
	 */
	public ActionPanel createRightPanel(String name)
	{
		//Method Vars
		JTextArea text;
		ToolTipButton button;
		Data data = ((Driver)getDriver()).getData();
		
		//Create new action panel
		ActionPanel right = new ActionPanel();
		right.setBackground(Color.white);
		right.createObjects();
		
 		//Check for Valid data
		Vector propList = ((Vector)data.getPropertyLists().get(name));
		if(propList != null)
		{				
			//add property list label
			right.addItem(new JLabel("Properties in List: "), endLine, 1, endLine);

			//add space
			right.addItem(new JTextArea("",1,0), endLine);
			
			//Cycle through names
			for(int i=0;i<propList.size();i++)
			{
				text = new JTextArea("  "+ propList.elementAt(i));
				text.setEditable(false);
				right.addItem(text, endLine);
			}
		}
		
		//Create new action panel, set and return
		right.setPanel();
		return right;
		
	}//createRightPanel
	
}//PropListDisplayPanel