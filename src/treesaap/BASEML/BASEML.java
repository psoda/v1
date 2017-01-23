/**
 *	BASEML.class prepares to run baseml. 
 *	It prints taxa and tree data to file
 *	that are necessary for running baseml.
 *	It will then execute baseml.
 *
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.BASEML;

import java.awt.Frame;
import java.util.Vector;
import java.util.Hashtable;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
	
public class BASEML
{		
	//CLASS Vars
	private ExecBaseml eb;
	private BASEMLUsageBean bean;

	
	/**
	 * BASEMLPrep Constructor
	 * creates BASEMLBean object and ExecBaseml object
	 */
	public BASEML()
	{
		bean = new BASEMLUsageBean();
		eb = new ExecBaseml(bean);
		
	}//Constructor	
	
	
	/**
	 * BASEMLPrep Constructor
	 * creates BASEMLBean object and ExecBaseml object
	 *
	 * @param Bean BASEMLUsageBean usage bean is set to that passed in
	 */
	public BASEML(BASEMLUsageBean Bean)
	{
		bean = Bean;
		eb = new ExecBaseml(bean);
		
	}//Constructor 

	/**
	 * Sets up and Runs BASEML
	 *
	 * @param thisTree BASEMLTreeBean the Tree that is being ran through baseml
	 */
	public void run(BASEMLTreeBean thisTree)
	{
		run(null, thisTree);
	}
	
	/**
	 * Sets up and Runs BASEML
	 *
	 * @param outWindow Frame the output window if a GUI is present
	 * @param thisTree BASEMLTreeBean the Tree that is being ran through baseml
	 */
	public void run(Frame outWindow, BASEMLTreeBean thisTree)
	{
		//Method var
		File file;
		
		//Check to see if tree has already been ran
		if(thisTree.getRan())
			return;
		
		//Write all data and settings to file
		if(!writeSettings() || !writeData(thisTree))
			return;
		
		//run baseml - switch OS
		switch(bean.getOS())
		{
			//Windows
		case 0:
			file = new File(bean.getBasemlPath() +"baseml.exe");
			if(!file.exists())
			{
				bean.errorMessage("\nSpecified BASEML directory does not contain baseml.exe.\n  Please move file to that location or change BASEML path");
				return;
			}
			eb.exec(outWindow, bean.getBasemlPath() +"baseml.exe basemlSettings.ctl");
			break;
			
			//MAC
		case 1:
			file = new File("./treesaap/BASEML/baseml.ex");
			if(!file.exists())
			{
				bean.errorMessage("\nSpecified BASEML directory does not contain baseml.ex.\n  Please move file to that location or change BASEML path");
				return;
			}
			eb.exec(outWindow,"./treesaap/BASEML/baseml.ex basemlSettings.ctl");			
			break;
				
			//Linux
		case 2:
			file = new File(bean.getBasemlPath() +"baseml");
			if(!file.exists())
			{
				bean.errorMessage("\nSpecified BASEML directory does not contain baseml\n  Please move file to that location or change BASEML path");
				return;
			}
			eb.exec(outWindow, bean.getBasemlPath() +"baseml basemlSettings.ctl");
			break;
		}
		
		//move BASEML temp files to directory specified for output files
		move("basemlSettings.ctl", bean.getOutFilePath());
		move("basemlData.txt", bean.getOutFilePath());
		move("basemlOutfile.txt", bean.getOutFilePath());
		move("mytrees.trees", bean.getOutFilePath());
		move("2base.t", bean.getOutFilePath());
		move("lnf", bean.getOutFilePath());
		move("rst1", bean.getOutFilePath());
		move("rub", bean.getOutFilePath());
		
	}//run
	
	/**
	 * Writes the file basemlSettings.ctl
	 * This is the data to be used for baseml.  Returns the success of writing settings.
	 */
	private boolean writeSettings()
	{
		try
		{
			//Open up a Print Writer of user_settings file
			PrintWriter of = new PrintWriter(new FileWriter("basemlSettings.ctl",false));
			
			//print to file user selected settings
			of.println("seqfile = basemlData.txt");
			of.println("outfile = basemlOutfile.txt");
			of.println("treefile = mytrees.trees");
			of.println("noisy = 3");
			of.println("verbose = 0");
			of.println("runmode = 0");
			of.println("model = "+ bean.getModel());
			of.println("Mgene = 0");
			of.print("fix_kappa = ");
			
			//if kappa is true, print 1, else 0
			if(bean.getFixKappa())	
				of.println("1");
			else
				of.println("0");
			
			of.println("kappa = "+ bean.getKappa());
			of.print("fix_alpha = ");
			
			//if alpha is true, print 1, else 0
			if(bean.getFixAlpha())	
				of.println("1");
			else
				of.println("0");
			
			of.println("alpha = "+ bean.getAlpha());
			of.println("Malpha = 0");
			of.println("ncatG = 5");
			of.println("fix_rho = 1");
			of.println("rho = 0");
			of.println("nparK = 0");
			of.println("clock = 0");
			of.println("nhomo = 0");
			of.println("getSE = 1");
			of.println("RateAncestor = 1");
			of.println("Small_Diff = 9e-6");
			of.println("cleandata = 0");
			of.println("ndata = 1");
			of.println("method = 0");
			
			//Close File
			of.close();
		}
		catch(Exception exception) 
		{
			bean.errorMessage("\nYou had an Exception while writing data to basemlSettings.ctl.");
			return false;
		}
		
		return true;
	}
	
	
	/**
	 * Writes the files basemlData.txt and mytrees.trees
	 * This is the data to be used for baseml.  Returns the success of writing a valid tree.
	 *
	 * @param thisTree BASEMLTreeBean tree containing all data to be written to file
	 */
	private boolean writeData(BASEMLTreeBean thisTree)
	{
		//Method vars
		Vector taxaNames = thisTree.getTaxaNames();
		Hashtable allTaxa = thisTree.getTaxa();
		
		//Error check for an invalid tree
		if(taxaNames.size() == 0)
			return false;
		
		//Write info to file
		try
		{
			PrintWriter of = new PrintWriter(new FileWriter ("basemlData.txt",false));
			of.println("#NEXUS");
			of.println("[Sequence file output from Substs]");
			of.println("Begin data;");
			of.print("Dimensions ntax=" + taxaNames.size());
			of.println(" nchar=" + ((String)allTaxa.get((String)taxaNames.elementAt(0))).length() + ";");
			of.println("Matrix");
			
			//iterate through all the taxa and print them to file
			for (int i=0;i<taxaNames.size();i++)
			{
				of.print(taxaNames.elementAt(i) + "\t     ");
				of.println(allTaxa.get((String)taxaNames.elementAt(i)));
			} 
			of.println(";");
			of.println("End;");
			
			//print trees to this file
			of.println("begin trees;");
			of.print("tree " + thisTree.getName() + " = ");
			
			//print rooted or not
			if(thisTree.getRooted())
				of.print("[&r] ");
			else
				of.print("[&u] ");
			
			//lastly print relation
			of.println(thisTree.getRelation() + ";");
			of.println("End;");
			of.close();
			
			// for each tree in the file, save it to a treefile for baseml to use;
			of = new PrintWriter(new FileWriter ("mytrees.trees",false));
			of.println(thisTree.getRelation() + "\n");
			of.close();
		}
		catch(Exception exception) 
		{
			bean.errorMessage("\nYou had an Exception while writing data to files for baseml.");
			return false;
		}
		
		return true;
	}//writeData

	/**
	 * Copy method, moves rst from root directory to the one specified
	 * Path must be validated to the Platform.  Directory structure will be created if
	 * not existant.  Also, file will remain named rst, only specify the path.
	 *
	 * @param path String copy rst file to specified path
	 */
	public boolean moveRST(String path)
	{
		return move("treesaap" + File.separator + "rst", path);
	}

	/**
	 * Copy method, moves specified file from root directory to the one specified.
	 * Path must be validated to the Platform.  Directory structure will be created if
	 * not existant.  Also, file will remain with the same name, only specify the path.
	 *
	 * @param fileName String name of file to be moved
	 * @param path String copy file to specified path
	 */
	private boolean move(String fileName, String path)
	{
		try
		{	
			//get validated path and file
			path = bean.getAbsoluteDirPath(path);
			fileName = bean.getAbsoluteFilePath(fileName, false);

			//error check
			if(path == null || fileName == null)
				return false;

			//File object of path and file
			File file = new File(fileName);
			File newFile = new File(path + File.separator + file.getName());
			
			//Overwrite the file
			if(newFile.exists())
				newFile.delete();

			//make move
			return file.renameTo(newFile);
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere were problems moving the "+ fileName +" file.  Please do so on your own.\n");
			return false;
		}
	}
	
	/**
	 * Get method - returns BASEMLUsageBean bean
	 */
	public BASEMLUsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Set method - sets bean here and lower classes
	 *	@param Bean BASEMLUsageBean is the new bean 
	 */
	public void setBean(BASEMLUsageBean Bean)
	{
		bean = Bean;
		eb.setBean(bean);
	}//getBean 
	
}//BASEML