/**
 *	BlockStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the Block program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Block;

import java.io.IOException;
import java.util.StringTokenizer;

public class BlockStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private BlockUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public BlockStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new BlockUsageBean();
		
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
	public BlockStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new BlockUsageBean();
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
		return false;
	}//determineToken	

	/**
	 * Returns current data values so they can be written to file
	 */
	public String writeData()
	{		
		//String to be returned
		String outputString = "";
		
	
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		
	
		return writeData();
		
	}//resetData
	
	/**
	 *  Get method - returns BlockUsageBean bean
	 */
	public treesaap.Objects.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//BlockStub
