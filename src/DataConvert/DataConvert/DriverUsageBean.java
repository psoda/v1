/**
 *	DriverUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the DataConvert package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package DataConvert.DataConvert;

import java.util.Hashtable;
import java.util.Enumeration;

public class DriverUsageBean extends DataConvert.Components.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private Hashtable components;				//The Reader/Writer Components
	
	/**
	 *  Set method - sets components to new Hashtable
	 *	@param Hashtable Components of new components
	 */
	public void setComponents(Hashtable Components)
	{
		components = Components;
	}
	
	/**
	 *  Get method - returns Hashtable components 
	 */
	public Hashtable getComponents()
	{
		return components;
	}
	
	public Enumeration getKeys()
	{
		return components.keys();
	}

	public Object getComponent(String key)
	{
		return components.get(key);
	}
	
	public void addComponent(String key, Object comp)
	{
		components.put(key, comp);
	}
	
}//DriverUsageBean
