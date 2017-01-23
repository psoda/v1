/**
 *	TaxaChoosePanel.class is an instantiated object that creates
 *	a JPanel containing all the taxa that can be displayed
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.util.Vector;
import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JTextArea;

import treesaap.Driver.Driver;
import treesaap.GeneralDNAFileParser.GeneralDNAFileParser;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;

public class TaxaChoosePanel extends ActionPanel
{
	/**
	 * Empty Constructor
	 */
	public TaxaChoosePanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public TaxaChoosePanel(Object driver, ActionControl act)
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

		//Method Vars
		ToolTipButton button;
		GeneralDNAFileParser fileParser = ((Driver)getDriver()).getGDFP();
		
		//Check for Valid data
		Vector names = fileParser.getTaxaNames();
		if(names == null || names.size() == 0)
		{
			addItem(new JLabel("  There are no taxa open"), endLine);
		}
		
		//Cycle through names
		else
		{
			//add space
			addItem(new JTextArea("",1,0), endLine);
			
			for(int i=0;i<names.size();i++)
			{
				//add button label
				button = new ToolTipButton("Display", "Display Taxa");
				button.addActionListener(getAction());
				button.setActionName("displayTaxa("+ names.elementAt(i) +")");
				addItem(button, cont);
				
				//add tree label
				addItem(new JLabel((String)names.elementAt(i)), endLine);
			}
		}
	}//create

}//TaxaChoosePanel