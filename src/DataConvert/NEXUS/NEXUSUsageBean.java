/**
 *	NEXUSUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the NEXUS package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package DataConvert.NEXUS;

class NEXUSUsageBean extends DataConvert.Components.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private boolean matchChar;				//Whether to look for a return after name or not
	private int interleaveSize;							//The size of chunks to break data into
	
	/**
	 *  Set method - sets chunk to new int
	 *	@param int Chunk of new chunk
	 */
	public void setInterleaveSize(int InterleaveSize)
	{
		interleaveSize = InterleaveSize;
	}
	
	/**
	 *  Get method - returns int chunk 
	 */
	public int getInterleaveSize()
	{
		return interleaveSize;
	}
	
	/**
	 *  Set method - sets hard_return to new boolean
	 *	@param boolean Hard_return of new hard_return
	 */
	public void setMatchChar(boolean MatchChar)
	{
		matchChar = matchChar;
	}
	
	/**
	 *  Get method - returns boolean hard_return 
	 */
	public boolean getMatchChar()
	{
		return matchChar;
	}
}//NEXUSUsageBean
