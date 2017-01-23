/**
 *	Tree.class is responsible for the display of a given tree
 *	on the TCS display and extrapolate data from that
 *
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Tree;

import java.util.Vector;
import java.util.Hashtable;
import java.util.Enumeration;
import java.util.StringTokenizer;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.BufferedReader;

import treesaap.Driver.Driver;
import treesaap.GeneralDNAFileParser.TreeBean;
import treesaap.GeneralDNAFileParser.TaxaBean;
import treesaap.GeneralDNAFileParser.GeneralDNAFileParser;

import EDU.auburn.VGJ.graph.Node;
import EDU.auburn.VGJ.graph.Edge;
import EDU.auburn.VGJ.gui.GraphWindow;
import EDU.auburn.VGJ.algorithm.tree.TreeAlgorithm;

public class Tree
{
	//CLASS Vars
	private TreeUsageBean bean;					//Bean containing all values necessary for operation
	private GraphWindow graph_editing_window;	//Object of graph for tree
	
	
	/**
	 * Tree Constructor
	 * creates TreeUsageBean object
	 */
	public Tree()
	{
		bean = new TreeUsageBean();
		
	}//Constructor	
	
	/**
	 * Tree Constructor
	 * @param Bean TreeUsageBean usage bean is set to that passed in
	 */
	public Tree(TreeUsageBean Bean)
	{
		bean = Bean;
		
	}//Constructor 

	/**
	 *	Writes Selected tree to a tcs file
	 *
	 *	@param String fileName - validated path, file to be displayed
	 *	@param String treeName - name of tree being displayed
	 */
	public void display(String fileName, String treeName)
	{
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, false);
			if(fileName == null)
				throw new Exception();
			
			//close old panels
			close();
			
			//show the tree, and allow it to be manipulated, etc...
			TreeAlgorithm talg = new TreeAlgorithm('r');
			graph_editing_window = new GraphWindow(true);			
			graph_editing_window.addAlgorithm(talg, "Tree Right");
			graph_editing_window.setTitle(Driver.title);
			graph_editing_window.pack();
			graph_editing_window.show();
			graph_editing_window.loadFile(fileName);
			graph_editing_window.treeName = treeName;
			graph_editing_window.treeRight();
			graph_editing_window.update(graph_editing_window.getGraphics());
			
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write TCS Tree.\n  Writing to file: "+ fileName);
		}
	}
	
	/**
	 *	Translates a tcs file into a nexus file, then opens that with gdfp
	 *
	 *	@param String fileName - validated path, tcs file to be opened
	 *	@param GeneralDNAFileParser gdfp - the general file parser
	 */
	public void open(String fileName, GeneralDNAFileParser gdfp)
	{
		try
		{
			//Method vars
			TaxaBean t;
			boolean pass = false;
			StringTokenizer st;
			Vector id = new Vector();
			Vector taxa = new Vector();
			Vector compare = new Vector();
			String thisLine = "", thisToken, name;
			
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, false);
			if(fileName == null)
				throw new Exception();
			
			//Translate tcs file
			BufferedReader inFile = new BufferedReader(new FileReader(fileName));
			
			//read through lines
			while(pass || (thisLine = inFile.readLine()) != null)
			{
				//set up loop - base case
				if(pass && thisLine == null)
					break;
				pass = false;

				//read through tokens
				st = new StringTokenizer(thisLine, " 	[]");
				while(st.hasMoreTokens())
				{
					//get next token
					thisToken = st.nextToken();
					
					//check for node
					if(thisToken.equalsIgnoreCase("node"))
					{
						thisLine = thisLine.toLowerCase();
						thisLine = getNodeInfo(taxa, thisLine.substring(thisLine.indexOf("node")+4), inFile);
						pass = true;
						break;
					}
					
					//check for edge
					else if(thisToken.equalsIgnoreCase("edge"))
					{
						thisLine = thisLine.toLowerCase();
						thisLine = getEdgeInfo(compare, thisLine.substring(thisLine.indexOf("edge")+4), inFile);
						pass = true;
						break;
					}
				}	
			}
			//close file
			inFile.close();

			//change data in taxa - remove id from name
			for(int i=0;i<taxa.size();i++)
			{
				t = (TaxaBean)taxa.elementAt(i);
				name = t.getName();
				id.add(name.substring(1, name.indexOf(")")));
				t.setName(name.substring(name.indexOf(")")+1));
			}

			//change data in compare - change id into name
			for(int i=0;i<compare.size();i++)
				compare.set(i, ((TaxaBean)taxa.elementAt(id.indexOf(compare.elementAt(i)))).getName());
			
			//write and open file
			writeFile(taxa, compare, gdfp);
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to open TCS Tree.\n  Opening to file: "+ fileName);
		}
	}		

	/**
	 *	Translates a tcs file into a nexus file, then opens that with gdfp
	 *
	 *	@param TreeBean tree - the tree to be used to create new tree
	 *	@param Vector compare - the ancestral tree for a given tree
	 *	@param GeneralDNAFileParser gdfp - the general file parser
	 */
	private void writeFile(Vector taxa, Vector compare, GeneralDNAFileParser gdfp)
	{
		//Method vars
		String fileName = "", relation;
		
		try
		{
			//get tree from compare
			relation = getRelationFromCompare(compare);
	
			//Validate path
			fileName = bean.getAbsoluteFilePath("./treesaap/Output/treeFromTCS.txt", true);
			if(fileName == null)
				throw new Exception();
			
			//write data to file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//print tree
			outFile.println("Tree tcsTree	[&U]	"+ relation +"\n");
			
			//print ancestral tree
			outFile.print("Ancestral_Tree:	");
			for(int i=0;i<compare.size();i+=2)
				outFile.print(compare.elementAt(i) +".."+ compare.elementAt(i+1) +"	");
			outFile.println("\n");
			
			//print taxa
			for(int i=0;i<taxa.size();i++)
				outFile.println(((TaxaBean)taxa.elementAt(i)).getName() +"	"+ ((TaxaBean)taxa.elementAt(i)).getSequence());
			
			//close file
			outFile.close();
			
			//open with gdfp
			gdfp.openNewFile(fileName);
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to open TCS Tree.\n  Opening to temp file: "+ fileName);
		}
	}
	
	/**
	 *	Creates a new tree as a subset of the given tree
	 *
	 *	@param Vector taxa - taxa read in from tcs file
	 *	@param String thisLine - the current line read in
	 *	@param GeneralDNAFileParser gdfp - the general file parser
	 */
	private String getNodeInfo(Vector taxa, String thisLine, BufferedReader inFile) throws Exception
	{
		//Method vars
		StringTokenizer st;
		boolean cont = true;
		String thisToken, sequence = "", name = "", id = "";

		//break out if end of file, or if specified
		while(thisLine != null)
		{
			//Tokenize the string
			st = new StringTokenizer(thisLine, " 	[]");
			
			//break out if encounter "node" or "edge"
			while(st.hasMoreTokens())
			{
				//get token
				thisToken = st.nextToken();
				if(thisToken.equalsIgnoreCase("node") || thisToken.equalsIgnoreCase("edge"))
				{
					cont = false;
					break;
				}
				//get sequence
				else if(thisToken.equalsIgnoreCase("sequence"))
				{
					if(st.hasMoreTokens())
						sequence = st.nextToken();
					else
					{
						st = new StringTokenizer((thisLine = inFile.readLine()), " 	[]");
						sequence = st.nextToken();
					}
				}
				//get id
				else if(thisToken.equalsIgnoreCase("id"))
				{
					if(st.hasMoreTokens())
						id = st.nextToken();
					else
					{
						st = new StringTokenizer((thisLine = inFile.readLine()), " 	[]");
						id = st.nextToken();
					}
				}
				//get label
				else if(thisToken.equalsIgnoreCase("label"))
				{
					if(st.hasMoreTokens())
						name = st.nextToken();
					else
					{
						st = new StringTokenizer((thisLine = inFile.readLine()), " 	[]");
						name = st.nextToken();
					}
				}
			}
			//break on not continue
			if(!cont)
				break;
			
			//next line
			thisLine = inFile.readLine();
		}
		//adjust values
		name = name.replaceAll("\"","");
		id = id.replaceAll("\"","");
		sequence = sequence.replaceAll("\"","");

		//add on new taxa, return thisLine
		taxa.add(new TaxaBean("("+ id +")"+ name, sequence));
		return thisLine;
	}		
	
	/**
	 *	Creates a new tree as a subset of the given tree
	 *
	 * @param Vector compare - the ancestral tree for a given tree
	 *	@param String thisLine - the current line read in
	 *	@param GeneralDNAFileParser gdfp - the general file parser
	 */
	private String getEdgeInfo(Vector compare, String thisLine, BufferedReader inFile) throws Exception
	{
		//Method vars
		StringTokenizer st;
		boolean cont = true;
		String thisToken, source = "", target = "";
		
		//break out if end of file, or if specified
		while(thisLine != null)
		{
			//Tokenize the string
			st = new StringTokenizer(thisLine, " 	[]");
			
			//break out if encounter "node" or "edge"
			while(st.hasMoreTokens())
			{
				//get token
				thisToken = st.nextToken();
				if(thisToken.equalsIgnoreCase("node") || thisToken.equalsIgnoreCase("edge"))
				{
					cont = false;
					break;
				}
				//get source
				else if(thisToken.equalsIgnoreCase("source"))
				{
					if(st.hasMoreTokens())
						source = st.nextToken();
					else
					{
						st = new StringTokenizer((thisLine = inFile.readLine()), " 	[]");
						source = st.nextToken();
					}
				}
				//get target
				else if(thisToken.equalsIgnoreCase("target"))
				{
					if(st.hasMoreTokens())
						target = st.nextToken();
					else
					{
						st = new StringTokenizer((thisLine = inFile.readLine()), " 	[]");
						target = st.nextToken();
					}
				}
			}			
			//break on not continue
			if(!cont)
				break;
			
			//next line
			thisLine = inFile.readLine();
		}
		//adjust values
		source = source.replaceAll("\"","");
		target = target.replaceAll("\"","");
		
		//add on new taxa, return thisLine
		compare.add(source);
		compare.add(target);
		return thisLine;
	}		

	/**
	 *	Creates a new tree as a subset of the given tree
	 *
	 *	@param TreeBean tree - the tree to be used to create new tree
	 *	@param GeneralDNAFileParser gdfp - the general file parser
	 */
	public TreeBean createSubTree(TreeBean tree, GeneralDNAFileParser gdfp)
	{
		try
		{
			//Method Vars
			Edge edge;
			Hashtable oldNodes = tree.getNodes();	

			//if graph is not yet open, return thisTree (i.e. no selected nodes)
			if(graph_editing_window == null || !graph_editing_window.treeName.equals(tree.getName()))
				throw new Exception();
			
			//set-up new Tree
			String name = tree.getName()+"_TCS";
			TreeBean newTree = new TreeBean(name, "", false, new Vector());
			newTree.setTaxaNames(new Vector());
			newTree.setNodeNames(new Vector());
			newTree.setNodes(new Hashtable());
			
			//get Edges
			Enumeration edges = graph_editing_window.applyAlgorithm_().getEdges();
			
			//if no nodes are selected, return thisTree to run
			if(edges == null)
				throw new Exception();
				
			//Set Comparison depending on selected nodes
			while(edges.hasMoreElements())
			{
				edge = (Edge)edges.nextElement();
				
				//only those nodes selected by user
				if(edge.selected)
				{
					placeInfo(edge.tail(), newTree, oldNodes);
					placeInfo(edge.head(), newTree, oldNodes);
				}
			}
			
			//verify something was selected
			if(newTree.getAncestralTree().size() == 0)
				throw new Exception();
			
			//build relation, from compare
			newTree.setRelation(getRelationFromCompare(newTree.getAncestralTree()));
			newTree.setRevisedRelation(newTree.getRelation());
			
			//add newTree to gdfp
			gdfp.getTrees().put(newTree.getName(), newTree);
			gdfp.getTreeNames().add(newTree.getName());
	
			return newTree;
		}
		catch(Exception e)
		{
			bean.errorMessage("There was a problem while creating TCS subset of tree: "+ tree.getName());
			return tree;
		}
	}		
	
	/**
	 * Places information from a given node into a given tree
	 *
	 *	@param Node node - node containing info to be placed in tree
	 *	@param TreeBean newTree - the tree to have info appended to it
	 *	@param Hashtable oldNodes - the hashtable containing TaxaBean nodes to be placed in new tree
	 */
	private void placeInfo(Node node, TreeBean newTree, Hashtable oldNodes)
	{		
		//Method vars
		String name = node.getLabel();;
		Vector compare = newTree.getAncestralTree();
		Vector taxaNames = newTree.getTaxaNames();
		Vector nodeNames = newTree.getNodeNames();
		Hashtable newNodes = newTree.getNodes();
		
		
		//Node = Taxa
		if(node.getShape() == node.RECTANGLE)
		{
			//make sure not in vector before adding it
			if(!taxaNames.contains(name))
				taxaNames.add(name);
			
			//place in compare vector
			compare.add(name);	
		}
		//Node = Ancestral Node
		else
		{
			//make sure not in vector before adding it
			if(!nodeNames.contains(name))
			{
				nodeNames.add(name);
				newNodes.put(name, oldNodes.get(name));
			}
			//place in compare vector
			compare.add(name);	
		}
	}//placeInfo 	
	
	/**
	 * Builds the relation of a tree from compare vector
	 * @param Vector compare - the ancestral tree for a given tree
	 */
	private String getRelationFromCompare(Vector compare)
	{
		//Method vars
		String relation = "";
		boolean unconnected = false;
		Vector nodes = new Vector();
		Vector results = new Vector();
		
		//go through compare - add one element of each occurance
		for(int i=0;i<compare.size();i+=2)
			if(!nodes.contains(compare.elementAt(i)))
				nodes.add(compare.elementAt(i));
		
		//go through nodes - add to taxa the taxa nodes point to
		for(int i=0;i<nodes.size();i++)
			results.add(getOccurances((String)nodes.elementAt(i), compare));
		
		//build the relations for each element in taxa
		for(int i=0;i<results.size();i++)
			collapseVector(i, results, nodes);
		
		//push instances of vector together
		for(int i=0;i<results.size();i++)
			if(!((String)results.elementAt(i)).equals(""))
			{
				//first occurance
				if(relation.equals(""))
					relation = (String)results.elementAt(i);
				
				//unconnected tree
				else
				{
					relation += ","+ results.elementAt(i);
					unconnected = true;
				}
			}

		//place parens
		if(unconnected)
			relation = "("+ relation +")";
		
		return relation;
	}//getRelationFromCompare 
	
	/**
	 * Returns a string of all right occurances of item of the compare vector
	 * @param String node - the item on the left
	 * @param Vector compare - the ancestral tree for a given tree
	 */
	public String getOccurances(String node, Vector compare)
	{
		//Method vars
		String replacement = "";
		
		//run through vector
		for(int i=0;i<compare.size();i+=2)
			//look for matches
			if(node.equals((String)compare.elementAt(i)))
				//add to replacement string
				replacement += (String)compare.elementAt(i+1) + ",";
	
		
		//only do something if replacement is not null
		if(!replacement.equals(""))
		{
			//remove last ","
			replacement = replacement.substring(0,replacement.length()-1);
			
			//add on parenthesis
			replacement = "("+ replacement +")";
		} 
		
		return replacement;
	}//getOccurances 
	
	/**
	 * Returns a string of all right occurances of item of the compare vector
	 * @param int index - the index where at in vector
	 * @param Vector results - the results to be utilized
	 * @param Vector nodes - vector of nodes that point to taxa in results
	 */
	public void collapseVector(int index, Vector results, Vector nodes)
	{
		//base case
		if(index >= results.size() || ((String)results.elementAt(index)).equals(""))
			return;
		
		//Method vars
		int n;
		String node, replacement;
		String tree = (String)results.elementAt(index);
		StringTokenizer st = new StringTokenizer(tree, ",() 	");
		
		//pull out string and parse it
		while(st.hasMoreTokens())
		{
			//get the next token
			node = st.nextToken();
			
			//see if node, else skip it
			if(nodes.contains(node))
			{
				//get index of node
				n = nodes.indexOf(node);
				
				//call collapse on that node
				collapseVector(n, results, nodes);
				
				//replace occurance in tree
				tree = tree.substring(0, tree.indexOf(node)) + results.elementAt(n) + tree.substring(tree.indexOf(node) + node.length());
				
				//wipe results vector for that node
				results.set(n, "");
			}
		}
		
		//set tree in vector
		results.set(index, tree);
	
	}//collapseVector 
	
	/**
	 * Close method - closes panel if open
	 */
	public void close()
	{
		//Make sure tcs windows has been opened
		if(graph_editing_window != null)
			graph_editing_window.dispose();
		
	}//close 
	
	/**
	 * Get method - returns TreeUsageBean bean
	 */
	public TreeUsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Set method - sets bean here and lower classes
	 *	@param Bean TreeUsageBean is the new bean 
	 */
	public void setBean(TreeUsageBean Bean)
	{
		bean = Bean;
	}//setBean 
	
}//Tree
