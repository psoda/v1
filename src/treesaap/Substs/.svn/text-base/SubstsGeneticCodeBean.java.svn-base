/**
 *	SubstsGeneticCodeBean.class is an instantiated data object that
 *	contains all of the values of a given geneticCode as applicable to substs
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Substs;

import java.util.Vector;
import java.util.Hashtable;
import java.text.DecimalFormat;

import treesaap.Data.ProteinBean;

public class SubstsGeneticCodeBean
{
	//Class Vars
	private SubstsUsageBean bean;		//Bean containing all values necessary for operation	
	
	//Derived Data
	private Vector propertyNames;			//Vector of property names
	private Hashtable properties;			//hashtable of all the properties and their corresponding values
	private Vector[] fromCodon;				//array of Vectors that denote the possible pathways

	//At run time data
	private int[] catTotal;					//contains totals of the categories across all properties		
	private Hashtable observedCatTotals;	//contains all the totals for each category under each property

	
	/**
	 * SubstsGeneticCodeBean Constructor
	 * calls assign Categories
	 *
	 * @param SubstsUsageBean Bean - bean containing necessary running information
	 */
	public SubstsGeneticCodeBean(SubstsUsageBean Bean) throws Exception
	{
		bean = Bean;	
		
		if(!bean.getDetailed())
		{
			assignProperties();
			assignCat(bean.getNumberOfCat());
		}
	}//Constructor	

	/**
	 * Places copies of properties in this genetic code
	 */
	private void assignProperties()
	{
		//Method vars
		String name;
		ProteinBean propBean, newPropBean;
		
		//get properties
		fromCodon = bean.getGCBean().getFromCodon();
		propertyNames = bean.getGCBean().getPropertyNames();
		Hashtable copyOfProperties = bean.getGCBean().getProperties();
		properties = new Hashtable();

		//assign hashtable values
		for(int j=0;j<propertyNames.size();j++)
		{
			name = (String)propertyNames.elementAt(j);
			propBean = (ProteinBean)copyOfProperties.get(name);
			newPropBean = new ProteinBean();
			newPropBean.setName(propBean.getName());
			newPropBean.setNumbers(propBean.getNumbers());
			newPropBean.setPValues(propBean.getPValues());
			newPropBean.setAAPropDiff(propBean.getAAPropDiff());
			newPropBean.setMax(propBean.getMax());
			properties.put(name, newPropBean);
		}

		//add to tracking var
		bean.addToWorkDone(propertyNames.size());
	}

	/**
	 *	Assigns each amino acid branch difference a category number
	 *	@param int numCat - the number of categories specified
	 */
	private void assignCat(int numCat) throws Exception
	{
		//method vars
		int num;
		ProteinBean propBean;
		float value, increment;
		int categories [][];
		Vector aaPropDiff;
		Hashtable catByCodon[];	
		Hashtable tempHash;
		DecimalFormat form = new DecimalFormat("0.###");

		//gcBean settings
		int numberOfCat = bean.getNumberOfCat();
		catTotal = new int[numCat];
		observedCatTotals = new Hashtable();
		
		//go through all the amino acid properties
		for(int i=0;i<propertyNames.size();i++)
		{
			//gcBean settings
			observedCatTotals.put((String)propertyNames.elementAt(i), new int[numCat]);
			
			//set-up vars
			propBean = (ProteinBean)properties.get((String)propertyNames.elementAt(i));
			propBean.setObservTable(new int[64][numCat]);
			increment = propBean.getMax()/numCat;
			categories = new int[64][numCat];
			catByCodon = new Hashtable[64];		

			//go through each codon
			for(int j=0;j<64;j++)
			{
				//set-up vars
				categories[j] = new int[numCat];
				tempHash = new Hashtable();
				aaPropDiff = propBean.getAAPropDiff()[j];

				//go thru each vector at this codon - has the difference
				for(int k=0;k<aaPropDiff.size();k++)
				{
					//set-up vars
					value = ((Float)aaPropDiff.elementAt(k)).floatValue();
					num = Math.abs((int)(value/increment));
					
					//Check for the max
					if(num >= numCat)	num = numCat-1;
					
					//increment the occurence of this category, place in hashtable
					categories[j][num]++;
			
					tempHash.put(fromCodon[j].elementAt(k)+"",(num+1)+"	"+form.format(value));
					/*if(value > 0)
						tempHash.put(fromCodon[j].elementAt(k)+"",(num+1)+"	+");
					else
						tempHash.put(fromCodon[j].elementAt(k)+"",(num+1)+"	-");	*/			
				}
				//add tempHash to catByCodon
				catByCodon[j] = tempHash;
			}
			//add to tracking var
			bean.addToWorkDone(65);
			
			//assign categories to each property
			propBean.setCategories(categories);
			
			//Assign each property the categories that are assoicated with each pathway
			propBean.setCatByCodon(catByCodon);
		}
	}//assignCat		

	/**
	 *	Return the category for this change for each property
	 *
	 *	@param int codon1	from codon1
	 *	@param int codon2	to codon2
	 */
	public Vector getCat(int codon1, int codon2) throws Exception
	{		
		//method vars
		Vector catVect = new Vector();
		ProteinBean propBean;
		String cat;
		int catNum;
		DecimalFormat form = new DecimalFormat("0.###");
		
		//gcBean settings
		int numberOfCat = bean.getNumberOfCat();
		int[] aAcid = bean.getAAcid();

		//go through each GeneticCode - build catVect
		for(int i=0;i<propertyNames.size();i++)
		{
			propBean = (ProteinBean)properties.get((String)propertyNames.elementAt(i));
			cat = (String)propBean.getCatByCodon()[codon1].get(codon2+"");
			
			//check to see if codons are a 2/3 step change
			if(cat == null)
			{
				//set-up vars
				float value = propBean.getPValues()[aAcid[codon2]-1] - propBean.getPValues()[aAcid[codon1]-1];
				int num = Math.abs((int)((value*numberOfCat)/propBean.getMax()));
				
				//Check for the max
				if(num >= numberOfCat)	num = numberOfCat-1;
				
				//increment the occurence of this category, place in hashtable
				cat = (num+1)+"	"+form.format(value);
				/*if(value > 0)
					cat = (num+1)+"	+";
				else if(value < 0)
					cat = (num+1)+"	-";
				else 
					cat = (num+1)+" 0";*/
			}
			
			//finally add cat to vector
			catVect.add(cat);
			
			//add to category that is placed in the total observed hashtable and total category array
			catNum = Integer.valueOf(cat.substring(0,cat.indexOf("	"))).intValue() -1;
			catTotal[catNum]++;
			((int[])observedCatTotals.get((String)propertyNames.elementAt(i)))[catNum]++;
			propBean.getObservTable()[codon1][catNum]++;
		}
		
		//return computed categories for each prop
		return catVect;
	}	

	/**
	 *  Set method - sets catTotal to new int[]
	 *	@param int[] CatTotal of new catTotal
	 */
	public void setCatTotal(int[] CatTotal)
	{
		catTotal = CatTotal;
	}
	
	/**
	 *  Get method - returns int[] catTotal 
	 */
	public int[] getCatTotal()
	{
		return catTotal;
	}
	
	/**
	 *  Set method - sets observedCatTotals to new Hashtable
	 *	@param Hashtable ObservedCatTotals of new observedCatTotals
	 */
	public void setObservedCatTotals(Hashtable ObservedCatTotals)
	{
		observedCatTotals = ObservedCatTotals;
	}
	
	/**
	 *  Get method - returns Hashtable observedCatTotals 
	 */
	public Hashtable getObservedCatTotals()
	{
		return observedCatTotals;
	}
	
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

}//SubstsGeneticCodeBean