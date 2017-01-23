/**
 *	EvpthwyPrint.class is responsible for the correct printing
 *	of results of a given EvpthwyResultBean to file
 *
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 *
 *	jExcel functionality added by Tim Adair
 */
package treesaap.Evpthwy;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Vector;
import java.util.Hashtable;
import java.text.DecimalFormat;

import treesaap.Data.ProteinBean;
import treesaap.Substs.SubstsBean;
import treesaap.Substs.SubstsResultBean;
import treesaap.Substs.TableBean;

import jxl.*;
import jxl.write.*;

public class EvpthwyPrint
{
	//CLASS Vars
	private EvpthwyUsageBean bean;			//Bean containing all values necessary for operation

	
	/**
	 * Evpthwy Constructor
	 * @param Bean EvpthwyUsageBean usage bean is set to that passed in
	 */
	public EvpthwyPrint(EvpthwyUsageBean Bean)
	{
		bean = Bean;
		
	}//Constructor 

	/**
	 *	Writes Selected Property results to file - one file for each property
	 *
	 *	@param String path - validated path, where information is printed
	 *	@param SubstsResultBean subResBean - contains the all substs information
	 *	@param EvpthwyResultBean evResBean - contains the all evpthwy information
	 */
	public void writeCategoryTotalsFolder(String path, SubstsResultBean subResBean, EvpthwyResultBean evResBean)
	{
		//Method Vars
		String validPath, newPath = "";
		PrintWriter outFile;
		Vector propertyNames = bean.getGCBean().getPropertyNames();
		
		try
		{
			//Validate path
			validPath = bean.getAbsoluteDirPath(path);
			if(validPath == null)				
				throw new Exception();
			path = validPath + File.separator;

			//Cycle through Properties
			for(int i=0;i<propertyNames.size();i++)
			{
				try
				{
					//set-up file
					newPath = path + (String)propertyNames.elementAt(i) +".txt";
					outFile = new PrintWriter(new FileWriter(newPath));
					outFile.println(writeProperty((String)propertyNames.elementAt(i), subResBean, evResBean, true));
					outFile.close();
				}
				catch(Exception e)
				{
					bean.errorMessage("\nThere was a problem trying to write Property Category totals.\n  Writing to file: "+ newPath);
				}
			}
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Property Category totals.\n  Writing to path: "+ path);
		}
	}	

	/**
	 *	Writes Selected Property results to file - all properties
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param SubstsResultBean subResBean - contains the all substs information
	 *	@param EvpthwyResultBean evResBean - contains the all evpthwy information
	 */
	public void writeCategoryTotalsFile(String fileName, SubstsResultBean subResBean, EvpthwyResultBean evResBean)
	{
		//Method Vars
		int[] freq = subResBean.getFreq();
		TableBean tBean = new TableBean();
		DecimalFormat form = new DecimalFormat("0.###");
		Vector propertyNames = bean.getGCBean().getPropertyNames();
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//header info
			if(bean.getHeader())
			{
				outFile.println("Total Observed Substitutions:	"+ subResBean.getNsynSubs().size());
				outFile.println("Total Evolutionary Pathways:	"+ evResBean.getTotalPathways());
				outFile.println("Mean Proportion:	"+ form.format((subResBean.getNsynSubs().size()/(double)evResBean.getTotalPathways())) +"\n");
			}		
			
			//Print Codon Tally
			if(bean.getCodonTally())
			{
				outFile.println("Codon Tally:	***	"+ freq[0]);
				for(int j=0;j<52;j++)
				{
					if(j != 0 && j%4 == 0 && j%16 != 0)
						j+=12;
					outFile.println(tBean.getCodonName(j) +"	"+ freq[j+1] +"	"+ tBean.getCodonName(j+4) +"	"+ freq[j+5] +"	"+ tBean.getCodonName(j+8) +"	"+ freq[j+9] +"	"+ tBean.getCodonName(j+12) +"	"+ freq[j+13]);
				}
				outFile.println();
			}
			
			//Cycle through Properties
			for(int i=0;i<propertyNames.size();i++)
				outFile.println(writeProperty((String)propertyNames.elementAt(i), subResBean, evResBean, false) +"\n");

			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Property Category totals.\n  Writing to file: "+ fileName);
		}
	}
	
	/**
	 *	Writes Selected Property results to file - all properties
	 *
	 *	@param SubstsResultBean subResBean - contains the all substs information
	 *	@param EvpthwyResultBean evResBean - contains the all evpthwy information
	 *	@param String name - name of property to write
	 *	@param boolean all - whether to ignore the bean print options or not
	 */
	private String writeProperty(String name, SubstsResultBean subResBean, EvpthwyResultBean evResBean, boolean all)
	{
		//Method Vars
		int[] catTotals;
		String outLine, table, entry;
		TableBean tBean = new TableBean();
		DecimalFormat form = new DecimalFormat("0.###");
		ProteinBean prop = (ProteinBean)subResBean.getSubGCBean().getProperties().get(name);
		
		//Header
		outLine = name +"\n";
		table = "";

		//significant vars
		if(all)
		{
			table += "	Total Observed Substitutions:	"+ subResBean.getNsynSubs().size() +"\n";
			table += "	Total Evolutionary Pathways:	"+ evResBean.getTotalPathways() +"\n";
			table += "	Mean Proportion:	"+ form.format((subResBean.getNsynSubs().size()/(double)evResBean.getTotalPathways())) +"\n";
		}
		
		//rest of header
		if(all || bean.getHeader())
		{
			//GF - Score of property
			table += "	Goodness-of-fit	" + form.format(evResBean.getGoodnessOfFit().get(name)) +"\n";
			
			//G - Score of property
			table += "	G-Score	" + form.format(evResBean.getGScores().get(name)) +"\n";
			
			//Z - Scores by Cat
			table += "	Z-Score";
			double[] zTotals = (double[])evResBean.getZScores().get(name);
			for(int j=0;j<bean.getNumberOfCat();j++)
				table += "	"+ form.format(zTotals[j]);	
		}
		//add table to outLine
		if(!table.equals(""))
			table += "\n\n";
		outLine += table;		
		

		//Observed Chart
		table = "";
		if(all || bean.getObservedChart())
		{
			table = "	Cn Chart (Observed Chart)\n";
			
			//go through codons - write table
			for(int j=0;j<64;j++)
			{
				entry = "	"+ tBean.getCodonName(j);	
				for(int k=0;k<bean.getNumberOfCat();k++)
					entry += "	"+ prop.getObservTable()[j][k];
				table += entry +"\n";
			}
		
			//Write totals of table
			entry = "	Total";
			catTotals = (int[])subResBean.getSubGCBean().getObservedCatTotals().get(name);
			for(int j=0;j<bean.getNumberOfCat();j++)
				entry += "	"+ catTotals[j];
			
			table += entry;
		}
		//add table to outLine
		if(!table.equals(""))
			table += "\n\n";
		outLine += table;
		

		//Pathway Chart
		table = "";
		if(all || bean.getPthwyChart())
		{
			table = "	C'n Chart (Pathway Chart)\n";
			
			//go through codons - write table
			for(int j=0;j<64;j++)
			{
				entry = "	"+ tBean.getCodonName(j);	
				for(int k=0;k<bean.getNumberOfCat();k++)
					entry += "	"+ prop.getCodonTable()[j][k];
				table += entry +"\n";
			}
			
			//Write totals of table
			entry = "	Total";
			catTotals = (int[])evResBean.getPathways().get(name);
			for(int j=0;j<bean.getNumberOfCat();j++)
				entry += "	"+ catTotals[j];
			
			table += entry;
		}
		//add table to outLine
		if(!table.equals(""))
			table += "\n\n";
		outLine += table;
		
		return outLine;
	}
	
	/**
	 *  Get method - writes Significant properties info to file by Z-Score
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector sigZScore - Contains EvpthwyBeans of all sig properties by Z-Scores
	 */
	public void writeSigPropByZ(String fileName, Vector sigZScore)
	{
		//Method vars
		int catNum;
		double value;
		EvpthwyBean evBean;
		String outLine = "";
		double[] z = bean.getZScore();
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
		
			//Header
			outLine = "Property	Category	Value";
			if(bean.getConf05())
				outLine += "	.05 ("+ z[0] +")";
			if(bean.getConf01())
				outLine += "	.01 ("+ z[1] +")";
			if(bean.getConf001())
				outLine += "	.001 ("+ z[2] +")";
			outFile.println(outLine);

			//Table
			for(int i=0;i<sigZScore.size();i++)
			{						
				evBean = (EvpthwyBean)sigZScore.elementAt(i);
				value = evBean.getValue();
				
				//Positive Selection
				if(value > 0)
					outFile.print(printSigPropZChart(evBean, bean.getPosCategories()));			
	
				//Negative Selection
				else
					outFile.print(printSigPropZChart(evBean, bean.getNegCategories()));
			}
			outFile.println();			
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Significant properties info to file by Z-Score.\n  Writing to file: "+ fileName);
		}

	}
	
	/**
	 *  Prints this given significant property into the proper columns based on catVect
	 *	@param Bean EvpthwyUsageBean is the Bean being used
	 *	@param Vector catVect - Contains the valid categories
	 */
	private String printSigPropZChart(EvpthwyBean evBean, Vector catVect)
	{
		//Method Vars
		String entry = "";
		int catNum = evBean.getCatNum() + 1;
		DecimalFormat form = new DecimalFormat("0.###");
	
		
		//Verify catNum is valid
		if(catVect.size() == 0 || catVect.contains(new Integer(catNum)))
		{			
			if(bean.getConf05() && evBean.getConf() >= 0)
			{
				if(entry.equals(""))
				{
					entry += evBean.getName() +"	"+ catNum;
					entry += "	"+ form.format(evBean.getValue());
				}
				entry += "	*";
			}
			if(bean.getConf01() && evBean.getConf() >= 1)	
			{
				if(entry.equals(""))
				{
					entry += evBean.getName() +"	"+ catNum;
					entry += "	"+ form.format(evBean.getValue());
				}
				entry += "	*";
			}
			if(bean.getConf001() && evBean.getConf() >= 2)	
			{
				if(entry.equals(""))
				{
					entry += evBean.getName() +"	"+ catNum;
					entry += "	"+ form.format(evBean.getValue());
				}
				entry += "	*";
			}
			if(!entry.equals(""))
				entry += "\n";
		}
		
		return entry;
	}

	/**
	 *  Get method - writes Significant properties info to file by GF-Score
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Hashtable sigGFScore - Contains EvpthwyBeans of all sig properties by GF-Scores
	 */
	public void writeSigPropByGF(String fileName, Hashtable sigGFScore)
	{
		//Method vars
		String[] values;
		String entry;
		double[] gf = bean.getGFScore();
		DecimalFormat form = new DecimalFormat("0.###");
		Vector propertyNames = bean.getGCBean().getPropertyNames();
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
		
			//Header
			entry = "Property	Value";
			if(bean.getConf05())
				entry += "	.05 ("+ gf[0] +")";
			if(bean.getConf01())
				entry += "	.01 ("+ gf[1] +")";
			if(bean.getConf001())
				entry += "	.001 ("+ gf[2] +")";
			outFile.println(entry);
			
			//go through all the properties	
			for(int i=0;i<propertyNames.size();i++)
			{
				entry = "";
				values = (String[])sigGFScore.get((String)propertyNames.elementAt(i));
					
				//.05 confidence level
				if(bean.getConf05() && values[0].equals("*"))
				{
					if(entry.equals(""))
					{
						entry += (String)propertyNames.elementAt(i);
						entry += "	"+ form.format(Double.valueOf(values[3]));
					}
					entry += "	*";
				}
				
				//.01 confidence level
				if(bean.getConf01() && values[1].equals("*"))
				{
					if(entry.equals(""))
					{
						entry += (String)propertyNames.elementAt(i);
						entry += "	"+ form.format(Double.valueOf(values[3]));
					}
					entry += "	*";
				}
				
				//.001 confidence level
				if(bean.getConf001() && values[2].equals("*"))
				{
					if(entry.equals(""))
					{
						entry += (String)propertyNames.elementAt(i);
						entry += "	"+ form.format(Double.valueOf(values[3]));
					}
					entry += "	*";
				}
				
				//Add a line
				if(!entry.equals(""))
					entry += "\n";
				
				outFile.print(entry);
			}
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Significant properties info to file by GF-Score.\n  Writing to file: "+ fileName);
		}
	}
	
	/**
	 *  Get method - writes Significant properties info to file by G-Score
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Hashtable sigGScore - Contains EvpthwyBeans of all sig properties by G-Scores
	 */
	public void writeSigPropByG(String fileName, Hashtable sigGScore)
	{
		//Method vars
		String[] values;
		String entry;
		double[] g = bean.getGFScore();
		DecimalFormat form = new DecimalFormat("0.###");
		Vector propertyNames = bean.getGCBean().getPropertyNames();
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//Header
			entry = "Property	Value";
			if(bean.getConf05())
				entry += "	.05 ("+ g[0] +")";
			if(bean.getConf01())
				entry += "	.01 ("+ g[1] +")";
			if(bean.getConf001())
				entry += "	.001 ("+ g[2] +")";
			outFile.println(entry);
			
			//go through all the properties	
			for(int i=0;i<propertyNames.size();i++)
			{
				entry = "";
				values = (String[])sigGScore.get((String)propertyNames.elementAt(i));
				
				//.05 confidence level
				if(bean.getConf05() && values[0].equals("*"))
				{
					if(entry.equals(""))
					{
						entry += (String)propertyNames.elementAt(i);
						entry += "	"+ form.format(Double.valueOf(values[3]));
					}
					entry += "	*";
				}
				
				//.01 confidence level
				if(bean.getConf01() && values[1].equals("*"))
				{
					if(entry.equals(""))
					{
						entry += (String)propertyNames.elementAt(i);
						entry += "	"+ form.format(Double.valueOf(values[3]));
					}
					entry += "	*";
				}
				
				//.001 confidence level
				if(bean.getConf001() && values[2].equals("*"))
				{
					if(entry.equals(""))
					{
						entry += (String)propertyNames.elementAt(i);
						entry += "	"+ form.format(Double.valueOf(values[3]));
					}
					entry += "	*";
				}
				
				//Add a line
				if(!entry.equals(""))
					entry += "\n";
				
				outFile.print(entry);
			}
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Significant properties info to file by G-Score.\n  Writing to file: "+ fileName);
		}
	}
	
	
	/**
	 *	Writes Significant properties listed by substitution to file specified
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector nsynSubs - Contains all nonsynonymous substitutions
	 */
	public void writeSigPropBySubs(String fileName, Vector nsynSubs)
	{
		//Method vars
		int entryNum;
		double value;
		Vector sigCat;
		SubstsBean subBean;
		EvpthwyBean evBean;
		String entries, entry = "";
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//header
			outFile.println("Codon#	Branch	Amount	Properties...");
					
			//go through all subs
			for(int i=0;i<nsynSubs.size();i++)
			{
				entries = "";			
				entryNum = 0;
				subBean = (SubstsBean)nsynSubs.elementAt(i);
				sigCat = subBean.getSigCat();
				
				//go through sigCat
				for(int j=0;j<sigCat.size();j++)
				{
					evBean = (EvpthwyBean)sigCat.elementAt(j);
					value = evBean.getValue();
	
					//Positive Selection
					if(value > 0)
						entry = printSigPropBySubsChart(evBean, bean.getPosCategories());			
					
					//Negative Selection
					else
						entry = printSigPropBySubsChart(evBean, bean.getNegCategories());
					
					//add entry to table 
					if(!entry.equals(""))
					{
						entryNum++;
						
						if(entries.equals(""))
							entries += entry;
						else
							entries += "	"+ entry;
					}
				}
				//add the identification
				if(!entries.equals(""))
				{
					entries = subBean.getCodon() +"	"+ subBean.getBranch() +"	"+ entryNum +"	"+ entries;
				
					//add entries to outLine
					outFile.println(entries);
				}
			}	
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Significant Properties listed by Substitution.\n  Writing to file: "+ fileName);
		}
	}	
	
	/**
	 *  Prints this given significant property values into the proper columns of substitutions based on catVect
	 *
	 *	@param Bean EvpthwyUsageBean is the evpthwy Bean being used
	 *	@param Vector catVect - Contains the valid categories
	 */
	private String printSigPropBySubsChart(EvpthwyBean evBean, Vector catVect)
	{
		//Method Vars
		String entry = "";
		int catNum = evBean.getCatNum() + 1;
		
		//Verify catNum is valid
		if(catVect.size() == 0 || catVect.contains(new Integer(catNum)))
		{
			if(bean.getConf05() && evBean.getConf() == 0)
			{					
				entry += "("+ catNum +", .05) "+ evBean.getName();
			}
			else if(bean.getConf01() && evBean.getConf() == 1)
			{					
				entry += "("+ catNum +", .01) "+ evBean.getName();
			}
			else if(bean.getConf001() && evBean.getConf() == 2)
			{					
				entry += "("+ catNum +", .001) "+ evBean.getName();
			}
		}
		
		return entry;
	}
	
	/**
	 *  Get method - writes Significant codon info to file
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector nsynSubs - Contains all nonsynonymous substitutions
	 */
	public void writeSigCodons(String fileName, Vector nsynSubs)
	{
		//Method vars
		int entryNum;
		double value;
		Integer subs, amount;
		SubstsBean subBean;
		EvpthwyBean evBean;
		Vector sigCat;
		Vector codons = new Vector();
		Hashtable sigC = new Hashtable();
		String entry;
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
		
			//Header
			outFile.println("Codon#	#Property");
			
			//go thru all the substitutions
			for(int i=0;i<nsynSubs.size();i++)
			{		
				entryNum = 0;
				subBean = (SubstsBean)nsynSubs.elementAt(i);
				sigCat = subBean.getSigCat();
				
				//go through sigCat
				for(int j=0;j<sigCat.size();j++)
				{
					evBean = (EvpthwyBean)sigCat.elementAt(j);
					value = evBean.getValue();
					
					//Positive Selection
					if(value > 0)
						entry = printSigPropBySubsChart(evBean, bean.getPosCategories());			
					
					//Negative Selection
					else
						entry = printSigPropBySubsChart(evBean, bean.getNegCategories());
					
					//add entry to table 
					if(!entry.equals(""))
						entryNum++;
				}
				
				//Add in the number of significant properties for this substitution, if any
				if(entryNum != 0)
				{
					subs = new Integer(subBean.getCodon());
					amount = new Integer(entryNum);
					
					//Not yet seen, add to vector and hashtable
					if(!codons.contains(subs))
					{
						codons.add(subs);
						sigC.put(subs, amount);
					}
					
					//Already placed, then add amount to value in hashtable
					else
					{
						entryNum = ((Integer)sigC.get(subs)).intValue() + entryNum;
						sigC.put(subs, new Integer(entryNum));
					}
				}
			}
			
			//go through codon vector and print out codons
			for(int i=0;i<codons.size();i++)
				outFile.println((Integer)codons.elementAt(i) +"	"+ (Integer)sigC.get((Integer)codons.elementAt(i)));
		
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Significant Codon totals.\n  Writing to file: "+ fileName);
		}

	}

	/**
	 *	Writes Selected Property results to file - one file for each property
	 *
	 *	@param String fileName - validated file path, where information is printed
	 *	@param String propertyName - property to be printed to specified file
	 *	@param Vector evResults - Vector of evResBeans - containing the all evpthwy information
	 */
	public void writeSlidingCatTotals(String fileName, String propertyName, Vector evResults)
	{
		//Method vars
		String windowID;
		double[] zTotals;
		EvpthwyResultBean evResBean;
		DecimalFormat form = new DecimalFormat("0.###");
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//Header
			outFile.println("Windows for "+ propertyName +"		Z-Scores By Category");
			outFile.print("From	To");
			for(int j=0;j<bean.getNumberOfCat();j++)
				outFile.print("	"+ (j+1));	
			outFile.println();
			
			//iterate through evpthwy results
			for(int i=2;i<evResults.size();i++)
			{
				evResBean = (EvpthwyResultBean)evResults.elementAt(i);
				
				//write id to file
				windowID = evResBean.getWindowID();
				outFile.print(windowID.substring(0,windowID.indexOf(" ")) +"	"+ windowID.substring(windowID.indexOf(" ")+1));
				
				//write z-scores to file
				zTotals = (double[])evResBean.getZScores().get(propertyName);
				for(int j=0;j<bean.getNumberOfCat();j++)
					outFile.print("	"+ form.format(zTotals[j]));	
				
				outFile.println();
			}

			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Sliding Window Category Totals.\n  Writing to file: "+ fileName);
		}
	}
	
	/**
	 *	Writes Selected Property results to file - one file for each property
	 *
	 *	@param String fileName - validated file path, where information is printed
	 *	@param String propertyName - property to be printed to specified file
	 *	@param Vector evResults - Vector of evResBeans - containing the all evpthwy information
	 */
	public void writeAllSlidingCatTotalsToExcel(String fileName, Vector evResults)
	{
		Vector propertyNames = bean.getGCBean().getPropertyNames();
		//DecimalFormat form = new DecimalFormat("0.###");	
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			WritableWorkbook excelFile = Workbook.createWorkbook(new File(fileName));
			WritableCellFormat sigCatFormat = new WritableCellFormat();
			sigCatFormat.setBackground(jxl.format.Colour.IVORY);

			for(int i=0;i<propertyNames.size();i++)
			{	
				String propertyName = (String)propertyNames.elementAt(i);
				WritableSheet sheet = excelFile.createSheet(propertyName.substring(0, propertyName.length() > 31 ? 31 : propertyName.length()), i);
				writeAttributeSlidingCatTotalsToExcel(sheet, propertyName, evResults, sigCatFormat);
			}
			excelFile.write();
			excelFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Sliding Window Category Totals.\n  Writing to file: "+ fileName);
		}
	}
	
	public void writeAttributeSlidingCatTotalsToExcel (WritableSheet sheet, String propertyName, Vector evResults, WritableCellFormat sigCatFormat)
		throws WriteException
	{
		//Method vars
		EvpthwyResultBean evResBean;
		String windowID;
		double[] zTotals;
		
		int row = 0;
		//Header
		WritableCell header = new Label(0, row, "Windows for "+ propertyName );
		sheet.addCell(header);
		header = new Label(1,row++, "Z-Scores By Category");
		sheet.addCell(header);
		
		// Column Header Rows
		WritableCell colHeader = new Label(0, row, "From");
		sheet.addCell(colHeader);
		colHeader = new Label(1, row, "To");
		sheet.addCell(colHeader);
		for(int j=0;j<bean.getNumberOfCat();j++)
		{
			colHeader = new Label(j+2, row, " "+(j+1));
			sheet.addCell(colHeader);
		}
		row++;
		//iterate through evpthwy results
		for(int i=2;i<evResults.size();i++)
		{
			evResBean = (EvpthwyResultBean)evResults.elementAt(i);
			
			//write id to file
			windowID = evResBean.getWindowID();
			WritableCell window = new Label(0, row, " " + windowID.substring(0,windowID.indexOf(" ")));
			sheet.addCell(window);
			window = new Label(1, row, windowID.substring(windowID.indexOf(" ")+1));
			sheet.addCell(window);
			//write z-scores to file
			zTotals = (double[])evResBean.getZScores().get(propertyName);
			for(int j=0;j<bean.getNumberOfCat();j++)
			{
				double currVal = zTotals[j];
				WritableCell currCell = new jxl.write.Number(j+2, row, currVal); 
				if (currVal >= 3.09)
				{
					currCell.setCellFormat(sigCatFormat);
				}

				sheet.addCell(currCell);
			}
			row++;
		}
	}
	
	public void printGraphsForAttributes (String fileName, Vector evResults, String propertyName)
	{
		String pictureName = propertyName + ".png";
	}
}//EvpthwyPrint