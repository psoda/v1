/*
 * Jalview - A Sequence Alignment Editor and Viewer (Version 2.4)
 * Copyright (C) 2008 AM Waterhouse, J Procter, G Barton, M Clamp, S Searle
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */
package jalview.structure;

import java.io.*;
import java.util.*;

import MCview.*;
import jalview.analysis.*;
import jalview.datamodel.*;

public class StructureSelectionManager
{
  static StructureSelectionManager instance;

  StructureMapping[] mappings;

  Hashtable mappingData = new Hashtable();

  public static StructureSelectionManager getStructureSelectionManager()
  {
    if (instance == null)
    {
      instance = new StructureSelectionManager();
    }

    return instance;
  }

  /**
   * flag controlling whether SeqMappings are relayed from received sequence
   * mouse over events to other sequences
   */
  boolean relaySeqMappings = true;

  /**
   * Enable or disable relay of seqMapping events to other sequences. You might
   * want to do this if there are many sequence mappings and the host computer
   * is slow
   * 
   * @param relay
   */
  public void setRelaySeqMappings(boolean relay)
  {
    relaySeqMappings = relay;
  }

  /**
   * get the state of the relay seqMappings flag.
   * 
   * @return true if sequence mouse overs are being relayed to other mapped
   *         sequences
   */
  public boolean isRelaySeqMappingsEnabled()
  {
    return relaySeqMappings;
  }

  Vector listeners = new Vector();

  public void addStructureViewerListener(Object svl)
  {
    if (!listeners.contains(svl))
    {
      listeners.addElement(svl);
    }
  }

  public String alreadyMappedToFile(String pdbid)
  {
    if (mappings != null)
    {
      for (int i = 0; i < mappings.length; i++)
      {
        if (mappings[i].getPdbId().equals(pdbid))
        {
          return mappings[i].pdbfile;
        }
      }
    }
    return null;
  }

  /*
   * There will be better ways of doing this in the future, for now we'll use
   * the tried and tested MCview pdb mapping
   */
  synchronized public MCview.PDBfile setMapping(SequenceI[] sequence,
          String[] targetChains, String pdbFile, String protocol)
  {
    MCview.PDBfile pdb = null;
    try
    {
      pdb = new MCview.PDBfile(pdbFile, protocol);
    } catch (Exception ex)
    {
      ex.printStackTrace();
      return null;
    }

    String targetChain;
    for (int s = 0; s < sequence.length; s++)
    {
      if (targetChains != null && targetChains[s] != null)
        targetChain = targetChains[s];
      else if (sequence[s].getName().indexOf("|") > -1)
      {
        targetChain = sequence[s].getName().substring(
                sequence[s].getName().lastIndexOf("|") + 1);
      }
      else
        targetChain = "";

      int max = -10;
      AlignSeq maxAlignseq = null;
      String maxChainId = " ";
      PDBChain maxChain = null;

      for (int i = 0; i < pdb.chains.size(); i++)
      {
        AlignSeq as = new AlignSeq(sequence[s], ((PDBChain) pdb.chains
                .elementAt(i)).sequence, AlignSeq.PEP);
        as.calcScoreMatrix();
        as.traceAlignment();
        PDBChain chain = ((PDBChain) pdb.chains.elementAt(i));

        if (as.maxscore > max
                || (as.maxscore == max && chain.id.equals(targetChain)))
        {
          maxChain = chain;
          max = as.maxscore;
          maxAlignseq = as;
          maxChainId = chain.id;
        }
      }

      final StringBuffer mappingDetails = new StringBuffer();
      mappingDetails.append("\n\nPDB Sequence is :\nSequence = "
              + maxChain.sequence.getSequenceAsString());
      mappingDetails.append("\nNo of residues = "
              + maxChain.residues.size() + "\n\n");
      PrintStream ps = new PrintStream(System.out)
      {
        public void print(String x)
        {
          mappingDetails.append(x);
        }

        public void println()
        {
          mappingDetails.append("\n");
        }
      };

      maxAlignseq.printAlignment(ps);

      mappingDetails.append("\nPDB start/end " + maxAlignseq.seq2start
              + " " + maxAlignseq.seq2end);
      mappingDetails.append("\nSEQ start/end "
              + (maxAlignseq.seq1start + sequence[s].getStart() - 1) + " "
              + (maxAlignseq.seq1end + sequence[s].getEnd() - 1));

      maxChain.makeExactMapping(maxAlignseq, sequence[s]);

      maxChain.transferRESNUMFeatures(sequence[s], null);

      int[][] mapping = new int[sequence[s].getEnd() + 2][2];
      int resNum = -10000;
      int index = 0;

      do
      {
        Atom tmp = (Atom) maxChain.atoms.elementAt(index);
        if (resNum != tmp.resNumber && tmp.alignmentMapping != -1)
        {
          resNum = tmp.resNumber;
          mapping[tmp.alignmentMapping + 1][0] = tmp.resNumber;
          mapping[tmp.alignmentMapping + 1][1] = tmp.atomIndex;
        }

        index++;
      } while (index < maxChain.atoms.size());

      if (mappings == null)
      {
        mappings = new StructureMapping[1];
      }
      else
      {
        StructureMapping[] tmp = new StructureMapping[mappings.length + 1];
        System.arraycopy(mappings, 0, tmp, 0, mappings.length);
        mappings = tmp;
      }

      if (protocol.equals(jalview.io.AppletFormatAdapter.PASTE))
        pdbFile = "INLINE" + pdb.id;

      mappings[mappings.length - 1] = new StructureMapping(sequence[s],
              pdbFile, pdb.id, maxChainId, mapping, mappingDetails
                      .toString());
      maxChain.transferResidueAnnotation(mappings[mappings.length - 1]);
    }
    // ///////

    return pdb;
  }

  public void removeStructureViewerListener(Object svl, String pdbfile)
  {
    listeners.removeElement(svl);

    boolean removeMapping = true;

    StructureListener sl;
    for (int i = 0; i < listeners.size(); i++)
    {
      if (listeners.elementAt(i) instanceof StructureListener)
      {
        sl = (StructureListener) listeners.elementAt(i);
        if (sl.getPdbFile().equals(pdbfile))
        {
          removeMapping = false;
          break;
        }
      }
    }

    if (removeMapping && mappings != null)
    {
      Vector tmp = new Vector();
      for (int i = 0; i < mappings.length; i++)
      {
        if (!mappings[i].pdbfile.equals(pdbfile))
        {
          tmp.addElement(mappings[i]);
        }
      }

      mappings = new StructureMapping[tmp.size()];
      tmp.copyInto(mappings);
    }
  }

  public void mouseOverStructure(int pdbResNum, String chain, String pdbfile)
  {
    boolean hasSequenceListeners = handlingVamsasMo || seqmappings != null;
    SearchResults results = null;
    for (int i = 0; i < listeners.size(); i++)
    {
      if (listeners.elementAt(i) instanceof SequenceListener)
      {
        if (results == null)
        {
          results = new SearchResults();
        }
        int indexpos;
        for (int j = 0; j < mappings.length; j++)
        {
          if (mappings[j].pdbfile.equals(pdbfile)
                  && mappings[j].pdbchain.equals(chain))
          {
            indexpos = mappings[j].getSeqPos(pdbResNum);
            results.addResult(mappings[j].sequence, indexpos, indexpos);
            // construct highlighted sequence list
            if (seqmappings != null)
            {

              Enumeration e = seqmappings.elements();
              while (e.hasMoreElements())

              {
                ((AlignedCodonFrame) e.nextElement()).markMappedRegion(
                        mappings[j].sequence, indexpos, results);
              }
            }
          }
        }
      }
    }
    if (results.getSize() > 0)
    {
      for (int i = 0; i < listeners.size(); i++)
      {
        Object li = listeners.elementAt(i);
        if (li instanceof SequenceListener)
          ((SequenceListener) li).highlightSequence(results);
      }
    }
  }

  Vector seqmappings = null; // should be a simpler list of mapped seuqence

  /**
   * highlight regions associated with a position (indexpos) in seq
   * 
   * @param seq
   *                the sequeence that the mouse over occured on
   * @param indexpos
   *                the absolute position being mouseovered in seq (0 to
   *                seq.length())
   * @param index
   *                the sequence position (if -1, seq.findPosition is called to
   *                resolve the residue number)
   */
  public void mouseOverSequence(SequenceI seq, int indexpos, int index)
  {
    boolean hasSequenceListeners = handlingVamsasMo || seqmappings != null;
    SearchResults results = null;
    if (index == -1)
      index = seq.findPosition(indexpos);
    StructureListener sl;
    int atomNo = 0;
    for (int i = 0; i < listeners.size(); i++)
    {
      if (listeners.elementAt(i) instanceof StructureListener)
      {
        sl = (StructureListener) listeners.elementAt(i);

        for (int j = 0; j < mappings.length; j++)
        {
          if (mappings[j].sequence == seq
                  || mappings[j].sequence == seq.getDatasetSequence())
          {
            atomNo = mappings[j].getAtomNum(index);

            if (atomNo > 0)
            {
              sl.highlightAtom(atomNo, mappings[j].getPDBResNum(index),
                      mappings[j].pdbchain, mappings[j].pdbfile);
            }
          }
        }
      }
      else
      {
        if (relaySeqMappings && hasSequenceListeners
                && listeners.elementAt(i) instanceof SequenceListener)
        {
          // DEBUG
          // System.err.println("relay Seq " + seq.getDisplayId(false) + " " +
          // index);

          if (results == null)
          {
            results = new SearchResults();
            if (index >= seq.getStart() && index <= seq.getEnd())
            {
              // construct highlighted sequence list

              if (seqmappings != null)
              {
                Enumeration e = seqmappings.elements();
                while (e.hasMoreElements())

                {
                  ((AlignedCodonFrame) e.nextElement()).markMappedRegion(
                          seq, index, results);
                }
              }
              // hasSequenceListeners = results.getSize() > 0;
              if (handlingVamsasMo)
              {
                // maybe have to resolve seq to a dataset seqeunce...
                // add in additional direct sequence and/or dataset sequence
                // highlighting
                results.addResult(seq, index, index);
              }
            }
          }
          if (hasSequenceListeners)
          {
            ((SequenceListener) listeners.elementAt(i))
                    .highlightSequence(results);
          }
        }
        else if (listeners.elementAt(i) instanceof VamsasListener
                && !handlingVamsasMo)
        {
          // DEBUG
          // System.err.println("Vamsas from Seq " + seq.getDisplayId(false) + "
          // " +
          // index);
          // pass the mouse over and absolute position onto the
          // VamsasListener(s)
          ((VamsasListener) listeners.elementAt(i))
                  .mouseOver(seq, indexpos);
        }
      }
    }
  }

  /**
   * true if a mouse over event from an external (ie Vamsas) source is being
   * handled
   */
  boolean handlingVamsasMo = false;

  long lastmsg = 0;

  /**
   * as mouseOverSequence but only route event to SequenceListeners
   * 
   * @param sequenceI
   * @param position
   *                in an alignment sequence
   */
  public void mouseOverVamsasSequence(SequenceI sequenceI, int position)
  {
    handlingVamsasMo = true;
    long msg = sequenceI.hashCode() * (1 + position);
    if (lastmsg != msg)
    {
      lastmsg = msg;
      mouseOverSequence(sequenceI, position, -1);
    }
    handlingVamsasMo = false;
  }

  public Annotation[] colourSequenceFromStructure(SequenceI seq,
          String pdbid)
  {
    return null;
    // THIS WILL NOT BE AVAILABLE IN JALVIEW 2.3,
    // UNTIL THE COLOUR BY ANNOTATION IS REWORKED
    /*
     * Annotation [] annotations = new Annotation[seq.getLength()];
     * 
     * StructureListener sl; int atomNo = 0; for (int i = 0; i <
     * listeners.size(); i++) { if (listeners.elementAt(i) instanceof
     * StructureListener) { sl = (StructureListener) listeners.elementAt(i);
     * 
     * for (int j = 0; j < mappings.length; j++) {
     * 
     * if (mappings[j].sequence == seq && mappings[j].getPdbId().equals(pdbid) &&
     * mappings[j].pdbfile.equals(sl.getPdbFile())) { System.out.println(pdbid+"
     * "+mappings[j].getPdbId() +" "+mappings[j].pdbfile);
     * 
     * java.awt.Color col; for(int index=0; index<seq.getLength(); index++) {
     * if(jalview.util.Comparison.isGap(seq.getCharAt(index))) continue;
     * 
     * atomNo = mappings[j].getAtomNum(seq.findPosition(index)); col =
     * java.awt.Color.white; if (atomNo > 0) { col = sl.getColour(atomNo,
     * mappings[j].getPDBResNum(index), mappings[j].pdbchain,
     * mappings[j].pdbfile); }
     * 
     * annotations[index] = new Annotation("X",null,' ',0,col); } return
     * annotations; } } } }
     * 
     * return annotations;
     */
  }

  public void structureSelectionChanged()
  {
  }

  public void sequenceSelectionChanged()
  {
  }

  public void sequenceColoursChanged(Object source)
  {
    StructureListener sl;
    for (int i = 0; i < listeners.size(); i++)
    {
      if (listeners.elementAt(i) instanceof StructureListener)
      {
        sl = (StructureListener) listeners.elementAt(i);
        sl.updateColours(source);
      }
    }
  }

  public StructureMapping[] getMapping(String pdbfile)
  {
    Vector tmp = new Vector();
    for (int i = 0; i < mappings.length; i++)
    {
      if (mappings[i].pdbfile.equals(pdbfile))
      {
        tmp.addElement(mappings[i]);
      }
    }

    StructureMapping[] ret = new StructureMapping[tmp.size()];
    for (int i = 0; i < tmp.size(); i++)
    {
      ret[i] = (StructureMapping) tmp.elementAt(i);
    }

    return ret;
  }

  public String printMapping(String pdbfile)
  {
    StringBuffer sb = new StringBuffer();
    for (int i = 0; i < mappings.length; i++)
    {
      if (mappings[i].pdbfile.equals(pdbfile))
      {
        sb.append(mappings[i].mappingDetails);
      }
    }

    return sb.toString();
  }

  private int[] seqmappingrefs = null; // refcount for seqmappings elements

  private synchronized void modifySeqMappingList(boolean add,
          AlignedCodonFrame[] codonFrames)
  {
    if (!add && (seqmappings == null || seqmappings.size() == 0))
      return;
    if (seqmappings == null)
      seqmappings = new Vector();
    if (codonFrames != null && codonFrames.length > 0)
    {
      for (int cf = 0; cf < codonFrames.length; cf++)
      {
        if (seqmappings.contains(codonFrames[cf]))
        {
          if (add)
          {
            seqmappingrefs[seqmappings.indexOf(codonFrames[cf])]++;
          }
          else
          {
            if (--seqmappingrefs[seqmappings.indexOf(codonFrames[cf])] <= 0)
            {
              int pos = seqmappings.indexOf(codonFrames[cf]);
              int[] nr = new int[seqmappingrefs.length - 1];
              if (pos > 0)
              {
                System.arraycopy(seqmappingrefs, 0, nr, 0, pos);
              }
              if (pos < seqmappingrefs.length - 1)
              {
                System.arraycopy(seqmappingrefs, pos + 1, nr, 0,
                        seqmappingrefs.length - pos - 2);
              }
            }
          }
        }
        else
        {
          if (add)
          {
            seqmappings.addElement(codonFrames[cf]);

            int[] nsr = new int[(seqmappingrefs == null) ? 1
                    : seqmappingrefs.length + 1];
            if (seqmappingrefs != null && seqmappingrefs.length > 0)
              System.arraycopy(seqmappingrefs, 0, nsr, 0,
                      seqmappingrefs.length);
            nsr[(seqmappingrefs == null) ? 0 : seqmappingrefs.length] = 1;
            seqmappingrefs = nsr;
          }
        }
      }
    }
  }

  public void removeMappings(AlignedCodonFrame[] codonFrames)
  {
    modifySeqMappingList(false, codonFrames);
  }

  public void addMappings(AlignedCodonFrame[] codonFrames)
  {
    modifySeqMappingList(true, codonFrames);
  }
}
