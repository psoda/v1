/**
 *	CDMUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the CDM package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.CDM;

public class CDMUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private boolean autoTransBias;		//Whether CDM determines the transition bias
	private double transitionBias;		//The user-specified transition bias
	private boolean fixedTransBias;		//Whether to fix the transbias of CDM at one value, or to recalculate it for each window
	
	//Operational Vars
	private boolean deriveTS4;			//How CDM determines ts4 - either it derives it or it uses a set of equations
	private double ts4;					//The actual value of the transition bias, per run
	
	//Print Variables
	private boolean gScore;				//Whether to output G-Scores to file
	private boolean gfScore;			//Whether to output GF-Scores to file
	private boolean tsTv;				//Whether to output ts:tv ratios to file
	private boolean transbias;			//Whether to output the transition bias to file
	private boolean observed;			//Whether to output observed substitutions to file
	private boolean observedFrequency;	//Whether to output observed frequencies to file
	private boolean expected;			//Whether to output expected substitutions to file
	private boolean expectedFrequency;	//Whether to output expected frequencies to file

	//Tracking vars
	private long workDone;
	private long totalWork;

	
	/**
	 *  Get method - returns boolean fixedTransBias 
	 */
	public boolean getFixedTransBias()
	{
		return fixedTransBias;
	}
	
	/**
	 *  Set method - sets fixedTransBias to new boolean
	 *	@param boolean FixedTransBias of new fixedTransBias
	 */
	public void setFixedTransBias(boolean FixedTransBias)
	{
		fixedTransBias = FixedTransBias;
	}
	
	/**
	 *  Get method - returns boolean deriveTS4 
	 */
	public boolean getDeriveTS4()
	{
		return deriveTS4;
	}
	
	/**
	 *  Set method - sets deriveTS4 to new boolean
	 *	@param boolean DeriveTS4 of new deriveTS4
	 */
	public void setDeriveTS4(boolean DeriveTS4)
	{
		deriveTS4 = DeriveTS4;
	}
	
	/**
	 *  Set method - sets ts4 to new double
	 *	@param double TS4 of new ts4
	 */
	public void setTS4(double TS4)
	{
		ts4 = TS4;
	}
	
	/**
	 *  Get method - returns double ts4 
	 */
	public double getTS4()
	{
		return ts4;
	}
	
	/**
	 *  Get method - returns boolean expectedFrequency 
	 */
	public boolean getExpectedFrequency()
	{
		return expectedFrequency;
	}
	
	/**
	 *  Set method - sets expectedFrequency to new boolean
	 *	@param boolean ExpectedFrequency of new expectedFrequency
	 */
	public void setExpectedFrequency(boolean ExpectedFrequency)
	{
		expectedFrequency = ExpectedFrequency;
	}
	
	/**
	 *  Get method - returns boolean expected 
	 */
	public boolean getExpected()
	{
		return expected;
	}
	
	/**
	 *  Set method - sets expected to new boolean
	 *	@param boolean Expected of new expected
	 */
	public void setExpected(boolean Expected)
	{
		expected = Expected;
	}
	
	/**
	 *  Get method - returns boolean observedFrequency 
	 */
	public boolean getObservedFrequency()
	{
		return observedFrequency;
	}
	
	/**
	 *  Set method - sets observedFrequency to new boolean
	 *	@param boolean ObservedFrequency of new observedFrequency
	 */
	public void setObservedFrequency(boolean ObservedFrequency)
	{
		observedFrequency = ObservedFrequency;
	}
	
	/**
	 *  Get method - returns boolean observed 
	 */
	public boolean getObserved()
	{
		return observed;
	}
	
	/**
	 *  Set method - sets observed to new boolean
	 *	@param boolean Observed of new observed
	 */
	public void setObserved(boolean Observed)
	{
		observed = Observed;
	}
	
	/**
	 *  Get method - returns boolean transbias 
	 */
	public boolean getTransbias()
	{
		return transbias;
	}
	
	/**
	 *  Set method - sets transbias to new boolean
	 *	@param boolean Transbias of new transbias
	 */
	public void setTransbias(boolean Transbias)
	{
		transbias = Transbias;
	}
	
	/**
	 *  Get method - returns boolean tsTv 
	 */
	public boolean getTsTv()
	{
		return tsTv;
	}
	
	/**
	 *  Set method - sets tsTv to new boolean
	 *	@param boolean TsTv of new tsTv
	 */
	public void setTsTv(boolean TsTv)
	{
		tsTv = TsTv;
	}
	
	/**
	 *  Get method - returns boolean gfScore 
	 */
	public boolean getGFScore()
	{
		return gfScore;
	}
	
	/**
	 *  Set method - sets gfScore to new boolean
	 *	@param boolean GFScore of new gfScore
	 */
	public void setGFScore(boolean GFScore)
	{
		gfScore = GFScore;
	}
	
	/**
	 *  Get method - returns boolean gScore 
	 */
	public boolean getGScore()
	{
		return gScore;
	}
	
	/**
	 *  Set method - sets gScore to new boolean
	 *	@param boolean GScore of new gScore
	 */
	public void setGScore(boolean GScore)
	{
		gScore = GScore;
	}
	
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
	 *  Set method - sets transitionBias to new double
	 *	@param double TransitionBias of new transitionBias
	 */
	public void setTransitionBias(double TransitionBias)
	{
		transitionBias = TransitionBias;
	}
	
	/**
	 *  Get method - returns double transitionBias 
	 */
	public double getTransitionBias()
	{
		return transitionBias;
	}

	/**
	 *  Get method - returns boolean autoTransBias 
	 */
	public boolean getAutoTransBias()
	{
		return autoTransBias;
	}
	
	/**
	 *  Set method - sets autoTransBias to new boolean
	 *	@param boolean AutoTransBias of new autoTransBias
	 */
	public void setAutoTransBias(boolean AutoTransBias)
	{
		autoTransBias = AutoTransBias;
	}

}//CDMUsageBean
