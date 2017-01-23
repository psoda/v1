/**
 *	TaxaBean.class is an instantiated data object that
 *	contains all of the data for a given taxa or node.
 *  Will make available all information for processing in 
 *  the program
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 */
/* changed name of class to TreeBean to match filename and changed packagename to match directory --Adam R. Teichert 20 Aug 2008 */
package DataConvert.Objects;

public class TreeBean{
	//Properties of this taxa
	private String name;				//name of taxa
	private String sequence;			//String of given sequence after editing
	
	/**
	 * The Empty Constructor for this class
	 */
	public TreeBean(){}
	
	/**
	 * This constructor takes in a name and a sequence to
	 * easily create this class with the basic variables.
	 *
	 * @param String Name String of the name of the taxa
	 * @param String Seq String value of the sequence of the taxa
	 */
	public TreeBean(String Name, String Seq)
	{
		name = Name;
		sequence = Seq;
	}//Constructor
	
	/**
	 * Set the Sequence to that passed in
	 * 
	 * @param String sequence The string of the new sequence
	 */
	public void setSequence(String Sequence)
	{
		sequence = Sequence;
	}//setSequence
	
	/**
	 * Appends the sequence that passed in
	 * 
	 * @param String sequence The string of the new sequence
	 */
	public void appendSequence(String Seq)
	{
		sequence = sequence.concat(Seq);
	}//appendSequence
	
	/**
	 * Returns the String of the sequence
	 */
	public String getSequence()
	{
		return sequence;
	}//getSequence

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
	
}//TaxaBean
