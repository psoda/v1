/**
 *	CDMStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the CDM program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.CDM;

import java.io.IOException;
import java.util.StringTokenizer;

public class CDMStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private CDMUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public CDMStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new CDMUsageBean();
		
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
	public CDMStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new CDMUsageBean();
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
		//autoTransBias
		if(thisToken.equalsIgnoreCase("autoTransBias"))
		{
			bean.setAutoTransBias(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//transitionBias
		else if(thisToken.equalsIgnoreCase("transitionBias"))
		{
			bean.setTransitionBias(Double.valueOf(st.nextToken()).doubleValue());
			return true;						
		}
		//deriveTS4
		else if(thisToken.equalsIgnoreCase("deriveTS4"))
		{
			bean.setDeriveTS4(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}	
		//fixedTransBias
		if(thisToken.equalsIgnoreCase("fixedTransBias"))
		{
			bean.setFixedTransBias(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}				
		//gScore
		else if(thisToken.equalsIgnoreCase("gScore"))
		{
			bean.setGScore(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//gfScore
		else if(thisToken.equalsIgnoreCase("gfScore"))
		{
			bean.setGFScore(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//tsTv
		else if(thisToken.equalsIgnoreCase("tsTv"))
		{
			bean.setTsTv(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//transbias
		else if(thisToken.equalsIgnoreCase("transbias"))
		{
			bean.setTransbias(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;						
		}
		//observed
		else if(thisToken.equalsIgnoreCase("observed"))
		{
			bean.setObserved(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;						
		}
		//observedFrequency
		else if(thisToken.equalsIgnoreCase("observedFrequency"))
		{
			bean.setObservedFrequency(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;						
		}
		//expected
		else if(thisToken.equalsIgnoreCase("expected"))
		{
			bean.setExpected(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;						
		}
		//expectedFrequency
		else if(thisToken.equalsIgnoreCase("expectedFrequency"))
		{
			bean.setExpectedFrequency(Boolean.valueOf(st.nextToken()).booleanValue());	
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
		
		outputString += "[ CDM User Input Values ]\n\n";			
		outputString += "[ Calculations ]\n";
		outputString += "1.	autoTransBias	"+ bean.getAutoTransBias() +"	[ Whether CDM determines the transition bias ]\n";
		outputString += "2.	transitionBias	"+ bean.getTransitionBias() +"	[ The user-specified transition bias, autoTransBias must be false ]\n";
		outputString += "3.	deriveTS4	"+ bean.getDeriveTS4() +"	[ How CDM determines ts4 - either it derives it or it uses a set of equations ]\n";
		outputString += "4.	fixedTransBias	"+ bean.getFixedTransBias() +"	[ Whether to fix the transbias of CDM at one value, or to recalculate it for each window ]\n\n";
		outputString += "[ Sliding-Window Output File ]\n";
		outputString += "5.	gScore	"+ bean.getGScore() +"	[ Whether to output G-Scores to file ]\n";
		outputString += "6.	gfScore	"+ bean.getGFScore() +"	[ Whether to output GF-Scores to file ]\n";
		outputString += "7.	tsTv	"+ bean.getTsTv() +"	[ Whether to output ts:tv ratios to file ]\n";
		outputString += "8.	transbias	"+ bean.getTransbias() +"	[ Whether to output the transition bias to file ]\n";
		outputString += "9.	observed	"+ bean.getObserved() +"	[ Whether to output observed substitutions to file ]\n";
		outputString += "10.	observedFrequency	"+ bean.getObservedFrequency() +"	[ Whether to output observed frequencies to file ]\n";
		outputString += "11.	expected	"+ bean.getExpected() +"	[ Whether to output expected substitutions to file ]\n";
		outputString += "12.	expectedFrequency	"+ bean.getExpectedFrequency() +"	[ Whether to output expected frequencies to file ]\n\n";
		
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setAutoTransBias(true);
		bean.setTransitionBias(0.0);
		bean.setDeriveTS4(false);
		bean.setFixedTransBias(false);
		bean.setGScore(true);
		bean.setGFScore(true);
		bean.setTsTv(true);
		bean.setTransbias(true);
		bean.setObserved(true);
		bean.setObservedFrequency(true);
		bean.setExpected(true);
		bean.setExpectedFrequency(true);
	
		return writeData();
		
	}//resetData
	
	/**
	 *  Get method - returns CDMUsageBean bean
	 */
	public treesaap.Objects.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//BASEMLStub
