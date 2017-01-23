/**
 *	RSTStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of RST algorithms.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package DataConvert.RST;

import java.io.IOException;
import java.util.Vector;
import java.util.StringTokenizer;

class RSTStub extends DataConvert.Components.Stub
{
	//Class Variables - Created Objects
	private RSTUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Empty Constructor, for utilization of this class's methods
	 */
	public RSTStub(){}

	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 */
	public RSTStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new RSTUsageBean();
		
		//Populate Usage Bean
		if(!getData(user_settingsFileName))
			throw new IOException();
		bean.setInputFile(user_settingsFileName);
	}//Constructor
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param errorWindow boolean specifies if utilizing a GUI
	 * @param logFile String to the log file of errors
	 */
	public RSTStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new RSTUsageBean();
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

		outputString += "[ RST User Input Values ]\n\n";
		return outputString;
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{	
		return writeData();
	}//resetData

	/**
	 *  Get method - returns DataUsageBean bean
	 */
	public DataConvert.Components.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//RSTStub
