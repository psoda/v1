/**
 *	SubstsBean.class is an instantiated data object that
 *	contains all of the values of a given substitution
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.Substs;

import java.util.Vector;

public class SubstsBean
{
	private int codon;				//codon number
	private int nuc;				//nucleotide number
	private int pos;				//postion of change - number
	private int trans;				//transition or transversion
	private int fromCClass;			//codon class of from codon
	private int toCClass;			//codon class of to codon
	private String branch;			//name of sequences or nodes where change was observed
	private String fromNuc;			//what the nucleotide was
	private String toNuc;			//what the nucleotide is	
	private String fromAA;			//what the amino acids was
	private String toAA;			//what the amino acids was
	private Vector categories;		//what the categories are for this substitution for each Amino Acid Property
	private Vector sigCat;			//contains all those categories under significant selection

	
	/**
	 *  Set method - sets toCClass to new int
	 *	@param int ToCClass of new toCClass
	 */
	public void setToCClass(int ToCClass)
	{
		toCClass = ToCClass;
	}
	
	/**
	 *  Get method - returns int toCClass 
	 */
	public int getToCClass()
	{
		return toCClass;
	}
	
	/**
	 *  Set method - sets fromCClass to new int
	 *	@param int FromCClass of new fromCClass
	 */
	public void setFromCClass(int FromCClass)
	{
		fromCClass = FromCClass;
	}
	
	/**
	 *  Get method - returns int fromCClass 
	 */
	public int getFromCClass()
	{
		return fromCClass;
	}
	
	/**
	 *  Set method - sets trans to new int
	 *	@param int Trans of new trans
	 */
	public void setTrans(int Trans)
	{
		trans = Trans;
	}
	
	/**
	 *  Get method - returns int trans 
	 */
	public int getTrans()
	{
		return trans;
	}
	
	/**
	 *  Set method - sets pos to new int
	 *	@param int Pos of new pos
	 */
	public void setPos(int Pos)
	{
		pos = Pos;
	}
	
	/**
	 *  Get method - returns int pos 
	 */
	public int getPos()
	{
		return pos;
	}
	
	/**
	 *  Set method - sets nuc to new int
	 *	@param int Nuc of new nuc
	 */
	public void setNuc(int Nuc)
	{
		nuc = Nuc;
	}
	
	/**
	 *  Get method - returns int nuc 
	 */
	public int getNuc()
	{
		return nuc;
	}
	
	/**
	 *  Set method - sets codon to new int
	 *	@param int Codon of new codon
	 */
	public void setCodon(int Codon)
	{
		codon = Codon;
	}
	
	/**
	 *  Get method - returns int codon 
	 */
	public int getCodon()
	{
		return codon;
	}
	
	/**
	 *  Set method - sets sigCat to new Vector
	 *	@param Vector SigCat of new sigCat
	 */
	public void setSigCat(Vector SigCat)
	{
		sigCat = SigCat;
	}
	
	/**
	 *  Get method - returns Vector sigCat 
	 */
	public Vector getSigCat()
	{
		return sigCat;
	}
	
	/**
	 *  Set method - sets categories to new Vector
	 *	@param Vector Categories of new categories
	 */
	public void setCategories(Vector Categories)
	{
		categories = Categories;
	}
	
	/**
	 *  Get method - returns Vector categories 
	 */
	public Vector getCategories()
	{
		return categories;
	}
	
	/**
	 *  Set method - sets toAA to new String
	 *	@param String ToAA of new toAA
	 */
	public void setToAA(String ToAA)
	{
		toAA = ToAA;
	}
	
	/**
	 *  Get method - returns String toAA 
	 */
	public String getToAA()
	{
		return toAA;
	}
	
	/**
	 *  Set method - sets fromAA to new String
	 *	@param String FromAA of new fromAA
	 */
	public void setFromAA(String FromAA)
	{
		fromAA = FromAA;
	}
	
	/**
	 *  Get method - returns String fromAA 
	 */
	public String getFromAA()
	{
		return fromAA;
	}
	
	/**
	 *  Set method - sets toNuc to new String
	 *	@param String ToNuc of new toNuc
	 */
	public void setToNuc(String ToNuc)
	{
		toNuc = ToNuc;
	}
	
	/**
	 *  Get method - returns String toNuc 
	 */
	public String getToNuc()
	{
		return toNuc;
	}
	
	/**
	 *  Set method - sets fromNuc to new String
	 *	@param String FromNuc of new fromNuc
	 */
	public void setFromNuc(String FromNuc)
	{
		fromNuc = FromNuc;
	}
	
	/**
	 *  Get method - returns String fromNuc 
	 */
	public String getFromNuc()
	{
		return fromNuc;
	}	
	
	/**
	 *  Set method - sets branch to new String
	 *	@param String Branch of new branch
	 */
	public void setBranch(String Branch)
	{
		branch = Branch;
	}
	
	/**
	 *  Get method - returns String branch 
	 */
	public String getBranch()
	{
		return branch;
	}
	
}//SubstsBean