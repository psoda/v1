/**
 *	DriverPrintStub.class is an instantiated object that reads in 
 *	data and places them in a UsageBean that
 * 	contains all of the operational decisions a user can 
 *	make regarding the files printed for the Driver.
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Driver;

import java.io.IOException;
import java.util.StringTokenizer;

public class DriverPrintStub extends treesaap.Objects.Stub
{
	//Class Variables - Created Objects
	private DriverPrintUsageBean bean;		//Bean created and populated by this class
	
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param user_settingsFileName String to file containing user_settings
	 */
	public DriverPrintStub(String user_settingsFileName) throws IOException
	{
		//create a new usage bean
		bean = new DriverPrintUsageBean();
		
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
	public DriverPrintStub(String user_settingsFileName, boolean errorWindow, String logFile) throws IOException
	{
		//create a new usage bean
		bean = new DriverPrintUsageBean();
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
		//treeHeader
		if(thisToken.equalsIgnoreCase("treeHeader"))
		{
			bean.setTreeHeader(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//treeTaxa
		else if(thisToken.equalsIgnoreCase("treeTaxa"))
		{
			bean.setTreeTaxa(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//ancestralTree
		else if(thisToken.equalsIgnoreCase("ancestralTree"))
		{
			bean.setAncestralTree(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//treeNodes
		else if(thisToken.equalsIgnoreCase("treeNodes"))
		{
			bean.setTreeNodes(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeTree
		else if(thisToken.equalsIgnoreCase("writeTree"))
		{
			bean.setWriteTree(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeTCSTree
		else if(thisToken.equalsIgnoreCase("writeTCSTree"))
		{
			bean.setWriteTCSTree(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeGeneticCodes
		else if(thisToken.equalsIgnoreCase("writeGeneticCodes"))
		{
			bean.setWriteGeneticCodes(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeProperties
		else if(thisToken.equalsIgnoreCase("writeProperties"))
		{
			bean.setWriteProperties(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeZScores
		else if(thisToken.equalsIgnoreCase("writeZScores"))
		{
			bean.setWriteZScores(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeGFScores
		else if(thisToken.equalsIgnoreCase("writeGFScores"))
		{
			bean.setWriteGFScores(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writePathways
		else if(thisToken.equalsIgnoreCase("writePathways"))
		{
			bean.setWritePathways(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeSynonymous
		else if(thisToken.equalsIgnoreCase("writeSynonymous"))
		{
			bean.setWriteSynonymous(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeNonsynonymous
		else if(thisToken.equalsIgnoreCase("writeNonsynonymous"))
		{
			bean.setWriteNonsynonymous(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeStop
		else if(thisToken.equalsIgnoreCase("writeStop"))
		{
			bean.setWriteStop(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//writeAmbiguous
		else if(thisToken.equalsIgnoreCase("writeAmbiguous"))
		{
			bean.setWriteAmbiguous(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//cdmResults
		else if(thisToken.equalsIgnoreCase("cdmResults"))
		{
			bean.setCDMResults(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//slidingWindowTotal
		else if(thisToken.equalsIgnoreCase("slidingWindowTotal"))
		{
			bean.setSlidingWindowTotal(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//slidingWindowFolder
		else if(thisToken.equalsIgnoreCase("slidingWindowFolder"))
		{
			bean.setSlidingWindowFolder(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//categoryTotalsFolder
		else if(thisToken.equalsIgnoreCase("categoryTotalsFolder"))
		{
			bean.setCategoryTotalsFolder(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//categoryTotalsFile
		else if(thisToken.equalsIgnoreCase("categoryTotalsFile"))
		{
			bean.setCategoryTotalsFile(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//significantByZScore
		else if(thisToken.equalsIgnoreCase("significantByZScore"))
		{
			bean.setSignificantByZScore(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//significantByGFScore
		else if(thisToken.equalsIgnoreCase("significantByGScore"))
		{
			bean.setSignificantByGScore(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//significantByGFScore
		else if(thisToken.equalsIgnoreCase("significantByGFScore"))
		{
			bean.setSignificantByGFScore(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//significantBySubs
		else if(thisToken.equalsIgnoreCase("significantBySubs"))
		{
			bean.setSignificantBySubs(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//significantByCodon
		else if(thisToken.equalsIgnoreCase("significantByCodon"))
		{
			bean.setSignificantByCodon(Boolean.valueOf(st.nextToken()).booleanValue());	
			return true;					
		}
		//slidingCatTotals
		else if(thisToken.equalsIgnoreCase("slidingCatTotals"))
		{
			bean.setSlidingCatTotals(Boolean.valueOf(st.nextToken()).booleanValue());	
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
	
		outputString += "[ PrintOut User Input Values ]\n\n";
		outputString += "[ Parser Print Options ]\n";
		outputString += "1.	treeHeader	"+ bean.getTreeHeader() +"	[ Whether to print the tree header to file ]\n";
		outputString += "2.	treeTaxa	"+ bean.getTreeTaxa() +"	[ Whether to print the tree's taxa to file ]\n";
		outputString += "3.	ancestralTree	"+ bean.getAncestralTree() +"	[ Whether to print the tree's ancestral data to file ]\n";
		outputString += "4.	treeNodes	"+ bean.getTreeNodes() +"	[ Whether to print the tree's nodes to file ]\n\n";	
		outputString += "[ Parser Files to print ]\n";
		outputString += "5.	writeTree	"+ bean.getWriteTree() +"	[ Whether to print the tree file with the options above ]\n";	
		outputString += "6.	writeTCSTree	"+ bean.getWriteTCSTree() +"	[ Whether to print the tcs tree file ]\n\n";	
		outputString += "[ Data Files to print ]\n";
		outputString += "7.	writeGeneticCodes	"+ bean.getWriteGeneticCodes() +"	[ Whether to print the geneticCode file ]\n";
		outputString += "8.	writeProperties	"+ bean.getWriteProperties() +"	[ Whether to print the Properties file ]\n";
		outputString += "9.	writeZScores	"+ bean.getWriteZScores() +"	[ Whether to print the ZScores file ]\n";
		outputString += "10.	writeGFScores	"+ bean.getWriteGFScores() +"	[ Whether to print the GFScores file ]\n";
		outputString += "11.	writePathways	"+ bean.getWritePathways() +"	[ Whether to print the Pathways file ]\n\n";	
		outputString += "[ Substs Files to print ]\n";
		outputString += "12.	writeSynonymous	"+ bean.getWriteSynonymous() +"	[ Whether to print the Synonymous Substitution file ]\n";
		outputString += "13.	writeNonsynonymous	"+ bean.getWriteNonsynonymous() +"	[ Whether to print the Nonsynonymous Substitution file ]\n";
		outputString += "14.	writeStop	"+ bean.getWriteStop() +"	[ Whether to print the Stop Codons file ]\n";
		outputString += "15.	writeAmbiguous	"+ bean.getWriteAmbiguous() +"	[ Whether to print the Ambiguous Codons file ]\n\n";		
		outputString += "[ Evpthwy Files to print ]\n";
		outputString += "16.	categoryTotalsFolder	"+ bean.getCategoryTotalsFolder() +"	[ Whether to print all the individual category totals to a folder ]\n";
		outputString += "17.	categoryTotalsFile	"+ bean.getCategoryTotalsFile() +"	[ Whether to print the category totals file ]\n";
		outputString += "18.	significantByZScore	"+ bean.getSignificantByZScore() +"	[ Whether to print significantByZScore file ]\n";
		outputString += "19.	significantByGScore	"+ bean.getSignificantByGScore() +"	[ Whether to print significantByGScore file ]\n";
		outputString += "20.	significantByGFScore	"+ bean.getSignificantByGFScore() +"	[ Whether to print significantByGFScore file ]\n";
		outputString += "21.	significantBySubs	"+ bean.getSignificantBySubs() +"	[ Whether to print significantBySubstitution file ]\n";
		outputString += "22.	significantByCodon	"+ bean.getSignificantByCodon() +"	[ Whether to print significantByCodon file ]\n";
		outputString += "23.	slidingCatTotals	"+ bean.getSlidingCatTotals() +"	[ Whether to print sliding Category Totals file ]\n	\n";	
		outputString += "[ CDM Files to print ]\n";
		outputString += "24.	cdmResults	"+ bean.getCDMResults() +"	[ Whether to print the CDM results file ]\n";
		outputString += "25.	slidingWindowTotal	"+ bean.getSlidingWindowTotal() +"	[ Whether to print the sliding window totals file ]\n";
		outputString += "26.	slidingWindowFolder	"+ bean.getSlidingWindowFolder() +"	[ Whether to print all the individual cdm totals to a folder ]\n\n";
		
		return outputString;
		
	}//writeData
	
	/**
	 * Returns current data values so they can be written to file
	 */
	public String resetData()
	{		
		//reset values
		bean.setTreeHeader(true);
		bean.setTreeTaxa(true);
		bean.setAncestralTree(true);
		bean.setTreeNodes(true);

		bean.setWriteTree(true);
		bean.setWriteTCSTree(true);
		bean.setWriteGeneticCodes(false);
		bean.setWriteProperties(false);
		bean.setWriteZScores(false);
		bean.setWriteGFScores(false);
		bean.setWritePathways(false);
		
		bean.setWriteSynonymous(true);
		bean.setWriteNonsynonymous(true);
		bean.setWriteStop(true);
		bean.setWriteAmbiguous(true);
		
		bean.setCategoryTotalsFolder(false);
		bean.setCategoryTotalsFile(true);
		bean.setSignificantByZScore(true);
		bean.setSignificantByGFScore(true);
		bean.setSignificantByGScore(true);
		bean.setSignificantBySubs(true);
		bean.setSignificantByCodon(true);
		bean.setSlidingCatTotals(true);
		
		bean.setCDMResults(true);
		bean.setSlidingWindowTotal(true);
		bean.setSlidingWindowFolder(false);
		
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
