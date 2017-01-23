/**
 *	XML.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.XML;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;

public class XML extends DataConvert.Components.Controller
{
	private XMLStub stub;
	private XMLUsageBean bean;
	private String name = "XML";
	private String[] extentions = {"fcgi"};
	
	public XML() throws IOException
	{
		readSettings("DataConvert/XML/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		XMLReader read = new XMLReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		throw new IOException("Can not write GenBankXML");
	}

	public void readSettings(String file)
	{
		try{
			stub = new XMLStub(file);
			bean = (XMLUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new XMLUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}
}
