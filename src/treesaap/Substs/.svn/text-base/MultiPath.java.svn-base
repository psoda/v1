/**
 *	MultiPath.class is responsible for the correct interpretation
 *	of multiple nucleotide paths.  Will add these to a given SubstsControl.java
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Substs;

import java.util.Vector;

class MultiPath
{
	//Class Vars
	private SubstsUsageBean bean;		//Bean containing all values necessary for operation
	private TableBean tableBean;		//Contains String values for all codons
	
	//Process Vars
	private Vector paths;				//All the chosen paths for this codon to follow
	private int[] cClass;				//contains all the codon classes pertaining to a given Genetic Code
	private int[] aAcid;				//Integer array containing amino acid information of codons			
	private int synScore1, synScore2;	//Determines synonmy of differnt pathways, used to determine 2,3 step changes

	
	/**
	 * Substs Constructor
	 * creates SubstsBean object
	 *
	 * @param Bean SubstsUsageBean usage bean is set to that passed in
	 */
	public MultiPath(SubstsUsageBean Bean)
	{
		bean = Bean;
		tableBean = new TableBean();
	}//Constructor 

	/**
	 *	For 2 and 3 step changes
	 *
	 *	@param int codon1	from codon1
	 *	@param int codon2	to codon2
	 */
	public void multipleChange(int codon1, int codon2)
	{
		paths = new Vector();
		aAcid = bean.getAAcid();
		cClass = bean.getCClass();
		
		int domain1[] = getDomain(codon1);
		int domain2[] = getDomain(codon2);
		
		Vector pthwys = findChanges(codon1, codon2, domain1, domain2);
		if(pthwys.size() != 0)
		{
			twoStep(pthwys);
			if(synScore1 == synScore2)
			{
				addPthwy(((Integer)pthwys.elementAt(0)).intValue(), ((Integer)pthwys.elementAt(1)).intValue());
				addPthwy(((Integer)pthwys.elementAt(1)).intValue(), ((Integer)pthwys.elementAt(2)).intValue());
			}
			else
			{
				if(synScore1 > synScore2)
				{
					addPthwy(((Integer)pthwys.elementAt(0)).intValue(), ((Integer)pthwys.elementAt(1)).intValue());
					addPthwy(((Integer)pthwys.elementAt(1)).intValue(), ((Integer)pthwys.elementAt(2)).intValue());
				}
				else
				{
					addPthwy(((Integer)pthwys.elementAt(3)).intValue(), ((Integer)pthwys.elementAt(4)).intValue());
					addPthwy(((Integer)pthwys.elementAt(4)).intValue(), ((Integer)pthwys.elementAt(5)).intValue());
				}
			}
		}
		else
		{
			Vector totalPthwys = new Vector();
			int domain3[] = new int[12];
			int choice = 0;
			for(int i=0;i<12;i++)
			{
				domain3 = getDomain(domain1[i]);
				pthwys = findChanges(domain1[i], codon2, domain3, domain2);
				if(pthwys.size() != 0)
				{
					twoStep(pthwys);
					
					if(aAcid[codon1] == aAcid[((Integer)pthwys.elementAt(0)).intValue()])
						synScore1++;
					
					totalPthwys.add(new Integer(codon1));
					totalPthwys.add((Integer)pthwys.elementAt(0));
					totalPthwys.add((Integer)pthwys.elementAt(1));
					totalPthwys.add((Integer)pthwys.elementAt(2));
					totalPthwys.add(new Integer(synScore1));
					
					if(synScore1 > ((Integer)totalPthwys.elementAt(choice*5+4)).intValue())
						choice = (totalPthwys.size()-4)/5;
					
					if(pthwys.size() > 3)
					{				
						if(aAcid[codon1] == aAcid[((Integer)pthwys.elementAt(3)).intValue()])
							synScore2++;
						
						totalPthwys.add(new Integer(codon1));
						totalPthwys.add((Integer)pthwys.elementAt(3));
						totalPthwys.add((Integer)pthwys.elementAt(4));
						totalPthwys.add((Integer)pthwys.elementAt(5));
						totalPthwys.add(new Integer(synScore2));
						
						if(synScore2 > ((Integer)totalPthwys.elementAt(choice*5+4)).intValue())
							choice = (totalPthwys.size()-4)/5;
					}
				}
			}			
			addPthwy(((Integer)totalPthwys.elementAt(choice*5)).intValue(), ((Integer)totalPthwys.elementAt(choice*5+1)).intValue());
			addPthwy(((Integer)totalPthwys.elementAt(choice*5+1)).intValue(), ((Integer)totalPthwys.elementAt(choice*5+2)).intValue());
			addPthwy(((Integer)totalPthwys.elementAt(choice*5+2)).intValue(), ((Integer)totalPthwys.elementAt(choice*5+3)).intValue());
		}
		
	}//multipleChange
	
	
	/**
	 *	returns the domain of a given codon
	 *	@param int codon	domain of this codon
	 */
	private int[] getDomain(int codon)
	{
		int domain[] = new int[12];
		
		//1p
		for(int i=0;i<4;i++)
			domain[i] = codon%16 + 16*i;
		
		//2p
		for(int i=0;i<4;i++)
			domain[i+4] = codon%4 + 16*(codon/16) + 4*i;
		
		//3p
		for(int i=0;i<4;i++)
			domain[i+8] = 4*(codon/4) + i;
		
		return domain;
		
	}//getDomain	
	
	/**
	 *	Determines if there exists an element in d1 that is in d2
	 *
	 *	@param int c1	from codon1
	 *	@param int c2	to codon2
	 *	@param int d1	from codon's domain
	 *	@param int d2	to codon's domain
	 */
	private Vector findChanges(int c1, int c2, int d1[], int d2[])
	{		
		Vector pthwys = new Vector(); 
		
		for(int i=0;i<12;i++)
		{
			for(int j=0;j<12;j++)
			{
				if(d1[i] == d2[j] && cClass[d1[i]] != 0)
				{
					pthwys.add(new Integer(c1));
					pthwys.add(new Integer(d1[i]));
					pthwys.add(new Integer(c2));
				}
			}
		}
		
		return pthwys;
		
	}//findChanges		
	
	/**
	 *	Determines the Two Step changes route
	 *	@param Vector pthwys	the possible pathways
	 */
	private void twoStep(Vector pthwys)
	{
		synScore1 = 0;
		synScore2 = 0;
		
		if(aAcid[((Integer)pthwys.elementAt(0)).intValue()] == aAcid[((Integer)pthwys.elementAt(1)).intValue()])
			synScore1++;
		if(aAcid[((Integer)pthwys.elementAt(1)).intValue()] == aAcid[((Integer)pthwys.elementAt(2)).intValue()])
			synScore1++;
		
		if(pthwys.size() > 3)
		{
			if(aAcid[((Integer)pthwys.elementAt(3)).intValue()] == aAcid[((Integer)pthwys.elementAt(4)).intValue()])
				synScore2++;
			if(aAcid[((Integer)pthwys.elementAt(4)).intValue()] == aAcid[((Integer)pthwys.elementAt(5)).intValue()])
				synScore2++;
		}
	}//twoStep	
	
	
	/**
	 *	Adds a given codon pathway to observed changes
	 *
	 *	@param int c1	from codon1
	 *	@param int c2	to codon2
	 */
	private void addPthwy(int c1, int c2)
	{
		//Method Vars
		SubstsBean s = new SubstsBean();		
		int locale = inThisDomain(c1,c2);
		
		s.setPos(locale);	
		s.setFromNuc(tableBean.getCodonName(c1));
		s.setToNuc(tableBean.getCodonName(c2));
		paths.add(s);
	}//addPthwy
	
	/**
	 *	Returns which domain codon2 is in of codon1
	 *
	 *	@param int c1	from codon1
	 *	@param int c2	to codon2
	 */
	private int inThisDomain(int c1, int c2)
	{
		//1p
		for(int i=0;i<4;i++)
			if(c2 == c1%16 + 16*i)
				return 0;
		
		//2p
		for(int i=0;i<4;i++)
			if(c2 == c1%4 + 16*(c1/16) + 4*i)
				return 1;
		
		//3p
		for(int i=0;i<4;i++)
			if(c2 == 4*(c1/4) + i)
				return 2;
		
		return -1;
	}	

	/**
	 *  Get method - returns Vector paths 
	 */
	public Vector getPaths()
	{
		return paths;
	}

}//MultiPath