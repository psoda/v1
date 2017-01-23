/**
 *	PHYLIP.class that reads in a PHYLIP file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.PHYLIP;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.GraphicInterface.Objects.ActionPanel;

public class PHYLIP extends DataConvert.Components.Controller
{
	private PHYLIPStub stub;
	private PHYLIPUsageBean bean;
	private String name = "PHYLIP";
	private String[] extentions = {"PHYLIP", "phylip", "phy"};
	
	public PHYLIP()
	{
		readSettings("DataConvert/PHYLIP/user_settings.txt");
		bean.setName(name);
		bean.setExtentions(extentions);	
	}
	
	public TaxaBeanManager open(String path)throws IOException
	{
		PHYLIPReader read = new PHYLIPReader(bean);
		return read.openInit(path);
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		PHYLIPWriter write = new PHYLIPWriter(bean);
		return write.writeInit(path, tbm);
	}

	public void readSettings(String file)
	{
		try{
			stub = new PHYLIPStub(file);
			bean = (PHYLIPUsageBean)stub.getBean();
		}
		catch(Exception e){
			bean = new PHYLIPUsageBean();
		}
		setStub(stub);
		setBean(bean);
	}

	public ActionPanel getPanel()
	{
		return new PHYLIPPanel(bean);
	}
}
