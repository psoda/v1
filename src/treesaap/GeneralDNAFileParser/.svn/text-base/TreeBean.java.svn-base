/**
 *	TreeBean.class is an instantiated data object that
 *	contains all of the data for a given tree, including ancestral data.
 *  Will make available all information for processing in 
 *  the program
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.GeneralDNAFileParser;

import java.util.Vector;
import java.util.Hashtable;

public class TreeBean
{
	//Basic Properties of this tree
	private String name;				//name of tree
	private String relation;			//String of given relation
	private boolean rooted;				//Whether the given tree is rooted or not
	
	//Extrapulated properties of this tree
	private Vector ancestralTree;		//The comparison tree for this tree
	private String revisedRelation;		//The relation of this tree, cut down of branch lengths

	//Process Vars
	private Vector translateBlock;		//this tree's translate block
	private Vector relationVariables;	//all the variables from the tree relation
	
	//Assigned Objects
	private Vector taxaNames;			//object containing all the taxa names of this tree
	private Vector nodeNames;			//object containing all the node names of this tree
	private Hashtable nodes;			//object containing all the nodes as TaxaBeans of this tree
	
	
	/**
	 * The Empty Constructor for this class
	 */
	public TreeBean(){}

	/**
	 * This constructor takes in a name and a sequence to
	 * easily create this class with the basic variables.
	 *
	 * @param String Name String of the name of the tree
	 * @param String Rel String value of the relations of the tree
	 * @param boolean Rooted whether the tree is rooted
	 * @param Vector TB The translateBlock Vector to be used when interpreting relation
	 */
	public TreeBean(String Name, String Rel, boolean Rooted, Vector TB)
	{
		name = Name;
		relation = Rel;
		rooted = Rooted;
		translateBlock = TB;
		
		//initialize vars not passed in
		ancestralTree = new Vector();

	}//Constructor
	
	/**
	 * Set the Relation to that passed in
	 * 
	 * @param String sequence The string of the new relation
	 */
	public void setRelation(String Relation)
	{
		relation = Relation;
	}//setRelation
	
	/**
	 * Returns the String of the relation
	 */
	public String getRelation()
	{
		return relation;
	}//getRelation

	/**
	 * Set the name to that passed in
	 * 
	 * @param String Name The string of the newest name
	 */
	public void setName(String Name)
	{
		name = Name;
	}//setName
	
	/**
	 * Returns the String of the name
	 */
	public String getName()
	{
		return name;
	}//getName
	
	/**
	 * Set rooted to that passed in
	 * 
	 * @param boolean Rooted Set whether the tree is rooted
	 */
	public void setRooted(boolean Rooted)
	{
		rooted = Rooted;
	}//setRooted
	
	/**
	 * Return rooted
	 */
	public boolean getRooted()
	{
		return rooted;
	}//getRooted
	
	/**
	 * Set translateBlock to that passed in
	 * 
	 * @param Vector TB Set the translate block
	 */
	public void setTranslateBlock(Vector TB)
	{
		translateBlock = TB;
	}//setTranslateBlock
	
	/**
	 * Return translateBlock
	 */
	public Vector getTranslateBlock()
	{
		return translateBlock;
	}//getTranslateBlock
	
	/**
	 * Set ancestralTree to that passed in
	 * 
	 * @param Vector AT Set the ancestral tree
	 */
	public void setAncestralTree(Vector AT)
	{
		ancestralTree = AT;
	}//setAncestralTree
	
	/**
	 * Return ancestralTree
	 */
	public Vector getAncestralTree()
	{
		return ancestralTree;
	}//getAncestralTree

	/**
	 * Set relationVariables to that passed in
	 * 
	 * @param Vector RV Set the relation variables
	 */
	public void setRelationVariables(Vector RV)
	{
		relationVariables = RV;
	}//setRelationVariables
	
	/**
	 * Return relationVariables
	 */
	public Vector getRelationVariables()
	{
		return relationVariables;
	}//getRelationVariables
	
	/**
	 * Set revisedRelation to that passed in
	 * 
	 * @param String RR Set the revised Relation
	 */
	public void setRevisedRelation(String RR)
	{
		revisedRelation = RR;
	}//setRelationVariables
	
	/**
	 * Return relationVariables
	 */
	public String getRevisedRelation()
	{
		return revisedRelation;
	}//getRevisedRelation

	/**
	 * Return taxaNames
	 */
	public Vector getTaxaNames()
	{
		return taxaNames;
	}//getTaxaNames
	
	/**
	 * Set taxaNames to that passed in
	 * 
	 * @param Vector TN Set the taxa names
	 */
	public void setTaxaNames(Vector TN)
	{
		taxaNames = TN;
	}//setTaxaNames
	
	/**
	 * Return nodes
	 */
	public Hashtable getNodes()
	{
		return nodes;
	}//getNodes
	
	/**
	 * Set nodes to that passed in
	 * 
	 * @param Hashtable N Set the nodes
	 */
	public void setNodes(Hashtable N)
	{
		nodes = N;
	}//setNodes

	/**
	 * Return nodeNames
	 */
	public Vector getNodeNames()
	{
		return nodeNames;
	}//getNodeNames
	
	/**
	 * Set nodeNames to that passed in
	 * 
	 * @param Vector NN Set the node names
	 */
	public void setNodeNames(Vector NN)
	{
		nodeNames = NN;
	}//setNodeNames	

}//TreeBean
