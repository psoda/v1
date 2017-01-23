/**
 *	ToolTipButton.class is an instantiated object that creates
 *	buttons with a pop-up message
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Objects;

import java.awt.Point;
import java.awt.Dimension;
import java.awt.event.MouseEvent;
import javax.swing.JButton;
import javax.swing.ImageIcon;

public class ToolTipButton extends JButton
{
	//Class Vars
	private String actionName;
	private boolean separated;
	
	/**
	 *	ToolTipButton Constructor - Instantiates Class Objects.	
	 *
	 *	@param ImageIcon icon - icon set on button
	 *	@param String tooltipText - text displayed on tool tip
	 */
	public ToolTipButton(ImageIcon icon, String tooltipText)
	{
		super(icon);
		setToolTipText(tooltipText);
		
	}//Construtor
	
	/**
	 *	ToolTipButton Constructor - Instantiates Class Objects.	
	 *
	 *	@param String title - text set on button
	 *	@param String tooltipText - text displayed on tool tip
	 */
	public ToolTipButton(String title, String tooltipText)
	{
		super(title);
		super.setBounds(50, 50, 15, 15);
		setToolTipText(tooltipText);
		
	}//Constructor
	
	/**
	 *	Displays tooltip text - called automatically
	 *	@param MouseEvent e - the event of pointing mouse at button
	 */
	public Point getToolTipLocation(MouseEvent e)
	{
		Dimension size = getSize();
		return new Point(0, size.height);
		
	}//getToolTipLocation
	
	/**
	 *  Get method - returns boolean separated 
	 */
	public boolean getSeparated()
	{
		return separated;
	}
	
	/**
	 *  Set method - sets separated to new boolean
	 *	@param boolean Separated of new separated
	 */
	public void setSeparated(boolean Separated)
	{
		separated = Separated;
	}
	
	/**
	 *  Get method - returns String actionName 
	 */
	public String getActionName()
	{
		return actionName;
	}
	
	/**
	 *  Set method - sets actionName to new String
	 *	@param String ActionName of new actionName
	 */
	public void setActionName(String ActionName)
	{
		actionName = ActionName;
	}

}//ToolTipButton