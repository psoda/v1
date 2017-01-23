/**
 *	BASEMLStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the data program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Data;

import java.io.IOException;
import java.util.StringTokenizer;

public class DataStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private DataUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public DataStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new DataUsageBean();
		
		//Populate Usage Bean
		if(!getData(user_settingsFileName))
			throw new IOException();
		bean.setInputFile(user_settingsFileName);
		
	}//Constructor
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 * @param errorWindow boolean specifies if utilizing a GUI
	 * @param logFile String to the log file of errors
	 */
	public DataStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new DataUsageBean();
		bean.setErrorWindow(errorWindow);
		bean.setLogFile(logFile);

		//Populate Usage Bean
		if(!getData(user_settingsFileName))
			throw new IOException();
		bean.setInputFile(user_settingsFileName);
		
	}//Constructor
	
	/**
	 * Reads data values from file and places them in usage bean
	 * Implemented by developer
	 *
	 * @param token String the token of the string
	 * @param thisLine String the line the tokens came from
	 * @param st StringTokenizer the tokens following the one given
	 */
	public boolean determineToken(String thisToken, String thisLine, StringTokenizer st) throws Exception
	{		
		//geneticCodePath
		if(thisToken.equalsIgnoreCase("geneticCodePath"))
		{
			bean.setGeneticCodePath(thisLine.substring(thisLine.indexOf("\"")+1, thisLine.lastIndexOf("\"")));
			return true;
		}
		//propertyPath
		else if(thisToken.equalsIgnoreCase("propertyPath"))
		{
			bean.setPropertyPath(thisLine.substring(thisLine.indexOf("\"")+1, thisLine.lastIndexOf("\"")));
			return true;
		}
		//criticalValuePath
		else if(thisToken.equalsIgnoreCase("criticalValuePath"))
		{
			bean.setCriticalValuePath(thisLine.substring(thisLine.indexOf("\"")+1, thisLine.lastIndexOf("\"")));
			return true;
		}
		return false;
			
	}//determineToken
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String writeData()
	{		
		//String to be returned
		String outputString = "";
		
		outputString += "[ Data User Input Values ]\n\n";
		outputString += "1.	geneticCodePath	\""+ bean.getGeneticCodePath() +"\"	[ Specified path to the genetic code files ]\n";
		outputString += "2.	propertyPath	\""+ bean.getPropertyPath() +"\"	[ Specified path to the amino acid property files ]\n";
		outputString += "3.	criticalValuePath \""+ bean.getCriticalValuePath() +"\"	[ Specified path to the critical values files ]\n\n";

		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setGeneticCodePath("./treesaap/Data/GENETIC_CODES");
		bean.setPropertyPath("./treesaap/Data/PROTEIN_PROPERTIES");
		bean.setCriticalValuePath("./treesaap/Data/CRITICAL_VALUES");
		
		return writeData();
		
	}//resetData
	
	/**
	 *  Get method - returns DataUsageBean bean
	 */
	public treesaap.Objects.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//BASEMLStub
