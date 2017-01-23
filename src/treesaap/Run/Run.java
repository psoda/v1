/**
 *	Run.class is responsible for the output
 *	of the progress in the substs and evpthwy algorithms. 
 *	Developed specifically for sliding window use.
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Run;

import java.util.Vector;
import java.util.Hashtable;

import treesaap.CDM.CDM;
import treesaap.CDM.CDMUsageBean;
import treesaap.CDM.CDMResultBean;
import treesaap.Evpthwy.Evpthwy;
import treesaap.Evpthwy.EvpthwyUsageBean;
import treesaap.Evpthwy.EvpthwyResultBean;
import treesaap.GeneralDNAFileParser.TaxaBean;
import treesaap.Substs.Substs;
import treesaap.Substs.SubstsResultBean;
import treesaap.Substs.SubstsUsageBean;

public class Run
{
	//Shared Objects
	private RunUsageBean bean;					//this class's usage bean
	
	//Control Objects
	private Substs substs;
	private Evpthwy evpthwy;
	private CDM cdm;
	
	//Thread Vars
	private Vector evpthwyResults, cdmResults;
	private int totalWindows;
	
	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * calls method to initialize variables to be used.
	 *
	 * 	@param RunUsageBean SUBbean - usage bean of this class
	 */
	public Run(RunUsageBean Bean)
	{
		bean = Bean;
		
	}//Constructor

	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * calls method to initialize variables to be used.
	 */
	public Run()
	{
		bean = new RunUsageBean();
		
	}//Constructor

	/**
	 * Runs Substs and Evpthwy calculations as specified in the UsageBean.
	 * Executes single pass run and a Sliding Window protocol.
	 */
	public Vector runSubstsEvpthwy()
	{
		//Check for multi-threading
		if(bean.getProcessors() > 1)
			return runSubstsEvpthwyThreads();
		
		//Set Beans
		substs = bean.getSubsts();
		evpthwy = bean.getEvpthwy();
		
		//Method Vars - get length from first taxa listed
		SubstsResultBean subResult;
		evpthwyResults = new Vector();

		try
		{
			int increment = bean.getIncrement();
			int winSize = bean.getSlidingWindowSize();	
			int length = ((TaxaBean)substs.getBean().getTaxa().get((String)substs.getBean().getTree().getTaxaNames().elementAt(0))).getSequence().length()/3;
			
			//Check for nodes
			if(substs.getBean().getTree().getNodes().size() < 1)
				throw new Exception("Fewer than one node in tree " + substs.getBean().getTree().getName());
			
			//verify winSize
			if(winSize == -1 || winSize > length)
				winSize = length;
			
			//set Work up
			substs.getBean().setDetailed(false);
			setSubstsTotalWork(length);
			setEvpthwyTotalWork(length);
			
			//Start progress window
			SubstsEvpthwyProgressWindow prog = new SubstsEvpthwyProgressWindow(bean);
			
			//Run Normal Calculation
			subResult = substs.calculate(0,length*3);
			evpthwyResults.add(subResult);
			evpthwyResults.add(evpthwy.run(subResult));
				
			//Run Sliding Window
			if(winSize != length)
			{		
				//go through the windows of the sequence
				for(int i=0;i<=length-winSize;i+=increment)
				{
					//Run Substs & Evpthwy
					subResult = substs.calculate(i*3,(i+winSize)*3);
					evpthwyResults.add(evpthwy.run(subResult));
				}
			}
			
			//End progress window
			prog.end();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was an error while running Substs/Evpthwy.  \nPlease Verify that correct data was inputted.");
			
			e.printStackTrace();
		}
		
		//add results to hashtable and return
		return evpthwyResults;
	}
	
	/**
	 * Runs Substs and CDM calculations as specified in the UsageBean.
	 * Executes single pass run and a Sliding Window protocol.
	 */
	public Vector runSubstsCDM()
	{
		//Check for multi-threading
		if(bean.getProcessors() > 1)
			return runSubstsCDMThreads();
		
		//Set Beans
		substs = bean.getSubsts();
		cdm = bean.getCDM();
		
		//Method Vars - get length from first taxa listed
		SubstsResultBean subResult;
		cdmResults = new Vector();
		
		try
		{
			int increment = bean.getIncrement();
			int winSize = bean.getSlidingWindowSize();			
			int length = ((TaxaBean)substs.getBean().getTaxa().get((String)substs.getBean().getTree().getTaxaNames().elementAt(0))).getSequence().length()/3;
			
			//Check for nodes
			if(substs.getBean().getTree().getNodes().size() < 1)
				throw new Exception();
			
			//verify winSize
			if(winSize == -1 || winSize > length)
				winSize = length;
			
			//set Work up
			substs.getBean().setDetailed(true);
			setSubstsTotalWork(length);
			setCDMTotalWork(length);
			
			//Start progress window
			SubstsCDMProgressWindow prog = new SubstsCDMProgressWindow(bean);
			
			//Run Normal Calculation
			subResult = substs.calculate(0,length*3);
			cdmResults.add(subResult);
			cdm.setTransbias(subResult);
			cdmResults.add(cdm.run(subResult));
			
			//Run Sliding Window
			if(winSize != length)
			{				
				//go through the windows of the sequence
				for(int i=0;i<=length-winSize;i+=increment)
				{
					//Run Substs
					subResult = substs.calculate(i*3,(i+winSize)*3);
					
					//check to see if transbias is fixed
					if(!cdm.getBean().getFixedTransBias())
						cdm.setTransbias(subResult);
					
					//Run CDM
					cdmResults.add(cdm.run(subResult));
				}
			}						
			
			//End progress window
			prog.end();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was an error while running Substs/CDM.  \nPlease Verify that correct data was inputted.");
		}
		
		//add results to vector and return
		return cdmResults;
	}
	
	/**
	 * Runs Substs and Evpthwy calculations as specified in the UsageBean.
	 * Executes Sliding Window protocol and seperates threads according to processors
	 */
	private Vector runSubstsEvpthwyThreads()
	{
		//Set Beans
		substs = bean.getSubsts();
		evpthwy = bean.getEvpthwy();
		
		//Method Vars - get length from first taxa listed
		SubstsResultBean subResult;
		evpthwyResults = new Vector();
		
		try
		{
			//More Method Vars
			int winSize = bean.getSlidingWindowSize();			
			int length = ((TaxaBean)substs.getBean().getTaxa().get((String)substs.getBean().getTree().getTaxaNames().elementAt(0))).getSequence().length()/3;

			//Check for nodes
			if(substs.getBean().getTree().getNodes().size() < 1)
				throw new Exception();
			
			//verify winSize
			if(winSize == -1 || winSize > length)
				winSize = length;
			
			//set Work up
			substs.getBean().setDetailed(false);
			setSubstsTotalWork(length);
			setEvpthwyTotalWork(length);
			totalWindows = getNumWindows();
			
			//Start progress window
			SubstsEvpthwyProgressWindow prog = new SubstsEvpthwyProgressWindow(bean);
			
			//Run Normal Calculation
			subResult = substs.calculate(0,length*3);
			evpthwyResults.add(subResult);
			evpthwyResults.add(evpthwy.run(subResult));
			
			//Run Sliding Window
			if(winSize != length)
			{				
				//Create control thread and wait for it
				ControlThread control = new ControlThread();
				control.runSubstsEvpthwy(control, length);	
			}

			//End progress window
			prog.end();
			
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was an error while running Substs/Evpthwy.  \nPlease Verify that correct data was inputted.");
		}
		
		//add results to hashtable and return
		return evpthwyResults;
	}

	/**
	 * Runs Substs and CDM calculations as specified in the UsageBean.
	 * Executes Sliding Window protocol and separates threads according to processors
	 */
	private Vector runSubstsCDMThreads()
	{
		//Set Beans
		substs = bean.getSubsts();
		cdm = bean.getCDM();
		
		//Method Vars - get length from first taxa listed
		SubstsResultBean subResult;
		cdmResults = new Vector();
		
		try
		{
			//More Method Vars
			int winSize = bean.getSlidingWindowSize();			
			int length = ((TaxaBean)substs.getBean().getTaxa().get((String)substs.getBean().getTree().getTaxaNames().elementAt(0))).getSequence().length()/3;
			
			//Check for nodes
			if(substs.getBean().getTree().getNodes().size() < 1)
				throw new Exception();
			
			//verify winSize
			if(winSize == -1 || winSize > length)
				winSize = length;
			
			//set Work up
			substs.getBean().setDetailed(true);
			setSubstsTotalWork(length);
			setCDMTotalWork(length);
			totalWindows = getNumWindows();
			
			//Start progress window
			SubstsCDMProgressWindow prog = new SubstsCDMProgressWindow(bean);
			
			//Run Normal Calculation
			subResult = substs.calculate(0,length*3);
			cdmResults.add(subResult);
			cdm.setTransbias(subResult);
			cdmResults.add(cdm.run(subResult));
			
			//Run Sliding Window
			if(winSize != length)
			{				
				//Create control thread and wait for it
				ControlThread control = new ControlThread();
				control.runSubstsCDM(control, length);	
			}			
			
			//End progress window
			prog.end();
			
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was an error while running Substs/CDM.  \nPlease Verify that correct data was inputted.");
			e.printStackTrace();
		}
		
		//add results to hashtable and return
		return cdmResults;
	}

	/**
	 *	Calculates and returns a long representing the total 
	 *	amount of work to be done by this class by parsing
	 *
	 *	@param int length - the length of the sequence being ran
	 */
	private void setSubstsTotalWork(int length)
	{
		//Method vars
		long totalWork = 0;
		SubstsUsageBean subBean = substs.getBean();
		int AncSize = subBean.getTree().getAncestralTree().size()/2;
		
		//Sliding Window total Work
		if(bean.getSlidingWindowSize() != -1 && bean.getSlidingWindowSize() != length)
		{
			//Basic amount - compare size * windowSize;	
			totalWork = AncSize * bean.getSlidingWindowSize();
			
			//if !detailed, add more work
			if(!subBean.getDetailed())
				totalWork += subBean.getGCBean().getPropertyNames().size() * (totalWork + 66);
			
			//Multiply totalWork by Number of Windows
			totalWork *= getNumWindows();
		}
		
		//add to totalWork one pass
		totalWork += AncSize * length;
		
		//if Evpthwy, add a little more
		if(!subBean.getDetailed())
			totalWork += subBean.getGCBean().getPropertyNames().size() * (AncSize * length + 66);
		
		subBean.setWorkDone(0);
		subBean.setTotalWork(totalWork);
	}//totalWork
	
	/**
	 *	Calculates and returns a long representing the total 
	 *	amount of work to be done by this class by parsing
	 *
	 *	@param int length - the length of the sequence being ran
	 */
	private void setEvpthwyTotalWork(int length)
	{
		//Method vars
		long totalWork = 0;
		EvpthwyUsageBean evBean = evpthwy.getBean();
		int propNum = evBean.getGCBean().getPropertyNames().size();

		//Sliding Window total Work
		if(bean.getSlidingWindowSize() != -1 && bean.getSlidingWindowSize() != length)
		{
			//Basic amount - property Names * cat Num * 64 codons
			totalWork = 70 * propNum;
			totalWork += 66 * evBean.getNumberOfCat() * propNum;
			
			//Multiply totalWork by Number of Windows
			totalWork *= (getNumWindows() + 1);
		}
		//No Sliding Window
		else
		{
			totalWork = 70 * propNum;
			totalWork += 66 * evBean.getNumberOfCat() * propNum;
		}
		
		evBean.setWorkDone(0);
		evBean.setTotalWork(totalWork);
	}//totalWork	
	
	/**
	 *	Calculates and returns a long representing the total 
	 *	amount of work to be done by this class by parsing
	 *
	 *	@param int length - the length of the sequence being ran
	 */
	private void setCDMTotalWork(int length)
	{
		//Method vars
		int amount = 80;
		int numWindows = 1;
		long totalWork = 0;
		CDMUsageBean cdmBean = cdm.getBean();
		
		//Sliding Window total Work
		if(bean.getSlidingWindowSize() != -1 && bean.getSlidingWindowSize() != length)
			numWindows += getNumWindows();

		//Total amount - if derive ts4
		if(cdmBean.getDeriveTS4())
		{
			if(cdm.getBean().getFixedTransBias() || numWindows == 1)
				totalWork = amount * (numWindows + 1000);
			else
				totalWork = numWindows * amount * 1001;
		}
		else
			totalWork = numWindows * amount;
		
		cdmBean.setWorkDone(0);
		cdmBean.setTotalWork(totalWork);
	}//totalWork	
	
	
	/**
	 * Calculates and returns an int 
	 * representing the number of windows will be used
	 */
	private int getNumWindows()
	{
		int windows = 0;
		SubstsUsageBean subBean = substs.getBean();
		
		//Length of sequence
		int length = ((TaxaBean)subBean.getTaxa().get((String)subBean.getTree().getTaxaNames().elementAt(0))).getSequence().length()/3;
		
		//Get number of windows
		windows = ((length - bean.getSlidingWindowSize())/bean.getIncrement()) + 1;
		
		//check - for(int i=1;i<=length-windowSize+1;i+=increment)	obsWin++;
		
		return windows;
	}//totalWork

	/**
	 * Get method - returns RunUsageBean bean
	 */
	public RunUsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Set method - sets bean here and lower classes
	 *	@param Bean RunUsageBean is the new bean 
	 */
	public void setBean(RunUsageBean Bean)
	{
		bean = Bean;
	}//setBean 
	
	
	/**
	 *	Runs sliding window analysis with manages SubstsEvpthwyTreads
	 */
	class ControlThread extends Thread
	{
		/**
		 *	Controls all threads involved with running Substs and CDM
		 *
		 *	@param ControlThread control - reference to a thread
		 *	@param int length - the length of the sequence
		 */
		private void runSubstsCDM(ControlThread control, int length)
		{
			try
			{
				//Method Vars
				SubstsCDMThread thread;
				int increment = bean.getIncrement();
				int chunk = totalWindows/bean.getProcessors();
				
				//Spawn Threads - i represents window number
				for(int i=0;i<totalWindows;i+=chunk)
				{					
					//Determine if there's an odd amount left
					if(totalWindows - (i+chunk) < chunk)
						chunk += totalWindows - (i+chunk);
					
//					System.out.println("Start: "+ i*increment +"-->"+ (i*increment+bean.getSlidingWindowSize()));
//					System.out.println("Finish: "+ ((i+chunk)*increment-increment) +"-->"+ ((i+chunk)*increment+bean.getSlidingWindowSize()-increment));
					
					//Create and set start on the thread
					thread = new SubstsCDMThread();
					thread.setStart(i*increment);
					thread.setFinish((i+chunk)*increment);
					
					//run thread
					thread.start();
				}
				
				//Resolve a deadlock
				int same = 0;
				int prevWindowNum = 0;
				
				//wait for threads to finish
				while(cdmResults.size()-2 != totalWindows)
				{
					//check deadlock situation
					if(prevWindowNum == cdmResults.size())
						same++;
					else
						same = 0;
					
					//get out of deadlock
					if(same == 15)
						throw new Exception();
					
					prevWindowNum = cdmResults.size();
					control.sleep(100);
				}
			}
			catch(Exception e)
			{ 
				bean.errorMessage("\nThread error in Run.java"); 
			}
			
		}//run
		
		
		/**
		 *	Controls all threads involved with running Substs and Evpthwy
		 *
		 *	@param ControlThread control - reference to a thread
		 *	@param int length - the length of the sequence
		 */
		private void runSubstsEvpthwy(ControlThread control, int length)
		{
			try
			{
				//Method Vars
				SubstsEvpthwyThread thread;
				int increment = bean.getIncrement();
				int chunk = totalWindows/bean.getProcessors();
		
				//Spawn Threads - i represents window number
				for(int i=0;i<totalWindows;i+=chunk)
				{					
					//Determine if there's an odd amount left
					if(totalWindows - (i+chunk) < chunk)
						chunk += totalWindows - (i+chunk);
					
//					System.out.println("Start: "+ i*increment +"-->"+ (i*increment+bean.getSlidingWindowSize()));
//					System.out.println("Finish: "+ ((i+chunk)*increment-increment) +"-->"+ ((i+chunk)*increment+bean.getSlidingWindowSize()-increment));

					//Create and set start on the thread
					thread = new SubstsEvpthwyThread();
					thread.setStart(i*increment);
					thread.setFinish((i+chunk)*increment);

					//run thread
					thread.start();
				}
			
				//Resolve a deadlock
				int same = 0;
				int prevWindowNum = 0;
				
				//wait for threads to finish
				while(evpthwyResults.size()-2 != totalWindows)
				{
					//check deadlock situation
					if(prevWindowNum == evpthwyResults.size())
						same++;
					else
						same = 0;
						
					//get out of deadlock
					if(same == 15)
						throw new Exception();
					
					prevWindowNum = evpthwyResults.size();
					control.sleep(100);
				}
			}
			catch(Exception e)
			{ 
				bean.errorMessage("\nThread error in Run.java"); 
			}
			
		}//run
	
	}//ProgressWindowThread

	/**
	 *	Runs sliding window analysis with different threads
	 */
	class SubstsCDMThread extends Thread
	{
		//Class Vars
		private int start;
		private int finish;
		
		/**
		 *	This is the class that is called when thread is started
		 */
		public void run()
		{
			//Method Var
			SubstsResultBean subResult;
			int increment = bean.getIncrement();
			int winSize = bean.getSlidingWindowSize();			
			
			//go through the windows of the sequence
			for(int i=start;i<finish;i+=increment)
			{
				//Run Substs
				subResult = substs.calculate(i*3,(i+winSize)*3);
				
				//check to see if transbias is fixed
				if(!cdm.getBean().getFixedTransBias())
					cdm.setTransbias(subResult);
				
				//Run CDM
				cdmResults.add(cdm.run(subResult));
			}
		}//run
		
		/**
		 *  Set method - sets start to new int
		 *	@param int Start of new start
		 */
		public void setStart(int Start)
		{
			start = Start;
		}
		
		/**
		 *  Set method - sets finish to new int
		 *	@param int Finish of new finish
		 */
		public void setFinish(int Finish)
		{
			finish = Finish;
		}
	}//SubstsCDMThread
	
	/**
	 *	Runs sliding window analysis with different threads
	 */
	class SubstsEvpthwyThread extends Thread
	{
		//Class Vars
		private int start;
		private int finish;
		
		/**
		 *	This is the class that is called when thread is started
		 */
		public void run()
		{
			//Method Var
			SubstsResultBean subResult;
			int increment = bean.getIncrement();
			int winSize = bean.getSlidingWindowSize();			
			
			//go through the windows of the sequence
			for(int i=start;i<finish;i+=increment)
			{
				//Run Substs & Evpthwy
				subResult = substs.calculate(i*3,(i+winSize)*3);
				evpthwyResults.add(evpthwy.run(subResult));
			}
		}//run
		
		/**
		 *  Set method - sets start to new int
		 *	@param int Start of new start
		 */
		public void setStart(int Start)
		{
			start = Start;
		}
		
		/**
		 *  Set method - sets finish to new int
		 *	@param int Finish of new finish
		 */
		public void setFinish(int Finish)
		{
			finish = Finish;
		}
	}//SubstsEvpthwyThread

}//Run
