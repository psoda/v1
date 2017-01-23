/**
 *	RunPanel.class is an instantiated object that creates
 *	a JPanel containing all the run options and choice of trees
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface.Panels;

import java.awt.Color;
import javax.swing.border.BevelBorder;

import DataConvert.GraphicInterface.ActionControl;
import DataConvert.GraphicInterface.Objects.ToolTipButton;
import DataConvert.GraphicInterface.Objects.ActionPanel;

public class TopPanel extends ActionPanel
{
	/**
	 * Constructor of this panel - must be given the driver
	 * @param Object driver - the driver of this program
	 * @param ActionControl act - contains the action class
	 */
	public TopPanel(ActionControl act)
	{
		setAction(act);
		createObjects();
	}
	
	/**
	 * Creates all the objects to be placed on this panel, places them in panelObjects
	 */
	public void create()
	{
		//set Background
		setBorder(new BevelBorder(BevelBorder.RAISED,Color.lightGray,Color.black));
		setBackground(Color.white);	

		//open file
		ToolTipButton openFile = new ToolTipButton("Open File", "Open a file");
		openFile.addActionListener(getAction());
		openFile.setActionName("openFile");
		addItem(openFile, endLine);
		
		//open dir
		ToolTipButton openDir = new ToolTipButton("Open Dir", "Open a directory of files");
		openDir.addActionListener(getAction());
		openDir.setActionName("openDir");
		addItem(openDir, endLine);

		//new file
		ToolTipButton newFile = new ToolTipButton("New File", "Create a new file");
		newFile.addActionListener(getAction());
		newFile.setActionName("newFile");
		addItem(newFile, endLine, 1, endLine);
		
		//display/edit data
		ToolTipButton display = new ToolTipButton("Display/Edit", "Display and Edit data from open file");
		display.addActionListener(getAction());
		display.setActionName("display");
		addItem(display, endLine);		
		
		//Close Files
		ToolTipButton close = new ToolTipButton("Close All", "Close all open files");
		close.addActionListener(getAction());
		close.setActionName("close");
		addItem(close, endLine, 1, endLine);
		
		//place the objects on the panel
		updateComponents(0);

	}//create

	/**
	 * Changes the settings on buttons depending on amnt
	 */
	public void updateComponents(int amnt)
	{	
		try
		{
			//Switch on amnt
			if(amnt > 1)
			{
				getItem(2).setEnabled(false);
				getItem(3).setEnabled(false);
				getItem(4).setEnabled(true);
			}
			else if(amnt == 1)
			{
				getItem(2).setEnabled(false);
				getItem(3).setEnabled(true);
				getItem(4).setEnabled(true);
			}
			else
			{
				getItem(2).setEnabled(true);
				getItem(3).setEnabled(false);
				getItem(4).setEnabled(false);
			}
		}
		catch(Exception e){}
		
	}//getSettings

}//TopPanel
