/**
 *	Stub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of a specified package
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Objects;

import java.io.FileReader;
import java.io.BufferedReader;
import java.util.StringTokenizer;

public class Stub
{	
	/**
	 * Reads data values from file and places them in usage bean
	 * Implemented by developer
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public boolean getData(String user_settingsFileName)
	{
		//Method Vars
		StringTokenizer st;
		String thisLine, thisToken;
		
		try
		{
			//Open up a Buffered Reader of user_settings file
			BufferedReader inFile = new BufferedReader(new FileReader(user_settingsFileName));
			thisLine = inFile.readLine();
			
			//Read through the file, determine settings
			while(thisLine != null)
			{
				//skip to actual value
				st = new StringTokenizer(thisLine);
				
				//parse tokens on line
				while(st.hasMoreTokens())
				{					
					//get next token
					thisToken = st.nextToken();
					
					//determine Token
					if(determineToken(thisToken, thisLine, st))
						break;
				}
				
				//next line
				thisLine = inFile.readLine();
			}
			
			//Close File
			inFile.close();
		}
		catch(Exception exception) 
		{
			(new UsageBean()).errorMessage("\nYou had an Exception while obtaining data from "+ user_settingsFileName + ". exception: " + exception.getStackTrace()[6]);
			return false;
		}
		
		return true;
	}//getData
	
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
	 * Implemented by developer
	 */
	public String writeData()
	{		
		//String to be returned
		String outputString = "";

		
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 * Implemented by developer
	 */
	public String resetData()
	{		
		return writeData();
		
	}//resetData

	/**
	 *  Get method - returns UsageBean bean
	 */
	public UsageBean getBean()
	{
		return new UsageBean();
	}//getBean 	

}//Stub
