/**
 *	PropertyPanel.class is an instantiated object that creates
 *	a JPanel containing all the options of the amino acid properties
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.util.Vector;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.BorderLayout;
import javax.swing.JPanel;
import javax.swing.ImageIcon;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.border.BevelBorder;

import treesaap.Driver.Driver;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.GraphicInterfaceUsageBean;
import treesaap.GraphicInterface.Objects.ActionPanel;

public class PropertyPanel extends ActionPanel
{
	//Class Vars
	Vector panels;			//The panels added to tabbed pane
	GraphicInterfaceUsageBean bean;	//The gui's usage bean
	
	/**
	 * Empty Constructor
	 */
	public PropertyPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 * @param GraphicInterfaceUsageBean bean - class created with this bean
	 */
	public PropertyPanel(Object driver, ActionControl act, GraphicInterfaceUsageBean Bean)
	{
		bean = Bean;
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
		setTabName("Properties");
		setIconName("propertyIcon.gif");
		setToolTip("Property Options");
		
		//Method Vars
		JPanel jp;
		ActionPanel a;
		JScrollPane jsp;
		panels = new Vector();
		JTabbedPane tp = new JTabbedPane();
		
		//add on property panels
		panels.add(new PropListSelectPanel(getDriver(), getAction()));
		panels.add(new PropListCreatePanel(getDriver(), getAction()));
		panels.add(new PropListDeletePanel(getDriver(), getAction()));	
		panels.add(new PropListDisplayPanel(getDriver(), getAction()));	
		
		//add on created panels
//		tp.setPreferredSize(new Dimension(800,400));
		for(int i=0;i<panels.size();i++)
		{
			//get button and add to toolbar
			a = (ActionPanel)panels.elementAt(i);
			
			//place action panel on another panel
			jp = new JPanel();
			jp.setBackground(Color.white);
			jp.add(a,BorderLayout.CENTER);
			
			//set-up the scroll pane for each panel
			jsp = new JScrollPane(jp);
			jsp.getHorizontalScrollBar().setUnitIncrement(10);
			jsp.getVerticalScrollBar().setUnitIncrement(10);
			
			//add panel to tabbed pane
			tp.addTab(a.getTabName(), new ImageIcon(bean.getIconPath()+a.getIconName()), jsp, a.getToolTip());
		}
		
		//add tabbed pane to panel objects
		addItem(tp, endLine);
		
	}//Constructor

	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		
		//set objects on lower panels
		for(int i=0;i<panels.size();i++)
			((ActionPanel)panels.elementAt(i)).setValues();
		
	}//setValues

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//set objects on lower panels
		for(int i=0;i<panels.size();i++)
			((ActionPanel)panels.elementAt(i)).getValues();	
		
	}//getValues
	
	/**
	 * Places all objects onto splitFrame
	 */
	public void place()
	{
		createObjects();
	}//place
	
	/**
	 * Displays a given object on the Right Split
	 * @param String name - the name of the object to be displayed
	 */
	public void displayObject(String name)
	{			
		//set objects on lower panels
		for(int i=0;i<panels.size();i++)
			((ActionPanel)panels.elementAt(i)).displayObject(name);	
	}//displayObject	

}//PropertyPanel