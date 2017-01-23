/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.GENBANKXML;

import java.io.IOException;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBeanManager;
import java.util.Vector;
import java.util.Date;

class GENBANKXMLReader extends DataConvert.Components.Reader
{	
	public GENBANKXMLReader(GENBANKXMLUsageBean bean)
	{
		setBean(bean);
	}//close GENBANKXML constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager GENBANKXMLManager = new TaxaBeanManager();
		GENBANKXMLManager.setFilename(filename);
		String title = "", line = getLine(), seq = "", gi = "", date = "";
		Date genDate = new Date();
		int from = 0, to = 0;
		boolean write = false;
		Vector CDSs = new Vector();
		while(line != null)
		{
			//Get CDS's
			if((line.indexOf("<GBFeature_key>") != -1) && (line.indexOf("CDS") != -1))
			{
				write = true;
			}//close if
			else if((line.indexOf("<GBInterval_from>") != -1) && write)
			{
				line = line.replaceAll("<GBInterval_from>","");
				line = line.replaceAll("</GBInterval_from>","");
				line = line.trim();
				from = Integer.parseInt(line);
			}//close else if
			else if((line.indexOf("<GBInterval_to>") != -1)&& write)
			{
				line = line.replaceAll("<GBInterval_to>","");
				line = line.replaceAll("</GBInterval_to>","");
				line = line.trim();
				to = Integer.parseInt(line);
			}//close else if
			else if((line.indexOf("<GBQualifier_value>") != -1) && (line.indexOf("GI") != -1) && write)
			{
				line = line.replaceAll("<GBQualifier_value>GI:","");
				line = line.replaceAll("</GBQualifier_value>","");
				line = line.trim();
				gi = line;
				CDSInfo temp = new CDSInfo(from, to, title, gi);
				CDSs.add(temp);
				write = false;
			}//close else if
			else if((line.indexOf("<GBInterval_accession>") != -1)&& write)
			{
				line = line.replaceAll("<GBInterval_accession>","");
				line = line.replaceAll("</GBInterval_accession>","");
				line = line.trim();
				title = line;
				if(from > to){
					int temp = from;
					from = to;
					to = temp;
				}//close if
			}//close else if
			else if(line.indexOf("<GBSeq_sequence>") != -1)
			{
				line = line.replaceAll("<GBSeq_sequence>","");
				line = line.replaceAll("</GBSeq_sequence>","");
				line = line.trim();	
				seq = line;
				for(int i = 0; i < CDSs.size(); i++)
				{
					CDSInfo temp = (CDSInfo)CDSs.elementAt(i);
					String subseq = seq.substring(temp.from, temp.to);
					String tempTitle = temp.title.concat(Integer.toString(i));
					GENBANKXMLManager.add(tempTitle, subseq, title, gi, genDate);
				}//close for
				gi = "";
				title = "";
				date = "";
				CDSs = new Vector();
			}//close else if
			else if(line.indexOf("<GBSeq_update-date>") != -1)
			{
				line = line.replaceAll("<GBSeq_update-date>","");
				line = line.replaceAll("</GBSeq_update-date>","");
				line = line.trim();	
				date = line;
				genDate = new Date(date);
			}//close else if
			
			line = getLine();
		}//close while
		return GENBANKXMLManager;
	}//close open method
	
	//finds the reverse compliment of a string
	private String compliment(String seq)
	{
		char [] sequence = seq.toCharArray();
		StringBuffer compliment = new StringBuffer();
		for(int i = seq.length() - 1; i >= 0; i--)
		{
			compliment.append(sequence[i]);
		}//close for
		return(compliment.toString());
	}//close compliment method
}//close GENBANKXMLReader class

class CDSInfo
{
	int from;
	int to;
	String title;
	String gi;
	
	CDSInfo (int From, int To, String Title, String GI)
	{
		from = From;
		to = To;
		title = Title;
		gi = GI;
	}//close CDSInfo constructor
}//CDSInfo class
