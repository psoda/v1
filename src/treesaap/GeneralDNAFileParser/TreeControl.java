/**
 *	TreeControl.class is responsible for the correct interpretation
 *	of trees read in from files.  It will also handle translate blocks and 
 *	ancestral trees.  It will utilize TreeBean to store all data it 
 *	generates.
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GeneralDNAFileParser;

import java.util.Vector;
import java.util.Hashtable;
import java.util.StringTokenizer;
import java.io.BufferedReader;

class TreeControl
{
	//Shared Objects
	private DNAFileParserUsageBean bean;	//UsageBean to identify user selections
	private String lastLine;				//the last line read in from a given file pointer
	
	//Shared Class Data objects
	private Vector treeNames;
	private Hashtable trees;
	
	//Specific Class Vars
	private TreeBean tb;			//A recently created TreeBean object
	private Vector translateBlock;	//Vector of strings representing translate block
	private Vector ancestralTree;	//Comparison tree between nodes and taxa
	
	
	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * calls method to initialize variables to be used.
	 *
	 */
	public TreeControl(DNAFileParserUsageBean usage)
	{
		bean = usage;
		initialize();
		
	}//Constructor

	/**
	 * Initializes data structures that contain tree info
	 */
	public void initialize()
	{
		treeNames = new Vector();
		trees = new Hashtable();
		
		translateBlock = new Vector();
		ancestralTree = new Vector();
	}//initialize
	
	/**
	 * Is given a current pointer to the file being parsed and the line
	 * that file is currently on.  It will then find the tree's name and 
	 * get it's relation before returning the file pointer
	 * 
	 * @param BufferedReader inFile Contains the file that is being parsed
	 * @param String currentLine The Line last read in from file
	 */
	public BufferedReader obtainTree(BufferedReader inFile, String currentLine)
	{
		//Method vars
		StringTokenizer st;		//tokenizes thisLine
		String temp;			//temporary pointer to token
		
		//Object vars
		String treeName = "";	//the tree's name
		String treeRel = "";	//the tree's relation
		boolean rooted = false;	//whether a tree is rooted or not
		

		//set last line to empty string
		lastLine = "";
		
		try
		{			
			//tokenize string
			st = new StringTokenizer(currentLine, "!@$%^&*<>?/\\|~`+=[]\" 	");

			//skip to the word containing tree
			while(st.hasMoreTokens())
			{
				temp = st.nextToken();
				if(temp.toLowerCase().indexOf("tree") != -1)
					break;
			}
			
			//build tree name by adding all non relation tokens, until the first valid token
			while(st.hasMoreTokens())
			{
				treeRel = st.nextToken();
				
				//First check to see if rooted or unrooted
				if(treeRel.compareToIgnoreCase("R") == 0)
					rooted = true;
				
				//Check to see if tree is unrooted
				else if(treeRel.compareToIgnoreCase("U") == 0)
					rooted = false;
				
				//add to nodeSeq if sequence
				else if(treeRel.indexOf("(") == -1)
					treeName += treeRel;
				
				//break out of iteration
				else
					break;
				
				//if no more tokens, and still valid sequences, get next line of tokens
				if(!st.hasMoreTokens())
					st = getMoreTokens(inFile);
			}
			
			//build tree relation by adding all relation tokens, until the first invalid token
			while(st.hasMoreTokens())
			{
				temp = st.nextToken();
				
				//add to nodeSeq if sequence
				if(temp.indexOf("(") != -1 || temp.indexOf(")") != -1 || temp.indexOf(",") != -1 || temp.indexOf(":") != -1)
					treeRel += temp;
				
				//break out of iteration
				else
					break;
				
				//Break if end of tree designated with ';'
				if(temp.indexOf(";") != -1)
					break;
				
				//if no more tokens, and still valid sequences, get next line of tokens
				if(!st.hasMoreTokens())
					st = getMoreTokens(inFile);
			}

			//verify at least some existant values for data structs
			if(!treeName.equals("") && !treeRel.equals("") && treeName.length() < 200)
			{		
				//this tree has already been read in, increment name
				if(treeNames.contains(treeName))
				{
					//iterate name until it's not found in name vector
					for(int i=0;true;i++)
					{
						temp = treeName +"_"+ i;
						if(!treeNames.contains(temp))
							break;
					}
					
					//valid tree name
					treeName = temp;
				}
				
				//create new TreeBean object and place in dataStructures
				tb = new TreeBean(treeName, treeRel, rooted, translateBlock);
				treeNames.add(treeName);
				trees.put(treeName, tb);
			}
		}
		
		//There was an exception to the file
		catch(Exception e) 
		{
			return null;
		}
		
		//Debug
		verbose();
		
		//clean up
		tb = null;
		
		return inFile;
	}//obtainTree

	/**
	 * Is given a current pointer to the file being parsed and the line
	 * that file is currently on.  It will then find the translate Block
	 * and set up vector for parsing
	 * 
	 * @param BufferedReader inFile Contains the file that is being parsed
	 * @param String currentLine The Line last read in from file
	 */
	public BufferedReader obtainTranslateBlock(BufferedReader inFile, String currentLine)
	{
		//Method vars
		StringTokenizer st;		//tokenizes thisLine
		String temp = "";		//temporary pointer to token
	
		//set last line to empty string
		lastLine = "";
		
		//Set/Reset translate block
		translateBlock = new Vector();
		
		try
		{			
			//tokenize string
			st = new StringTokenizer(currentLine, "{}, 	");
			
			//skip to the word containing "translate"
			while(st.hasMoreTokens())
			{
				temp = st.nextToken();
				if(temp.compareToIgnoreCase("translate") != -1)
					break;
			}

			//scan to next valid token
			while(!st.hasMoreTokens())
				st = getMoreTokens(inFile);
			
			//need different string tokenizer than that returned
			st = new StringTokenizer(lastLine, "{}, 	");
			
			//build translate block vector by placing in it each symbol-name pair, until the ';'
			while(st.hasMoreTokens())
			{					
				//get next token
				temp = st.nextToken();
				
				//Check for an invalid token
				if(temp.equals(";"))
					break;
				
				//check to see is ';' is part of token
				if(temp.indexOf(";") != -1)
				{
					translateBlock.add(temp.substring(0,temp.indexOf(";")));
					break;
				}
				else
					translateBlock.add(temp);				

				//if no more tokens, and still valid sequences, get next line of tokens
				if(!st.hasMoreTokens())
				{
					//need different string tokenizer than that returned
					getMoreTokens(inFile);
					st = new StringTokenizer(lastLine, "{}, 	");
				}
			}
		}
		
		//There was an exception to the file
		catch(Exception e) 
		{
			return null;
		}
		
		//Debug
		verbose();
		
		return inFile;
	}//obtainTranslateBlock
	
	/**
	 * Is given a current pointer to the file being parsed and the line
	 * that file is currently on.  It will then find the ancestral tree
	 * and place that in the last obtained tree
	 * 
	 * @param BufferedReader inFile Contains the file that is being parsed
	 * @param String currentLine The Line last read in from file
	 */
	public BufferedReader obtainAncestralTree(BufferedReader inFile, String currentLine)
	{
		//Method vars
		StringTokenizer st;		//tokenizes thisLine
		String temp = "";		//temporary pointer to token		
		
		//set last line to empty string
		lastLine = "";
		
		//Set/Reset previous vector
		ancestralTree = new Vector();
		
		try
		{			
			//tokenize string
			st = new StringTokenizer(currentLine, "!@$%^&*<>?/\\|~`+=[]\" 	");
		
			//skip to the word containing ".."
			while(st.hasMoreTokens())
			{
				temp = st.nextToken();
				if(temp.indexOf("..") != -1)
					break;
			}
			
			//build ancestral tree vector by placing in it each ".." pair, until the first invalid token
			while(st.hasMoreTokens())
			{
				ancestralTree.add(temp.substring(0,temp.indexOf("..")));
				ancestralTree.add(temp.substring(temp.indexOf("..") + 2));
				
				//get next token
				temp = st.nextToken();

				//Check for an invalid token
				if(temp.indexOf("..") == -1)
					break;
				
				//if no more tokens, and still valid sequences, get next line of tokens
				if(!st.hasMoreTokens())
					st = getMoreTokens(inFile);
			}
			
			//verify there exists a tree to place it into
			if(treeNames.size() > 0)
			{		
				//get the last tree
				tb = (TreeBean)trees.get((String)treeNames.elementAt(treeNames.size()-1));
				
				//verify it is a longer ancestral tree
				if(tb.getAncestralTree().size() < ancestralTree.size())
					tb.setAncestralTree(ancestralTree);
			}
		}
		
		//There was an exception to the file
		catch(Exception e) 
		{
			return null;
		}
		
		//Debug
		verbose();
		
		return inFile;
	}//obtainAncestralTree

	/**
	 * Error Checking - checks the  tree parenthesis format, 
	 * and other general formats of the trees.
	 * Places tokens in vectors in tree beans
	 */
	public void validateTrees()
	{
		//Method Variables
		TreeBean treeTemp;
		Vector relationVariables;
		String relation;
		int length;
		
		//iterate through all the trees
		for(int i=0;i<treeNames.size();i++)
		{
			//Set-up for processing
			treeTemp = (TreeBean)trees.get((String)treeNames.elementAt(i));
			relation = treeTemp.getRelation();
			length = relation.length();
			
			//create a new relation variable
			relationVariables = new Vector();
			
			//Do not reparse relations of done trees
			if(treeTemp.getRelationVariables() == null)
			{
				//No occurances of '()' or '(,'
				if(relation.indexOf("()") != -1 || relation.indexOf("(,") != -1)
				{
					trees.remove(treeTemp.getName());
					treeNames.remove(i);
					i--;
				}
				else
				{
					//TOTALS
					int lParen = 0, rParen = 0, comma = 0;
					
					//parsing variables
					int start, finish, tempFinish;
					
					//remove branch lengths from relation, place in revisedRelation
					while(relation.indexOf(":") != -1)
					{
						//start of branch length
						start = relation.indexOf(":");
						
						//set finish
						finish = length;
						
						//end of branch length
						tempFinish = relation.indexOf(",", start);
						if(tempFinish < finish && tempFinish != -1)
							finish = tempFinish;
						
						//possible other end ')'
						tempFinish = relation.indexOf(")", start);
						if(tempFinish < finish && tempFinish != -1)
							finish = tempFinish;
						
						//last possible end '('
						tempFinish = relation.indexOf("(", start);
						if(tempFinish < finish && tempFinish != -1)
							finish = tempFinish;
						
						relation = relation.substring(0,start) + relation.substring(finish);
					}
					
					//get variables
					StringTokenizer st = new StringTokenizer(relation, "(),;");
					while(st.hasMoreTokens())
						relationVariables.add(st.nextToken());
					
					//lParen,rParen,comma
					for(int j=0;j<relation.length();j++)
					{
						if(relation.charAt(j) == '(') 	lParen++;
						else if(relation.charAt(j) == ')') rParen++;
						else if(relation.charAt(j) == ',') comma++;
					}
					
					//Must have just one less ',' than names
					//Must have same number of '(' as ')'
					if(relationVariables.size() != comma+1 || lParen != rParen)
					{					
						trees.remove(treeTemp.getName());
						treeNames.remove(i);
						i--;
					}
					
					//valid tree, add in revised relation and new vector
					treeTemp.setRevisedRelation(relation);
					treeTemp.setRelationVariables(relationVariables);
				}
			}
			bean.setDataParsed(bean.getDataParsed() + length);
		}
		
	}//validateTrees

	/**
	 * Error Checking - checks the  tree parenthesis format, 
	 * and other general formats of the trees.
	 * Places tokens in vectors in tree beans
	 */
	public void applyTranslateBlock()
	{
		//Method Variables
		TreeBean treeTemp;
		Vector translateBlock;
		Vector relationVariables;
		int length, index;
		
		//iterate through all the trees
		for(int i=0;i<treeNames.size();i++)
		{
			//Set-up for processing
			treeTemp = (TreeBean)trees.get((String)treeNames.elementAt(i));
			translateBlock = treeTemp.getTranslateBlock();
			relationVariables = treeTemp.getRelationVariables();
			length = translateBlock.size();
			
			//make sure that there is a translate block
			if(translateBlock.size() > 0)
			{
				//go through the translate block, then change relationVariables
				for(int j=0;j<translateBlock.size();j+=2)
				{
					index = relationVariables.indexOf(translateBlock.elementAt(j));
	
					if(index != -1)
						relationVariables.set(index, translateBlock.elementAt(j+1));
				}
				
				//reset translate block to avoid coming here again
				translateBlock = new Vector();
			}

			//Tracking Variable
			bean.setDataParsed(bean.getDataParsed() + length);
		}
	}//applyTranslateBlock

	/**
	 * Assignment - takes in objects represeting taxa and then assigns given taxa 
	 * to each tree not already assigned.  All taxa must be the same length.
	 *
	 * @param Vector taxaNames the object that contains all taxa names
	 * @param Hashtable taxa the object that contains all taxa
	 */
	public void placeTaxa(Vector taxaNames, Hashtable taxa)
	{
		//Method Variables
		TreeBean treeTemp;
		String variable = "";
		Vector relationVariables;
		Vector transRelVars;
		int length, var, min;
		
		//Variables to be built and placed in tree
		Vector thisTaxaNames;
		
		//iterate through all the trees
		for(int i=0;i<treeNames.size();i++)
		{
			//Set-up for processing
			treeTemp = (TreeBean)trees.get((String)treeNames.elementAt(i));
			relationVariables = treeTemp.getRelationVariables();
			
			//Track
			length = treeTemp.getRelation().length();

			//Only manipulate trees that haven't been assigned taxa
			if(treeTemp.getTaxaNames() == null || treeTemp.getTaxaNames().size() == 0)
			{
				//Create new variables
				thisTaxaNames = new Vector();
				transRelVars = new Vector();
				
				//Process vars
				min = taxaNames.size();
				
				//go through all the elements of the relation variable
				for(int j=0;j<relationVariables.size();j++)
				{				
					//get string from relation variable
					variable = (String)relationVariables.elementAt(j);
					
					//determine whether it is a number or not
					try
					{
						var = Integer.valueOf(variable).intValue() - 1;
					}
					
					//Not a Number
					catch(NumberFormatException e)
					{
						var = taxaNames.indexOf(variable);
					}
					
					//if found an entry & it's not larger than Vector's size
					if(var != -1 && var < taxaNames.size())
					{
						variable = (String)taxaNames.elementAt(var);
						
						//add on index to variable name to sort later
						thisTaxaNames.add(var +"$"+ variable);
						transRelVars.add(variable);
						
						//set min value
						if(var < min)
							min = var;
					}
						
					//Entry not found, this tree does not yet have all it's taxa loaded in
					else
					{
						//reinitialize objects
						thisTaxaNames = new Vector();
						
						//break out of for loop
						break;
					}
				}
				
				//Processing and error checking - only bother if found taxa
				if(thisTaxaNames.size() > 0)
				{
					//temp variable
					Vector tempVec = (Vector)thisTaxaNames.clone();
					
					//decrement min by 1, for the search to return a first answer
					min--;
					
					try
					{			
						//Sort Taxa based on index from taxaNames, Ancestral tree is dependent on ordering
						for(int j=0;j<thisTaxaNames.size();j++)
						{
							//get next var
							if(j == 0)
								var = min;
							
							//get sorting variable, pop off number
							else
							{
								var = Integer.valueOf(variable.substring(0,variable.indexOf("$"))).intValue();
							}
							
							//get next element from taxa names
							variable = nextTaxa(tempVec,var);
							
							//set String in thisTaxaNames the result from the search
							thisTaxaNames.set(j, variable.substring(variable.indexOf("$") + 1));
						}						

						//Check all entries to verify they all have same length
						//get length of first taxa
						var = ((TaxaBean)taxa.get((String)thisTaxaNames.elementAt(0))).getSequence().length();
						
						//iterate through all taxa, checking length
						for(int j=0;j<thisTaxaNames.size();j++)
						{
							if(var != ((TaxaBean)taxa.get((String)thisTaxaNames.elementAt(j))).getSequence().length())
							{
								//reinitialize objects
								thisTaxaNames = new Vector();
								
								//let user know about lengths
								bean.errorMessage("\nTaxa being assigned to Tree: "+ treeTemp.getName() +" were not the same length.\n	Please fix taxa.");
								
								//break out of for loop
								break;
							}
						}
					}
				
					//Taxa Assigning Error
					catch(Exception E)
					{
						thisTaxaNames = new Vector();
						bean.errorMessage("\nTaxa being assigned to Tree: "+ treeTemp.getName() +" did not line up properly.\n	Please fix taxa.");
					}
				}
			
				//set new vector into tree
				treeTemp.setTaxaNames(thisTaxaNames);
				treeTemp.setRelationVariables(transRelVars);
			}
			
			//Tracking Variable
			bean.setDataParsed(bean.getDataParsed() + length);
		}
	}//placeTaxa

	/**
	 * Assignment - takes in vectors for nodes and then assigns given nodes 
	 * to each tree with an ancestral tree and has not been assigned.  
	 * All nodes must be the same length.
	 *
	 * @param Vector nodeNames the object that contains all node names
	 * @param Vector nodes the object that contains all nodes
	 */
	public void placeNodes(Vector nodeNames, Vector nodes)
	{
		//Method Variables
		TreeBean treeTemp;
		String variable;
		Vector ancestralTree;
		int length, var, taxa;
		
		//Variables to be built and placed in tree
		Vector thisNodeNames;
		Hashtable thisNodes;
		
		//Pointer variables
		Vector taxaNames;
		
		//iterate through all the trees
		for(int i=0;i<treeNames.size();i++)
		{
			//Set-up for processing
			treeTemp = (TreeBean)trees.get((String)treeNames.elementAt(i));
			ancestralTree = treeTemp.getAncestralTree();
			taxaNames = treeTemp.getTaxaNames();
			taxa = taxaNames.size();
			
			//Tracking
			length = ancestralTree.size();
	
			//Create new variables
			thisNodeNames = new Vector();
			thisNodes = new Hashtable();

			//Only manipulate trees that haven't been assigned nodes
			if((treeTemp.getNodeNames() == null || treeTemp.getNodeNames().size() == 0) && taxa > 0)
			{				
				//go through all the elements of the relation variable
				for(int j=0;j<ancestralTree.size();j++)
				{	
					//get string from relation variable
					variable = (String)ancestralTree.elementAt(j);

					//determine whether it is a number or not
					try
					{
						var = Integer.valueOf(variable).intValue() - taxa - 1;
					}
					
					//Not a Number
					catch(NumberFormatException e)
					{
						var = nodeNames.indexOf(variable);
					}

					//If var is negative - not a node
					if(var >= 0)
					{						
						//get String name of node
						variable = (String)nodeNames.elementAt(var);
						
						//add node to tree if doesn't already have it
						if(thisNodeNames.indexOf(variable) == -1)
						{
							thisNodeNames.add(variable);
							thisNodes.put(variable,nodes.elementAt(var));
						}
						
						//set name in ancestral tree
						ancestralTree.set(j, variable);
					}
					
					//Var possibly represents a taxa
					else
					{
						//determine whether it is a number or not
						try
						{
							var = Integer.valueOf(variable).intValue() - 1;
						}
						
						//Not a Number
						catch(NumberFormatException e)
						{
							var = taxaNames.indexOf(variable);
						}
						
						//If var is negative - not a taxa or node
						if(var >= 0)
						{						
							//get String name of taxa
							variable = (String)taxaNames.elementAt(var);

							//set name in ancestral tree
							ancestralTree.set(j, variable);
						}
						
						else
						{
							bean.errorMessage("\nNodes & Taxa being assigned to Tree: "+ treeTemp.getName() +" were not all valid.\n	Please review all your data.");
							break;
						}
					}
				}

				//Check all entries to verify they all have same length
				if(thisNodeNames.size() > 0)
				{
					//get length of first node
					var = ((TaxaBean)thisNodes.get((String)thisNodeNames.elementAt(0))).getSequence().length();
					
					//iterate through all nodes, checking length
					for(int j=0;j<thisNodeNames.size();j++)
					{
						if(var != ((TaxaBean)thisNodes.get((String)thisNodeNames.elementAt(j))).getSequence().length())
						{
							//reinitialize objects
							thisNodeNames = new Vector();
							thisNodes = new Hashtable();
							
							//let user know about lengths
							bean.errorMessage("\nNodes being assigned to Tree: "+ treeTemp.getName() +" were not the same length.\n	Please fix nodes.");
							
							//break out of for loop
							break;
						}
					}
				}
				
				//set new vectors into tree
				treeTemp.setNodeNames(thisNodeNames);
				treeTemp.setNodes(thisNodes);
				
				//remove from nodes and node names those nodes selected
				for(int j=0;j<thisNodeNames.size();j++)
				{
					nodes.remove((String)thisNodeNames.elementAt(j));
					nodeNames.remove((String)thisNodeNames.elementAt(j));
				}
			}
			
			//there are no taxa, place in new vectors
			else if(taxa <= 0)			
			{
				treeTemp.setNodeNames(thisNodeNames);
				treeTemp.setNodes(thisNodes);
			}

			//Tracking Variable
			bean.setDataParsed(bean.getDataParsed() + length);
		}
	}//placeNodes
	
	/**
	 * After Taxa have been assigned and everything validated,
	 * simply replace all values in relation tree with the 
	 * String name representation of the taxa
	 */
	public void updateRelation()
	{
		//Method Variables
		TreeBean treeTemp;
		String relation;
		String revisedRelation = "";
		Vector relationVariables;
		char thisChar, prevChar = '(';
		int length, index;
		
		//iterate through all the trees
		for(int i=0;i<treeNames.size();i++)
		{
			//Set-up for processing
			treeTemp = (TreeBean)trees.get((String)treeNames.elementAt(i));
			relationVariables = treeTemp.getRelationVariables();
			relation = treeTemp.getRevisedRelation();
			
			//Tracking
			length = treeTemp.getRelation().length();
			
			//replace all symbols in relation with $ in revisedRelation
			for(int j=0;j<relation.length();j++)
			{
				thisChar = relation.charAt(j);
				
				//if char is not a ( ) or , then change to a $
				if(thisChar == '(' || thisChar == ')' || thisChar == ',')
				{
					revisedRelation += thisChar;
					prevChar = thisChar;
				}
				
				//add to revisedRelation if prevChar != $
				else if(prevChar != '$' && thisChar != ';')
				{
					revisedRelation += '$';
					prevChar = '$';
				}
			}
			
			//replace all $ with a relation variable
			for(int j=0;j<relationVariables.size();j++)
			{
				index = revisedRelation.indexOf("$");
				
				//valid index
				if(index != -1)
				{
					revisedRelation = revisedRelation.substring(0,index) 
						+ (String)relationVariables.elementAt(j) 
						+ revisedRelation.substring(index+1);
				}
			}
			
			treeTemp.setRevisedRelation(revisedRelation);
	
			//reset for next loop
			revisedRelation = "";
			
			//Tracking Variable
			bean.setDataParsed(bean.getDataParsed() + length);
		}
	}//updateRelation
	

	/**
	 * Returns a Tokenized String of the next line of the file if
	 * it exists.  This code is referenced for obtainTree, 
	 * obtainTranslateBlock, and obtainAncestralTree.
	 *
	 * @param BufferedReader inFile Allows method to access next line to be tokenized
	 */
	private StringTokenizer getMoreTokens(BufferedReader inFile)
	{
		try
		{
			//get next line valid line
			String nextLine = inFile.readLine();	
			while(nextLine != null && nextLine.equals(""))
			{
				//increment charsRead
				if(nextLine.equals(""))
					bean.setCharsRead(bean.getCharsRead() + 2);
				nextLine = inFile.readLine();
			}
			
			//if there was a line
			if(nextLine != null)
			{
				//set last line, to allow super class access to it
				lastLine = nextLine;
				
				//increment charsRead, to keep accuracy on progress
				bean.setCharsRead(bean.getCharsRead() + nextLine.length() + 2);
				
				//return a Tokenizer
				return new StringTokenizer(nextLine, "!@$%^&*<>?/\\|~`+=\" 	");
			}
			return new StringTokenizer("");
		}
		//There was an exception to the file
		catch(Exception e) 
		{
			return new StringTokenizer("");
		}
	}//getMoreTokens

	/**
	 * Returns a String of the next taxa name to be inserted 
	 * into taxaNames for a given tree.  Thus, preserving order
	 * to allow use of a numbering system for the ancestral tree.
	 *
	 * @param Vector names Contains all the names, with attached indexes
	 * @param int min The next name looking for must be greater than min
	 */
	private String nextTaxa(Vector names, int min)
	{
		//method vars
		int var;
		int localLeast = -1;
		String variable, nextName = "";
		
		//go through name vector
		for(int i=0;i<names.size();i++)
		{
			//get sorting variable, pop off number
			variable = (String)names.elementAt(i);
			var = Integer.valueOf(variable.substring(0,variable.indexOf("$"))).intValue();

			//first check that var is greater than min
			if(var > min)
			{
				//set up localLeast
				if(localLeast == -1)
					localLeast = var;

				//if var is less than localLeast, set nextName
				if(var <= localLeast)
				{
					nextName = variable;
					localLeast = var;
				}
			}
		}
		
		//return the last set String
		return nextName;
		
	}//nextTaxa
	
	/**
	 * Returns the lastLine variable.
	 * This allows super class to obtain the last line 
	 * read from file while in this class
	 */
	public String getLastLine()
	{
		return lastLine;
	}
	
	/**
	 * Returns the tree hashtable.
	 * This allows super class to obtain 
	 * the data read from file.
	 */
	public Hashtable getTrees()
	{
		return trees;
	}
	
	/**
	 * Returns the tree names vector.
	 * This allows super class to obtain 
	 * the names of trees read from file.
	 */
	public Vector getTreeNames()
	{
		return treeNames;
	}

	/**
	 * Sets bean to the one given
	 *
	 *	@param Bean DNAFileParserUsageBean is the bean to be used
	 */
	public void setBean(DNAFileParserUsageBean Bean)
	{
		bean = Bean;
	}

	/**
	 * Output to programmer for testing purposes
	 * The UsageBean will indicate the level of verbose
	 *
	 */
	private void verbose()
	{
		String line;
		int verbose = bean.getGdfpVerbose();
		
		if(verbose <= 1 && tb != null)
		{
			if(tb.getRooted())
				bean.verboseMessage("TREE "+ tb.getName() +": [&R] "+ tb.getRelation());
			else
				bean.verboseMessage("TREE "+ tb.getName() +": [&U] "+ tb.getRelation());
		}
		if(verbose <= 2 && translateBlock.size() > 0)
		{
			line = "";
			bean.verboseMessage("Translate Block: ");
			for(int i=0;i<translateBlock.size();i++)
				line += translateBlock.elementAt(i)+", ";
			bean.verboseMessage(line);
		}
		if(verbose <= 3 && ancestralTree.size() > 0)
		{
			line = "";
			bean.verboseMessage("Ancestral Tree: ");
			for(int i=0;i<ancestralTree.size();i++)
				line += ancestralTree.elementAt(i)+", ";
			bean.verboseMessage(line);
		}

	}//verbose

}//TreeControl
