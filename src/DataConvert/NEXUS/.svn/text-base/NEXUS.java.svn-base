/**
 *	NEXUS.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.NEXUS;

import java.io.IOException;
import java.util.Enumeration;
import java.util.StringTokenizer;
import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.GraphicInterface.Objects.ActionPanel;

public class NEXUS extends DataConvert.Components.Controller
{
	private NEXUSStub stub;
	private NEXUSUsageBean bean;
	private String name = "NEXUS";
	private String[] extentions = {"nex", "nxs", "nexus", "paup"};
	
	public NEXUS() throws IOException
	{
		readSettings("DataConvert/NEXUS/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);			
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		NEXUSReader read = new NEXUSReader(bean);
		NEXUSBlockReader readB = new NEXUSBlockReader(bean);
		
		TaxaBeanManager tbm = readB.openInit(path);
		if(tbm == null)
			tbm = read.openInit(path);
		return tbm;
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		NEXUSWriter write = new NEXUSWriter(bean);
		return write.writeInit(path, tbm);
	}

	public void readSettings(String file)
	{
		try{
			stub = new NEXUSStub(file);
			bean = (NEXUSUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new NEXUSUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}
	
	public ActionPanel getPanel()
	{
		return new NEXUSPanel(bean);
	}
}
