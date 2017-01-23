/*
 * Temporary way of isolating the 
 */

package treesaap.GraphicInterface.Panels;

import treesaap.Driver.Driver;
import treesaap.GraphicInterface.ActionControl;
import treesaap.GraphicInterface.GraphicInterfaceUsageBean;
import treesaap.GraphicInterface.Objects.ActionPanel;
import treesaap.GraphicInterface.Objects.ToolTipButton;
import java.awt.Color;
import java.awt.GridBagLayout;
import java.awt.LayoutManager;
import java.util.Vector;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

/**
 *
 * @author Jay
 */
public class FilePanel extends ActionPanel{
    
    
public FilePanel() {}

    Vector buttons;
    ActionControl action;

    	public FilePanel(Object driver, ActionControl act)
	{
		setAction(act);
		setDriver(driver);
                createObjects();
	}
        
        
        	/**
	 * Creates the Buttons to be placed on the GUI
	 * Developers - enter code here
	 */
 
	public void create()
	{
            		//set Background
		setBorder(new BevelBorder(BevelBorder.RAISED,Color.lightGray,Color.black));
		setBackground(Color.white);
		setTabName("File");
		setIconName("saveIcon.gif");
		setToolTip("File Options");
            
		//Method Vars
		ImageIcon icon;
		ToolTipButton item;
		JPanel panel;
                Border border;
                
                JLabel label = new JLabel("");
		LayoutManager mgr;
                
                
                
                
                border = new TitledBorder("File");
                mgr = new GridBagLayout();
                panel = new JPanel();
                panel.setBorder(border);
                
                panel.setLayout(mgr);

                //openNew - opens new sequence file
                item = new ToolTipButton("Open New","Open New File");
                item.setActionName("openNewFile");
                item.addActionListener(getAction());
                panel.add(item);

			//openAddtional - opens additional sequence file
			item = new ToolTipButton("Open Additional", "Open Additional File");
			item.setActionName("openFile");
			item.addActionListener(getAction());
			panel.add(item);
			
			//openTree - opens TCS Tree
			item = new ToolTipButton("Open TCS", "Open TCS Tree");
			item.setActionName("openTree");
			item.addActionListener(getAction());
			panel.add(item);
			
                        //menu.addSeparator();
				
			//close - close files
			item = new ToolTipButton("Close", "Close Files");
			item.setActionName("close");
			item.addActionListener(action);
			panel.add(item);
			
                addItem(panel, endLine,2,endLine);
                border = new TitledBorder("Genetic Codes");
                panel = new JPanel();
                panel.setBorder(border);
 
                panel.setLayout(mgr);
				
		//Code added to menu bar
			
			//Make code choice into a button group
			Vector gcNames = ((Driver)driver).getData().getGeneticCodeNames();
			ButtonGroup geneticCodeGroup = new ButtonGroup();	
			
                        
			//go through genetic codes, add them to menu
			for(int i=0;i<gcNames.size();i++)
			{
				JRadioButton rb = new JRadioButton((String)gcNames.elementAt(i));
				if(((Driver)driver).getBean().getGeneticCode().equalsIgnoreCase((String)gcNames.elementAt(i)))	
					rb.setSelected(true);
				geneticCodeGroup.add(rb);
				panel.add(rb);
			}
			addItem(panel, endLine,2,endLine);
                       
                        
                       
		//Help added to the menu bar
                border = new TitledBorder("Settings");
                panel = new JPanel();
                panel.setBorder(border);
 
                panel.setLayout(mgr);
			
			//help - opens new help file
			item = new ToolTipButton("Save","Save Settings");
			item.setActionName("save");
			item.addActionListener(getAction());
			panel.add(item);
			
			//documentation - opens documentation file
			item = new ToolTipButton("Reset","Reset Settings");
			item.setActionName("reset");
			item.addActionListener(getAction());
			panel.add(item);
//			menu.addSeparator();
			
			//credit - opens acreditation file
			item = new ToolTipButton("Reload","Reload Settings From File");
			item.setActionName("reload");
			item.addActionListener(getAction());
			panel.add(item);		
                        
                        addItem(panel, endLine,2,endLine);
			
		//Help added to the menu bar
                border = new TitledBorder("Help");
                panel = new JPanel();
                panel.setBorder(border);
 
                panel.setLayout(mgr);
			//help - opens new help file
			item = new ToolTipButton("Help","Help File");
			item.setActionName("helpFile");
			item.addActionListener(getAction());
			panel.add(item);
			
			//documentation - opens documentation file
			item = new ToolTipButton("Documentation","Documentation");
			item.setActionName("documentation");
			item.addActionListener(getAction());
			panel.add(item);
		
			//credit - opens acreditation file
			item = new ToolTipButton("Credits","Credits");
			item.setActionName("credit");
			item.addActionListener(getAction());
			panel.add(item);	
                        
                                                addItem(panel, endLine,2,endLine);
                        
	}
        
    
}
