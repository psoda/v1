/**
 *	TaxaDisplayPanel.class is an instantiated object that creates
 *	a JPanel containing a given taxa's information
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.awt.Font;
import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JTextArea;

import treesaap.Driver.Driver;
import treesaap.GeneralDNAFileParser.TaxaBean;
import treesaap.GeneralDNAFileParser.GeneralDNAFileParser;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;

public class TaxaDisplayPanel extends ActionPanel
{
	//Class Vars
	String taxaName;		//Taxa to be opened
	
	/**
	 * Empty Constructor
	 */
	public TaxaDisplayPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 * @param String taxaName - the taxa to be displayed
	 */
	public TaxaDisplayPanel(Object driver, ActionControl act, String TaxaName)
	{
		taxaName = TaxaName;
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
		String sequence;
		JTextArea text;
		GeneralDNAFileParser fileParser = ((Driver)getDriver()).getGDFP();
			
		//Get Taxa
		TaxaBean taxa = (TaxaBean)fileParser.getTaxa().get(taxaName);			
		if(taxa != null)
		{				
			//add taxa label
			addItem(new JLabel(" Length in Nucleotides: "), cont, 1, endLine);
			
			//add textfield
			sequence = taxa.getSequence();
			text = new JTextArea("  "+ sequence.length());
			text.setEditable(false);
			addItem(text, endLine);
			
			//add taxa label
			addItem(new JLabel(" Sequence: "), endLine, 1, endLine);

			//add textfield
			for(int i=200;i<sequence.length();i+=203)
				sequence = sequence.substring(0,i) +"\n  "+sequence.substring(i);
			text = new JTextArea("  "+ sequence);
			text.setFont(new Font("Courier", Font.PLAIN, 12));
			text.setEditable(false);
			addItem(text, endLine);
		}
	}//create

}//TaxaDisplayPanel