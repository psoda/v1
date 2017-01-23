/**
 *	CLUSTAL.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.CLUSTAL;

import java.io.IOException;
import java.util.Enumeration;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;

class CLUSTALWriter extends DataConvert.Components.Writer
{	
	public CLUSTALWriter(CLUSTALUsageBean bean)
	{
		setBean(bean);
	}//close CLUSTAL constructor

	public void preconditions(TaxaBeanManager tbm) throws Exception
	{
		makeSameLength(tbm);
	}
	
	public void write(TaxaBeanManager tbm) throws IOException
	{
		CLUSTALUsageBean bean = (CLUSTALUsageBean)getBean();
		int interleaveSize = bean.getInterleaveSize();
		println("CLUSTAL X (1.81) multiple sequence alignment");
		println("");
		
		TaxaBean temp = tbm.get(0);
		String seq = temp.getSequence();
		int length = seq.length();
		
		if(interleaveSize <= 0)
			for(int i=0;i<tbm.size();i++)
			{
				temp = tbm.get(i);
				print(temp.getName()+"	");
				printSeq(temp.getSequence(), true);
			}//close for
		else
		{
			int j = 0;
			while(j<length)
			{
				for(int i=0;i<tbm.size();i++)
				{
					temp = tbm.get(i);
					seq = temp.getSequence();
					print(temp.getName() + "	");
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
	
}//close CLUSTALReader class
