/**
 *	HelpWindow displays help file
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface.EditWindow;

import java.io.FileReader;
import java.io.BufferedReader;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.ImageIcon;
import javax.swing.JScrollPane;
import java.awt.BorderLayout;
import javax.swing.border.BevelBorder;
import java.awt.Font;
import java.awt.Color;
import java.awt.Rectangle;

public class FileWindow extends JFrame
{
	/**
	 *	GraphicInterface Constructor - Instantiates Class Objects.	
	 *
	 *	@param Object Driver - the driver class, must have access
	 */
	public FileWindow(String filename, String icon, int x, int y, int width, int height)
	{
		//Set text area
		setTextArea(filename);
		
		//Set the frame up
		setTitle("Help");
		setBounds(new Rectangle(x, y, width, height));	
		setIconImage(new ImageIcon(icon).getImage());
		
		//Package & Visualize the GUI
		//pack();
		setVisible(true);
	}

	/**
	 * Sets the table panel from the row vector
	 */
	private void setTextArea(String filename)
	{		
		//Method vars
		StringBuffer text = new StringBuffer("");
		
		try{
			String line;
			BufferedReader inFile = new BufferedReader(new FileReader(filename));
			while((line = inFile.readLine()) != null)
				text.append("\n"+line);
			inFile.close();
		}catch(Exception e){}
		
		//Text Area
		JTextArea ta = new JTextArea(text.toString());
		ta.setFont(new Font("Courier", Font.PLAIN, 12));
	//	ta.setLineWrap(true);
		
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

}//FileWindow
