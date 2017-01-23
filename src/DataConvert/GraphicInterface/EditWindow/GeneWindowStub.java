/**
 *	GraphicInterfaceStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the GraphicInterface program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface.EditWindow;

import java.io.IOException;
import java.util.StringTokenizer;

class GeneWindowStub extends DataConvert.Components.Stub
{
	//Class Variables - Created Objects
	private GeneWindowUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public GeneWindowStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new GeneWindowUsageBean();
		
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
	public GeneWindowStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new GeneWindowUsageBean();
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
		//position
		if(thisToken.equalsIgnoreCase("position"))
		{
			bean.setPosition(thisLine.substring(thisLine.indexOf("position")+8, thisLine.indexOf("[")));
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
		
		outputString += "[ Edit_Window Input Values ]\n\n";
		outputString += "1.	position	"+ bean.getPosition().getX() +" "+ bean.getPosition().getY() +" "+ bean.getPosition().getWidth() +" "+ bean.getPosition().getHeight() +"	[ The location the display is set to ]\n\n";
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		
		System.out.println("reseting gene widow");
		
		//reset values
		bean.setPosition(0, 400, 417, 437);
		return writeData();
		
	}//resetData
	
	/**
	 *  Get method - returns GraphicInterfaceBean bean
	 */
	public DataConvert.Components.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//GraphicInterfaceStub
