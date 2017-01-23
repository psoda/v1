/**
 *	EvpthwyPanel.class is an instantiated object that creates
 *	a JPanel containing all the options of the Evpthwy Calculation
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.util.Vector;
import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JCheckBox;
import javax.swing.JTextField;
import javax.swing.border.BevelBorder;

import treesaap.Driver.Driver;
import treesaap.Evpthwy.EvpthwyStub;
import treesaap.Evpthwy.EvpthwyUsageBean;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class EvpthwyPanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public EvpthwyPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public EvpthwyPanel(Object driver, ActionControl act)
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
		setBorder(new BevelBorder(BevelBorder.RAISED,Color.lightGray,Color.black));
		setBackground(Color.white);
		setTabName("Evpthwy");
		setIconName("evpthwyIcon.gif");
		setToolTip("Evpthwy Calculation Options");
		
		//numberOfCategories
		addItem(new JLabel("  Number of Categories Used in Calculations (>1) :"), endLine, 1, endLine);
		addItem(new JTextField("8",5), endLine, 1, cont);	

		//which Categories
		addItem(new JLabel("  Which Categories to print to file - see documentation (+,-,<,>1,all) :"), endLine);
		addItem( new JTextField("all",25), endLine, 1, cont);	

		//category output options
		addItem(new JLabel("  Category-Totals File Output Options :"), endLine, 1, endLine);
		addItem(new JCheckBox(" Print Tallied Codons to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" (Cn)Print Observed Pathways to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" (C'n)Print Possible Pathways to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print Analysis variables to file", true), endLine, 1, cont);
		
		//significant output options
		addItem(new JLabel("  Significant Properties Files Output Options :"), endLine, 1, endLine);
		addItem(new JCheckBox(" Print values at the .05 confidence level to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print values at the .01 confidence level to file", true), endLine, 1, cont);
		addItem(new JCheckBox(" Print values at the .001 confidence level to file", true), endLine, 1, cont);		
		
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
		EvpthwyUsageBean evBean = ((Driver)getDriver()).getEvpthwy().getBean();
		
		try
		{
			//Text Fields
			((JTextField)getItem(1)).setText(evBean.getNumberOfCat()+"");
			
			//Check Boxes
			((JCheckBox)getItem(5)).setSelected(evBean.getCodonTally());
			((JCheckBox)getItem(6)).setSelected(evBean.getObservedChart());
			((JCheckBox)getItem(7)).setSelected(evBean.getPthwyChart());
			((JCheckBox)getItem(8)).setSelected(evBean.getHeader());
			
			((JCheckBox)getItem(10)).setSelected(evBean.getConf05());
			((JCheckBox)getItem(11)).setSelected(evBean.getConf01());
			((JCheckBox)getItem(12)).setSelected(evBean.getConf001());


			//Categories
			String cats = "";
			if(evBean.getPosCategories().size() + evBean.getNegCategories().size() == 0)
				cats = "all";
			else
			{
				//add on pos and neg categories
				for(int i=0;i<evBean.getPosCategories().size();i++)
					cats += "+"+ evBean.getPosCategories().elementAt(i) +", ";
				for(int i=0;i<evBean.getNegCategories().size();i++)
					cats += "-"+ evBean.getNegCategories().elementAt(i) +", ";				
				
				//remove last ", "
				if(cats.length() != 0)
					cats = cats.substring(0, cats.length()-2);

				//remove positive zeros
				int index = cats.indexOf("+0");
				if(index != -1)
					if(index+2 == cats.length())
						cats = cats.substring(0,index-1);
					else
						cats = cats.substring(0,index) + cats.substring(index+4);	
				
				//remove negative zeros
				index = cats.indexOf("-0");
				if(index != -1)
					if(index+2 == cats.length())
						cats = cats.substring(0,index-1);
					else
						cats = cats.substring(0,index) + cats.substring(index+4);							
			}
			((JTextField)getItem(3)).setText(cats);
			
		}
		catch(Exception e)
		{
			evBean.errorMessage("\nThere was a problem while placing user-settings on the Evpthwy Panel.");
		}
		
	}//setValues

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getValues()
	{	
		//Method Vars
		EvpthwyUsageBean evBean = ((Driver)getDriver()).getEvpthwy().getBean();
		
		try
		{
			//Check Boxes
			evBean.setCodonTally(((JCheckBox)getItem(5)).isSelected());
			evBean.setObservedChart(((JCheckBox)getItem(6)).isSelected());
			evBean.setPthwyChart(((JCheckBox)getItem(7)).isSelected());
			evBean.setHeader(((JCheckBox)getItem(8)).isSelected());

			evBean.setConf05(((JCheckBox)getItem(10)).isSelected());
			evBean.setConf01(((JCheckBox)getItem(11)).isSelected());
			evBean.setConf001(((JCheckBox)getItem(12)).isSelected());
			
			//Categories
				String thisLine = "category	"+ ((JTextField)getItem(3)).getText();
				EvpthwyStub stub = new EvpthwyStub();
				
				//get chars
				Vector pos = stub.getCats(thisLine, "-");
				Vector neg = stub.getCats(thisLine, "+");
				
				//set vectors
				evBean.setPosCategories(pos);
				evBean.setNegCategories(neg);
			
			//Text Fields
			try{
				String value = ((JTextField)getItem(1)).getText();
				int num = Integer.valueOf(value).intValue();
				if(num >= 2)
					evBean.setNumberOfCat(num);
				else
					throw new Exception();
			}catch(Exception e){
				evBean.setNumberOfCat(8);
				((JTextField)getItem(1)).setText(8+"");
			}
		}
		catch(Exception e)
		{
			evBean.errorMessage("\nThere was a problem while reading values from Evpthwy Panel.");
		}
		
	}//getValues

}//EvpthwyPanel