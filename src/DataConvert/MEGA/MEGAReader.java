/**
 *	MEGA.class that reads in a Mega file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */
package DataConvert.MEGA;

import java.io.*;
import java.util.*;
import java.lang.*;
import java.util.StringTokenizer;

import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;

class MEGAReader extends DataConvert.Components.Reader{
	public MEGAReader(MEGAUsageBean bean)
	{
		setBean(bean);
	}//close MEGA constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager MegaManager = new TaxaBeanManager();
		MegaManager.setFilename(filename);
		String title = "", tok = "";
		StringBuffer seq = new StringBuffer();
		
		while(tok != null)
		{
			if(tok.indexOf("#") != -1 && tok.indexOf("mega") == -1 && tok.toLowerCase().indexOf("nexus") == -1)
			{
				if(((MEGAUsageBean)getBean()).getHard_return())
					tok += restOfLine();
				title = tok.replaceAll("#","").trim();
				tok = getToken();
				while(tok != null && tok.indexOf("#") == -1)
				{
					seq.append(tok);
					tok = getToken();
				}//close while
				
				MegaManager.add(title, seq.toString());
				title = "";
				seq = new StringBuffer();
			}//close if
			else{
				tok = getToken();
			}//close else	
		}//close while
		
		MegaManager.finish("?","-",".");
		return MegaManager;
	}//close open method
	
}//close MEGAReader class
