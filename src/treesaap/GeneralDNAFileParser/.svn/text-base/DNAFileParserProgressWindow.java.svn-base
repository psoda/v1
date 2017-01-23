/**
 *	DNAFileParserProgressWindow.class is responsible for the output
 *	of the progress in opening the file.  Allows the user to track 
 *	effectiveness of program.
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GeneralDNAFileParser;

import java.util.Timer;
import java.util.TimerTask;
import java.awt.Container;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JProgressBar;

class DNAFileParserProgressWindow
{
	//Shared Objects
	private DNAFileParserUsageBean bean;	//UsageBean to identify user selections
	
	//Class Vars used in building and running progress Window
	private JFrame frame;	
	private Container contentPane;					//The content pane of window
	private JLabel charsProgressLabel;					//labels to be placed on window
	private JLabel dataProgressLabel;					//labels to be placed on window
	private JLabel dataProgressValue;					//labels to be placed on window
	private JProgressBar charsProgressBar;				//A progress bar
	private JProgressBar dataProgressBar;				//A progress bar
	
	//Running Vars
	private boolean charDone, print;
	private int charNum, dataNum, step;
	private ProgressWindowThread windowThread;
	
	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * calls method to initialize variables to be used.
	 *
	 * @param DNAFileParserUsageBean usage - usage bean this class utilizes
	 * @param String fileName - fileName of file being opened
	 */
	public DNAFileParserProgressWindow(DNAFileParserUsageBean usage, String fileName)
	{
		bean = usage;	
		
		if(bean.getProgressWindow())
			createFrame(fileName);
		
		//Write to log file
		step = -1;
		print = true;
		charDone = false;
		bean.logMessage("Opening File: "+ fileName +"\n|*******10********20********30********40********50********60********70********80********90*********|");
		
		//Start Thread
		windowThread = new ProgressWindowThread();
		windowThread.start();
		
	}//Constructor
	
	/**
	 * Builds the progress window
	 * @param String fileName - fileName of file being opened
	 */
	private void createFrame(String fileName)
	{
		//frame set up
		frame = new JFrame();
		frame.setTitle("File Opening Progress Window");		//create new JFrame - ie. the window
		contentPane = frame.getContentPane();  				//retrieve the content pane as to modify it
		contentPane.setLayout(null);
		
		//Place on the labels and text fields on to the window
		JLabel label_1 = new JLabel("Opening File Progress:");
		label_1.setBounds(75, 5, 200, 15);
		contentPane.add(label_1);	
		
		JLabel label_2 = new JLabel("  File:  " + fileName);
		label_2.setBounds(0, 40, 1000, 15);
		contentPane.add(label_2);
		
		JLabel label_3 = new JLabel("  Reading File...");
		label_3.setBounds(0, 70, 100, 15);
		contentPane.add(label_3);
		
		//Create and set up Chars Progress Bar
		charsProgressBar = new JProgressBar();
		charsProgressBar.setBounds(5, 90, 260, 25);
		charsProgressBar.setStringPainted(true);
		contentPane.add(charsProgressBar);
		
		charsProgressLabel = new JLabel("0%");
		charsProgressLabel.setBounds(270, 90, 35, 25);
		contentPane.add(charsProgressLabel);
		
		//Label for 2nd Progress Bar
		dataProgressLabel = new JLabel("  Parsing Data...");
		dataProgressLabel.setBounds(0, 120, 100, 15);
		contentPane.add(dataProgressLabel);
		
		//Create and set up Data Progress Bar
		dataProgressBar = new JProgressBar();
		dataProgressBar.setBounds(5, 140, 260, 25);
		dataProgressBar.setStringPainted(true);
		contentPane.add(dataProgressBar);
		
		dataProgressValue = new JLabel("0%");
		dataProgressValue.setBounds(270, 140, 35, 25);
		contentPane.add(dataProgressValue);

		//Set Frame
		frame.pack();					//put the object all together
		frame.setSize(310,225);			//set the frame size
		frame.setLocation(800,0);		//set the frame location	
		frame.setVisible(true);			//set frame to visible!
		frame.show();
	}
	
	/**
	 *	Responsible for closing window
	 */
	public void end()
	{
		//stop printing
		print = false;
		
		//stopTimer && destroy thread?
		if(windowThread != null)
		{
			windowThread.stopTimer();
			windowThread = null;
		}
		
		//destroy window
		if(bean.getProgressWindow())
		{
			frame.dispose();
		}
		
		//log completion
		bean.logMessage(" DONE");	
	}


	/**
	 *	Runs progress window methods under a different thread
	 */
	class ProgressWindowThread extends Thread
	{
		//Class Vars
		Timer timer;
		
		/**
		 *	This is the class that is called when thread is started
		 */
		public void run()
		{
			//the timer to set regular intervals to check progress
			timer = new Timer();
			timer.scheduleAtFixedRate(new timerTask(),25,75);
		}//run
		
		/**
		 *	Responsible for stopping timer
		 */
		public void stopTimer()
		{
			if(timer != null)
				timer.cancel();
		}
		
	}//ProgressWindowThread


	/**
	 *	Responsible for set checking of time to update the progress window
	 */
	class timerTask extends TimerTask
	{
		/**
		 *	Called automatically, contains all code
		 */
		public void run()
		{
			//Using a JFrame
			if(bean.getProgressWindow())
			{
				//Do not divide by 0
				if(bean.getCharsToRead() != 0)
				{
					charNum = (int)(100*bean.getCharsRead()/(int)bean.getCharsToRead());
					charsProgressBar.setValue(charNum);					//set the given value on the Progress Bar
					charsProgressLabel.setText(charNum+"%");			//add a percentage sign to that	
				}
				//Do not divide by 0
				if(bean.getDataToParse() != 0)
				{
					dataProgressValue.setEnabled(true);
					dataProgressLabel.setEnabled(true);
					dataProgressBar.setEnabled(true);

					dataNum = (int)(100*bean.getDataParsed()/(int)bean.getDataToParse());
					dataProgressBar.setValue(dataNum);					//set the given value on the Progress Bar
					dataProgressValue.setText(dataNum+"%");				//add a percentage sign to that	
				}
				else
				{
					dataProgressValue.setEnabled(false);
					dataProgressLabel.setEnabled(false);
					dataProgressBar.setEnabled(false);
				}

				frame.update(frame.getGraphics());
			}
			
			//Check to see if chars are done
			if(!charDone && bean.getDataToParse() != 0)
			{
				step = -1;
				charDone = true;
				bean.logMessage(" DONE\nParsing Data...");
				bean.logMessage("|*******10********20********30********40********50********60********70********80********90*********|");
			}
		
			//Check for first step
			if(step == -1)
			{
				if(print)
					bean.logMessage("*");
				step = 0;
			}
			
			//Write Chars to Log File
			if(!charDone && bean.getCharsToRead() != 0)
			{
				//Get current percentage, print to file in steps of 1
				charNum = (int)(100*bean.getCharsRead()/(int)bean.getCharsToRead());
				while(charNum > step + 1)
				{					
					if(print)
						bean.logMessage("*");
					step++;
				}
			}
			//Write Data to Log File
			if(charDone && bean.getDataToParse() != 0)
			{
				//Get current percentage, print to file in steps of 1
				dataNum = (int)(100*bean.getDataParsed()/(int)bean.getDataToParse());
				while(dataNum > step + 1)
				{					
					if(print)
						bean.logMessage("*");
					step++;
				}
			}
			
		}//run
	}//timerTask

}//DNAFileParserProgressWindow
