/**
 *	NEXUS.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.NEXUS;

import java.io.*;
import java.util.*;
import java.lang.*;
import java.util.StringTokenizer;

import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;

class NEXUSBlockReader extends DataConvert.Components.Reader
{	
	public NEXUSBlockReader(NEXUSUsageBean bean)
	{
		setBean(bean);
	}//close NEXUS constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager NexusManager = new TaxaBeanManager();
		String tok = "", datatype ="", gap = "-", missing = "?", matchchar = "."; 
		int nchar = 0, ntax = 0; 

		while(tok != null)
		{
			String temp = tok.toLowerCase();
			if(temp.indexOf("nchar") != -1)
			{
				nchar = Integer.parseInt(retValue(tok));
				tok = getToken();
			}//close if
			else if(temp.indexOf("ntax") != -1)
			{
				ntax = Integer.parseInt(retValue(tok));
				tok = getToken();
			}//close else if
			else if(temp.indexOf("datatype") != -1)
			{
				datatype = retValue(tok);
				tok = getToken();
			}//close else if
			else if(temp.indexOf("gap") != -1)
			{
				gap = retValue(tok);
				tok = getToken();
			}//close else if
			else if(temp.indexOf("missing") != -1)
			{
				missing = retValue(tok);
				tok = getToken();
			}//close else if
			else if(temp.indexOf("matchchar") != -1)
			{
				matchchar = retValue(tok);
				tok = getToken();
			}//close else if
			else if(temp.indexOf("matrix") != -1)
			{	
				//check nchar
				if(nchar == 0)
					return null;
				
				String title = ""; 
				tok = getToken();
				StringBuffer seq = new StringBuffer();
				while(tok.indexOf(";") == -1)
				{				
					title = tok;
					seq = new StringBuffer(restOfLine().replaceAll(" ",""));
					
//					System.out.println(seq+":");
					
					//Get rest of sequence
					while((tok = getToken()) != null && tok.indexOf(";") == -1 && seq.toString().length() < nchar)
						seq.append(tok);
					
//					System.out.println(nchar +" "+ seq.toString().length());
					
					//Error check
					if(seq.toString().length() > nchar)
						return null;
						
					NexusManager.add(title, seq.toString());
				}//close while
				
				tok = null;
			}//close else if
			else {
				tok = getToken();
			}//close else
		}//close while
		
		
		//System.out.println("nchar: " + nchar + " ntax: " + ntax + " datatype: " + datatype + " gap: " + gap + " missing: " + missing + " matchchar: " + matchchar);
		NexusManager.setFilename(filename);
		NexusManager.finish(missing,gap,matchchar);
		return NexusManager;
	}//close open method

	private String retValue(String tok){
		char [] temp = tok.toCharArray();
		boolean write = false;
		StringBuffer retString = new StringBuffer();
		for(int i = 0; i < tok.length(); i++){
			if(temp[i] == '='){
				write = true;
			}//close if
			else if(temp[i] == ';'){
				write = false;
			}//close else if
			else if(write){
				retString.append(Character.toString(temp[i]));
			}//close else if 
		}
		return (retString.toString());
	}//close retValue method

}//close NEXUSReader class
