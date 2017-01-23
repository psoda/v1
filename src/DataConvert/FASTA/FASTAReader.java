/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.FASTA;

import java.io.IOException;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBeanManager;

class FASTAReader extends DataConvert.Components.Reader
{	
	public FASTAReader(FASTAUsageBean bean)
	{
		setBean(bean);
	}//close FASTA constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager FastaManager = new TaxaBeanManager();
		FastaManager.setFilename(filename);
		String title = "", tok = "";
		StringBuffer seq = new StringBuffer();
		
		while(tok != null)
		{
			if(tok.indexOf(">") != -1)
			{
				if(((FASTAUsageBean)getBean()).getHard_return())
					tok += restOfLine();
				title = tok.replaceAll(">","").trim();
				tok = getToken();
				while(tok != null && tok.indexOf(">") == -1)
				{
					seq.append(tok);
					tok = getToken();
				}//close while
				
				FastaManager.add(title, seq.toString());
				title = "";
				seq = new StringBuffer();
			}//close if
			else{
				tok = getToken();
			}//close else	
		}//close while
		
		FastaManager.finish("?","-",".");
		return FastaManager;
	}//close open method

}//close FASTAReader class
