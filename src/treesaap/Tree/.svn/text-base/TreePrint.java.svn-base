/**
 *	TreePrint.class is responsible for printing of TCS tree to file
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Tree;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Vector;
import java.util.Hashtable;

import treesaap.GeneralDNAFileParser.TreeBean;
import treesaap.GeneralDNAFileParser.TaxaBean;

public class TreePrint
{
	//CLASS Vars
	private TreeUsageBean bean;			//Bean containing all values necessary for operation

	
	/**
	 * TreePrint Constructor
	 * @param Bean TreeUsageBean usage bean is set to that passed in
	 */
	public TreePrint(TreeUsageBean Bean)
	{
		bean = Bean;
		
	}//Constructor 

	/**
	 *	Writes Selected tree to a tcs file
	 *
	 *	@param String fileName - validated path, where information is printed
	 *	@param TreeBean tree - the tree to be writen to file
	 *	@param Hashtable taxa - the taxa containing sequence data
	 */
	public void writeTCSTree(String fileName, TreeBean tree, Hashtable taxa)
	{
		try
		{
			//Method Vars
			Vector taxaNames = tree.getTaxaNames();
			Vector nodeNames = tree.getNodeNames();
			Vector ancTree = tree.getAncestralTree();
			
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//Create file printer
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//start graph
			outFile.println("graph\n[\n	directed 1");
			
			//print Taxa
			for(int i=0;i<taxaNames.size();i++)
			{
				outFile.println("\tnode\n\t[\n\t\tdata\n\t\t[\n\t\t\tSequence \""+((TaxaBean)taxa.get((String)taxaNames.elementAt(i))).getSequence()+"\"\n\t\t]");
				outFile.println("\t\tid "+i+"\n\t\tlabel \""+taxaNames.elementAt(i)+"\"\n\t\tgraphics\n\t\t[\n\t\t\tImage\n\t\t\t[\n\t\t\t\tType \"\"\n\t\t\t\tLocation \"\"\n\t\t\t]");
				outFile.println("\t\t\tcenter\n\t\t\t[\n\t\t\t\tx 0.0\n\t\t\t\ty 0.0\n\t\t\t\tz 0.0\n\t\t\t]\n\t\t\twidth 45.0\n\t\t\theight 20.0\n\t\t\tdepth 20.0\n\t\t]");
				outFile.println("\t\tvgj\n\t\t[\n\t\t\tlabelPosition \"center\"\n\t\t\tshape \"Rectangle\"\n\t\t]\n\t]");
			}
			
			//print Nodes
			for(int i=0;i<nodeNames.size();i++)
			{
				outFile.println("\tnode\n\t[\n\t\tdata\n\t\t[\n\t\t\tSequence \""+((TaxaBean)tree.getNodes().get((String)nodeNames.elementAt(i))).getSequence()+"\"\n\t\t]");
				outFile.println("\t\tid "+(i+taxaNames.size())+"\n\t\tlabel \""+nodeNames.elementAt(i)+"\"\n\t\tgraphics\n\t\t[\n\t\t\tImage\n\t\t\t[\n\t\t\t\tType \"\"\n\t\t\t\tLocation \"\"\n\t\t\t]");
				outFile.println("\t\t\tcenter\n\t\t\t[\n\t\t\t\tx 0.0\n\t\t\t\ty 0.0\n\t\t\t\tz 0.0\n\t\t\t]\n\t\t\twidth 45.0\n\t\t\theight 20.0\n\t\t\tdepth 20.0\n\t\t]");
				outFile.println("\t\tvgj\n\t\t[\n\t\t\tlabelPosition \"center\"\n\t\t\tshape \"Oval\"\n\t\t]\n\t]");
			}
			
			//print Comparison - Ancestral tree
			int t1, t2;
			String taxa1, taxa2;
			for(int i=0;i<ancTree.size();i+=2)
			{
				taxa1 = (String)ancTree.elementAt(i);
				taxa2 = (String)ancTree.elementAt(i+1);
				
				//get location of taxa1
				if(taxaNames.contains(taxa1))
					t1 = taxaNames.indexOf(taxa1);
				else
					t1 = nodeNames.indexOf(taxa1) + taxaNames.size();
				
				//get location of taxa2
				if(taxaNames.contains(taxa2))
					t2 = taxaNames.indexOf(taxa2);
				else
					t2 = nodeNames.indexOf(taxa2) + taxaNames.size();
				
				outFile.println ("\tedge\n\t[\n\t\tlinestyle \"solid\"\n\t\tlabel \"\"\n\t\tsource "+t1+"\n\t\ttarget "+t2+"\n\t]");
			}
	
			//end graph
			outFile.println("]");
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write TCS Tree.\n  Writing to file: "+ fileName);
		}
	}

}//TreePrint