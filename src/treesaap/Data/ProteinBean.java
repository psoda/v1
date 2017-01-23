/**
 *	ProteinBean.class is an instantiated data object that
 *	contains all of the values of a given property
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Data;

import java.util.Vector;
import java.util.Hashtable;

public class ProteinBean
{
	private String name;					//name of this property
	private String numbers;					//all values for this property
	private float[] pValues;				//array of all the values in for a given property
	private float max;						//max value of this property for a given genetic code
	private Vector[] aaPropDiff;			//array of Vectors that have the amino acid differences from the denoted possible pathways
	private int categories[][];				//array of where all the differences tally category counts
	private Hashtable catByCodon[];			//the categories that are assoicated with each pathway
	private int observTable[][];			//double integer array representing the substitutions observed in sequence by codon
	private int codonTable[][];				//double integer array representing the values of codons observed in sequence * possible pathways

	
	/**
	 *  Set method - sets catByCodon to new String
	 *	@param Hashtable[] CatByCodon of new catByCodon
	 */
	public void setCatByCodon(Hashtable[] CatByCodon)
	{
		catByCodon = CatByCodon;
	}
	
	/**
	 *  Get method - returns Hashtable[] catByCodon 
	 */
	public Hashtable[] getCatByCodon()
	{
		return catByCodon;
	}
	
	/**
	 *  Set method - sets codonTable to new int[][]
	 *	@param int[][] CodonTable of new codonTable
	 */
	public void setCodonTable(int[][] CodonTable)
	{
		codonTable = CodonTable;
	}
	
	/**
	 *  Get method - returns int[][] codonTable 
	 */
	public int[][] getCodonTable()
	{
		return codonTable;
	}
	
	/**
	 *  Set method - sets observTable to new int[][]
	 *	@param int[][] ObservTable of new observTable
	 */
	public void setObservTable(int[][] ObservTable)
	{
		observTable = ObservTable;
	}
	
	/**
	 *  Get method - returns int[][] observTable 
	 */
	public int[][] getObservTable()
	{
		return observTable;
	}
	
	/**
	 *  Set method - sets categories to new int[][]
	 *	@param int[][] Categories of new categories
	 */
	public void setCategories(int[][] Categories)
	{
		categories = Categories;
	}
	
	/**
	 *  Get method - returns int[][] categories 
	 */
	public int[][] getCategories()
	{
		return categories;
	}
	
	/**
	 *  Set method - sets name to new String
	 *	@param String Name of new name
	 */
	public void setName(String Name)
	{
		name = Name;
	}
	
	/**
	 *  Get method - returns String name 
	 */
	public String getName()
	{
		return name;
	}
	
	/**
	 *  Set method - sets numbers to new String
	 *	@param String Numbers of new numbers
	 */
	public void setNumbers(String Numbers)
	{
		numbers = Numbers;
	}
	
	/**
	 *  Get method - returns String numbers 
	 */
	public String getNumbers()
	{
		return numbers;
	}
	
	/**
	 *  Set method - sets pValues to new String
	 *	@param float[] PValues of new pValues
	 */
	public void setPValues(float[] PValues)
	{
		pValues = PValues;
	}
	
	/**
	 *  Get method - returns float[] pValues 
	 */
	public float[] getPValues()
	{
		return pValues;
	}
	
	/**
	 *  Set method - sets aaPropDiff to new Vector[]
	 *	@param Vector[] AAPropDiff of new aaPropDiff
	 */
	public void setAAPropDiff(Vector[] AAPropDiff)
	{
		aaPropDiff = AAPropDiff;
	}
	
	/**
	 *  Get method - returns Vector[] aaPropDiff 
	 */
	public Vector[] getAAPropDiff()
	{
		return aaPropDiff;
	}
	
	/**
	 *  Set method - sets max to new float
	 *	@param float Max of new max
	 */
	public void setMax(float Max)
	{
		max = Max;
	}
	
	/**
	 *  Get method - returns float max 
	 */
	public float getMax()
	{
		return max;
	}
	
}//ProteinBean