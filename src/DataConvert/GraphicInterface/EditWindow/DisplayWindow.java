/**
 *	GeneWindow displays genes for editing purposes
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface.EditWindow;

import DataConvert.Objects.TaxaBean;
import DataConvert.GraphicInterface.ActionControl;
import DataConvert.GraphicInterface.Objects.ToolTipButton;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import java.awt.BorderLayout;
import javax.swing.border.BevelBorder;
import java.awt.Font;
import java.awt.Color;
import java.awt.Rectangle;
import java.awt.Dimension;

public class DisplayWindow extends JFrame
{
	//Class Variables - Created Objects
	private TaxaBean tb;
	private JTextArea ta;
	private ActionControl action;
	
	/**
	 *	GraphicInterface Constructor - Instantiates Class Objects.	
	 */
	public DisplayWindow(ActionControl act)
	{
		action = act;
	}//constructor

	public void save()
	{
		//Get sequence
		String seq = ta.getText();
	
		//Check for change
		if(seq != null && !seq.equals("") && tb != null)
			tb.setSequence(seq);
	}
	
	/**
	 * Builds the GUI
	 */
	public void setTB(TaxaBean TB)
	{
		//Check TB
		if(TB == null)
			return;
		
		//Set vars
		tb = TB;
		
		//set Items on GUI
		setButtonPanel();
		setTextArea(tb.getSequence());
		
		//Add on additional features
		setTitle("Sequence of "+ tb.getName());

		//Package up the GUI
		pack();

		//Visualize the GUI
		setVisible(true);
	}

	/**
	 * Places panels on the GUI
	 */
	private void setButtonPanel()
	{
		//Method Vars
		JPanel buttonPanel = new JPanel();
		buttonPanel.setBackground(Color.white);
		
		//Update button
		ToolTipButton save = new ToolTipButton("Save", "Save Changes to Sequence");
		save.addActionListener(action);
		save.setActionName("saveSeq");
		buttonPanel.add(save);

		//add panels to content pane
		getContentPane().add(buttonPanel, BorderLayout.NORTH);
	}	

	/**
	 * Sets the table panel from the row vector
	 */
	private void setTextArea(String seq)
	{		
		//Method vars
		int chars = seq.length();
		int width = 400;
		int height = chars/400 + 1;
		
		//set jframe
		if(height < 500)
			setBounds(new Rectangle(400, 400, width + 50, height + 50));
		else
			setBounds(new Rectangle(400, 400, width + 50, 500));
		
		//Text Area
		ta = new JTextArea(seq);
		ta.setFont(new Font("Courier", Font.PLAIN, 12));
		ta.setSize(new Dimension(width, height));
		ta.setLineWrap(true);
		
		//Table Panel
		JPanel tablePanel = new JPanel();
		tablePanel.setBackground(Color.white);
		tablePanel.setBorder(new BevelBorder(BevelBorder.RAISED,Color.lightGray,Color.black));
		tablePanel.add(ta);
		
		//Scroll Panel		
		JScrollPane jsp = new JScrollPane(tablePanel);
		jsp.getHorizontalScrollBar().setUnitIncrement(10);
		jsp.getVerticalScrollBar().setUnitIncrement(10);
		
		getContentPane().add(jsp, BorderLayout.CENTER);
	}

}//GraphicInterface
