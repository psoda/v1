/**
 *	ActionComponent.class is an instantiated object that creates
 *	a Component that is self-describing
 *	
 *	@author	Joshua Sailsbery
 *	@version	1.0
 *	@since	1.0
 */
package DataConvert.GraphicInterface.Objects;

import java.awt.Color;
import java.awt.Component;

public class ActionComponent
{
	//Class Vars
	private int space;				//The number of spaces down to start component
	private int position;			//The Relative position of a component
	private int spacePosition;		//The position of the space
	private Component component;	//The component being added
	
	/**
	 * Empty Constructor
	 */	
	public ActionComponent(){}

	/**
	 * Constructor of this component, takes in all possible values
	 *
	 * @param Component newComponent - the component to place
	 * @param int newPosition - the position of the component
	 * @param int Space - the spacing of the component
	 * @param int SpacePosition - position of the spacing
	 */
	public ActionComponent(Component newComponent, int newPosition, int Space, int SpacePosition)
	{
		component = newComponent;
		component.setBackground(Color.white);
		position = newPosition;
		space = Space;
		spacePosition = SpacePosition;
	}
	
	/**
	 *  Get method - returns int spacePosition 
	 */
	public int getSpacePosition()
	{
		return spacePosition;
	}
	
	/**
	 *  Set method - sets spacePosition to new int
	 *	@param int SpacePosition of new spacePosition
	 */
	public void setSpacePosition(int SpacePosition)
	{
		spacePosition = SpacePosition;
	}
	
	/**
	 *  Get method - returns int position 
	 */
	public int getPosition()
	{
		return position;
	}
	
	/**
	 *  Set method - sets position to new int
	 *	@param int newPosition of new position
	 */
	public void setPosition(int newPosition)
	{
		position = newPosition;
	}
	
	/**
	 *  Get method - returns int space 
	 */
	public int getSpace()
	{
		return space;
	}
	
	/**
	 *  Set method - sets space to new int
	 *	@param int Space of new space
	 */
	public void setSpace(int Space)
	{
		space = Space;
	}
	
	/**
	 *  Get method - returns Component component 
	 */
	public Component getComponent()
	{
		return component;
	}
	
	/**
	 *  Set method - sets component to new Component
	 *	@param Component newComponent of new component
	 */
	public void setComponent(Component newComponent)
	{
		component = newComponent;
	}

}//ActionComponent
