/**
 *	TreeSAAP.class is the Controller.  TreeSAAP.class directs usage of requested classes from the
 *	user (or the View classes).  It also handles control of information between Buisness Logic Classes
 *	and Data Classes.
 *
 *	@author	Joshua Sailsbery
 *	@version	3.2
 *	@since	1.0
 */
package treesaap.Driver;

import java.awt.Toolkit;
import java.util.Date;
import java.util.Vector;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.IOException;
import java.text.DateFormat;

import treesaap.BASEML.BASEML;
import treesaap.BASEML.BASEMLStub;
import treesaap.BASEML.BASEMLUsageBean;
import treesaap.BASEML.BASEMLTreeBean;
import treesaap.Block.Block;
import treesaap.Block.BlockStub;
import treesaap.Block.BlockUsageBean;
import treesaap.CDM.CDM;
import treesaap.CDM.CDMStub;
import treesaap.CDM.CDMUsageBean;
import treesaap.CommandLine.CommandLine;
import treesaap.CommandLine.CommandLineStub;
import treesaap.CommandLine.CommandLineUsageBean;
import treesaap.Data.Data;
import treesaap.Data.DataStub;
import treesaap.Data.DataUsageBean;
import treesaap.Data.GeneticCodeBean;
import treesaap.Evpthwy.Evpthwy;
import treesaap.Evpthwy.EvpthwyStub;
import treesaap.Evpthwy.EvpthwyUsageBean;
import treesaap.GeneralDNAFileParser.TreeBean;
import treesaap.GeneralDNAFileParser.TaxaBean;
import treesaap.GeneralDNAFileParser.DNAFileParserStub;
import treesaap.GeneralDNAFileParser.GeneralDNAFileParser;
import treesaap.GeneralDNAFileParser.DNAFileParserUsageBean;
import treesaap.GraphicInterface.GraphicInterface;
import treesaap.GraphicInterface.GraphicInterfaceStub;
import treesaap.GraphicInterface.GraphicInterfaceUsageBean;
import treesaap.Objects.Stub;
import treesaap.Objects.UsageBean;
import treesaap.Run.Run;
import treesaap.Run.RunStub;
import treesaap.Run.RunUsageBean;
import treesaap.Substs.Substs;
import treesaap.Substs.SubstsStub;
import treesaap.Substs.SubstsUsageBean;
import treesaap.Tree.Tree;
import treesaap.Tree.TreeStub;
import treesaap.Tree.TreeUsageBean;

public class Driver
{
	//GLOBAL Variables
	public static String title = "TreeSAAP 3.2";		//Allows version iteration to be viewed across entire program
	private static Driver driver;						//Allows indirect instantiation of this class

	//CLASS Variables
	private DriverStub stub;				//Creates the Usage Bean from user-input settings
	private DriverUsageBean bean;			//This object contains all opeational settings needed for program
	private DriverPrint print;				//Controls the printing of files
	private DriverPrintStub printStub;		//Creates and maintains a printBean for the DriverPrint class
	
	//Control Objects - contain business logic
	private BASEML baseml;					//Object used for accessing BASEML methods
	private Block block;					//Object used for accessing Block methods
	private CDM cdm;						//Object used for accessing CDM methods
	private CommandLine commandLine;		//Object used for accessing CommandLine methods
	private Data data;						//Object used for accessing Data methods
	private Evpthwy evpthwy;				//Object used for accessing Evpthwy methods
	private GeneralDNAFileParser gdfp;		//Object used for accessing GeneralDNAFileParser methods
	private GraphicInterface gui;			//Object used for accessing GraphicInterface methods
	private Run run;						//Object used for accessing Run methods	
	private Substs substs;					//Object used for accessing Substs methods
	private Tree tcsTree;					//Object used for accessing Tree methods
	
	//Contains the Stubs and Beans of each control object
	private Vector Stubs;					//Contains the Stubs of objects
	private Vector Beans;					//Contains the Beans of objects
	
	//Process Vars
	private boolean firstTry = true;		//Whether this is the first initialization of driver class or not
	
	
	/**
	 *	Main.  This method instantiates TreeSAAP and maintains its integrity.
	 *
	 *	@param String[]	When the program is ran, a single command line argument may
	 *					be passed into main for an automatic function call.
	 *					Also, a -c flag may be passed in to cause the program to 
	 *					function in command line mode.		
	 */
	public static void main(String[] args) throws InterruptedException
	{	
		try
		{	
			//check the parameters
			boolean commandLine = getCommandLine(args);
			boolean exit = getExit(args);
			String logFile = getLogFile(args);
			String fileName = getFile(args);

			//create driver
			driver = new Driver((!commandLine && !exit), logFile);
			driver.initialize();
			
			//open file if given
			if(fileName != null && !fileName.equals(""))
				autoFile(fileName);
			
			//exit program
			if(exit)
				System.exit(0);
		}
		catch(IOException e)
		{
			System.out.println("There were problems starting the program.\n  Please verify settings files are located in the correct location.");
		}
		catch(OutOfMemoryError e)
		{
			driver.getBean().errorMessage("\nThe file is too big.\n     Increase Memory allocation or contact David_McClellan@byu.edu");
		}
	}//Main
	
	/**
	 * Constructor, creates an object to be referenced and utilized.
	 * It then calls methods to populate that object.
	 *
	 * @param boolean gui - whether a gui is being used
	 * @param String log - the log file outputted to
	 */
	public Driver(boolean gui, String log) throws IOException
	{
		//Create the stub
		stub = new DriverStub("./treesaap/Settings/driver_settings.txt", gui, log);
		
		//get the new usage bean
		bean = (DriverUsageBean)stub.getBean();
		
		//Print vars
		printStub = new DriverPrintStub("./treesaap/Settings/print_settings.txt", gui, bean.getLogFile());
		
	}//Constructor
		
	/**
	 *	TreeSAAP Initialize - Instantiates Class Objects.
	 *	
	 *	@param String	If passed a valid file path, calls openNew()
	 */
	public void initialize()
	{	
		//try/catch for ioexceptions
		try
		{
			//Print Vars
			print = new DriverPrint(driver, bean, (DriverPrintUsageBean)printStub.getBean());
			
			//Create all the Stubs
			Stubs = new Vector();
			Stubs.add(new BASEMLStub("./treesaap/Settings/baseml_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new BlockStub("./treesaap/Settings/block_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new CDMStub("./treesaap/Settings/cdm_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new CommandLineStub("./treesaap/Settings/commandLine_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new DataStub("./treesaap/Settings/data_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new EvpthwyStub("./treesaap/Settings/evpthwy_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new DNAFileParserStub("./treesaap/Settings/gdfp_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new GraphicInterfaceStub("./treesaap/Settings/gui_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new RunStub("./treesaap/Settings/run_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new SubstsStub("./treesaap/Settings/substs_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(new TreeStub("./treesaap/Settings/tree_settings.txt", bean.getErrorWindow(), bean.getLogFile()));
			Stubs.add(stub);
			Stubs.add(printStub);
			
			//Get all the beans
			Beans = new Vector();
			for(int i=0;i<Stubs.size();i++)
				Beans.add(((Stub)Stubs.elementAt(i)).getBean());
			
			//Get Beans accessed here
			EvpthwyUsageBean evBean = (EvpthwyUsageBean)Beans.elementAt(5);
			DNAFileParserUsageBean gdfpBean = (DNAFileParserUsageBean)Beans.elementAt(6);
			GraphicInterfaceUsageBean guiBean = (GraphicInterfaceUsageBean)Beans.elementAt(7);
			RunUsageBean runBean = (RunUsageBean)Beans.elementAt(8);
			
			//Objects
			baseml = new BASEML((BASEMLUsageBean)Beans.elementAt(0));
			block = new Block((BlockUsageBean)Beans.elementAt(1));
			cdm = new CDM((CDMUsageBean)Beans.elementAt(2));
			data = new Data((DataUsageBean)Beans.elementAt(4));
			evpthwy = new Evpthwy(evBean);
			gdfp = new GeneralDNAFileParser(gdfpBean);
			run = new Run(runBean);
			substs = new Substs((SubstsUsageBean)Beans.elementAt(9));	
			tcsTree = new Tree((TreeUsageBean)Beans.elementAt(10));

			//get settings for data, run, evpthwy, gdfp
			bean.setProperties("all");
			data.obtainData();
			data.setPathways();
			runBean.setSubsts(substs);
			runBean.setEvpthwy(evpthwy);
			runBean.setCDM(cdm);
			evBean.setZScore(data.getZCriticalValues());
			gdfpBean.setVerboseFile(bean.getResultsDirectory());

			//gui
			if(bean.getErrorWindow())
			{
				guiBean.setTitle(title);
				gui = new GraphicInterface(driver, guiBean);

				gui.update(gui.getGraphics());
				gui.repaint();
				gui.show();
			}
			//commandLine
			else if(bean.getCommandLine())
			{
				commandLine = new CommandLine((CommandLineUsageBean)Beans.elementAt(3));
			}
		}
		catch(Exception e)
		{
			bean.errorMessage("There was an error initializing Driver Objects.\n  Please Verify User_Input settings files.");
			if(firstTry)
			{
				firstTry = false;
				resetSettings();
				reload();
			}
		}
	}//constructor
		
	/**
	 * Runs a given tree utilizing user-selections
	 * @param String tree - the tree specified to be ran
	 */
	public void run(String tree)
	{		
		//Method Vars
		BASEMLStub basemlStub;
		BASEMLTreeBean thisTree;
		GeneticCodeBean gcBean;
		GeneralDNAFileParser gdfpForRST;
		SubstsUsageBean subBean = substs.getBean();
		EvpthwyUsageBean evBean = evpthwy.getBean();
		TreeBean t = (TreeBean)gdfp.getTrees().get(tree);

		//Time
		long startTime = new Date().getTime();
		bean.logMessage("\nTime Started: "+ (DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.MEDIUM)).format(new Date()));
		
		//catch any problems
		try
		{
			//get settings from gui
			getSettings();
	
			//Watch for bad trees
			if(t == null || t.getNodeNames() == null)
				throw new Exception();
			
			//set output dir
			String dirTemp = bean.getResultsDirectory();
			print.setDirectory(tree);
	
			//Next check for TCS Edges
			if(tcsTree.getBean().getSelectedTCSEdges())
			{
				t = tcsTree.createSubTree(t, getGDFP());
				tree = t.getName();
			}
	
			//Verify that Baseml should be ran
			if(t.getNodeNames().size() == 0)
			{
				//BASEML operation
				basemlStub = (BASEMLStub)Stubs.elementAt(0);
				basemlStub.convert(gdfp);
				
				//get Choosen Tree
				thisTree = (BASEMLTreeBean)basemlStub.getTreeData().get(tree);
				
				//Move Temp Files
				if(bean.getMove())
					baseml.getBean().setOutFilePath(bean.getResultsDirectory());
				else
					baseml.getBean().setOutFilePath("");
				
				//Run BASEML
				baseml.run(gui, thisTree);
		
				//Open the RST file
				gdfpForRST = new GeneralDNAFileParser(gdfp.getBean());
				gdfpForRST.openNewFile("rst");
			
				//move data from gdfpForRST into the chosen tree
				gdfp.copyData((TreeBean)gdfpForRST.getTrees().get((String)gdfpForRST.getTreeNames().elementAt(0)), t);
				
				//move RST file
				if(bean.getMoveRst())
					baseml.moveRST(bean.getResultsDirectory());
			}
	
			//Choose Genetic Code and Properties
			gcBean = (GeneticCodeBean)data.getGeneticCodes().get(bean.getGeneticCode());
			if(bean.getProperties().equals("all"))
				gcBean.setPropertyNames(data.getPropertyNames());
			else
				gcBean.setPropertyNames((Vector)data.getPropertyLists().get(bean.getProperties()));
			subBean.setGCBean(gcBean);
			evBean.setGCBean(gcBean);
			
			//get Choosen Tree again
			subBean.setTree(t);
			subBean.setTaxa(gdfp.getTaxa());	
			
			//Choose Number of Categories
			subBean.setNumberOfCat(evBean.getNumberOfCat());
			evBean.setGFScore((double[])data.getGFCriticalValues().get((evBean.getNumberOfCat()-1)+""));		

			//Analysis
			Vector cdmResults = new Vector();
	//		if(Correlate)
	//			cdmResults = run.runSubstsCDM();
			Vector evResults = run.runSubstsEvpthwy();
				
			//Check for items to be removed
			t.getNodes().remove("false");
			t.getNodeNames().remove("false");
				
			//Print Results
			print.printAll(t, evResults, cdmResults);
	
			//Place Objects
			if(bean.getErrorWindow())
				gui.placeObjects();
			
			//Display the TCS Window
			if(bean.getErrorWindow() && tcsTree.getBean().getDisplayTCSWindow())
				tcsTree.display(bean.getResultsDirectory()+"tree.txt", tree);

			//reset ResultsDirectory
			bean.setResultsDirectory(dirTemp);
		}
		catch(Exception e)
		{
			bean.errorMessage("There was a problem while running tree: "+ tree);
		}

		//Time
		long endTime = new Date().getTime();
		bean.logMessage("Time Ended: "+ (DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.MEDIUM)).format(new Date()));
		bean.logMessage("Run Time(s): "+ ((endTime - startTime)/(double)1000) +"\n");
		
		//beep if gui
		if(bean.getErrorWindow() && tcsTree.getBean().getDisplayTCSWindow())
			Toolkit.getDefaultToolkit().beep();
	}
	
	/**
	 * determines branch pairwise comparisons
	 * @param boolean all - whether to do all pair wise comparisons
	 */
	public void pairWiseComparison(boolean all)
	{
		//Method vars
		String taxa1, taxa2;
		
		//catch any problems
		try
		{
			//get settings from gui
			getSettings();
			
			//get settings
			String oldPath = bean.getResultsDirectory();
			boolean selTCS = tcsTree.getBean().getSelectedTCSEdges();
			boolean disTCS = tcsTree.getBean().getDisplayTCSWindow();
			
			//change settings & results directory
			tcsTree.getBean().setSelectedTCSEdges(false);
			tcsTree.getBean().setDisplayTCSWindow(false);
			bean.setResultsDirectory(bean.getResultsDirectory() +"/pairwise");
			if(bean.getErrorWindow())
				gui.setObjects();
			
			//create compilation file
			
			
			//run a selection
			if(!all)
			{		
				String compare = bean.getBranchComparison();
				
				//Verify there is selected taxa
				if(compare == null || compare.equals(""))
					return;
				
				//break compare into taxa & tree name
				taxa1 = compare.substring(0, compare.indexOf(" .. "));
				taxa2 = compare.substring(compare.indexOf(" .. ")+4);
				
				//run comparison
				runPairWise(taxa1, taxa2);
			}
			//do all pair wise comparisions
			else
			{
				//cycle through taxa vector
				Vector taxa = gdfp.getTaxaNames();
				for(int i=0;i<taxa.size();i++)
				{
					//cycle through again, starting at i
					for(int j=i+1;j<taxa.size();j++)
					{
						runPairWise((String)taxa.elementAt(i), (String)taxa.elementAt(j));
					}
				}
			}
			
			//reset outfile path & settings
			bean.setResultsDirectory(oldPath);
			tcsTree.getBean().setSelectedTCSEdges(selTCS);
			tcsTree.getBean().setDisplayTCSWindow(disTCS);
			if(bean.getErrorWindow())
			{
				gui.setObjects();
				gui.placeObjects();
			}
		}
		catch(Exception e)
		{
			bean.errorMessage("There was a problem while running pair-wise comparisons");
		}
	}
	
	/**
	 * runs branch pairwise comparisons
	 * @param String taxa1 - the first taxa to be compared
	 * @param String taxa2 - the second taxa to be compared
	 */
	public void runPairWise(String taxa1, String taxa2)
	{
		//verify taxa are same length
		if(((TaxaBean)gdfp.getTaxa().get(taxa1)).getSequence().length() != ((TaxaBean)gdfp.getTaxa().get(taxa2)).getSequence().length())
			return;
			
		//Create a new tree, add to tree
		TreeBean t = ((DNAFileParserStub)Stubs.elementAt(6)).createTree(taxa1, taxa2);
		gdfp.getTreeNames().add(t.getName());
		gdfp.getTrees().put(t.getName(), t);
	
		//set certain vars to utilize run function
		run(t.getName());
		
		//add to compilation file
		
		//remove tree folder
		
		
		//remove trees from data struct
		gdfp.getTrees().remove(t.getName());
		gdfp.getTreeNames().remove(t.getName());
	}
	
	/**
	 * Gets a folder from user
	 */
	public void getFolder()
	{
		//Method Vars
		String fileName = "";
		
		//get settings from gui
		getSettings();
		
		//get fileName from gui
		if(bean.getErrorWindow())
			fileName = gui.getFolder();
		
		//Verify fileName & set bean
		if(fileName != null && !fileName.equals(""))
			bean.setResultsDirectory(fileName);
		
		//set objects
		if(bean.getErrorWindow())
			gui.setObjects();
	}
	
	/**
	 * Opens a file
	 * @param boolean newFile - whether to open a new file or not
	 */
	public void open(boolean newFile)
	{
		//Method Vars
		String fileName = "";

		//Time
		long startTime = new Date().getTime();
		bean.logMessage("\nTime Started: "+ (DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.MEDIUM)).format(new Date()));
		
		//get settings from gui
		getSettings();
		
		//get fileName from gui
		if(bean.getErrorWindow())
			fileName = gui.getFileName();
		
		//Verify fileName
		if(fileName != null && !fileName.equals(""))
		{
			//open File
			if(newFile)
				gdfp.openNewFile(fileName);
			else
				gdfp.openFile(fileName);

			//Place objects
			if(bean.getErrorWindow())
				gui.placeObjects();
		}
		
		//Time
		long endTime = new Date().getTime();
		bean.logMessage("Time Ended: "+ (DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.MEDIUM)).format(new Date()));
		bean.logMessage("Run Time(s): "+ ((endTime - startTime)/(double)1000) +"\n");
	}	

	/**
	 * Opens a tcs treefile
	 */
	public void openTree()
	{
		//Method Vars
		String fileName = "";
		
		//Time
		long startTime = new Date().getTime();
		bean.logMessage("\nTime Started: "+ (DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.MEDIUM)).format(new Date()));
		
		//close other files
		close();
		
		//get settings from gui
		getSettings();
		
		//get fileName from gui
		if(bean.getErrorWindow())
			fileName = gui.getFileName();
		
		//Verify fileName
		if(fileName != null && !fileName.equals(""))
		{
			//open File
			tcsTree.open(fileName, gdfp);
			
			//open up first tree - if one exists and only one exists
			if(gdfp.getTreeNames().size() == 1 && bean.getErrorWindow() && tcsTree.getBean().getDisplayTCSWindow())
				tcsTree.display(fileName, (String)gdfp.getTreeNames().elementAt(0));
			
			//Place objects
			if(bean.getErrorWindow())
				gui.placeObjects();
		}
		
		//Time
		long endTime = new Date().getTime();
		bean.logMessage("Time Ended: "+ (DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.MEDIUM)).format(new Date()));
		bean.logMessage("Run Time(s): "+ ((endTime - startTime)/(double)1000) +"\n");
	}	

	/**
	 * Closes any currently open files
	 */
	public void close()
	{
		//get rid of data
		gdfp.initialize();
		
		//change gui
		if(bean.getErrorWindow())
		{
			gui.placeObjects();
			tcsTree.close();
		}
	}	
	
	/**
	 * Reloads all Beans and entire program
	 */
	public void reload()
	{
		//set settings from gui
		if(bean.getErrorWindow())
		{
			//Keep track of old gui
			GraphicInterface guiTemp = gui;
			
			//restart everything
			initialize();
			
			//dispose old gui after, to avoid System exit
			guiTemp.dispose();
		}
		else
			initialize();
	}	
	
	/**
	 * Calls GUI to reset settings to file
	 */
	public void resetSettings()
	{
		//write settings to file
		String fileName = "";
		PrintWriter settings;
		
		try
		{
			//Loop through Stubs
			for(int i=0;i<Stubs.size();i++)
			{
				fileName = ((UsageBean)Beans.elementAt(i)).getInputFile();
				settings = new PrintWriter(new FileWriter(fileName));	
				settings.println(((Stub)Stubs.elementAt(i)).resetData());
				settings.close();
			}
			
			//reset properties
			bean.setProperties("all");
			
			//set settings from gui
			if(bean.getErrorWindow())
				gui.setObjects();
			else if(bean.getCommandLine())
				commandLine.setObjects();
		}
		catch(Exception e)
		{
			bean.errorMessage("There was a problem trying to write Settings to file: "+ fileName);
		}
	}

	/**
	 * Calls GUI to save settings to file
	 */
	public void saveSettings()
	{
		//get settings from gui
		getSettings();

		//write settings to file
		PrintWriter settings;

		try
		{			
			//Loop through Stubs
			for(int i=0;i<Stubs.size();i++)
			{
				settings = new PrintWriter(new FileWriter(((UsageBean)Beans.elementAt(i)).getInputFile()));	
				settings.println(((Stub)Stubs.elementAt(i)).writeData());
				settings.close();
			}
		}
		catch(Exception e)
		{
			bean.errorMessage("There was a problem trying to write Settings to file.");
		}
	}
		
	/**
	 * Calls Data to delete property lists and places these lists in its bean
	 */
	public void deletePropLists()
	{
		//get settings from gui
		if(bean.getErrorWindow())
			gui.getObjects();
		
		//set to all properties
		bean.setProperties("all");
		
		//delete the lists
		data.deletePropertyLists();
		
		//reset the gui
		if(bean.getErrorWindow())
		{
			gui.placeObjects();
			gui.setObjects();
		}
	}
	
	/**
	 * Calls GUI or commandLine to get settings and place in beans
	 */
	public void getSettings()
	{
		//get settings from gui
		if(bean.getErrorWindow())
			gui.getObjects();
		
		//command Line
		else if(bean.getCommandLine())
			commandLine.getObjects();
	}	

	/**
	 * Calls GUI to display specified object
	 */
	public void displayObject(String name)
	{
		gui.displayObject(name);
	}

	/**
	 *  Get method - returns BASEML baseml
	 */
	public BASEML getBaseml()
	{
		return baseml;
	}	
	
	/**
	 *  Get method - returns Block block
	 */
	public Block getBlock()
	{
		return block;
	}	

	/**
	 *  Get method - returns CDM cdm
	 */
	public CDM getCDM()
	{
		return cdm;
	}	
	
	/**
	 *  Get method - returns CommandLine commandLine
	 */
	public CommandLine getCommandLine()
	{
		return commandLine;
	}	
	
	/**
	 *  Get method - returns Data data
	 */
	public Data getData()
	{
		return data;
	}	

	/**
	 *  Get method - returns Evpthwy evpthwy
	 */
	public Evpthwy getEvpthwy()
	{
		return evpthwy;
	}	

	/**
	 *  Get method - returns GeneralDNAFileParser gdfp
	 */
	public GeneralDNAFileParser getGDFP()
	{
		return gdfp;
	}
	
	/**
	 *  Get method - returns GraphicInterface gui
	 */
	public GraphicInterface getGUI()
	{
		return gui;
	}	
	
	/**
	 *  Get method - returns Run run
	 */
	public Run getRun()
	{
		return run;
	}	
	
	/**
	 *  Get method - returns Substs substs
	 */
	public Substs getSubsts()
	{
		return substs;
	}	
	
	/**
	 *  Get method - returns Tree tree
	 */
	public Tree getTree()
	{
		return tcsTree;
	}		
	
	/**
	 *  Get method - returns DriverPrint print
	 */
	public DriverPrint getPrint()
	{
		return print;
	}

	/**
	 *  Get method - returns DriverUsageBean bean
	 */
	public DriverUsageBean getBean()
	{
		return bean;
	}

	/**
	 *  Determines if user specified commandLine use
	 *	@param String[] args - the arguments passed into main
	 */
	static private boolean getCommandLine(String[] args)
	{
		//Method var
		String param;
		
		//run through all args
		for(int i=0;i<args.length;i++)
		{
			param = args[i];
			
			//check for a "-"
			if(param.indexOf("-") == 0 && param.indexOf("c") != -1)
				return true;
		}
		
		//default
		return false;
	}
	
	/**
	 *  Determines if user specified exit immediately after a run
	 *	@param String[] args - the arguments passed into main
	 */
	static private boolean getExit(String[] args)
	{
		//Method var
		String param;
		
		//run through all args
		for(int i=0;i<args.length;i++)
		{
			param = args[i];
			
			//check for a "-"
			if(param.indexOf("-") == 0 && param.indexOf("x") != -1)
				return true;
		}
		
		//default
		return false;
	}
	
	/**
	 *  Determines the path to logFile, given by user
	 *	@param String[] args - the arguments passed into main
	 */
	static private String getLogFile(String[] args)
	{
		//Method var
		String param, fileName = "";
		
		try
		{
			//run through all args
			for(int i=0;i<args.length;i++)
			{
				param = args[i];
				
				//check for a "-" and a "l"
				if(param.indexOf("-") == 0 && param.indexOf("l") != -1)
				{
					//get fileName
					for(int j=i+1;j<args.length;j++)
					{
						param = args[j];
						
						//read all file until next parameter
						if(param.indexOf("-") == -1)
							fileName += " " + param;
						else
							break;
					}

					//return compiled fileName
					return fileName;		
				}
			}
		}
		catch(Exception e)
		{
			usage();
		}
		
		//default
		return fileName;
	}
	
	/**
	 *  Determines the path and File given by user
	 *	@param String[] args - the arguments passed into main
	 */
	static private String getFile(String[] args)
	{
		//Method var
		String param, fileName = "";
		
		try
		{
			//run through all args
			for(int i=0;i<args.length;i++)
			{
				param = args[i];
				
				//check for a "-" and a "l"
				if(param.indexOf("-") == 0 && param.indexOf("f") != -1)
				{
					//get fileName
					for(int j=i+1;j<args.length;j++)
					{
						param = args[j];
						
						//read all file until next parameter
						if(param.indexOf("-") == -1)
							fileName += param;
						else
							break;
					}
					
					//return compiled fileName
					return fileName;		
				}
			}
		}
		catch(Exception e)
		{
			usage();
		}
		
		//default
		return fileName;
	}
	
	/**
	 *  Displays to user the proper usage of treeSAAP start-up
	 */
	static private void usage()
	{
		System.out.println("Proper CommandLine Arguments for "+ title);
		System.out.println("-c		specifies Commandline interface.  Default = GUI");
		System.out.println("-l	name.txt	specifies the location of the Logfile.  Default = \"\"");
		System.out.println("-f	name.txt	specifies a file to be ran immediately.");
		System.out.println("-x		exit TreeSAAP immediately after a file is ran.");
	}
	
	/**
	 *  Immediately opens files specified by user, each must contain a block
	 *	@param String fileName - the name (or containing names) of the file(s) to be opened
	 */
	static private void autoFile(String fileName)
	{
	}
}//Driver