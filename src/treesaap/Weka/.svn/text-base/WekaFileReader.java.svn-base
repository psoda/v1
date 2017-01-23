/**
 * Parses the weka neural network file and loads it into a graph
 */

package treesaap.Weka;

import java.io.*;

public class WekaFileReader 
{
    private String filename;

    /**
     * Constructor
     * @param fileName file that contains weka neural network
     */
    public WekaFileReader(String fileName)
    {
        filename = fileName;
    }
    
    /**
     * Parses the neural network file and returns a graph
     * 
     * @return
     * @throws Weka.InvalidWekaFileException
     */
    public DiGraph ReadFile() throws InvalidWekaFileException
    {
        //System.out.println("Reading file...");
        DiGraph graph = new DiGraph();
		
        try
        {
            BufferedReader reader = new BufferedReader(new FileReader(filename));
            String line = null;
            Node node = null;

            while ((line = reader.readLine()) != null)
            {
                line = line.trim();
                String[] strings = line.split("\\s+");
                if (strings.length > 0)
                {
                    // Ex: Sigmoid Node 2
                    if ((strings.length == 3) && (strings[0].compareToIgnoreCase("Sigmoid") == 0))
                    {
                        // Save node that was being built
                        if (node != null) 
                        {
                            graph.AddNode(node);
                        }
                        
                        node = new Node(Integer.valueOf(strings[2]).intValue());
                    }
                    // Ex: Threshold -5.542
                    else if ((strings.length == 2) && (strings[0].compareToIgnoreCase("Threshold") == 0))
                    {
                        if (node != null)
                        {
                            node.SetThreshold(Double.valueOf(strings[1]).doubleValue());
                        }
                        else
                        {
                            throw new InvalidWekaFileException("Threshold: node is null");
                        }
                    }
                    // Ex: Node 5 -4.567
                    else if ((strings.length == 3) && (strings[0].compareToIgnoreCase("Node") == 0))
                    {
                        if (node != null)
                        {
                            graph.AddEdge(Integer.valueOf(strings[1]).intValue(), node.NodeId(), Double.valueOf(strings[2]).doubleValue());
                        }
                        else
                        {
                            throw new InvalidWekaFileException("Node: node is null");
                        }
                    }
                    else if ((strings.length == 3) && (strings[0].compareToIgnoreCase("Attrib") == 0))
                    {
                        if (node != null)
                        {
                            double weight = Double.valueOf(strings[2]).doubleValue();
                        
                            // Ex: Attrib aa6cp0 -5.5763
                            if (strings[1].contains("cp"))
                            {
                                node.AddAAChemicalProperty(GetNum("aa", "cp", strings[1]), weight);
                            }
                            // Ex: Attrib aa0Present -1.3442
                            else if (strings[1].contains("Present"))
                            {
                                node.AddAAPresent(GetNum("aa", "Present", strings[1]), weight);
                            }
                        }
                        else
                        {
                            throw new InvalidWekaFileException("Attrib: node is null");
                        }
                    }
                    else if ((strings.length == 2) && (strings[0].compareToIgnoreCase("Class") == 0))
                    {
                        char letter = strings[1].charAt(0);
                        // Grab the next two lines
                        line = reader.readLine();
                        line = reader.readLine();
                        line = line.trim();
                        strings = line.split("\\s+");
                        if ((strings.length == 2) && (strings[0].compareToIgnoreCase("Node") == 0))
                        {
                            int nodeNum = Integer.valueOf(strings[1]).intValue();
                            graph.AddClass(nodeNum, letter);
                        }
                    }
                    else if ((strings.length == 2) && (strings[0].startsWith("cp")))
                    {
                        graph.AddChemPropertyName(strings[1]);
                    }
                }
            }
            
            // Save off last node
            if (node != null) 
            {
                graph.AddNode(node);
            }
            
            reader.close();
        }
        catch (Exception e)
        {
            throw new InvalidWekaFileException("An error occured while reading the file: " + e.getMessage());
        }
        
        // if no chemical properties were in the file then throw an error
        // format:
        // cp0: Bulkiness
        // cp1: Alpha-helical_tendencies
        if (graph.GetChemPropertyNames().size() == 0)
        {
            throw new InvalidWekaFileException("No chemical property names were defined in the file");
        }
        
        return graph;
    }
    
    /**
     * Gets the number in searchStr that is between startStr and endStr
     * 
     * @param startStr
     * @param endStr
     * @param searchStr
     * @return
     */
    private int GetNum(String startStr, String endStr, String searchStr)
    {
        int startIndex = searchStr.indexOf(startStr);
        int endIndex = searchStr.indexOf(endStr);
        String temp = searchStr.substring(startIndex + startStr.length(), endIndex);
        return Integer.valueOf(temp).intValue();
    }
}
