/**
 *	DriverStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the Driver.
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Driver;

import java.io.IOException;
import java.util.StringTokenizer;

public class DriverStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private DriverUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public DriverStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new DriverUsageBean();
		
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
	public DriverStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new DriverUsageBean();
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
		//geneticCode
		if(thisToken.equalsIgnoreCase("geneticCode"))
		{
			bean.setGeneticCode(st.nextToken());	
			return true;					
		}
		//moveTempFiles
		else if(thisToken.equalsIgnoreCase("moveTempFiles"))
		{
			bean.setMove(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;						
		}
		//moveRSTFile
		else if(thisToken.equalsIgnoreCase("moveRSTFile"))
		{
			bean.setMoveRst(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//createTreeFolder
		else if(thisToken.equalsIgnoreCase("createTreeFolder"))
		{
			bean.setCreateTreeFolder(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//resultsDirectory
		else if(thisToken.equalsIgnoreCase("resultsDirectory"))
		{
			bean.setResultsDirectory(thisLine.substring(thisLine.indexOf("\"")+1, thisLine.lastIndexOf("\"")));
			return true;
		}
		//logFile
		else if(thisToken.equalsIgnoreCase("logFile"))
		{
			//Don't set logFile unless one hasn't been set yet
			if(bean.getLogFile() == null || bean.getLogFile().equals(""))
			{
				bean.setLogFile(thisLine.substring(thisLine.indexOf("\"")+1, thisLine.lastIndexOf("\"")));
			}
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

		outputString += "[ Driver User Input Values ]\n\n";
		outputString += "1.	geneticCode	"+ bean.getGeneticCode() +"	[ The Genetic Code used by Driver ]\n";
		outputString += "2.	moveTempFiles	"+ bean.getMove() +"	[ Move the Baseml Temp Files ]\n";
		outputString += "3.	moveRSTFile	"+ bean.getMoveRst() +"	[ Move the RST file ]\n";
		outputString += "4.	resultsDirectory	\""+ bean.getResultsDirectory() +"\"	[ The Results Directory ]\n";
		outputString += "5.	logFile	\""+ bean.getLogFile() +"\"	[ The location of the log file ]\n";
		outputString += "6.	createTreeFolder	"+ bean.getCreateTreeFolder() +"	[ Whether to create a tree folder or not ]\n\n";
		
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setGeneticCode("Universal");
		bean.setMove(true);
		bean.setMoveRst(true);
		bean.setResultsDirectory("./Output/");
		bean.setLogFile("");
		bean.setCreateTreeFolder(true);
	
		return writeData();
		
	}//resetData
	
	/**
	 *  Get method - returns DriverUsageBean bean
	 */
	public treesaap.Objects.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//DriverStub
