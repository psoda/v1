/**
 *	CorrelatePanel.class is an instantiated object that creates
 *	a JPanel containing all the options of correlating treeSAAP and CDM results
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.awt.Color;
import javax.swing.border.BevelBorder;

import treesaap.Driver.Driver;
import treesaap.Driver.DriverUsageBean;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;

public class CorrelatePanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public CorrelatePanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public CorrelatePanel(Object driver, ActionControl act)
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
		setTabName("Correlate");
		setIconName("correlateIcon.gif");
		setToolTip("Correlation Options");
		
		//Method Vars
		/*BASEMLUsageBean basemlBean = ((Driver)getDriver()).getBaseml().getBean();
		
		//fixKappa
		addItem(new JCheckBox(" Fix Kappa", false), cont, 1, endLine);
		addItem(new JTextField("2.5",5), endLine);		
		
		//fixAlpha
		addItem(new JCheckBox(" Fix Alpha", true), cont);
		addItem(new JTextField("0.0",5), endLine);		
		
		//Label for Radio Buttons
		addItem(new JLabel("  Choose Evolution Model :"), endLine, 1, endLine);
		
		//Radio Buttons
		String name;
		JRadioButton jb;
		ButtonGroup models = new ButtonGroup();
		for(int i=0;i<basemlBean.getModelNames().size();i++)
		{
			//create button
			name = (String)basemlBean.getModelNames().elementAt(i);
			jb = new JRadioButton(name);
			models.add(jb);
			
			//set selected
			if(name.equals("REV"))
				jb.setSelected(true);
			
			//add to panelObjects
			addItem(jb, endLine, 1, cont);
		}	
		
		//move files
		addItem(new JCheckBox(" Move BASEML temp files to Output Directory", true), endLine, 1, endLine);
		addItem(new JCheckBox(" Move RST file to Output Directory", true), endLine);
		
		//Buttons
		ToolTipButton save = new ToolTipButton("Save", "Save Settings to File");
		save.setBackground(Color.white);
		save.addActionListener(getAction());
		save.setActionName("save");
		addItem(save, cont, 2, endLine);
		
		ToolTipButton reset = new ToolTipButton("Reset", "Reset Settings");
		reset.setBackground(Color.white);
		reset.addActionListener(getAction());
		reset.setActionName("reset");
		addItem(reset, endLine);*/

	}//create
	
	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setValues()
	{		
		//Method Vars
		DriverUsageBean driverBean = ((Driver)getDriver()).getBean();
		
		try
		{
			//kappa
			/*((JCheckBox)getItem(0)).setSelected(basemlBean.getFixKappa());
			((JTextField)getItem(1)).setText(basemlBean.getKappa()+"");
			
			//alpha
			((JCheckBox)getItem(2)).setSelected(basemlBean.getFixAlpha());
			((JTextField)getItem(3)).setText(basemlBean.getAlpha()+"");
			
			//move
			((JCheckBox)getItem(size+5)).setSelected(driverBean.getMove());
			((JCheckBox)getItem(size+6)).setSelected(driverBean.getMoveRst());
			
			//Model
			JRadioButton jb;
			for(int i=5;i<size;i++)
			{
				jb = (JRadioButton)getItem(i);
				if(jb.getText().equals(basemlBean.getModel()))
					jb.setSelected(true);
			}*/
			
		}
		catch(Exception e)
		{
			driverBean.errorMessage("\nThere was a problem while placing user-settings on the Correlate Panel.");
		}
		
	}//setValues
	
	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars
		DriverUsageBean driverBean = ((Driver)getDriver()).getBean();
		
		try
		{
			//kappa
		/*	basemlBean.setFixKappa(((JCheckBox)getItem(0)).isSelected());
			
			//alpha
			basemlBean.setFixAlpha(((JCheckBox)getItem(2)).isSelected());
			
			//move
			driverBean.setMove(((JCheckBox)getItem(size+5)).isSelected());
			driverBean.setMoveRst(((JCheckBox)getItem(size+6)).isSelected());
			
			//Model
			JRadioButton jb;
			for(int i=5;i<size;i++)
			{
				jb = (JRadioButton)getItem(i);
				if(jb.isSelected())
					basemlBean.setModel(jb.getText());
			}			
			
			//Text Fields
			double num;
			String value;
			try{
				value = ((JTextField)getItem(1)).getText();
				num = Double.valueOf(value).doubleValue();
				basemlBean.setKappa(num);
			}catch(Exception e){
				basemlBean.setKappa(2.5);
				((JTextField)getItem(1)).setText(2.5+"");
			}
			try{
				value = ((JTextField)getItem(3)).getText();
				num = Double.valueOf(value).doubleValue();
				basemlBean.setAlpha(num);
			}catch(Exception e){
				basemlBean.setAlpha(0.0);
				((JTextField)getItem(3)).setText(0.0+"");
			}*/
		}
		catch(Exception e)
		{
			driverBean.errorMessage("\nThere was a problem while reading values from Correlate Panel.");
		}
		
	}//getValues
	
}//CorrelatePanel