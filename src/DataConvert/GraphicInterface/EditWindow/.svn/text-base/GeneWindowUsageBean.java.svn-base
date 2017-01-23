/**
 *	GraphicInterfaceUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the GraphicInterface package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.2
 *	@since	1.0
 */
package DataConvert.GraphicInterface.EditWindow;

import java.awt.Rectangle;
import java.util.StringTokenizer;

class GeneWindowUsageBean extends DataConvert.Components.UsageBean
{
	//Operational Variables, manipulated by super class
	private Rectangle position;		//The position of the window
	
	/**
	 *  Set method - sets position to new Rectangle
	 *	@param String line of new Rectangle
	 */
	public void setPosition(String line)
	{
		StringTokenizer st = new StringTokenizer(line);
		int x = (int)Double.valueOf(st.nextToken()).doubleValue();
		int y = (int)Double.valueOf(st.nextToken()).doubleValue();
		int wd = (int)Double.valueOf(st.nextToken()).doubleValue();
		int ht = (int)Double.valueOf(st.nextToken()).doubleValue();
		
		position = new Rectangle(x,y,wd,ht);
	}
	
	/**
	 *  Get method - returns Rectangle position 
	 */
	public Rectangle getPosition()
	{
		return position;
	}
	
	/**
	 *  Set method - sets position to new Rectangle
	 *	@param int, int, int, int of new Rectangle
	 */
	public void setPosition(int x, int y, int width, int height)
	{
		position = new Rectangle(x, y, width, height);
	}
	
	/**
	 *  Set method - sets action to new position
	 *	@param Rectangle newPosition of new position
	 */
	public void setPosition(Rectangle newPosition)
	{
		position = newPosition;
	}

}//GeneWindowUsageBean
