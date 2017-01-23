/**
 * This class is used to predict the secondary structure of amino acid sequences. It reads in output of neural networks from Weka and uses the network to predict 
 * the secondary structure of a given sequence. Additional methods of secondary structure prediction can be added in the future. Ideally an abstract class will be
 * created that can be inhertied from, so they can be plugged in arbitrarily.
 */

package treesaap.Weka;

import java.util.*;
import treesaap.Data.ProteinBean;

public class wekasim {
    private int currCharIndex;
    private String sequence;
    private DiGraph graph;
    private Hashtable<Integer, Double> nodeValues;
    private Hashtable<Character, Character> threeClassReduction;
    private Hashtable proteinProperties;
    private Hashtable<Character, Integer> letterToIndexMap;
    
    /**
     * Constructor for wekasim
     * 
     * @param filename file that contains neural network (weka stdout)
     * @param proteinProperties contains the property name (string) as the key and a protein bean as the value
     * @throws Weka.InvalidWekaFileException
     */
    public wekasim(String filename, Hashtable proteinProperties) throws InvalidWekaFileException
    {
        WekaFileReader wfr = new WekaFileReader(filename);
        graph = wfr.ReadFile();
        this.proteinProperties = proteinProperties;
        Init();
    }
    
    
    /**
     * Simulates the neural network passed in through the constructor and outputs the predicted secondary structure
     * @param sequence amino acid sequence to run prediction on
     * @return predicted secondary structure
     * @throws Weka.InvalidWekaFileException
     * @throws Weka.ChemicalPropertiesException
     */
    public String Simulate(String sequence) throws InvalidWekaFileException, ChemicalPropertiesException
    {
        this.sequence = sequence;
        
        StringBuilder secondStruct = new StringBuilder(); // predicted secondary structure, use StringBuild because it is mutable
        ArrayList<Node> nodes = graph.GetNodeList();
        
        // loop over entire sequence, we need to find the output node of the neural net with the highest value
        for (currCharIndex = 0; currCharIndex < this.sequence.length(); currCharIndex++)
        {
            // want to use new values for the current character
            nodeValues.clear();
            double maxNodeValue = -60000.0;
            char maxNodeValLetter = ' '; // set to blank, this is just a initial value, should never be blank in the end
            
            // loop through all the nodes in network, only looking with output nodes
            for (int i = 0; i < nodes.size(); i++)
            {
                Node node = nodes.get(i);
                char classLetter = node.GetClassLetter(); // find secondary structure letter this node corresponds to
                
                // only output nodes have a letter, the rest are blank
                // take the largest node value, that is the secondary structure
                if (classLetter != ' ')
                {
                    double nodeVal = GetNodeValue(i);
                    if (nodeVal > maxNodeValue)
                    {
                        maxNodeValue = nodeVal;
                        maxNodeValLetter = classLetter;
                    }
                    
                    //System.out.println("SeqChar: " + sequence.charAt(currCharIndex) + " Node " + i + " value: " + nodeVal + " letter: " + classLetter);
                }
            }
            
            // Add letter to secondary structure string
            secondStruct.append(maxNodeValLetter);
        }
        
        // The secondary structure is filtered from 8 classes down to 3
        return FilterSecondStruct(secondStruct);
    }
    
    /**
     * Filters the secondary structure from 8 classes to 3 according to "Prediction of Protein Secondary Structure at Better than 70% Accuracy" (Rost & Sander, 1993)
     * 
     * @param secondStruct secondary structure string to filter
     * @return filtered secondary structure
     */
    private String FilterSecondStruct(StringBuilder secondStruct)
    {
        int hCount = 0;
        StringBuilder filteredSecondStruct = new StringBuilder();
        
        // perform corrections outlined in: Prediction of Protein Secondary Structure at Better than 70% Accuracy (Rost & Sander, 1993)
        secondStruct = StringReplace(secondStruct, "BCB", "CCC");
        secondStruct = StringReplace(secondStruct, "BC", "BB");
        
        // first convert to 3 class, and convert all single H and double H to C
        // conversions found in Rost & Sander paper
        for (int i = 0; i < secondStruct.length(); i++)
        {
            char letter = threeClassReduction.get(secondStruct.charAt(i));
            
            if (letter == 'H')
            {
                hCount++;
            }
            else 
            {
                // check if we only have 1 or 2 consecutive Hs, if so change to Cs
                if ((hCount == 1) || (hCount == 2))
                {
                    for (int j = 1; j <= hCount; j++)
                    {
                        filteredSecondStruct.setCharAt(i-j, 'C');
                    }
                }
                
                hCount = 0;
            }
            
            filteredSecondStruct.append(letter);
        }
        
        return filteredSecondStruct.toString();
    }
    
    /**
     * Get the value of the specified node
     * 
     * @param nodeNum the node number
     * @return the value of the specified node
     * @throws Weka.InvalidWekaFileException
     * @throws Weka.ChemicalPropertiesException
     */
    private double GetNodeValue(int nodeNum) throws InvalidWekaFileException, ChemicalPropertiesException
    {
        Integer nNum = new Integer(nodeNum);
        double nodeValue;
        
        // nodeValues is used to cache previously calculated values so we don't compute the same thing twice
        if (nodeValues.containsKey(nNum))
        {
            nodeValue = nodeValues.get(nNum);
        }
        else
        {
            nodeValue = CalcNodeValue(nodeNum);
            nodeValues.put(nNum, nodeValue);
        }
        
        return nodeValue;
    }
    
    /**
     * The value of a node is the result of multiplying each of the node's inputs with its corresponding weight and summing these values. This sum is the
     * input into the sigmoid function, and the output of this function is the final value of the node
     * 
     * @param nodeNum the node number to calculate the value of
     * @return the final value of the specified node
     * @throws Weka.InvalidWekaFileException
     * @throws Weka.ChemicalPropertiesException
     */
    private double CalcNodeValue(int nodeNum) throws InvalidWekaFileException, ChemicalPropertiesException
    {
        // get the threshold value, this isn't saved as an input in the graph, but it needs to be included (just add it in)
        double sum = graph.GetNode(nodeNum).GetThreshold();
        // get all the edges (inputs) that enter this node
        ArrayList<Edge> edges = graph.GetEdgesTo(nodeNum);
        
        // if we have edges then this is a middle or output node
        if (edges.size() > 0)
        {
            for (int i = 0; i < edges.size(); i++)
            {
                Edge edge = edges.get(i);
                sum += GetNodeValue(edge.NodeFrom()) * edge.Weight();
            }
        }
        // if we don't have edges this is an input node, calc using Attrib values
        else
        {
            sum += CalcAttribs(nodeNum);
        }

        return CalcSigmoid(sum);
    }
    
    
    
    /**
     * Find the node's value using its attrib values. We will make a window
     * that uses the characters in the sequence before and after the current
     * character
     * 
     * @param nodeNum the node number
     * @return the sum of multipying the Attrib values and their weights
     * @throws Weka.InvalidWekaFileException
     * @throws Weka.ChemicalPropertiesException
     */
    private double CalcAttribs(int nodeNum) throws InvalidWekaFileException, ChemicalPropertiesException
    {
        //System.out.println("Calcing attribs of node: " + nodeNum);
        Node node = graph.GetNode(nodeNum);
        ArrayList<Double> aaPresent = node.GetAAPresent();
        ArrayList<ArrayList<Double>> aaChemProperties = node.GetAAChemProperties();
        int windowSize = aaPresent.size();
        
        if ((windowSize % 2) == 0) //windowSize should always be odd
        {
            throw new InvalidWekaFileException("Number of aaPresent attributes must be odd, number found: " + windowSize);
        }
        
        if (aaChemProperties.size() <= 0)
        {
            throw new InvalidWekaFileException("Chemical property attributes (aaXcpY) not found");
        }
        
        if (proteinProperties.size() <= 0)
        {
            throw new ChemicalPropertiesException("No chemical properties defined");
        }
        
        if (graph.GetChemPropertyNames().size() != aaChemProperties.get(0).size())
        {
            throw new ChemicalPropertiesException("Not enough chemical properties defined, needed: " + graph.GetChemPropertyNames().size() + ", found: " + aaChemProperties.get(0).size());
        }
        
        // look at each character in the window, lining them up with their corresponding chemical property and using that value and the corresponding weight
        if (windowSize > 0)
        {
            int windowCenter = (windowSize / 2);
            double sum = 0.0;
            
            // loop through window
            for (int j = 0; j < windowSize; j++)
            {
                int charIndex = j - windowCenter + currCharIndex;
                if ((charIndex >= 0) && (charIndex < sequence.length()))
                {
                    sum += aaPresent.get(j);
                    
                    // loop through chemical properties
                    for (int k = 0; k < aaChemProperties.get(j).size(); k++)
                    {
                       sum += ChemPropValue(k, sequence.charAt(charIndex)) * aaChemProperties.get(j).get(k);
                    }
                }
            }
            
            return sum;
        }
        
        throw new InvalidWekaFileException("Number of aaPresent attributes must be greater than zero, number found: " + windowSize);
    }
    
    /**
     * Gets the chemical property value that corresponds to the specified protein character
     * 
     * @param chemPropNum
     * @param protein
     * @return
     * @throws Weka.ChemicalPropertiesException
     */
    private double ChemPropValue(int chemPropNum, char protein) throws ChemicalPropertiesException
    {
        String chemPropName = graph.GetChemPropertyNames().get(chemPropNum);
        chemPropName = chemPropName.replace('_', ' '); // need to replace _ with a space so names will match in hastable
        ProteinBean pb = (ProteinBean)proteinProperties.get(chemPropName);
        
        if (pb == null)
        {
            throw new ChemicalPropertiesException("Chemical property " + chemPropName + " not found in protein properties file.");
        }
        
        int index = letterToIndexMap.get(new Character(protein)).intValue();
        return pb.getPValues()[index];
    }
    
    /**
     * Calculates the sigmoid value of the argument
     * 
     * @param number
     * @return
     */
    private double CalcSigmoid(double number) 
    {
        return 1.0 / (1.0 + Math.exp(-number));
    }
    
    private StringBuilder StringReplace(StringBuilder sourceStr, String from, String to)
    {
        int index = sourceStr.indexOf(from);
        
        while (index != -1)
        {
            sourceStr = sourceStr.replace(index, to.length()+index, to);
            index = sourceStr.indexOf(from);
        }
        
        return sourceStr;
    }
    
    private void Init()
    {
        currCharIndex = 0;
        nodeValues = new Hashtable<Integer, Double>();
        threeClassReduction = new Hashtable<Character, Character>();
        
        // we will reduce our 8 class network to a three state one
        // reduction given by: Prediction of Protein Secondary Structure at Better than 70% Accuracy (Rost & Sander, 1993)
        // except B maps to B instead of C
        threeClassReduction.put('H', 'H');
        threeClassReduction.put('G', 'H');
        threeClassReduction.put('I', 'H');
        threeClassReduction.put('E', 'B');
        threeClassReduction.put('B', 'B');
        threeClassReduction.put('T', 'C');
        threeClassReduction.put('S', 'C');
        threeClassReduction.put('C', 'C');
        
        // need to resolve protein letters to indexes into protein arrays
        letterToIndexMap = new Hashtable<Character, Integer>();
        letterToIndexMap.put(new Character('A'), new Integer(0));
        letterToIndexMap.put(new Character('R'), new Integer(1));
        letterToIndexMap.put(new Character('N'), new Integer(2));
        letterToIndexMap.put(new Character('D'), new Integer(3));
        letterToIndexMap.put(new Character('C'), new Integer(4));
        letterToIndexMap.put(new Character('Q'), new Integer(5));
        letterToIndexMap.put(new Character('E'), new Integer(6));
        letterToIndexMap.put(new Character('G'), new Integer(7));
        letterToIndexMap.put(new Character('H'), new Integer(8));
        letterToIndexMap.put(new Character('I'), new Integer(9));
        letterToIndexMap.put(new Character('L'), new Integer(10));
        letterToIndexMap.put(new Character('K'), new Integer(11));
        letterToIndexMap.put(new Character('M'), new Integer(12));
        letterToIndexMap.put(new Character('F'), new Integer(13));
        letterToIndexMap.put(new Character('P'), new Integer(14));
        letterToIndexMap.put(new Character('S'), new Integer(15));
        letterToIndexMap.put(new Character('T'), new Integer(16));
        letterToIndexMap.put(new Character('W'), new Integer(17));
        letterToIndexMap.put(new Character('Y'), new Integer(18));
        letterToIndexMap.put(new Character('V'), new Integer(19));
    }
}
