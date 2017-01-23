/**
 *	TreeDisplayPanel.class is an instantiated object that creates
 *	a JPanel containing a given tree's information
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JTextArea;

import treesaap.Driver.Driver;
import treesaap.GeneralDNAFileParser.TreeBean;
import treesaap.GeneralDNAFileParser.DNAFileParserPrint;
import treesaap.GeneralDNAFileParser.GeneralDNAFileParser;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.Objects.ActionPanel;

public class TreeDisplayPanel extends ActionPanel
{
	//Class Vars
	String treeName;		//Taxa to be opened
	
	/**
	 * Empty Constructor
	 */
	public TreeDisplayPanel(){}
	
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 * @param String treeName - the tree to be displayed
	 */
	public TreeDisplayPanel(Object driver, ActionControl act, String TreeName)
	{
		treeName = TreeName;
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
		String line;
		JTextArea text;
		GeneralDNAFileParser fileParser = ((Driver)getDriver()).getGDFP();
		DNAFileParserPrint print = new DNAFileParserPrint(fileParser.getBean());
			
		//Get Taxa
		TreeBean tree = (TreeBean)fileParser.getTrees().get(treeName);			
		if(tree != null)
		{				
			//add relation label
			addItem(new JLabel("Derived Relation: "), endLine, 1, endLine);
			
			//add textfield
			line = print.writeTreeHeader(tree);
			text = new JTextArea("  "+ line);//.substring(0,line.indexOf("\n")));
			text.setEditable(false);
			addItem(text, endLine);
			
			//add relation label
		/*	addItem(new JLabel("Derived Relation: "), endLine, 1, endLine);
			text = new JTextArea("  "+ line.substring(line.indexOf("\n")+1));
			text.setEditable(false);
			addItem(text, endLine);*/
			
			//taxa names
			addItem(new JLabel("Taxa: "), endLine);
			
			//add textfield		
			line = "    ";
			for(int i=0;i<tree.getTaxaNames().size();i++)
				line += (String)tree.getTaxaNames().elementAt(i)+", ";
			if(line.indexOf(",")!= -1)
				line = line.substring(0, line.length()-2);
			text = new JTextArea("  "+ line);
			text.setEditable(false);
			addItem(text, endLine);
				
			//add ancestral tree
			addItem(new JLabel("Ancestral Tree: "), endLine, 1, endLine);
			
			//add textfield
			line = print.writeTreeAncestralTree(tree);
			line = line.replaceAll("	",", ");
			line = line.replaceAll(":,", ":");
			line = line.substring(0,line.length()-3);
			text = new JTextArea("  "+ line);
			text.setEditable(false);
			addItem(text, endLine);
			
			//add nodes
			addItem(new JLabel("Nodes: "), endLine, 1, endLine);
			
			//add textfield
			line = "    ";
			for(int i=0;i<tree.getNodeNames().size();i++)
				line += (String)tree.getNodeNames().elementAt(i)+", ";
			if(line.indexOf(",")!= -1)
				line = line.substring(0, line.length()-2);
			text = new JTextArea("  "+ line);
			text.setEditable(false);
			addItem(text, endLine);
		}
	}//create

}//TreeDisplayPanel