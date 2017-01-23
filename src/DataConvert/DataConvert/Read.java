package DataConvert.DataConvert;

import java.io.File;
import java.io.IOException;
import java.util.Vector;
import java.util.Enumeration;

import DataConvert.Components.Controller;
import DataConvert.Objects.TaxaBeanManager;

class Read
{
	//Class Vars
	private DriverUsageBean bean;
	
	public Read(DriverUsageBean Bean)
	{
		bean = Bean;
	}
	
	public TaxaBeanManager open(String file) throws IOException
	{		
		//Method Vars
		TaxaBeanManager tbm = null;
		Vector elements = new Vector();
		Vector validTBMs = new Vector();

		//Validate file
		file = bean.getAbsoluteFilePath(file, false);
		if(file == null)
			throw new IOException("Invalid input file specified: "+ file);
		
		//Try opening the extention
		try{setTBM(((Controller)bean.getComponent(getExtention(file))).open(file), validTBMs);}catch(Exception e){}
		
		//If the extension method fails, open in all formats
		if(validTBMs.size() == 0)
		{
			//Go thru each component in hashtable, open file
			for(Enumeration e = bean.getComponents().elements(); e.hasMoreElements();) 
			{
				Controller c = (Controller)e.nextElement();
				String name = c.getBean().getName();
				if(!elements.contains(name))
				{
					try{setTBM(c.open(file), validTBMs);}catch(Exception ex){}
					elements.add(name);
				}
			}
		}
		
		//Return the valid TBM
		if(validTBMs.size() == 0)
			throw new IOException("File was of unknown type, unable to open.  \nFile: "+ file);
		if(validTBMs.size() == 1)
			tbm = (TaxaBeanManager)validTBMs.elementAt(0);
		else
			tbm = eliminateTBMs(validTBMs);
		
		return tbm;
	}
	
	private TaxaBeanManager eliminateTBMs(Vector tbms)
	{
		//Method Vars
		int index = 0, treeIndex = -1, amntIndex = -1;
		long trNum = 0, amnt = 0, temp;
		TaxaBeanManager tbm;
		
		//Go through all tbms
		for(int i=0;i<tbms.size();i++)
		{
			TaxaBeanManager t = (TaxaBeanManager)tbms.elementAt(i);
			
			//Eliminate on most trees
			if(trNum < t.getTreeNum())
			{
				trNum = t.getTreeNum();
				treeIndex = i;
			}
			
			//Eliminate on most taxa, chars	
			temp = t.size() * t.getDataSize();
			if(amnt < temp)
			{
				amnt = temp;
				amntIndex = i;
			}
		}
		
		//Pick the one with the most trees over the one with the most data
		if(trNum > 0)
			index = treeIndex;
		else
			index = amntIndex;
		
		return (TaxaBeanManager)tbms.elementAt(index);
	}
	
	private String getExtention(String fileName)
	{
		//Method Vars
		String name = "", ext = "";
		File fd = new File(fileName);
		
		//Get name
		if((name = fd.getName()) == null)
			return "";
		
		//Get ext
		if(name.indexOf(".") != -1)
			ext = name.substring(name.lastIndexOf(".")+1);
		
		//return ext
		if(!ext.equals(""))
			return ext.toLowerCase();
		else if(name.equalsIgnoreCase("rst"))
			return "rst";
		
		return "";
	}
	
	private void setTBM(TaxaBeanManager tbm, Vector validTBMs)
	{
		if(tbm != null && tbm.size() > 0 && tbm.getDataSize() > 30)
			validTBMs.add(tbm);
	}
	
	public Vector openDir(String dir) throws IOException
	{
		//Method Vars
		Vector tbms = new Vector();

		//Validate file
		dir = bean.getAbsoluteDirPath(dir, false);
		if(dir == null)
			throw new IOException("Invalid input directory specified: "+ dir);
		
		//Get Files
		File fd = new File(dir);
		String[] files = fd.list();

		//Open each file
		TaxaBeanManager tbm;
		for(int i=0;i<files.length;i++)
		{
			try{
				tbm = open(dir + files[i]);
				if(tbm != null)
					tbms.add(tbm);
			}
			catch(Exception e){}
		}			
		
		//Check for valid tbms
		if(tbms.size() == 0)
			throw new IOException("No files located in directory: "+ dir);
		
		return tbms;
	}

}
