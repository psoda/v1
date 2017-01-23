/**
 *	MEGA.class that reads in a Mega file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */
package DataConvert.MEGA;

import java.io.IOException;
import java.util.Enumeration;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;

class MEGAWriter extends DataConvert.Components.Writer
{	
	public MEGAWriter(MEGAUsageBean bean)
	{
		setBean(bean);
	}//close MEGA constructor
	
	public void write(TaxaBeanManager tbm) throws IOException
	{
		MEGAUsageBean bean = (MEGAUsageBean)getBean();
		int interleaveSize = bean.getInterleaveSize();
		println("#mega");
		println("!Format indel=-;");
		println("");
		
		for(int i=0;i<tbm.size();i++)
		{
			TaxaBean temp = tbm.get(i);
			String seq = temp.getSequence();
			int length = seq.length();
			println("#"+temp.getName()+"	");
			
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
	
}//close MEGAReader class
