/**
 *	CriticalValueControl.class is responsible for the correct interpretation
 *	of Z-score and GF-score values in from files.  These values are stored
 *	in the CriticalValueBean.
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Data;

import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.util.Hashtable;

class CriticalValueControl
{
	//Class Vars
	private CriticalValueBean cvBean;
	private DataUsageBean bean;
	
	
	/**
	 *	Data Constructor - Instantiates Class Objects.	
	 */
	public CriticalValueControl(DataUsageBean Bean)
	{
		bean = Bean;
		cvBean = new CriticalValueBean();
	}//constructor
	
	/**
	 *	Calls methods to read in data from files Z_CriticalValues and GF_CriticalValues.
	 */
	public boolean obtainData()
	{
		return (readinZScore() && readinGFScore());
	}
	
	/**
	 *	Reads in data from files Z_CriticalValues.
	 *	Places this information into the cvBean.
	 */
	private boolean readinZScore()
	{
		//Method Vars
		double[] zCriticalValues = new double[3];
		
		try
		{
			//Start Z-score File
			BufferedReader inFile = new BufferedReader(new FileReader(bean.getCriticalValuePath()+"Z_CriticalValues.txt"));
			String line = inFile.readLine();	
	
			//Get to line with Z-Score Numbers
			while(line.indexOf("Z-Score") == -1)
				line = inFile.readLine();
			
			//Skip heading line
			line = inFile.readLine();
			
			//Obtain Values
			line = inFile.readLine();
			zCriticalValues[0] = Double.valueOf(line.substring(0,line.indexOf("	"))).doubleValue();
			zCriticalValues[1] = Double.valueOf(line.substring(line.indexOf("	")+1, line.lastIndexOf("	"))).doubleValue();
			zCriticalValues[2] = Double.valueOf(line.substring(line.lastIndexOf("	")+1)).doubleValue();
			
			//place in cvBean
			cvBean.setZCriticalValues(zCriticalValues);
			
			//close file
			inFile.close();
		}
		catch(FileNotFoundException exception)	
		{
			bean.errorMessage("\nZ_CriticalValues is missing!\n    Please place it in specified path ("+ bean.getCriticalValuePath() +").");
			return false;
		}
		catch(Exception exception) 
		{
			bean.errorMessage("\nYou had an Exception while obtaining data from "+ bean.getCriticalValuePath() +"Z_CriticalValues.\n  Please verify the format of the file.");
			return false;
		}
		
		return true;
	}
	
	/**
	 *	Reads in data from files GF_CriticalValues.
	 *	Places this information into the cvBean.
	 */
	private boolean readinGFScore()
	{	
		//Method Vars
		String catNum;
		double[] gfCV;
		Hashtable gfCriticalValues = new Hashtable();
		
		try
		{
			//Start GF-score File
			BufferedReader inFile = new BufferedReader(new FileReader(bean.getCriticalValuePath()+"GF_CriticalValues.txt"));
			String line = inFile.readLine();	
			
			//Get to line with GF-Score Numbers
			while(line.indexOf("GF-Score") == -1)
				line = inFile.readLine();
			
			//Skip heading line
			line = inFile.readLine();
			
			//Obtain Values for each different Category number
			while((line = inFile.readLine()) != null)
			{
				//get Category number
				catNum = line.substring(0, line.indexOf("	"));
				
				//Obtain Values
				line = line.substring(line.indexOf("	")+1);
				gfCV = new double[3];
				gfCV[0] = Double.valueOf(line.substring(0,line.indexOf("	"))).doubleValue();
				gfCV[1] = Double.valueOf(line.substring(line.indexOf("	")+1, line.lastIndexOf("	"))).doubleValue();
				gfCV[2] = Double.valueOf(line.substring(line.lastIndexOf("	")+1)).doubleValue();
				
				//put values in data struct, get next line
				gfCriticalValues.put(catNum, gfCV);
			}
			
			//set values in bean
			cvBean.setGFCriticalValues(gfCriticalValues);
			
			//close file
			inFile.close();
		}
		catch(FileNotFoundException exception)	
		{
			bean.errorMessage("\nGF_CriticalValues is missing!\n    Please place it in specified path ("+ bean.getCriticalValuePath() +").");
			return false;
		}
		catch(Exception exception) 
		{
			bean.errorMessage("\nYou had an Exception while obtaining data from "+ bean.getCriticalValuePath() +"GF_CriticalValues.\n  Please verify the format of the file.");
			return false;
		}
		
		return true;
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
	 *  Get method - returns Hashtable gfCriticalValues 
	 */
	public Hashtable getGFCriticalValues()
	{
		return cvBean.getGFCriticalValues();
	}
	
	/**
	 *  Get method - returns double[] zCriticalValues 
	 */
	public double[] getZCriticalValues()
	{
		return cvBean.getZCriticalValues();
	}
	
}//CriticalValueControl