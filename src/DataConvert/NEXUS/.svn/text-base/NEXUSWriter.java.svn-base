/**
 *	NEXUS.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.NEXUS;

import java.io.IOException;
import java.util.Enumeration;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;

class NEXUSWriter extends DataConvert.Components.Writer
{	
	public NEXUSWriter(NEXUSUsageBean bean)
	{
		setBean(bean);
	}//close NEXUS constructor

	public void preconditions(TaxaBeanManager tbm) throws Exception
	{
		makeSameLength(tbm);
	}
	
	public void write(TaxaBeanManager tbm) throws IOException
	{
		NEXUSUsageBean bean = (NEXUSUsageBean)getBean();
		int interleaveSize = bean.getInterleaveSize();
		TaxaBean temp = tbm.get(0);
		String seq = temp.getSequence();
		int length = seq.length();
		String datatype = "PROTEIN";
		String type = "";
		if(interleaveSize <= 0){
			type = "noninterleave";
		}//close if
		else{
			type = "interleave";
		}//close else
		
		if((seq.indexOf("A") != -1) && (seq.indexOf("C") != -1) && (seq.indexOf("T") != -1) && (seq.indexOf("G") != -1)){
			datatype = "DNA";
		}//close if
		println("#Nexus"); 
		println("BEGIN DATA;");
		println("dimensions ntax=" + tbm.size() +  " nchar=" + length +";");
		println("format " + type + " datatype=" + datatype + " gap=- missing=? matchchar=.;");
		println("");
		println("matrix");

		
		
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
		println(";");
		println("End;");
	}//close write method
}//close NEXUSWriter class
