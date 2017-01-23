/**
 *	Block.class is responsible for the correct extrapolation
 *	of values tacked onto the end of a file in a block
 *
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Block;

public class Block
{
	//CLASS Vars
	private BlockUsageBean bean;			//Bean containing all values necessary for operation
	
	
	/**
	 * Block Constructor
	 * creates BlockBean object
	 */
	public Block()
	{
		bean = new BlockUsageBean();
		
	}//Constructor	
	
	/**
	 * Block Constructor
	 * @param Bean BlockUsageBean usage bean is set to that passed in
	 */
	public Block(BlockUsageBean Bean)
	{
		bean = Bean;
		
	}//Constructor 


	/**
	 * Get method - returns BlockUsageBean bean
	 */
	public BlockUsageBean getBean()
	{
		return bean;
	}//getBean 	
	
	/**
	 *  Set method - sets bean here and lower classes
	 *	@param Bean BlockUsageBean is the new bean 
	 */
	public void setBean(BlockUsageBean Bean)
	{
		bean = Bean;
	}//setBean 
	
}//CDM
