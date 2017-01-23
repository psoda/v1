/**
 *	CLUSTAL.class that reads in a Clustal file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */
package DataConvert.CLUSTAL;

import java.io.*;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBeanManager;

class CLUSTALReader extends DataConvert.Components.Reader
{	
	public CLUSTALReader(CLUSTALUsageBean bean)
	{
		setBean(bean);
	}//close CLUSTALK constructor
	
	public TaxaBeanManager open(String filename)throws IOException
	{
		//Method Vars
		TaxaBeanManager ClustalManager = new TaxaBeanManager();
		ClustalManager.setFilename(filename);
		String tok = "";
		if(getLine().toLowerCase().indexOf("clustal") != -1)
		{
			tok = getToken();
			while(tok != null)
			{			
				StringTokenizer st = new StringTokenizer(tok, ".:* 	");
				if(st.hasMoreTokens())
					ClustalManager.add(tok, restOfLine());

				tok = getToken();
			}
		}
		
		ClustalManager.finish("?","-",".");
		return ClustalManager;
	}//close open method
}//close CLUSTALReader class
