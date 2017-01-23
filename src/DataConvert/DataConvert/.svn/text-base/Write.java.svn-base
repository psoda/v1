package DataConvert.DataConvert;

import java.io.IOException;
import java.util.Vector;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.Components.Controller;

class Write
{
	private DriverUsageBean bean;
	
	public Write(DriverUsageBean Bean)
	{
		bean = Bean;
	}
	
	public boolean set(String type, String file, TaxaBeanManager tbm) throws IOException
	{		
		//Validate file
		file = bean.getAbsoluteFilePath(file, true);
		if(file == null)
			throw new IOException("Invalid output file specified");

		return ((Controller)bean.getComponent(type)).write(file, tbm);
	}
	
	public boolean setDir(String type, String dir, Vector tbms) throws IOException
	{
		//Method vars
		String temp;
		boolean total = true;
		
		//Validate dir
		dir = bean.getAbsoluteDirPath(dir, true);
		if(dir == null)
			throw new IOException("Invalid input directory specified");

		//go through vector of tbms
		for(int i=0;i<tbms.size();i++)
		{
			TaxaBeanManager t = (TaxaBeanManager)tbms.elementAt(i);
			temp = dir + t.getFilename() + "." + type;
			total = total && set(type, temp, t);
		}
		return total;
	}
}
