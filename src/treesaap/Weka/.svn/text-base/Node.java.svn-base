/**
 * Data structure that stores a node in the graph
 */

package treesaap.Weka;

import java.util.*;

public class Node 
{
	private int nodeId;
	private double threshold;
        private char classLetter;
        private ArrayList<ArrayList<Double>> aaChemProperties;
        private ArrayList<Double> aaPresent;
        
	
	public Node(int nId)
	{
		Init(nId, 0.0);
	}
	
	public Node(int nId, double t)
	{
		Init(nId, t);
	}
	
	public int NodeId()
	{
		return nodeId;
	}
	
	public double GetThreshold()
	{
		return threshold;
	}
	
	public void SetThreshold(double t)
	{
		threshold = t;
	}
        
        public char GetClassLetter()
        {
            return classLetter;
        }
        
        public void SetClassLetter(char letter)
        {
            classLetter = letter;
        }
        
        /**
         * Adds the specified chemical property (number and weight)
         * @param aaNum
         * @param weight
         */
        public void AddAAChemicalProperty(int aaNum, double weight)
        {
            if (aaChemProperties.size() > aaNum)
            {
                ArrayList<Double> chems = aaChemProperties.get(aaNum);
                chems.add(new Double(weight));
            }
            else
            {
                ArrayList<Double> chems = new ArrayList<Double>();
                chems.add(new Double(weight));
                aaChemProperties.add(chems);
            }
        }
        
        public ArrayList<ArrayList<Double>> GetAAChemProperties()
        {
            return aaChemProperties;
        }
        
        public void AddAAPresent(int aaNum, double weight)
        {
            aaPresent.add(new Double(weight));
        }
        
        public ArrayList<Double> GetAAPresent()
        {
            return aaPresent;
        }

	private void Init(int nId, double t)
	{
		nodeId = nId;
		SetThreshold(t);
                classLetter = ' ';
                aaChemProperties = new ArrayList<ArrayList<Double>>();
                aaPresent = new ArrayList<Double>();
	}
}
