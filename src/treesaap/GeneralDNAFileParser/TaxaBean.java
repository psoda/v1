/**
 *	TaxaBean.class is an instantiated data object that
 *	contains all of the data for a given taxa or node.
 *  Will make available all information for processing in 
 *  the program
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.GeneralDNAFileParser;

public class TaxaBean
{
	//Properties of this taxa
	private String name;				//name of taxa
	private String sequence;			//String of given sequence after editing
	
	//Process Variables
	private int codon[];				//array of numbers representing each codon

	
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
		sequence = Seq;
	}//Constructor
	
	/**
	 * Set the codon array to that passed in
	 * 
	 * @param int[] codon An integer array representing codons 0-64
	 */
	public void setCodon(int[] Codon)
	{
		codon = Codon;
	}//setCodon
	
	/**
	 * Return codon array
	 */
	public int[] getCodon()
	{
		return codon;
	}//getCodon
	
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
