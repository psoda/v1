/**
 *	PHYLIP.class that reads in a PHYLIP file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.PHYLIP;

import java.io.IOException;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBeanManager;

class PHYLIPReader extends DataConvert.Components.Reader
{	
	public PHYLIPReader(PHYLIPUsageBean bean)
	{
		setBean(bean);
	}//close PHYLIP constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager PHYLIPManager = new TaxaBeanManager();
		PHYLIPManager.setFilename(filename);
		String title = "", tok = "";
		StringBuffer seq = new StringBuffer();
		int numOfSeq = Integer.parseInt(getToken());
		int numOfNuc = Integer.parseInt(getToken());
		String[] titles = new String[numOfSeq];
		
		for(int i = 0; i < numOfSeq; i++)
		{
			titles[i] = getToken();
			seq = seq.append(restOfLine());
			PHYLIPManager.add(titles[i], seq.toString());
			seq = new StringBuffer();
		}
		
		int i = 0;
		String line = getLine();
		while(line != null && !line.equals(""))
		{
			title = titles[i];
			PHYLIPManager.add(title, line);
			line = getLine();
			i++;
			if(i == numOfSeq)
				i = 0;
		}
		PHYLIPManager.finish("?","-",".");
		return PHYLIPManager;
	}//close open method

}//close PHYLIPReader class
