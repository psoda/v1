/**
 *	GeneticCodeControl.class is a parent class for each of the
 *	different Genetic Codes.  It reads in values from file and
 *	sets up pathways with amino acid properties.
 *
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Data;

import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.util.Vector;
import java.util.Hashtable;

class GeneticCodeControl
{
	//Class Vars determined by super class
	private DataUsageBean bean;			//bean of all the settings of this class
	
	//Data Struct -- Genetic Codes
	private Vector geneticCodeNames;		//vector of all the names read in for genetic codes
	private Hashtable geneticCodes;			//hashtable of all the genetic codes and their classes
	
	//Temp Class Vars
	private Vector propertyNames;		//Vector of property names read in
	private String[] geneticFileNames;	//Array of strings of potential files containing genetic code data
	
	
	/**
	 *	GeneticCodeControl Constructor - Instantiates Class Objects.	
	 */
	public GeneticCodeControl(DataUsageBean Bean)
	{
		bean = Bean;
	}//constructor

	/**
	 * Obtains data from files and inputs it into gcBean
	 */
	public boolean obtainData()
	{
		return (getGeneticCodeFiles() && getGeneticCodeData());
		
	}//obtainData

	/**
	 * Reads all files in a given directory into a 
	 * array of strings, denoting names of genetic codes
	 */
	private boolean getGeneticCodeFiles()
	{
		try
		{
			//Create file object and get all files in path
			File file = new File(bean.getGeneticCodePath());
			geneticFileNames = file.list();
			
			if(geneticFileNames.length <= 0)
				throw new Exception();
		}
		catch(Exception exception) 
		{
			bean.errorMessage("\nYou had an Exception while obtaining data from Genetic Code Files in the path ("+ bean.getGeneticCodePath() +").\n  If genetic code files exist in this directory, verify their format.");
			return false;
		}

		return true;
		
	}//getGeneticCodeFiles
	
	/**
	 * Reads all data from files found in getGeneticCodeFiles
	 * into structures stored in gcBean
	 */
	private boolean getGeneticCodeData()
	{
		//Initialize Vars
		String name;
		geneticCodeNames = new Vector();
		geneticCodes = new Hashtable();
		
		//go through all the given file names
		for(int i=0;i<geneticFileNames.length;i++)
		{
			name = readinCodeFile(geneticFileNames[i]);	
			if(name != null)
				geneticCodeNames.add(name);
		}
		
		//test readin success
		if(geneticCodeNames.size() <= 0)
			return false;

		return true;
	
	}//getGeneticCodeData

	/**
	 * Read in genetic code data from a specific file, store data
	 */
	private String readinCodeFile(String fileName)
	{	
		//Method Vars
		String name = null;
		GeneticCodeBean gcBean = new GeneticCodeBean();
		
		//Process Vars
		String line;
		int codon = 0, pos, pos2;
		int[] cClass = new int[64];
		int[] aAcid = new int[64];
		
		//go through file
		try
		{
			BufferedReader inFile = new BufferedReader(new FileReader(bean.getGeneticCodePath() + fileName));
			
			//get each number and input into cClass[]
			while((line = inFile.readLine()) != null)
			{
				pos = line.indexOf("	");
				pos2 = line.lastIndexOf("	");
					
				//number appears after . and a '/t'
				if(pos != -1 && pos2 != -1)
				{	
					aAcid[codon] = Integer.valueOf(line.substring(pos+1,pos2)).intValue();
					cClass[codon] = Integer.valueOf(line.substring(pos2+1)).intValue();					//place number in array
					codon++;
				}
			}
			
			//set-up genetic code name
			pos = fileName.indexOf(".");
			if(pos != -1)
				name = fileName.substring(0,pos);
			else
				name = fileName;
	
			//now place everything in the gcBean and the hashtable
			gcBean.setName(name);
			gcBean.setCClass(cClass);
			gcBean.setAAcid(aAcid);
			geneticCodes.put(name, gcBean);

			//close file
			inFile.close();
			
			return name;
		}
		catch(Exception exception) 
		{
			return null;
		}
	}//readinCodeFile

	/**
	 *	Calculate all the pathways for each codon for each genetic code
	 */
	public void pathways()
	{
		//Method vars
		int num;
		int[] aAcid;
		Vector toCodon;
		Vector[] fromCodon;
		GeneticCodeBean gcBean;
		
		try
		{
			//go thru each genetic code
			for(int i=0;i<geneticCodeNames.size();i++)
			{
				//set-up vars
				gcBean = (GeneticCodeBean)geneticCodes.get((String)geneticCodeNames.elementAt(i));
				aAcid = gcBean.getAAcid();
				fromCodon = new Vector[64];			
				
				//go through each codon
				for(int j=0;j<64;j++)
				{
					//set-up new vector for each pathway
					toCodon = new Vector();
					
					//add only Nsyn pathwys that code for AA
					if(aAcid[j] != 0)
					{					
						//1p pathways
						for(int k=0;k<4;k++)
						{
							num = j%16 + 16*k; 					//locate all 1p 1step pathways
							
							//add all NS pathways to Vector
							if(aAcid[j] != aAcid[num] && aAcid[num] != 0)
								toCodon.add(Integer.valueOf(num +""));
	
						}
						
						//2p pathways
						for(int k=0;k<4;k++)
						{
							num = j%4 + (16*(j/16)) + 4*k; 		//locate all 2p 1step pathways
							
							//add all NS pathways to Vector
							if(aAcid[j] != aAcid[num] && aAcid[num] != 0)
								toCodon.add(Integer.valueOf(num +""));
	
						}
						
						//3p pathways
						for(int k=0;k<4;k++)
						{
							num = (4*(j/4)) + k; 						//locate all 3p 1step pathways
						
							//add all NS pathways to Vector
							if(aAcid[j] != aAcid[num] && aAcid[num] != 0)
								toCodon.add(Integer.valueOf(num +""));	
						}
					}
	
					//add possible pathways to array
					fromCodon[j] = toCodon;			
				}
				//add to given genetic code prop
				gcBean.setFromCodon(fromCodon);	
			}	
		}
		catch(Exception e)
		{
			bean.errorMessage("\nAn error occured while calculating pathways.\n  Please review Genetic Code requirements.");
		}
		
	}//pathways

	/**
	 * Calculate the difference between amino acids in 1-step changes for each property for each genetic code
	 */
	public void calcDiff()
	{
		//method vars
		int[] aAcid;
		float[] pValues;		
		GeneticCodeBean gcBean;
		ProteinBean propBean;
		int num, num1;
		float max, diff;
		Vector toCodon;
		Hashtable properties;
		Vector[] fromCodon, aaPropDiff;
		
		try
		{
			//go thru each genetic code
			for(int i=0;i<geneticCodeNames.size();i++)
			{
				//set-up vars
				gcBean = (GeneticCodeBean)geneticCodes.get((String)geneticCodeNames.elementAt(i));
				properties = gcBean.getProperties();
				fromCodon = gcBean.getFromCodon();
				aAcid = gcBean.getAAcid();
				
				//go thru each property
				for(int j=0;j<propertyNames.size();j++)
				{
					//set-up vars
					propBean = (ProteinBean)properties.get((String)propertyNames.elementAt(j));
					aaPropDiff = new Vector[64];
					pValues = propBean.getPValues();
					max = 0;
					
					//go through each codon
					for(int k=0;k<64;k++)
					{
						//set-up vars
						toCodon = new Vector();
						
						//go through the vector
						for(int z=0;z<fromCodon[k].size();z++)
						{
							//Subtract
							num = aAcid[k]-1;
							num1 = aAcid[((Integer)fromCodon[k].elementAt(z)).intValue()]-1;	
							diff = pValues[num1] - pValues[num];
							
							//increase max
							if(diff > max)	
								max = diff;
							
							//add diff to vector
							toCodon.add(Float.valueOf(diff +""));						
						}
						aaPropDiff[k] = toCodon;
					}		
					propBean.setAAPropDiff(aaPropDiff);
					propBean.setMax(max);
				}
			}
		}
		catch(Exception e)
		{
			bean.errorMessage("\nAn error occured while calculating differences in amino acid 1-step changes.\n  Please review Genetic Code requirements.");
		}
	}//calcDiff

	/**
	 *	Sets this Usagebean to the one passed in
	 *	@param DataUsageBean Bean - the new bean to be used
	 */
	public void setBean(DataUsageBean Bean)
	{
		bean = Bean;		
	}

	/**
	 *	Set method - sets property names used in this class
	 *	@param Vector propNames - Vector of new propNames
	 */
	public void setPropertyNames(Vector propNames)
	{
		propertyNames = propNames;
	}
	
	/**
	 *  Get method - returns Hashtable geneticCodes 
	 */
	public Hashtable getGeneticCodes()
	{
		return geneticCodes;
	}
	
	/**
	 *  Get method - returns Vector geneticCodeNames 
	 */
	public Vector getGeneticCodeNames()
	{
		return geneticCodeNames;
	}
	
}//GeneticCodeControl