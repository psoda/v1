/**
 *	CriticalValueControl.class is responsible for the correct interpretation
 *	of observed changes in the sequences.  These values are stored
 *	in objects of the SubstsBean.
 *
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Substs;

import treesaap.GeneralDNAFileParser.TaxaBean;
import java.util.Vector;
import java.util.Hashtable;

public class Substs
{
	//Class Vars
	private SubstsUsageBean bean;		//Bean containing all values necessary for operation	
	private TableBean tableBean;		//Contains String values for all codons
	

	/**
	 * Substs Constructor
	 * creates SubstsBean object
	 */
	public Substs()
	{
		bean = new SubstsUsageBean();	
		tableBean = new TableBean();
	}//Constructor	

	/**
	 * Substs Constructor
	 * creates SubstsBean object
	 *
	 * @param Bean SubstsUsageBean usage bean is set to that passed in
	 */
	public Substs(SubstsUsageBean Bean)
	{
		bean = Bean;	
		tableBean = new TableBean();
	}//Constructor 

	/**
	 *	Calculates all Syn,Nonsyn,trans,transv, between each branch and for all branches
	 *
	 *	@param int start - which codon to start on on the sequences
	 *	@param int finish - which codon to finish on on the sequences
	 */
	public SubstsResultBean calculate(int start, int finish)
	{	
		try
		{			
			//Set-up Result Class
			SubstsResultBean results = new SubstsResultBean();								//Contains all the results from this call to calculate
			SubstsGeneticCodeBean subGCBean = new SubstsGeneticCodeBean(bean);	//Bean containing all genetic code values necessary for substs operation

			//Result Vars
			SubstsControl control = new SubstsControl(bean, tableBean, subGCBean);		//Control class, handles all recorded substitutions	
			MultiPath nucPthwy = new MultiPath(bean);							//Logic class, determines nucleotide pathways
			int[] freq = new int[65];							//contains the added up number of codons in a tree
			int[] cClassFreq = new int[8];						//contains the added up number of codon classes in a tree
			
			//Method Vars
			TaxaBean taxa1, taxa2;					//The taxa referenced here
			String sequence1, sequence2;			//represent sequences from in a branch
			String node1, node2;					//obtained from compare - nodes on either side of a given branch
			char nuc1, nuc2;						//characters of specific locale in sequence
			int codons1[], codons2[];				//all codon values of given sequences of a branch
			int codon1, codon2;						//actual codon value for comparison
			int aa1, aa2;							//amino acid values of given codons
			int cClass1, cClass2;					//codon class for a given codon
			int trans, pos, codonNum;				//several temp vars
			boolean isNode1, isNode2;				//whether the given codon is from a node or not
			
			//Set Window ID
			results.setWindowID(start/3 +" --> "+ finish/3);
			
			//Variables obtained from outside class
			Vector compare = bean.getTree().getAncestralTree();		//assign compare to represent that read in from tree
			Vector nodeNames = bean.getTree().getNodeNames();		//assign nodeNames to represent that from tree
			Hashtable taxa = bean.getTaxa();						//assign taxa to represent the one from tree
			Hashtable nodes = bean.getTree().getNodes();			//assign nodes to represent the one from tree
			int[] cClass = bean.getCClass();					//contains all the codon classes pertaining to a given Genetic Code
			int[] aAcid = bean.getAAcid();						//contains all the codon classes pertaining to a given Genetic Code
			boolean detailed = bean.getDetailed();				//whether two and three step changes to be recorded as well
			int propSize = 0;									//Number of properties
			if(!detailed)
				propSize = bean.getGCBean().getPropertyNames().size();
			
			//compare for each branch
			for(int i=0;i<compare.size();i+=2)
			{				
				//nodes 1&2 represent that from compare at i and i+1
				node1 = (String)compare.elementAt(i);
				node2 = (String)compare.elementAt(i+1);
				
				//switch taxa to whether they are a node or not
				if(nodeNames.contains(node1))
					isNode1 = true;
				else
					isNode1 = false;
				
				if(nodeNames.contains(node2))
					isNode2 = true;
				else
					isNode2 = false;
				
				//get taxa1
				if(isNode1)
					taxa1 = (TaxaBean)nodes.get(node1);	
				else
					taxa1 = (TaxaBean)taxa.get(node1);	
				
				//get taxa2
				if(isNode2)
					taxa2 = (TaxaBean)nodes.get(node2);	
				else
					taxa2 = (TaxaBean)taxa.get(node2);	
				
				//get sequences and codons 
				sequence1 = taxa1.getSequence();	
				sequence2 = taxa2.getSequence();
				codons1 = taxa1.getCodon();	
				codons2 = taxa2.getCodon();
				
				//Verify valid finish
				if(finish > sequence1.length() || sequence1.length() != sequence2.length())
					throw new Exception();
				
				//compare each codon in both sequences
				for(int j=start;j<finish;j+=3)
				{	
					//add to tracking var
					bean.addToWorkDone(1);
					
					codonNum = j/3;						//number represents which codon is being considered
					
					codon1 = codons1[codonNum];			//codons from a given sequence
					codon2 = codons2[codonNum];					
					
					
					//add up codon frequency if it's from taxa, not a node
					if(!isNode1)	freq[codon1+1]++;
					if(!isNode2)	freq[codon2+1]++;
					
					//if ambiguous, skip rest of calculations
					if(codon1 == -1 || codon2 == -1)	
						control.addAmbig(codonNum, node1 +" --> "+ node2, sequence1.substring(j,j+3), sequence2.substring(j,j+3));
					
					else
					{
						//get amino acids and codon classes
						aa1 = aAcid[codon1];
						aa2 = aAcid[codon2];
						cClass1 = cClass[codon1]-1;
						cClass2 = cClass[codon2]-1;
						
						if(!isNode1 && cClass1 != -1)	cClassFreq[cClass1]++;
						if(!isNode2 && cClass2 != -1)	cClassFreq[cClass2]++;
						
						//check both codons to make sure not stop codons
						if(aa1 == 0 || aa2 == 0 || cClass1 == -1 || cClass2 == -1)
							control.addStop(codonNum, node1 +" --> "+ node2, codon1, codon2);	
						
						//same codon
						else if(codon1 != codon2)							//make sure not same codon
						{			
							if(!detailed)
							{
								//Synonymous
								if(aa1 == aa2)
									control.addSyn(codonNum+1, 0, 0, 0, node1 +" --> "+ node2, codon1, codon2, cClass1+1, cClass2+1);
								//Nonsynonymous
								else
									control.addNsyn(codonNum+1, 0, 0, 0, node1 +" --> "+ node2, codon1, codon2, cClass1+1, cClass2+1);
							}
							//one-step change
							else if(inDomain(codon1, codon2))
							{
								//go through each character (nucleotide) of codon - to determine ts:tv and Syn:Nsyn
								for(int k=j;k<j+3;k++)
								{
									//specific nucleotide from given codon
									nuc1 = sequence1.charAt(k);			
									nuc2 = sequence2.charAt(k);
									
									//get transversion and position
									trans = getTrans(nuc1, nuc2);
									pos = k-j;			
									
									//Cannot be the same nucelotide
									if(trans != -1)
									{
										//Synonymous
										if(aa1 == aa2)
											control.addSyn(codonNum+1, k+1, pos+1, trans, node1 +" --> "+ node2, codon1, codon2, cClass1+1, cClass2+1);
										//Nonsynonymous
										else
											control.addNsyn(codonNum+1, k+1, pos+1, trans, node1 +" --> "+ node2, codon1, codon2, cClass1+1, cClass2+1);
									}
								}
							}
							//two-three step change
							else
							{
								nucPthwy.multipleChange(codon1, codon2);
								Vector paths = nucPthwy.getPaths();
								SubstsBean s;
								
								for(int k=0;k<paths.size();k++)
								{
									//get vars
									s = (SubstsBean)paths.elementAt(k);
									pos = s.getPos();
									trans = getTrans(sequence1.charAt((codonNum*3)+pos), sequence2.charAt((codonNum*3)+pos));
									
									//Get amino acids
									aa1 = aAcid[tableBean.getCodonNumber(s.getFromNuc())];
									aa2 = aAcid[tableBean.getCodonNumber(s.getToNuc())];
									
									//Check synonymy
									if(aa1 == aa2)
										control.addSyn(codonNum+1, (codonNum*3)+pos+1, pos+1, trans, node1 +" --> "+ node2, codon1, codon2, cClass1+1, cClass2+1);
									else
										control.addNsyn(codonNum+1, (codonNum*3)+pos+1, pos+1, trans, node1 +" --> "+ node2, codon1, codon2, cClass1+1, cClass2+1);
								}
							}
						}
					}
					
					//add to tracking var
					if(!detailed)
						bean.addToWorkDone(propSize);
				}
			}					
			
			//place values in results
			results.setNsynSubs(control.getNsynSubs());
			results.setSynSubs(control.getSynSubs());
			results.setStop(control.getStop());
			results.setAmbig(control.getAmbig());
			results.setFreq(freq);
			results.setCClassFreq(cClassFreq);
			results.setSubGCBean(subGCBean);
			results.setSynonymous(control.getSynonymous());
			results.setNonSynonymous(control.getNonSynonymous());
			
			return results;
			
		}
		catch(Exception e)
		{
			bean.errorMessage("\nAn error occured while processing in Substs.\n  Please review Substs requirements.");
			e.printStackTrace();
			return null;
		}
	}//calculate
	
	/**
	 *	Return number representing whether transition (0), transversion (1), or same variable (-1)
	 *
	 *	@param int nuc1 - the from nucleotide
	 *	@param int nuc2 - the to nucleotide
	 */
	private int getTrans(int nuc1, int nuc2)
	{
		if(nuc1 == nuc2)	return -1;				//same, return -1
		
		switch(nuc1)
		{
		case 'T':
			if(nuc2 == 'C')	return 0;					//transition
			return 1;													//else transversion
			
		case 'C':
			if(nuc2 == 'T')	return 0;					//transition
			return 1;													//else transversion
			
		case 'G':
			if(nuc2 == 'A')	return 0;					//transition
			return 1;													//else transversion
			
		case 'A':
			if(nuc2 == 'G')	return 0;					//transition
			return 1;													//else transversion
		}
		
		return -1;			//default return
		
	}//getTrans
	
	/**
	 *	Returns whether a certain codon2 is in the domain of codon1
	 *
	 *	@param int c1 - from codon
	 *	@param int c1 - to codon
	 */
	private boolean inDomain(int c1, int c2)
	{
		//1p
		for(int i=0;i<4;i++)
			if(c2 == c1%16 + 16*i)
				return true;
		
		//2p
		for(int i=0;i<4;i++)
			if(c2 == c1%4 + 16*(c1/16) + 4*i)
				return true;
		
		//3p
		for(int i=0;i<4;i++)
			if(c2 == 4*(c1/4) + i)
				return true;
		
		return false;
	}//inDomain
	
	/**
	 * Get method - returns SubstsUsageBean bean
	 */
	public SubstsUsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Set method - sets bean here and lower classes
	 *	@param Bean SubstsUsageBean is the new bean 
	 */
	public void setBean(SubstsUsageBean Bean)
	{
		bean = Bean;
	}//setBean 


}//Substs