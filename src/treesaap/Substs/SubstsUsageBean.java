/**
 *	SubstsUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the Substs package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Substs;

import treesaap.Data.GeneticCodeBean;
import treesaap.GeneralDNAFileParser.TreeBean;

import java.util.Hashtable;

public class SubstsUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private boolean detailed;			//whether two and three step changes to be recorded as well
	private TreeBean tree;				//Tree to be used in substs calculation
	private GeneticCodeBean gcBean;		//GeneticCodeBean provides aAcid, cClass, and getCat
	private Hashtable taxa;				//All the taxa that GDFP contains
	private int numberOfCat;			//Number of categories specified by user
	
	//Tracking vars
	private long workDone;
	private long totalWork;


	/**
	 *  Add method - adds given int to workDone
	 *	@param int WorkDone of addition
	 */
	public void addToWorkDone(int WorkDone)
	{
		workDone += WorkDone;
	}
	
	/**
	 *  Set method - sets workDone to new long
	 *	@param long WorkDone of new workDone
	 */
	public void setWorkDone(long WorkDone)
	{
		workDone = WorkDone;
	}
	
	/**
	 *  Get method - returns long workDone 
	 */
	public long getWorkDone()
	{
		return workDone;
	}
	
	/**
	 *  Set method - sets totalWork to new long
	 *	@param long TotalWork of new totalWork
	 */
	public void setTotalWork(long TotalWork)
	{
		totalWork = TotalWork;
	}
	
	/**
	 *  Get method - returns long totalWork 
	 */
	public long getTotalWork()
	{
		return totalWork;
	}
	
	/**
	 *  Set method - sets numberOfCat to new int
	 *	@param int NumberOfCat of new numberOfCat
	 */
	public void setNumberOfCat(int NumberOfCat)
	{
		numberOfCat = NumberOfCat;
	}
	
	/**
	 *  Get method - returns int numberOfCat 
	 */
	public int getNumberOfCat()
	{
		return numberOfCat;
	}
	
	/**
	 *  Get method - returns GeneticCodeBean gcBean 
	 */
	public GeneticCodeBean getGCBean()
	{
		return gcBean;
	}
	
	/**
	 *  Set method - sets gcBean to new GeneticCodeBean
	 *	@param GeneticCodeBean GCBean of new gcBean
	 */
	public void setGCBean(GeneticCodeBean GCBean)
	{
		gcBean = GCBean;
	}

	/**
	 *  Get method - returns boolean detailed 
	 */
	public boolean getDetailed()
	{
		return detailed;
	}
	
	/**
	 *  Set method - sets detailed to new boolean
	 *	@param boolean Detailed of new detailed
	 */
	public void setDetailed(boolean Detailed)
	{
		detailed = Detailed;
	}
	
	/**
	 *  Get method - returns Hashtable taxa 
	 */
	public Hashtable getTaxa()
	{
		return taxa;
	}
	
	/**
	 *  Set method - sets taxa to new Hashtable
	 *	@param Hashtable Taxa of new taxa
	 */
	public void setTaxa(Hashtable Taxa)
	{
		taxa = Taxa;
	}
	
	/**
	 *  Get method - returns TreeBean tree 
	 */
	public TreeBean getTree()
	{
		return tree;
	}
	
	/**
	 *  Set method - sets tree to new TreeBean
	 *	@param TreeBean Tree of new tree
	 */
	public void setTree(TreeBean Tree)
	{
		tree = Tree;
	}

	/**
	 *  Get method - returns int[] cClass 
	 */
	public int[] getCClass()
	{
		return gcBean.getCClass();
	}
	
	/**
	 *  Set method - sets cClass to new int[]
	 *	@param int[] CClass of new cClass
	 */
	public void setCClass(int[] CClass)
	{
		gcBean.setCClass(CClass);
	}
	
	/**
	 *  Get method - returns int[] aAcid 
	 */
	public int[] getAAcid()
	{
		return gcBean.getAAcid();
	}
	
	/**
	 *  Set method - sets aAcid to new int[]
	 *	@param int[] AAcid of new aAcid
	 */
	public void setAAcid(int[] AAcid)
	{
		gcBean.setAAcid(AAcid);
	}

}//SubstsUsageBean
