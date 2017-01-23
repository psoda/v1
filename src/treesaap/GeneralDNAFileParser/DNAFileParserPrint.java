/**
 *	DNAFileParserPrint.class is responsible for the printing
 *	of information of a given tree.  It will return String values,
 *	easily printed to file.
 *
 *	@author	Joshua Sailsbery
 *	@version	3.0
 *	@since	1.0
 */
package treesaap.GeneralDNAFileParser;

import java.util.Vector;
import java.util.Hashtable;

public class DNAFileParserPrint
{
	//Class Variables - determined by super user
	private DNAFileParserUsageBean bean;		//Settings (ie. variables) used by this class to determine user preferences
	

	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * Initializes variables to be used.
	 * Use this Constructor if bean has been specified - used in conjunction with Driver
	 *
	 * @param Bean DNAFileParserUsageBean given - containing values determined by parent class
	 */
	public DNAFileParserPrint(DNAFileParserUsageBean Bean)
	{
		//set up usage bean
		bean = Bean;
	}//Constructor

	/**
	 *	Returns a String to be written to file representing a given tree's header
	 *	@param TreeBean tree - the tree to be written
	 */
	public String writeTreeHeader(TreeBean tree)
	{		
		try
		{
			//Method vars
			String line, name, rooted;
			
			//Tree name
			name = "Tree "+ tree.getName() +"	";
			
			//Rooted
			if(!tree.getRooted())
				rooted = "[&U]	";
			else
				rooted = "[&R]	";
			
			//Relation
		//	line = name + rooted + tree.getRelation() +"\n";
			
			//Revised Relation
			line = name + rooted + tree.getRevisedRelation() +"\n";

			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access the tree: "+ tree.getName());
			return null;
		}
	}
	
	/**
	 *	Returns a String to be written to file representing a given tree's ancestral tree
	 *	@param TreeBean tree - the tree to be written
	 */
	public String writeTreeAncestralTree(TreeBean tree)
	{		
		try
		{
			//Method vars
			String line = "Ancestral_Tree:	";
			Vector ancestralTree = tree.getAncestralTree();

			//Write out ancestral tree
			for(int i=0;i<ancestralTree.size();i+=2)
				line += ancestralTree.elementAt(i) +".."+ ancestralTree.elementAt(i+1) +"	";	

			line += "\n";

			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access the tree: "+ tree.getName());
			return null;
		}
	}
	
	/**
	 *	Returns a String to be written to file representing a given tree's taxa
	 *	@param TreeBean tree - the tree to be written
	 *	@param Hashtable taxa - the taxa of DNAFileParser
	 */
	public String writeTreeTaxa(TreeBean tree, Hashtable taxa)
	{		
		try
		{
			//Method vars
			TaxaBean taxaBean;
			String line = "Taxa:\n";
			Vector taxaNames = tree.getTaxaNames();
			
			//Write out taxa
			for(int i=0;i<taxaNames.size();i++)
			{
				taxaBean = (TaxaBean)taxa.get((String)taxaNames.elementAt(i));		
				line += taxaBean.getName() + "	" + taxaBean.getSequence() + "\n";		
			}
			
			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access the tree: "+ tree.getName());
			return null;
		}
	}
	
	/**
	 *	Returns a String to be written to file representing a given tree's nodes
	 *	@param TreeBean tree - the tree to be written
	 */
	public String writeTreeNodes(TreeBean tree)
	{		
		try
		{
			//Method vars
			TaxaBean nodeBean;
			String line = "";
			Vector nodeNames = tree.getNodeNames();
			Hashtable nodes = tree.getNodes();
			
			//Write out taxa
			for(int i=0;i<nodeNames.size();i++)
			{
				nodeBean = (TaxaBean)nodes.get((String)nodeNames.elementAt(i));		
				line += nodeBean.getName() + "	" + nodeBean.getSequence() + "\n";		
			}

			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access the tree: "+ tree.getName());
			return null;
		}
	}

}//DNAFileParserPrint