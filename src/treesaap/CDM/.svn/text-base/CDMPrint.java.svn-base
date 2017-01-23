/**
 *	CDMPrint.class is responsible for the correct extrapolation
 *	of expected changes in the sequences.  These values are stored
 *	in objects of the EvpthwyBean.
 *
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.CDM;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Vector;
import java.util.Hashtable;
import java.text.DecimalFormat;

import treesaap.Substs.SubstsResultBean;
import treesaap.Substs.TableBean;

public class CDMPrint
{
	//CLASS Vars
	private CDMUsageBean bean;			//Bean containing all values necessary for operation

	
	/**
	 * CDMPrint Constructor
	 * @param Bean CDMUsageBean usage bean is set to that passed in
	 */
	public CDMPrint(CDMUsageBean Bean)
	{
		bean = Bean;
		
	}//Constructor 

	/**
	 *	Writes Substs and CDM results to file
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector runResults - contains the all cdm and substs result information
	 */
	public void writeResults(String fileName, Vector runResults)
	{
		//Substs Method Vars
		CDMResultBean results = (CDMResultBean)runResults.elementAt(1);
		SubstsResultBean subResults = (SubstsResultBean)runResults.elementAt(0);
		TableBean tBean = new TableBean();
		int[] freq = subResults.getFreq();
		int[] cClass = subResults.getCClassFreq();
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//Codon Class Tally
			outFile.println("Codon Class Count:");
			for(int i=0;i<8;i++)
				outFile.println((i+1) +":	"+ cClass[i]);
			
			//Codon Tally chart
			outFile.println("\nCodon Tally:	***	"+ freq[0]);
			for(int j=0;j<52;j++)
			{
				if(j != 0 && j%4 == 0 && j%16 != 0)
					j+=12;
				outFile.println(tBean.getCodonName(j) +"	"+ freq[j+1] +"	"+ tBean.getCodonName(j+4) +"	"+ freq[j+5] +"	"+ tBean.getCodonName(j+8) +"	"+ freq[j+9] +"	"+ tBean.getCodonName(j+12) +"	"+ freq[j+13]);
			}

			//close file
			outFile.close();
			
			//write cdm results
			writeCDMResults(fileName, results);
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write CDM results.\n  Writing to file: "+ fileName);
		}
	}	

	/**
	 *	Writes Sliding window CDM results to a single file
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector runResults - contains the all cdm and substs result information
	 */
	public void writeSlidingWindowResultFile(String fileName, Vector runResults)
	{
		//Method Vars
		String outLine = "", header = "";
		CDMResultBean results;
		int[][][] observed;
		double[][][] observedFreq, expected, expectedFreq;
		TableBean tBean = new TableBean();
		DecimalFormat form = new DecimalFormat("0.####");

		try
		{			
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//Header
			header = "Window		";
			outLine = "From	To	";
			
			if(bean.getGScore())
			{
				header += "G-Score		";
				outLine += "Syn	Nsyn	";
			}
			if(bean.getGFScore())
			{
				header += "GF-Score		";
				outLine += "Syn	Nsyn	";
			}
			if(bean.getTsTv())
			{
				header += "Observed Ts:Tv		Expected Ts:Tv		";
				outLine += "Syn	Nsyn	Syn	Nsyn	";
			}
			if(bean.getTransbias())			
			{
				header += "Transbias	";
				outLine += "ts4	";
			}
			if(bean.getObserved())
			{
				header += "Observed Substitution										";
				outLine += "Syn 1pts	Syn 1ptv	Syn 3pts	Syn 3ptv	Nsyn 1pts	Nsyn 1ptv	Nsyn 2pts	Nsyn 2ptv	Nsyn 3pts	Nsyn 3ptv	";
			}
			if(bean.getObservedFrequency())
			{
				header += "Observed Frequency										";
				outLine += "Syn 1pts	Syn 1ptv	Syn 3pts	Syn 3ptv	Nsyn 1pts	Nsyn 1ptv	Nsyn 2pts	Nsyn 2ptv	Nsyn 3pts	Nsyn 3ptv	";
			}
			if(bean.getExpected())
			{
				header += "Expected Substitution										";
				outLine += "Syn 1pts	Syn 1ptv	Syn 3pts	Syn 3ptv	Nsyn 1pts	Nsyn 1ptv	Nsyn 2pts	Nsyn 2ptv	Nsyn 3pts	Nsyn 3ptv	";
			}
			if(bean.getExpectedFrequency())
			{
				header += "Expected Frequency										";
				outLine += "Syn 1pts	Syn 1ptv	Syn 3pts	Syn 3ptv	Nsyn 1pts	Nsyn 1ptv	Nsyn 2pts	Nsyn 2ptv	Nsyn 3pts	Nsyn 3ptv	";
			}
			
			//Remove extra tab
			header = header.substring(0,header.length()-1);
			outLine = outLine.substring(0,outLine.length()-1);
			
			//Print Header
			outFile.println(header);
			outFile.println(outLine);
			
			//Iterate through results
			for(int i=2;i<runResults.size();i++)
			{
				//CDM Method Vars
				outLine = "";
				results = (CDMResultBean)runResults.elementAt(i);
				observed = results.getObserved();
				observedFreq = results.getObservedFrequency();
				expected = results.getExpected();
				expectedFreq = results.getExpectedFrequency();
				
				//Print Window
				header = results.getWindowID();
				outLine = header.substring(0,header.indexOf(" ")) +"	"+ header.substring(header.indexOf(" ")+1) +"	";
				
				//Print Calculations
				if(bean.getGScore())
					outLine += form.format(results.getGScores()[0]) +"	"+ form.format(results.getGScores()[1]) +"	";
				if(bean.getGFScore())
					outLine += form.format(results.getGoodnessOfFit()[0]) +"	"+ form.format(results.getGoodnessOfFit()[1]) +"	";
				if(bean.getTsTv())
					outLine += form.format(results.getTSTVRatios()[0][0]) +"	"+ form.format(results.getTSTVRatios()[0][1]) +"	"+ form.format(results.getTSTVRatios()[1][0]) +"	"+ form.format(results.getTSTVRatios()[1][1]) +"	";				
				
				//Print Intermediate vars
				if(bean.getTransbias())			
					outLine += form.format(results.getTS4()) +"	";

				//Print total vars
				if(bean.getObserved())
				{
					outLine += form.format(observed[0][0][0]) +"	"+ form.format(observed[0][0][1]) +"	"+ form.format(observed[0][2][0]) +"	"+ form.format(observed[0][2][1]) +"	";
					outLine += form.format(observed[1][0][0]) +"	"+ form.format(observed[1][0][1]) +"	"+ form.format(observed[1][1][0]) +"	"+ form.format(observed[1][1][1]) +"	"+ form.format(observed[1][2][0]) +"	"+ form.format(observed[1][2][1]) +"	";
				}
				if(bean.getObservedFrequency())
				{
					outLine += form.format(observedFreq[0][0][0]) +"	"+ form.format(observedFreq[0][0][1]) +"	"+ form.format(observedFreq[0][2][0]) +"	"+ form.format(observedFreq[0][2][1]) +"	";
					outLine += form.format(observedFreq[1][0][0]) +"	"+ form.format(observedFreq[1][0][1]) +"	"+ form.format(observedFreq[1][1][0]) +"	"+ form.format(observedFreq[1][1][1]) +"	"+ form.format(observedFreq[1][2][0]) +"	"+ form.format(observedFreq[1][2][1]) +"	";
				}
				if(bean.getExpected())
				{
					outLine += form.format(expected[0][0][0]) +"	"+ form.format(expected[0][0][1]) +"	"+ form.format(expected[0][2][0]) +"	"+ form.format(expected[0][2][1]) +"	";
					outLine += form.format(expected[1][0][0]) +"	"+ form.format(expected[1][0][1]) +"	"+ form.format(expected[1][1][0]) +"	"+ form.format(expected[1][1][1]) +"	"+ form.format(expected[1][2][0]) +"	"+ form.format(expected[1][2][1]) +"	";
				}
				if(bean.getExpectedFrequency())
				{
					outLine += form.format(expectedFreq[0][0][0]) +"	"+ form.format(expectedFreq[0][0][1]) +"	"+ form.format(expectedFreq[0][2][0]) +"	"+ form.format(expectedFreq[0][2][1]) +"	";
					outLine += form.format(expectedFreq[1][0][0]) +"	"+ form.format(expectedFreq[1][0][1]) +"	"+ form.format(expectedFreq[1][1][0]) +"	"+ form.format(expectedFreq[1][1][1]) +"	"+ form.format(expectedFreq[1][2][0]) +"	"+ form.format(expectedFreq[1][2][1]) +"	";
				}
				
				//Remove extra tab
				outLine = outLine.substring(0,outLine.length()-1);
			
				outFile.println(outLine);
			}
			//close file
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write CDM results.\n  Writing to file: "+ fileName);
		}
	}	

	/**
	 *	Writes Sliding window CDM results for each window to a file in the specified directory
	 *
	 *	@param String path - validated path to directory where information files are printed
	 *	@param Vector runResults - contains the all cdm and substs result information
	 */
	public void writeSlidingWindowResults(String path, Vector runResults)
	{
		//Method Vars
		CDMResultBean results;
		String validPath, windowNum;
		
		try
		{			
			//Validate path
			validPath = bean.getAbsoluteDirPath(path);
			if(validPath == null)				
				throw new Exception();
			path = validPath + File.separator;
			
			//Iterate through results
			for(int i=2;i<runResults.size();i++)
			{
				results = (CDMResultBean)runResults.elementAt(i);
				windowNum = results.getWindowID();
				windowNum = windowNum.substring(0, windowNum.indexOf(" "));
				writeCDMResults(path + windowNum +".txt" , results);
			}
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write CDM results.\n  Writing to path: "+ path);
		}
	}	

	/**
	 *	Appends the CDM results to the file specified
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector runResults - contains the all cdm and substs result information
	 */
	private void writeCDMResults(String fileName, CDMResultBean results)
	{
		//CDM Method Vars
		int[][][] observed = results.getObserved();
		double[][][] observedFreq = results.getObservedFrequency();
		double[][][] expected = results.getExpected();
		double[][][] expectedFreq = results.getExpectedFrequency();
		DecimalFormat form = new DecimalFormat("0.####");
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName, true));
		
			//Print Calculations
			outFile.println("\nChange	G-Score	Goodness-of-fit	Observed ts:tv	Expected ts:tv");
			outFile.println("Syn	"+ form.format(results.getGScores()[0]) +"	"+ form.format(results.getGoodnessOfFit()[0]) +"	"+ form.format(results.getTSTVRatios()[0][0]) +"	"+ form.format(results.getTSTVRatios()[1][0]));
			outFile.println("Nsyn	"+ form.format(results.getGScores()[1]) +"	"+ form.format(results.getGoodnessOfFit()[1]) +"	"+ form.format(results.getTSTVRatios()[0][1]) +"	"+ form.format(results.getTSTVRatios()[1][1]));
			
			//Print Intermediate vars
			outFile.println("\nTransition Bias (ts4):	"+ form.format(results.getTS4()));
			
			//Print total vars
			outFile.println("\nChange	Amnt	Observed Sub	Expected Sub	Observed Freq	Expected Freq");
			outFile.println("Syn	1pTs	"+ form.format(observed[0][0][0]) +"	"+ form.format(expected[0][0][0]) +"	"+ form.format(observedFreq[0][0][0]) +"	"+ form.format(expectedFreq[0][0][0]));
			outFile.println("Syn	1pTv	"+ form.format(observed[0][0][1]) +"	"+ form.format(expected[0][0][1]) +"	"+ form.format(observedFreq[0][0][1]) +"	"+ form.format(expectedFreq[0][0][1]));
			outFile.println("Syn	3pTs	"+ form.format(observed[0][2][0]) +"	"+ form.format(expected[0][2][0]) +"	"+ form.format(observedFreq[0][2][0]) +"	"+ form.format(expectedFreq[0][2][0]));
			outFile.println("Syn	3pTv	"+ form.format(observed[0][2][1]) +"	"+ form.format(expected[0][2][1]) +"	"+ form.format(observedFreq[0][2][1]) +"	"+ form.format(expectedFreq[0][2][1]));
			outFile.println("Nsyn	1pTs	"+ form.format(observed[1][0][0]) +"	"+ form.format(expected[1][0][0]) +"	"+ form.format(observedFreq[1][0][0]) +"	"+ form.format(expectedFreq[1][0][0]));
			outFile.println("Nsyn	1pTv	"+ form.format(observed[1][0][1]) +"	"+ form.format(expected[1][0][1]) +"	"+ form.format(observedFreq[1][0][1]) +"	"+ form.format(expectedFreq[1][0][1]));
			outFile.println("Nsyn	2pTs	"+ form.format(observed[1][1][0]) +"	"+ form.format(expected[1][1][0]) +"	"+ form.format(observedFreq[1][1][0]) +"	"+ form.format(expectedFreq[1][1][0]));
			outFile.println("Nsyn	2pTv	"+ form.format(observed[1][1][1]) +"	"+ form.format(expected[1][1][1]) +"	"+ form.format(observedFreq[1][1][1]) +"	"+ form.format(expectedFreq[1][1][1]));
			outFile.println("Nsyn	3pTs	"+ form.format(observed[1][2][0]) +"	"+ form.format(expected[1][2][0]) +"	"+ form.format(observedFreq[1][2][0]) +"	"+ form.format(expectedFreq[1][2][0]));
			outFile.println("Nsyn	3pTv	"+ form.format(observed[1][2][1]) +"	"+ form.format(expected[1][2][1]) +"	"+ form.format(observedFreq[1][2][1]) +"	"+ form.format(expectedFreq[1][2][1]));
			
			//close file
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write CDM results.\n  Writing to file: "+ fileName);
		}
	}

}//CDMPrint