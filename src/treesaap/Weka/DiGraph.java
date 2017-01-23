/**
 * Stores the contents of a neural network
 */

package treesaap.Weka;

import java.util.ArrayList;

public class DiGraph 
{
	private ArrayList<Node> nodes;
	private ArrayList<Edge> edges;
        private ArrayList<String> chemPropertyNames;
	
	public DiGraph()
	{
		Init();
	}
        
        /**
         * Adds the specified name to the chemical properties list, assigning it the next index number. This number corresponds to the checmical
         * property number in the neural network file
         * @param name
         */
        public void AddChemPropertyName(String name)
        {
            chemPropertyNames.add(name);
        }
	
        /**
         * Adds an edge from nodeFrom to nodeTo assigned the specified weight
         * @param nodeFrom
         * @param nodeTo
         * @param weight
         */
	public void AddEdge(int nodeFrom, int nodeTo, double weight)
	{
		Edge edge = new Edge(nodeFrom, nodeTo, weight);
		if (!edges.contains(edge))
		{
			// TODO Do we throw an exception if the nodes aren't in the graph?
			edges.add(edge);
		}
	}
	
        /**
         * Adds a node to the graph with the specified threshold value
         * @param nodeNum
         * @param threshold
         */
	public void AddNode(int nodeNum, double threshold)
	{
		Node node = new Node(nodeNum, threshold);
		AddNode(node);
	}
	
	public void AddNode(Node node)
	{
		if (!nodes.contains(node))
		{
			nodes.add(node);
		}
	}
        
        /**
         * Gets the node with the specified number
         * @param nodeNum
         * @return null if the node wasn't found
         */
        public Node GetNode(int nodeNum)
        {
            for (int i = 0; i < nodes.size(); i++)
            {
                if (nodeNum == nodes.get(i).NodeId())
                {
                    return nodes.get(i);
                }
            }
            
            return null;
        }
        
        /**
         * Gets the list of nodes
         * @return
         */
        public ArrayList<Node> GetNodeList()
        {
            return nodes;
        }
        
        /**
         * Adds the specified class letter to the specified node
         * @param nodeNum
         * @param letter
         */
        public void AddClass(int nodeNum, char letter)
        {
            if (nodes.size() > nodeNum)
            {
                nodes.get(nodeNum).SetClassLetter(letter);
            }
        }
	
        /**
         * Returns a list of edges that go to the specified node
         * @param nodeNum
         * @return
         */
	public ArrayList<Edge> GetEdgesTo(int nodeNum)
	{
		ArrayList<Edge> edgesToNode = new ArrayList<Edge>();
		
		for (int i = 0; i < edges.size(); i++)
		{
			if (edges.get(i).ConnectsTo(nodeNum))
			{
				edgesToNode.add(edges.get(i));
			}
		}
		
		return edgesToNode;
	}
        
        /**
         * Gets the list of chemical property names
         * @return
         */
        public ArrayList<String> GetChemPropertyNames()
        {
            return chemPropertyNames;
        }

	private void Init()
	{
		nodes = new ArrayList();
		edges = new ArrayList();
                chemPropertyNames = new ArrayList();
	}
}

