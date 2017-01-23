/**
 *	Write.java from the Components jar, contains shared components of all
 *	the writers in each extended component class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */
package DataConvert.Components;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.IOException;

import DataConvert.Objects.TaxaBeanManager;
import DataConvert.Objects.TaxaBean;

public class Writer
{
	private UsageBean bean;
	private PrintWriter out;
	
	public boolean writeInit(String path, TaxaBeanManager TBM)throws IOException
	{
		try
		{
			//Writer Conditions
			TaxaBeanManager tbm = new TaxaBeanManager();
			tbm.setFilename(TBM.getFilename());
			tbm.absorb(TBM);
			preconditions(tbm);
			
			//Set-up File Reader
			File fd = new File(path);
			out = new PrintWriter(new FileWriter(fd));
			
			//Set work to do
			bean.setTotalWork(tbm.getDataSize());
			bean.setWorkDone(0);
			
			//Call open method & set work done
			write(tbm);
			bean.setWorkDone(bean.getTotalWork());
			out.close();
		}
		catch(Exception e)
		{
			bean.setWorkDone(bean.getTotalWork());
			out.close();
			return false;
		}
		return true;
	}//close writeInit method

	public void preconditions(TaxaBeanManager tbm) throws Exception{}
	
	public void write(TaxaBeanManager tbm) throws IOException{}
	
	public void makeSameLength(TaxaBeanManager tbm) throws Exception
	{
		//makes all sequences the same length, used for writing CLUSTAL, PHYLIP, NEXUS
		int longSeq = 0;
		int shortSeq = 0;
		for(int i = 0; i<tbm.size(); i++)
		{
			TaxaBean temp = (TaxaBean)tbm.get(i);
			String seq = temp.getSequence();
			if(seq.length() > longSeq)
			{
				longSeq = seq.length();
			}//close if
			else if(seq.length() < shortSeq || shortSeq == 0)
			{
				shortSeq = seq.length();
			}
		}//close for
		
		if(longSeq - shortSeq >= 1500)
			throw new Exception("The disparity in sequence length is too great to write file type specified.");
		
		for(int i = 0; i<tbm.size(); i++)
		{
			TaxaBean temp = (TaxaBean)tbm.get(i);
			int seqLength = temp.getSequence().length();
			for(int j = 0; j<(longSeq - seqLength); j++)
			{
				temp.appendSequence("-");
			}//close for
			
		}//close for
	}//close makeSameLength method

	public void setNameLength(TaxaBeanManager tbm, int strlen) throws Exception
	{
		//Method vars
		TaxaBean tb;
		String name, newName;
		
		//Go through all taxa beans
		for(int i=0;i<tbm.size();i++)
		{
			tb = tbm.get(i);
			name = tb.getName();
			newName = setName(name, strlen);

			//update name
			tbm.updateName(name, newName, false);
		}
	}
	
	private String setName(String name, int strlen)
	{
		int len = name.length();
		if(len < strlen)
		{
			for(int i=len;i<10;i++)
				name += " ";
		}
		else if(len > strlen)		
			name = name.substring(0, strlen);
		
		return name;
	}

	public void print(String line) throws IOException
	{
		out.print(line);
	}
	
	
	public void println(String line) throws IOException
	{
		out.println(line);
	}

	public void printSeq(String seq, boolean newLine) throws IOException
	{
		//Print line to file
		if(newLine)
			out.println(seq);
		else
			out.print(seq);
		
		//Add to work done
		bean.addToWorkDone(seq.length());
	}	
	
	public void setBean(UsageBean Bean)
	{
		bean = Bean;
	}	
	
	public UsageBean getBean()
	{
		return bean;
	}
	
	public PrintWriter getBufferedReader()
	{
		return out;
	}
	
}//close Reader class
