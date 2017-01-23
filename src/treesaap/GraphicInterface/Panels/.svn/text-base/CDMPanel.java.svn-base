/**
 *	CDMPanel.class is an instantiated object that creates
 *	a JPanel containing all the options of the CDM calculations
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
import javax.swing.border.BevelBorder;

import treesaap.Driver.Driver;
import treesaap.CDM.CDMUsageBean;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class CDMPanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public CDMPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public CDMPanel(Object driver, ActionControl act)
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
		setTabName("CDM");
		setIconName("cdmIcon.gif");
		setToolTip("CDM Calculation Options");
		
		//user-determined transition bias
		addItem(new JCheckBox(" Set the transition bias", false), cont, 1, endLine);
		addItem( new JTextField("0.0",10), endLine);
				
		//deriveTransibias
		addItem(new JCheckBox(" Derive Transbias from algorithmic search not equation", false), endLine);
		
		//setTransbias
		addItem(new JCheckBox(" Fix Transbias at same value during sliding window", false), endLine);

		//category output options
		addItem(new JLabel("  Sliding-Window File Output Options :"), endLine, 1, endLine);
		addItem(new JCheckBox(" Print G-Scores to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print GF-Scores to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print ts(ti):tv to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print transbias(ts4) to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print observed substitutions to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print frequency of observed substitutions to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print expected substitutions to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print frequency of expected substitutions to file", true), endLine, 1, cont);		
		
		//Buttons
		ToolTipButton save = new ToolTipButton("Save", "Save Settings to File");
		save.addActionListener(getAction());
		save.setActionName("save");
		addItem(save, cont, 2, endLine);
		
		ToolTipButton reset = new ToolTipButton("Reset", "Reset Settings");
		reset.addActionListener(getAction());
		reset.setActionName("reset");
		addItem(reset, endLine);
		
	}//create
	
	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		//Method Vars
		Driver driver = (Driver)getDriver();
		CDMUsageBean cdmBean = driver.getCDM().getBean();
		
		try
		{
			//Text Fields
			((JTextField)getItem(1)).setText(cdmBean.getTransitionBias()+"");
			
			//Check Boxes
			((JCheckBox)getItem(0)).setSelected(!cdmBean.getAutoTransBias());
			((JCheckBox)getItem(2)).setSelected(cdmBean.getDeriveTS4());
			((JCheckBox)getItem(3)).setSelected(cdmBean.getFixedTransBias());
			((JCheckBox)getItem(5)).setSelected(cdmBean.getGScore());
			((JCheckBox)getItem(6)).setSelected(cdmBean.getGFScore());
			((JCheckBox)getItem(7)).setSelected(cdmBean.getTsTv());
			((JCheckBox)getItem(8)).setSelected(cdmBean.getTransbias());
			((JCheckBox)getItem(9)).setSelected(cdmBean.getObserved());
			((JCheckBox)getItem(10)).setSelected(cdmBean.getObservedFrequency());
			((JCheckBox)getItem(11)).setSelected(cdmBean.getExpected());
			((JCheckBox)getItem(12)).setSelected(cdmBean.getExpectedFrequency());
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while placing user-settings on the CDM Panel.");
		}
		
	}//setValues
	
	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars
		Driver driver = (Driver)getDriver();
		CDMUsageBean cdmBean = driver.getCDM().getBean();
		
		try
		{
			//Check Boxes
			cdmBean.setAutoTransBias(!((JCheckBox)getItem(0)).isSelected());
			cdmBean.setDeriveTS4(((JCheckBox)getItem(2)).isSelected());
			cdmBean.setFixedTransBias(((JCheckBox)getItem(3)).isSelected());
			cdmBean.setGScore(((JCheckBox)getItem(5)).isSelected());
			cdmBean.setGFScore(((JCheckBox)getItem(6)).isSelected());
			cdmBean.setTsTv(((JCheckBox)getItem(7)).isSelected());
			cdmBean.setTransbias(((JCheckBox)getItem(8)).isSelected());
			cdmBean.setObserved(((JCheckBox)getItem(9)).isSelected());
			cdmBean.setObservedFrequency(((JCheckBox)getItem(10)).isSelected());
			cdmBean.setExpected(((JCheckBox)getItem(11)).isSelected());
			cdmBean.setExpectedFrequency(((JCheckBox)getItem(12)).isSelected());
			
		//Text Fields
		try{
				String value = ((JTextField)getItem(1)).getText();
				double num = Double.valueOf(value).doubleValue();
				if(num >= 0 && num <= 1)
					cdmBean.setTransitionBias(num);
				else
					throw new Exception();
			}catch(Exception e){
				cdmBean.setTransitionBias(0);
				((JTextField)getItem(1)).setText(0.0+"");
			}
		}
		catch(Exception e)
		{
			driver.getBean().errorMessage("\nThere was a problem while reading values from CDM Panel.");
		}
		
	}//getValues
	
}//CDMPanel