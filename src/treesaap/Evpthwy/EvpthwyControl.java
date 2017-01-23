/**
 *	EvpthwyControl.class is responsible for the evaluating
 *	and maintaining information pertaining to pathways
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Evpthwy;

import java.util.Vector;
import java.util.Hashtable;

import treesaap.Data.ProteinBean;
import treesaap.Substs.SubstsBean;

class EvpthwyControl
{
	//Class Vars
	private EvpthwyUsageBean bean;			//Bean containing all values necessary for operation
	private Vector propertyNames;			//Vector of property names

	//Rudimentary values for calculations
	private int totalPathways;				//total number of all possible pathways per property
	
	//Data Struct
	private Hashtable pathways;				//contains all the pathways by category and by property
	private Hashtable gScores;				//Contains Floats of each G-Score for each property
	private Hashtable zScores;				//Contains Floats of each Z-Score for each category
	private Hashtable goodnessOfFit;		//contains the goodness-of-fit for each property

	//Output vars
	private Vector sigZScore;				//Contains the properties and category numbers that are statistically significant in Z Scores
	private Hashtable sigGScore;			//Contains all the properties and whether they are statistically significant in G Scores
	private Hashtable sigGFScore;			//Contains all the properties and whether they are statistically significant in GF Scores
	
	
	/**
	 *	Constructor - Sets up rudimentary values for comparison
	 *
	 *	@param EvpthwyUsageBean Bean - the Usage bean
	 *	@param Hashtable properties - properties containing necessary info
	 *	@param Vector PropertyNames - names of properties
	 *	@param int[] freq - frequency of codons as recorded by Substs
	 */
	public EvpthwyControl(EvpthwyUsageBean Bean, Hashtable properties, Vector PropertyNames, int[] freq) throws Exception
	{
		//Given vars
		bean = Bean;
		propertyNames = PropertyNames;
		
		//set-up total of the pathways by category and by property
		pathways = new Hashtable();
		int catNum = bean.getNumberOfCat();
		
		//Method Vars
		String name;
		ProteinBean prop;
		int codons[] = new int[catNum];
		int codonTable[][];
		
		for(int i=0;i<propertyNames.size();i++)		//go through all the amino acid properties
		{	
			//set-up vars
			name = (String)propertyNames.elementAt(i);
			prop = (ProteinBean)properties.get(name);
			codons = new int[catNum];
			codonTable = new int[64][catNum];
			
			//go through each codon
			for(int j=0;j<64;j++)
			{
				codonTable[j] = new int[catNum];
				
				//go thru each category
				for(int k=0;k<catNum;k++)
				{
					codonTable[j][k] = prop.getCategories()[j][k] * freq[j+1];
					codons[k] += prop.getCategories()[j][k] * freq[j+1];
				}
				
				//add to tracking var
				bean.addToWorkDone(catNum + 1);
			}
			//assign to hashtable to hold for each property
			pathways.put(name, codons);
			prop.setCodonTable(codonTable);
			
			//add to tracking var
			bean.addToWorkDone(1);
		}
		
		//set-up for the total number of pathways by category
		totalPathways = 0;
		for(int i=0;i<catNum;i++)
			totalPathways += codons[i];
		
	}//constructor
	
	/**	
	 *	Calculate GF and Z-scores
	 *
	 *	@param int totalSubstsObs - total number of nonsynonymous substitutions observed
	 *	@param Hashtable observedCatTotals - the category totals observed
	 *	@param Vector propertyNames - names of properties
	 */
	public void calculate(int totalSubstsObs, Hashtable observedCatTotals)
	{
		//Set-up Storage Vars
		goodnessOfFit = new Hashtable();		//contains the goodness-of-fit for each property
		gScores = new Hashtable();			//Contains gScores of properties
		zScores = new Hashtable();			//Contains Floats of each Z-Score for each category
		
		//Set-up Method Vars
		String name;
		int[] catTotals;					//(#obs.subst)n
		int[] pathTotals;					//(#ptw)n 
		double[] zTotals;
		double g, gOF, En, On;				//GF-score
		double Pn, Pt, x, diffPnPt;			//Z-scores
		int catNum = bean.getNumberOfCat();
		
		//go thru each property
		for(int i=0;i<propertyNames.size();i++)
		{
			//initialize vars
			name = (String)propertyNames.elementAt(i);
			catTotals = (int[])observedCatTotals.get(name);
			pathTotals = (int[])pathways.get(name);
			
			//G & GF-score
			g = 0;
			gOF = 0;	
			
			//Z-scores
			zTotals = new double[catNum];		
			Pt = totalSubstsObs/(double)totalPathways;
			
			//go thru each category
			for(int j=0;j<catNum;j++)
			{
				if(pathTotals[j] != 0)
				{
					//Goodness-of-Fit
					En = (pathTotals[j]*totalSubstsObs)/(double)totalPathways;
					On = catTotals[j];
					if(En != 0)
					{
						gOF += ((En-On)*(En-On))/(double)En;
						if(On != 0)
							g += 2 * On * Math.log(On/En);;
					}
				
					//Z-Score
					Pn = catTotals[j]/(double)pathTotals[j];
					x = (catTotals[j]+totalSubstsObs)/(double)(pathTotals[j]+totalPathways);
					diffPnPt = Math.sqrt(x*(1-x)*((1/(double)pathTotals[j])+(1/(double)totalPathways)));
					if(diffPnPt != 0)
						zTotals[j] = (Pn-Pt)/(double)diffPnPt;
					else
						zTotals[j] = 0;
				}
			}
			//add to tracking var
			bean.addToWorkDone(catNum + 1);
			
			//add Caculated terms to respective data structs
			goodnessOfFit.put(name, new Double(gOF));
			gScores.put(name, new Double(g));
			zScores.put(name, zTotals);
		}	
		
	}//calculate
	
	/**
	 *	Determines whether a given property is significant for a given value
	 */
	public void determineSig()
	{
		//Process Vars
		int testZ;
		double[] gf = bean.getGFScore();
		double[] g = gf;
		double[] z = bean.getZScore();
		sigZScore = new Vector();
		sigGScore = new Hashtable();
		sigGFScore = new Hashtable();
			
		//Set-up Method Vars
		String name;
		double zScr[];
		double gfScr[];
		EvpthwyBean evBean;
		int catNum = bean.getNumberOfCat();
		
		//go thru each property
		for(int i=0;i<propertyNames.size();i++)
		{	
			//initialize vars
			name = (String)propertyNames.elementAt(i);
			zScr = (double[])zScores.get(name);
			
			//go thru each category
			for(int j=0;j<catNum;j++)
			{
				//test significance
				if(zScr[j] > z[0] || zScr[j] < (-1)*z[0])	
				{
					testZ = 0;
					if(zScr[j] > z[1] || zScr[j] < (-1)*z[1])
					{
						testZ++;
						if(zScr[j] > z[2] || zScr[j] < (-1)*z[2])	
							testZ++;
					}
			
					//add to sigZScores
					evBean = new EvpthwyBean();
					evBean.setName(name);
					evBean.setConf(testZ);
					evBean.setCatNum(j);
					evBean.setValue(zScr[j]);
					
					sigZScore.add(evBean);
				}
			}
			
			//go thru array, setting to 1 if it is significant
			String[] testG = new String[4];
			String[] testGF = new String[4];
			for(int j=0;j<3;j++)
			{			
				//look at all gf scores
				if(gf!=null && g!=null)	
				{
					//G - Score
					if(((Double)gScores.get(name)).doubleValue() > g[j])
					{
						testG[j] = "*";
						if(j==0)
							testG[3] = (Double)gScores.get(name) +"";
					}
					else
						testG[j] = "";
					
					//GF - Score
					if(((Double)goodnessOfFit.get(name)).doubleValue() > gf[j])
					{
						testGF[j] = "*";
						if(j==0)
							testGF[3] = (Double)goodnessOfFit.get(name) +"";
					}
					else
						testGF[j] = "";
				}
			}
			//add to tracking var
			bean.addToWorkDone(catNum + 4);

			sigGScore.put(name, testG);
			sigGFScore.put(name, testGF);
		}
		
	}//determineSig
	
	/**
	 *	Determines which subsitutions have property by category that significantly deviate with Z-Score analysis
	 *	@param Vector allNsynSubs - nonsynonymous substitutions observed
	 */
	public void getSigSubs(Vector allNsynSubs)
	{
		//method vars
		SubstsBean s;
		EvpthwyBean sig;
		int pos, catnum;
		String catValue;
		Vector sigCat;
		
		//go thru Substitutions
		for(int i=0;i<allNsynSubs.size();i++)
		{
			s = (SubstsBean)allNsynSubs.elementAt(i);

			//go thru significant
			for(int j=0;j<sigZScore.size();j++)
			{
				sig = (EvpthwyBean)sigZScore.elementAt(j);
				sigCat = s.getSigCat();
				
				//get position of this sigZScore in categories
				pos = propertyNames.indexOf(sig.getName());
				
				//read catnum
				catValue = (String)s.getCategories().elementAt(pos);
				catnum = Integer.valueOf(catValue.substring(0,catValue.indexOf("	"))).intValue();
				
				//add to substitution if match
				if((sig.getCatNum()+1) == catnum)
					sigCat.add(sig);
			}
			
			bean.addToWorkDone(10);
		}
	}//getSigSubs

	/**
	 *  Set method - sets totalPathways to new int
	 *	@param int TotalPathways of new totalPathways
	 */
	public void setTotalPathways(int TotalPathways)
	{
		totalPathways = TotalPathways;
	}
	
	/**
	 *  Get method - returns Vector sigZScore 
	 */
	public Vector getSigZScore()
	{
		return sigZScore;
	}
	
	/**
	 *  Get method - returns Hashtable sigGScore 
	 */
	public Hashtable getSigGScore()
	{
		return sigGScore;
	}
	
	/**
	 *  Get method - returns int totalPathways 
	 */
	public int getTotalPathways()
	{
		return totalPathways;
	}
	
	/**
	 *  Get method - returns Hashtable pathways 
	 */
	public Hashtable getPathways()
	{
		return pathways;
	}
	
	/**
	 *  Get method - returns Hashtable zScores 
	 */
	public Hashtable getZScores()
	{
		return zScores;
	}
	
	/**
	 *  Get method - returns Hashtable gScores 
	 */
	public Hashtable getGScores()
	{
		return gScores;
	}	
	
	/**
	 *  Get method - returns Hashtable goodnessOfFit 
	 */
	public Hashtable getGoodnessOfFit()
	{
		return goodnessOfFit;
	}
	
	/**
	 *  Get method - returns Hashtable sigGFScore 
	 */
	public Hashtable getSigGFScore()
	{
		return sigGFScore;
	}

}//EvpthwyControl