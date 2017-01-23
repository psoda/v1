/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.RST;

import java.io.IOException;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBeanManager;

class RSTReader extends DataConvert.Components.Reader
{	
	public RSTReader(RSTUsageBean bean)
	{
		setBean(bean);
	}//close RST constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager RSTManager = new TaxaBeanManager();
		RSTManager.setFilename(filename);
		String line = getLine();
		StringBuffer seq = new StringBuffer();
		StringBuffer title = new StringBuffer();
		
		while(line != null)
		{
			if(line.indexOf("List of extant and reconstructed sequences") != -1)
			{
				String tok = getToken();
				while(!tok.equals("Overall"))
				{
					if(!tok.equals("")){
						title.append(tok);
						if(tok.equals("node"))
						{
							title.append(getToken());
							seq.append(restOfLine() + "\n");
						}//close if
						else
						{
							seq.append(restOfLine() + "\n");
						}//close else
						tok = getToken();
						RSTManager.add(title.toString(),seq.toString());
						seq = new StringBuffer();
						title = new StringBuffer();
					}//close if
				}//close while
			}//close if
			line = getLine();
		}//close while
		return RSTManager;
	}//close open method
	
}//close RSTReader class
