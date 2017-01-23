/**
 *	EvpthwyUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the Evpthwy package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Evpthwy;

import java.util.Vector;
import java.util.Hashtable;


import treesaap.Data.GeneticCodeBean;

public class EvpthwyUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private GeneticCodeBean gcBean;			//Contains necessary information about genetic code
	private double[] zScore;				//Contains z-score critical values
	private double[] gfScore;				//Contains gf-score critical values	
	private int numberOfCat;				//Number of categories specified by user
	
	//Print Vars for Category Totals file
	private boolean codonTally;				//Whether to print tallied codons to file
	private boolean observedChart;			//Whether to print observed pathways to file
	private boolean pthwyChart;				//Whether to print possible pathways to file
	private boolean header;					//Whether to print several analysis variables to file 

	//Print Vars for Significant Property files
	private boolean conf05;				//Whether to print values at the .05 confidence level to file
	private boolean conf01;				//Whether to print values at the .01 confidence level to file
	private boolean conf001;			//Whether to print values at the .001 confidence level to file
	private Vector posCategories;		//Which significant categories (specifically) to print to file that are positive
	private Vector negCategories;		//Which significant categories (specifically) to print to file that are negative

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
	 *  Set method - sets negCategories to new Vector
	 *	@param Vector NegCategories of new negCategories
	 */
	public void setNegCategories(Vector NegCategories)
	{
		negCategories = NegCategories;
	}
	
	/**
	 *  Get method - returns Vector negCategories 
	 */
	public Vector getNegCategories()
	{
		return negCategories;
	}
	
	/**
	 *  Set method - sets posCategories to new Vector
	 *	@param Vector PosCategories of new posCategories
	 */
	public void setPosCategories(Vector PosCategories)
	{
		posCategories = PosCategories;
	}
	
	/**
	 *  Get method - returns Vector posCategories 
	 */
	public Vector getPosCategories()
	{
		return posCategories;
	}
	
	/**
	 *  Set method - sets conf001 to new boolean
	 *	@param boolean Conf001 of new conf001
	 */
	public void setConf001(boolean Conf001)
	{
		conf001 = Conf001;
	}
	
	/**
	 *  Get method - returns boolean conf001 
	 */
	public boolean getConf001()
	{
		return conf001;
	}
	
	/**
	 *  Set method - sets conf01 to new boolean
	 *	@param boolean Conf01 of new conf01
	 */
	public void setConf01(boolean Conf01)
	{
		conf01 = Conf01;
	}
	
	/**
	 *  Get method - returns boolean conf01 
	 */
	public boolean getConf01()
	{
		return conf01;
	}
	
	/**
	 *  Set method - sets conf05 to new boolean
	 *	@param boolean Conf05 of new conf05
	 */
	public void setConf05(boolean Conf05)
	{
		conf05 = Conf05;
	}
	
	/**
	 *  Get method - returns boolean conf05 
	 */
	public boolean getConf05()
	{
		return conf05;
	}
	
	/**
	 *  Set method - sets header to new boolean
	 *	@param boolean Header of new header
	 */
	public void setHeader(boolean Header)
	{
		header = Header;
	}

	/**
	 *  Get method - returns boolean header 
	 */
	public boolean getHeader()
	{
		return header;
	}
	
	/**
	 *  Set method - sets pthwyChart to new boolean
	 *	@param boolean PthwyChart of new pthwyChart
	 */
	public void setPthwyChart(boolean PthwyChart)
	{
		pthwyChart = PthwyChart;
	}
	
	/**
	 *  Get method - returns boolean pthwyChart 
	 */
	public boolean getPthwyChart()
	{
		return pthwyChart;
	}
	
	/**
	 *  Set method - sets observedChart to new boolean
	 *	@param boolean ObservedChart of new observedChart
	 */
	public void setObservedChart(boolean ObservedChart)
	{
		observedChart = ObservedChart;
	}
	
	/**
	 *  Get method - returns boolean observedChart 
	 */
	public boolean getObservedChart()
	{
		return observedChart;
	}
	
	/**
	 *  Set method - sets codonTally to new boolean
	 *	@param boolean CodonTally of new codonTally
	 */
	public void setCodonTally(boolean CodonTally)
	{
		codonTally = CodonTally;
	}
	
	/**
	 *  Get method - returns boolean codonTally 
	 */
	public boolean getCodonTally()
	{
		return codonTally;
	}

	/**
	 *  Get method - returns double[] gfScore 
	 */
	public double[] getGFScore()
	{
		return gfScore;
	}
	
	/**
	 *  Set method - sets gfScore to new double[]
	 *	@param double[] GFScore of new gfScore
	 */
	public void setGFScore(double[] GFScore)
	{
		gfScore = GFScore;
	}
	
	/**
	 *  Get method - returns double[] zScore 
	 */
	public double[] getZScore()
	{
		return zScore;
	}
	
	/**
	 *  Set method - sets zScore to new double[]
	 *	@param double[] ZScore of new zScore
	 */
	public void setZScore(double[] ZScore)
	{
		zScore = ZScore;
	}
	
	/**
	 *  Set method - sets numberOfCat to new int
	 *	@param int NumberOfCat of new numberOfCat
	 */
	public void setNumberOfCat(int NumberOfCat)
	{
		//Assign vars
		numberOfCat = NumberOfCat;
		
		//Get greater than categories
		assignGreaterCat(posCategories);
		assignGreaterCat(negCategories);	
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
	 *	Assign Greater than categories at run time
	 *	@param Vector catVec - contains all the specified categories
	 */
	public void assignGreaterCat(Vector catVec)
	{
		//Method vars
		int start;
		Integer cat;
		Object entry;
		String token;
		
		//go through positive category vector
		for(int i=0;i<catVec.size();i++)
		{
			entry = catVec.elementAt(i);
			token = entry.toString();
			
			//Greater than found
			if(token.indexOf(">") != -1)
			{
				token = token.replaceAll(">","");				
				start = Integer.valueOf(token).intValue() + 1;
				
				//add up to numberOfCat
				while(start <= numberOfCat)
				{
					cat = new Integer(start);
					if(!catVec.contains(cat))
						catVec.add(cat);
					start++;
				}
			}
		}
	}
}//EvpthwyUsageBean
