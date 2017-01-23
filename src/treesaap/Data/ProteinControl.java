/**
 *	ProteinControl.class is a parent class for each of the
 *	different amino acid properties.  It reads in values from file.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Data;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.util.Vector;
import java.util.Hashtable;
import java.util.StringTokenizer;

class ProteinControl
{
	//Class Vars determined by super class
	private DataUsageBean bean;			//bean of all the settings of this class

	//Data Struct -- Genetic Codes
	private Vector propertyNames;		//vector of all the names read in for each property
	private Hashtable properties;		//hashtable of all the properties
	private Vector propertyListNames;	//Vector of all the names of lists of properties	
	private Hashtable propertyLists;	//Hashtable containing lists of property names - predefined user lists

	//Temp Class Vars
	private String[] propertyFileNames;	//Array of strings of potential files containing genetic code data
	
	
	/**
	 *	GeneticCodeControl Constructor - Instantiates Class Objects.	
	 */
	public ProteinControl(DataUsageBean Bean)
	{
		bean = Bean;
	}//constructor
	
	/**
	 * Obtains data from files and inputs it into propBean
	 */
	public boolean obtainData()
	{
		return (getPropertyFiles() && getPropertyData());
		
	}//obtainData
	
	/**
	 * Reads all files in a given directory into a 
	 * array of strings, denoting names of property files
	 */
	private boolean getPropertyFiles()
	{
		try
		{
			//Create file object and get all files in path
			File file = new File(bean.getPropertyPath());
			propertyFileNames = file.list();
			
			if(propertyFileNames.length <= 0)
				throw new Exception();
		}
		catch(Exception exception) 
		{
			bean.errorMessage("\nYou had an Exception while obtaining data from Property Files in the path ("+ bean.getPropertyPath() +").\n  If Property files exist in this directory, verify their format.");
			return false;
		}
		
		return true;
		
	}//getPropertyFiles
	
	/**
	 * Reads all data from files found in getPropertyFiles
	 * into structures stored in propBean
	 */
	private boolean getPropertyData()
	{
		//Initialize Vars
		String name;
		propertyNames = new Vector();
		properties = new Hashtable();
		propertyListNames = new Vector();
		propertyLists = new Hashtable();
		
		//go through all the given file names
		for(int i=0;i<propertyFileNames.length;i++)
		{
			if(!readinPropertyFile(propertyFileNames[i]))
				readinPropertyList(propertyFileNames[i]);
		}
		
		//test readin success
		if(propertyNames.size() <= 0)
			return false;
		
		//Verify property lists
		Vector propListBean;
		for(int i=0;i<propertyListNames.size();i++)
		{
			propListBean = (Vector)propertyLists.get((String)propertyListNames.elementAt(i));
			
			//go through this vector and match each name to one in propertyNames
			for(int j=0;j<propListBean.size();j++)
			{
				if(!propertyNames.contains((String)propListBean.elementAt(j)))
				{
					propListBean.remove(j);
					j--;
				}
			}
			
			//remove from hashtable and propertyListNames
			if(propListBean.size() <= 0)
			{
				propertyLists.remove((String)propertyListNames.elementAt(i));
				propertyListNames.remove(i);
				i--;
			}
		}

		return true;
		
	}//getPropertyData

	/**
	 * Read in property data from a specific file, store data
	 */
	private boolean readinPropertyFile(String fileName)
	{	
		//Method Vars
		float[] pValues;
		ProteinBean propBean;

		//Process Vars
		StringTokenizer st;
		int aacid = 0, pos;
		String line, name, numbers, num;
		
		//go through file
		try
		{
			BufferedReader inFile = new BufferedReader(new FileReader(bean.getPropertyPath() + fileName));

			//get each name and place in propertyNames
			while((line = inFile.readLine()) != null)
			{				
				pos = line.indexOf("	");
				name = line.substring(0,pos);	
				numbers = line.substring(pos+1);
					
				//Do not add ambiguous properties	
				if(numbers.indexOf("NA") == -1 && numbers.indexOf("Ala") == -1)
				{
					//initialize vars
					pValues = new float[20];
					propBean = new ProteinBean();
					aacid = 0;
						
					//Tokenize the numbers string and parse through it
					st = new StringTokenizer(numbers);
					while(st.hasMoreTokens())
					{
						num = st.nextToken();
						pValues[aacid] = Float.valueOf(num).floatValue();
						aacid++;
					}					
						
					//now place everything in the propBean
					propBean.setName(name);
					propBean.setNumbers(numbers);
					propBean.setPValues(pValues);
				
					//place name in String Vector and propBean in hashtable
					propertyNames.add(name);			
					properties.put(name, propBean);
				}
			}	
		
			inFile.close();
			return true;
		}
		catch(Exception exception) 
		{
			return false;
		}
	}//readinPropertyFile
	
	/**
	 * Read in property data from a specific file, store data
	 */
	private boolean readinPropertyList(String fileName)
	{	
		//Method Vars
		int pos;
		String line, name = null;
		Vector names = new Vector();
		
		//go through file
		try
		{
			BufferedReader inFile = new BufferedReader(new FileReader(bean.getPropertyPath() + fileName));
			line = inFile.readLine();
			
			//Get to line with Amino Acid Property List:
			while(line.indexOf("Amino Acid Property List:") == -1)
				line = inFile.readLine();
			
			//set name
			name = line.substring(line.indexOf(":")+1).trim();	
			
			//get each name and place in propertyNames
			while((line = inFile.readLine()) != null)
			{			
				names.add(line);
			}
			
			if(name == null)
				return false;
			
			//place lists in data structs
			propertyListNames.add(name);
			propertyLists.put(name, names);

			//close file
			inFile.close();
			
			return true;
		}
		catch(Exception exception) 
		{
			return false;
		}
	}//readinPropertyFile
	
	
	/**
	 *	Writes the property lists to file
	 */
	public void writeLists()
	{
		//Method vars
		Vector list;
		String name;
		
		//go through all propertyLists
		for(int i=0;i<propertyListNames.size();i++)
		{
        //get name
        name = (String) propertyListNames.elementAt(i);
        
            writeList(name);
		}
	}	

	/**
	 *	Sets this Usagebean to the one passed in
	 *	@param DataUsageBean Bean - the new bean to be used
	 */
	public void setBean(DataUsageBean Bean)
	{
		bean = Bean;		
	}

	/**
	 *  Get method - returns Hashtable properties 
	 */
	public Hashtable getProperties()
	{
		return properties;
	}
	
	/**
	 *  Get method - returns Vector propertyNames 
	 */
	public Vector getPropertyNames()
	{
		return propertyNames;
	}	

	/**
	 *  Get method - returns Hashtable propertyLists
	 */
	public Hashtable getPropertyLists()
	{
		return propertyLists;
	}
	
	/**
	 *  Get method - returns Vector propertyListNames 
	 */
	public Vector getPropertyListNames()
	{
		return propertyListNames;
	}

	/**
	 *  Delete method - deletes from propertyLists those in the delete vector
	 */
	public void deletePropertyLists()
	{
		try
		{
			//Cycle thru propertyLists to delete
			String listName;
			Vector removeList = bean.getDeletePropLists();
			for(int i=0;i<removeList.size();i++)
			{
				//get the list
				listName = (String)removeList.elementAt(i);
				
				//remove it from both data structs
				propertyListNames.remove(listName);
				propertyLists.remove(listName);
				
				//remove the file
				File removeFile = new File(bean.getPropertyPath()+listName+".txt");
				removeFile.delete();
			}
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was an error while trying to delete the specified property lists.\n");
		}
	}

    public void writeList(String name) {

        Vector list = (Vector) propertyLists.get(name);
        String path;
        
        
		PrintWriter outFile;
                
        try {
            //Get file and write lines
            path = bean.getAbsoluteFilePath(bean.getPropertyPath() + name + ".txt", true);
            outFile = new PrintWriter(new FileWriter(path));

            //print name
            outFile.println("Amino Acid Property List: " + name);

            //print list
            for (int j = 0; j < list.size(); j++) {
                outFile.println(list.elementAt(j));
            }
            outFile.close();
        } catch (Exception e) {
            bean.errorMessage("\nCould not write property list (" + name + ") to file.");
        }
    }
}//ProteinControl