/**
 *	ResultsBean.class is an instantiated data object that
 *	contains all of the values of a set of substitution information
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Substs;

import java.util.Vector;
import java.util.Hashtable;

public class SubstsResultBean
{
	//From Substs Calculations
	private String windowID;		//Id of a window of the sequences - uniquely identifies a specific window
	private Vector synSubs;			//contains all the synonymous substitutions
	private Vector nsynSubs;		//contains all the nonsynonymous substitutions
	private Vector ambig;			//contains all the ambiguous changes
	private Vector stop;			//contains all the stop codons used
	private int[] freq;						//contains the added up number of codons in a tree
	private int[] cClassFreq;				//contains the added up number of codon classes in a tree
	private int[][] synonymous;			//Number of synonymous substitutions
	private int[][] nonSynonymous;		//Number of nonsynonymous substitutions
	
	//From Genetic Code calculations
	private SubstsGeneticCodeBean subGCBean;	//Bean containing all genetic code values necessary for substs operation
	
	
	/**
	 *  Set method - sets nonSynonymous to new int[][]
	 *	@param int[][] NonSynonymous of new nonSynonymous
	 */
	public void setNonSynonymous(int[][] NonSynonymous)
	{
		nonSynonymous = NonSynonymous;
	}
	
	/**
	 *  Get method - returns int[][] nonSynonymous 
	 */
	public int[][] getNonSynonymous()
	{
		return nonSynonymous;
	}
	
	/**
	 *  Set method - sets synonymous to new int[][]
	 *	@param int[][] Synonymous of new synonymous
	 */
	public void setSynonymous(int[][] Synonymous)
	{
		synonymous = Synonymous;
	}
	
	/**
	 *  Get method - returns int[][] synonymous 
	 */
	public int[][] getSynonymous()
	{
		return synonymous;
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
	 *  Set method - sets subGCBean to new SubstsGeneticCodeBean
	 *	@param SubstsGeneticCodeBean SubGCBean of new subGCBean
	 */
	public void setSubGCBean(SubstsGeneticCodeBean SubGCBean)
	{
		subGCBean = SubGCBean;
	}
	
	/**
	 *  Get method - returns SubstsGeneticCodeBean subGCBean 
	 */
	public SubstsGeneticCodeBean getSubGCBean()
	{
		return subGCBean;
	}
	
	/**
	 *  Set method - sets cClassFreq to new int[]
	 *	@param int[] CClassFreq of new cClassFreq
	 */
	public void setCClassFreq(int[] CClassFreq)
	{
		cClassFreq = CClassFreq;
	}
	
	/**
	 *  Get method - returns int[] cClassFreq 
	 */
	public int[] getCClassFreq()
	{
		return cClassFreq;
	}
	
	/**
	 *  Set method - sets freq to new int[]
	 *	@param int[] Freq of new freq
	 */
	public void setFreq(int[] Freq)
	{
		freq = Freq;
	}
	
	/**
	 *  Get method - returns int[] freq 
	 */
	public int[] getFreq()
	{
		return freq;
	}

	/**
	 *  Set method - sets stop to new Vector
	 *	@param Vector Stop of new stop
	 */
	public void setStop(Vector Stop)
	{
		stop = Stop;
	}
	
	/**
	 *  Get method - returns Vector stop 
	 */
	public Vector getStop()
	{
		return stop;
	}
	
	/**
	 *  Set method - sets ambig to new Vector
	 *	@param Vector Ambig of new ambig
	 */
	public void setAmbig(Vector Ambig)
	{
		ambig = Ambig;
	}
	
	/**
	 *  Get method - returns Vector ambig 
	 */
	public Vector getAmbig()
	{
		return ambig;
	}
	
	/**
	 *  Set method - sets nsynSubs to new Vector
	 *	@param Vector NsynSubs of new nsynSubs
	 */
	public void setNsynSubs(Vector NsynSubs)
	{
		nsynSubs = NsynSubs;
	}
	
	/**
	 *  Get method - returns Vector nsynSubs 
	 */
	public Vector getNsynSubs()
	{
		return nsynSubs;
	}
	
	/**
	 *  Set method - sets synSubs to new Vector
	 *	@param Vector SynSubs of new synSubs
	 */
	public void setSynSubs(Vector SynSubs)
	{
		synSubs = SynSubs;
	}
	
	/**
	 *  Get method - returns Vector synSubs 
	 */
	public Vector getSynSubs()
	{
		return synSubs;
	}
	
}//SubstsResultBean