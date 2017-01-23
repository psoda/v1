/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.FASTA;

import java.io.IOException;
import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;

class FASTAWriter extends DataConvert.Components.Writer
{	
	public FASTAWriter(FASTAUsageBean bean)
	{
		setBean(bean);
	}//close FASTA constructor
	
	public void write(TaxaBeanManager tbm) throws IOException
	{
		FASTAUsageBean bean = (FASTAUsageBean)getBean();
		int interleaveSize = bean.getInterleaveSize();
		
		for(int i=0;i<tbm.size();i++)
		{
			TaxaBean temp = tbm.get(i);
			String seq = temp.getSequence();
			int length = seq.length();
			println(">"+temp.getName()+"	");
			
			if(interleaveSize <= 0)
				printSeq(seq, true);
			else
			{
				for(int j=0;j<length;j+=interleaveSize)
				{
					if(j + interleaveSize <= length)
						printSeq(seq.substring(j, j+interleaveSize), true);
					else
						printSeq(seq.substring(j), true);
				}//close for
			}
		}
	}//close write method

}//close FASTAReader class
