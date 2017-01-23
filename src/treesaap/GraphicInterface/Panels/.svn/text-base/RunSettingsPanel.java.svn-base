/**
 *	RunSettingsPanel.class is an instantiated object that creates
 *	a JPanel containing all the run options 
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JCheckBox;
import javax.swing.JTextField;

import treesaap.Driver.Driver;
import treesaap.Run.RunUsageBean;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class RunSettingsPanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public RunSettingsPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public RunSettingsPanel(Object driver, ActionControl act)
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
		setBackground(Color.white);
		
		//Check boxes
		addItem(new JCheckBox(" Display TCS Tree in Window", true), endLine, 1, endLine);
		addItem(new JCheckBox(" Display Window to monitor Progress", true), endLine);
		
		//Labels and Fields
		addItem(new JLabel("  Number of Processors to be utilized (>0)"), cont, 1, endLine);
		addItem(new JTextField("1",5), endLine);		
		addItem(new JLabel("  Sliding Window Size (>0,<=length)"), cont);
		addItem(new JTextField("length",5), endLine);		
		addItem(new JLabel("  Increment of Sliding Window (>0,<=length)"), cont);
		addItem(new JTextField("1",5), endLine);
		
		//Buttons
		ToolTipButton save = new ToolTipButton("Save", "Save Settings to File");
		save.addActionListener(getAction());
		save.setActionName("save");
		addItem(save, cont, 2, endLine);
		
		ToolTipButton reset = new ToolTipButton("Reset", "Reset Settings");
		reset.addActionListener(getAction());
		reset.setActionName("reset");
		addItem(reset, endLine);

	}//Constructor


	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		//Method Vars
		Driver driver = (Driver)getDriver();
		RunUsageBean runBean = driver.getRun().getBean();

 		//driver
		((JCheckBox)getItem(0)).setSelected(driver.getTree().getBean().getDisplayTCSWindow());
		
		//run
		((JCheckBox)getItem(1)).setSelected(runBean.getProgressWindow());
		((JTextField)getItem(3)).setText(runBean.getProcessors()+"");
		((JTextField)getItem(7)).setText(runBean.getIncrement()+"");
		if(runBean.getSlidingWindowSize() != -1)
			((JTextField)getItem(5)).setText(runBean.getSlidingWindowSize()+"");
		else
			((JTextField)getItem(5)).setText("length");		
	}//setValues			
	
	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars
		Driver driver = (Driver)getDriver();
		RunUsageBean runBean = driver.getRun().getBean();		

		//Check Boxes
		driver.getTree().getBean().setDisplayTCSWindow(((JCheckBox)getItem(0)).isSelected());
		runBean.setProgressWindow(((JCheckBox)getItem(1)).isSelected());

		
		//Text Fields
		int num;
		String value;
		try{
			value = ((JTextField)getItem(3)).getText();
			num = Integer.valueOf(value).intValue();
			if(num > 0)
				runBean.setProcessors(num);
			else
				throw new Exception();
		}catch(Exception e){
			runBean.setProcessors(1);
			((JTextField)getItem(3)).setText(1+"");
		}
		try{
			value = ((JTextField)getItem(7)).getText();
			num = Integer.valueOf(value).intValue();
			if(num > 0)
				runBean.setIncrement(num);
			else
				throw new Exception();
		}catch(Exception e){
			runBean.setIncrement(1);
			((JTextField)getItem(7)).setText(1+"");
		}
		try{
			value = ((JTextField)getItem(5)).getText();
			num = Integer.valueOf(value).intValue();
			if(num > 0)
				runBean.setSlidingWindowSize(num);
			else
				throw new Exception();
		}catch(Exception e){
			runBean.setSlidingWindowSize(-1);
			((JTextField)getItem(5)).setText("length");
		}
	}//getValues	

}//RunSettingsPanel