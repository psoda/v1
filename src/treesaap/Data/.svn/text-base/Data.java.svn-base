/**
 *	Data.class is a controlling class, directing the obtaining of
 *	different types of data including the Genetic Code and of 
 *	Amino Acid properties and of the Crititcal Values.
 *
 *	@author	Joshua Sailsbery
 *	@version	1.2
 *	@since	1.0
 */
package treesaap.Data;

import java.util.Vector;
import java.util.Hashtable;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.text.DecimalFormat;

public class Data
{
	//Class Vars used for object design
	private DataUsageBean bean;					//bean of all the settings of this class
	private CriticalValueControl critValCntrl;	//Control code - used to get CriticalValueControl	
	private GeneticCodeControl genCodeCntrl;	//Control code - used to get GeneticCodeControl
	private ProteinControl proteinCntrl;		//Control code - used to get ProteinControl
	private TableBean tableBean;				//Contains String values of codons and amino acids

	
	/**
	 *	Data Constructor - Instantiates Class Objects.	
	 */
	public Data()
	{
		bean = new DataUsageBean();
		critValCntrl = new CriticalValueControl(bean);
		genCodeCntrl = new GeneticCodeControl(bean);
		proteinCntrl = new ProteinControl(bean);
		tableBean = new TableBean();

	}//constructor

	/**
	 *	Data Constructor - Instantiates Class Objects.	
	 *	@param DataUsageBean bean - class created with this bean
	 */
	public Data(DataUsageBean Bean)
	{
		bean = Bean;
		critValCntrl = new CriticalValueControl(bean);
		genCodeCntrl = new GeneticCodeControl(bean);
		proteinCntrl = new ProteinControl(bean);
		tableBean = new TableBean();
		
	}//constructor
	
	/**
	 * Obtains data from files and inputs it into data struct
	 */
	public void obtainData()
	{
		//read in critical values
		if(!critValCntrl.obtainData())
			return;
		
		//read in genetic code values
		if(!genCodeCntrl.obtainData())
			return;
		
		//read in property values
		if(!proteinCntrl.obtainData())
			return;
		
	}//obtainData

	/**
	 * Places copies of properties in each genetic code
	 */
	private void assignProperties()
	{
		//Method vars
		String name;
		GeneticCodeBean gcBean;
		ProteinBean propBean, newPropBean;
		
		//get genetic codes
		Vector geneticCodeNames = getGeneticCodeNames();
		Hashtable geneticCodes = getGeneticCodes();
		
		//get properties
		Vector propertyNames = getPropertyNames();
		Hashtable copyOfProperties;
		Hashtable properties = getProperties();
		
		//go through each genetic code
		for(int i=0;i<geneticCodeNames.size();i++)
		{
			//create new hashtable and assign it values
			copyOfProperties = new Hashtable();
			for(int j=0;j<propertyNames.size();j++)
			{
				name = (String)propertyNames.elementAt(j);
				propBean = (ProteinBean)properties.get(name);
				newPropBean = new ProteinBean();
				newPropBean.setName(propBean.getName());
				newPropBean.setNumbers(propBean.getNumbers());
				newPropBean.setPValues(propBean.getPValues());
				copyOfProperties.put(name, newPropBean);
			}
			
			//assing new properties hashtable
			gcBean = (GeneticCodeBean)geneticCodes.get((String)geneticCodeNames.elementAt(i));
			gcBean.setPropertyNames(propertyNames);
			gcBean.setProperties(copyOfProperties);
		}
	}
	
	/**
	 *	Sets up pathways and determines amount of change
	 */
	public void setPathways()
	{
		//Pathway Set-up
		assignProperties();
		genCodeCntrl.setPropertyNames(proteinCntrl.getPropertyNames());
		
		//Pathway determination
		genCodeCntrl.pathways();
		genCodeCntrl.calcDiff();
	}	
	
	/**
	 *	Creates a property list
	 */
	public void createLists(String name, Vector list)
	{
		getPropertyListNames().add(name);
		getPropertyLists().put(name, list);
		writeLists();
	}

	/**
	 *	Writes the property lists to file
	 */
	public void writeLists()
	{
		proteinCntrl.writeLists();
	}	
        
        public void writeList(String name) 
        {
            proteinCntrl.writeList(name);
        }
	
	/**
	 *	Returns a String to be written to file representing the genetic codes
	 */
	public String writeGeneticCodes()
	{		
		try
		{
			//Method vars
			String line = "", name;
			int[] cClass, aAcid;
			Vector[] fromCodon;
			Vector geneticCodeNames = getGeneticCodeNames();
			Hashtable geneticCodes = getGeneticCodes();
			
			//to through all the genetic codes
			for(int i=0;i<geneticCodeNames.size();i++)
			{
				name = (String)geneticCodeNames.elementAt(i);
				cClass = ((GeneticCodeBean)geneticCodes.get(name)).getCClass();
				aAcid = ((GeneticCodeBean)geneticCodes.get(name)).getAAcid();
				fromCodon = ((GeneticCodeBean)geneticCodes.get(name)).getFromCodon();
				
				line += name +"\n";	
				for(int j=0;j<64;j++)
				{
					line += tableBean.getCodonName(j) +": (";
					line += tableBean.getAAName(aAcid[j])+ ","+ cClass[j] +")	(";
	
					for(int k=0;k<fromCodon[j].size();k++)
						line += tableBean.getCodonName(((Integer)fromCodon[j].elementAt(k)).intValue()) +",";

					if(fromCodon[j].size() > 0)
						line = line.substring(0,line.length()-1) +")\n";
					else
						line += ")\n";
				}
				line += "\n";
			}
			
			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access genetic codes");
			return null;
		}
	}	
	
	/**
	 *	Returns a String to be written to file representing the properties
	 */
	public String writeProperties()
	{		
		try
		{
			//Method vars
			String line = "", name;
			float[] pValues;
			Vector propertyNames = getPropertyNames();
			Hashtable properties = getProperties();
			TableBean tBean = new TableBean();
			
			line = "Property Name";
			for(int i=1;i<21;i++)
				line += "	"+tBean.getAAName(i);
			line += "\n";
			
			//to through all the properties
			for(int i=0;i<propertyNames.size();i++)
			{
				name = (String)propertyNames.elementAt(i);
				pValues = ((ProteinBean)properties.get(name)).getPValues();
				
				line += name;	
				for(int j=0;j<20;j++)
				{
					line += "	"+pValues[j];
				}
				line += "\n";
			}
			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access the amino acid properties");
			return null;
		}
	}		

	/**
	 *	Returns a String to be written to file representing the properties by list
	 *	@param String listName - the name of the list to write
	 */
	public String writePropertiesByList(String listName)
	{		
		try
		{
			//Method vars
			String line = "", name;
			float[] pValues;
			Vector propertyNames = (Vector)getPropertyLists().get(listName);
			Hashtable properties = getProperties();

			//to through all the properties
			for(int i=0;i<propertyNames.size();i++)
			{
				name = (String)propertyNames.elementAt(i);
				pValues = ((ProteinBean)properties.get(name)).getPValues();

				line += name +"\n";	
				for(int j=0;j<20;j++)
				{
					line += pValues[j] +", ";
				}
				line = line.substring(0,line.length()-2) +"\n";
			}
			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access genetic codes");
			return null;
		}
	}

	/**
	 *	Returns a String to be written to file representing the z-scores
	 */
	public String writeZScores()
	{		
		try
		{
			//Method vars
			String line = "Z-Scores:	";
			
			//write the z-scores
			double[] z = getZCriticalValues();
			line += z[0] +" "+ z[1] +" "+ z[2] +"\n";
			
			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access the Z-Scores");
			return null;
		}
	}		
	
	/**
	 *	Returns a String to be written to file representing the gf-scores
	 */
	public String writeGFScores()
	{		
		try
		{
			//Method vars
			String line = "GF-Scores:\n";
			double[] gfTemp;
			Hashtable gf = getGFCriticalValues();
			
			//go through GF-Scores
			for(int i=1;i<30;i++)
			{
				gfTemp = (double[])gf.get(i+"");
				line += i +":	"+ gfTemp[0] +" "+ gfTemp[1] +" "+ gfTemp[2] +"\n";
			}
			
			return line;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access the GF-Scores");
			return null;
		}
	}		
	
	/**
	 *	Returns a String to be written to file representing the pathways
	 *	@param String fileName - the fileName to print all the pathways to due to extreme amount of data, must be written straight to file
	 */
	public void writePathways(String fileName)
	{		
		//Method vars
		ProteinBean propBean;
		GeneticCodeBean gcBean;
		String gcName, propName, table, entry;
		
		//Objects
		Hashtable properties;
		Vector propertyNames = getPropertyNames();
		Vector geneticCodeNames = getGeneticCodeNames();
		Hashtable geneticCodes = getGeneticCodes();
		DecimalFormat form = new DecimalFormat("0.##");
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//to through all the genetic codes
			for(int i=0;i<geneticCodeNames.size();i++)
			{
				gcName = (String)geneticCodeNames.elementAt(i);
				gcBean = (GeneticCodeBean)geneticCodes.get(gcName);
				properties = gcBean.getProperties();

				//Set Genetic Code Name
				outFile.println(gcName);	
				
				//go through each property of each genetic code
				for(int j=0;j<propertyNames.size();j++)
				{
					propName = (String)propertyNames.elementAt(j);
					propBean = (ProteinBean)properties.get(propName);
					
					//Set name and max
					outFile.println(propName +"	MAX: "+ form.format(propBean.getMax()));
					
					//write each pathway table
					for(int k=0;k<64;k++)
					{
						//table = "";
						table = tableBean.getCodonName(k) +": ";
						for(int z=0;z<propBean.getAAPropDiff()[k].size();z++)
						{
							entry = "("+ tableBean.getCodonName(((Integer)gcBean.getFromCodon()[k].elementAt(z)).intValue()) +","+ form.format(propBean.getAAPropDiff()[k].elementAt(z)) +"), ";
							
							/*entry = "("+ tableBean.getCodonName(k) +","+ tableBean.getCodonName(((Integer)gcBean.getFromCodon()[k].elementAt(z)).intValue()) +"), ";
							entry += "("+ tableBean.getAAName(gcBean.getAAcid()[k]-1) +","+ tableBean.getAAName(gcBean.getAAcid()[((Integer)gcBean.getFromCodon()[k].elementAt(z)).intValue()]-1) +"), ";							
							entry += "("+ propBean.getPValues()[gcBean.getAAcid()[k]-1] +","+ propBean.getPValues()[gcBean.getAAcid()[((Integer)gcBean.getFromCodon()[k].elementAt(z)).intValue()]-1] +") ";
							entry += form.format(propBean.getAAPropDiff()[k].elementAt(z));
							entry += "\n";*/
							
							table += entry;
						}
						outFile.println(table.substring(0,table.length()-2));
					}
					outFile.println();
				}
				outFile.println();
			}
			//close file
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to access genetic codes and the properties assigned to them.");
		}
	}		
	
	/**
	 * Returns the bean of user settings for Data
	 */
	public DataUsageBean getBean()
	{
		return bean;
	}	

	/**
	 *	Sets this Usagebean to the one passed in
	 *	@param DataUsageBean Bean - the new bean to be used
	 */
	public void setBean(DataUsageBean Bean)
	{
		bean = Bean;
		critValCntrl.setBean(bean);
		genCodeCntrl.setBean(bean);
		proteinCntrl.setBean(bean);
		
	}
	
	/**
	 * Returns the tableBean of string values for codons and aminoacids
	 */
	public TableBean getTableBean()
	{
		return tableBean;
	}
	
	/**
	 *  Get method - returns Hashtable gfCriticalValues 
	 */
	public Hashtable getGFCriticalValues()
	{
		return critValCntrl.getGFCriticalValues();
	}

	/**
	 *  Get method - returns double[] zCriticalValues 
	 */
	public double[] getZCriticalValues()
	{
		return critValCntrl.getZCriticalValues();
	}

	/**
	 *  Get method - returns Hashtable geneticCodes 
	 */
	public Hashtable getGeneticCodes()
	{
		return genCodeCntrl.getGeneticCodes();
	}
	
	/**
	 *  Get method - returns Vector geneticCodeNames 
	 */
	public Vector getGeneticCodeNames()
	{
		return genCodeCntrl.getGeneticCodeNames();
	}
	
	/**
	 * Get method - returns Hashtable properties 
	 */
	public Hashtable getProperties()
	{
		return proteinCntrl.getProperties();
	}

	/**
	 *  Get method - returns Vector propertyNames 
	 */
	public Vector getPropertyNames()
	{
		return proteinCntrl.getPropertyNames();
	}
	 
	/**
	 *  Get method - returns Hashtable propertyLists
	 */
	public Hashtable getPropertyLists()
	{
		return proteinCntrl.getPropertyLists();
	}
	
	/**
	 *  Get method - returns Vector propertyListNames 
	 */
	public Vector getPropertyListNames()
	{
		return proteinCntrl.getPropertyListNames();
	}
	
	/**
	 *  Delete method - deletes from propertyLists those in the delete vector
	 */
	public void deletePropertyLists()
	{
		proteinCntrl.deletePropertyLists();
	}
	
}//Data