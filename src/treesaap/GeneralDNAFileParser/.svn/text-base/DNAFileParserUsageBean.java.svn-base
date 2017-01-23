/**
 *	DNAFileParserUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the general dna file parser.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.GeneralDNAFileParser;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class DNAFileParserUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private boolean progressWindow = true;
	private boolean allCaps = true;
	private double evenHanded = .5;
	private int gdfpVerbose = 7;
	
	//GeneralDNAFileParser Tracking VARIABLES
	private long charsRead;
	private long charsToRead;
	private long dataParsed;
	private long dataToParse;
	
	//Output vars
	private String verboseFile = "";

	/**
	 *  Get method - returns boolean progressWindow
	 */
	public boolean getProgressWindow()
	{
		return progressWindow;
	}
	
	/**
	 *  Set method - sets progressWindow to new boolean
	 *	@param ProgressWindow boolean of new progressWindow
	 */
	public void setProgressWindow(boolean ProgressWindow)
	{
		progressWindow = ProgressWindow;
	}
	
	/**
	 *  Get method - returns boolean allCaps
	 */
	public boolean getAllCaps()
	{
		return allCaps;
	}
	
	/**
	 *  Set method - sets allCaps to new boolean
	 *	@param newAllCaps boolean of new allCaps
	 */
	public void setAllCaps(boolean newAllCaps)
	{
		allCaps = newAllCaps;
	}
	
	/**
	 *  Get method - returns double evenHanded
	 */
	public double getEvenHanded()
	{
		return evenHanded;
	}
	
	/**
	 *  Set method - sets evenHanded to new double
	 *	@param setEvenHanded double of new evenHanded
	 */
	public void setEvenHanded(double newEvenHanded)
	{
		evenHanded = newEvenHanded;
	}
	
	/**
	 *  Get method - returns int gdfpVerbose
	 */
	public int getGdfpVerbose()
	{
		return gdfpVerbose;
	}
	
	/**
	 *  Set method - sets gdfpVerbose to new int
	 *	@param setGdfpVerbose int of new gdfpVerbose
	 */
	public void setGdfpVerbose(int newGdfpVerbose)
	{
		gdfpVerbose = newGdfpVerbose;
	}
	
	/**
	 *  Get method - returns long charsRead
	 */
	public long getCharsRead()
	{
		return charsRead;
	}
	
	/**
	 *  Set method - sets charsRead to new long
	 *	@param setCharsRead long of new charsRead
	 */
	public void setCharsRead(long newCharsRead)
	{
		charsRead = newCharsRead;
	}
	
	/**
	 *  Get method - returns long charsToRead
	 */
	public long getCharsToRead()
	{
		return charsToRead;
	}
	
	/**
	 *  Set method - sets charsToRead to new long
	 *	@param setCharsToRead long of new charsToRead
	 */
	public void setCharsToRead(long newCharsToRead)
	{
		charsToRead = newCharsToRead;
	}
	
	/**
	 *  Get method - returns long dataParsed
	 */
	public long getDataParsed()
	{
		return dataParsed;
	}
	
	/**
	 *  Set method - sets dataParsed to new long
	 *	@param setDataParsed long of new dataParsed
	 */
	public void setDataParsed(long newDataParsed)
	{
		dataParsed = newDataParsed;
	}

	/**
	 *  Get method - returns long dataToParse
	 */
	public long getDataToParse()
	{
		return dataToParse;
	}
	
	/**
	 *  Set method - sets dataToParse to new long
	 *	@param setDataToParse long of new dataToParse
	 */
	public void setDataToParse(long newDataToParse)
	{
		dataToParse = newDataToParse;
	}

	/**
	 *  Get method - returns String verboseFile
	 */
	public String getVerboseFile()
	{
		return verboseFile;
	}
	
	/**
	 *  Set method - sets verboseFile to new String passed in
	 *	@param VerboseFile String of new verboseFile
	 */
	public void setVerboseFile(String dir)
	{
		if(dir.equals(""))
			verboseFile = "";
		else
		{		
			verboseFile = getAbsoluteFilePath(dir + File.separator + "verbose.txt", true);
			
			//Error Control
			if(verboseFile == null)
				verboseFile = "";
		}
	}

	/**
	 * Verbose File input here.  Writes message to file.
	 *
	 * @param String message - the class specified message to be outputted to file
	 */
	public void verboseMessage(String message)
	{
		//Verify log message has been specified
		if(!verboseFile.equals(""))
		{
			try
			{
				//Open file and write message to it
				PrintWriter of = new PrintWriter(new FileWriter(verboseFile,true));
				of.println(message);
				of.flush();
				of.close();
			}
			catch(Exception exception) 
			{
				System.out.println("You had an IO exception while writing to Verbose file.\n\n     Try that again.");
			}
		}
	}
}//DNAFileParserUsageBean
