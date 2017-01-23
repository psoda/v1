/**
 * Data structure that stores an edge in the graph
 */

package treesaap.Weka;


public class Edge 
{
	private int nodeFrom;
	private int nodeTo;
	private double weight;
	
        /**
         * Constructor
         * @param n1 from node
         * @param n2 to node
         * @param w weight
         */
	public Edge(int from, int to, double weight)
	{
		nodeFrom = from;
		nodeTo = to;
		this.weight = weight;
	}
	
        /**
         * Returns true if this edge connects to the specified node
         * @param node
         * @return
         */
	public boolean ConnectsTo(int node)
	{
		return (nodeTo == node);
	}
	
        /**
         * Returns the node this edge connects from (e.g., a connects to b, returns a)
         * @return
         */
	public int NodeFrom()
	{
		return nodeFrom;
	}
	
        /**
         * Returns the node this edge connects to  (e.g., a connects to b, returns b)
         * @return
         */
	public int NodeTo()
	{
		return nodeTo;
	}
	
	public double Weight()
	{
		return weight;
	}
	
        @Override
	public String toString()
	{
		String str = "From " + nodeFrom + " to " + nodeTo + " weight " + weight;
		return str;
	}
}
