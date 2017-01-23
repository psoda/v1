package treesaap.Weka;

import java.util.*;

public class TestDriver 
{
    /**
     * @param args
    */
    public static void main(String[] args) 
    {
        try
        {
            // need to populate this with properties Key = property name, value = populated protein bean
            Hashtable proteinProperties = new Hashtable();
            
            // file: 1ndh_A.ss
            // output should be: CCBSCCCTTSEEEEEEEEEEECCSSEEEEEECCSSTTCBCCCCTTCEEEEEEEETTEEEEEEECCSSCTTSCSCCCEEEEECCCTTSSSCTTCCHHHHHHHHCCTTCEEEEEEEECSEEEEETTEEEECSSSSCEEECCCTTEEEEEEEGGGHHHHHHHHHHHHHCTTCCCEEEEEEEEEEGGGCTTIIIIICCHHHHTTTEEEEEEEEECCTTCCSEESCCTTHHHHHHSCCGGGCCEEESSSCSGGGTTSSCHHHHHTTCCGGGEECC
            String sequence = "PAITLENPDIKYPLRLIDKEVVNHDTRRFRFALPSPEHILGLPVGQHIYLSARIDGNLVIRPYTPVSSDDDKGFVDLVIKVYFKDTHPKFPAGGKMSQYLESMKIGDTIEFRGPNGLLVYQGKGKFAIRPDKKSSPVIKTVKSVGMIAGGTGITPMLQVIRAIMKDPDDHTVCHLLFANQTEKDILLRPELEELRNEHSARFKLWYTVDRAPEAWDYSQGFVNEEMIRDHLPPPEEEPLVLMCGPPPMIQYACLPNLERVGHPKERCFAF";
            wekasim ws = new wekasim("Data/SECONDARY_STRUCTURES/secondarystructure.txt", proteinProperties);
            String output = ws.Simulate(sequence);
            System.out.println(" Input: " + sequence);
            System.out.println("Output: " + output);
        }
        catch (Exception e)
        {
            System.out.println(e.getMessage());
        }
    }
}
