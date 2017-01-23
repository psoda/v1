/**
 *	Stub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the general dna file parser.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.GeneralDNAFileParser;

import java.io.IOException;
import java.util.Vector;
import java.util.Hashtable;
import java.util.StringTokenizer;

public class DNAFileParserStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private DNAFileParserUsageBean bean;		//Bean created and populated by this class
	

	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public DNAFileParserStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new DNAFileParserUsageBean();
		
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
	public DNAFileParserStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new DNAFileParserUsageBean();
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
		//allCaps
		if(thisToken.equalsIgnoreCase("allCaps"))
		{
			bean.setAllCaps(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		
		//evenHanded
		else if(thisToken.equalsIgnoreCase("evenHanded"))
		{
			bean.setEvenHanded(Double.valueOf(st.nextToken()).doubleValue());
			return true;
		}
		
		//gdfpVerbose
		else if(thisToken.equalsIgnoreCase("gdfpVerbose"))	
		{
			bean.setGdfpVerbose(Integer.valueOf(st.nextToken()).intValue());
			return true;
		}
		
		//progressWindow 
		else if(thisToken.equalsIgnoreCase("DNAFileParserProgressWindow"))	
		{
			bean.setProgressWindow(Boolean.valueOf(st.nextToken()).booleanValue());
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
		
		outputString += "[ GeneralDNAFileParser User Input Values ]\n\n";
		outputString += "1.	allCaps	"+ bean.getAllCaps() +"	[ whether DNA data must be all capitals ]\n";
		outputString += "2.	evenHanded	"+ bean.getEvenHanded() +"	[ determines percentage of ambiguous codons may be over valid codons ]\n";
		outputString += "3.	gdfpVerbose	"+ bean.getGdfpVerbose() +"	[ the depth of debug data ]\n";
		outputString += "4.	DNAFileParserProgressWindow "+ bean.getProgressWindow() +"	[ whether to display progress window for DNAFileParser ]\n\n";

		return outputString;

	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setProgressWindow(true);
		bean.setAllCaps(true);
		bean.setEvenHanded(.5);
		bean.setGdfpVerbose(7);
		
		return writeData();
		
	}//resetData

	/**
	 * Returns a false TreeBean inorder to allow the comparison of two taxa directly
	 *
	 * @param String taxa1 - the first taxa to be compared
	 * @param String taxa2 - the second taxa to be compared
	 */
	public TreeBean createTree(String taxa1, String taxa2)
	{
		//Method Vars
		String relation = "("+ taxa1 +","+ taxa2 +")";
		String name = taxa1 +"."+ taxa2;
		
		//set new tree bean
		TreeBean t = new TreeBean(name, relation, false, new Vector());
		
		//assign revisedRelation
		t.setRevisedRelation(relation);
		
		//assign taxaNames
		t.setTaxaNames(new Vector());
		t.getTaxaNames().add(taxa1);
		t.getTaxaNames().add(taxa2);
		
		//assign nodes
		t.setNodes(new Hashtable());
		t.getNodes().put("false", taxa1);
		t.setNodeNames(new Vector());
		t.getNodeNames().add("false");
		
		//assign ancestral tree
		t.getAncestralTree().add(taxa1);
		t.getAncestralTree().add(taxa2);
		
		return t;
	}
	
	
	
	/**
	 *  Get method - returns DNAFileParserUsageBean bean
	 */
	public treesaap.Objects.UsageBean getBean()
	{
		return bean;
	}//getBean 	

}//DNAFileParserStub
