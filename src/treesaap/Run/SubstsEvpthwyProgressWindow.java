/**
 *	SubstsEvpthwyProgressWindow.class is responsible for the output
 *	of the progress in the substs and evpthwy algorithms.  Allows the user to track 
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

import treesaap.Evpthwy.EvpthwyUsageBean;
import treesaap.Substs.SubstsUsageBean;

class SubstsEvpthwyProgressWindow
{
	//Shared Objects
	private RunUsageBean bean;		//this class's usage bean
	private SubstsUsageBean subBean;			//UsageBean to get work from substs
	private EvpthwyUsageBean evBean;			//UsageBean to get work from evpthwy
	
	//Class Vars used in building and running progress Window
	private JFrame frame;
	private Container contentPane;					//The content pane of window
	private JLabel subProgressLabel;					//labels to be placed on window
	private JLabel evProgressLabel;						//labels to be placed on window
	private JLabel evProgressValue;						//labels to be placed on window
	private JProgressBar subProgressBar;				//A progress bar
	private JProgressBar evProgressBar;				//A progress bar
	
	//Running Vars
	private boolean print;
	private int subNum, evNum, step;
	private ProgressWindowThread windowThread;
	
	
	/**
	 * Constructor, creates objects to be referenced and utilized.
	 * calls method to initialize variables to be used.
	 *
	 * 	@param RunUsageBean SUBbean - usage bean of this class
	 */
	public SubstsEvpthwyProgressWindow(RunUsageBean Bean)
	{
		//Beans
		bean = Bean;
		subBean = bean.getSubsts().getBean();	
		evBean = bean.getEvpthwy().getBean();

		//GUI
		if(bean.getProgressWindow())
			createFrame();
		
		//Write to log file
		step = -1;
		print = true;
		bean.logMessage("Running Substs, Evpthwy");
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
		frame.setTitle("Substs/Evpthwy Progress Window");	//create new JFrame - ie. the window
		contentPane = frame.getContentPane();  				//retrieve the content pane as to modify it
		contentPane.setLayout(null);
		
		//Place on the labels and text fields on to the window
		JLabel label_1 = new JLabel("Substs/Evpthwy Progress");
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
		evProgressLabel = new JLabel("  Evpthwy");
		evProgressLabel.setBounds(0, 90, 100, 15);
		contentPane.add(evProgressLabel);
		
		//Create and set up Data Progress Bar
		evProgressBar = new JProgressBar();
		evProgressBar.setBounds(5, 110, 260, 25);
		evProgressBar.setStringPainted(true);
		contentPane.add(evProgressBar);
		
		evProgressValue = new JLabel("0%");
		evProgressValue.setBounds(270, 110, 35, 25);
		contentPane.add(evProgressValue);

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
				if(evBean.getTotalWork() != 0)
				{					
					evNum = (int)(100*evBean.getWorkDone()/(int)evBean.getTotalWork());
					evProgressBar.setValue(evNum);					//set the given value on the Progress Bar
					evProgressValue.setText(evNum+"%");				//add a percentage sign to that	
				}
				//Work done on Evpthwy
				if(evBean.getWorkDone() == 0)
				{
					evProgressValue.setEnabled(false);
					evProgressLabel.setEnabled(false);
					evProgressBar.setEnabled(false);
				}
				else
				{
					evProgressValue.setEnabled(true);
					evProgressLabel.setEnabled(true);
					evProgressBar.setEnabled(true);	
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
			if(subBean.getTotalWork() != 0 && evBean.getTotalWork() != 0)
			{
				subNum = (int)(100*subBean.getWorkDone()/(int)subBean.getTotalWork());
				evNum = (int)(100*evBean.getWorkDone()/(int)evBean.getTotalWork());
				while((subNum+evNum)/2 > step + 1)
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
