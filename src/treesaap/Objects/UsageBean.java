/**
 *	UsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of a specified class
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Objects;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

public class UsageBean
{
	//Error Message Variables
	public boolean errorWindow = false;
	public String inputFile = "";
	public String logFile = "";
	

	/**
	 *  Get method - returns String inputFile
	 */
	public String getInputFile()
	{
		return inputFile;
	}
	
	/**
	 *  Set method - sets inputFile to new String passed in
	 *	@param InputFile String of new inputFile
	 */
	public void setInputFile(String InputFile)
	{
		inputFile = getAbsoluteFilePath(InputFile, true);
		
		//Error Control
		if(inputFile == null)
			inputFile = "";
	}	
	
	/**
	 *  Set method - sets errorWindow to boolean passed in
	 *	@param setErrorWindow long of new errorWindow
	 */
	public void setErrorWindow(boolean newErrorWindow)
	{
		errorWindow = newErrorWindow;
	}

	/**
	 *  Set method - sets logFile to new String passed in
	 *	@param setLogFile String of new logFile
	 */
	public void setLogFile(String newLogFile)
	{
		if(newLogFile.equals(""))
			logFile = "";
		else
		{		
			logFile = getAbsoluteFilePath(newLogFile, true);
			
			//Error Control
			if(logFile == null)
				logFile = "";
		}
	}
	
	/**
	 * Error input here.  Determines how to inform user
	 *
	 * @param String message - the class specified message to be outputted to user
	 */
	public void errorMessage(String message)
	{
		//System out error message
		System.out.println(message);
		
		//Interface desired
		if(errorWindow)
		{
			JFrame this_frame = new JFrame();
			JOptionPane.showMessageDialog(this_frame, message, "ERROR", JOptionPane.ERROR_MESSAGE);		
			this_frame.dispose();
		}
		
		//Call log file
		logMessage(message);		
	}//errorMessage	

	/**
	 * Log File input here.  Writes message to file.
	 *
	 * @param String message - the class specified message to be outputted to file
	 */
	public void logMessage(String message)
	{
		//Verify log message has been specified
		if(!logFile.equals(""))
		{
			try
			{
				//Open file and write message to it
				PrintWriter of = new PrintWriter(new FileWriter(logFile,true));
				of.print(message);
				if(!message.equals("*"))
					of.println();
				of.flush();
				of.close();
			}
			catch(Exception exception) 
			{
				System.out.println("You had an IO exception while writing to LOG file.\n\n     Try that again.");
			}
		}
	} 	

	/**
	 * Validates any file path - syntax is correct.
	 * Will create path to the file (not the file itself) if specified.
	 * Returns null if invalid path, or not created.
	 *
	 * @param String path - the file path
	 * @param boolean createPath - whether to create the path to file or not
	 */
	public String getAbsoluteFilePath(String path, boolean createPath)
	{
		try
		{
			//File object of path
			File file = new File(path);
			
			//See if file exists
			if(!file.exists())
			{
				//Simple exists check, no validation of path
				if(!createPath)
					return null;
				
				//File does not exist, now create the path to it
				else
				{
					file.mkdirs();
					file.delete();
				}
			}
			
			//Verify file is a file, not a directory
			else if(file.isDirectory())
			{
				System.out.println(path);
				return null;
			}
			
			return file.getCanonicalPath();
		}
		catch(Exception e)
		{
			errorMessage("The specified File path ("+ path +") could not be validated.\n  Please check Operating System rules and try again.\n");
			return null;
		}
	}
	
	/**
	 * Validates any directory path - syntax is correct.
	 * Will create path to the directory, including the directory.
	 * Returns null if invalid path, or not created.
	 *
	 * @param String path - the dir path
	 */
	public String getAbsoluteDirPath(String path)
	{
		try
		{
			//File object of path
			File file = new File(path);
			
			//See if file exists, if it doesn't create a path
			if(!file.exists())
				file.mkdirs();
			
			//Verify file is a file, not a directory
			else if(!file.isDirectory())
				return null;
			
			return file.getCanonicalPath();
		}
		catch(Exception e)
		{
			errorMessage("The specified Directory path ("+ path +") could not be validated.\n  Please check Operating System rules and try again.\n");
			return null;
		}
	}

}//UsageBean
