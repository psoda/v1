/**
 *	MEGA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.MEGA;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.GraphicInterface.Objects.ActionPanel;

public class MEGA extends DataConvert.Components.Controller
{
	private MEGAStub stub;
	private MEGAUsageBean bean;
	private String name = "MEGA";
	private String[] extentions = {"meg", "mega", "gde"};
	
	public MEGA() throws IOException
	{
		readSettings("DataConvert/MEGA/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);		
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		MEGAReader read = new MEGAReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		MEGAWriter write = new MEGAWriter(bean);
		return write.writeInit(path, tbm);
	}

	public void readSettings(String file)
	{
		try{
			stub = new MEGAStub(file);
			bean = (MEGAUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new MEGAUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}

	
	public ActionPanel getPanel()
	{
		return new MEGAPanel(bean);
	}
}
