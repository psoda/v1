/**
 *	GENBANKXML.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.GENBANKXML;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;

public class GENBANKXML extends DataConvert.Components.Controller
{
	private GENBANKXMLStub stub;
	private GENBANKXMLUsageBean bean;
	private String name = "GENBANKXML";
	private String[] extentions = {"fcgi", "gbxml"};
	
	public GENBANKXML() throws IOException
	{
		readSettings("DataConvert/GENBANKXML/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		GENBANKXMLReader read = new GENBANKXMLReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		throw new IOException("Can not write GenBankXML");
	}

	public void readSettings(String file)
	{
		try{
			stub = new GENBANKXMLStub(file);
			bean = (GENBANKXMLUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new GENBANKXMLUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}
}
