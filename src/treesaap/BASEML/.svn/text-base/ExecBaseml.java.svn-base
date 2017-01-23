/**
 *	ExecBaseml.class runs Baseml. 
 *	It executes a new process based on the command given it.
 *	It will output streaming data to gui, if that is the method
 *	exectuted.
 *
 *	@author	Joshua Sailsbery, Steve Wooley
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.BASEML;

import java.awt.Frame;
import java.awt.TextArea;
import java.awt.Container;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.BufferedInputStream;
import javax.swing.JDialog;

class ExecBaseml
{
	//Class Vars
	private BASEMLUsageBean bean;
	private boolean firstAttempt = true;
	
	/**
	 * Constructor - creates an instantiation of this class
	 * @param bean BASEMLUsageBean the bean for error messages
	 */	
	public ExecBaseml(BASEMLUsageBean Bean)
	{
		bean = Bean;
	}
	
	/**
	 * Method to be called to execute Baseml command
	 * with an output window - for use with GUI
	 *
	 * @param outWindow Frame output window to place streaming data from baseml
	 * @param command String The command to be executed - Run Baseml
	 */
 	public boolean exec(Frame outWindow, String command) 
	{
		//Check to see if this is the method to be ran
		if(outWindow == null)
			return exec(command);
			
		//Method Vars
		String stream = null;
		
		//Create Output Window and Associated Objects
		JDialog jd = new JDialog(outWindow, "Baseml Output", false);
		Container cp = jd.getContentPane();
		TextArea output = new TextArea(20,80);
		
		//Settings of Objects
		output.setEditable(false);
		cp.add(output);
		jd.pack();
		jd.show();
		jd.getParent().setVisible(true);

		//Try Catch PrintWriter
		try
		{
			//Create file to place all info from Baseml into
			PrintWriter of = new PrintWriter(new BufferedWriter(new FileWriter(bean.getOutFilePath() +"basemlLogFile.txt",false)));

			//Try Catch Process & Buffered Reader
			try 
			{
				//Run Process
				Process p = Runtime.getRuntime().exec(command);

				//Set-up Buffered Reader to obtain Streaming Data
				BufferedReader commandResult = new BufferedReader(new  InputStreamReader(new BufferedInputStream(p.getInputStream())));
				
				//Try Catch OutStream
				try 
				{
					//Read in a Line from the Stream
					while((stream  = commandResult.readLine()) != null)
					{
						//print line to file
						of.println(stream);
						
						//place line on the output window
						output.append("\n" + stream );
						output.update(output.getGraphics());
						cp.update(cp.getGraphics());
					}
					
					//If Baseml terminates before with errors
					if(p.exitValue() != 0) 
					{
						//output error to file
						of.println(command + " -- p.exitValue() != 0");
						
						//output error on GUI
						output.append("\n" + command + " -- p.exitValue() != 0");
						cp.update(cp.getGraphics());
						
						//close files and return
						of.close();
						return false;
					}
				} 
				//Error Catching Stream
				catch(Exception e){}	
			} 
			
			//Error With Process or Buffered Reader
			catch (Exception e) 
			{
				//only do this once
				if(firstAttempt)
				{
					//reset file permissions
					Process p2 = Runtime.getRuntime().exec("chmod +x "+ command.substring(0, command.lastIndexOf(" ")));
					(new Thread()).sleep(500);
					
					//reattempt to run baseml
					of.close();
					jd.dispose();
					firstAttempt = false;
					return exec(outWindow, command);
				}				
				else
				{
					//print error to file
					of.println(command + e);
					
					//close files and return
					of.close();
					return false;
				}
			}
			
			//Close File
			of.close();
		}

		//Error with PrintWriter File
		catch (Exception e) 
		{
			return false;
		}

		//get rid of frame
		jd.dispose();
		
		//return a success
		return true;
	
	}//exec

	/**
	 * Method to be called to execute Baseml command
	 * without an output window - for use without a GUI
	 *
	 * @param command String The command to be executed - Run Baseml
	 */
	private boolean exec(String command) 
	{		
		//Method Vars
		String stream = null;
		
		//Try Catch PrintWriter
		try
		{
			//Create file to place all info from Baseml into
			PrintWriter of = new PrintWriter(new BufferedWriter(new FileWriter(bean.getOutFilePath() +"basemlLogFile.txt",false)));
			
			//Try Catch Process & Buffered Reader
			try 
			{
				//Run Process
				Process p = Runtime.getRuntime().exec(command);
				
				//Set-up Buffered Reader to obtain Streaming Data
				BufferedReader commandResult = new BufferedReader(new  InputStreamReader(new BufferedInputStream(p.getInputStream())));
				
				//Try Catch OutStream
				try 
				{
					//Read in a Line from the Stream
					while((stream  = commandResult.readLine()) != null)
					{
						//print line to file
						of.println(stream);
					}
					
					//If Baseml terminates before with errors
					if(p.exitValue() != 0) 
					{
						//output error to file
						of.println(command + " -- p.exitValue() != 0");
						
						//close files and return
						of.close();
						return false;
					}
				} 
				//Error Catching Stream
				catch(Exception e){}	
			} 
			
			//Error With Process or Buffered Reader
			catch (Exception e) 
			{
				//only do this once
				if(firstAttempt)
				{
					//reset file permissions
					Process p2 = Runtime.getRuntime().exec("chmod +x "+ command.substring(0, command.lastIndexOf(" ")));
					(new Thread()).sleep(500);
					
					//reattempt to run baseml
					of.close();
					firstAttempt = false;
					return exec(command);
				}				
				else
				{
					//print error to file
					of.println(command + e);
					
					//close files and return
					of.close();
					return false;
				}
			}
			
			//Close File
			of.close();
		}
		
		//Error with PrintWriter File
		catch (Exception e) 
		{
			return false;
		}
		
		//return a success
		return true;
	}//exec

	/**
	 *  Set method - sets bean
	 *	@param Bean BASEMLUsageBean is the new bean 
	 */
	public void setBean(BASEMLUsageBean Bean)
	{
		bean = Bean;
	}//getBean 
	
}//ExecBaseml
