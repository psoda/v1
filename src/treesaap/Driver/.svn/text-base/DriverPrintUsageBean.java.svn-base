/**
 *	DriverPrintUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the Driver
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Driver;

import java.io.File;
import java.util.Vector;

public class DriverPrintUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private boolean treeHeader;			//Whether to print the tree header to file 
	private boolean treeTaxa;			//Whether to print the tree's taxa to file 
	private boolean ancestralTree;		//Whether to print the tree's ancestral data to file 
	private boolean treeNodes;			//Whether to print the tree's nodes to file 
	
	//Which Files to Print
	private boolean writeTree;			//Whether to print the tree file with the options above 
	private boolean writeTCSTree;		//Whether to print the tcs tree to file
	private boolean writeGeneticCodes;	//Whether to print the geneticCode file 
	private boolean writeProperties;	//Whether to print the Properties file 
	private boolean writeZScores;		//Whether to print the ZScores file 
	private boolean writeGFScores;		//Whether to print the GFScores file 
	private boolean writePathways;		//Whether to print the Pathways file 
	
	private boolean writeSynonymous;	//Whether to print the Synonymous Substitution file 
	private boolean writeNonsynonymous;	//Whether to print the Nonsynonymous Substitution file 
	private boolean writeStop;			//Whether to print the Stop Codons file 
	private boolean writeAmbiguous;		//Whether to print the Ambiguous Codons file 
	
	private boolean categoryTotalsFolder;	//Whether to print all the individual category totals to a folder 
	private boolean categoryTotalsFile;		//Whether to print the category totals file 
	private boolean significantByZScore;	//Whether to print significantByZScore file
	private boolean significantByGScore;	//Whether to print significantByGScore file 
	private boolean significantByGFScore;	//Whether to print significantByGFScore file 
	private boolean significantBySubs;		//Whether to print significantBySubstitution file 
	private boolean significantByCodon;		//Whether to print significantByCodon file 
	private boolean slidingCatTotals;		//Whether to print sliding Category Totals file 
	
	private boolean cdmResults;				//Whether to print the CDM results file 
	private boolean slidingWindowTotal;		//Whether to print the sliding window totals file 
	private boolean slidingWindowFolder;	//Whether to print all the individual cdm totals to a folder 
	
	
	/**
	 *  Get method - returns boolean significantByGScore 
	 */
	public boolean getSignificantByGScore()
	{
		return significantByGScore;
	}
	
	/**
	 *  Set method - sets significantByGScore to new boolean
	 *	@param boolean SignificantByGScore of new significantByGScore
	 */
	public void setSignificantByGScore(boolean SignificantByGScore)
	{
		significantByGScore = SignificantByGScore;
	}
	
	/**
	 *  Get method - returns boolean writeTCSTree 
	 */
	public boolean getWriteTCSTree()
	{
		return writeTCSTree;
	}
	
	/**
	 *  Set method - sets writeTCSTree to new boolean
	 *	@param boolean WriteTCSTree of new writeTCSTree
	 */
	public void setWriteTCSTree(boolean WriteTCSTree)
	{
		writeTCSTree = WriteTCSTree;
	}
	
	/**
	 *  Get method - returns boolean slidingCatTotals 
	 */
	public boolean getSlidingCatTotals()
	{
		return slidingCatTotals;
	}
	
	/**
	 *  Set method - sets slidingCatTotals to new boolean
	 *	@param boolean SlidingCatTotals of new slidingCatTotals
	 */
	public void setSlidingCatTotals(boolean SlidingCatTotals)
	{
		slidingCatTotals = SlidingCatTotals;
	}
	
	/**
	 *  Get method - returns boolean significantByCodon 
	 */
	public boolean getSignificantByCodon()
	{
		return significantByCodon;
	}
	
	/**
	 *  Set method - sets significantByCodon to new boolean
	 *	@param boolean SignificantByCodon of new significantByCodon
	 */
	public void setSignificantByCodon(boolean SignificantByCodon)
	{
		significantByCodon = SignificantByCodon;
	}
	
	/**
	 *  Get method - returns boolean significantBySubs 
	 */
	public boolean getSignificantBySubs()
	{
		return significantBySubs;
	}
	
	/**
	 *  Set method - sets significantBySubs to new boolean
	 *	@param boolean SignificantBySubs of new significantBySubs
	 */
	public void setSignificantBySubs(boolean SignificantBySubs)
	{
		significantBySubs = SignificantBySubs;
	}
	
	/**
	 *  Get method - returns boolean significantByGFScore 
	 */
	public boolean getSignificantByGFScore()
	{
		return significantByGFScore;
	}
	
	/**
	 *  Set method - sets significantByGFScore to new boolean
	 *	@param boolean SignificantByGFScore of new significantByGFScore
	 */
	public void setSignificantByGFScore(boolean SignificantByGFScore)
	{
		significantByGFScore = SignificantByGFScore;
	}
	
	/**
	 *  Get method - returns boolean significantByZScore 
	 */
	public boolean getSignificantByZScore()
	{
		return significantByZScore;
	}
	
	/**
	 *  Set method - sets significantByZScore to new boolean
	 *	@param boolean SignificantByZScore of new significantByZScore
	 */
	public void setSignificantByZScore(boolean SignificantByZScore)
	{
		significantByZScore = SignificantByZScore;
	}
	
	/**
	 *  Get method - returns boolean categoryTotalsFile 
	 */
	public boolean getCategoryTotalsFile()
	{
		return categoryTotalsFile;
	}
	
	/**
	 *  Set method - sets categoryTotalsFile to new boolean
	 *	@param boolean CategoryTotalsFile of new categoryTotalsFile
	 */
	public void setCategoryTotalsFile(boolean CategoryTotalsFile)
	{
		categoryTotalsFile = CategoryTotalsFile;
	}
	
	/**
	 *  Get method - returns boolean categoryTotalsFolder 
	 */
	public boolean getCategoryTotalsFolder()
	{
		return categoryTotalsFolder;
	}
	
	/**
	 *  Set method - sets categoryTotalsFolder to new boolean
	 *	@param boolean CategoryTotalsFolder of new categoryTotalsFolder
	 */
	public void setCategoryTotalsFolder(boolean CategoryTotalsFolder)
	{
		categoryTotalsFolder = CategoryTotalsFolder;
	}
	
	/**
	 *  Get method - returns boolean slidingWindowFolder 
	 */
	public boolean getSlidingWindowFolder()
	{
		return slidingWindowFolder;
	}
	
	/**
	 *  Set method - sets slidingWindowFolder to new boolean
	 *	@param boolean SlidingWindowFolder of new slidingWindowFolder
	 */
	public void setSlidingWindowFolder(boolean SlidingWindowFolder)
	{
		slidingWindowFolder = SlidingWindowFolder;
	}
	
	/**
	 *  Get method - returns boolean slidingWindowTotal 
	 */
	public boolean getSlidingWindowTotal()
	{
		return slidingWindowTotal;
	}
	
	/**
	 *  Set method - sets slidingWindowTotal to new boolean
	 *	@param boolean SlidingWindowTotal of new slidingWindowTotal
	 */
	public void setSlidingWindowTotal(boolean SlidingWindowTotal)
	{
		slidingWindowTotal = SlidingWindowTotal;
	}
	
	/**
	 *  Get method - returns boolean cdmResults 
	 */
	public boolean getCDMResults()
	{
		return cdmResults;
	}
	
	/**
	 *  Set method - sets cdmResults to new boolean
	 *	@param boolean CDMResults of new cdmResults
	 */
	public void setCDMResults(boolean CDMResults)
	{
		cdmResults = CDMResults;
	}
	
	/**
	 *  Get method - returns boolean writeAmbiguous 
	 */
	public boolean getWriteAmbiguous()
	{
		return writeAmbiguous;
	}
	
	/**
	 *  Set method - sets writeAmbiguous to new boolean
	 *	@param boolean WriteAmbiguous of new writeAmbiguous
	 */
	public void setWriteAmbiguous(boolean WriteAmbiguous)
	{
		writeAmbiguous = WriteAmbiguous;
	}
	
	/**
	 *  Get method - returns boolean writeStop 
	 */
	public boolean getWriteStop()
	{
		return writeStop;
	}
	
	/**
	 *  Set method - sets writeStop to new boolean
	 *	@param boolean WriteStop of new writeStop
	 */
	public void setWriteStop(boolean WriteStop)
	{
		writeStop = WriteStop;
	}
	
	/**
	 *  Get method - returns boolean writeNonsynonymous 
	 */
	public boolean getWriteNonsynonymous()
	{
		return writeNonsynonymous;
	}
	
	/**
	 *  Set method - sets writeNonsynonymous to new boolean
	 *	@param boolean WriteNonsynonymous of new writeNonsynonymous
	 */
	public void setWriteNonsynonymous(boolean WriteNonsynonymous)
	{
		writeNonsynonymous = WriteNonsynonymous;
	}
	
	/**
	 *  Get method - returns boolean writeSynonymous 
	 */
	public boolean getWriteSynonymous()
	{
		return writeSynonymous;
	}
	
	/**
	 *  Set method - sets writeSynonymous to new boolean
	 *	@param boolean WriteSynonymous of new writeSynonymous
	 */
	public void setWriteSynonymous(boolean WriteSynonymous)
	{
		writeSynonymous = WriteSynonymous;
	}
	
	/**
	 *  Get method - returns boolean writePathways 
	 */
	public boolean getWritePathways()
	{
		return writePathways;
	}
	
	/**
	 *  Set method - sets writePathways to new boolean
	 *	@param boolean WritePathways of new writePathways
	 */
	public void setWritePathways(boolean WritePathways)
	{
		writePathways = WritePathways;
	}
	
	/**
	 *  Get method - returns boolean writeGFScores 
	 */
	public boolean getWriteGFScores()
	{
		return writeGFScores;
	}
	
	/**
	 *  Set method - sets writeGFScores to new boolean
	 *	@param boolean WriteGFScores of new writeGFScores
	 */
	public void setWriteGFScores(boolean WriteGFScores)
	{
		writeGFScores = WriteGFScores;
	}
	
	/**
	 *  Get method - returns boolean writeZScores 
	 */
	public boolean getWriteZScores()
	{
		return writeZScores;
	}
	
	/**
	 *  Set method - sets writeZScores to new boolean
	 *	@param boolean WriteZScores of new writeZScores
	 */
	public void setWriteZScores(boolean WriteZScores)
	{
		writeZScores = WriteZScores;
	}	
	
	/**
	 *  Get method - returns boolean writeProperties 
	 */
	public boolean getWriteProperties()
	{
		return writeProperties;
	}
	
	/**
	 *  Set method - sets writeProperties to new boolean
	 *	@param boolean WriteGeneticCodes of new writeProperties
	 */
	public void setWriteProperties(boolean WriteProperties)
	{
		writeProperties = WriteProperties;
	}	
	
	/**
	 *  Get method - returns boolean writeGeneticCodes 
	 */
	public boolean getWriteGeneticCodes()
	{
		return writeGeneticCodes;
	}
	
	/**
	 *  Set method - sets writeGeneticCodes to new boolean
	 *	@param boolean WriteGeneticCodes of new writeGeneticCodes
	 */
	public void setWriteGeneticCodes(boolean WriteGeneticCodes)
	{
		writeGeneticCodes = WriteGeneticCodes;
	}	
	
	/**
	 *  Get method - returns boolean writeTree 
	 */
	public boolean getWriteTree()
	{
		return writeTree;
	}
	
	/**
	 *  Set method - sets writeTree to new boolean
	 *	@param boolean WriteTree of new writeTree
	 */
	public void setWriteTree(boolean WriteTree)
	{
		writeTree = WriteTree;
	}		
	
	/**
	 *  Get method - returns boolean treeNodes 
	 */
	public boolean getTreeNodes()
	{
		return treeNodes;
	}
	
	/**
	 *  Set method - sets treeNodes to new boolean
	 *	@param boolean TreeNodes of new treeNodes
	 */
	public void setTreeNodes(boolean TreeNodes)
	{
		treeNodes = TreeNodes;
	}	
	
	/**
	 *  Get method - returns boolean ancestralTree 
	 */
	public boolean getAncestralTree()
	{
		return ancestralTree;
	}
	
	/**
	 *  Set method - sets ancestralTree to new boolean
	 *	@param boolean AncestralTree of new ancestralTree
	 */
	public void setAncestralTree(boolean AncestralTree)
	{
		ancestralTree = AncestralTree;
	}
	
	/**
	 *  Get method - returns boolean treeTaxa 
	 */
	public boolean getTreeTaxa()
	{
		return treeTaxa;
	}
	
	/**
	 *  Set method - sets treeTaxa to new boolean
	 *	@param boolean TreeTaxa of new treeTaxa
	 */
	public void setTreeTaxa(boolean TreeTaxa)
	{
		treeTaxa = TreeTaxa;
	}
	
	/**
	 *  Get method - returns boolean treeHeader 
	 */
	public boolean getTreeHeader()
	{
		return treeHeader;
	}
	
	/**
	 *  Set method - sets treeHeader to new boolean
	 *	@param boolean TreeHeader of new treeHeader
	 */
	public void setTreeHeader(boolean TreeHeader)
	{
		treeHeader = TreeHeader;
	}

}//DriverPrintUsageBean
