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
package DataConvert.GraphicInterface;

import java.io.IOException;
import java.util.StringTokenizer;

class GraphicInterfaceStub extends DataConvert.Components.Stub
{
	//Class Variables - Created Objects
	private GraphicInterfaceUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public GraphicInterfaceStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new GraphicInterfaceUsageBean();
		
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
	public GraphicInterfaceStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new GraphicInterfaceUsageBean();
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
		//fullScreen 
		if(thisToken.equalsIgnoreCase("fullScreen"))
		{
			bean.setFullScreen(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		
		//height
		else if(thisToken.equalsIgnoreCase("height"))
		{
			bean.setHeight(Integer.valueOf(st.nextToken()).intValue());
			return true;
		}
		
		//length
		else if(thisToken.equalsIgnoreCase("length"))
		{
			bean.setLength(Integer.valueOf(st.nextToken()).intValue());
			return true;
		}
		
		//iconPath
		else if(thisToken.equalsIgnoreCase("iconPath"))
		{
			bean.setIconPath(thisLine.substring(thisLine.indexOf("\"")+1, thisLine.lastIndexOf("\"")));
			return true;
		}
		
		//fileType
		else if(thisToken.equalsIgnoreCase("type"))
		{
			 bean.setFileType(st.nextToken());
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
		
		outputString += "[ GUI User Input Values ]\n\n";
		outputString += "1.	fullScreen	"+ bean.getFullScreen() +"	[ Whether the GUI is full-screen or not ]\n";
		outputString += "2.	height	"+ bean.getHeight() +"	[ Height of the GUI frame ]\n";
		outputString += "3.	length	"+ bean.getLength() +"	[ Length of the GUI frame ]\n";
		outputString += "4.	iconPath	\""+ bean.getIconPath() +"\"	[ The full path to the location of the icons for this program ]\n";
		outputString += "5.	type	"+ bean.getFileType() +"	[ The file type to print out ]\n\n";
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setFullScreen(false);
		bean.setHeight(390);
		bean.setLength(649);
		bean.setIconPath("DataConvert/GraphicInterface/ICONS");
		bean.setFileType("FASTA");
		
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
