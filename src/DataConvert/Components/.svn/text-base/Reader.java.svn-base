/**
 *	Reader.java from the Components jar, contains shared components of all
 *	the readers in each extended component class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.Components;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import java.util.StringTokenizer;

import DataConvert.Objects.TaxaBeanManager;

public class Reader
{
	private UsageBean bean;
	private BufferedReader in;
	private StringTokenizer strtok;
	
	public TaxaBeanManager openInit(String path)throws IOException
	{
		//Set-up File Reader
		File fd = new File(path);
		in = new BufferedReader(new FileReader(fd));
		
		//Set work to do
		bean.setTotalWork(fd.length());
		bean.setWorkDone(0);
		
		//Set up file name
		String name = fd.getName();
		//if(name.indexOf(".") != -1)
		//	name = name.substring(0, name.indexOf("."));

		//Call open method & set work done
		TaxaBeanManager tb = open(name);
		bean.setWorkDone(bean.getTotalWork());
		in.close();
		
		return tb;
	}//close openInit method
	
	public TaxaBeanManager open(String filename) throws IOException
	{
		return null;
	}

	public String getToken()throws IOException{
		if(strtok == null)
			strtok = new StringTokenizer(getLine());
		
		if(strtok.hasMoreTokens())
			return(strtok.nextToken());
		else
		{
			String line = getLine();
			if(line != null)
			{
				strtok = new StringTokenizer(line);
				return(strtok.nextToken());
			}	
		}
		return null;
	}//close getToken method
	
	public String getLine() throws IOException
	{
		//Return Line w/o comments
		String subline = null;
		String line = getNonEmptyLine();
		if(line != null && line.indexOf("[") != -1)
		{
			//set subline
			subline = line.substring(0,line.indexOf("["));
			
			//Move file to point at non commentted line
			while(line != null && line.indexOf("]") == -1)
				line = getNonEmptyLine();
			
			//Check for end of file
			if(line == null)
				return null;
			
			//append to subline
			subline += " "+line.substring(line.indexOf("]")+1);
		
			//Check for subline contents
			if(subline == null || subline.equals("") || (new StringTokenizer(subline)).countTokens() == 0)
				return getLine();
		}
		else
			subline = line;
		
		return subline;
	}
	
	private String getNonEmptyLine() throws IOException
	{
		String line = in.readLine();
		while(line != null && line.equals("") && (new StringTokenizer(line)).countTokens() == 0)
		{
			line = in.readLine();
			bean.addToWorkDone(1);
		}
		if(line != null)
			bean.addToWorkDone(line.length() + 1);
		else
			bean.setWorkDone(bean.getTotalWork());
		
		return line;
	}
	
	public int getTokenCount()
	{
		return strtok.countTokens();
	}
	
	public String restOfLine()
	{
		StringBuffer line = new StringBuffer();
		while(strtok.hasMoreTokens())
			line.append(" "+strtok.nextToken());
		return line.toString();
	}
	
	public void setBean(UsageBean Bean)
	{
		bean = Bean;
	}
	
	public UsageBean getBean()
	{
		return bean;
	}

	public BufferedReader getBufferedReader()
	{
		return in;
	}

}//close Reader class
