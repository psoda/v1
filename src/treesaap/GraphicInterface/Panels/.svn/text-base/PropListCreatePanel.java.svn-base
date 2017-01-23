/**
 *	PropListCreatePanel.class is an instantiated object that creates
 *	a JPanel containing all options for creating a property list
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
import javax.swing.JCheckBox;
import javax.swing.JTextArea;
import javax.swing.JSplitPane;
import javax.swing.JTextField;
import javax.swing.border.BevelBorder;

import treesaap.Data.Data;
import treesaap.Driver.Driver;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class PropListCreatePanel extends ActionPanel
{
	//Class Vars
	private Vector allProperties;		//Vector of all Properties
	private Vector newList;			//Vector of properties for the new list
	private Vector newListBoxes;		//Vector of new property list of boxes
	
	/**
	 * Empty Constructor
	 */
	public PropListCreatePanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public PropListCreatePanel(Object driver, ActionControl act)
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
		setTabName("Create");
		setIconName("createPropIcon.gif");
		setToolTip("Create Property Lists");
		
		//initialize vectors
		newList = new Vector();
		newListBoxes = new Vector();
		allProperties = new Vector();

		//Add on creation options
		addItem(new JTextField("",20), cont, 1, endLine);
		ToolTipButton create = new ToolTipButton("Create New List", "Create New Properties List from those selected");
		create.addActionListener(getAction());
		create.setActionName("display(createPropList)");
		addItem(create, endLine);
		
		//SplitPane
		addItem(new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, true, createLeftPanel(), new JPanel()), endLine, 1, endLine);		
		
	}//Constructor
	
	/**
	 * Displays a given object on the Right Split
	 * @param String name - the name of the object to be displayed
	 */
	public void displayObject(String name)
	{		
		//Method Vars
		JCheckBox prop;
		Driver driver = (Driver)getDriver();
		Data data = driver.getData();
		
		try
		{
			//add selected properties to newList
			if(name.equals("addPropsToList"))
			{
				//cycle through vector, add selected
				for(int i=0;i<allProperties.size();i++)
				{
					prop = (JCheckBox)allProperties.elementAt(i);
					if(prop.isSelected() && !newList.contains(prop.getText()))
					{
						newList.add(prop.getText());
						newListBoxes.add(new JCheckBox(prop.getText()));
					}
				}
			}
			//remove selected properties from newList
			else if(name.equals("removePropsFromList"))
			{
				//cycle through vector, add selected
				for(int i=0;i<newListBoxes.size();i++)
				{
					prop = (JCheckBox)newListBoxes.elementAt(i);
					if(prop.isSelected())
					{
						newListBoxes.remove(i);
						newList.remove(i);
						i--;
					}
				}
				
				//if Vector down to a certain size, place
				if(newListBoxes.size() == 0)
					driver.getGUI().placeObjects();
				
			}
			//remove all properties from newList
			else if(name.equals("removeAllPropsFromList"))
			{
				//create new lists
				newList = new Vector();
				newListBoxes = new Vector();
				
				//reset gui
				driver.getGUI().placeObjects();
			}
			//create a new list
			else if(name.equals("createPropList") && newList.size() > 0)
			{
				//get property list name, add to data
				data.createLists(getListName(), newList);
				
				//place items on gui
				driver.getGUI().placeObjects();
				
				//create new vectors
				newList = new Vector();
				newListBoxes = new Vector();
			}
			
			//reset selected
			reset();
			
			//set them on the split pane
			((JSplitPane)getItem(2)).setRightComponent(createRightPanel());
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing values on to the new List.");
		}
		
	}//displayObject

	/**
	 * Creates and returns an ActionPanel of property lists containing buttons for each
	 */
	public JPanel createLeftPanel()
	{
		//Method Vars
		ToolTipButton button;
		Driver driver = (Driver)getDriver();
		Data data = driver.getData();

		//set-up new ActionPanel
		ActionPanel left = new ActionPanel();
		left.setBackground(Color.white);
		left.createObjects();
		
		try
		{
			//Check for Valid data
			Vector names = data.getPropertyNames();
			if(names == null || names.size() == 0)
			{
				left.addItem(new JLabel("  There are no Properties open"), endLine, 1, endLine);
			}
			
			//add property lists
			else
			{						
				//Set-up background action panel
				ActionPanel aPanel = new ActionPanel();
				aPanel.setBackground(Color.white);
				aPanel.createObjects();
				
				//Buttons
				ToolTipButton addButton = new ToolTipButton("Add to New List", "Add Selected Properties to New List");
				addButton.addActionListener(getAction());
				addButton.setActionName("display(addPropsToList)");
				aPanel.addItem(addButton, endLine);
				
				//Cycle through names - add check boxes
				for(int i=0;i<names.size();i++)
				{
					//Create a new panel to add to every 100 items - max on panel is ~500, not first time
					if(i%100 == 0 && i != 0)
					{
						aPanel.setPanel();
						left.addItem(aPanel, endLine);
						
						//Create a new panel
						aPanel = new ActionPanel();
						aPanel.setBackground(Color.white);
						aPanel.createObjects();
					}
				
					//add on checkbox
					JCheckBox prop = new JCheckBox((String)names.elementAt(i));
					aPanel.addItem(prop, endLine);
					allProperties.add(prop);
				}
					
				//pick up the end
				aPanel.setPanel();
				left.addItem(aPanel, endLine);
			} 
			//set actionPanel
			left.setPanel();
			
			//Place on next Panel			
			JPanel backing = new JPanel();
			backing.setBackground(Color.white);
			backing.add(left, BorderLayout.NORTH);
			
			return backing;
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the Property Create Panel.");
			return new JPanel();
		}
		
	}//createLeftPanel
	
	/**
	 * Creates a display panel of the property list specified
	 */
	public JPanel createRightPanel()
	{
		//Method Vars
		ToolTipButton button;
		Driver driver = (Driver)getDriver();

		//set-up new ActionPanel
		ActionPanel right = new ActionPanel();
		right.setBackground(Color.white);
		right.createObjects();
		
		try
		{
			//Check for Valid data
			if(newListBoxes.size() != 0)
			{						
				//Set-up background action panel
				ActionPanel aPanel = new ActionPanel();
				aPanel.setBackground(Color.white);
				aPanel.createObjects();
				
				//Buttons
				ToolTipButton removeButton = new ToolTipButton("Remove from New List", "Remove Selected Properties from New List");
				removeButton.addActionListener(getAction());
				removeButton.setActionName("display(removePropsFromList)");
				aPanel.addItem(removeButton, cont);
				
				ToolTipButton removeAllButton = new ToolTipButton("Remove All", "Remove All Properties from New List");
				removeAllButton.addActionListener(getAction());
				removeAllButton.setActionName("display(removeAllPropsFromList)");
				aPanel.addItem(removeAllButton, endLine);
				
				//Cycle through names - add check boxes
				for(int i=0;i<newListBoxes.size();i++)
				{
					//Create a new panel to add to every 100 items - max on panel is ~500, not first time
					if(i%100 == 0 && i != 0)
					{
						aPanel.setPanel();
						right.addItem(aPanel, endLine);
						
						//Create a new panel
						aPanel = new ActionPanel();
						aPanel.setBackground(Color.white);
						aPanel.createObjects();
					}
				
					//add on checkbox
					JCheckBox prop = (JCheckBox)newListBoxes.elementAt(i);
					aPanel.addItem(prop, endLine);
				}
					
				//pick up the end
				aPanel.setPanel();
				right.addItem(aPanel, endLine);
			} 
			//set actionPanel
			right.setPanel();
			
			//Place on next Panel			
			JPanel backing = new JPanel();
			backing.setBackground(Color.white);
			backing.add(right, BorderLayout.NORTH);
			
			return backing;
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the Property Create Panel.");
			return new JPanel();
		}
		
	}//createRightPanel
	
	/**
	 * reset the check boxes to false
	 */
	public void reset()
	{
		//go through allProperties
		for(int i=0;i<allProperties.size();i++)
			((JCheckBox)allProperties.elementAt(i)).setSelected(false);
		
		//go through new list boxes
		for(int i=0;i<newListBoxes.size();i++)
			((JCheckBox)newListBoxes.elementAt(i)).setSelected(false);
	}
	
	/**
	 * returns the user-specified string
	 */
	public String getListName()
	{
		//get the name on the box
		Data data = ((Driver)getDriver()).getData();
		String name = ((JTextField)getItem(0)).getText();

		//check for valid name
		if(name == null || name.equals("") || name.indexOf("*") != -1 || name.indexOf("/") != -1 || name.indexOf("\\") != -1 || name.indexOf("'") != -1 || name.indexOf("\"") != -1)
			name = "property_list";
		
		//get a name that is unique
		int i = 0;
		String temp = name;
		while(data.getPropertyListNames().contains(temp))
		{
			temp = name +"_"+ i;
			i++;
		}
		name = temp;
		
		return name;
	}
	
}//PropListDisplayPanel