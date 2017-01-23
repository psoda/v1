/**
 *	Controller.java is an interface class with open and write methods
 *	for use in dictating flow of each component
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.Components;

import java.io.IOException;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.GraphicInterface.Objects.ActionPanel;

public class Controller
{
	private Stub stub;
	private UsageBean bean;

	public TaxaBeanManager open(String path)throws IOException
	{
		return null;
	}
	
	public boolean write(String path, TaxaBeanManager tbm)throws IOException
	{
		return false;
	}
	
	public void readSettings(String file){}
	
	public ActionPanel getPanel()
	{
		return null;
	}
	
	public void writeSettings()
	{
		try{
			stub.writeSettings();
		}
		catch(Exception e){}
	}
	
	public void resetSettings()	
	{
		try{
			stub.resetSettings();
		}
		catch(Exception e){}
	}
	
	public void setStub(Stub newStub)
	{
		stub = newStub;
	}
	
	public Stub getStub()
	{
		return stub;
	}
	
	public void setBean(UsageBean Bean)
	{
		bean = Bean;
	}
	
	public UsageBean getBean()
	{
		return bean;
	}
}
