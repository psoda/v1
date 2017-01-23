/**
 *	DriverUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the Driver
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Driver;

import java.io.File;

public class DriverUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private String geneticCode;			//The Genetic Code used by Driver
	private String resultsDirectory;	//The Results Directory
	private String outputDir; 			// the Results directory concatenated with the name of the tree (the output directory of this instance)
	private boolean move;				//Whether Move the Baseml Temp Files
	private boolean moveRst;			//Whether Move the RST file
	private boolean createTreeFolder;	//Whether to create a tree folder at the output directory
	private String branchComparison;	//The branches to be compared directly utilizing algorithm
	
	//Process Vars
	private boolean commandLine;		//Whether the program started with commandLine
	private String properties;			//The User-Specified Properties to be used with Evpthwy

	
	/**
	 *  Get method - returns String branchComparison 
	 */
	public String getBranchComparison()
	{
		return branchComparison;
	}
	
	/**
	 *  Set method - sets branchComparison to new String
	 *	@param String BranchComparison of new BranchComparison
	 */
	public void setBranchComparison(String BranchComparison)
	{
		branchComparison = BranchComparison;
	}

	/**
	 *  Get method - returns boolean createTreeFolder 
	 */
	public boolean getCreateTreeFolder()
	{
		return createTreeFolder;
	}
	
	/**
	 *  Set method - sets createTreeFolder to new boolean
	 *	@param boolean CreateTreeFolder of new createTreeFolder
	 */
	public void setCreateTreeFolder(boolean CreateTreeFolder)
	{
		createTreeFolder = CreateTreeFolder;
	}
	
	/**
	 *  Get method - returns String properties 
	 */
	public String getProperties()
	{
		return properties;
	}
	
	/**
	 *  Set method - sets properties to new String
	 *	@param String newProperties of new properties
	 */
	public void setProperties(String newProperties)
	{
		properties = newProperties;
	}
	
	/**
	 *  Get method - returns boolean commandLine 
	 */
	public boolean getCommandLine()
	{
		return commandLine;
	}
	
	/**
	 *  Set method - sets commandLine to new boolean
	 *	@param boolean CommandLine of new commandLine
	 */
	public void setCommandLine(boolean CommandLine)
	{
		commandLine = CommandLine;
	}
	
	/**
	 *  Set method - sets resultsDirectory to new String
	 *	@param String ResultsDirectory of new resultsDirectory
	 */
	public void setResultsDirectory(String ResultsDirectory)
	{
		resultsDirectory = getAbsoluteDirPath(ResultsDirectory);
		if(resultsDirectory == null)
			resultsDirectory = "";
		else
			resultsDirectory += File.separator;
	}
	
	/**
	 *  Get method - returns String resultsDirectory 
	 */
	public String getResultsDirectory()
	{
		return resultsDirectory;
	}
	
	/**
	 *  Get method - returns String geneticCode 
	 */
	public String getGeneticCode()
	{
		return geneticCode;
	}
	
	/**
	 *  Set method - sets geneticCode to new String
	 *	@param String GeneticCode of new geneticCode
	 */
	public void setGeneticCode(String GeneticCode)
	{
		geneticCode = GeneticCode;
	}
	
	/**
	 *  Get method - returns boolean move 
	 */
	public boolean getMove()
	{
		return move;
	}
	
	/**
	 *  Set method - sets move to new boolean
	 *	@param boolean Move of new move
	 */
	public void setMove(boolean Move)
	{
		move = Move;
	}
	
	/**
	 *  Get method - returns boolean moveRst 
	 */
	public boolean getMoveRst()
	{
		return moveRst;
	}
	
	/**
	 *  Set method - sets moveRst to new boolean
	 *	@param boolean MoveRst of new moveRst
	 */
	public void setMoveRst(boolean MoveRst)
	{
		moveRst = MoveRst;
	}	
	
	/**
	 *  Get method - returns boolean errorWindow 
	 */
	public boolean getErrorWindow()
	{
		return errorWindow;
	}

	
	/**
	 *  Get method - returns String logFile 
	 */
	public String getLogFile()
	{
		return logFile;
	}

	public void setOutputDirectory(String string) {
		//System.out.println("setting output dir: " + string);
                outputDir = getAbsoluteDirPath(string);
		if(outputDir == null)
			outputDir = "";
		else
			outputDir += File.separator;
	}

	public String getOutputDirectory() {
		return outputDir;
	}
	
}//DriverUsageBean
