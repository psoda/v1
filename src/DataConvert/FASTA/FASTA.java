/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.FASTA;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.GraphicInterface.Objects.ActionPanel;

public class FASTA extends DataConvert.Components.Controller
{
	private FASTAStub stub;
	private FASTAUsageBean bean;
	private String name = "FASTA";
	private String[] extentions = {"fa", "fas", "fasta", "aa", "nt", "pir"};
	
	public FASTA()
	{
		readSettings("DataConvert/FASTA/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);	
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		FASTAReader read = new FASTAReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		FASTAWriter write = new FASTAWriter(bean);
		return write.writeInit(path, tbm);
	}

	public void readSettings(String file)
	{
		try{
			stub = new FASTAStub(file);
			bean = (FASTAUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new FASTAUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}

	public ActionPanel getPanel()
	{
		return new FASTAPanel(bean);
	}
}
