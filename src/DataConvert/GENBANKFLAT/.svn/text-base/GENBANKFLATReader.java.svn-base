/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.GENBANKFLAT;

import java.io.IOException;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBeanManager;
import java.util.Date;
import java.util.Vector;

class GENBANKFLATReader extends DataConvert.Components.Reader
{	
	public GENBANKFLATReader(GENBANKFLATUsageBean bean)
	{
		setBean(bean);
	}//close GENBANKFLAT constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		TaxaBeanManager GENBANKFLATManager = new TaxaBeanManager();
		GENBANKFLATManager.setFilename(filename);
		String tok = getToken(), accession = "", gi = "", line = "";
		StringTokenizer strtok;
		StringBuffer seq;
		Date gendate = null;
		Vector cdsbeans = new Vector();
		while(tok != null)
		{
			if(tok.equals("LOCUS"))
			{
				line = restOfLine();
				strtok = new StringTokenizer(line);
				int toks = strtok.countTokens();
				for (int i = 0; i < toks; i++){
					tok = strtok.nextToken();
				}//close for
				gendate = new Date(tok);
			}//close if
			else if(tok.equals("VERSION"))
			{
				line = restOfLine();
				strtok = new StringTokenizer(line);
				if(strtok.countTokens() == 2){
					accession = strtok.nextToken();
					gi = strtok.nextToken();
					gi = gi.replaceAll("GI:","");
					gi = gi.trim();
				}//close if
				else {
					//what if there is not acc or gi
				}//close else
			}//close else if
			else if(tok.equals("CDS"))
			{
				String nucs = getToken();
				char [] tempnucs = nucs.toCharArray();
				String start = "";
				String finish = "";
				boolean write = true;
				for(int i = 0; i < nucs.length(); i++){
					if(Character.isDigit(tempnucs[i]) && write)
					{
						start = start.concat(Character.toString(tempnucs[i]));
					}//close if
					else if(Character.isDigit(tempnucs[i]) && !write)
					{
						finish = finish.concat(Character.toString(tempnucs[i]));
					}//close if
					if(!Character.isDigit(tempnucs[i]))
					{
						write = false;
					}//close if
				}//close for
				
				String cdstitle = getToken();
				cdstitle = cdstitle.replaceAll("/gene=\"", "").replaceAll("\"","").trim();
				CDSInfo temp = new CDSInfo(Integer.parseInt(start), Integer.parseInt(finish), cdstitle);
				cdsbeans.add(temp);
			}//close else if
			else if(tok.equals("ORIGIN"))
			{
				seq = new StringBuffer();
				tok = getToken();
				while(!tok.equals("//"))
				{
					boolean num = true;
					try{
						Integer.parseInt(tok);
					} catch (Exception e){
						num = false;	
					}//close try/catch
					
					if(num)
					{
						seq = seq.append(restOfLine());	
					}//close if
					
					tok = getToken();
				}//close while
				String tempseq = seq.toString().replaceAll(" ", "").trim();
				
				for(int i = 0; i < cdsbeans.size(); i++)
				{
					CDSInfo temp = (CDSInfo)cdsbeans.elementAt(i);
					String subseq = tempseq.substring(temp.from, temp.to);
//					System.out.println("Title: " + temp.title + " Seq: " + subseq);
//					System.out.println("Acc: " + accession + " GI: " + gi + " Date: " + gendate.toString());
					String temptitle = temp.title.concat(Integer.toString(i));
					GENBANKFLATManager.add(temptitle, subseq, accession, gi, gendate);
				}//close for
			}//close else if

			tok  = getToken();
		}//close while
//		System.out.println("Size: " + GENBANKFLATManager.size());
		return GENBANKFLATManager;
	}//close open method
}//close GENBANKFLATReader class

class CDSInfo
{
	int from;
	int to;
	String title;
	
	CDSInfo (int From, int To, String Title)
	{
		from = From;
		to = To;
		title = Title;
	}//close CDSInfo constructor
}//CDSInfo class
