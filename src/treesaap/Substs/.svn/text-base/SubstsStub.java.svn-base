/**
 *	SubstsStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the substs program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Substs;

import java.io.IOException;
import java.util.StringTokenizer;

public class SubstsStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private SubstsUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public SubstsStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new SubstsUsageBean();
		
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
	public SubstsStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new SubstsUsageBean();
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
		//detailed
		if(thisToken.equalsIgnoreCase("detailed"))
		{
			bean.setDetailed(Boolean.valueOf(st.nextToken()).booleanValue());
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
		
		outputString += "[ Substs User Input Values ]\n\n";
		outputString += "1.	detailed	"+ bean.getDetailed() +"	[ Whether mulitple changes will be recorded ]\n\n";

		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setDetailed(false);
		
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
