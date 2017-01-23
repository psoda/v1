/**
 *	TaxaControl.class is responsible for the correct interpretation
 *	of taxa read in from files.  It will also handle nodes and the 
 *	case of nodes being taxa.  It will utilize TaxaBean to store all
 *	data it generates.
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

class TaxaControl
{
	//Shared Objects
	private DNAFileParserUsageBean bean;	//UsageBean to identify user selections
	private String lastLine;				//the last line read in from a given file pointer
	
	//Shared Class Data objects
	private Vector taxaNames;		//Names of the taxa read in
	private Vector nodeNames;		//Names of the nodes read in
	private Vector nodes;			//Contains all the nodes
	private Hashtable taxa;			//Contains all the taxa
	
	//Specific Class Vars
	private TaxaBean tb;			//A recently created TaxaBean object
	
	//DNA data chars
	Vector sequenceChar;			//Vector of the chars representing DNA data
	Vector weakSequenceChar;
	private char sequenceChars[] = {'A','G','T','C','U','N','-','a','g','t','c','u','n','?'};
	private char weakSequenceChars[] = {'R','Y','M','K','S','W','H','B','V','D','r','y','m','k','s','w','h','b','v','d'};

	
	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * calls method to initialize variables to be used.
	 *
	 */
	public TaxaControl(DNAFileParserUsageBean usage)
	{
		bean = usage;
		initialize();
		
		//Set-up Analysis Data
		sequenceChar = new Vector();
		for(int i=0;i<sequenceChars.length;i++)
			sequenceChar.add(sequenceChars[i]+"");
			
		weakSequenceChar = new Vector();
		for(int i=0;i<weakSequenceChars.length;i++)
			weakSequenceChar.add(weakSequenceChars[i]+"");
		
	}//Constructor

	/**
	 * Initializes data structures that contain taxa info
	 */
	public void initialize()
	{
		taxaNames = new Vector();
		nodeNames = new Vector();
		taxa = new Hashtable();
		nodes = new Vector();
	
	}//initialize
	
	/**
	 * Is given a current pointer to the file being parsed and the line
	 * that file is currently on.  It will then find the node and get it's 
	 * associated sequence and return the file pointer
	 * 
	 * @param BufferedReader inFile Contains the file that is being parsed
	 * @param String currentLine The Line last read in from file
	 * @param boolean rst Whether the file is an rst file or not, may take much longer to parse than necessary if not set
	 */
	public BufferedReader obtainNode(BufferedReader inFile, String currentLine, boolean rst)
	{
		//Method vars
		StringTokenizer st;		//tokenizes thisLine
		String temp;			//for evaluating the next token
		
		//Object vars
		String nodeName = "";	//the node's name
		String nodeSeq = "";	//the node's sequence

		//set last line to empty string
		lastLine = "";
		
		try
		{		
			//tokenize string
			st = new StringTokenizer(currentLine, "!@$%^&*<>?/\\|~`+=\" 	");

			//skip to the word containing node
			while(st.hasMoreTokens())
			{
				nodeName = st.nextToken();
				if(nodeName.toLowerCase().indexOf("node") != -1)
					break;
			}
			
			//if no more tokens, scan till find more
			while(!st.hasMoreTokens())
				st = getMoreTokens(inFile);
			
			//check to see whether the next token is a sequence or not 
			temp = st.nextToken();
			
			//temp is beginning of sequence
			if(isSequence(temp))
				nodeSeq += temp.toUpperCase();
			
			//temp is part of name
			else 
				nodeName += temp;
			
			//Look for RST file - take that into account
			if(rst && isSequence(st.nextToken()))
			{
				nodeSeq = currentLine.substring(currentLine.indexOf("node") + 15);
				nodeSeq = nodeSeq.replaceAll(" ", "");
			}
			
			else
			{
				//build sequence by adding all valid tokens, until the first invalid token
				while(st.hasMoreTokens())
				{
					temp = st.nextToken();
					
					//add to nodeSeq if sequence
					if(isSequence(temp))
						nodeSeq += temp.toUpperCase();
					
					//break out of iteration
					else
						break;
					
					//if no more tokens, and still valid sequences, get next line of tokens
					if(!st.hasMoreTokens())
						st = getMoreTokens(inFile);
				}
			}

			//verify at least some existant values for data structs
			if(!nodeName.equals("") && !nodeSeq.equals(""))
			{
				//create new TaxaBean object and place in dataStructure
				tb = new TaxaBean(nodeName, nodeSeq);
				nodeNames.add(nodeName);
				nodes.add(tb);
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
	}//obtainNodes

	/**
	 * Is given a current pointer to the file being parsed and the line
	 * that file is currently on and the last non sequence token observed.
	 * It will then find the taxa name and then get it's 
	 * associated sequence and return the file pointer
	 * 
	 * @param BufferedReader inFile Contains the file that is being parsed
	 * @param String currentLine The Line last read in from file
	 * @param String lastToken The last non-sequence token observed - possibly the taxa's name
	 * @param boolean rst Whether the file is an rst file or not, may take much longer to parse than necessary if not set
	 */
	public BufferedReader obtainTaxa(BufferedReader inFile, String currentLine, String lastToken, boolean rst)
	{
		//Method vars
		StringTokenizer st;		//tokenizes thisLine
		String temp;			//for evaluating the next token
		
		//Object vars
		String taxaName = "";	//the taxa's name
		String taxaSeq = "";	//the taxa's sequence
		
		//set last line to empty string
		lastLine = "";
		
		try
		{					
			//tokenize string
			st = new StringTokenizer(currentLine, "!@$%^&*<>?/\\|~`+=\" 	");
			
			//skip to the word containing name of taxa (just before the sequences)
			while(st.hasMoreTokens())
			{
				taxaName = st.nextToken();
				if(!isSequence(taxaName))
					lastToken = taxaName;
				else
				{
					taxaSeq += taxaName.toUpperCase();
					break;
				}
			}
		
			//assign the lastToken read to taxaName
			taxaName = lastToken;
			
			//Look for RST file - take that into account
			if(rst && isSequence(st.nextToken()) && isSequence(st.nextToken())  && isSequence(st.nextToken()))
			{
				taxaSeq = currentLine.substring(currentLine.indexOf(taxaName) + taxaName.length());	
				taxaSeq = taxaSeq.replaceAll(" ", "");
			}
			
			else
			{
				//build sequence by adding all valid tokens, until the first invalid token
				while(st.hasMoreTokens())
				{
					temp = st.nextToken();
					
					//add to nodeSeq if sequence
					if(isSequence(temp))
						taxaSeq += temp.toUpperCase();
					
					//break out of iteration
					else
						break;
					
					//if no more tokens, and still valid sequences, get next line of tokens
					if(!st.hasMoreTokens())
						st = getMoreTokens(inFile);
				}
			}
				
			//verify at least some existant values for data structs
			if(!taxaName.equals("") && !taxaSeq.equals(""))
			{
				//this taxa has already been read in, append to it
				if(taxaNames.contains(taxaName))
				{
					tb = (TaxaBean)taxa.get(taxaName);
					tb.setSequence(tb.getSequence() + taxaSeq);
				}
				
				else
				{
					//create new TaxaBean object and place in dataStructures
					tb = new TaxaBean(taxaName, taxaSeq);
					taxaNames.add(taxaName);
					taxa.put(taxaName, tb);
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
	}//obtainTaxa
	
	/**
	 * Scans data in taxa and validates them based on 
	 * length and the parseSequence method
	 * 
	 */
	public void validateTaxa()
	{
		//Value to increment Tracker
		int length;
		
		//go through all the taxa
		for(int i=0;i<taxaNames.size();i++)
		{
			tb = (TaxaBean)taxa.get((String)taxaNames.elementAt(i));
			length = tb.getSequence().length();
			
			//A sequence should not be less than 5 codons all together
			if(tb.getSequence().length() <= 5*3)
			{
				taxa.remove(tb.getName());
				taxaNames.remove(i);
				i--;
			}
			//Parse the sequence setting up the codon array in each taxa bean that has just been read in from file
			else if(tb.getCodon() == null)
			{
				parseSequence(tb);
				
				if(tb.getSequence() == null)
				{
					taxa.remove(tb.getName());
					taxaNames.remove(i);
					i--;
				}
			}
			
			//increment tracking variable
			bean.setDataParsed(bean.getDataParsed() + length);
		}
	}//validateTaxa
	
	/**
	 * Scans data in nodes and validates them based on 
	 * length and the parseSequence method
	 *
	 */
	public void validateNodes()
	{
		//Value to increment Tracker
		int length;
		
		//go through all the nodes
		for(int i=0;i<nodes.size();i++)
		{
			tb = (TaxaBean)nodes.elementAt(i);
			length = tb.getSequence().length();
			
			//A sequence should not be less than 5 codons all together
			if(tb.getSequence().length() <= 5*3)
			{
				nodes.remove(i);
				nodeNames.remove(i);
				i--;
			}
			//Parse the sequence setting up the codon array in each taxa bean
			else
			{
				parseSequence(tb);
				
				if(tb.getSequence() == null)
				{
					nodes.remove(i);
					nodeNames.remove(i);
					i--;
				}
			}
			
			//increment tracking variable
			bean.setDataParsed(bean.getDataParsed() + length);
		}
	}//validateNodes
	
	/*
	 * This method parses a given sequence from a TaxaBean,
	 * removing all white spaces, and placing codon numbers in 
	 * the codon[] array
	 *
	 * @param TaxaBean tb taxa bean's sequence to be parsed
	 */
	private void parseSequence(TaxaBean tb)
	{
		//Method vars
		String sequence = tb.getSequence() + " ";	//must end string with a space as not to leave off last codons
		String temp = "";							//temporary string that contains new sequence
		
		boolean mRNA = false;						//set-up to see if using U instead of T
		boolean ambig = false;						//variable to represent if codon is ambiguous or not
		boolean unknown = false;					//whether there has been an ambiguous codon encountered
		
		int pos = 0;								//represents which position that character is in, in a given codon
		int sum = 0;								//represents unique number for that given codon
		int num = 0;								//number representing last non-white space
		int codonNum = 0;							//represents which codon is being summed

		//set-up codon var
		int length = sequence.length()/3;			//temporary var to determine length of codon object
		
		//error checking
		if(length < 1) 
			length = 1;
		
		//set codon array to correct size
		int codon[] = new int[length];	
		
		//go through sequence character by character
		for(int i=0;i<sequence.length();i++)
		{
			//get a value for each nucleotide
			switch(sequence.charAt(i))
			{
				//T adds nothing to sum
			case 'T':	
				pos++;
				break;
				
				//U adds nothing to sum
			case 'U':
				mRNA = true;
				pos++;
				break;
				
				//C adds 1,4, and 16 to sum depending on position
			case 'C': 
				switch(pos)
				{
				case 0:	sum += 16;
					break;
				case 1:	sum += 4;
					break;
				case 2:	sum += 1;
					break;
				}
				pos++;
				break;
				
				//A adds 2,8, and 32 to sum depending on position
			case 'A':	
				switch(pos)
				{
				case 0:	sum += 32;
					break;
				case 1:	sum += 8;
					break;
				case 2:	sum += 2;
					break;
				}
				pos++;
				break;
				
				//G adds 3,12, and 48 to sum depending on position
			case 'G':	
				switch(pos)
				{
				case 0:	sum += 48;
					break;
				case 1:	sum += 12;
					break;
				case 2:	sum += 3;
					break;
				}
				pos++;
				break;
				
				//ignore blank spaces - remove them from the String sequence	
			case ' ':
				temp += sequence.substring(num, i);
				num = i+1;
				break;
				
				//all else fails, it is an ambiguous codon
			default:
				if(!sequenceChar.contains(sequence.charAt(i)+"") && !weakSequenceChar.contains(sequence.charAt(i)+""))
				{
					tb.setCodon(null);
					tb.setSequence("");
					return;
				}	
				
				ambig = true;
				pos++;	
				break;
			}
			
			//reached end of codon
			if(pos == 3)
			{	
				//set in codon number
				if(!ambig)	
					codon[codonNum] = sum;

				else				
				{
					//lets user know there is an unknown
					unknown = true;	
					
					//set codon to ambiguous
					codon[codonNum] = -1;			
					
					//reset ambiguous variable
					ambig = false;						
				}
				
				//increment to next codon, then reset position & codon number
				codonNum++;			
				pos = 0;
				sum = 0;
			}	
		}
		
		//replace all occurrences of U
		if(mRNA)	
			temp.replace('U','T');
		
		//if there was an ambiguous codon, scan to make sure they are not all ambiguous
		if(unknown)
		{
			int pointer = 0;
			
			//scan for any non-ambiguous codons
			for(;pointer<length;pointer++)
				if(codon[pointer] != -1)
					break;
			
			//reached end of array w/o any valid codons
			if(pointer >= length-1)
			{
				tb.setSequence(null);
				return;
			}
		}
		
		//set taxa bean, return it
		tb.setCodon(codon);
		tb.setSequence(temp);
		
	}//parseSequence

	/**
	 * Determines whether given String is a Sequence or not
	 *
	 * @param String word  the string to be analyzed
	 */
	public boolean isSequence(String word)
	{	
		//Usage Vars
		boolean allCaps = bean.getAllCaps();
		double evenHanded = bean.getEvenHanded();
		
		//method vars
		int sequenceCharCount = 0;
		int weakSequenceCharCount = 0;
		char evalChar;
		boolean seqChar;
		
		//number of characters to check
		int length = word.length();
		
		//Only check first 20 characters - make special case for smaller sequences
		if(length > 20)
			length = 20;
		
		//go through each character and evaluate it
		for(int i=0;i<length;i++)
		{
			evalChar = word.charAt(i);
			seqChar = false;
			
			//break on lower case
			if(Character.isLowerCase(evalChar) && allCaps)
				return false;

			//check for nucleotide
			if(sequenceChar.contains(evalChar+""))
			{
				sequenceCharCount++;
				seqChar = true;
			}
			
			//check to see if less common unknown symbols
			else if(weakSequenceChar.contains(evalChar+""))
			{
				weakSequenceCharCount++;
				seqChar = true;
			}
			
			//otherwise return false
			if(!seqChar)
				return false;
			
		}
		
		if(weakSequenceCharCount != 0)
			if((double)weakSequenceCharCount/sequenceCharCount > evenHanded)
				return false;
		
		return true;
	}//isSequence

	/**
	 * Returns a Tokenized String of the next line of the file if
	 * it exists.  This code is referenced for both obtainNode and 
	 * obtainTaxa.
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
	 * Returns the lastLine variable.
	 * This allows super class to obtain the last line 
	 * read from file while in this class
	 */
	public String getLastLine()
	{
		return lastLine;
	}
	
	/**
	 * Returns the taxa hashtable.
	 * This allows super class to obtain 
	 * the data read from file.
	 */
	public Hashtable getTaxa()
	{
		return taxa;
	}
	
	/**
	 * Returns the taxa vector.
	 * This allows super class to obtain 
	 * the names of taxa read from file.
	 */
	public Vector getTaxaNames()
	{
		return taxaNames;
	}
	
	/**
	 * Returns the nodes vector.
	 * This allows super class to obtain 
	 * the data read from file.
	 */
	public Vector getNodes()
	{
		return nodes;
	}
	
	/**
	 * Returns the nodes vector.
	 * This allows super class to obtain 
	 * the names of nodes read from file.
	 */
	public Vector getNodeNames()
	{
		return nodeNames;
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
		int verbose = bean.getGdfpVerbose();
		
		if(verbose <= 0 && tb != null)
		{
			bean.verboseMessage(tb.getName() +": "+ tb.getSequence());
		}
	}//verbose

}//TaxaControl
