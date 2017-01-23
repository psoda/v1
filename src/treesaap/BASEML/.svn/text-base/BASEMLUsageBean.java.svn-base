/**
 *	BASEMLUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the baseml package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.BASEML;

import java.io.File;
import java.util.Vector;

public class BASEMLUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private String model = "REV";				//Default model chosen by user
	private boolean fixKappa = false;					//Whether Kappa is default chosen or not
	private boolean fixAlpha = true;					//Whether Alpha is default chosen or not
	private double kappa = 2.5;						//Value if Kappa is default chosen
	private double alpha = 0.0;						//Value if Alpha is default chosen
	
	//Variables set by super class
	private int OS = 0;									//The operating system being used
	private String basemlPath = "";					//The Path to the baseml executable
	private String outFilePath = "";					//The Path to all output files
	
	//Validation Variables
	private Vector ModelNames;
	private String models[] = {"JC69", "K80", "F81", "F84", "HKY85", "TN93", "REV", "UNREST"};
	
	
	/**
	 * Constructor, initializes vector to contain all possible models BASEML will accept
	 */
	public BASEMLUsageBean()
	{
		ModelNames = new Vector();
		for(int i=0;i<models.length;i++)
			ModelNames.add(models[i]);
	
	}//constructor
	
	/**
	 *  Get method - returns Vector ModelNames
	 */
	public Vector getModelNames()
	{
		return ModelNames;
	}
	
	/**
	 *  Get method - returns boolean fixKappa
	 */
	public boolean getFixKappa()
	{
		return fixKappa;
	}
	
	/**
	 *  Set method - sets fixKappa to new boolean
	 *	@param boolean FixKappa of new fixKappa
	 */
	public void setFixKappa(boolean FixKappa)
	{
		fixKappa = FixKappa;
	}

	/**
	 *  Get method - returns double kappa
	 */
	public double getKappa()
	{
		return kappa;
	}
	
	/**
	 *  Set method - sets kappa to new double
	 *	@param double Kappa of new kappa
	 */
	public void setKappa(double Kappa)
	{
		kappa = Kappa;
	}

	/**
	 *  Get method - returns boolean fixAlpha
	 */
	public boolean getFixAlpha()
	{
		return fixAlpha;
	}
	
	/**
	 *  Set method - sets fixAlpha to new boolean
	 *	@param boolean FixAlpha of new fixAlpha
	 */
	public void setFixAlpha(boolean FixAlpha)
	{
		fixAlpha = FixAlpha;
	}

	/**
	 *  Get method - returns double alpha
	 */
	public double getAlpha()
	{
		return alpha;
	}
	
	/**
	 *  Set method - sets alphaNum to new double
	 *	@param double AlphaNum of new alphaNum
	 */
	public void setAlpha(double Alpha)
	{
		alpha = Alpha;
	}

	/**
	 *  Get method - returns String model 
	 */
	public String getModel()
	{
		return model;
	}
	
	/**
	 *  Set method - sets model to new String
	 *	@param String Model of new model
	 */
	public void setModel(String Model)
	{
		if(ModelNames.contains(Model))	
			model = Model;
	}
	
	/**
	 *  Set method - sets OS to new int
	 *	@param int os of new OS
	 */
	public void setOS(int os)
	{
		OS = os;
	}
	
	/**
	 *  Get method - returns int OS 
	 */
	public int getOS()
	{
		return OS;
	}
	
	/**
	 *  Set method - sets basemlPath to new String
	 *	@param String BasemlPath of new basemlPath
	 */
	public void setBasemlPath(String BasemlPath)
	{
		basemlPath = getAbsoluteDirPath(BasemlPath);
		if(basemlPath == null)
			basemlPath = "";
		else
			basemlPath += File.separator;
	}
	
	/**
	 *  Get method - returns String basemlPath 
	 */
	public String getBasemlPath()
	{
		return basemlPath;
	}
	
	/**
	 *  Set method - sets outFilePath to new String
	 *	@param String OutFilePath of new outFilePath
	 */
	public void setOutFilePath(String OutFilePath)
	{
		outFilePath = getAbsoluteDirPath(OutFilePath);
		if(outFilePath == null)
			outFilePath = "";
		else
			outFilePath += File.separator;
	}
	
	/**
	 *  Get method - returns String outFilePath 
	 */
	public String getOutFilePath()
	{
		return outFilePath;
	}

}//BASEMLUsageBean
