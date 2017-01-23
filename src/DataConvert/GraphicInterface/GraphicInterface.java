/**
 *	GraphicInterface.class is an instantiated object that creates
 *	and controls the operations of the GUI, as defined by a developer in
 *	the classes ActionControl and ObjectControl
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
/* I changed the psodaGui class name --Adam R. Teichert 20 Aug 2008 */
package DataConvert.GraphicInterface;

import DataConvert.DataConvert.Driver;
import DataConvert.Components.Controller;
import DataConvert.Objects.TaxaBean;
import DataConvert.Objects.TaxaBeanManager;
import DataConvert.GraphicInterface.Objects.ActionPanel;
import DataConvert.GraphicInterface.EditWindow.GeneWindow;
import DataConvert.GraphicInterface.EditWindow.FileWindow;
import DataConvert.GraphicInterface.EditWindow.DisplayWindow;

import java.io.File;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Enumeration;
import java.awt.FileDialog;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

import gui.*;

public class GraphicInterface
{
	//Class Vars
	private Viewer gui;							//The visualization of the GUI
	private Driver driver;						//The driver class (or parent of this class)
	private Vector openFiles;					//The currently open files
	private GraphicInterfaceStub stub;			//Stub of the GUI
	private GraphicInterfaceUsageBean bean;		//Bean created and populated by this class
	private GeneWindow geneWindow;				//To display genes read in
	private FileWindow helpWindow;				//The window to display help file
	private DisplayWindow disSeq;				//Displays a sequence of a gene
	private PSODA psodaGui;
	
	/**
	 *	GraphicInterface Constructor - Instantiates Class Objects.	
	 *
	 *	@param Object Driver - the driver class, must have access
	 */
	public GraphicInterface(Driver driver, String title)
	{
		init(driver, title);
	}

	/**
	 * Will set the PSODAGUI object so the file can be opened by PSODA
	 */
	public void setPsodaGui(PSODA newPsodaGui){
		psodaGui = newPsodaGui;
	}
	
	/**
	 * Initializes the gui, seperated from constructor to allow for reload method
	 */
	private void init(Driver newDriver, String title)
	{
		//set vars
		try{
			stub = new GraphicInterfaceStub("DataConvert/GraphicInterface/user_settings.txt");
			bean = (GraphicInterfaceUsageBean)stub.getBean();
			driver = newDriver;
		}
		catch(Exception e){
			bean = new GraphicInterfaceUsageBean();
		}		
		//Initialize open files
		openFiles = new Vector();
		
		//Create GUI
		gui = new Viewer(this, title);
		gui.update(gui.getGraphics());
		gui.repaint();
		gui.show();
	}
	
	/**
	 * Determines the action made by user
	 */
	public void command(String c)
	{
		//Determine Command
		if(c.equals("reload")){
			reload();
		}
		else if(c.equals("save")){
			save();
		}
		else if(c.equals("reset")){
			reset();
		}
		else if(c.equals("exit")){
			//System.exit(0);
		}
		else if(c.equals("close")){
			openFiles = new Vector();
			gui.updateComponents(0);
			if(geneWindow != null)
				geneWindow.dispose();
			if(disSeq != null)
				disSeq.dispose();
		}
		else if(c.equals("openFile")){
			openFile(false);
		}
		else if(c.equals("openDir")){
			openFile(true);
		}
		else if(c.equals("write")){
			write();
		}
		else if(c.equals("credit")){
			credits();
		}
		else if(c.equals("helpFile")){
			helpFile();
		}
		else if(c.equals("combine")){
			combine();
		}
		else if(c.equals("newFile")){
			newFile();
		}
		else if(c.equals("display")){
			display();
		}
		else if(c.equals("update") && geneWindow != null){
			geneWindow.update();
		}
		else if(c.equals("delete") && geneWindow != null){
			geneWindow.delete();
		}
		else if(c.equals("add") && geneWindow != null){
			geneWindow.add();
			display(((TaxaBeanManager)openFiles.elementAt(0)).size() - 1);
		}
		else if(c.indexOf("display(") != -1){
			display(Integer.valueOf(c.substring(c.indexOf("(")+1, c.indexOf(")"))).intValue());
		}
		else if(c.equals("saveSeq")){
			if(disSeq != null)
				disSeq.save();
		}
		else{
			System.out.println(c);
		}
	}
	
	/**
	 * Opens and displays Help File
	 */
	private void helpFile()
	{
		//Dispose one help file
		if(helpWindow != null)
			helpWindow.dispose();
		
		helpWindow = new FileWindow("DataConvert/help_file.txt", bean.getIconPath()+"helpFileIcon.gif", 600, 0, 400, 600);
	}

	/**
	 * Displays the Genes on Gene Window for editing
	 */
	private void newFile()
	{
		if(openFiles.size() == 0)
		{
			TaxaBeanManager mgr = new TaxaBeanManager();
			mgr.setFilename("new");
			openFiles.add(mgr);
			display();
			gui.updateComponents(openFiles.size());
		}
	}
	
	/**
	 * Displays the Genes on Gene Window for editing
	 */
	private void display(int index)
	{
		if(openFiles.size() == 1)
		{
			//Only can display sequence with latest open file
			TaxaBeanManager tbm = (TaxaBeanManager)openFiles.get(0);
	
			//Check index
			if(index < 0 || index >= tbm.size())
				return;
	
			if(disSeq == null)
			{
				disSeq = new DisplayWindow(gui.getAction());
			}
			else
			{
				DisplayWindow temp = new DisplayWindow(gui.getAction());
				disSeq.dispose();
				disSeq = temp;
			}
			disSeq.setTB((TaxaBean)tbm.get(index));
		}
	}	
	
	/**
	 * Displays the Genes on Gene Window for editing
	 */
	private void display()
	{
		if(openFiles.size() == 1)
		{
			try
			{
				int size = ((TaxaBeanManager)openFiles.elementAt(0)).size();
				
				if(geneWindow == null)
				{
					geneWindow = new GeneWindow(size +" Genes to Modify", gui.getAction());
				}
				else
				{
					GeneWindow temp = new GeneWindow(geneWindow.getBean(), size +" Genes to Modify", gui.getAction());
					geneWindow.dispose();
					geneWindow = temp;
				}
				geneWindow.setTBM((TaxaBeanManager)openFiles.elementAt(0));
			}
			catch(Exception e){}
		}
	}
	
	/**
	 * Combine the TaxaBeanManagers into one record
	 */
	private void combine()
	{
		//Verify enough results
		if(openFiles.size() < 2)
		{
			JOptionPane.showMessageDialog(gui, "Must have 2 or more files open", "ERROR", JOptionPane.ERROR_MESSAGE);		
			return;
		}
		
		try
		{
			Vector temp = openFiles;
			openFiles = new Vector();
			addToFiles(driver.combine(temp));
			
			if(geneWindow != null)
				geneWindow.dispose();
			if(disSeq != null)
				disSeq.dispose();
		}
		catch(Exception e){}
	}
	
	/**
	 * Displays the credits for this program
	 */
	private void credits()
	{
		String credit = 
			"DataConvert is being developed by:\n" +
			"     Matt Dyer\n"+
			"     Joshua Sailsbery\n"+
			"Under the Direction and Guidance of:\n"+
			"     Dr. David A. McClellan, Integrative Biology Dept. at BYU\n" +
			"     Email: david_mcclellan@byu.edu  Phone: (801) 422-1733\n\n"+
			"Data Conert may be distributed under the terms of the GNU General Public License, Version 2.\n" +
			"Versions for MacOS, Windows, and Linux may be downloaded at http://inbio.byu.edu/faculty/dam83/cdm\n\n"+
			"Thanks to all users that report problems!";

			JOptionPane.showMessageDialog(gui, credit, "Data Convert credits", JOptionPane.INFORMATION_MESSAGE);
	}
	
	/**
	 * Writes a file or dir
	 */
	private void write()
	{
		//get Settings from gui
		gui.getSettings();
		
		try{
			//get file name
			String filename;
			if(openFiles.size() > 1)
				filename = getFolder();
			else if(openFiles.size() == 1)
				filename = getFileName(false);
			else
				throw new Exception("There are no files open");
			if(filename != null && !filename.equals(""))
			{
				//Read file and add to results
				if(openFiles.size() == 1 && driver.write(bean.getFileType().toLowerCase(), filename, openFiles.elementAt(0))){
					JOptionPane.showMessageDialog(gui, "File Successfully Written", "DONE", JOptionPane.INFORMATION_MESSAGE);
					psodaGui.openDataConvertFile(filename);
				}
				else if(openFiles.size() > 1 && driver.write(bean.getFileType().toLowerCase(), filename, openFiles))
					JOptionPane.showMessageDialog(gui, "Files Successfully Written", "DONE", JOptionPane.INFORMATION_MESSAGE);		
				else
					throw new Exception("Could not write file(s) as specified");
			}
		}
		catch(Exception e){
			//Error Message
			JOptionPane.showMessageDialog(gui, e.toString(), "ERROR", JOptionPane.ERROR_MESSAGE);		
		}
	}		
	
	/**
	 * Opens a single file
	 */
	private void openFile(boolean dir)
	{
		//get Settings from gui
		gui.getSettings();
		
		try{
			//get file name
			String filename;
			if(dir)
				filename = getFolder();
			else
				filename = getFileName(true);
			if(filename != null && !filename.equals(""))
			{
				//Read file and add to results
				addToFiles(driver.open(filename));
				JOptionPane.showMessageDialog(gui, "File(s) Successfully Opened", "DONE", JOptionPane.INFORMATION_MESSAGE);	
			}
		}
		catch(Exception e){
			//Error Message
			JOptionPane.showMessageDialog(gui, e.toString(), "ERROR", JOptionPane.ERROR_MESSAGE);		
		}
	}		

	/*
	 * Opens a single file given the string to the file
	 */
	public void openFile(String path){
		//get Settings from gui
		gui.getSettings();
		
		try{
			if(path != null && !path.equals(""))
			{
				//Read file and add to results
				addToFiles(driver.open(path));
				JOptionPane.showMessageDialog(gui, "File(s) Successfully Opened", "DONE", JOptionPane.INFORMATION_MESSAGE);	
			}
		}
		catch(Exception e){
			//Error Message
			JOptionPane.showMessageDialog(gui, e.toString(), "ERROR", JOptionPane.ERROR_MESSAGE);		
		}

	}
	
	/**
	 * Keeps track of the open files
	 */
	private void addToFiles(Object o) throws Exception
	{
		//Initialize open files if null
		if(openFiles == null)
			openFiles = new Vector();
		
		//Validate open object
		if(o == null)
			throw new Exception("File of unknown type, could not open");
		
		//add files to openFiles
		if(driver.determineObjectType(o))
		{
			Vector files = (Vector)o;
			for(int i=0;i<files.size();i++)
				openFiles.add(files.elementAt(i));
		}
		else
			openFiles.add(o);
		
		//update components
		gui.updateComponents(openFiles.size());
	}
	
	/**
	 * Resets all the settings on gui to file
	 */
	private void reset()
	{
		//reset gui
		try{
			stub.resetSettings();
			if(geneWindow != null)
				geneWindow.resetSettings();
		}
		catch(Exception e){}
		
		//Go through all components, write settings to file
		Vector elements = new Vector();
		Hashtable comps = driver.getBean().getComponents();
		for(Enumeration e = comps.elements(); e.hasMoreElements();) 
		{
			Controller c = (Controller)e.nextElement();
			String name = c.getBean().getName();
			if(!elements.contains(name))
			{
				c.resetSettings();
				elements.add(name);
			}
		}
		
		//set settings
		gui.setSettings();
	}		
	
	/**
	 * Saves all the settings on gui to file
	 */
	private void save()
	{
		//get settings
		gui.getSettings();
		bean.setLength(gui.getWidth());
		bean.setHeight(gui.getHeight());

		//save gui settings
		try{
			stub.writeSettings();
			if(geneWindow != null)
				geneWindow.writeSettings();
		}
		catch(Exception e){}
		
		//Go through all components, write settings to file
		Vector elements = new Vector();
		Hashtable comps = driver.getBean().getComponents();
		for(Enumeration e = comps.elements(); e.hasMoreElements();) 
		{
			Controller c = (Controller)e.nextElement();
			String name = c.getBean().getName();
			if(!elements.contains(name))
			{
				c.writeSettings();
				elements.add(name);
			}
		}
	}	
		
	
	/**
	 * Reloads the GUI and Driver from file
	 */
	private void reload()
	{
		//Get rid of old gui
		String title = gui.getTitle();
		gui.dispose();

		if(geneWindow != null)
		{
			geneWindow.dispose();
			geneWindow = null;
		}
		
		//Create new one, and set gui in the driver
		init(new Driver(), title);
		driver.gui(this);
	}

	/**
	 * Prompts User to select File to open graphically
	 */
	private String getFileName(boolean load)
	{
		try 
		{
			//Use Java FileDialog to get user to input which file to open
			FileDialog fd;
			if(load)
				fd = new FileDialog(gui,"Choose a File to Load", FileDialog.LOAD);
			else
				fd = new FileDialog(gui,"Choose a File to Write", FileDialog.SAVE);
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
			//	bean.errorMessage("\nThere was a problem opening that file.\n\nPlease Check the format and try again.");
		}	
		return null;
	}	
	
	/**
	 * Prompts User to select Folder to open graphically
	 */
	private String getFolder()
	{
		try 
		{
			//Use Java VFSBrowser to get user to input which file to open
			JFileChooser fd = new JFileChooser();
			fd.setDialogTitle("Choose a Folder");
			
			fd.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			
			//choose file
			fd.showOpenDialog(gui);
			
			//return folderName obtained in JChooser
			String folderName = "";
			File file = fd.getSelectedFile();
			if(file != null)
				folderName = file.getAbsolutePath();
			return folderName;
		}
		
		catch(Exception e) 
		{
		//	bean.errorMessage("\nThere was a problem opening that folder.\n\nPlease Check the format and try again.");
		}	
		return null;
	}	

	/**
	 * Returns the components located on the driver
	 */
	public Hashtable getComponents()
	{
		return driver.getBean().getComponents();
	}
	
	/**
	 * Returns the UsageBean for this class
	 */
	public GraphicInterfaceUsageBean getBean()
	{
		return bean;
	}

}//GraphicInterface
