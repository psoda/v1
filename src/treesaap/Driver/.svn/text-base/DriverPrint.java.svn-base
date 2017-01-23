/**
 *	DriverPrint.class is responsible for the printing to 
 * 	file of all data for the program
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 *
 *	jExcel functionality added by Tim Adair
 */
package treesaap.Driver;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Vector;

import treesaap.CDM.CDMPrint;
import treesaap.CDM.CDMResultBean;
import treesaap.Substs.SubstsPrint;
import treesaap.Substs.SubstsResultBean;
import treesaap.Tree.TreePrint;
import treesaap.Evpthwy.EvpthwyPrint;
import treesaap.Evpthwy.EvpthwyResultBean;
import treesaap.GeneralDNAFileParser.TreeBean;
import treesaap.GeneralDNAFileParser.DNAFileParserPrint;

public class DriverPrint
{
	//CLASS Vars
	private Driver driver;						//The control object contianing all classes
	private DriverUsageBean bean;				//Bean containing all values necessary for operation
	private DriverPrintUsageBean printBean;		//Bean containing all values of printing options
	
	/**
	 * CDMPrint Constructor
	 *
	 * @param Driver newDriver - reference to the main class
	 * @param Bean DriverUsageBean usage bean is set to that passed in
	 * @param PrintBean DriverPrintUsageBean usage bean is set to that passed in
	 */
	public DriverPrint(Driver newDriver, DriverUsageBean Bean, DriverPrintUsageBean PrintBean)
	{
		bean = Bean;
		driver = newDriver;
		printBean = PrintBean;

	}//Constructor 

	/**
	 * Prints all selected files
	 *
	 *	@param TreeBean tree - the Tree being ran
	 *	@param Vector mainResults - the results of a run
	 *	@param Vector secResults - the secondary results of a run - correlate
	 */
	public void printAll(TreeBean tree, Vector mainResults, Vector secResults)
	{				
		//Print the tree File
		if(printBean.getWriteTree())
			writeTree(bean.getOutputDirectory() + tree.getName() +".txt", tree);
		
		//Print TCS tree file
		if(driver.getTree().getBean().getDisplayTCSWindow() || printBean.getWriteTCSTree())
			writeTCSTree(bean.getOutputDirectory() +"tree.txt", tree);
		
		//Data
		writeData();
		
		//Print Substs
		if(mainResults.size() >= 1)
		{
			//Method Vars
			SubstsResultBean subResults = (SubstsResultBean)mainResults.elementAt(0);
			
			//Print Substs
			SubstsPrint subPrint = new SubstsPrint(driver.getSubsts().getBean());
			if(printBean.getWriteNonsynonymous())
				subPrint.writeNsynSubs(bean.getOutputDirectory() + "Substs/NSynSubs.txt", subResults.getNsynSubs());
			if(printBean.getWriteSynonymous())
				subPrint.writeSynSubs(bean.getOutputDirectory() + "Substs/SynSubs.txt", subResults.getSynSubs());
			if(printBean.getWriteAmbiguous())
				subPrint.writeAmbig(bean.getOutputDirectory() + "Substs/Ambig.txt", subResults.getAmbig());
			if(printBean.getWriteStop())
				subPrint.writeStop(bean.getOutputDirectory() + "Substs/Stop.txt", subResults.getStop());
		}

		//Verify Results were obtained
		if(mainResults.size() >= 2)
		{
			//Method Vars
			SubstsResultBean subResults = (SubstsResultBean)mainResults.elementAt(0);
			EvpthwyResultBean evResults = (EvpthwyResultBean)mainResults.elementAt(1);
			
			//Print Evpthwy
			EvpthwyPrint evPrint = new EvpthwyPrint(driver.getEvpthwy().getBean());
			if(printBean.getCategoryTotalsFile())
				evPrint.writeCategoryTotalsFile(bean.getOutputDirectory() + "Evpthwy/CatTotals.txt", subResults, evResults);
			if(printBean.getCategoryTotalsFolder())
				evPrint.writeCategoryTotalsFolder(bean.getOutputDirectory() + "Evpthwy/Totals", subResults, evResults);
			if(printBean.getSignificantBySubs())
				evPrint.writeSigPropBySubs(bean.getOutputDirectory() + "Evpthwy/Subs-SigProp.txt", subResults.getNsynSubs());
			if(printBean.getSignificantByZScore())
				evPrint.writeSigPropByZ(bean.getOutputDirectory() + "Evpthwy/Z-SigProp.txt", evResults.getSigZScore());
			if(printBean.getSignificantByGScore())
				evPrint.writeSigPropByG(bean.getOutputDirectory() + "Evpthwy/G-SigProp.txt", evResults.getSigGScore());
			if(printBean.getSignificantByGFScore())
				evPrint.writeSigPropByGF(bean.getOutputDirectory() + "Evpthwy/GF-SigProp.txt", evResults.getSigGFScore());
			if(printBean.getSignificantByCodon())
				evPrint.writeSigCodons(bean.getOutputDirectory() + "Evpthwy/SigCodons.txt", subResults.getNsynSubs());

			if(printBean.getSlidingCatTotals() && mainResults.size() > 2)
			{
				//Sliding Window Evpthwy
				Vector propNames;
				
				//get properties
				if(bean.getProperties().equals("all"))
					propNames = driver.getData().getPropertyNames();
				else
					propNames = (Vector)driver.getData().getPropertyLists().get(bean.getProperties());
				
				//Cycle through them and print each one off

				for(int i=0;i<propNames.size();i++)
				{
					evPrint.writeSlidingCatTotals(bean.getOutputDirectory() + "Evpthwy/SlidingWindow/"+propNames.elementAt(i)+".txt",(String)propNames.elementAt(i), mainResults);
				}
				String excelFileName = bean.getOutputDirectory() + "Evpthwy/SlidingWindow/AllAttributes.xls";
				evPrint.writeAllSlidingCatTotalsToExcel(excelFileName, mainResults);
			}
		}
	
		//Print CDM	
		if(secResults.size() >= 2)
		{
			CDMPrint cdmPrint = new CDMPrint(driver.getCDM().getBean());
			if(printBean.getCDMResults())
				cdmPrint.writeResults(bean.getOutputDirectory() + "CDM/Results.txt", secResults);
			if(printBean.getSlidingWindowTotal() && secResults.size() > 2)
				cdmPrint.writeSlidingWindowResultFile(bean.getOutputDirectory() + "CDM/SlidingWindowResults.xls", secResults);
			if(printBean.getSlidingWindowFolder() && secResults.size() > 2)
				cdmPrint.writeSlidingWindowResults(bean.getOutputDirectory() + "CDM/SlidingWindow/", secResults);
		}

	}//printAll	

	/**
	 * Sets the directory to print files to
	 * @param String treeName - the name of the tree
	 */
	public void setDirectory(String treeName)
	{		
		//get the create tree folder option
		if(bean.getCreateTreeFolder())
		{
			//build string
			String folderName = bean.getResultsDirectory() + treeName +"_";
			
			//add on genetic Code
			if(bean.getGeneticCode() != null && bean.getGeneticCode().length() >=2)
				folderName += bean.getGeneticCode().substring(0,2);
			
			//add on Category Number
			if(driver.getEvpthwy() != null && driver.getEvpthwy().getBean() != null)
				folderName += driver.getEvpthwy().getBean().getNumberOfCat()+"";

			//Check to see if folderName exists
			if(!(new File(folderName)).exists())
				bean.setOutputDirectory(folderName);
	
			//Create a unique folder
			else
			{
				for(int i=1;true;i++)
					if(!(new File(folderName +"_"+ i)).exists())
					{
						bean.setOutputDirectory(folderName +"_"+ i);
						break;
					}
			}
			
		}
		
	}//setDirectory
	
	/**
	 *	Writes Tree Info to file
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param TreeBean tree - the Tree being ran
	 */
	public void writeTree(String fileName, TreeBean tree)
	{	
		try
		{
			//Method Vars
			DNAFileParserPrint parserPrint = new DNAFileParserPrint(driver.getGDFP().getBean());
			
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//header
			if(printBean.getTreeHeader())
				outFile.println(parserPrint.writeTreeHeader(tree));
			
			//ancestralTree
			if(printBean.getAncestralTree())
				outFile.println(parserPrint.writeTreeAncestralTree(tree));
			
			//taxa
			if(printBean.getTreeTaxa())
				outFile.println(parserPrint.writeTreeTaxa(tree, driver.getGDFP().getTaxa()));
			
			//nodes
			if(printBean.getTreeNodes())
				outFile.println(parserPrint.writeTreeNodes(tree));

			//close file
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Tree Information.\n  Writing to file: "+ fileName);
		}
	}	
	
	/**
	 *	Writes Tree Info to file
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param TreeBean tree - the Tree being ran
	 */
	public void writeTCSTree(String fileName, TreeBean tree)
	{			
		//Method Vars
		TreePrint treePrint = new TreePrint(driver.getTree().getBean());
		treePrint.writeTCSTree(fileName, tree, driver.getGDFP().getTaxa());
	}
	
	/**
	 *	Writes Tree Info to file
	 */
	public void writeData()
	{	
		//Method Vars
		PrintWriter outFile;
		
		try
		{
			//Print Genetic Codes
			if(printBean.getWriteGeneticCodes())
			{
				outFile = getWriter(bean.getOutputDirectory() + "Data/GeneticCodes.txt");		
				outFile.println(driver.getData().writeGeneticCodes());
				outFile.close();
			}
			//Print Properties
			if(printBean.getWriteProperties())
			{
				outFile = getWriter(bean.getOutputDirectory() + "Data/Properties.txt");	
				if(bean.getProperties().equals("all"))
					outFile.println(driver.getData().writeProperties());
				else
					outFile.println(driver.getData().writePropertiesByList(bean.getProperties()));
				outFile.close();
			}
			//Print Genetic Codes
			if(printBean.getWriteZScores())
			{
				outFile = getWriter(bean.getOutputDirectory() + "Data/ZScores.txt");		
				outFile.println(driver.getData().writeZScores());
				outFile.close();
			}
			//Print Genetic Codes
			if(printBean.getWriteGFScores())
			{
				outFile = getWriter(bean.getOutputDirectory() + "Data/GFScores.txt");		
				outFile.println(driver.getData().writeGFScores());
				outFile.close();
			}
			
			//Write Pathways
			if(printBean.getWritePathways())
				driver.getData().writePathways(bean.getOutputDirectory() + "Data/Pathways.txt");

		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Data Information to file.\n  Writing to directory: "+ bean.getOutputDirectory());
		}
	}	
	
	/**
	 *	Writes Data Info to file
	 *	@param String fileName - validated fileName, where information is printed
	 */
	public PrintWriter getWriter(String fileName)
	{	
		try
		{			
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			return new PrintWriter(new FileWriter(fileName));
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to create PrintWriter.\n  Writing to file: "+ fileName);
			try{
				return new PrintWriter(new FileWriter(bean.getOutputDirectory()+"dump.txt"));
			}catch(Exception ex){return null;}
		}
	}	
	
	
	/**
	 *  Get method - returns DriverPrintUsageBean printBean
	 */
	public DriverPrintUsageBean getPrintBean()
	{
		return printBean;
	}

}//DriverPrint