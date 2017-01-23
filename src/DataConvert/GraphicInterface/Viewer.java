/**
 *	GraphicInterface.class is an instantiated object that creates
 *	and controls the operations of the GUI, as defined by a developer in
 *	the classes ActionControl and ObjectControl
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface;

import DataConvert.GraphicInterface.Objects.ToolTipButton;
import DataConvert.GraphicInterface.Objects.ActionPanel;

import java.util.Vector;
import javax.swing.JMenu;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JToolBar;
import javax.swing.JMenuBar;
import javax.swing.ImageIcon;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.JScrollPane;
import javax.swing.JRadioButton;
import javax.swing.border.BevelBorder;
import java.awt.Color;
import java.awt.Frame;
import java.awt.Dimension;
import java.awt.BorderLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;

class Viewer extends JFrame
{
	//Class Variables - Created Objects
	private ActionControl action;				//Object that controls actions from GUI
	private ObjectControl objects;				//Contains all objects to be placed on GUI
	private GraphicInterface controller;		//Class that Controlls input and visualizion of this class
	
	/**
	 *	GraphicInterface Constructor - Instantiates Class Objects.	
	 *
	 *	@param Object Driver - the driver class, must have access
	 */
	public Viewer(GraphicInterface Controller, String title)
	{
		controller = Controller;
		GraphicInterfaceUsageBean bean = controller.getBean();
		
		//Create Control Objects
		action = new ActionControl(controller);
		objects = new ObjectControl(action, controller.getBean());
		
		//set Items on GUI
		setMenuBar();
		setButtons();
		setPanels();
		
		//Add on additional features
		//addWindowListener(new WindowAdapter(){public void windowClosing(WindowEvent e) {System.exit(0);}});		
		setTitle(title);
		setIconImage(new ImageIcon(bean.getIconPath()+"mainIcon.gif").getImage());
		
		//Package up the GUI
//		pack();
		
		//set size
		if(bean.getFullScreen())
			setExtendedState(Frame.MAXIMIZED_BOTH);
		else
			setSize(bean.getLength(), bean.getHeight());
		
		//Visualize the GUI
		setVisible(true);
	}//constructor

	/**
	 * Places menu items on the GUI
	 */
	private void setMenuBar()
	{
		//Create the menu bar
		JMenuBar menuBar = new JMenuBar();
		setJMenuBar(menuBar);
		
		//Cycle through Vector, adding menu items
		Vector menuItems = objects.createMenuItems();
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
		Vector buttons = objects.createButtons();
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
		//Tabbed panel
		JPanel t = new JPanel();
		t.setBorder(new BevelBorder(BevelBorder.RAISED,Color.lightGray,Color.black));
		t.setBackground(Color.white);
		JTabbedPane tb = setTabbedPane();
		tb.setPreferredSize(new Dimension(485, 279));
		t.add(tb, BorderLayout.NORTH);

		
		//Main panel
		JPanel p = new JPanel();
		p.add(setLeftPanel());
		p.add(t);
		getContentPane().add(p, BorderLayout.WEST);
	}
	
	/**
	 * Sets the panel containing action buttons
	 */
	private JSplitPane setLeftPanel()
	{
		//Get Button Panels
		Vector buttonPanels = objects.createButtonPanels();
		
		//add panels to splitpane
		JSplitPane js = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		js.setTopComponent((ActionPanel)buttonPanels.elementAt(0));
		js.setBottomComponent((ActionPanel)buttonPanels.elementAt(1));
		
		return js;
	}

	
	/**
	 * Places panels on the GUI
	 */
	private JTabbedPane setTabbedPane()
	{
		//Method vars
		JPanel jp;
		ActionPanel a;
		JScrollPane jsp;
		
		//Create new tabbed pane and add on panels
		JTabbedPane tp = new JTabbedPane();
		
		//Cycle through Vector, adding panels
		Vector tabbedPanels = objects.createTabbedPanels(controller.getComponents());
		for(int i=0;i<tabbedPanels.size();i++)
		{
			//get button and add to toolbar
			a = (ActionPanel)tabbedPanels.elementAt(i);
			
			//place action panel on another panel
			jp = new JPanel();
			jp.setBackground(Color.white);
			jp.add(a,BorderLayout.CENTER);
			
			//set-up the scroll pane for each panel
			jsp = new JScrollPane(jp);
			jsp.getHorizontalScrollBar().setUnitIncrement(10);
			jsp.getVerticalScrollBar().setUnitIncrement(10);
			
			//add panel to tabbed pane
			tp.addTab(a.getTabName(), new ImageIcon(controller.getBean().getIconPath()+a.getIconName()), jsp, a.getToolTip());
		}
		
		return tp;
	}	

	/**
	 * Calls Object Control, setting objects
	 */
	public void setSettings()
	{
		objects.setSettings();
		repaint();
	}	
	
	/**
	 * Calls Object Control, getting objects
	 */
	public void getSettings()
	{
		objects.getSettings();
		repaint();
	}		
	
	/**
	 * Calls Object Control, placing objects
	 */
	public void updateComponents(int amnt)
	{
		
		objects.updateComponents(amnt);
		repaint();
	}	

	/**
	 * Return the action control object
	 */
	public ActionControl getAction()
	{
		return action;
	}	

}//GraphicInterface
