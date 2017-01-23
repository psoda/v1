/**
 *	Evpthwy.class is responsible for the correct extrapolation
 *	of expected changes in the sequences.  These values are stored
 *	in objects of the EvpthwyBean.
 *
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Evpthwy;

import treesaap.Substs.SubstsResultBean;

public class Evpthwy
{
	//CLASS Vars
	private EvpthwyUsageBean bean;			//Bean containing all values necessary for operation
	
	
	/**
	 * Evpthwy Constructor
	 * creates EvpthwyBean object
	 */
	public Evpthwy()
	{
		bean = new EvpthwyUsageBean();
		
	}//Constructor	

	/**
	 * Evpthwy Constructor
	 * @param Bean EvpthwyUsageBean usage bean is set to that passed in
	 */
	public Evpthwy(EvpthwyUsageBean Bean)
	{
		bean = Bean;
		
	}//Constructor 
	
	/**
	 * Runs all Evpthwy calculations - Returns an EvpthwyResultBean
	 * @param SubstsResultBean results - the results determined by Substs
	 */
	public EvpthwyResultBean run(SubstsResultBean subResults)
	{
		try
		{
			//Add to total
			bean.setTotalWork(bean.getTotalWork() + (subResults.getNsynSubs().size() * 10));
			
			//Run calculations
			EvpthwyControl control = new EvpthwyControl(bean, subResults.getSubGCBean().getProperties(), subResults.getSubGCBean().getPropertyNames(), subResults.getFreq());
			control.calculate(subResults.getNsynSubs().size(), subResults.getSubGCBean().getObservedCatTotals());
			control.determineSig();
			control.getSigSubs(subResults.getNsynSubs());
			
			EvpthwyResultBean evResults = new EvpthwyResultBean();
			evResults.setWindowID(subResults.getWindowID());
			evResults.setTotalPathways(control.getTotalPathways());
			evResults.setSigZScore(control.getSigZScore());
			evResults.setSigGScore(control.getSigGScore());
			evResults.setSigGFScore(control.getSigGFScore());
			evResults.setPathways(control.getPathways());
			evResults.setZScores(control.getZScores());
			evResults.setGScores(control.getGScores());
			evResults.setGoodnessOfFit(control.getGoodnessOfFit());
			
			return evResults;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nAn error occured while processing in Evpthwy.\n  Please review Evpthwy requirements.");
			return null;
		}
		
	}//run
	
	/**
	 * Get method - returns EvpthwyUsageBean bean
	 */
	public EvpthwyUsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Set method - sets bean here and lower classes
	 *	@param Bean EvpthwyUsageBean is the new bean 
	 */
	public void setBean(EvpthwyUsageBean Bean)
	{
		bean = Bean;
	}//setBean 

}//Evpthwy