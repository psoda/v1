/**
 *	GraphicInterfaceUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the GraphicInterface package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.2
 *	@since	1.0
 */
package treesaap.GraphicInterface;

import java.io.File;
import java.util.Vector;


public class GraphicInterfaceUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private boolean fullScreen;			//Whether the GUI is full-screen or not
	private int height;					//Height of the GUI frame
	private int length;					//Length of the GUI frame
	private String iconPath;			//The full path to the location of the icons for this program
	
	//Operational Variables, manipulated by super class
	private String title;				//Title of the program
	
	/**
	 *  Get method - returns String title 
	 */
	public String getTitle()
	{
		return title;
	}
	
	/**
	 *  Set method - sets title to new String
	 *	@param String Title of new title
	 */
	public void setTitle(String Title)
	{
		title = Title;
	}
	
	/**
	 *  Get method - returns String iconPath 
	 */
	public String getIconPath()
	{
		return iconPath;
	}
	
	/**
	 *  Set method - sets iconPath to new String
	 *	@param String IconPath of new iconPath
	 */
	public void setIconPath(String IconPath)
	{
		iconPath = getAbsoluteDirPath(IconPath);
		
		//Error Control
		if(iconPath == null)
			iconPath = "";
		else
			iconPath += File.separator;
	}
	
	/**
	 *  Get method - returns boolean fullScreen 
	 */
	public boolean getFullScreen()
	{
		return fullScreen;
	}
	
	/**
	 *  Set method - sets fullScreen to new boolean
	 *	@param boolean FullScreen of new fullScreen
	 */
	public void setFullScreen(boolean FullScreen)
	{
		fullScreen = FullScreen;
	}
	
	/**
	 *  Get method - returns int length 
	 */
	public int getLength()
	{
		return length;
	}
	
	/**
	 *  Set method - sets length to new int
	 *	@param int Length of new length
	 */
	public void setLength(int Length)
	{
		length = Length;
	}
	
	/**
	 *  Get method - returns int height 
	 */
	public int getHeight()
	{
		return height;
	}
	
	/**
	 *  Set method - sets height to new int
	 *	@param int Height of new height
	 */
	public void setHeight(int Height)
	{
		height = Height;
	}

}//GraphicInterfaceUsageBean
