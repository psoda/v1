/**
 *	BASEMLTreeBean.class is an instantiated data object that
 *	contains all of the data for a given tree that BASEML needs.
 *  Will make available all information for processing in 
 *  the program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.BASEML;

import java.util.Vector;
import java.util.Hashtable;

public class BASEMLTreeBean
{
	//Basic Properties of this tree
	private String name;				//name of tree
	private String relation;			//String of given relation
	private boolean rooted;				//Whether the given tree is rooted or not
	
	//Assigned Objects
	private Vector taxaNames;			//object containing all the taxa names of this tree
	private Hashtable taxa;				//object containing all the taxa
	
	//Process Vars
	private boolean ran;				//whether tree has already been ran
	
	
	/**
	 * The Empty Constructor for this class
	 */
	public BASEMLTreeBean(){}
	
	/**
	 * This constructor takes in a name and a sequence to
	 * easily create this class with the basic variables.
	 *
	 * @param String Name String of the name of the tree
	 * @param String Rel String value of the relations of the tree
	 * @param boolean Rooted whether the tree is rooted
	 * @param Vector TN The taxaNames Vector to be used when reading taxa for this tree
	 * @param Hashtable Taxa the table containing all the taxa references
	 */
	public BASEMLTreeBean(String Name, String Rel, boolean Rooted, Vector TN, Hashtable Taxa, boolean Ran)
	{
		name = Name;
		relation = Rel;
		rooted = Rooted;
		taxaNames = TN;
		taxa = Taxa;
		ran = Ran;
		
	}//Constructor
	
	/**
	 * Set ran to that passed in
	 * 
	 * @param boolean Ran - Set whether the tree has been ran
	 */
	public void setRan(boolean Ran)
	{
		ran = Ran;
	}//setRan
	
	/**
	 * Return ran
	 */
	public boolean getRan()
	{
		return ran;
	}//getRan
	
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
	 * Return taxa
	 */
	public Hashtable getTaxa()
	{
		return taxa;
	}//getTaxa
	
	/**
	 * Set taxa to that passed in
	 * 
	 * @param Vector Taxa Set the taxa
	 */
	public void setTaxa(Hashtable Taxa)
	{
		taxa = Taxa;
	}//setTaxa
	
	
}