/**
 *	EvpthwyResultBean.class is an instantiated data object that
 *	contains all of the values of a set of evaluated pathway information
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Evpthwy;

import java.util.Vector;
import java.util.Hashtable;

public class EvpthwyResultBean
{
	//From Evpthwy Calculations
	private String windowID;		//Id of a window of the sequences - uniquely identifies a specific window
	private int totalPathways;				//total number of all possible pathways per property
	private Vector sigZScore;				//Contains the properties and category numbers that are statistically significant in Z Scores
	private Hashtable sigGScore;			//Contains all the properties and whether they are statistically significant in G Scores
	private Hashtable sigGFScore;			//Contains all the properties and whether they are statistically significant in GF Scores
	
	private Hashtable pathways;				//contains all the pathways by category and by property
	private Hashtable zScores;				//Contains Floats of each Z-Score for each category
	private Hashtable gScores;				//Contains Floats of each G-Score for each property
	private Hashtable goodnessOfFit;		//contains the goodness-of-fit for each property

	
	/**
	 *  Set method - sets sigGScore to new Hashtable
	 *	@param Hashtable SigGScore of new sigGScore
	 */
	public void setSigGScore(Hashtable SigGScore)
	{
		sigGScore = SigGScore;
	}
	
	/**
	 *  Get method - returns Hashtable sigGScore 
	 */
	public Hashtable getSigGScore()
	{
		return sigGScore;
	}
	
	/**
	 *  Set method - sets gScores to new Hashtable
	 *	@param Hashtable GScores of new gScores
	 */
	public void setGScores(Hashtable GScores)
	{
		gScores = GScores;
	}
	
	/**
	 *  Get method - returns Hashtable gScores 
	 */
	public Hashtable getGScores()
	{
		return gScores;
	}
	
	/**
	 *  Set method - sets windowID to new String
	 *	@param String SubGCBean of new windowID
	 */
	public void setWindowID(String WindowID)
	{
		windowID = WindowID;
	}
	
	/**
	 *  Get method - returns String windowID 
	 */
	public String getWindowID()
	{
		return windowID;
	}
	
	/**
	 *  Set method - sets goodnessOfFit to new Hashtable
	 *	@param Hashtable GoodnessOfFit of new goodnessOfFit
	 */
	public void setGoodnessOfFit(Hashtable GoodnessOfFit)
	{
		goodnessOfFit = GoodnessOfFit;
	}
	
	/**
	 *  Get method - returns Hashtable goodnessOfFit 
	 */
	public Hashtable getGoodnessOfFit()
	{
		return goodnessOfFit;
	}
	
	/**
	 *  Set method - sets zScores to new Hashtable
	 *	@param Hashtable ZScores of new zScores
	 */
	public void setZScores(Hashtable ZScores)
	{
		zScores = ZScores;
	}
	
	/**
	 *  Get method - returns Hashtable zScores 
	 */
	public Hashtable getZScores()
	{
		return zScores;
	}
	
	/**
	 *  Set method - sets pathways to new Hashtable
	 *	@param Hashtable Pathways of new pathways
	 */
	public void setPathways(Hashtable Pathways)
	{
		pathways = Pathways;
	}
	
	/**
	 *  Get method - returns Hashtable pathways 
	 */
	public Hashtable getPathways()
	{
		return pathways;
	}
	
	/**
	 *  Set method - sets sigGFScore to new Hashtable
	 *	@param Hashtable SigGFScore of new sigGFScore
	 */
	public void setSigGFScore(Hashtable SigGFScore)
	{
		sigGFScore = SigGFScore;
	}
	
	/**
	 *  Get method - returns Hashtable sigGFScore 
	 */
	public Hashtable getSigGFScore()
	{
		return sigGFScore;
	}
	
	/**
	 *  Set method - sets sigZScore to new Vector
	 *	@param Vector SigZScore of new sigZScore
	 */
	public void setSigZScore(Vector SigZScore)
	{
		sigZScore = SigZScore;
	}
	
	/**
	 *  Get method - returns Vector sigZScore 
	 */
	public Vector getSigZScore()
	{
		return sigZScore;
	}
	
	/**
	 *  Set method - sets totalPathways to new int
	 *	@param int TotalPathways of new totalPathways
	 */
	public void setTotalPathways(int TotalPathways)
	{
		totalPathways = TotalPathways;
	}
	
	/**
	 *  Get method - returns int totalPathways 
	 */
	public int getTotalPathways()
	{
		return totalPathways;
	}
	
}//EvpthwyResultBean