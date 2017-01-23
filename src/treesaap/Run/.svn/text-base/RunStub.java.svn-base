/**
 *	RunStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the substs/evpthwy progress window program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Run;

import java.io.IOException;
import java.util.StringTokenizer;

public class RunStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private RunUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public RunStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new RunUsageBean();
		
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
	public RunStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new RunUsageBean();
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
		//progressWindow
		if(thisToken.equalsIgnoreCase("progressWindow"))
		{
			bean.setProgressWindow(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//slidingWindowSize
		else if(thisToken.equals("slidingWindowSize"))	
		{
			thisToken = st.nextToken();
			if(thisToken.equalsIgnoreCase("length"))
				bean.setSlidingWindowSize(-1);
			else
				bean.setSlidingWindowSize(Integer.valueOf(thisToken).intValue());
			return true;
		}
		//increment
		else if(thisToken.equalsIgnoreCase("increment"))	
		{
			bean.setIncrement(Integer.valueOf(st.nextToken()).intValue());
			return true;
		}
		//processors
		else if(thisToken.equalsIgnoreCase("processors"))	
		{
			bean.setProcessors(Integer.valueOf(st.nextToken()).intValue());
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
		
		outputString += "[ Run User Input Values ]\n\n";
		outputString += "1.	progressWindow	"+ bean.getProgressWindow() +"	[ Whether progress window is displayed ]\n";
		if(bean.getSlidingWindowSize() != -1)
			outputString += "2.	slidingWindowSize	"+ bean.getSlidingWindowSize() +"	[ The length of the sliding window ]\n";
		else
			outputString += "2.	slidingWindowSize	length	[ The length of the sliding window ]\n";
		outputString += "3.	increment	"+ bean.getIncrement() +"	[ The amount by which to increment the sliding window ]\n";
		outputString += "4.	processors	"+ bean.getProcessors() +"	[ The number of chunks to divide groups of threads into ]\n\n";

		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setProgressWindow(true);
		bean.setSlidingWindowSize(-1);
		bean.setIncrement(1);
		bean.setProcessors(1);
		
		return writeData();
		
	}//resetData
	
	/**
	 *  Get method - returns RunUsageBean bean
	 */
	public treesaap.Objects.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//RunStub
