/**
 *	SubstsCDMProgressWindow.class is responsible for the output
 *	of the progress in the substs and cdm algorithms.  Allows the user to track 
 *	effectiveness of program.  Developed specifically for sliding window use.
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Run;

import java.util.Timer;
import java.util.TimerTask;
import java.awt.Container;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JProgressBar;

import treesaap.CDM.CDMUsageBean;
import treesaap.Substs.SubstsUsageBean;

class SubstsCDMProgressWindow
{
	//Shared Objects
	private RunUsageBean bean;		//this class's usage bean
	private SubstsUsageBean subBean;			//UsageBean to get work from substs
	private CDMUsageBean cdmBean;				//UsageBean to get work from cdm
	
	//Class Vars used in building and running progress Window
	private JFrame frame;
	private Container contentPane;					//The content pane of window
	private JLabel subProgressLabel;					//labels to be placed on window
	private JLabel cdmProgressLabel;					//labels to be placed on window
	private JLabel cdmProgressValue;					//labels to be placed on window
	private JProgressBar subProgressBar;				//A progress bar
	private JProgressBar cdmProgressBar;				//A progress bar
	
	//Running Vars
	private boolean print;
	private int subNum, cdmNum, step;
	private ProgressWindowThread windowThread;
	
	
	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * calls method to initialize variables to be used.
	 *
	 * 	@param RunUsageBean SUBbean - usage bean of this class
	 */
	public SubstsCDMProgressWindow(RunUsageBean Bean)
	{
		//Beans
		bean = Bean;
		subBean = bean.getSubsts().getBean();	
		cdmBean = bean.getCDM().getBean();

		//GUI
		if(bean.getProgressWindow())
			createFrame();
		
		//Write to log file
		step = -1;
		print = true;
		bean.logMessage("Running Substs, CDM");
		bean.logMessage("|*******10********20********30********40********50********60********70********80********90*********|");
		
		//Start Thread
		windowThread = new ProgressWindowThread();
		windowThread.start();
		
	}//Constructor
	
	/**
	 * Builds the progress window
	 */
	private void createFrame()
	{
		//frame set up
		frame = new JFrame();
		frame.setTitle("Substs/CDM Progress Window");	//create new JFrame - ie. the window
		contentPane = frame.getContentPane();  				//retrieve the content pane as to modify it
		contentPane.setLayout(null);
		
		//Place on the labels and text fields on to the window
		JLabel label_1 = new JLabel("Substs/CDM Progress");
		label_1.setBounds(75, 5, 200, 15);
		contentPane.add(label_1);	
		
		JLabel label_3 = new JLabel("  Substs");
		label_3.setBounds(0, 40, 100, 15);
		contentPane.add(label_3);
		
		//Create and set up Chars Progress Bar
		subProgressBar = new JProgressBar();
		subProgressBar.setBounds(5, 60, 260, 25);
		subProgressBar.setStringPainted(true);
		contentPane.add(subProgressBar);
		
		subProgressLabel = new JLabel("0%");
		subProgressLabel.setBounds(270, 60, 35, 25);
		contentPane.add(subProgressLabel);
		
		//Label for 2nd Progress Bar
		cdmProgressLabel = new JLabel("  CDM");
		cdmProgressLabel.setBounds(0, 90, 100, 15);
		contentPane.add(cdmProgressLabel);
		
		//Create and set up Data Progress Bar
		cdmProgressBar = new JProgressBar();
		cdmProgressBar.setBounds(5, 110, 260, 25);
		cdmProgressBar.setStringPainted(true);
		contentPane.add(cdmProgressBar);
		
		cdmProgressValue = new JLabel("0%");
		cdmProgressValue.setBounds(270, 110, 35, 25);
		contentPane.add(cdmProgressValue);

		//Set Frame
		frame.pack();					//put the object all together
		frame.setSize(310,195);			//set the frame size
		frame.setLocation(800,0);		//set the frame location	
		frame.setVisible(true);			//set frame to visible!
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
		
		bean.logMessage(" DONE");	
	}

	/**
	 * Get method - returns RunUsageBean bean
	 */
	public RunUsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Set method - sets bean here and lower classes
	 *	@param Bean RunUsageBean is the new bean 
	 */
	public void setBean(RunUsageBean Bean)
	{
		bean = Bean;
	}//setBean 
	
	
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
				if(subBean.getTotalWork() != 0)
				{
					subNum = (int)(100*subBean.getWorkDone()/(int)subBean.getTotalWork());
					subProgressBar.setValue(subNum);					//set the given value on the Progress Bar
					subProgressLabel.setText(subNum+"%");				//add a percentage sign to that	
				}
				//Do not divide by 0
				if(cdmBean.getTotalWork() != 0)
				{					
					cdmNum = (int)(100*cdmBean.getWorkDone()/(int)cdmBean.getTotalWork());
					cdmProgressBar.setValue(cdmNum);					//set the given value on the Progress Bar
					cdmProgressValue.setText(cdmNum+"%");				//add a percentage sign to that	
				}
				//Work done on Evpthwy
				if(cdmBean.getWorkDone() == 0)
				{
					cdmProgressValue.setEnabled(false);
					cdmProgressLabel.setEnabled(false);
					cdmProgressBar.setEnabled(false);
				}
				else
				{
					cdmProgressValue.setEnabled(true);
					cdmProgressLabel.setEnabled(true);
					cdmProgressBar.setEnabled(true);	
				}

				frame.update(frame.getGraphics());
			}
		
			//Check for first step
			if(step == -1)
			{
				if(print)
					bean.logMessage("*");
				step = 0;
			}

			//Get current percentage, print to file in steps of 1
			if(subBean.getTotalWork() != 0 && cdmBean.getTotalWork() != 0)
			{
				subNum = (int)(100*subBean.getWorkDone()/(int)subBean.getTotalWork());
				cdmNum = (int)(100*cdmBean.getWorkDone()/(int)cdmBean.getTotalWork());
				while((subNum+cdmNum)/2 > step + 1)
				{			
					//Write Chars to Log File
					if(print)
						bean.logMessage("*");
					step++;
				}
			}
			
		}//run
	}//timerTask

}//SubstsEvpthwyProgressWindow
