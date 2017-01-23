/**
 *	BASEMLStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the uses of the baseml program.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.BASEML;

import java.io.IOException;
import java.util.Vector;
import java.util.Hashtable;
import java.util.StringTokenizer;

import treesaap.GeneralDNAFileParser.TaxaBean;
import treesaap.GeneralDNAFileParser.TreeBean;
import treesaap.GeneralDNAFileParser.GeneralDNAFileParser;

public class BASEMLStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private BASEMLUsageBean bean;		//Bean created and populated by this class
	private Hashtable treeData;			//Hashtable of all the trees BASEML can execute
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public BASEMLStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new BASEMLUsageBean();
		
		//Populate Usage Bean
		if(!getData(user_settingsFileName))
			throw new IOException();
		bean.setInputFile(user_settingsFileName);
		
		//Determine OS
		String osName = System.getProperty("os.name");
		if(osName.indexOf("Win") != -1)
			bean.setOS(0);
		else if(osName.indexOf("OS") != -1)
			bean.setOS(1);
		else
			bean.setOS(2);
		
	}//Constructor
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 * @param errorWindow boolean specifies if utilizing a GUI
	 * @param logFile String to the log file of errors
	 */
	public BASEMLStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new BASEMLUsageBean();
		bean.setErrorWindow(errorWindow);
		bean.setLogFile(logFile);
		
		//Populate Usage Bean
		if(!getData(user_settingsFileName))
			throw new IOException();
		bean.setInputFile(user_settingsFileName);
		
		//Determine OS
		String osName = System.getProperty("os.name");
		if(osName.indexOf("Win") != -1)
			bean.setOS(0);
		else if(osName.indexOf("OS") != -1)
			bean.setOS(1);
		else
			bean.setOS(2);
		
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
		//model
		if(thisToken.equalsIgnoreCase("model"))
		{
			bean.setModel(st.nextToken());
			return true;
		}
		//fix_kappa
		else if(thisToken.equalsIgnoreCase("fix_kappa"))
		{
			bean.setFixKappa(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//kappa
		else if(thisToken.equalsIgnoreCase("kappa"))
		{
			bean.setKappa(Double.valueOf(st.nextToken()).doubleValue());
			return true;
		}
		//fix_alpha
		else if(thisToken.equalsIgnoreCase("fix_alpha"))
		{
			bean.setFixAlpha(Boolean.valueOf(st.nextToken()).booleanValue());
			return true;
		}
		//alpha
		else if(thisToken.equalsIgnoreCase("alpha"))
		{
			bean.setAlpha(Double.valueOf(st.nextToken()).doubleValue());
			return true;
		}
		//BASEMLOutfilePath
		else if(thisToken.equalsIgnoreCase("BASEMLOutfilePath"))
		{
			bean.setOutFilePath(thisLine.substring(thisLine.indexOf("\"")+1, thisLine.lastIndexOf("\"")));
			return true;
		}
		//BASEMLPath
		else if(thisToken.equalsIgnoreCase("BASEMLPath"))
		{
			bean.setBasemlPath(thisLine.substring(thisLine.indexOf("\"")+1, thisLine.lastIndexOf("\"")));
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
		
		outputString += "[ BASEML User Input Values ]\n\n";
		outputString += "1.	model	"+ bean.getModel() +"	[ the model that BASEML will implement ]\n";
		outputString += "2.	fix_kappa	"+ bean.getFixKappa() +"	[ whether kappa is fixed ]\n";
		outputString += "3.	kappa 	"+ bean.getKappa() +"	[ value at which kappa is fixed ]\n";
		outputString += "4.	fix_alpha 	"+ bean.getFixAlpha() +"	[ whether alpha is fixed ]\n";
		outputString += "5.	alpha 	"+ bean.getAlpha() +"	[ value at which alpha is fixed ]\n";
		outputString += "6.	BASEMLOutfilePath \""+ bean.getOutFilePath() +"\" [ Specified file path for baseml files ]\n";
		outputString += "7.	BASEMLPath \""+ bean.getBasemlPath() +"\" [ Specified path to the baseml executable ]\n\n";

		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setModel("REV");
		bean.setFixKappa(false);
		bean.setKappa(2.5);
		bean.setFixAlpha(true);
		bean.setAlpha(0.0);
		bean.setOutFilePath("");
		bean.setBasemlPath("./BASEML");
		
		return writeData();
		
	}//resetData

	/**
	 * Converts data from fileParser and places it in a Hashtable of BASMEL objects
	 * @param GeneralDNAFileParser fileParser - the FileParser object containing all the information necessary for baseml
	 */
	public void convert(GeneralDNAFileParser fileParser)
	{
		//Method Vars
		BASEMLTreeBean tree;
		treeData = new Hashtable();
		Hashtable sequenceData = new Hashtable();
		
		//get data from fileParser
		Vector treeNames = fileParser.getTreeNames();
		Vector taxaNames = fileParser.getTaxaNames();
		Hashtable trees = fileParser.getTrees();
		Hashtable taxa = fileParser.getTaxa();
		
		//go through all the taxa and put sequence data in own hashtable
		for(int i=0;i<taxaNames.size();i++)
			sequenceData.put(taxaNames.elementAt(i),((TaxaBean)taxa.get((String)taxaNames.elementAt(i))).getSequence());
		
		//go through all trees, take out need info, and place into treeData
		for(int i=0;i<treeNames.size();i++)
		{
			TreeBean t = ((TreeBean)trees.get((String)treeNames.elementAt(i)));
			if(t.getNodes().size() == 0)
				tree = new BASEMLTreeBean(t.getName(), t.getRevisedRelation(), t.getRooted(), t.getTaxaNames(), sequenceData, false);
			else
				tree = new BASEMLTreeBean(t.getName(), t.getRevisedRelation(), t.getRooted(), t.getTaxaNames(), sequenceData, true);
			treeData.put(treeNames.elementAt(i), tree);
		}
		
	}//convert
	
	/**
	 *  Get method - returns BASEMLUsageBean bean
	 */
	public treesaap.Objects.UsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Get method - returns Hashtable treeData
	 */
	public Hashtable getTreeData()
	{
		return treeData;
	}//getTreeData 	

}//BASEMLStub
