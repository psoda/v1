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
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Vector;
import java.util.Hashtable;

public class SubstsPrint
{
	//Class Vars
	private SubstsUsageBean bean;		//Bean containing all values necessary for operation	
	private TableBean tableBean;		//Contains String values for all codons


	/**
	 * Substs Constructor
	 * creates SubstsBean object
	 *
	 * @param Bean SubstsUsageBean usage bean is set to that passed in
	 */
	public SubstsPrint(SubstsUsageBean Bean)
	{
		bean = Bean;	
		tableBean = new TableBean();
	}//Constructor 

	/**
	 *  Get method - writes Syn info to file
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector synSubs - Vector contianing information to print
	 */
	public void writeSynSubs(String fileName, Vector synSubs)
	{
		//Method vars
		SubstsBean s;
		String entry;

		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
		
			//Header
			entry = "Codon#";
			if(bean.getDetailed())
				entry += "	Nuc#";
			entry += "	Branch	From Codon	To Codon	Amino Acid";
			if(bean.getDetailed())
				entry += "	CClass	Pos	Amnt";
			outFile.println(entry);
			
			//go through all ambiguous
			for(int i=0;i<synSubs.size();i++)
			{
				s = (SubstsBean)synSubs.elementAt(i);
				entry = s.getCodon() +"	";
				if(bean.getDetailed())
					entry += s.getNuc() +"	";
				entry += s.getBranch() +"	";
				entry += s.getFromNuc() +"	";
				entry += s.getToNuc() +"	";
				entry += s.getFromAA();
				
				if(bean.getDetailed())
				{
					entry += "	";
					entry += s.getFromCClass() +"	";
					entry += s.getPos() +"	";
					if(s.getTrans() == 0)
						entry += "ts";
					else
						entry += "tv";
				}
				
				outFile.println(entry);
			}
			
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Synonymous substitution totals.\n  Writing to file: "+ fileName);
		}
	}
	
	/**
	 *  Get method - writes Nsyn info to file
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector nsynSubs - Vector contianing information to print
	 */
	public void writeNsynSubs(String fileName, Vector nsynSubs)
	{
		//Method vars
		Vector cat;
		SubstsBean s;
		String entry;	
		Vector propertyNames = bean.getGCBean().getPropertyNames();		
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
			
			//Header
			entry = "Codon#";
			if(bean.getDetailed())
				entry += "	Nuc#";
			entry += "	Branch	From Codon	To Codon	From AA	To AA";
			if(bean.getDetailed())
				entry += "	From CClass	To CClass	Pos	Amnt";
			else
			{
				for(int i=0;i<propertyNames.size();i++)
					entry += "	"+ (String)propertyNames.elementAt(i) +"	Amnt";
			}
			outFile.println(entry);
			
			//go through all ambiguous
			for(int i=0;i<nsynSubs.size();i++)
			{
				s = (SubstsBean)nsynSubs.elementAt(i);
				entry = s.getCodon() +"	";
				if(bean.getDetailed())
					entry += s.getNuc() +"	";
				entry += s.getBranch() +"	";
				entry += s.getFromNuc() +"	";
				entry += s.getToNuc() +"	";
				entry += s.getFromAA() +"	";
				entry += s.getToAA() + "	";
				
				//print additional info
				if(bean.getDetailed())
				{
					entry += s.getFromCClass() +"	";
					entry += s.getToCClass() +"	";
					entry += s.getPos() +"	";
					if(s.getTrans() == 0)
						entry += "ts";
					else
						entry += "tv";
				}
				//print cat vector
				else
				{
					cat = s.getCategories();
					for(int j=0;j<cat.size();j++)
						entry += (String)cat.elementAt(j) +"	";
				}
				
				outFile.println(entry);
			}		
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Nonsynonymous substitution totals.\n  Writing to file: "+ fileName);
		}
	}
	
	/**
	 *  Get method - writes Stop info to file
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector stop - Vector contianing information to print
	 */
	public void writeStop(String fileName, Vector stop)
	{
		//Method vars
		SubstsBean s;
		String entry;
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
		
			//Header
			outFile.println("Codon#	Branch	From Codon	To Codon	From AA	To AA");
			
			//go through all ambiguous
			for(int i=0;i<stop.size();i++)
			{
				s = (SubstsBean)stop.elementAt(i);
				entry = s.getCodon() +"	";
				entry += s.getBranch() +"	";
				entry += s.getFromNuc() +"	";
				entry += s.getToNuc() +"	";
				entry += s.getFromAA() +"	";
				entry += s.getToAA();
				
				outFile.println(entry);
			}
		
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Stop codon totals.\n  Writing to file: "+ fileName);
		}
	}
	
	/**
	 *  Get method - writes Ambig info to file
	 *
	 *	@param String fileName - validated fileName, where information is printed
	 *	@param Vector ambig - Vector contianing information to print
	 */
	public void writeAmbig(String fileName, Vector ambig)
	{
		//Method vars
		SubstsBean s;
		String entry;
		
		try
		{
			//Validate fileName
			fileName = bean.getAbsoluteFilePath(fileName, true);
			if(fileName == null)
				throw new Exception();
			
			//set-up file
			PrintWriter outFile = new PrintWriter(new FileWriter(fileName));
		
			//Header
			outFile.println("Codon#	Branch	From Codon	To Codon");
	
			//go through all ambiguous
			for(int i=0;i<ambig.size();i++)
			{
				s = (SubstsBean)ambig.elementAt(i);
				entry = s.getCodon() +"	";
				entry += s.getBranch() +"	";
				entry += s.getFromNuc() +"	";
				entry += s.getToNuc();
				
				outFile.println(entry);
			}
			outFile.close();
		}
		catch(Exception e)
		{
			bean.errorMessage("\nThere was a problem trying to write Ambiguous codon totals.\n  Writing to file: "+ fileName);
		}
	}
	

}//SubstsPrint