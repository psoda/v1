/**
 *	TreePanel.class is an instantiated object that creates
 *	a JPanel containing all the open trees
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

public class TreePanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public TreePanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public TreePanel(Object driver, ActionControl act)
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
		setTabName("Tree");
		setIconName("treeIcon.gif");
		setToolTip("View Open Tree(s)");

		//SplitPane
		addItem(new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, true, new JPanel(), new JPanel()), endLine);
		
		//place the objects on the panel
		place();

	}//create

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
			backing.add(new TreeChoosePanel(getDriver(), getAction()), BorderLayout.NORTH);
			
			//set them on the split pane
			((JSplitPane)getItem(0)).setLeftComponent(backing);
			((JSplitPane)getItem(0)).setRightComponent(new JPanel());
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the Tree Panel.");
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
			backing.add(new TreeDisplayPanel(getDriver(), getAction(), name), BorderLayout.NORTH);
			
			//set on split panel
			((JSplitPane)getItem(0)).setRightComponent(backing);
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while displaying the tree "+ name +".");
		}
	}//displayObject
	
}//TreePanel