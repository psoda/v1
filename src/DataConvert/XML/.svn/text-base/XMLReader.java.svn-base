/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.XML;

import java.io.IOException;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBeanManager;

class XMLReader extends DataConvert.Components.Reader
{	
	public XMLReader(XMLUsageBean bean)
	{
		setBean(bean);
	}//close XML constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager XMLManager = new TaxaBeanManager();
		XMLManager.setFilename(filename);
		String title = "", line = getLine(), seq = "";
		while(line != null)
		{
			if(line.indexOf("<Seqdesc_title>") != -1)
			{
				title = line;
				title = title.replaceAll("<Seqdesc_title>","");
				title = title.replaceAll("</Seqdesc_title>","");
				title = title.trim();
				title = title.replaceAll(" ","_");
			}//close if
			else if(line.indexOf("<IUPACna>") != -1)
			{
				seq = line;
				seq = seq.replaceAll("<IUPACna>","");
				seq = seq.replaceAll("</IUPACna>","");
				seq = seq.trim();
				seq = seq.replaceAll(" ","_");
				
				XMLManager.add(title, seq);
			}//close else if
			line  = getLine();
		}//close while
		
		return XMLManager;
	}//close open method
}//close XMLReader class
