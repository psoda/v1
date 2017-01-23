/**
 *	FASTA.class that reads in a Fasta file and stored information into the java beans class
 *	
 *	@author	Joshua Sailsbery and Matt Dyer
 *	@version	1.0
 */

package DataConvert.Objects;

import java.io.*;
import java.util.*;
import java.lang.*;
import java.util.Hashtable;

/* changed name of class to TreeBeanManager to match filename and changed packagename to match directory --Adam R. Teichert 20 Aug 2008 */
public class TreeBeanManager{
	Hashtable table;
	
	public TreeBeanManager(){
		table = new Hashtable();
	}//close TaxaBeanManager constructor
	
	public void add(String name, String sequence){
		TaxaBean bean = (TaxaBean)table.get(name);
		name = name.replaceAll(" ", "_");
		if(bean == null){
			TaxaBean temp = new TaxaBean(name, sequence);
			table.put(name, temp);
		}//close if
		else{
			bean.appendSequence(sequence);
		}//close else
	}//close add method
	
	public void remove(String name){
		TaxaBean bean = (TaxaBean)table.get(name);
		if(bean != null){
			table.remove(name);
		}//close if
	}//close remove method
	
	public TaxaBean get(String name){
		TaxaBean bean = (TaxaBean)table.get(name);
		return(bean); 
	}//close get method
	
	public int size(){
		return (table.size()); 
	}//close size method
	
	//replace all - Go through TaxaBeans and replace in sequence
	
	public Enumeration returnResults(){
		return(table.elements());
	}//close returnResults method
}//close TaxaBeanManager class
