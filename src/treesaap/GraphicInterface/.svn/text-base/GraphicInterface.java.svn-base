/**
 *	GraphicInterface.class is an instantiated object that creates
 *	and controls the operations of the GUI, as defined by a developer in
 *	the classes ActionControl and ObjectControl
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface;

import treesaap.Driver.Driver;
import treesaap.Driver.DriverForPsoda;
import treesaap.GraphicInterface.Objects.ToolTipButton;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.*;

import java.io.File;
import java.util.Vector;
import javax.swing.JMenu;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JToolBar;
import javax.swing.JMenuBar;
import javax.swing.ImageIcon;
import javax.swing.JTabbedPane;
import javax.swing.JScrollPane;
import javax.swing.JFileChooser;
import java.awt.Color;
import java.awt.Frame;
import java.awt.FileDialog;
import java.awt.BorderLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;

public class GraphicInterface extends JFrame
{
	//Class Variables - Created Objects
	private Object driver;						//Object of the driver class
	private ActionControl action;				//Object that controls actions from GUI
	private ObjectControl objects;				//Contains all objects to be placed on GUI
	private GraphicInterfaceUsageBean bean;		//Bean created and populated by this class
	
	/**
	 *	GraphicInterface Constructor - Instantiates Class Objects.	
	 *
	 *	@param Object Driver - the driver class, must have access
	 */
	public GraphicInterface(Object Driver)
	{
		//set vars
		driver = Driver;
		bean = new GraphicInterfaceUsageBean();
		
		//build GUI
		build();		
	}//constructor
	
	/**
	 *	GraphicInterface Constructor - Instantiates Class Objects.
	 *
	 *	@param Object Driver - the driver class, must have access
	 *	@param GraphicInterfaceUsageBean bean - class created with this bean
	 */
	public GraphicInterface(Object Driver, GraphicInterfaceUsageBean Bean)
	{
		//set vars
		bean = Bean;
		driver = Driver;
		
		//build GUI
		build();		
	}//constructor

	/**
	 * Builds the GUI
	 */
	private void build()
	{
		//Create Control Objects
		action = new ActionControl(driver, bean);
		objects = new ObjectControl(driver, action, bean);
		
		//set Items on GUI
		setMenuBar();
		setButtons();
		setPanels();
		
		//Add on additional features
		//addWindowListener(new WindowAdapter(){public void windowClosing(WindowEvent e) {System.exit(0);}});		
        //addWindowListener(new WindowAdapter(){public void windowClosing(WindowEvent e) {DriverForPsoda.getDriver().HideGui();}});		
		setTitle(bean.getTitle());
		setIconImage(new ImageIcon(bean.getIconPath()+"mainIcon.gif").getImage());
		
		//Package up the GUI
		pack();

		//set size
		if(bean.getFullScreen())
			setExtendedState(Frame.MAXIMIZED_BOTH);
		else
			setSize(bean.getLength(), bean.getHeight());
		
		//Visualize the GUI
		setVisible(true);
	}
	
	/**
	 * Places menu items on the GUI
	 */
	private void setMenuBar()
	{
		//Create the menu bar
		JMenuBar menuBar = new JMenuBar();
		setJMenuBar(menuBar);
		
		//Cycle through Vector, adding menu items
		Vector menuItems = objects.getMenuItems();
		for(int i=0;i<menuItems.size();i++)
			menuBar.add((JMenu)menuItems.elementAt(i));		
	}
	
	/**
	 * Places buttons on the GUI
	 */
	private void setButtons()
	{
		//Method vars
		ToolTipButton t;
		
		//Create new toolBar and add on buttons
		JToolBar tb = new JToolBar();
		
		//Cycle through Vector, adding buttons
		Vector buttons = objects.getButtons();
		for(int i=0;i<buttons.size();i++)
		{
			//get button and add to toolbar
			t = (ToolTipButton)buttons.elementAt(i);
			tb.add(t);
			
			//check to see if followed by a separator
			if(t.getSeparated())
				tb.addSeparator();		
		}
		
		//add to content pane
		getContentPane().add(tb, BorderLayout.NORTH);
	}
	
	/**
	 * Places panels on the GUI
	 */
	private void setPanels()
	{
		//Method vars
		JPanel jp;
		ActionPanel a;
		JScrollPane jsp;
		
		//Create new tabbed pane and add on panels
		JTabbedPane tp = new JTabbedPane();
		
		//Cycle through Vector, adding panels
		Vector panels = objects.getPanels();
		for(int i=0;i<panels.size();i++)
		{
			//get button and add to toolbar
			a = (ActionPanel)panels.elementAt(i);
			
			//place action panel on another panel
			jp = new JPanel();
			jp.setBackground(Color.white);
			jp.add(a,BorderLayout.CENTER);
			
			//set-up the scroll pane for each panel
			jsp = new JScrollPane(jp);
			jsp.getHorizontalScrollBar().setUnitIncrement(10);
			jsp.getVerticalScrollBar().setUnitIncrement(10);
			
			//add panel to tabbed pane
			tp.addTab(a.getTabName(), new ImageIcon(bean.getIconPath()+a.getIconName()), jsp, a.getToolTip());
		}
		
		getContentPane().add(tp, BorderLayout.CENTER);
	}	
	
	/**
	 * Calls Object Control, setting objects
	 */
	public void setObjects()
	{
		objects.setObjects();
		repaint();
	}	
	
	/**
	 * Calls Object Control, getting objects
	 */
	public void getObjects()
	{
		objects.getObjects();
	}	
	
	/**
	 * Calls Object Control, placing objects
	 */
	public void placeObjects()
	{
		objects.placeObjects();
		repaint();
	}	
	
	/**
	 * Calls Object Control, displaying object
	 */
	public void displayObject(String name)
	{
		objects.displayObject(name);
		repaint();
	}
	
	/**
	 * Prompts User to select File to open graphically
	 */
	public String getFileName()
	{
		//place objects from gui
		getObjects();
		
		try 
		{
			//Use Java FileDialog to get user to input which file to open
			FileDialog fd = new FileDialog(this,"Choose a File to Load", FileDialog.LOAD);
			fd.show();
			
			//set fileName to that obtained in FileDialog
			String fileName = fd.getFile();	
			
			//Make sure a correct file was chosen
			if(fileName == null)
				return null;
			
			//add on directory to the file name			
			fileName = fd.getDirectory() + fileName;			
			fd.dispose();
			
			return fileName;
		}
		
		catch(Exception e) 
		{
			bean.errorMessage("\nThere was a problem opening that file.\n\nPlease Check the format and try again.");
		}	
		return null;
	}	
	
	/**
	 * Prompts User to select Folder to open graphically
	 */
	public String getFolder()
	{
		//place objects from gui
		getObjects();
		
		try 
		{
			//Use Java VFSBrowser to get user to input which file to open
			JFileChooser fd = new JFileChooser(((Driver)driver).getBean().getResultsDirectory());
			fd.setDialogTitle("Choose a Folder");
			fd.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			
			//choose file
			fd.showOpenDialog(this);

			//return folderName obtained in JChooser
			String folderName = "";
			File file = fd.getSelectedFile();
			if(file != null)
				folderName = file.getAbsolutePath();
			return folderName;
		}
		
		catch(Exception e) 
		{
			bean.errorMessage("\nThere was a problem opening that folder.\n\nPlease Check the format and try again.");
		}	
		return null;
	}	
	
	/**
	 * Returns the bean of user settings for GraphicInterface
	 */
	public GraphicInterfaceUsageBean getBean()
	{
		return bean;
	}	
	
	/**
	 *	Sets this Usagebean to the one passed in
	 *	@param GraphicInterfaceUsageBean Bean - the new bean to be used
	 */
	public void setBean(GraphicInterfaceUsageBean Bean)
	{
		bean = Bean;		
	}

}//GraphicInterface