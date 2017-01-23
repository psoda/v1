/**
 *	PHYLIP.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.PHYLIP;

import java.io.IOException;
import java.util.Enumeration;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;

class PHYLIPWriter extends DataConvert.Components.Writer
{	
	public PHYLIPWriter(PHYLIPUsageBean bean)
	{
		setBean(bean);
	}//close PHYLIP constructor
	
	public void preconditions(TaxaBeanManager tbm) throws Exception
	{
		setNameLength(tbm, 10);
		makeSameLength(tbm);
	}
	
	public void write(TaxaBeanManager tbm) throws IOException
	{
		PHYLIPUsageBean bean = (PHYLIPUsageBean)getBean();
		int interleaveSize = bean.getInterleaveSize();
		TaxaBean temp = tbm.get(0);
		String seq = temp.getSequence();
		int length = seq.length();

		//Number of taxa, then size of taxa
		println("	"+ tbm.size() +"	"+ length);

		
		if(interleaveSize <= 0)
			for(int i=0;i<tbm.size();i++)
			{
				temp = tbm.get(i);
				print(temp.getName());
				printSeq(temp.getSequence(), true);
			}//close for
		else
		{
			int j = 0;
			boolean first = true;
			while(j<length)
			{
				for(int i=0;i<tbm.size();i++)
				{
					temp = tbm.get(i);
					seq = temp.getSequence();
					if(first)
					{
						print(temp.getName());
						if(i == tbm.size() -1)
							first = false;
					}
					else
						print("          ");
						
					if(j + interleaveSize <= length)
						printSeq(seq.substring(j, j+interleaveSize), true);
					else
						printSeq(seq.substring(j), true);
				}//close for
				j+=interleaveSize;
				println("");
			}//close while
		}
	}//close write method

}//close PHYLIPWriter class
