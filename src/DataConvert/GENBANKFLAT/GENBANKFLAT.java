/**
 *	GENBANKFLAT.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.GENBANKFLAT;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;

public class GENBANKFLAT extends DataConvert.Components.Controller
{
	private GENBANKFLATStub stub;
	private GENBANKFLATUsageBean bean;
	private String name = "GENBANKFLAT";
	private String[] extentions = {"fcgi", "flat"};
	
	public GENBANKFLAT() throws IOException
	{
		readSettings("DataConvert/GENBANKFLAT/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);		
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		GENBANKFLATReader read = new GENBANKFLATReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		throw new IOException("Can not write GenBankGENBANKFLAT");
	}
	
	public void readSettings(String file)
	{
		try{
			stub = new GENBANKFLATStub(file);
			bean = (GENBANKFLATUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new GENBANKFLATUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}
}
