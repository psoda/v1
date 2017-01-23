/**
 *	ObjectControl.class is an instantiated object that contains
 *	all the developer-defined objects to be placed on the GUI
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface;

import DataConvert.Components.Controller;
import DataConvert.GraphicInterface.Panels.TopPanel;
import DataConvert.GraphicInterface.Panels.BottomPanel;
import DataConvert.GraphicInterface.Objects.ActionPanel;
import DataConvert.GraphicInterface.Objects.ToolTipButton;
import DataConvert.GraphicInterface.Objects.ActionMenuItem;

import java.util.Vector;
import java.util.Hashtable;
import java.util.Enumeration;
import java.awt.Font;
import java.awt.event.KeyEvent;
import javax.swing.JMenu;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;

class ObjectControl
{
	//Class Variables - Created Objects
	private ActionControl action;				//Object that controls actions from GUI
	private GraphicInterfaceUsageBean bean;		//Bean created and populated by this class
	
	//Created Vars - to be placed on GUI
	private Vector tabbedPanels;				//Vector of panel objects to be placed on GUI for JTabbed Pane
	private Vector buttonPanels;				//Vector of panel objects to be placed on GUI containing buttons

	/**
	 *	ObjectControl Constructor - Instantiates Class Objects.	
	 *
	 *	@param Object Driver - the driver class, must have access
	 *	@param ActionControl act - contains the action class
	 *	@param GraphicInterfaceUsageBean bean - class created with this bean
	 */
	public ObjectControl(ActionControl act, GraphicInterfaceUsageBean Bean)
	{
		bean = Bean;	
		action = act;	
	}//constructor

	/**
	 * Creates the JMenuItems to be placed on the GUI
	 * Developers - enter code here
	 */
	public Vector createMenuItems()
	{
		//Method Vars
		JMenu menu;
		ActionMenuItem item;
		Vector menuItems = new Vector();

		//File added to the menu bar
		menu = new JMenu("File");
		menu.setFont(new Font("Helvetica", Font.PLAIN, 12));
		menuItems.add(menu);
		
			//exit - exit program
			item = new ActionMenuItem("New File");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("newFile");
			item.addActionListener(action);
			menu.add(item);
			
			//exit - exit program
			item = new ActionMenuItem("Display/Edit");	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("display");
			item.addActionListener(action);
			menu.add(item);
			menu.addSeparator();
		
			//exit - exit program
/*
			item = new ActionMenuItem("Exit");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("exit");
			item.addActionListener(action);
			menu.add(item);
*/
		
		//Input option added to GUI
		menu = new JMenu("Input");
		menu.setFont(new Font("Helvetica", Font.PLAIN, 12));
		menuItems.add(menu);	
			
			//openNew - opens new sequence file
			item = new ActionMenuItem("Open File(s)");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("openFile");
			item.addActionListener(action);
			menu.add(item);
			
			//Open a dir of files
			item = new ActionMenuItem("Open Dir");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_D, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("openDir");
			item.addActionListener(action);
			menu.add(item);
			menu.addSeparator();

			//close - close files
			item = new ActionMenuItem("Close File(s)");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("close");
			item.addActionListener(action);
			menu.add(item);
			

		//Output options added to GUI
		menu = new JMenu("Output");
		menu.setFont(new Font("Helvetica", Font.PLAIN, 12));
		menuItems.add(menu);	
			
			//openNew - opens new sequence file
			item = new ActionMenuItem("Combine Files");
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("combine");
			item.addActionListener(action);
			menu.add(item);
			menu.addSeparator();
			
			//Write Files
			item = new ActionMenuItem("Write File(s)");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_W, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("write");
			item.addActionListener(action);
			menu.add(item);


		//Help added to the menu bar
		menu = new JMenu("Options");
		menu.setFont(new Font("Helvetica", Font.PLAIN, 12));
		menuItems.add(menu);
			
			//help - opens new help file
			item = new ActionMenuItem("Save Settings");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S,java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("save");
			item.addActionListener(action);
			menu.add(item);
			
			//documentation - opens documentation file
			item = new ActionMenuItem("Reset Settings");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_R,java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("reset");
			item.addActionListener(action);
			menu.add(item);
			menu.addSeparator();
			
			//credit - opens acreditation file
			item = new ActionMenuItem("Reload Settings From File");
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("reload");
			item.addActionListener(action);
			menu.add(item);	
			
		//Help added to the menu bar
		menu = new JMenu("Help");
		menu.setFont(new Font("Helvetica", Font.PLAIN, 12));
		menuItems.add(menu);
			
			//documentation - opens documentation file
			item = new ActionMenuItem("Documentation");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_H,java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("helpFile");
			item.addActionListener(action);
			menu.add(item);
		
			//credit - opens acreditation file
			item = new ActionMenuItem("Credits");
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("credit");
			item.addActionListener(action);
			menu.add(item);	
			
		return menuItems;
	}
	
	/**
	 * Creates the Buttons to be placed on the GUI
	 * Developers - enter code here
	 */
	public Vector createButtons()
	{
		//Method Vars
		ImageIcon icon;
		ToolTipButton t;
		Vector buttons = new Vector();

		//Create new File icon
		icon = new ImageIcon(bean.getIconPath()+"newFileIcon.gif");
		System.out.println("New File" +bean.getIconPath()+"newFileIcon.gif");
		t = new ToolTipButton(icon, "Create New File");
		t.setActionName("newFile");
		t.addActionListener(action);
		t.setSeparated(false);
		buttons.add(t);
		
		//Open File Icon
		icon = new ImageIcon(bean.getIconPath()+"openFileIcon.gif");
		t = new ToolTipButton(icon, "Open New File");
		t.setActionName("openFile");
		t.addActionListener(action);
		t.setSeparated(false);
		buttons.add(t);
		
		//Open File Icon
		icon = new ImageIcon(bean.getIconPath()+"openDirIcon.gif");
		t = new ToolTipButton(icon, "Open Dir");
		t.setActionName("openDir");
		t.addActionListener(action);
		t.setSeparated(true);
		buttons.add(t);
		
		//Write File Icon
		icon = new ImageIcon(bean.getIconPath()+"writeIcon.gif");
		t = new ToolTipButton(icon, "Write File(s)");
		t.setActionName("write");
		t.addActionListener(action);
		t.setSeparated(true);
		buttons.add(t);
		
		//Open File Icon
		icon = new ImageIcon(bean.getIconPath()+"saveIcon.gif");
		t = new ToolTipButton(icon, "Save Settings");
		t.setActionName("save");
		t.addActionListener(action);
		t.setSeparated(false);
		buttons.add(t);
		
		//Open File Icon
		icon = new ImageIcon(bean.getIconPath()+"resetIcon.gif");
		t = new ToolTipButton(icon, "Reset Settings");
		t.setActionName("reset");
		t.addActionListener(action);
		t.setSeparated(true);
		buttons.add(t);
		
		//Open Help File Icon
		icon = new ImageIcon(bean.getIconPath()+"helpFileIcon.gif");
		t = new ToolTipButton(icon, "Open Help File");
		t.setActionName("helpFile");
		t.addActionListener(action);
		t.setSeparated(false);
		buttons.add(t);
		
		return buttons;
	}
	
	/**
	 * Creates the Panels to be placed on the GUI
	 * Developers - enter code here
	 */
	public Vector createTabbedPanels(Hashtable comps)
	{
		//Method Vars
		Controller c;
		ActionPanel a;
		tabbedPanels = new Vector();
		Vector elements = new Vector();

		//Go through elements in the hash
		for(Enumeration e = comps.elements(); e.hasMoreElements();) 
		{
			c = (Controller)e.nextElement();
			if(!elements.contains(c.getBean().getName()))
			{
				elements.add(c.getBean().getName());
				a = c.getPanel();
				if(a != null)
					tabbedPanels.add(a);
			}
		}

		//lastly, set panels in gui
		return tabbedPanels;
	}

	/**
	 * Creates the Panels to be placed on the GUI
	 * Developers - enter code here
	 */
	public Vector createButtonPanels()
	{
		//Method Vars
		buttonPanels = new Vector();
		buttonPanels.add(new TopPanel(action));
		buttonPanels.add(new BottomPanel(bean, action, tabbedPanels));

		//lastly, set panels in gui
		return buttonPanels;
	}

	/**
	 * Calls Object Control, setting objects
	 */
	public void setSettings()
	{
		//Go through all the panels
		for(int i=0;i<tabbedPanels.size();i++)
			((ActionPanel)tabbedPanels.elementAt(i)).setSettings();
		for(int i=0;i<buttonPanels.size();i++)
			((ActionPanel)buttonPanels.elementAt(i)).setSettings();
	}	
	
	/**
	 * Calls Object Control, getting objects
	 */
	public void getSettings()
	{
		//Go through all the panels
		for(int i=0;i<tabbedPanels.size();i++)
			((ActionPanel)tabbedPanels.elementAt(i)).getSettings();
		for(int i=0;i<buttonPanels.size();i++)
			((ActionPanel)buttonPanels.elementAt(i)).getSettings();
	}		
	
	/**
	 * Calls Object Control, placing objects
	 */
	public void updateComponents(int amnt)
	{	
		//Go through all the panels
		for(int i=0;i<tabbedPanels.size();i++)
			((ActionPanel)tabbedPanels.elementAt(i)).updateComponents(amnt);
		for(int i=0;i<buttonPanels.size();i++)
			((ActionPanel)buttonPanels.elementAt(i)).updateComponents(amnt);
	}	
	
}//ObjectControl
