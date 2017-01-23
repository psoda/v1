/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.Objects;

import java.io.IOException;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Enumeration;
import java.util.Date;

public class TaxaBeanManager{
	
	private Hashtable table;
	private long data;
	private String filename;
	private Vector taxa;
	private Vector trees;
	
	public TaxaBeanManager()
	{
		taxa = new Vector();
		trees = new Vector();
		table = new Hashtable();
		data = 0;
	}//close TaxaBeanManager constructor
	
	public void add(String name, String sequence) throws IOException
	{	
		if(name == null || name.length() < 1)
			throw new IOException("Invalid Naming of Taxa");
		if(sequence == null || sequence.length() < 1)
			throw new IOException("Invalid Sequence Data");
		
		name = name.replaceAll(" ", "_");
		sequence = sequence.replaceAll(" ","");
		data += sequence.length();
		
		TaxaBean bean = (TaxaBean)table.get(name);
		if(bean == null)
		{
			TaxaBean temp = new TaxaBean(name, sequence);
			taxa.add(temp);
			table.put(name, temp);
		}//close if
		else
		{
			bean.appendSequence(sequence);
		}//close else
	}//close add method
	
	public void add(String name, String sequence, String accession, String gi, Date genDate) throws IOException
	{
		if(name == null || name.length() < 1)
			throw new IOException("Invalid Naming of Taxa");
		if(sequence == null || sequence.length() < 1)
			throw new IOException("Invalid Sequence Data");
		
		name = name.replaceAll(" ", "_");
		sequence = sequence.replaceAll(" ","");
		data += sequence.length();
		
		TaxaBean bean = (TaxaBean)table.get(name);
		if(bean == null)
		{
			TaxaBean temp = new TaxaBean(name, sequence, accession, gi, genDate);
			taxa.add(temp);
			table.put(name, temp);
		}//close if
		else
		{
			bean.appendSequence(sequence);
		}//close else
	}//close Genbank add method
	
	public void remove(String name){
		TaxaBean bean = (TaxaBean)table.get(name);
		if(bean != null)
		{
			taxa.remove(bean);
			table.remove(name);
			data -= bean.getSequence().length();
		}//close if
	}//close remove method
	
	public void remove(int index){
		TaxaBean bean = (TaxaBean)taxa.get(index);
		if(bean != null)
		{
			taxa.remove(bean);
			table.remove(bean.getName());
			data -= bean.getSequence().length();
		}//close if
	}//close remove method
	
	public TaxaBean get(int index){
		if(index < taxa.size())
			return(TaxaBean)taxa.elementAt(index); 
		else
			return null;
	}//close get method
	
	public TaxaBean get(String name){
		TaxaBean bean = (TaxaBean)table.get(name);
		return(bean); 
	}//close get method
	
	public void updateName(String index, String name, boolean combine) throws Exception
	{
		//Method vars
		TaxaBean t = get(index);
		
		//Verify change needed
		if(name == null || name.equals("") || t == null || name.equals(t.getName()))
			return;
		
		//Check to see if name exists
		if(table.get(name) == null)
		{
			t.setName(name);
			table.remove(index);
			table.put(name, t);
		}
		
		//Combine beans
		else if(combine)
		{
			remove(index);
			add(name, t.getSequence());
		}
		
		//Change dependencies
		else
		{
			//get unique name
			int i = 0;
			String newName = name +"__________";
			newName = newName.substring(0,10);
			while(table.get(newName) != null)
			{
				newName = i+name +"__________";
				newName = newName.substring(0,10);
				i++;
			}
			t.setName(newName);
			table.remove(index);
			table.put(newName, t);
		}
	}
	
	public void absorb(TaxaBeanManager victim)
	{
		try
		{
			//add in beans
			TaxaBean t;
			for(int i=0;i<victim.size();i++)
			{
				t = victim.get(i);
				add(t.getName(), t.getSequence());
			}
			
			//add in trees
			Vector tr = victim.getTrees();
			for(int i=0;i<tr.size();i++)
				trees.add(tr.elementAt(i));
		}
		catch(Exception e){}
	}	

	public void finish(String unknown, String gap, String mChar)
	{
		//Set the vars to chars
		unknown = unknown.substring(0,1);
		gap = gap.substring(0,1);
		mChar = mChar.substring(0,1);
		for(int i=0; i < taxa.size(); i++)
		{
			TaxaBean temp = (TaxaBean)taxa.elementAt(i);
			if(!(unknown.equals("?")))
			{
				temp.setSequence(temp.getSequence().replaceAll(unknown, "~~").trim());
			}
			if(!(gap.equals("-")))
			{
				temp.setSequence(temp.getSequence().replaceAll(gap, "@@").trim());
			}
			if(!(mChar.equals(".")))
			{
				temp.setSequence(temp.getSequence().replaceAll(unknown, "##").trim());
			}
			temp.setSequence(temp.getSequence().replaceAll("~~", "?").trim());
			temp.setSequence(temp.getSequence().replaceAll("@@", "-").trim());
			temp.setSequence(temp.getSequence().replaceAll("##", ".").trim());
		}
	}
	
	public int size(){
		return (table.size()); 
	}//close size method
	
	public void setTrees(Vector Trees)
	{
		trees = Trees;
	}
	
	public Vector getTrees()
	{
		return trees;
	}
	
	public int getTreeNum()
	{
		return trees.size();
	}

	public void setFilename(String Filename)
	{
		filename = Filename;
	}
	
	public String getFilename()
	{
		return filename;
	}
	
	public long getDataSize()
	{
		return data;
	}
}//close TaxaBeanManager class
