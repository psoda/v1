/**
 *	BasemlPanel.class is an instantiated object that creates
 *	a JPanel containing all the options of the BASEML
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.NEXUS;

import DataConvert.GraphicInterface.Objects.ActionPanel;
import java.awt.Color;
import javax.swing.JTextField;
import javax.swing.ButtonGroup;
import javax.swing.JRadioButton;
import javax.swing.border.BevelBorder;

class NEXUSPanel extends ActionPanel
{	
	//Class Vars
	NEXUSUsageBean bean;			//Bean of NEXUS object
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public NEXUSPanel(NEXUSUsageBean Bean)
	{
		bean = Bean;
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
		setTabName("NEXUS");
		setIconName("nexusIcon.gif");
		setToolTip("NEXUS File Format Options");

		//Radio Buttons
		ButtonGroup models = new ButtonGroup();
		JRadioButton oneLine = new JRadioButton("Entire sequence on the same line");
		JRadioButton chunks = new JRadioButton("Interleave sequence.  Size of: ");
		oneLine.setSelected(true);
		
		//add buttons to panelObjects and buttonGroup
		models.add(oneLine);
		models.add(chunks);
		addItem(oneLine, endLine, 1, endLine);
		addItem(chunks, cont);
		
		//Add on text field
		addItem(new JTextField("60",5), endLine);
		
		setSettings();
	}//create

	/**
	 * Sets all Possible Displayed items in Panel to what is read in from file
	 */
	public void setSettings()
	{				
		try
		{
			//Switch on interleaved size
			if(bean.getInterleaveSize() < 1)
			{
				((JRadioButton)getItem(0)).setSelected(true);
			}
			else
			{
				((JRadioButton)getItem(1)).setSelected(true);
				((JTextField)getItem(2)).setText(bean.getInterleaveSize()+"");
			}
		}
		catch(Exception e)
		{
	//		driverBean.errorMessage("\nThere was a problem while placing user-settings on the BASEML Panel.");
		}
		
	}//setSettings

	/**
	 * Places all options selected from the GUI into the usage bean
	 */
	public void getSettings()
	{	
		try
		{
			//Switch on interleaved size
			if(((JRadioButton)getItem(0)).isSelected())
			{
				bean.setInterleaveSize(0);
			}
			else
			{
				try{
					String value = ((JTextField)getItem(2)).getText();	
					int num = Integer.valueOf(value).intValue();
					if(num > 0)
						bean.setInterleaveSize(num);
				}catch(Exception e){
					bean.setInterleaveSize(60);
					((JTextField)getItem(2)).setText(60+"");
				}
			}
		}
		catch(Exception e){}
		
	}//getSettings

}//NEXUSPanel
