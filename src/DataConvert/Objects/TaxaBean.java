/**
 *	TaxaBean.class is an instantiated data object that
 *	contains all of the data for a given taxa or node.
 *  Will make available all information for processing in 
 *  the program
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 */

package DataConvert.Objects;

import java.util.Date;

public class TaxaBean{
	//Properties of this taxa
	private String name;				//name of taxa
	private StringBuffer sequence;			//String of given sequence after editing
	private GenbankBean genInfo;
	
	/**
	 * The Empty Constructor for this class
	 */
	public TaxaBean(){}
	
	/**
	 * This constructor takes in a name and a sequence to
	 * easily create this class with the basic variables.
	 *
	 * @param String Name String of the name of the taxa
	 * @param String Seq String value of the sequence of the taxa
	 */
	public TaxaBean(String Name, String Seq)
	{
		name = Name;
		sequence = new StringBuffer(Seq);
		genInfo = null;
	}//Constructor
	
	public TaxaBean(String Name, String Seq, String Accession, String GI, Date genDate)
	{
		name = Name;
		sequence = new StringBuffer(Seq);
		GenbankBean temp = new GenbankBean(Accession, GI, genDate);
		genInfo = temp;
	}//close Constructor
	
	/**
	 * Set the Sequence to that passed in
	 * 
	 * @param String sequence The string of the new sequence
	 */
	public void setSequence(String Sequence)
	{
		sequence = new StringBuffer(Sequence);
	}//setSequence
	
	/**
	 * Appends the sequence that passed in
	 * 
	 * @param String sequence The string of the new sequence
	 */
	public void appendSequence(String Seq)
	{
		sequence.append(Seq);
	}//appendSequence
	
	/**
	 * Returns the String of the sequence
	 */
	public String getSequence()
	{
		return sequence.toString();
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
	
	public GenbankBean getGenbankInfo()
	{
		return (genInfo);
	}//close getGenbankInfo method
}//TaxaBean
