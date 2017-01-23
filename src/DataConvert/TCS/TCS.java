/**
 *	TCS.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.TCS;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;

public class TCS extends DataConvert.Components.Controller
{
	private TCSStub stub;
	private TCSUsageBean bean;
	private String name = "TCS";
	private String[] extentions = {};
	
	public TCS() throws IOException
	{
		readSettings("DataConvert/TCS/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		TCSReader read = new TCSReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		throw new IOException("Can not write TCS");
	}

	public void readSettings(String file)
	{
		try{
			stub = new TCSStub(file);
			bean = (TCSUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new TCSUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}
}
