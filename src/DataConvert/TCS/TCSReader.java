/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.TCS;

import java.io.IOException;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBeanManager;

class TCSReader extends DataConvert.Components.Reader
{	
	public TCSReader(TCSUsageBean bean)
	{
		setBean(bean);
	}//close TCS constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager TCSManager = new TaxaBeanManager();
		TCSManager.setFilename(filename);
		String tok = getToken(), title = "", seq = "";
		while(!tok.equals("edge"))
		{
			if(tok.equals("Sequence"))
			{
				seq = restOfLine();
				seq = seq.replaceAll("\"", "");
			}//close if
			else if(tok.equals("label"))
			{
				title = restOfLine();
				title = title.replaceAll("\"", "");
				TCSManager.add(title, seq);
			}//close else if
			tok = getToken();
		}//close while
		return TCSManager;
	}//close open method
	
}//close TCSReader class
