/**
 *	RunPanel.class is an instantiated object that creates
 *	a JPanel containing all the run options and choice of trees
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface.Panels;

import java.util.Vector;
import java.awt.Color;
import javax.swing.JComboBox;
import javax.swing.border.BevelBorder;
import javax.swing.JLabel;

import DataConvert.GraphicInterface.ActionControl;
import DataConvert.GraphicInterface.GraphicInterfaceUsageBean;
import DataConvert.GraphicInterface.Objects.ToolTipButton;
import DataConvert.GraphicInterface.Objects.ActionPanel;

public class BottomPanel extends ActionPanel
{
	//Class Vars
	private Vector panels;							//The panels on the GUI
	private GraphicInterfaceUsageBean bean;			//Bean for the GUI
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public BottomPanel(GraphicInterfaceUsageBean Bean, ActionControl act, Vector Panels)
	{
		bean = Bean;
		panels = Panels;
		setAction(act);
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

		//label
		JLabel outputLabel = new JLabel("Output Format:");
		addItem(outputLabel, endLine);
		
		//File Type Chooser
		JComboBox jc = new JComboBox();
		for(int i=0;i<panels.size();i++)
			jc.addItem(((ActionPanel)panels.elementAt(i)).getTabName());
		if(panels.size() > 0)
			jc.setSelectedItem(((ActionPanel)panels.elementAt(0)).getTabName());
		addItem(jc, endLine, 1, endLine);
		
		//Combine Button
		ToolTipButton combine = new ToolTipButton("Combine Files", "Combine open files into same file");
		combine.addActionListener(getAction());
		combine.setActionName("combine");
		addItem(combine, endLine, 1, endLine);
		
		//Write Button
		ToolTipButton write = new ToolTipButton("Write File(s)", "Write files to type specified");
		write.addActionListener(getAction());
		write.setActionName("write");
		addItem(write, endLine);
		
		//place the objects on the panel
		setSettings();
		updateComponents(0);

	}//create

	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setSettings()
	{				
		try
		{
			((JComboBox)getItem(1)).setSelectedItem(bean.getFileType());
		}
		catch(Exception e){}
		
	}//setSettings
	
	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getSettings()
	{	
		try
		{
			bean.setFileType((String)((JComboBox)getItem(1)).getSelectedItem());
		}
		catch(Exception e){}	
	}//getSettings

	/**
	 * Changes the settings on buttons depending on amnt
	 */
	public void updateComponents(int amnt)
	{	
		try
		{
			//Switch on amnt
			if(amnt > 1)
			{
				getItem(2).setEnabled(true);
				getItem(3).setEnabled(true);
			}
			else if(amnt == 1)
			{
				getItem(2).setEnabled(false);
				getItem(3).setEnabled(true);
			}
			else
			{
				getItem(2).setEnabled(false);
				getItem(3).setEnabled(false);
			}
		}
		catch(Exception e){}
		
	}//getSettings

}//BottomPanel
