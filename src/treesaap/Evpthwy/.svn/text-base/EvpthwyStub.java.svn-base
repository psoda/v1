/**
 *	EvpthwyStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the evpthwy program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Evpthwy;

import java.io.IOException;
import java.util.Vector;
import java.util.StringTokenizer;

public class EvpthwyStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private EvpthwyUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Empty Constructor, for utilization of this class's methods
	 */
	public EvpthwyStub(){}

	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 */
	public EvpthwyStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new EvpthwyUsageBean();
		
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
	public EvpthwyStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new EvpthwyUsageBean();
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
		//numberOfCategories
		if(thisToken.equalsIgnoreCase("numberOfCategories"))
		{						
			bean.setNumberOfCat(Integer.valueOf(st.nextToken()).intValue());
			return true;
		}					
		//codonTally
		else if(thisToken.equalsIgnoreCase("codonTally"))
		{
			bean.setCodonTally(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//observedChart
		else if(thisToken.equalsIgnoreCase("observedChart"))
		{
			bean.setObservedChart(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//pthwyChart
		else if(thisToken.equalsIgnoreCase("pthwyChart"))
		{
			bean.setPthwyChart(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//zScoreTally
		else if(thisToken.equalsIgnoreCase("header"))
		{
			bean.setHeader(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//.05Conf
		else if(thisToken.equalsIgnoreCase(".05Conf"))
		{
			bean.setConf05(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//.01Conf
		else if(thisToken.equalsIgnoreCase(".01Conf"))
		{
			bean.setConf01(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//.001Conf
		else if(thisToken.equalsIgnoreCase(".001Conf"))
		{
			bean.setConf001(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//category
		else if(thisToken.equalsIgnoreCase("category"))
		{
			//get chars
			Vector pos = getCats(thisLine, "-");
			Vector neg = getCats(thisLine, "+");
			
			//set vectors
			bean.setPosCategories(pos);
			bean.setNegCategories(neg);
			
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

		outputString += "[ Evpthwy User Input Values ]\n\n";
		outputString += "[ Category-Totals File ]\n";
		outputString += "1.	codonTally	"+ bean.getCodonTally() +"	[ Whether to print tallied codons to file ]\n";
		outputString += "2.	observedChart	"+ bean.getObservedChart() +"	[ Whether to print observed pathways to file ]\n";
		outputString += "3.	pthwyChart	"+ bean.getPthwyChart() +"	[ Whether to print possible pathways to file ]\n";
		outputString += "4.	header	"+ bean.getHeader() +"	[ Whether to print gf-score analysis to file ]\n\n";
		outputString += "[ Significant Property files ]\n";
		outputString += "5.	.05Conf	"+ bean.getConf05() +"	[ Whether to print values at the .05 confidence level to file ]\n";
		outputString += "6.	.01Conf	"+ bean.getConf01() +"	[ Whether to print values at the .01 confidence level to file ]\n";
		outputString += "7.	.001Conf	"+ bean.getConf001() +"	[ Whether to print values at the .001 confidence level to file ]\n";
		
		//Check for category entries
		Vector pos = bean.getPosCategories();
		Vector neg = bean.getNegCategories();
		if(pos.size() == 0 && neg.size() == 0)
			outputString += "8.	category	all	[ Which categories (specifically) to file ]\n\n";
		else
		{
			outputString += "8.	category	";			
			
			//Positive Categories
			for(int i=0;i<pos.size();i++)
					outputString += "+"+ pos.elementAt(i) +",";
			
			//Negative Categories
			for(int i=0;i<neg.size();i++)
					outputString += "-"+ neg.elementAt(i) +",";
			
			//remove zeros
			if(outputString.indexOf("+0")!= -1)
				outputString = outputString.substring(0,outputString.indexOf("+0")) + outputString.substring(outputString.indexOf("+0")+3);
			if(outputString.indexOf("-0")!= -1)
				outputString = outputString.substring(0,outputString.indexOf("-0")) + outputString.substring(outputString.indexOf("-0")+3);
			
			//adjust string
			outputString = outputString.substring(0,outputString.length()-1);
			outputString += "	[ Which categories (specifically) to file ]\n\n";
		
		}
		outputString += "[ Calculations ]\n";
		outputString += "9.	numberOfCategories	"+ bean.getNumberOfCat() +"	[ The number of categories ]\n\n";		
		
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setNumberOfCat(8);
		bean.setCodonTally(true);
		bean.setObservedChart(true);
		bean.setPthwyChart(true);
		bean.setHeader(true);
		bean.setConf05(true);
		bean.setConf01(true);
		bean.setConf001(true);
		
		bean.setPosCategories(new Vector());
		bean.setNegCategories(new Vector());

		return writeData();
	}//resetData

	/**
	 *	Determines the positive categories specified by user to be used in printing files
	 *
	 *	@param String line - line containing chars
	 *	@param String determinate - the deciding factor + or -
	 */
	public Vector getCats(String line, String determinate) throws Exception
	{
		//Method vars
		int start;
		String token;
		Integer cat;	
		Vector charVec = new Vector();		
		
		//Scan to work "categories" & Tokenize
		line = line.substring(line.indexOf("category")+8);
		StringTokenizer st = new StringTokenizer(line, " 	,");
		
		//add in base case
		charVec.add("0");
		
		//parse tokens
		while(st.hasMoreTokens())
		{
			token = st.nextToken();
			
			//if error with category, just ignore
			try
			{
				//check for all validation
				if(token.equals("all"))
				{
					charVec = new Vector();
					return charVec;
				}				
				
				//Check for comments
				if(token.indexOf("[") != -1)
					return charVec;
				
				//check for a negative
				else if(token.indexOf(determinate) == -1)
				{
					//remove all +
					if(token.indexOf("+") != -1)
						token = token.substring(token.indexOf("+")+1);
					
					//remove all -
					if(token.indexOf("-") != -1)
						token = token.substring(token.indexOf("-")+1);					
					
					//no signs
					if(token.indexOf("<") == -1 && token.indexOf(">") == -1)
					{						
						cat = new Integer(token);
						if(cat.intValue() > 0)
							if(!charVec.contains(cat))
								charVec.add(cat);
					}
					
					//only > sign
					else if(token.indexOf("<") == -1 && token.indexOf(">") != -1)
					{
						token = token.replaceAll(">","");
						if(!charVec.contains(">"+ token))
							charVec.add(">"+ token);
					}
					
					//only < sign
					else if(token.indexOf("<") != -1 && token.indexOf(">") == -1)
					{
						token = token.replaceAll("<","");
						start = Integer.valueOf(token).intValue() - 1;
						
						//add down till 1
						while(start > 0)
						{
							cat = new Integer(start);
							if(!charVec.contains(cat))
								charVec.add(cat);
							start--;
						}
					}
				}
			}
			catch(Exception exception) 
			{
				bean.logMessage("Invalid category ("+ token +") specified in Evpthwy Settings file.");
				return new Vector();
			}
		}
		return charVec;
	}

	/**
	 *  Get method - returns DataUsageBean bean
	 */
	public treesaap.Objects.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//BASEMLStub
