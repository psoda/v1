/**
 *	CLUSTAL.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.CLUSTAL;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.GraphicInterface.Objects.ActionPanel;

public class CLUSTAL extends DataConvert.Components.Controller
{
	private CLUSTALStub stub;
	private CLUSTALUsageBean bean;
	private String name = "CLUSTAL";
	private String[] extentions = {"aln"};
	
	public CLUSTAL() throws IOException
	{
		readSettings("DataConvert/CLUSTAL/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);		
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		CLUSTALReader read = new CLUSTALReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		CLUSTALWriter write = new CLUSTALWriter(bean);
		return write.writeInit(path, tbm);
	}

	public void readSettings(String file)
	{
		try{
			stub = new CLUSTALStub(file);
			bean = (CLUSTALUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new CLUSTALUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}

	public ActionPanel getPanel()
	{
		return new CLUSTALPanel(bean);
	}
}
