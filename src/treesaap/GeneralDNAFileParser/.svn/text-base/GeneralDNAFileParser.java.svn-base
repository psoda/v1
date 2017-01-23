/**
 *	GeneralDNAFileParser.class is responsible for the correct interpretation
 *	of files to be opened.  It must handle cases from NEXUS to RST files.  
 *	This class also creates and intrepretes two crucial data structures.
 *	Trees and Taxa are necessary for later use in any program this class is 
 *	associated with.
 *
 *	@author	Joshua Sailsbery
 *	@version	3.0
 *	@since	1.0
 */
package treesaap.GeneralDNAFileParser;

import java.util.Vector;
import java.util.Hashtable;
import java.util.StringTokenizer;
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;

public class GeneralDNAFileParser
{
	//Class Variables - determined by super user
	private DNAFileParserUsageBean bean;					//Settings (ie. variables) used by this class to determine user preferences
	private DNAFileParserProgressWindow progressWindow;		//Object of progress Window
	
	//Classes instantiated
	private TaxaControl taxaC;					//Creates and manages the taxa read in from file
	private TreeControl treeC;					//Creates and manages the trees read in from file
	
	
	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * Initializes variables to be used.
	 * Use this Constructor if bean not specified
	 */
	public GeneralDNAFileParser()
	{
		//set up usage bean
		bean = new DNAFileParserUsageBean();

		//create control objects
		taxaC = new TaxaControl(bean);
		treeC = new TreeControl(bean);
	}//Constructor

	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * Initializes variables to be used.
	 * Use this Constructor if bean has been specified - used in conjunction with Driver
	 *
	 * @param Bean DNAFileParserUsageBean given - containing values determined by parent class
	 */
	public GeneralDNAFileParser(DNAFileParserUsageBean Bean)
	{
		//set up usage bean
		bean = Bean;
		
		//create control objects
		taxaC = new TaxaControl(bean);
		treeC = new TreeControl(bean);
	}//Constructor

	/**
	 * Resets objects that are referenced and utilized.
	 * Initializes variables to be used.
	 * Does not reset settings.
	 */
	public void initialize()
	{
		taxaC.initialize();
		treeC.initialize();
		
	}//initialize

	/**
	 * OpenNewFile simply calls initialize to reset all
	 * class vars, then calls openFile() that will control
	 * the obtaining of new data objects
	 *
	 * @param fileName String of the entire path of file to be opened
	 */
	public void openNewFile(String fileName)
	{
		initialize();
		openFile(fileName);
		
	}//openNewFile

	/**
	 * Calls method to obtain file name from user, 
	 * then it will call the appropiate methods and classes
	 * to parse the file.
	 *
	 * @param fileName String of the entire path of file to be opened
	 */
	public void openFile(String fileName)
	{		
		//Validate file name
		String fileNameTemp = bean.getAbsoluteFilePath(fileName, false);
		if(fileNameTemp == null)
		{
			bean.errorMessage("\nFile ("+ fileName +") does not exist!\n  Please try again.");
			return;
		}
		fileName = fileNameTemp;
		
		//Start Progress Window
		progressWindow = new DNAFileParserProgressWindow(bean, fileName);
		
		
		//search file for key words and pass those to appropriate methods
		if(!searchFile(fileName))
			return;
		
		//set up counter to inform user of progress
		bean.setDataToParse(totalWork());
		bean.setDataParsed(0);
		
		//validate taxa & nodes
		taxaC.validateTaxa();
		taxaC.validateNodes();

		//parse & validate trees
		treeC.validateTrees();
		
		//apply translate blocks
		treeC.applyTranslateBlock();
		
		//debug
		if(bean.getGdfpVerbose() <= 5)
		{
			bean.verboseMessage("VALIDATED:");
			verbose();
		}

		//build trees - associate taxa, nodes, and ancestral trees
		treeC.placeTaxa(taxaC.getTaxaNames(), taxaC.getTaxa());
		treeC.placeNodes(taxaC.getNodeNames(), taxaC.getNodes());
		
		//update the relation for this environment
		treeC.updateRelation();

		//more debugging
		if(bean.getGdfpVerbose() <= 6)
		{
			//Method var
			String line;
			
			bean.verboseMessage("ASSIGNED:");
			for(int i=0;i<treeC.getTreeNames().size();i++)
			{
				bean.verboseMessage(treeC.getTreeNames().elementAt(i) +": "+ ((TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i))).getRelation());
				bean.verboseMessage(((TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i))).getRevisedRelation());
				TreeBean tb = (TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i));
				
				line = "	Taxa: ";
				Vector print = tb.getTaxaNames();
				for(int j=0;j<print.size();j++)
					line += print.elementAt(j) +", ";
				bean.verboseMessage(line);
				
				line = "	Nodes: ";
				print = tb.getNodeNames();
				for(int j=0;j<print.size();j++)
					line += print.elementAt(j) +", ";
				bean.verboseMessage(line);
				
				line = "	Ancestral Tree: ";
				print = tb.getAncestralTree();
				for(int j=0;j<print.size();j++)
					line += print.elementAt(j) +", ";
				bean.verboseMessage(line);
			}
		}
		
		//End Progress Window
		progressWindow.end();
		
		//reset counters
		bean.setCharsRead(0);
		bean.setCharsToRead(0);
		bean.setDataParsed(0);
		bean.setDataToParse(0);
		
	}//openFile
	
	/**
	 * Determines the location of specific occurences of 
	 * taxa, trees, nodes, translate blocks, and ancestral trees
	 * Then the appropriate methods associated with each is called
	 *
	 * @param String fileName - file to be parsed
	 */
	private boolean searchFile(String fileName)
	{		
		//Method Vars
		boolean rst;				//whether file being read in is an rst file or not
		String token;				//current token
		String prevToken = "";		//previous token
		StringTokenizer st;			//To obtain tokens of each line
		
		try
		{			
			//Create File object
			File fd = new File(fileName);
			BufferedReader inFile = new BufferedReader(new FileReader(fd));
			
			//get rst		
			if(fd.getName().equals("rst"))
				rst = true;
			else
				rst = false;
			
			//set up progress monitoring
			bean.setCharsToRead(fd.length());
			bean.setCharsRead(0);
			
			//Go through line by line until the end of the file
			String thisLine = inFile.readLine();
			while(thisLine != null)
			{					
				//Get each token from string
				st = new StringTokenizer(thisLine, "!@$%^&*<>?/\\|~`+=\" 	");
				
				//track total chars for progress window
				bean.setCharsRead(bean.getCharsRead() + thisLine.length() + 2);
				
                                int tokenNumber = 0;
				//Check for a sequence with each token
				while(st.hasMoreTokens()) 
				{
					token = st.nextToken();
					
					//Check for a tree flag, add charsRead if not already there
					if(!rst && token.compareToIgnoreCase("tree") == 0 && tokenNumber == 0)	
					{		
                        inFile = treeC.obtainTree(inFile, thisLine);
						//Get certain values from TreeControl
						thisLine = treeC.getLastLine();
						st = new StringTokenizer(thisLine, "!@$%^&*<>?/\\|~`+=\" 	");
						
						//Error from TreeControl
						if(inFile == null)
							new Exception();
					}
                    //RST tree format has the word 'tree' as the first word on the line.
                    else if(rst && token.compareToIgnoreCase("tree") == 0 && tokenNumber == 0)	
					{		
                                                inFile = treeC.obtainTree(inFile, thisLine);
						//Get certain values from TreeControl
						thisLine = treeC.getLastLine();
						st = new StringTokenizer(thisLine, "!@$%^&*<>?/\\|~`+=\" 	");
						
						//Error from TreeControl
						if(inFile == null)
							new Exception();
					}

					//Check for a translate block flag, charsRead number if not already there
					else if(token.compareToIgnoreCase("translate") == 0)
					{
						inFile = treeC.obtainTranslateBlock(inFile, thisLine);
						
						//Get certain values from TreeControl
						thisLine = treeC.getLastLine();
						st = new StringTokenizer(thisLine, "!@$%^&*<>?/\\|~`+=\" 	");
						
						//Error from TreeControl
						if(inFile == null)
							new Exception();
					}
					
					//Check for an ancestral tree flag, add charsRead if not already there
					else if(token.indexOf("..") != -1)
					{
						inFile = treeC.obtainAncestralTree(inFile, thisLine);
						
						//Get certain values from TreeControl
						thisLine = treeC.getLastLine();
						st = new StringTokenizer(thisLine, "!@$%^&*<>?/\\|~`+=\" 	");
						
						//Error from TreeControl
						if(inFile == null)
							new Exception();
					}
					
					//Check for a node flag, call obtainNode in TaxaControl
					else if(token.toLowerCase().indexOf("node") != -1)
					{						
						inFile = taxaC.obtainNode(inFile, thisLine, rst);
						
						//Get certain values from TaxaControl
						thisLine = taxaC.getLastLine();
						st = new StringTokenizer(thisLine, "!@$%^&*<>?/\\|~`+=\" 	");
						
						//Error from TaxaControl
						if(inFile == null)
							new Exception();
					}
					
					//Lastly check to see if it's a sequence, call obtainTaxa in TaxaControl
					else if(taxaC.isSequence(token))
					{
						inFile = taxaC.obtainTaxa(inFile, thisLine, prevToken, rst);

						//Get certain values from TaxaControl
						thisLine = taxaC.getLastLine();
						st = new StringTokenizer(thisLine, "!@$%^&*<>?/\\|~`+=\" 	");
						
						//Error from TaxaControl
						if(inFile == null)
							new Exception();
					}
					
					//Not any of these default values, set to prevToken
					else
					{
						prevToken = token;
					}
                                        tokenNumber++;
				}
				
				//obtain the next line of the file
				thisLine = inFile.readLine();
			}
			
			//reached end of file
			bean.setCharsRead(bean.getCharsToRead());

			//close the file
			inFile.close();
		}
		
		//There was an exception to the file
		catch(Exception e) 
		{
			bean.errorMessage("\nThere was an Exception while obtaining data from file "+ fileName +"\nCheck for format errors.");
			return false;
		}
		/*
		System.out.println("verbose: ");
		String line;
		
		System.out.println("Nodes: ");
		for(int i=0;i<taxaC.getNodeNames().size();i++)
			System.out.println(taxaC.getNodeNames().elementAt(i) +": "+ ((TaxaBean)taxaC.getNodes().elementAt(i)).getSequence());	
		
		System.out.println("Taxa: ");
		for(int i=0;i<taxaC.getTaxaNames().size();i++)
			System.out.println(taxaC.getTaxaNames().elementAt(i) +": "+ ((TaxaBean)taxaC.getTaxa().get((String)taxaC.getTaxaNames().elementAt(i))).getSequence());
	
		System.out.println("Trees: ");
		for(int i=0;i<treeC.getTreeNames().size();i++)
		{
			System.out.println(treeC.getTreeNames().elementAt(i) +": "+ ((TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i))).getRelation());
			System.out.println(((TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i))).getRevisedRelation());
			TreeBean tb = (TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i));
			
			line = "	Translate Block: ";
			Vector print = tb.getTranslateBlock();
			for(int j=0;j<print.size();j++)
				line += print.elementAt(j) +", ";
			System.out.println(line);
			
			line = "	Ancestral Tree: ";
			print = tb.getAncestralTree();
			for(int j=0;j<print.size();j++)
				line += print.elementAt(j) +", ";
			System.out.println(line);
		}
		System.out.println("endverbose");
		*/
		
		
		//debug
		if(bean.getGdfpVerbose() <= 4)
		{
			bean.verboseMessage("LOCATED:");
			verbose();
		}
		
		//nothing fails, return true
		return true;
		
	}//searchFile

	/**
	 * Copies all relevant data from the fromTree into the specified toTree
	 *
	 * @param TreeBean fromTree - tree to be copied from
	 * @param TreeBean toTree - tree to be copied into
	 */
	public void copyData(TreeBean fromTree, TreeBean toTree)
	{		
		//Copy over Ancestral Data
		toTree.setAncestralTree(fromTree.getAncestralTree());
		toTree.setNodes(fromTree.getNodes());
		toTree.setNodeNames(fromTree.getNodeNames());
	}//copyData

	/**
	 * Calculates and returns a long 
	 * representing the total amount of work to be 
	 * done by this class by parsing
	 */
	private long totalWork()
	{
		long dataToParse = 0;

		//get the amount of work to be done on taxa
		Vector v = taxaC.getTaxaNames();
		Hashtable h = taxaC.getTaxa();
		for(int i=0;i<v.size();i++)
			dataToParse += ((TaxaBean)h.get((String)v.elementAt(i))).getSequence().length();
	
		//get the amount of work to be done on nodes
		v = taxaC.getNodes();
		for(int i=0;i<v.size();i++)
			dataToParse += ((TaxaBean)v.elementAt(i)).getSequence().length();
		
		//get the amount of work to be done on trees
		TreeBean treeB;
		v = treeC.getTreeNames();
		h = treeC.getTrees();
		for(int i=0;i<v.size();i++)
		{
			treeB = (TreeBean)h.get((String)v.elementAt(i));
			dataToParse += treeB.getRelation().length() * 3;
			dataToParse += treeB.getTranslateBlock().size();
			dataToParse += treeB.getAncestralTree().size();
		}

		return dataToParse;
	}//totalWork

	/**
	 * Returns the tree hashtable.
	 * This allows super class to obtain 
	 * the data read from file.
	 */
	public Hashtable getTrees()
	{
		return treeC.getTrees();
	}//getTrees
	
	/**
	 * Returns the tree names vector.
	 * This allows super class to obtain 
	 * the names of trees read from file.
	 */
	public Vector getTreeNames()
	{
		return treeC.getTreeNames();
	}//getTreeNames
	
	/**
	 * Returns the taxa hashtable.
	 * This allows super class to obtain 
	 * the data read from file.
	 */
	public Hashtable getTaxa()
	{
		return taxaC.getTaxa();
	}//getTaxa
	
	/**
	 * Returns the taxa names vector.
	 * This allows super class to obtain 
	 * the names of taxa read from file.
	 */
	public Vector getTaxaNames()
	{
		return taxaC.getTaxaNames();
	}//getTaxaNames
	
	/**
	 * Returns the DNAFileParserUsageBean bean
	 * This allows super class to obtain the bean
	 * settings and manipulate them as necessary.
	 */
	public DNAFileParserUsageBean getBean()
	{
		return bean;
	}
	
	/**
	 * Sets bean to the one given, and sets the Controls beans as well
	 *
	 *	@param Bean DNAFileParserUsageBean is the bean to be used
	 */
	public void setBean(DNAFileParserUsageBean Bean)
	{
		bean = Bean;
		taxaC.setBean(bean);
		treeC.setBean(bean);
	}

	/**
	 * Output to programmer for testing purposes
	 * The UsageBean will indicate the level of verbose
	 *
	 */
	private void verbose()
	{
		//Method vars
		String line;
		
		bean.verboseMessage("Nodes: ");
		for(int i=0;i<taxaC.getNodeNames().size();i++)
			bean.verboseMessage(taxaC.getNodeNames().elementAt(i) +": "+ ((TaxaBean)taxaC.getNodes().elementAt(i)).getSequence());	
		
		bean.verboseMessage("Taxa: ");
		for(int i=0;i<taxaC.getTaxaNames().size();i++)
			bean.verboseMessage(taxaC.getTaxaNames().elementAt(i) +": "+ ((TaxaBean)taxaC.getTaxa().get((String)taxaC.getTaxaNames().elementAt(i))).getSequence());
	
		bean.verboseMessage("Trees: ");
		for(int i=0;i<treeC.getTreeNames().size();i++)
		{
			bean.verboseMessage(treeC.getTreeNames().elementAt(i) +": "+ ((TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i))).getRelation());
			bean.verboseMessage(((TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i))).getRevisedRelation());
			TreeBean tb = (TreeBean)treeC.getTrees().get((String)treeC.getTreeNames().elementAt(i));
			
			line = "	Translate Block: ";
			Vector print = tb.getTranslateBlock();
			for(int j=0;j<print.size();j++)
				line += print.elementAt(j) +", ";
			bean.verboseMessage(line);
			
			line = "	Ancestral Tree: ";
			print = tb.getAncestralTree();
			for(int j=0;j<print.size();j++)
				line += print.elementAt(j) +", ";
			bean.verboseMessage(line);
		}
		
	}//verbose

}//GeneralDNAFileParser