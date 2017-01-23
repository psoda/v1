/**
 *	RST.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.RST;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;

public class RST extends DataConvert.Components.Controller
{
	private RSTUsageBean bean;
	private RSTStub stub;
	private String name = "RST";
	private String[] extentions = {};
	
	public RST() throws IOException
	{
		readSettings("DataConvert/RST/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);	
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		RSTReader read = new RSTReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		throw new IOException("Can not write RST");
	}
	
	public void readSettings(String file)
	{
		try{
			stub = new RSTStub(file);
			bean = (RSTUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new RSTUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}
}
