/**
 *	GeneWindow displays genes for editing purposes
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface.EditWindow;

import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.GraphicInterface.ActionControl;
import DataConvert.GraphicInterface.Objects.ToolTipButton;

import java.util.Vector;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JCheckBox;
import javax.swing.JTextField;
import javax.swing.JSeparator;
import javax.swing.JScrollPane;
import javax.swing.border.BevelBorder;
import java.awt.Color;
import java.awt.GridLayout;
import java.awt.BorderLayout;

public class GeneWindow extends JFrame
{
	//Class Variables - Created Objects
	private Vector rows;
	private JScrollPane jsp;
	private JTextField newTaxa;
	private TaxaBeanManager tbm;
	private ActionControl action;
	private GeneWindowStub stub;
	private GeneWindowUsageBean bean;
	
	/**
	 *	GraphicInterface Constructor - Instantiates Class Objects.	
	 */
	public GeneWindow(String title, ActionControl act) throws Exception
	{
		//get values from file
		stub = new GeneWindowStub("DataConvert/GraphicInterface/EditWindow/user_settings.txt");
		bean = (GeneWindowUsageBean)stub.getBean();
		action = act;
		build(title);
		
	}//constructor
	
	/**
	 *	GraphicInterface Constructor - Instantiates Class Objects.	
	 */
	public GeneWindow(GeneWindowUsageBean Bean, String title, ActionControl act) throws Exception
	{
		//set vars
		bean = Bean;
		action = act;
		build(title);
			
	}//constructor
	
	/**
	 * Updates the tbm according to user input
	 */
	public void update()
	{
		//Method Vars
		Row r;
		TaxaBean t;
		String name;
		Vector names = new Vector();
		
		//Set names to change
		for(int i=0;i<rows.size();i++)
		{
			t = tbm.get(i);
			r = (Row)rows.elementAt(i);
			if(!t.getName().equals(r.getName().getText()))
			{
				names.add(t.getName());
				names.add(r.getName().getText());
			}
		}

		try
		{
			//Compare vectors
			for(int i=0;i<names.size();i+=2)
			{
				tbm.updateName((String)names.elementAt(i), (String)names.elementAt(i+1), true);
			}
		}
		catch(Exception e){}
		
		//update gui
		setTBM(tbm);
	}
	
	/**
	 * Deletes the specified genes
	 */
	public void delete()
	{
		//Remove from tbm, selected taxa
		Row r;
		for(int i=rows.size()-1;i>=0;i--)
		{
			r = (Row)rows.elementAt(i);
			if(r.getBox().isSelected())
				tbm.remove(i);
		}
		
		//update gui
		setTBM(tbm);
	}
	
	/**
	 * Adds to the tbm the name specified in the add field
	 */
	public void add()
	{
		//Add to TBM
		try{
			tbm.add(newTaxa.getText(), " ");
			newTaxa.setText("");
		}catch(Exception e){e.printStackTrace();}

		//Reset the GUI
		setTBM(tbm);
	}
	
	/**
	 * Builds the GUI
	 */
	private void build(String title)
	{
		//set Items on GUI
		setPanels();
		
		//Add on additional features
		setBounds(bean.getPosition());
		setTitle(title);
		
		//Package up the GUI
		pack();

		//Visualize the GUI
		setVisible(true);
	}

	/**
	 * Places panels on the GUI
	 */
	private void setPanels()
	{
		//Method Vars
		JPanel buttonPanel = new JPanel();
		buttonPanel.setBackground(Color.white);
		
		//Update button
		ToolTipButton update = new ToolTipButton("Update", "Update Gene Field Values");
		update.addActionListener(action);
		update.setActionName("update");
		buttonPanel.add(update);
		
		JSeparator s = new JSeparator();
		buttonPanel.add(s);
		
		//Delete Button
		ToolTipButton delete = new ToolTipButton("Delete", "Delete Selected Genes");
		delete.addActionListener(action);
		delete.setActionName("delete");
		buttonPanel.add(delete);
		
		s = new JSeparator();
		buttonPanel.add(s);
		s = new JSeparator();
		buttonPanel.add(s);
		s = new JSeparator();
		buttonPanel.add(s);
		
		//Add field
		newTaxa = new JTextField("", 13);
		buttonPanel.add(newTaxa);
		
		//Add button
		ToolTipButton add = new ToolTipButton("Add", "Add this Gene");
		add.addActionListener(action);
		add.setActionName("add");
		buttonPanel.add(add);
		
		/*s = new JSeparator();
		buttonPanel.add(s);
		s = new JSeparator();
		buttonPanel.add(s);
		s = new JSeparator();
		buttonPanel.add(s);
		s = new JSeparator();
		buttonPanel.add(s);
		s = new JSeparator();
		buttonPanel.add(s);
		
		//GeneBank Button
		ToolTipButton GeneBank = new ToolTipButton("GeneBank", "Use a GeneBank table");
		GeneBank.addActionListener(action);
		GeneBank.setActionName("GeneBank");
		buttonPanel.add(GeneBank);*/

		//add panels to content pane
		getContentPane().add(buttonPanel, BorderLayout.NORTH);
	}	

	/**
	 * Sets the values from the given tbm onto the tablePanel
	 */
	public void setTBM(TaxaBeanManager TBM)
	{
		//set tbm
		if(TBM == null)
			return;
		
		//set tbm & display table
		tbm = TBM;
		initTaxaVector();
		setTablePanel();
		
		//set the bounds
		setBounds(bean.getPosition());
		setTitle(tbm.size() +" "+getTitle().substring(getTitle().indexOf(" ")+1));
	}
	
	/**
	 * Initializes the vector of row objects for table
	 */
	private void initTaxaVector()
	{
		//Method vars
		TaxaBean bean;
		
		//init rows
		rows = new Vector();
		
		//go through elements in tbm
		for(int i=0;i<tbm.size();i++)
		{
			bean = tbm.get(i);
			rows.add(new Row(i, bean.getName()));
		}
	}

	/**
	 * Sets the table panel from the row vector
	 */
	private void setTablePanel()
	{		
		//Method vars
		Row r;
		JPanel jp;
		
		//Table Panel
		JPanel tablePanel = new JPanel(new GridLayout(rows.size(), 1));
		tablePanel.setBackground(Color.white);
		tablePanel.setBorder(new BevelBorder(BevelBorder.RAISED,Color.lightGray,Color.black));
		
		//Go through row vector
		for(int i=0;i<rows.size();i++)
		{
			r = (Row)rows.elementAt(i);
			jp = new JPanel();
			jp.setBackground(Color.white);
			jp.add(r.getBox(), BorderLayout.WEST);
			jp.add(r.getName(), BorderLayout.CENTER);
			jp.add(r.getDisplay(), BorderLayout.EAST);
			tablePanel.add(jp);
		}
		
		//Scroll Panel		
		if(jsp != null)
			getContentPane().remove(jsp);
		jsp = new JScrollPane(tablePanel);
		jsp.getHorizontalScrollBar().setUnitIncrement(10);
		jsp.getVerticalScrollBar().setUnitIncrement(10);
		
		getContentPane().add(jsp, BorderLayout.CENTER);
		pack();
	}

	/**
	 * Write default values onto file
	 */
	public void writeSettings() throws Exception
	{
		bean.setPosition(getX(), getY(), getWidth(), getHeight());		
		stub.writeSettings();
	}
	
	/**
	 * Reset default values onto file
	 */
	public void resetSettings() throws Exception
	{
		stub.resetSettings();
	}
	
	/**
	 * Get the Usage Bean
	 */
	public GeneWindowUsageBean getBean()
	{
		return bean;
	}

	/**
	 * Bean class representing each row as displayed on panel
	 */
	class Row
	{
		//Class Vars
		int index;
		JCheckBox box;
		JTextField name;
		ToolTipButton display;
	
		//Constructor
		public Row(int Index, String Name)
		{
			index = Index;
			box = new JCheckBox();
			box.setBackground(Color.white);
			name = new JTextField(Name, 20);
			display = new ToolTipButton("Display Sequence", "Display the Sequence for this gene");
			display.addActionListener(action);
			display.setActionName("display("+index+")");
		}

		public JTextField getName(){
			return name;
		}
		public int getIndex(){
			return index;
		}
		public JCheckBox getBox(){
			return box;
		}
		public ToolTipButton getDisplay(){
			return display;
		}
	}

}//GraphicInterface
