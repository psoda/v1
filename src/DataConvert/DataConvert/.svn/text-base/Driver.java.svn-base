package DataConvert.DataConvert;
/* I changed the psodaGui class name --Adam R. Teichert 20 Aug 2008 */

import java.io.File;
import java.io.IOException;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Enumeration;

import DataConvert.Objects.TaxaBeanManager;
import DataConvert.Components.Controller;
import DataConvert.GraphicInterface.GraphicInterface;
import DataConvert.FASTA.FASTA;
import DataConvert.MEGA.MEGA;
import DataConvert.NEXUS.NEXUS;
import DataConvert.CLUSTAL.CLUSTAL;
import DataConvert.RST.RST;
import DataConvert.TCS.TCS;
import DataConvert.PHYLIP.PHYLIP;
import DataConvert.XML.XML;
import DataConvert.GENBANKXML.GENBANKXML;
import DataConvert.GENBANKFLAT.GENBANKFLAT;
//import GENERAL.GENERAL;

import gui.*;


public class Driver
{
	//Program Vars
	static private Driver driver;
	private String title = "DataConvert 1.0";
	
	//Class Vars
	private GraphicInterface gui;
	private DriverUsageBean bean;
	private PSODA psodaGui;
	
	public static void main(String [] args)
	{
		//Create object of driver
		driver = new Driver();
		
		//Switch between commandLine and GUI
		if(args.length == 0)
			driver.gui(null);
		else if(args.length == 4 || args.length == 6)
			driver.commandLine(args);
		else
			driver.usage();
	}	
	
	public Driver()
	{
		driver = this;
		try
		{
			//Create a new bean
			bean = new DriverUsageBean();
			
			//Method Vars
			Vector components = new Vector();
			
			//add Components to vector
			components.add(new FASTA());
			components.add(new MEGA());
			components.add(new NEXUS());
			components.add(new CLUSTAL());
			components.add(new RST());
			components.add(new GENBANKXML());
			components.add(new XML());
			components.add(new TCS());
			components.add(new PHYLIP());
			components.add(new GENBANKFLAT());
			
			setComponents(components);
		}
		catch(Exception e)
		{
			System.out.println("Could not initialize file components");
			e.printStackTrace();
		}
	}	

	/*
	 * Will return the gui object
	 */
	public GraphicInterface getGuiObject(){
		return gui;
	}
	
	private void setComponents(Vector components)
	{
		//set new hashtable
		bean.setComponents(new Hashtable());
		
		//set each component into hashtable with extentions as keys
		for(int i=0;i<components.size();i++)
		{
			Controller c = (Controller)components.elementAt(i);
			bean.addComponent(c.getBean().getName().toLowerCase(), c);
			for(int j=0;j<c.getBean().getExtentions().length;j++)
				bean.addComponent(c.getBean().getExtentions()[j].toLowerCase(), c);
		}
	}

	public void setPsodaGui(PSODA newPsodaGui){
		gui.setPsodaGui(newPsodaGui);
	}
	
	public void gui(GraphicInterface GUI)
	{
		if(GUI == null){
			gui = new GraphicInterface(driver, title);
		}
		else
			gui = GUI;
	}
	
	private void commandLine(String[] args)
	{
		//Method Vars
		String 	in = "", out = "", type = "";
	
		//get vars from args
		for(int i=0;i<args.length;i++)
		{
			if(args[i].equals("-t"))
			{
				type = args[i+1];
				type = type.toLowerCase();
				i++;
			}
			else if(args[i].equals("-i"))
			{
				in = args[i+1];
				i++;
			}
			else if(args[i].equals("-o"))
			{
				out = args[i+1];
				i++;
			}
		}
		
		//Check conditions, Read in file(s) and Write them
		if(type.equals("") || bean.getComponent(type) == null || in.equals(""))
		{
			usage();
			return;
		}
		
		//Open file
		Object o;
		try{
			if((o = open(in)) == null)
				throw new Exception("\nInvalid input file specified: "+ in);
		}
		catch(Exception e)
		{
			e.printStackTrace();
			usage();
			return;
		}
		
		//Write file
		try{
			//Verify out
			if(out.equals(""))
			{
				if(determineObjectType(o))
					out = in;
				else
				{
					out = (new File(in)).getCanonicalPath();
					if(out.lastIndexOf(File.separator) != -1)
						out = out.substring(0, out.lastIndexOf(File.separator));
				}
			}

			if(!write(type, out, o))
				throw new Exception("\nInvalid output file specified: "+ out);
		}
		catch(Exception e)
		{
			e.printStackTrace();
			usage();
			return;
		}
	}
	
	private void usage()
	{
		//Method Vars
		Vector elements = new Vector();
		
		System.out.println("\n\nDataConvert Usage:");
		System.out.println("REQUIRED");
		System.out.println("-t [TYPE]	File Type, The file type to be written");
		System.out.println("-i [PATH/FILENAME]	Open File(s), File or Files in a Dir to be parsed");
		System.out.println("OPTIONAL:");
		System.out.println("-o [PATH/FILENAME]	Write File, File to be written or Directory for Files");
		
		System.out.println("\nDataConvert File TYPE(s):");
		for(Enumeration e = bean.getComponents().elements() ; e.hasMoreElements() ;) 
		{
			Controller c = (Controller)e.nextElement();
			String name = c.getBean().getName();
			if(!elements.contains(name))
			{
				System.out.print(name+"	");
				for(int i=0;i<c.getBean().getExtentions().length;i++)
					System.out.print(c.getBean().getExtentions()[i]+", ");
				System.out.println();
				elements.add(name);
			}
		}
	}

	public Object open(String in) throws IOException
	{
		//Verify in
		if(in == null || in.equals(""))
			throw new IOException("Invalid input file specified: "+ in);
	
		//Validate file or Dir
		String file;
		Read r = new Read(bean);
		if((file = bean.getAbsoluteFilePath(in, false)) != null)
		{
			return r.open(in);
		}
		else if((file = bean.getAbsoluteDirPath(in, false)) != null)
		{
			return r.openDir(in);
		}
		return null;
	}

	public boolean write(String type, String out, Object in) throws IOException
	{
		//Verify type
		if(type == null || type.equals("") || bean.getComponent(type) == null)
			throw new IOException("Invalid output file type specified: "+ type);
		
		//Verify out
		if(out == null || out.equals(""))
			throw new IOException("Invalid output file specified.");
		
		//Verify in
		boolean isDir = determineObjectType(in);
		
		//Switch on directory
		String file;
		Write w = new Write(bean);
		if(!isDir)
		{
			TaxaBeanManager tbm = (TaxaBeanManager)in;
			if((file = bean.getAbsoluteFilePath(out, true)) != null)
			{
				return w.set(type, file, tbm);
			}
			else if((file = bean.getAbsoluteDirPath(out, true)) != null)
			{
				return w.set(type, (file + tbm.getFilename() +"."+ type), tbm);
			}
		}
		else
		{
			Vector tbms = (Vector)in;
			if((file = bean.getAbsoluteDirPath(out, true)) != null)
			{
				return w.setDir(type, out, tbms);
			}
			else if((file = bean.getAbsoluteFilePath(out, true)) != null)
			{
				throw new IOException("Cannot Write a Directory to a file");
			}
		}

		return false;
	}

	public boolean determineObjectType(Object in) throws IOException
	{
		//Verify in object
		if(in == null)
			throw new IOException("Invalid Object. Cannot write outfile(s).");
		
		//Determine in object type
		try
		{
			Vector v = (Vector)in;
			return true;
		}
		catch(Exception e){}
		
		try
		{
			TaxaBeanManager tbm = (TaxaBeanManager)in;
			return false;
		}
		catch(Exception ex){}
		
		throw new IOException("Invalid Object.  Cannot write outfile(s).");
	}

	public TaxaBeanManager combine(Vector tbms)
	{
		//Check tbms
		if(tbms == null || tbms.size() == 0)
			return null;
		
		//Method vars
		TaxaBeanManager tbm = (TaxaBeanManager)tbms.elementAt(0);
		
		//Go through vector of tbms, combine them
		for(int i=1;i<tbms.size();i++)
			tbm.absorb((TaxaBeanManager)tbms.elementAt(i));

		return tbm;
	}
	
	public DriverUsageBean getBean()
	{
		return bean;
	}
}
