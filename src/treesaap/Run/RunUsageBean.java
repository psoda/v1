/**
 *	RunUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the Substs/Evpthwy Progress Window package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Run;

import treesaap.Evpthwy.Evpthwy;
import treesaap.Substs.Substs;
import treesaap.CDM.CDM;

public class RunUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private boolean progressWindow;			//whether to show window
	private int slidingWindowSize;			//Size of sliding window
	private int increment;					//the amount to increment sliding window
	private int processors;					//the number of processors to split threads into
	
	//Processing Vars
	private Substs substs;
	private Evpthwy evpthwy;
	private CDM cdm;
	
	/**
	 *  Get method - returns CDM cdm 
	 */
	public CDM getCDM()
	{
		return cdm;
	}
	
	/**
	 *  Set method - sets cdm to new CDM
	 *	@param CDM Cdm of new cdm
	 */
	public void setCDM(CDM Cdm)
	{
		cdm = Cdm;
	}
	
	/**
	 *  Get method - returns Substs substs 
	 */
	public Substs getSubsts()
	{
		return substs;
	}
	
	/**
	 *  Set method - sets substs to new Substs
	 *	@param Substs SUBSTS of new substs
	 */
	public void setSubsts(Substs SUBSTS)
	{
		substs = SUBSTS;
	}
	
	/**
	 *  Get method - returns Evpthwy evpthwy 
	 */
	public Evpthwy getEvpthwy()
	{
		return evpthwy;
	}
	
	/**
	 *  Set method - sets evpthwy to new Evpthwy
	 *	@param Evpthwy EVPTHWY of new evpthwy
	 */
	public void setEvpthwy(Evpthwy EVPTHWY)
	{
		evpthwy = EVPTHWY;
	}
	
	/**
	 *  Get method - returns int processors 
	 */
	public int getProcessors()
	{
		return processors;
	}
	
	/**
	 *  Set method - sets processors to new int
	 *	@param int Processors of new processors
	 */
	public void setProcessors(int Processors)
	{
		processors = Processors;
	}
	
	/**
	 *  Get method - returns int increment 
	 */
	public int getIncrement()
	{
		return increment;
	}
	
	/**
	 *  Set method - sets increment to new int
	 *	@param int Increment of new increment
	 */
	public void setIncrement(int Increment)
	{
		increment = Increment;
	}
	
	/**
	 *  Get method - returns int slidingWindowSize 
	 */
	public int getSlidingWindowSize()
	{
		return slidingWindowSize;
	}
	
	/**
	 *  Set method - sets slidingWindowSize to new int
	 *	@param int SlidingWindowSize of new slidingWindowSize
	 */
	public void setSlidingWindowSize(int SlidingWindowSize)
	{
		slidingWindowSize = SlidingWindowSize;
	}

	/**
	 *  Get method - returns boolean progressWindow 
	 */
	public boolean getProgressWindow()
	{
		return progressWindow;
	}
	
	/**
	 *  Set method - sets progressWindow to new boolean
	 *	@param boolean ProgressWindow of new progressWindow
	 */
	public void setProgressWindow(boolean ProgressWindow)
	{
		progressWindow = ProgressWindow;
	}

}//RunUsageBean
