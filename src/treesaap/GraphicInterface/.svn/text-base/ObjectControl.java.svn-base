/**
 *	ObjectControl.class is an instantiated object that contains
 *	all the developer-defined objects to be placed on the GUI
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface;

import treesaap.Driver.Driver;
import treesaap.GraphicInterface.Objects.ActionMenuItem;
import treesaap.GraphicInterface.Objects.ToolTipButton;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Panels.CDMPanel;
import treesaap.GraphicInterface.Panels.RunPanel;
import treesaap.GraphicInterface.Panels.TaxaPanel;
import treesaap.GraphicInterface.Panels.TreePanel;
import treesaap.GraphicInterface.Panels.BasemlPanel;
import treesaap.GraphicInterface.Panels.ParserPanel;
import treesaap.GraphicInterface.Panels.EvpthwyPanel;
import treesaap.GraphicInterface.Panels.BranchesPanel;
import treesaap.GraphicInterface.Panels.PropertyPanel;
import treesaap.GraphicInterface.Panels.CorrelatePanel;
import treesaap.GraphicInterface.Panels.OperationPanel;
import treesaap.GraphicInterface.Panels.FilePanel;

import java.util.Vector;
import java.util.Enumeration;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Color;
import java.awt.event.KeyEvent;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;
import javax.swing.ButtonGroup;
import javax.swing.JRadioButtonMenuItem;

public class ObjectControl
{
	//Class Variables - Created Objects
	private Object driver;						//Object of the driver class
	private ActionControl action;				//Object that controls actions from GUI
	private GraphicInterfaceUsageBean bean;		//Bean created and populated by this class
	
	//Created Vars - to be placed on GUI
	private Vector panels;						//Vector of panel objects to be placed on GUI
	private Vector buttons;						//Vector of button objects to be placed on GUI
	private Vector menuItems;					//Vector of JMenuItems objects to be placed on GUI	
	private ButtonGroup geneticCodeGroup;		//Button Group of the genetic codes

	/**
	 *	ObjectControl Constructor - Instantiates Class Objects.	
	 *
	 *	@param Object Driver - the driver class, must have access
	 *	@param ActionControl act - contains the action class
	 *	@param GraphicInterfaceUsageBean bean - class created with this bean
	 */
	public ObjectControl(Object Driver, ActionControl act, GraphicInterfaceUsageBean Bean)
	{
		bean = Bean;	
		action = act;
		driver = Driver;
		
		createMenuItems();
		createButtons();
		createPanels();
		
	}//constructor

	/**
	 * Creates the JMenuItems to be placed on the GUI
	 * Developers - enter code here
	 */
	private void createMenuItems()
	{
            
		//Method Vars
		JMenu menu;
		ActionMenuItem item;
		menuItems = new Vector();

		//File added to the menu bar
		menu = new JMenu("File");
		menu.setFont(new Font("Helvetica", Font.PLAIN, 12));
		menuItems.add(menu);
		
			//openNew - opens new sequence file
			item = new ActionMenuItem("Open New File");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("openNewFile");
			item.addActionListener(action);
			menu.add(item);
			
			//openAddtional - opens additional sequence file
			item = new ActionMenuItem("Open Additional File");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("openFile");
			item.addActionListener(action);
			menu.add(item);
			
			//openTree - opens TCS Tree
			item = new ActionMenuItem("Open TCS Tree");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_T, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("openTree");
			item.addActionListener(action);
			menu.add(item);
			menu.addSeparator();
				
			//close - close files
			item = new ActionMenuItem("Close Files");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("close");
			item.addActionListener(action);
			menu.add(item);
			
			//exit - exit program
			item = new ActionMenuItem("Exit");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("exit");
			item.addActionListener(action);
			menu.add(item);
		
		//Code added to menu bar
		menu = new JMenu("Genetic Codes");
		menu.setFont(new Font("Helvetica", Font.PLAIN, 12));
		menuItems.add(menu);
			
			//Make code choice into a button group
			Vector gcNames = ((Driver)driver).getData().getGeneticCodeNames();
			geneticCodeGroup = new ButtonGroup();	
			
			//go through genetic codes, add them to menu
			for(int i=0;i<gcNames.size();i++)
			{
				JRadioButtonMenuItem rb = new JRadioButtonMenuItem((String)gcNames.elementAt(i));
				if(((Driver)driver).getBean().getGeneticCode().equalsIgnoreCase((String)gcNames.elementAt(i)))	
					rb.setSelected(true);
				rb.setFont(new Font("Helvetica", Font.PLAIN, 12));
				geneticCodeGroup.add(rb);
				menu.add(rb);
			}
			
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
		
			//help - opens new help file
			item = new ActionMenuItem("Help File");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_H,java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("helpFile");
			item.addActionListener(action);
			menu.add(item);
			
			//documentation - opens documentation file
			item = new ActionMenuItem("Documentation");
			item.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_D,java.awt.Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));	
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("documentation");
			item.addActionListener(action);
			menu.add(item);
		
			//credit - opens acreditation file
			item = new ActionMenuItem("Credits");
			item.setFont(new Font("Helvetica", Font.PLAIN, 12));
			item.setActionName("credit");
			item.addActionListener(action);
			menu.add(item);	
	}
	
	/**
	 * Creates the Buttons to be placed on the GUI
	 * Developers - enter code here
	 */
	private void createButtons()
	{
		//Method Vars
		ImageIcon icon;
		ToolTipButton t;
		buttons = new Vector();

		//Open New File Icon
		icon = new ImageIcon(bean.getIconPath()+"openNewFileIcon.gif");
		t = new ToolTipButton(icon, "Open New File");
		t.setActionName("openNewFile");
		t.addActionListener(action);
		t.setSeparated(false);
		buttons.add(t);
		
		//Open File Icon
		icon = new ImageIcon(bean.getIconPath()+"openFileIcon.gif");
		t = new ToolTipButton(icon, "Open File");
		t.setActionName("openFile");
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
	}
	
	/**
	 * Creates the Panels to be placed on the GUI
	 * Developers - enter code here
	 */
	private void createPanels()
	{
		//Method Vars
		panels = new Vector();
		
		//go through panels, place objects
                panels.add(new FilePanel(driver,action));
		panels.add(new ParserPanel(driver, action));	
		panels.add(new TreePanel(driver, action));	
		panels.add(new TaxaPanel(driver, action));	
		panels.add(new PropertyPanel(driver, action, bean));
		panels.add(new BasemlPanel(driver, action));	
//		panels.add(new CDMPanel(driver, action));
		panels.add(new EvpthwyPanel(driver, action));
		panels.add(new OperationPanel(driver, action));
		panels.add(new RunPanel(driver, action));
//		panels.add(new CorrelatePanel(driver, action));
		panels.add(new BranchesPanel(driver, action));

		//lastly, set objects
		setObjects();
	}
	
	/**
	 * Calls objects with set methods
	 */
	public void setObjects()
	{
		//set panels
		for(int i=0;i<panels.size();i++)
			((ActionPanel)panels.elementAt(i)).setValues();
		
		//set geneticCode
		JRadioButtonMenuItem jb;
		Enumeration tokens = geneticCodeGroup.getElements();
		while(tokens.hasMoreElements())
		{
			jb = (JRadioButtonMenuItem)tokens.nextElement();
			if(jb.getText().equals(((Driver)driver).getBean().getGeneticCode()))
				jb.setSelected(true);
		}
		
		//WindowSize
		GraphicInterface gui = ((Driver)driver).getGUI();
		if(gui != null)
		{
			//set full screen
			if(bean.getFullScreen())
				gui.setExtendedState(Frame.MAXIMIZED_BOTH);
			else
				gui.setSize(bean.getLength(), bean.getHeight());
			
			//Repaint and show the gui
			gui.repaint();
			gui.show();
		}
	}	
	
	/**
	 * Calls objects with get methods
	 */
	public void getObjects()
	{
		//get from panels
		for(int i=0;i<panels.size();i++)
			((ActionPanel)panels.elementAt(i)).getValues();
		
		//get geneticCode
		JRadioButtonMenuItem jb;
		Enumeration tokens = geneticCodeGroup.getElements();
		while(tokens.hasMoreElements())
		{
			jb = (JRadioButtonMenuItem)tokens.nextElement();			
			if(jb.isSelected())
				((Driver)driver).getBean().setGeneticCode(jb.getText());
		}
		
		//WindowSize
		GraphicInterface gui = ((Driver)driver).getGUI();		
		if(gui != null)
		{
			//set full screen
			if(gui.getExtendedState() == Frame.MAXIMIZED_BOTH)
				bean.setFullScreen(true);
			else
			{
				bean.setFullScreen(false);
				bean.setHeight(gui.getHeight());
				bean.setLength(gui.getWidth());
			}
		}
	}	
	
	/**
	 * Calls objects with place methods
	 */
	public void placeObjects()
	{
		for(int i=0;i<panels.size();i++)
			((ActionPanel)panels.elementAt(i)).place();
	}	
	
	/**
	 * Calls objects with display methods
	 */
	public void displayObject(String name)
	{
		for(int i=0;i<panels.size();i++)
			((ActionPanel)panels.elementAt(i)).displayObject(name);
	}
	
	/**
	 * Returns the Vector of user settings for panels
	 */
	public Vector getPanels()
	{
		return panels;
	}
	
	/**
	 * Returns the Vector of user settings for buttons
	 */
	public Vector getButtons()
	{
		return buttons;
	}
	
	/**
	 * Returns the Vector of user settings for menuItems
	 */
	public Vector getMenuItems()
	{
		return menuItems;
	}
	
}//ObjectControl
