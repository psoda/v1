/**
 *	CDM.class is responsible for the correct extrapolation
 *	of expected changes in the sequences.  These values are stored
 *	in objects of the CDMResultBean.
 *
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.CDM;

import treesaap.Substs.SubstsResultBean;

public class CDM
{
	//CLASS Vars
	private CDMUsageBean bean;			//Bean containing all values necessary for operation
	
	
	/**
	 * CDM Constructor
	 * creates CDMBean object
	 */
	public CDM()
	{
		bean = new CDMUsageBean();
		
	}//Constructor	
	
	/**
	 * CDM Constructor
	 * @param Bean CDMUsageBean usage bean is set to that passed in
	 */
	public CDM(CDMUsageBean Bean)
	{
		bean = Bean;
		
	}//Constructor 
	
	/**
	 * Determines the value of ts4, sets that in the usage bean
	 * @param SubstsResultBean results - the results determined by Substs
	 */
	public void setTransbias(SubstsResultBean subResults)
	{		
		//Check for user inputted transition bias
		if(!bean.getAutoTransBias())
		{
			bean.setTS4(bean.getTransitionBias());
			return;
		}
		
		//Set ts4 - derived or calculated depending on input from user
		if(bean.getDeriveTS4())
			bean.setTS4(getTransBiasByIteration(0.0, 1.0, 0, subResults));
		else
			bean.setTS4(getTransBiasFromEquation(subResults.getCClassFreq(), subResults.getSynonymous()));
	
	}//run
	
	/**
	 * Runs all CDM calculations - Returns an CDMResultBean
	 * @param SubstsResultBean results - the results determined by Substs
	 */
	public CDMResultBean run(SubstsResultBean subResults)
	{
		try
		{
			//Get Totals
			int[][][] observed = getObserved(subResults.getSynonymous(), subResults.getNonSynonymous());
			double[][][] observedFrequency = getObservedFrequency(observed);
			double[][][] expectedFrequency = getExpectedFrequency(subResults.getCClassFreq());
			double[][][] expected = getExpected(observed, expectedFrequency);
			
			//Run Calculations
			double[][] tstvRatios = getTsTvRatios(observed, expected);
			double[] goodnessOfFit = getGoodnessOfFit(observed, expected);
			double[] gScores = getGScores(observed, expected);

			//Place values in result bean and return			
			CDMResultBean cdmResults = new CDMResultBean();		
			cdmResults.setWindowID(subResults.getWindowID());
			cdmResults.setTS4(bean.getTS4());
			cdmResults.setObserved(observed);
			cdmResults.setExpected(expected);
			cdmResults.setObservedFrequency(observedFrequency);
			cdmResults.setExpectedFrequency(expectedFrequency);
			cdmResults.setTSTVRatios(tstvRatios);
			cdmResults.setGoodnessOfFit(goodnessOfFit);
			cdmResults.setGScores(gScores);

			//add to counter
			bean.addToWorkDone(80);
			
			return cdmResults;
		}
		catch(Exception e)
		{
			bean.errorMessage("\nAn error occured while processing in CDM.\n  Please review CDM requirements.");
			return null;
		}
		
	}//run
	
	/**
	 * Returns the calculated transition bias
	 *
	 * @param int[] cclass - contains observed codon class tallies
	 * @param int[][] syn - contains observed synonymous substitions by type
	 */
	private double getTransBiasFromEquation(int[] cclass, int[][] syn)
	{
		//Method vars
		double ts4 = 0.0;
		int syn3pTs = syn[2][0];
		int syn3pTv = syn[2][1];
		
		//Substitute in x and y into calculations
		double x = cclass[0] + cclass[6] + (4.0/3.0)*(cclass[3] + cclass[5]);
		double y = cclass[1] + cclass[2] + cclass[4];
		
		//Calculate transbias
		if(x != 0 && (syn3pTs != 0 || syn3pTv != 0))	
			ts4 = (x*syn3pTs - y*syn3pTv)/(x*(syn3pTs + syn3pTv));
		
		//No codon class 1,4,6, or 7 observed
		else 
			ts4 = 0;
		
		
		//make sure ts4 is positive
		if(ts4 < 0)
			ts4 = 0.0;
		
		return ts4;
	}	
	
	/**
	 * Returns the calculated transition bias
	 *
	 * @param double start - where to start the search
	 * @param double finish - where to end the search
	 * @param int depth - the number of recursions made
	 * @param SubstsResultBean results - the results determined by Substs
	 */
	private double getTransBiasByIteration(double start, double finish, int depth, SubstsResultBean subResults)
	{	
		//return when 10 recursive calls have been made
		if(depth == 10)
			return start;
		
		//get the chunks of examination
		double chunks = (finish-start)/100;
		
		//Go through each chunk, find the one with the least G-Score Syn
		double ts4 = 0.0, gScore = 0.0;
		CDMResultBean results;
		for(int i=0;i<100;i++)
		{
			bean.setTS4(start+(i*chunks));
			results = run(subResults);
			
			if((gScore > results.getGScores()[0] || gScore == 0) && results.getGScores()[0] >= 0)
			{
				gScore = results.getGScores()[0];
				ts4 = start+(i*chunks);
			}
		}
		
		//set start and finish for next iteration
		start = ts4 - 5*chunks;
		finish = ts4 + 5*chunks;
		
		//make sure start & finish 0 < < 1
		if(start < 0)
			start = 0;
		if(finish > 1)
			finish = 1;
		
		return getTransBiasByIteration(start, finish, ++depth, subResults);
	}
	
	/**
	 * Returns the expected frequency of synonymous and nonsynonymous codons
	 * @param int[] cclass - contains observed codon class tallies
	 */
	private double[][][] getExpectedFrequency(int[] cclass)
	{
		//Method vars
		double ts4 = bean.getTS4();
		double [][][] expectedFreq = new double[2][3][2];
		
		//Derive number of potential synonomous & nonsynonymous substitutions
		int synTotal = cclass[0]+cclass[1]+cclass[6]+2*(cclass[2]+cclass[3]+cclass[4]+cclass[5]);
		int nSynTotal = 2*(cclass[0]+cclass[3]+cclass[5])+3*(cclass[1]+cclass[2]+cclass[4]+cclass[6]+cclass[7]);
		
		//Calculate expected frequency of synonymous substitutions by type
		if(synTotal != 0)
		{
			expectedFreq[0][0][0] = (cclass[2]+(2.0/3.0)*cclass[3])/synTotal;
			expectedFreq[0][0][1] = (cclass[4]+(2.0/3.0)*cclass[5])/synTotal;
			expectedFreq[0][2][0] = (cclass[1]+cclass[2]+cclass[4]+ts4*(cclass[0]+cclass[6]+(4.0/3.0)*(cclass[3]+cclass[5])))/synTotal;
			expectedFreq[0][2][1] = ((1.0-ts4)*(cclass[0]+cclass[6]+(4.0/3.0)*(cclass[3]+cclass[5])))/synTotal;
		}
		
		//Calculate expected frequency of nonsynonymous substitutions by type
		if(nSynTotal != 0)
		{
			expectedFreq[1][0][0] = ((3.0/4.0)*cclass[4]+(2.0/3.0)*cclass[5]+ts4*(cclass[0]+(6.0/5.0)*cclass[1]+(4.0/3.0)*cclass[6]+cclass[7]))/nSynTotal;
			expectedFreq[1][0][1] = ((3.0/4.0)*cclass[2]+(2.0/3.0)*cclass[3]+(1.0-ts4)*(cclass[0]+(6.0/5.0)*cclass[1]+(4.0/3.0)*cclass[6]+cclass[7]))/nSynTotal;
			expectedFreq[1][1][0] = (ts4*(cclass[0]+(6.0/5.0)*cclass[1]+(3.0/2.0)*(cclass[2]+cclass[4])+(4.0/3.0)*(cclass[3]+cclass[5]+cclass[6])+cclass[7]))/nSynTotal;
			expectedFreq[1][1][1] = ((1.0-ts4)*(cclass[0]+(6.0/5.0)*cclass[1]+(3.0/2.0)*(cclass[2]+cclass[4])+(4.0/3.0)*(cclass[3]+cclass[5]+cclass[6])+cclass[7]))/nSynTotal;
			expectedFreq[1][2][0] = (ts4*((1.0/3.0)*cclass[6]+cclass[7]))/nSynTotal;
			expectedFreq[1][2][1] = ((3.0/5.0)*cclass[1]+(3.0/4.0)*(cclass[2]+cclass[4])+(1.0-ts4)*((1.0/3.0)*cclass[6]+cclass[7]))/nSynTotal;
		}		
		
		//return the expected frequencies
		return expectedFreq;
	}	
	
	/**
	 * Returns the number of observed substituions
	 *
	 * @param int[][] syn - contains observed synonymous substitions by type
	 * @param int[][] nsyn - contains observed nonsynonymous substitions by type
	 */
	private int[][][] getObserved(int[][] syn, int[][] nsyn)
	{
		//Method Vars
		int[][][] observed = new int[2][3][2];
		
		//place values in subs
		observed[0] = syn;
		observed[1] = nsyn;
		
		return observed;
	}		
	
	/**
	 * Returns the frequency of observed substituions
	 * @param int[][][] observed - contains observed synonymous & nonsynonymous substitions by type
	 */
	private double[][][] getObservedFrequency(int[][][] observed)
	{
		//Method Vars
		int[] total = new int[2];
		double[][][] observedFreq = new double[2][3][2];
	
		//Get totals
		total[0] = observed[0][0][0] + observed[0][0][1] + observed[0][2][0] + observed[0][2][1];
		total[1] = observed[1][0][0] + observed[1][0][1] + observed[1][1][0] + observed[1][1][1] + observed[1][2][0] + observed[1][2][1];
	
		//Both Synonymous and Nonsynonymous
		for(int i=0;i<2;i++)
			//Cannot divide by zero
			if(total[i] != 0)
				//go through positions
				for(int j=0;j<3;j++)
					//go through ts:tv
					for(int k=0;k<2;k++)
						//assign value
						observedFreq[i][j][k] = observed[i][j][k]/(double)total[i];
		
		return observedFreq;
	}	
	
	/**
	 * Returns the number of expected substitions by type
	 *
	 * @param int[][][] observed - contains observed synonymous & nonsynonymous substitions by type
	 * @param double[][][] expectedFreq - contains expected substition frequencies by type
	 */
	private double[][][] getExpected(int[][][] observed, double[][][] expectedFreq)
	{
		//Method Vars
		int[] total = new int[2];
		double[][][] expected = new double[2][3][2];
		
		//Get totals
		total[0] = observed[0][0][0] + observed[0][0][1] + observed[0][2][0] + observed[0][2][1];
		total[1] = observed[1][0][0] + observed[1][0][1] + observed[1][1][0] + observed[1][1][1] + observed[1][2][0] + observed[1][2][1];
		
		//Both Synonymous and Nonsynonymous
		for(int i=0;i<2;i++)
			//go through positions
			for(int j=0;j<3;j++)
				//go through ts:tv
				for(int k=0;k<2;k++)
					//assign value
					expected[i][j][k] = expectedFreq[i][j][k]*total[i];
		
		return expected;
	}	

	/**
	 * Returns the goodness of fit values for synonymous and nonsynonymous substitions
	 *
	 * @param int[][][] observed - contains observed synonymous & nonsynonymous substitions by type
	 * @param double[][][] expected - contains expected substitions by type
	 */
	private double[] getGoodnessOfFit(int[][][] observed, double[][][] expected)
	{
		//Method Vars
		double tempNum;
		double[] goodnessOfFit = new double[2];
		
		//Both Synonymous and Nonsynonymous
		for(int i=0;i<2;i++)
			//go through positions
			for(int j=0;j<3;j++)
				//go through ts:tv
				for(int k=0;k<2;k++)
					//Cannot divide by zero
					if(expected[i][j][k] != 0)
					{
						//assign value
						tempNum = observed[i][j][k] - expected[i][j][k];
						goodnessOfFit[i] += (tempNum*tempNum) / expected[i][j][k];
					}

		return goodnessOfFit;
	}	

	/**
	 * Returns the ts:tv ratios
	 *
	 * @param int[][][] observed - contains observed synonymous & nonsynonymous substitions by type
	 * @param double[][][] expected - contains expected substitions by type
	 */
	private double[][] getTsTvRatios(int[][][] observed, double[][][] expected)
	{
		//Method Vars
		double tempNum;
		double[][] tstvRatios = new double[2][2];

		//Both Synonymous and Nonsynonymous Observed
		for(int j=0;j<2;j++)
		{
			tempNum = observed[j][0][1] + observed[j][1][1] + observed[j][2][1];
			
			//Cannot divide by zero
			if(tempNum != 0)
				tstvRatios[0][j] = (observed[j][0][0] + observed[j][1][0] + observed[j][2][0])/tempNum;
		}
		
		//Both Synonymous and Nonsynonymous Expected
		for(int j=0;j<2;j++)
		{
			tempNum = expected[j][0][1] + expected[j][1][1] + expected[j][2][1];
			
			//Cannot divide by zero
			if(tempNum != 0)
				tstvRatios[1][j] = (expected[j][0][0] + expected[j][1][0] + expected[j][2][0])/tempNum;
		}

		
		return tstvRatios;
	}	
	
	/**
	 * Returns the caculated G-Scores
	 *
	 * @param int[][][] observed - contains observed synonymous & nonsynonymous substitions by type
	 * @param double[][][] expected - contains expected substitions by type
	 */
	private double[] getGScores(int[][][] observed, double[][][] expected)
	{
		//Method Vars
		double[] gScores = new double[2];
		
		//Both Synonymous and Nonsynonymous
		for(int i=0;i<2;i++)
			//go through positions
			for(int j=0;j<3;j++)
				//go through ts:tv
				for(int k=0;k<2;k++)			
					//Cannot divide by zero
					if(observed[i][j][k] != 0 && expected[i][j][k] != 0)
						//assign value
						gScores[i] += 2 * observed[i][j][k] * Math.log(observed[i][j][k]/expected[i][j][k]);

		return gScores;
	}	
	

	/**
	 * Get method - returns CDMUsageBean bean
	 */
	public CDMUsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Set method - sets bean here and lower classes
	 *	@param Bean CDMUsageBean is the new bean 
	 */
	public void setBean(CDMUsageBean Bean)
	{
		bean = Bean;
	}//setBean 
	
}//CDM
