/**
 *	BASEMLUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the baseml package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Data;

import java.io.File;
import java.util.Vector;


public class DataUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private String geneticCodePath = "";				//Path to genetic code data
	private String propertyPath = "";					//Path to Amino Acid Property data
	private String criticalValuePath = "";				//Path to Critical Value data
	
	//Process Vars
	private Vector deletePropLists;					//Property Lists to be deleted

	/**
	 *  Get method - returns Vector deletePropLists 
	 */
	public Vector getDeletePropLists()
	{
		return deletePropLists;
	}
	
	/**
	 *  Set method - sets deletePropLists to new Vector
	 *	@param Vector DeletePropLists of new deletePropLists
	 */
	public void setDeletePropLists(Vector DeletePropLists)
	{
		deletePropLists = DeletePropLists;
	}
	
	/**
	 *  Get method - returns String geneticCodePath 
	 */
	public String getGeneticCodePath()
	{
		return geneticCodePath;
	}
	
	/**
	 *  Set method - sets geneticCodePath to new String
	 *	@param String GeneticCodePath of new geneticCodePath
	 */
	public void setGeneticCodePath(String GeneticCodePath)
	{
		geneticCodePath = getAbsoluteDirPath(GeneticCodePath);
		if(geneticCodePath == null)
			geneticCodePath = "";
		else
			geneticCodePath += File.separator;
	}
	
	/**
	 *  Set method - sets propertyPath to new String
	 *	@param String PropertyPath of new propertyPath
	 */
	public void setPropertyPath(String PropertyPath)
	{
		propertyPath = getAbsoluteDirPath(PropertyPath);
		if(propertyPath == null)
			propertyPath = "";
		else
			propertyPath += File.separator;
	}
	
	/**
	 *  Get method - returns String propertyPath 
	 */
	public String getPropertyPath()
	{
		return propertyPath;
	}
	
	/**
	 *  Set method - sets criticalValuePath to new String
	 *	@param String CriticalValuePath of new criticalValuePath
	 */
	public void setCriticalValuePath(String CriticalValuePath)
	{
		criticalValuePath = getAbsoluteDirPath(CriticalValuePath);
		if(criticalValuePath == null)
			criticalValuePath = "";
		else
			criticalValuePath += File.separator;
	}
	
	/**
	 *  Get method - returns String criticalValuePath 
	 */
	public String getCriticalValuePath()
	{
		return criticalValuePath;
	}

}//DataUsageBean
