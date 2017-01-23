/**
 *	GeneticCodeBean.class is an instantiated data object that
 *	contains all of the values of a given geneticCode
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Data;

import java.util.Vector;
import java.util.Hashtable;

public class GeneticCodeBean
{
	//Data from file
	private String name;					//name of this geneticCode
	private int[] cClass;					//array representing class of codon for a given genetic code
	private int[] aAcid;					//array representing amino acid, sorted by codon number
	
	//Derived Data
	private Vector propertyNames;			//Vector of property names
	private Hashtable properties;			//hashtable of all the properties and their corresponding values
	private Vector[] fromCodon;				//array of Vectors that denote the possible pathways
	
	
	/**
	 *  Set method - sets propertyNames to new Vector
	 *	@param Vector PropertyNames of new propertyNames
	 */
	public void setPropertyNames(Vector PropertyNames)
	{
		propertyNames = PropertyNames;
	}
	
	/**
	 *  Get method - returns Vector propertyNames 
	 */
	public Vector getPropertyNames()
	{
		return propertyNames;
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
	 *  Set method - sets cClass to new int[]
	 *	@param int[] CClass of new cClass
	 */
	public void setCClass(int[] CClass)
	{
		cClass = CClass;
	}
	
	/**
	 *  Get method - returns int[] cClass 
	 */
	public int[] getCClass()
	{
		return cClass;
	}
	
	/**
	 *  Set method - sets aAcid to new int[]
	 *	@param int[] AAcid of new aAcid
	 */
	public void setAAcid(int[] AAcid)
	{
		aAcid = AAcid;
	}
	
	/**
	 *  Get method - returns String aAcid 
	 */
	public int[] getAAcid()
	{
		return aAcid;
	}
	
	/**
	 *  Set method - sets fromCodon to new Vector[]
	 *	@param Vector[] FromCodon of new fromCodon
	 */
	public void setFromCodon(Vector[] FromCodon)
	{
		fromCodon = FromCodon;
	}
	
	/**
	 *  Get method - returns Vector[] fromCodon 
	 */
	public Vector[] getFromCodon()
	{
		return fromCodon;
	}
	
	/**
	 *  Set method - sets properties to new Hashtable
	 *	@param Hashtable props of new properties
	 */
	public void setProperties(Hashtable props)
	{
		properties = props;
	}
	
	/**
	 *  Get method - returns Hashtable properties 
	 */
	public Hashtable getProperties()
	{
		return properties;
	}

}//GeneticCodeBean