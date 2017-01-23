/**
 *	SubstsControl.class is responsible for the placing and 
 *	holding of all substitutions recorded by Substs.java
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Substs;

import java.util.Vector;

class SubstsControl
{
	//Class Vars
	private SubstsGeneticCodeBean subGCBean;	//Bean containing all genetic code values necessary for substs operation
	private SubstsUsageBean bean;		//Bean containing all values necessary for operation
	private TableBean tBean;			//To convert codon numbers to strings
	private Vector synSubs;				//contains all the synonymous substitutions
	private Vector nsynSubs;			//contains all the nonsynonymous substitutions
	private Vector ambig;				//contains all the ambiguous changes
	private Vector stop;				//contains all the stop codons used
	private int[][] synonymous;			//Number of synonymous substitutions
	private int[][] nonSynonymous;		//Number of nonsynonymous substitutions
	
	
	/**
	 * SubstsControl Constructor
	 * creates all Vector object
	 *
	 * @param Bean SubstsUsageBean usage bean is set to that passed in
	 * @param tableBean TableBean - bean containing all the Strings representations of codons
	 * @param SubstsGeneticCodeBean SubGCBean - bean containing all geneticCode information necessary for substs
	 */
	public SubstsControl(SubstsUsageBean Bean, TableBean tableBean, SubstsGeneticCodeBean SubGCBean)
	{
		//set Var
		bean = Bean;
		tBean = tableBean;
		subGCBean = SubGCBean;
		
		//Set-up Vars
		synSubs = new Vector();
		nsynSubs = new Vector();
		ambig = new Vector();
		stop = new Vector();
		synonymous = new int[3][2];
		nonSynonymous = new int[3][2];

	}//constructor
	
	/**
	 *	called by substs to tally all types of Syn changes
	 *
	 *	@param int codonNum the location in sequence
	 *	@param int nucNum the location (by nucleotide) in the sequence
	 *	@param int pos the position of the change
	 *	@param int trans (tv or ts)
	 *	@param String	the branch of the change
	 *	@param int codon1	from codon1
	 *	@param int codon2	to codon2
	 *	@param int cClass1	from codon class 
	 *	@param int cClass2	to codon class
	 */
	public void addSyn(int codonNum, int nucNum, int pos, int trans, String branch, int codon1, int codon2, int cClass1, int cClass2)
	{
		SubstsBean s = new SubstsBean();
		s.setCodon(codonNum);
		s.setNuc(nucNum);
		s.setPos(pos);
		s.setTrans(trans);
		s.setBranch(branch);
		s.setFromNuc(tBean.getCodonName(codon1));
		s.setToNuc(tBean.getCodonName(codon2));	
		s.setFromAA(tBean.getAAName(bean.getAAcid()[codon1]-1));
		s.setToAA(tBean.getAAName(bean.getAAcid()[codon2]-1));
		s.setFromCClass(cClass1);
		s.setToCClass(cClass2);
		
		//add to totals		
		if(bean.getDetailed())
			synonymous[pos-1][trans]++;
		
		synSubs.add(s);
	}//addSyn

	/**
	 *	called by substs to tally all types of NSyn changes
	 *
	 *	@param int codonNum the location in sequence
	 *	@param int nucNum the location (by nucleotide) in the sequence
	 *	@param int pos the position of the change
	 *	@param int trans (tv or ts)
	 *	@param String	the branch of the change
	 *	@param int codon1	from codon1
	 *	@param int codon2	to codon2
	 *	@param int cClass1	from codon class 
	 *	@param int cClass2	to codon class
	 */
	public void addNsyn(int codonNum, int nucNum, int pos, int trans, String branch, int codon1, int codon2, int cClass1, int cClass2)
	{		
		SubstsBean s = new SubstsBean();
		s.setCodon(codonNum);
		s.setNuc(nucNum);
		s.setPos(pos);
		s.setTrans(trans);
		s.setBranch(branch);
		s.setFromNuc(tBean.getCodonName(codon1));
		s.setToNuc(tBean.getCodonName(codon2));	
		s.setFromAA(tBean.getAAName(bean.getAAcid()[codon1]-1));
		s.setToAA(tBean.getAAName(bean.getAAcid()[codon2]-1));
		s.setFromCClass(cClass1);
		s.setToCClass(cClass2);
		
		//Get Category Results
		if(!bean.getDetailed())
		{
			try
			{
				s.setCategories(subGCBean.getCat(codon1, codon2));
				s.setSigCat(new Vector());
			}
			catch(Exception e)
			{
				bean.errorMessage("\nThere was a problem placing evalutated changed into a category.\n  Please review manual.");
				return;
			}
		}
		
		//add to totals
		if(bean.getDetailed())
			nonSynonymous[pos-1][trans]++;
		
		nsynSubs.add(s);
	}//addNsyn

	/**
	 *	called by substs to tally all types of Ambiguous codons
	 *
	 *	@param int codonNum the location in sequence
	 *	@param String	the branch of the change
	 *	@param String codon1	from codon1
	 *	@param String codon2	to codon2
	 */
	public void addAmbig(int codonNum, String branch, String codon1, String codon2)
	{
		SubstsBean s = new SubstsBean();
		s.setCodon(codonNum);
		s.setBranch(branch);
		s.setFromNuc(codon1);
		s.setToNuc(codon2);
		
		ambig.add(s);		
	}//addAmbig

	/**
	 *	called by substs to tally all types of stops
	 *
	 *	@param int codonNum the location in sequence
	 *	@param String	the branch of the change
	 *	@param int codon1	from codon1
	 *	@param int codon2	to codon2
	 */
	public void addStop(int codonNum, String branch, int codon1, int codon2)
	{
		SubstsBean s = new SubstsBean();
		s.setCodon(codonNum);
		s.setBranch(branch);
		s.setFromNuc(tBean.getCodonName(codon1));
		s.setToNuc(tBean.getCodonName(codon2));
			
		//Determine fromAA
		int aa = bean.getAAcid()[codon1];
		if(aa == 0)	
			s.setFromAA("***");
		else	
			s.setFromAA(tBean.getAAName(aa-1));
		
		//Determine toAA
		aa = bean.getAAcid()[codon2];
		if(aa == 0)	
			s.setToAA("***");
		else	
			s.setToAA(tBean.getAAName(aa-1));
		
		stop.add(s);
	}//addStop

	/**
	 *  Set method - sets stop to new Vector
	 *	@param Vector Stop of new stop
	 */
	public void setStop(Vector Stop)
	{
		stop = Stop;
	}
	
	/**
	 *  Get method - returns Vector stop 
	 */
	public Vector getStop()
	{
		return stop;
	}
	
	/**
	 *  Set method - sets ambig to new Vector
	 *	@param Vector Ambig of new ambig
	 */
	public void setAmbig(Vector Ambig)
	{
		ambig = Ambig;
	}
	
	/**
	 *  Get method - returns Vector ambig 
	 */
	public Vector getAmbig()
	{
		return ambig;
	}
	
	/**
	 *  Set method - sets nsynSubs to new Vector
	 *	@param Vector NsynSubs of new nsynSubs
	 */
	public void setNsynSubs(Vector NsynSubs)
	{
		nsynSubs = NsynSubs;
	}
	
	/**
	 *  Get method - returns Vector nsynSubs 
	 */
	public Vector getNsynSubs()
	{
		return nsynSubs;
	}
	
	/**
	 *  Set method - sets synSubs to new Vector
	 *	@param Vector SynSubs of new synSubs
	 */
	public void setSynSubs(Vector SynSubs)
	{
		synSubs = SynSubs;
	}
	
	/**
	 *  Get method - returns Vector synSubs 
	 */
	public Vector getSynSubs()
	{
		return synSubs;
	}
	
	/**
	 *  Set method - sets nonSynonymous to new int[][]
	 *	@param int[][] NonSynonymous of new nonSynonymous
	 */
	public void setNonSynonymous(int[][] NonSynonymous)
	{
		nonSynonymous = NonSynonymous;
	}
	
	/**
	 *  Get method - returns int[][] nonSynonymous 
	 */
	public int[][] getNonSynonymous()
	{
		return nonSynonymous;
	}
	
	/**
	 *  Set method - sets synonymous to new int[][]
	 *	@param int[][] Synonymous of new synonymous
	 */
	public void setSynonymous(int[][] Synonymous)
	{
		synonymous = Synonymous;
	}
	
	/**
	 *  Get method - returns int[][] synonymous 
	 */
	public int[][] getSynonymous()
	{
		return synonymous;
	}


}//SubstsControl