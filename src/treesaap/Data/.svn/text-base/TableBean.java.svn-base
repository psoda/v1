/**
 *	TableBean.class is an instantiated data object that
 *	contains all of the values of amino acids and codons
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Data;

import java.util.Vector;

public class TableBean
{
	//From number to String
	private String[] aaNames;					//array of Strings, each representing an amino acid's name
	private String[] codonNames;				//array of Strings, each representing an codon's name
	
	//From String to Number
	private Vector aaNumbers;					//Vector of Strings, each representing an amino acid's name
	private Vector codonNumbers;				//Vector of Strings, each representing an codon's name
	

	/**
	 *	Constructor
	 *	Set's up string arrays for access to string representation of codons and amino acids
	 */
	public TableBean()
	{
		codonNames = new String[64];
		aaNames = new String[20];
		
		//Set Amino Acid Names	
		aaNames[0] = "Ala";
		aaNames[1] = "Arg";
		aaNames[2] = "Asn";
		aaNames[3] = "Asp";
		aaNames[4] = "Cys";
		aaNames[5] = "Gln";
		aaNames[6] = "Glu";
		aaNames[7] = "Gly";
		aaNames[8] = "His";
		aaNames[9] = "Ile";
		aaNames[10] = "Leu";
		aaNames[11] = "Lys";
		aaNames[12] = "Met";
		aaNames[13] = "Phe";
		aaNames[14] = "Pro";
		aaNames[15] = "Ser";
		aaNames[16] = "Thr";
		aaNames[17] = "Trp";
		aaNames[18] = "Tyr";
		aaNames[19] = "Val";
		
		//Set Nucleotide values into string array to be accessed
		//T Block            
		codonNames[0] = "TTT";   
		codonNames[1] = "TTC";   
		codonNames[2] = "TTA";   
		codonNames[3] = "TTG";   
		codonNames[4] = "TCT";   
		codonNames[5] = "TCC";   
		codonNames[6] = "TCA";   
		codonNames[7] = "TCG";   
		codonNames[8] = "TAT";   
		codonNames[9] = "TAC";   
		codonNames[10] = "TAA";  
		codonNames[11] = "TAG";  
		codonNames[12] = "TGT";  
		codonNames[13] = "TGC";  
		codonNames[14] = "TGA";  
		codonNames[15] = "TGG";  
		
		//C Block            
		codonNames[16] = "CTT";  
		codonNames[17] = "CTC";  
		codonNames[18] = "CTA";  
		codonNames[19] = "CTG";  
		codonNames[20] = "CCT";  
		codonNames[21] = "CCC";  
		codonNames[22] = "CCA";  
		codonNames[23] = "CCG";  
		codonNames[24] = "CAT";  
		codonNames[25] = "CAC";  
		codonNames[26] = "CAA";  
		codonNames[27] = "CAG";  
		codonNames[28] = "CGT";  
		codonNames[29] = "CGC";  
		codonNames[30] = "CGA";  
		codonNames[31] = "CGG";  
		
		//A Block            
		codonNames[32] = "ATT";  
		codonNames[33] = "ATC";  
		codonNames[34] = "ATA";  
		codonNames[35] = "ATG";  
		codonNames[36] = "ACT";  
		codonNames[37] = "ACC";  
		codonNames[38] = "ACA";  
		codonNames[39] = "ACG";  
		codonNames[40] = "AAT";  
		codonNames[41] = "AAC";  
		codonNames[42] = "AAA";  
		codonNames[43] = "AAG";  
		codonNames[44] = "AGT";  
		codonNames[45] = "AGC";  
		codonNames[46] = "AGA";  
		codonNames[47] = "AGG";  
		
		//G Block            
		codonNames[48] = "GTT";  
		codonNames[49] = "GTC";  
		codonNames[50] = "GTA";  
		codonNames[51] = "GTG";  
		codonNames[52] = "GCT";  
		codonNames[53] = "GCC";  
		codonNames[54] = "GCA";  
		codonNames[55] = "GCG";  
		codonNames[56] = "GAT";  
		codonNames[57] = "GAC";  
		codonNames[58] = "GAA";  
		codonNames[59] = "GAG";  
		codonNames[60] = "GGT";  
		codonNames[61] = "GGC";  
		codonNames[62] = "GGA";  
		codonNames[63] = "GGG";  	
		
		
		//Place all String values in Vector
		aaNumbers = new Vector();
		for(int i=0;i<20;i++)
			aaNumbers.add(aaNames[i]);
		
		codonNumbers = new Vector();
		for(int i=0;i<64;i++)
			codonNumbers.add(codonNames[i]);
				
	}//Constructor

	/**
	 *  Get method - returns int of amino acid
	 *	@param String name amino acid name
	 */
	public int getAANumber(String name)
	{
		return aaNumbers.indexOf(name);
	}	
	
	/**
	 *  Get method - returns int of codon
	 *	@param String name codon name
	 */
	public int getCodonNumber(String name)
	{
		return codonNumbers.indexOf(name);
	}
	
	/**
	 *  Get method - returns String of amino acid
	 *	@param int num amino acid number
	 */
	public String getAAName(int num)
	{
		if(num <= 0 || num > 20)
			return null;
		
		return aaNames[num-1];
	}			
	
	/**
	 *  Get method - returns String of codon
	 *	@param int num codon number
	 */
	public String getCodonName(int num)
	{
		if(num < 0 || num > 63)
			return null;
		
		return codonNames[num];
	}

	/**
	 *  Set method - sets aaNumbers to new Vector
	 *	@param Vector AANumbers of new aaNumbers
	 */
	public void setAANumbers(Vector AANumbers)
	{
		aaNumbers = AANumbers;
	}
	
	/**
	 *  Get method - returns Vector aaNumbers 
	 */
	public Vector getAANumbers()
	{
		return aaNumbers;
	}
	
	/**
	 *  Set method - sets codonNumbers to new Vector
	 *	@param Vector CodonNumbers of new codonNumbers
	 */
	public void setCodonNumbers(Vector CodonNumbers)
	{
		codonNumbers = CodonNumbers;
	}
	
	/**
	 *  Get method - returns Vector codonNumbers 
	 */
	public Vector getCodonNumbers()
	{
		return codonNumbers;
	}

	/**
	 *  Set method - sets aaNames to new String[]
	 *	@param String[] AANames of new aaNames
	 */
	public void setAANames(String[] AANames)
	{
		aaNames = AANames;
	}
	
	/**
	 *  Get method - returns String[] aaNames 
	 */
	public String[] getAANames()
	{
		return aaNames;
	}

	/**
	 *  Set method - sets codonNames to new String[]
	 *	@param String[] CodonNames of new codonNames
	 */
	public void setCodonNames(String[] CodonNames)
	{
		codonNames = CodonNames;
	}
	
	/**
	 *  Get method - returns String[] codonNames 
	 */
	public String[] getCodonNames()
	{
		return codonNames;
	}

}//TableBean