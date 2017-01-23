/**
 *	ActionPanel.class is an instantiated object that creates
 *	a JPanel that is self-describing
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Objects;

import java.util.Vector;
import java.awt.Color;
import java.awt.Component;
import java.awt.BorderLayout;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import javax.swing.JPanel;
import javax.swing.JTextArea;

import treesaap.Driver.Driver;
import treesaap.GraphicInterface.ActionControl;

public class ActionPanel extends JPanel
{
	//Class Vars
	protected Object driver;			//Object of the driver class
	private String tabName;			//The tabbed name of this panel
	private String toolTip;			//The tooltip name of this panel
	private String iconName;		//The icon name of this panel
	private JPanel foreground;		//The panel that contains all the objects
	private ActionControl action;	//Action Control class for this panel
	
	//Process Vars
	private Vector panelObjects;		//Objects placed on this panel
	
	//Access Vars
	public int cont = GridBagConstraints.RELATIVE;		//continuation of a line on the panel
	public int endLine = GridBagConstraints.REMAINDER;	//end of a line on the panel
	
	/**
	 * To be implemented in panels to create objects
	 */	
	public void create(){}

	/**
	 * To be implemented in panels to place objects
	 */	
	public void place(){}
	
	/**
	 * To be implemented in panels to set objects
	 */	
	public void setValues(){}
	
	/**
	 * To be implemented in panels to get objects
	 */	
	public void getValues(){}
	
	/**
	 * To be implemented in panels to display an object
	 */	
	public void displayObject(String name){}

	/**
	 * To be implemented in panels to create objects - Should be called by super classes
	 */	
	public void createObjects()
	{
		//Remove old panel
		if(foreground != null)
			remove(foreground);
		
		//Create a new panel and object vector
		foreground = new JPanel();
		panelObjects = new Vector();
		
		//call create
		create();
		
		//place the objects on the panel
		setPanel();
	}
	
	/**
	 * Places objects from Vector onto this Panel
	 */
	public void setPanel()
	{			
		//Method Vars
		ActionComponent comp;
		GridBagConstraints constraint = new GridBagConstraints();
		constraint.anchor = GridBagConstraints.NORTHWEST;
		
		//set layout
		foreground.setLayout(new GridBagLayout());
		foreground.setBackground(Color.white);
		
		//place objects on the panel
		try
		{
			//cycle through objects vector
			for(int i=0;i<panelObjects.size();i++)
			{
				comp = (ActionComponent)panelObjects.elementAt(i);

				//insert space
				if(comp.getSpace() != 0)
				{
					constraint.gridwidth = comp.getSpacePosition();
					foreground.add(new JTextArea("",comp.getSpace(),0), constraint);
				}
				
				//set the constraints
				constraint.gridwidth = comp.getPosition();
				
				//add component
				foreground.add(comp.getComponent(), constraint);
				
			}		
			
			//lastly, add foreground panel
			add(foreground, BorderLayout.CENTER);
		}
		catch(Exception e)
		{
			((Driver)driver).getBean().errorMessage("\nThere was a problem while placing objects on the Panel "+ tabName +".");
		}
		
	}//setPanel

	/**
	 *  Adds an item to panelObjects, with given parameters
	 *
	 *	@param Component comp - the component to be added
	 *	@param int pos - the position item to be placed on panel
	 *	@param int spacing - the spacing around item
	 *	@param int spacePos - the position of spacing
	 */
	public void addItem(Component comp, int pos, int spacing, int spacePos)
	{
		panelObjects.add(new ActionComponent(comp, pos, spacing, spacePos));
	}
	
	/**
	 *  Adds an item to panelObjects, with given parameters - no spacing
	 *
	 *	@param Component comp - the component to be added
	 *	@param int pos - the position item to be placed on panel
	 */
	public void addItem(Component comp, int pos)
	{
		panelObjects.add(new ActionComponent(comp, pos, 0, 0));
	}	
	
	/**
	 *  Adds an item to panelObjects, with given parameters
	 *	@param int pos - get the element at this position in the panelObjects vector
	 */
	public Component getItem(int pos)
	{
		return ((ActionComponent)panelObjects.elementAt(pos)).getComponent();
	}	
	
	/**
	 *  Get method - returns String tabName 
	 */
	public String getTabName()
	{
		return tabName;
	}
	
	/**
	 *  Set method - sets tabName to new String
	 *	@param String TabName of new tabName
	 */
	public void setTabName(String TabName)
	{
		tabName = TabName;
	}
	
	/**
	 *  Get method - returns String toolTip 
	 */
	public String getToolTip()
	{
		return toolTip;
	}
	
	/**
	 *  Set method - sets toolTip to new String
	 *	@param String ToolTip of new toolTip
	 */
	public void setToolTip(String ToolTip)
	{
		toolTip = ToolTip;
	}
	
	/**
	 *  Get method - returns String iconName 
	 */
	public String getIconName()
	{
		return iconName;
	}
	
	/**
	 *  Set method - sets iconName to new String
	 *	@param String IconName of new iconName
	 */
	public void setIconName(String IconName)
	{
		iconName = IconName;
	}

	/**
	 *  Get method - returns Vector panelObjects 
	 */
	public Vector getPanelObjects()
	{
		return panelObjects;
	}
	
	/**
	 *  Set method - sets panelObjects to new Vector
	 *	@param Vector PanelObjects of new panelObjects
	 */
	public void setPanelObjects(Vector PanelObjects)
	{
		panelObjects = PanelObjects;
	}

	/**
	 *  Get method - returns Object driver 
	 */
	public Object getDriver()
	{
		return driver;
	}
	
	/**
	 *  Set method - sets driver to new Object
	 *	@param Object Driver of new driver
	 */
	public void setDriver(Object Driver)
	{
		driver = Driver;
	}

	/**
	 *  Get method - returns ActionControl action 
	 */
	public ActionControl getAction()
	{
		return action;
	}
	
	/**
	 *  Set method - sets action to new ActionControl
	 *	@param ActionControl newAction of new action
	 */
	public void setAction(ActionControl newAction)
	{
		action = newAction;
	}

}//ActionPanel