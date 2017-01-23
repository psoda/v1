/**
 *	NEXUSStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of NEXUS algorithms.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package DataConvert.NEXUS;

import java.io.IOException;
import java.util.Vector;
import java.util.StringTokenizer;

class NEXUSStub extends DataConvert.Components.Stub
{
	//Class Variables - Created Objects
	private NEXUSUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Empty Constructor, for utilization of this class's methods
	 */
	public NEXUSStub(){}
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 */
	public NEXUSStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new NEXUSUsageBean();
		
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
	public NEXUSStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new NEXUSUsageBean();
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
		//hard_return
		if(thisToken.equalsIgnoreCase("matchChar"))
		{						
			bean.setMatchChar(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}					
		//chunk
		else if(thisToken.equalsIgnoreCase("interleaveSize"))
		{	
			bean.setInterleaveSize(Integer.valueOf(st.nextToken()).intValue());
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
		
		outputString += "[ CLUSTAL User Input Values ]\n\n";
		
		outputString += "[ WRITER ]\n";
		outputString += "1.	interleaveSize	"+ bean.getInterleaveSize() + "	[ The size of interleave chunks to break data into, 0 puts whole sequence on one line ]\n";
		outputString += "2.	matchChar " + bean.getMatchChar() + "	[The sequence uses a character as a representation of a match character]\n\n";
		
		
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setMatchChar(false);
		bean.setInterleaveSize(60);
		
		return writeData();
	}//resetData
	
	/**
	 *  Get method - returns DataUsageBean bean
	 */
	public DataConvert.Components.UsageBean getBean()
	{
		return bean;
	}//getBean 	
	
}//NEXUSStub
