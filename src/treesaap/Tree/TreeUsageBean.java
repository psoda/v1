/**
 *	TreeUsageBean.class is an instantiated data object that
 *	contains all of the operational decisions a user can 
 *	make regarding the uses of the Tree package.
 *	
 *	@author	Joshua Sailsbery
 *	@version	2.0
 *	@since	1.0
 */
package treesaap.Tree;

public class TreeUsageBean extends treesaap.Objects.UsageBean
{
	//DEFAULT VARIABLES - all values to be changed as read in from file
	private boolean displayTCSWindow;	//Whether to display TCS window on a run

	//Process Vars
	private boolean selectedTCSEdges;	//Whether to use selected edges from TCS to reconstruct a tree

	/**
	 *  Get method - returns boolean selectedTCSEdges 
	 */
	public boolean getSelectedTCSEdges()
	{
		return selectedTCSEdges;
	}
	
	/**
	 *  Set method - sets selectedTCSEdges to new boolean
	 *	@param boolean SelectedTCSEdges of new selectedTCSEdges
	 */
	public void setSelectedTCSEdges(boolean SelectedTCSEdges)
	{
		selectedTCSEdges = SelectedTCSEdges;
	}

	/**
	 *  Get method - returns boolean displayTCSWindow 
	 */
	public boolean getDisplayTCSWindow()
	{
		return displayTCSWindow;
	}
	
	/**
	 *  Set method - sets displayTCSWindow to new boolean
	 *	@param boolean DisplayTCSWindow of new displayTCSWindow
	 */
	public void setDisplayTCSWindow(boolean DisplayTCSWindow)
	{
		displayTCSWindow = DisplayTCSWindow;
	}

}//TreeUsageBean
