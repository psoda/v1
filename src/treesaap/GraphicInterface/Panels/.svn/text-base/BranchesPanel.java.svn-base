/**
 *	BranchesPanel.class is an instantiated object that creates
 *	a JPanel containing all the options of running calculation on specified branches of a tree
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.util.Vector;
import java.awt.Color;
import java.awt.BorderLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.ButtonGroup;
import javax.swing.JRadioButton;
import javax.swing.border.BevelBorder;

import treesaap.Driver.Driver;
import treesaap.GeneralDNAFileParser.GeneralDNAFileParser;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class BranchesPanel extends ActionPanel
{
	//Class Vars
	private Vector leftList;		//Compare specified taxa
	private Vector rightList;		//To this specified taxa
	
	/**
	 * Empty Constructor
	 */
	public BranchesPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public BranchesPanel(Object driver, ActionControl act)
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
		setTabName("Pair-Wise");
		setIconName("branchesIcon.gif");
		setToolTip("Direct Taxa Pair-Wise Comparison Options");
		
		//Method Vars
		leftList = new Vector();
		rightList = new Vector();
	
		//Add on creation options
		ToolTipButton runAll = new ToolTipButton("Run All Comparisons", "Run all pair-wise comparisons");
		runAll.addActionListener(getAction());
		runAll.setActionName("branches(all)");
		addItem(runAll, endLine, 1, endLine);
		
		//add button and split pane
		ToolTipButton runButton = new ToolTipButton("Run Selected Taxa", "Evaluate Specified Taxa Directly");
		runButton.addActionListener(getAction());
		runButton.setActionName("branches(specific)");
		addItem(runButton, endLine, 1, endLine);
		//addItem(new JLabel(" Utilize Algorithms with Specified Conditions on Branches Selected Below"), endLine);
		
		//SplitPane
		addItem(new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, true, createBranchPanel(leftList), createBranchPanel(rightList)), endLine);	
		
	}//create

	/**
	 * Place objects on gui
	 */
	public void place()
	{	
		//reset vectors
		leftList = new Vector();
		rightList = new Vector();
		
		//reset panels on splitpane
		((JSplitPane)getItem(2)).setLeftComponent(createBranchPanel(leftList));
		((JSplitPane)getItem(2)).setRightComponent(createBranchPanel(rightList));
		
		//select something
		setValues();
	}
	
	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		//Method Vars
		Driver driver = (Driver)getDriver();
		
		try
		{
			//select first elements
			if(leftList.size() > 0 && rightList.size() > 0)
			{
				((JRadioButton)leftList.elementAt(0)).setSelected(true);
				((JRadioButton)rightList.elementAt(0)).setSelected(true);
			}
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the Branches Panel.");
		}
		
	}//setValues
	
	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars
		String compare = "";
		Driver driver = (Driver)getDriver();
		
		try
		{
			//run through left vector
			for(int i=0;i<leftList.size();i++)
			{
				if(((JRadioButton)leftList.elementAt(i)).isSelected())
				{
					compare = ((JRadioButton)leftList.elementAt(i)).getText();
					break;
				}
			}
			
			//run through right vector
			for(int i=0;i<rightList.size();i++)
			{
				if(((JRadioButton)rightList.elementAt(i)).isSelected())
				{
					compare += " .. "+ ((JRadioButton)rightList.elementAt(i)).getText();
					break;
				}
			}
			
			//set on bean the list name
			driver.getBean().setBranchComparison(compare);
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while reading values from Branches Panel.");
		}
		
	}//getValues
	
	/**
	 * Creates and returns an ActionPanel of property lists containing buttons for each
	 * @param Vector buttonVector - the vector containing created buttons
	 */
	public JPanel createBranchPanel(Vector buttonVector)
	{
		//Method Vars
		ButtonGroup bg = new ButtonGroup();
		Driver driver = (Driver)getDriver();
		GeneralDNAFileParser fileParser = ((Driver)getDriver()).getGDFP();
		
		//set-up new ActionPanel
		ActionPanel panel = new ActionPanel();
		panel.setBackground(Color.white);
		panel.createObjects();
		
		try
		{
			//Check for Valid data
			Vector names = fileParser.getTaxaNames();
			if(names == null || names.size() == 0)
			{
				panel.addItem(new JLabel("  There are no taxa open"), endLine);
			}
			
			//add property lists
			else
			{						
				//Set-up background action panel
				ActionPanel aPanel = new ActionPanel();
				aPanel.setBackground(Color.white);
				aPanel.createObjects();
				
				//Cycle through names - add check boxes
				for(int i=0;i<names.size();i++)
				{
					//Create a new panel to add to every 100 items - max on panel is ~500, not first time
					if(i%100 == 0 && i != 0)
					{
						aPanel.setPanel();
						panel.addItem(aPanel, endLine);
						
						//Create a new panel
						aPanel = new ActionPanel();
						aPanel.setBackground(Color.white);
						aPanel.createObjects();
					}
					
					//add on checkbox
					JRadioButton prop = new JRadioButton((String)names.elementAt(i));
					bg.add(prop);
					aPanel.addItem(prop, endLine);
					buttonVector.add(prop);
				}
				
				//pick up the end
				aPanel.setPanel();
				panel.addItem(aPanel, endLine);
			} 
			//set actionPanel
			panel.setPanel();
			
			//Place on next Panel			
			JPanel backing = new JPanel();
			backing.setBackground(Color.white);
			backing.add(panel, BorderLayout.NORTH);
			
			return backing;
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the Branches Panel.");
			return new JPanel();
		}
		
	}//createBranchPanel

}//BranchesPanel