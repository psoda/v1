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
package jalview.gui;

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.jar.*;

import javax.swing.*;

import org.exolab.castor.xml.*;

import uk.ac.vamsas.objects.utils.MapList;
import jalview.datamodel.Alignment;
import jalview.datamodel.AlignmentI;
import jalview.datamodel.SequenceI;
import jalview.schemabinding.version2.*;
import jalview.schemes.*;
import jalview.structure.StructureSelectionManager;

/**
 * Write out the current jalview desktop state as a Jalview XML stream.
 * 
 * Note: the vamsas objects referred to here are primitive versions of the
 * VAMSAS project schema elements - they are not the same and most likely never
 * will be :)
 * 
 * @author $author$
 * @version $Revision: 1.106.2.2 $
 */
public class Jalview2XML
{
  /**
   * create/return unique hash string for sq
   * 
   * @param sq
   * @return new or existing unique string for sq
   */
  String seqHash(SequenceI sq)
  {
    if (seqsToIds == null)
    {
      initSeqRefs();
    }
    if (seqsToIds.containsKey(sq))
    {
      return (String) seqsToIds.get(sq);
    }
    else
    {
      // create sequential key
      String key = "sq" + (seqsToIds.size() + 1);
      seqsToIds.put(sq, key);
      return key;
    }
  }

  void clearSeqRefs()
  {
    seqRefIds.clear();
    seqsToIds.clear();
  }

  void initSeqRefs()
  {
    if (seqsToIds == null)
    {
      seqsToIds = new IdentityHashMap();
    }
    if (seqRefIds == null)
    {
      seqRefIds = new Hashtable();
    }
  }

  java.util.IdentityHashMap seqsToIds = null; // SequenceI->key resolution

  java.util.Hashtable seqRefIds = null; // key->SequenceI resolution

  Vector frefedSequence = null;

  boolean raiseGUI = true; // whether errors are raised in dialog boxes or not

  public Jalview2XML()
  {
  }

  public Jalview2XML(boolean raiseGUI)
  {
    this.raiseGUI = raiseGUI;
  }

  public void resolveFrefedSequences()
  {
    if (frefedSequence.size() > 0)
    {
      int r = 0, rSize = frefedSequence.size();
      while (r < rSize)
      {
        Object[] ref = (Object[]) frefedSequence.elementAt(r);
        if (ref != null)
        {
          String sref = (String) ref[0];
          if (seqRefIds.containsKey(sref))
          {
            if (ref[1] instanceof jalview.datamodel.Mapping)
            {
              SequenceI seq = (SequenceI) seqRefIds.get(sref);
              while (seq.getDatasetSequence() != null)
              {
                seq = seq.getDatasetSequence();
              }
              ((jalview.datamodel.Mapping) ref[1]).setTo(seq);
            }
            else
            {
              if (ref[1] instanceof jalview.datamodel.AlignedCodonFrame)
              {
                SequenceI seq = (SequenceI) seqRefIds.get(sref);
                while (seq.getDatasetSequence() != null)
                {
                  seq = seq.getDatasetSequence();
                }
                if (ref[2] != null
                        && ref[2] instanceof jalview.datamodel.Mapping)
                {
                  jalview.datamodel.Mapping mp = (jalview.datamodel.Mapping) ref[2];
                  ((jalview.datamodel.AlignedCodonFrame) ref[1]).addMap(
                          seq, mp.getTo(), mp.getMap());
                }
                else
                {
                  System.err
                          .println("IMPLEMENTATION ERROR: Unimplemented forward sequence references for AlcodonFrames involving "
                                  + ref[2].getClass() + " type objects.");
                }
              }
              else
              {
                System.err
                        .println("IMPLEMENTATION ERROR: Unimplemented forward sequence references for "
                                + ref[1].getClass() + " type objects.");
              }
              frefedSequence.remove(r);
              rSize--;
            }
          }
          else
          {
            System.err
                    .println("IMPLEMENTATION WARNING: Unresolved forward reference for hash string "
                            + ref[0]
                            + " with objecttype "
                            + ref[1].getClass());
            r++;
          }
        }
        else
        {
          frefedSequence.remove(r);
          rSize--;
        }
      }
    }
  }

  /**
   * This maintains a list of viewports, the key being the seqSetId. Important
   * to set historyItem and redoList for multiple views
   */
  Hashtable viewportsAdded;

  Hashtable annotationIds = new Hashtable();

  String uniqueSetSuffix = "";

  /**
   * List of pdbfiles added to Jar
   */
  Vector pdbfiles = null;

  // SAVES SEVERAL ALIGNMENT WINDOWS TO SAME JARFILE
  public void SaveState(File statefile)
  {
    JInternalFrame[] frames = Desktop.desktop.getAllFrames();

    if (frames == null)
    {
      return;
    }

    try
    {
      FileOutputStream fos = new FileOutputStream(statefile);
      JarOutputStream jout = new JarOutputStream(fos);

      // NOTE UTF-8 MUST BE USED FOR WRITING UNICODE CHARS
      // //////////////////////////////////////////////////
      // NOTE ALSO new PrintWriter must be used for each new JarEntry
      PrintWriter out = null;

      Vector shortNames = new Vector();

      // REVERSE ORDER
      for (int i = frames.length - 1; i > -1; i--)
      {
        if (frames[i] instanceof AlignFrame)
        {
          AlignFrame af = (AlignFrame) frames[i];

          String shortName = af.getTitle();

          if (shortName.indexOf(File.separatorChar) > -1)
          {
            shortName = shortName.substring(shortName
                    .lastIndexOf(File.separatorChar) + 1);
          }

          int count = 1;

          while (shortNames.contains(shortName))
          {
            if (shortName.endsWith("_" + (count - 1)))
            {
              shortName = shortName
                      .substring(0, shortName.lastIndexOf("_"));
            }

            shortName = shortName.concat("_" + count);
            count++;
          }

          shortNames.addElement(shortName);

          if (!shortName.endsWith(".xml"))
          {
            shortName = shortName + ".xml";
          }

          int ap, apSize = af.alignPanels.size();
          for (ap = 0; ap < apSize; ap++)
          {
            AlignmentPanel apanel = (AlignmentPanel) af.alignPanels
                    .elementAt(ap);
            String fileName = apSize == 1 ? shortName : ap + shortName;
            if (!fileName.endsWith(".xml"))
            {
              fileName = fileName + ".xml";
            }

            SaveState(apanel, fileName, jout);
          }
        }
      }
      try
      {
        jout.flush();
      } catch (Exception foo)
      {
      }
      ;
      jout.close();
    } catch (Exception ex)
    {
      // TODO: inform user of the problem - they need to know if their data was
      // not saved !
      ex.printStackTrace();
    }
  }

  // USE THIS METHOD TO SAVE A SINGLE ALIGNMENT WINDOW
  public boolean SaveAlignment(AlignFrame af, String jarFile,
          String fileName)
  {
    try
    {
      int ap, apSize = af.alignPanels.size();
      FileOutputStream fos = new FileOutputStream(jarFile);
      JarOutputStream jout = new JarOutputStream(fos);
      for (ap = 0; ap < apSize; ap++)
      {
        AlignmentPanel apanel = (AlignmentPanel) af.alignPanels
                .elementAt(ap);
        String jfileName = apSize == 1 ? fileName : fileName + ap;
        if (!jfileName.endsWith(".xml"))
        {
          jfileName = jfileName + ".xml";
        }
        SaveState(apanel, jfileName, jout);
      }

      try
      {
        jout.flush();
      } catch (Exception foo)
      {
      }
      ;
      jout.close();
      return true;
    } catch (Exception ex)
    {
      ex.printStackTrace();
      return false;
    }
  }

  /**
   * create a JalviewModel from an algnment view and marshall it to a
   * JarOutputStream
   * 
   * @param ap
   *                panel to create jalview model for
   * @param fileName
   *                name of alignment panel written to output stream
   * @param jout
   *                jar output stream
   * @param out
   *                jar entry name
   */
  public JalviewModel SaveState(AlignmentPanel ap, String fileName,
          JarOutputStream jout)
  {
    initSeqRefs();

    Vector userColours = new Vector();

    AlignViewport av = ap.av;

    JalviewModel object = new JalviewModel();
    object.setVamsasModel(new jalview.schemabinding.version2.VamsasModel());

    object.setCreationDate(new java.util.Date(System.currentTimeMillis()));
    object.setVersion(jalview.bin.Cache.getProperty("VERSION"));

    jalview.datamodel.AlignmentI jal = av.alignment;

    if (av.hasHiddenRows)
    {
      jal = jal.getHiddenSequences().getFullAlignment();
    }

    SequenceSet vamsasSet = new SequenceSet();
    Sequence vamsasSeq;
    JalviewModelSequence jms = new JalviewModelSequence();

    vamsasSet.setGapChar(jal.getGapCharacter() + "");

    if (jal.getDataset() != null)
    {
      // dataset id is the dataset's hashcode
      vamsasSet.setDatasetId(jal.getDataset().hashCode() + "");
    }
    if (jal.getProperties() != null)
    {
      Enumeration en = jal.getProperties().keys();
      while (en.hasMoreElements())
      {
        String key = en.nextElement().toString();
        SequenceSetProperties ssp = new SequenceSetProperties();
        ssp.setKey(key);
        ssp.setValue(jal.getProperties().get(key).toString());
        vamsasSet.addSequenceSetProperties(ssp);
      }
    }

    JSeq jseq;

    // SAVE SEQUENCES
    String id = "";
    jalview.datamodel.SequenceI jds;
    for (int i = 0; i < jal.getHeight(); i++)
    {
      jds = jal.getSequenceAt(i);
      id = seqHash(jds);

      if (seqRefIds.get(id) != null)
      {
        // This happens for two reasons: 1. multiple views are being serialised.
        // 2. the hashCode has collided with another sequence's code. This DOES
        // HAPPEN! (PF00072.15.stk does this)
        // JBPNote: Uncomment to debug writing out of files that do not read
        // back in due to ArrayOutOfBoundExceptions.
        // System.err.println("vamsasSeq backref: "+id+"");
        // System.err.println(jds.getName()+"
        // "+jds.getStart()+"-"+jds.getEnd()+" "+jds.getSequenceAsString());
        // System.err.println("Hashcode: "+seqHash(jds));
        // SequenceI rsq = (SequenceI) seqRefIds.get(id + "");
        // System.err.println(rsq.getName()+"
        // "+rsq.getStart()+"-"+rsq.getEnd()+" "+rsq.getSequenceAsString());
        // System.err.println("Hashcode: "+seqHash(rsq));
      }
      else
      {
        vamsasSeq = createVamsasSequence(id, jds);
        vamsasSet.addSequence(vamsasSeq);
        seqRefIds.put(id, jds);
      }

      jseq = new JSeq();
      jseq.setStart(jds.getStart());
      jseq.setEnd(jds.getEnd());
      jseq.setColour(av.getSequenceColour(jds).getRGB());

      jseq.setId(id); // jseq id should be a string not a number

      if (av.hasHiddenRows)
      {
        jseq.setHidden(av.alignment.getHiddenSequences().isHidden(jds));

        if (av.hiddenRepSequences != null
                && av.hiddenRepSequences.containsKey(jal.getSequenceAt(i)))
        {
          jalview.datamodel.SequenceI[] reps = ((jalview.datamodel.SequenceGroup) av.hiddenRepSequences
                  .get(jal.getSequenceAt(i))).getSequencesInOrder(jal);

          for (int h = 0; h < reps.length; h++)
          {
            if (reps[h] != jal.getSequenceAt(i))
            {
              jseq.addHiddenSequences(jal.findIndex(reps[h]));
            }
          }
        }
      }

      if (jds.getDatasetSequence().getSequenceFeatures() != null)
      {
        jalview.datamodel.SequenceFeature[] sf = jds.getDatasetSequence()
                .getSequenceFeatures();
        int index = 0;
        while (index < sf.length)
        {
          Features features = new Features();

          features.setBegin(sf[index].getBegin());
          features.setEnd(sf[index].getEnd());
          features.setDescription(sf[index].getDescription());
          features.setType(sf[index].getType());
          features.setFeatureGroup(sf[index].getFeatureGroup());
          features.setScore(sf[index].getScore());
          if (sf[index].links != null)
          {
            for (int l = 0; l < sf[index].links.size(); l++)
            {
              OtherData keyValue = new OtherData();
              keyValue.setKey("LINK_" + l);
              keyValue.setValue(sf[index].links.elementAt(l).toString());
              features.addOtherData(keyValue);
            }
          }
          if (sf[index].otherDetails != null)
          {
            String key;
            Enumeration keys = sf[index].otherDetails.keys();
            while (keys.hasMoreElements())
            {
              key = keys.nextElement().toString();
              OtherData keyValue = new OtherData();
              keyValue.setKey(key);
              keyValue.setValue(sf[index].otherDetails.get(key).toString());
              features.addOtherData(keyValue);
            }
          }

          jseq.addFeatures(features);
          index++;
        }
      }

      if (jds.getDatasetSequence().getPDBId() != null)
      {
        Enumeration en = jds.getDatasetSequence().getPDBId().elements();
        while (en.hasMoreElements())
        {
          Pdbids pdb = new Pdbids();
          jalview.datamodel.PDBEntry entry = (jalview.datamodel.PDBEntry) en
                  .nextElement();

          pdb.setId(entry.getId());
          pdb.setType(entry.getType());

          AppJmol jmol;
          // This must have been loaded, is it still visible?
          JInternalFrame[] frames = Desktop.desktop.getAllFrames();
          for (int f = frames.length - 1; f > -1; f--)
          {
            if (frames[f] instanceof AppJmol)
            {
              jmol = (AppJmol) frames[f];
              if (!jmol.pdbentry.getId().equals(entry.getId()))
                continue;

              StructureState state = new StructureState();
              state.setVisible(true);
              state.setXpos(jmol.getX());
              state.setYpos(jmol.getY());
              state.setWidth(jmol.getWidth());
              state.setHeight(jmol.getHeight());

              String statestring = jmol.viewer.getStateInfo();
              if (state != null)
              {
                state.setContent(statestring.replaceAll("\n", ""));
              }
              for (int s = 0; s < jmol.sequence.length; s++)
              {
                if (jal.findIndex(jmol.sequence[s]) > -1)
                {
                  pdb.addStructureState(state);
                }
              }
            }
          }

          if (entry.getFile() != null)
          {
            pdb.setFile(entry.getFile());
            if (pdbfiles == null)
            {
              pdbfiles = new Vector();
            }

            if (!pdbfiles.contains(entry.getId()))
            {
              pdbfiles.addElement(entry.getId());
              try
              {
                File file = new File(entry.getFile());
                if (file.exists() && jout != null)
                {
                  byte[] data = new byte[(int) file.length()];
                  jout.putNextEntry(new JarEntry(entry.getId()));
                  DataInputStream dis = new DataInputStream(
                          new FileInputStream(file));
                  dis.readFully(data);

                  DataOutputStream dout = new DataOutputStream(jout);
                  dout.write(data, 0, data.length);
                  dout.flush();
                  jout.closeEntry();
                }
              } catch (Exception ex)
              {
                ex.printStackTrace();
              }

            }
          }

          if (entry.getProperty() != null)
          {
            PdbentryItem item = new PdbentryItem();
            Hashtable properties = entry.getProperty();
            Enumeration en2 = properties.keys();
            while (en2.hasMoreElements())
            {
              Property prop = new Property();
              String key = en2.nextElement().toString();
              prop.setName(key);
              prop.setValue(properties.get(key).toString());
              item.addProperty(prop);
            }
            pdb.addPdbentryItem(item);
          }

          jseq.addPdbids(pdb);
        }
      }

      jms.addJSeq(jseq);
    }

    if (av.hasHiddenRows)
    {
      jal = av.alignment;
    }
    // SAVE MAPPINGS
    if (jal.getCodonFrames() != null && jal.getCodonFrames().length > 0)
    {
      jalview.datamodel.AlignedCodonFrame[] jac = jal.getCodonFrames();
      for (int i = 0; i < jac.length; i++)
      {
        AlcodonFrame alc = new AlcodonFrame();
        vamsasSet.addAlcodonFrame(alc);
        for (int p = 0; p < jac[i].aaWidth; p++)
        {
          Alcodon cmap = new Alcodon();
          cmap.setPos1(jac[i].codons[p][0]);
          cmap.setPos2(jac[i].codons[p][1]);
          cmap.setPos3(jac[i].codons[p][2]);
          alc.addAlcodon(cmap);
        }
        if (jac[i].getProtMappings() != null
                && jac[i].getProtMappings().length > 0)
        {
          SequenceI[] dnas = jac[i].getdnaSeqs();
          jalview.datamodel.Mapping[] pmaps = jac[i].getProtMappings();
          for (int m = 0; m < pmaps.length; m++)
          {
            AlcodMap alcmap = new AlcodMap();
            alcmap.setDnasq("" + dnas[m].hashCode());
            alcmap.setMapping(createVamsasMapping(pmaps[m], dnas[m], null,
                    false));
            alc.addAlcodMap(alcmap);
          }
        }
      }
    }

    // SAVE TREES
    // /////////////////////////////////
    if (av.currentTree != null)
    {
      // FIND ANY ASSOCIATED TREES
      // NOT IMPLEMENTED FOR HEADLESS STATE AT PRESENT
      if (Desktop.desktop != null)
      {
        JInternalFrame[] frames = Desktop.desktop.getAllFrames();

        for (int t = 0; t < frames.length; t++)
        {
          if (frames[t] instanceof TreePanel)
          {
            TreePanel tp = (TreePanel) frames[t];

            if (tp.treeCanvas.av.alignment == jal)
            {
              Tree tree = new Tree();
              tree.setTitle(tp.getTitle());
              tree.setCurrentTree((av.currentTree == tp.getTree()));
              tree.setNewick(tp.getTree().toString());
              tree.setThreshold(tp.treeCanvas.threshold);

              tree.setFitToWindow(tp.fitToWindow.getState());
              tree.setFontName(tp.getTreeFont().getName());
              tree.setFontSize(tp.getTreeFont().getSize());
              tree.setFontStyle(tp.getTreeFont().getStyle());
              tree.setMarkUnlinked(tp.placeholdersMenu.getState());

              tree.setShowBootstrap(tp.bootstrapMenu.getState());
              tree.setShowDistances(tp.distanceMenu.getState());

              tree.setHeight(tp.getHeight());
              tree.setWidth(tp.getWidth());
              tree.setXpos(tp.getX());
              tree.setYpos(tp.getY());

              jms.addTree(tree);
            }
          }
        }
      }
    }

    // SAVE ANNOTATIONS
    if (jal.getAlignmentAnnotation() != null)
    {
      jalview.datamodel.AlignmentAnnotation[] aa = jal
              .getAlignmentAnnotation();

      for (int i = 0; i < aa.length; i++)
      {
        Annotation an = new Annotation();

        if (aa[i].annotationId != null)
        {
          annotationIds.put(aa[i].annotationId, aa[i]);
        }

        an.setId(aa[i].annotationId);

        if (aa[i] == av.quality || aa[i] == av.conservation
                || aa[i] == av.consensus)
        {
          an.setLabel(aa[i].label);
          an.setGraph(true);
          vamsasSet.addAnnotation(an);
          continue;
        }

        an.setVisible(aa[i].visible);

        an.setDescription(aa[i].description);

        if (aa[i].sequenceRef != null)
        {
          an.setSequenceRef(aa[i].sequenceRef.getName());
        }

        if (aa[i].graph > 0)
        {
          an.setGraph(true);
          an.setGraphType(aa[i].graph);
          an.setGraphGroup(aa[i].graphGroup);
          if (aa[i].getThreshold() != null)
          {
            ThresholdLine line = new ThresholdLine();
            line.setLabel(aa[i].getThreshold().label);
            line.setValue(aa[i].getThreshold().value);
            line.setColour(aa[i].getThreshold().colour.getRGB());
            an.setThresholdLine(line);
          }
        }
        else
        {
          an.setGraph(false);
        }

        an.setLabel(aa[i].label);
        if (aa[i].hasScore())
        {
          an.setScore(aa[i].getScore());
        }
        AnnotationElement ae;
        if (aa[i].annotations != null)
        {
          an.setScoreOnly(false);
          for (int a = 0; a < aa[i].annotations.length; a++)
          {
            if ((aa[i] == null) || (aa[i].annotations[a] == null))
            {
              continue;
            }

            ae = new AnnotationElement();
            if (aa[i].annotations[a].description != null)
              ae.setDescription(aa[i].annotations[a].description);
            if (aa[i].annotations[a].displayCharacter != null)
              ae.setDisplayCharacter(aa[i].annotations[a].displayCharacter);

            if (!Float.isNaN(aa[i].annotations[a].value))
              ae.setValue(aa[i].annotations[a].value);

            ae.setPosition(a);
            if (aa[i].annotations[a].secondaryStructure != ' '
                    && aa[i].annotations[a].secondaryStructure != '\0')
              ae
                      .setSecondaryStructure(aa[i].annotations[a].secondaryStructure
                              + "");

            if (aa[i].annotations[a].colour != null
                    && aa[i].annotations[a].colour != java.awt.Color.black)
            {
              ae.setColour(aa[i].annotations[a].colour.getRGB());
            }

            an.addAnnotationElement(ae);
          }
        }
        else
        {
          an.setScoreOnly(true);
        }
        vamsasSet.addAnnotation(an);
      }
    }

    // SAVE GROUPS
    if (jal.getGroups() != null)
    {
      JGroup[] groups = new JGroup[jal.getGroups().size()];

      for (int i = 0; i < groups.length; i++)
      {
        groups[i] = new JGroup();

        jalview.datamodel.SequenceGroup sg = (jalview.datamodel.SequenceGroup) jal
                .getGroups().elementAt(i);
        groups[i].setStart(sg.getStartRes());
        groups[i].setEnd(sg.getEndRes());
        groups[i].setName(sg.getName());
        if (sg.cs != null)
        {
          if (sg.cs.conservationApplied())
          {
            groups[i].setConsThreshold(sg.cs.getConservationInc());

            if (sg.cs instanceof jalview.schemes.UserColourScheme)
            {
              groups[i].setColour(SetUserColourScheme(sg.cs, userColours,
                      jms));
            }
            else
            {
              groups[i]
                      .setColour(ColourSchemeProperty.getColourName(sg.cs));
            }
          }
          else if (sg.cs instanceof jalview.schemes.AnnotationColourGradient)
          {
            groups[i]
                    .setColour(ColourSchemeProperty
                            .getColourName(((jalview.schemes.AnnotationColourGradient) sg.cs)
                                    .getBaseColour()));
          }
          else if (sg.cs instanceof jalview.schemes.UserColourScheme)
          {
            groups[i]
                    .setColour(SetUserColourScheme(sg.cs, userColours, jms));
          }
          else
          {
            groups[i].setColour(ColourSchemeProperty.getColourName(sg.cs));
          }

          groups[i].setPidThreshold(sg.cs.getThreshold());
        }

        groups[i].setOutlineColour(sg.getOutlineColour().getRGB());
        groups[i].setDisplayBoxes(sg.getDisplayBoxes());
        groups[i].setDisplayText(sg.getDisplayText());
        groups[i].setColourText(sg.getColourText());
        groups[i].setTextCol1(sg.textColour.getRGB());
        groups[i].setTextCol2(sg.textColour2.getRGB());
        groups[i].setTextColThreshold(sg.thresholdTextColour);

        for (int s = 0; s < sg.getSize(); s++)
        {
          jalview.datamodel.Sequence seq = (jalview.datamodel.Sequence) sg
                  .getSequenceAt(s);
          groups[i].addSeq(seqHash(seq));
        }
      }

      jms.setJGroup(groups);
    }

    // /////////SAVE VIEWPORT
    Viewport view = new Viewport();
    view.setTitle(ap.alignFrame.getTitle());
    view.setSequenceSetId(av.getSequenceSetId());
    view.setViewName(av.viewName);
    view.setGatheredViews(av.gatherViewsHere);

    if (ap.av.explodedPosition != null)
    {
      view.setXpos(av.explodedPosition.x);
      view.setYpos(av.explodedPosition.y);
      view.setWidth(av.explodedPosition.width);
      view.setHeight(av.explodedPosition.height);
    }
    else
    {
      view.setXpos(ap.alignFrame.getBounds().x);
      view.setYpos(ap.alignFrame.getBounds().y);
      view.setWidth(ap.alignFrame.getBounds().width);
      view.setHeight(ap.alignFrame.getBounds().height);
    }

    view.setStartRes(av.startRes);
    view.setStartSeq(av.startSeq);

    if (av.getGlobalColourScheme() instanceof jalview.schemes.UserColourScheme)
    {
      view.setBgColour(SetUserColourScheme(av.getGlobalColourScheme(),
              userColours, jms));
    }
    else if (av.getGlobalColourScheme() instanceof jalview.schemes.AnnotationColourGradient)
    {
      jalview.schemes.AnnotationColourGradient acg = (jalview.schemes.AnnotationColourGradient) av
              .getGlobalColourScheme();

      AnnotationColours ac = new AnnotationColours();
      ac.setAboveThreshold(acg.getAboveThreshold());
      ac.setThreshold(acg.getAnnotationThreshold());
      ac.setAnnotation(acg.getAnnotation());
      if (acg.getBaseColour() instanceof jalview.schemes.UserColourScheme)
      {
        ac.setColourScheme(SetUserColourScheme(acg.getBaseColour(),
                userColours, jms));
      }
      else
      {
        ac.setColourScheme(ColourSchemeProperty.getColourName(acg
                .getBaseColour()));
      }

      ac.setMaxColour(acg.getMaxColour().getRGB());
      ac.setMinColour(acg.getMinColour().getRGB());
      view.setAnnotationColours(ac);
      view.setBgColour("AnnotationColourGradient");
    }
    else
    {
      view.setBgColour(ColourSchemeProperty.getColourName(av
              .getGlobalColourScheme()));
    }

    ColourSchemeI cs = av.getGlobalColourScheme();

    if (cs != null)
    {
      if (cs.conservationApplied())
      {
        view.setConsThreshold(cs.getConservationInc());
        if (cs instanceof jalview.schemes.UserColourScheme)
        {
          view.setBgColour(SetUserColourScheme(cs, userColours, jms));
        }
      }

      if (cs instanceof ResidueColourScheme)
      {
        view.setPidThreshold(cs.getThreshold());
      }
    }

    view.setConservationSelected(av.getConservationSelected());
    view.setPidSelected(av.getAbovePIDThreshold());
    view.setFontName(av.font.getName());
    view.setFontSize(av.font.getSize());
    view.setFontStyle(av.font.getStyle());
    view.setRenderGaps(av.renderGaps);
    view.setShowAnnotation(av.getShowAnnotation());
    view.setShowBoxes(av.getShowBoxes());
    view.setShowColourText(av.getColourText());
    view.setShowFullId(av.getShowJVSuffix());
    view.setRightAlignIds(av.rightAlignIds);
    view.setShowSequenceFeatures(av.showSequenceFeatures);
    view.setShowText(av.getShowText());
    view.setWrapAlignment(av.getWrapAlignment());
    view.setTextCol1(av.textColour.getRGB());
    view.setTextCol2(av.textColour2.getRGB());
    view.setTextColThreshold(av.thresholdTextColour);

    if (av.featuresDisplayed != null)
    {
      jalview.schemabinding.version2.FeatureSettings fs = new jalview.schemabinding.version2.FeatureSettings();

      String[] renderOrder = ap.seqPanel.seqCanvas.getFeatureRenderer().renderOrder;

      Vector settingsAdded = new Vector();
      for (int ro = 0; ro < renderOrder.length; ro++)
      {
        Setting setting = new Setting();
        setting.setType(renderOrder[ro]);
        setting.setColour(ap.seqPanel.seqCanvas.getFeatureRenderer()
                .getColour(renderOrder[ro]).getRGB());

        setting.setDisplay(av.featuresDisplayed
                .containsKey(renderOrder[ro]));
        float rorder = ap.seqPanel.seqCanvas.getFeatureRenderer().getOrder(
                renderOrder[ro]);
        if (rorder > -1)
        {
          setting.setOrder(rorder);
        }
        fs.addSetting(setting);
        settingsAdded.addElement(renderOrder[ro]);
      }

      // Make sure we save none displayed feature settings
      Enumeration en = ap.seqPanel.seqCanvas.getFeatureRenderer().featureColours
              .keys();
      while (en.hasMoreElements())
      {
        String key = en.nextElement().toString();
        if (settingsAdded.contains(key))
        {
          continue;
        }

        Setting setting = new Setting();
        setting.setType(key);
        setting.setColour(ap.seqPanel.seqCanvas.getFeatureRenderer()
                .getColour(key).getRGB());

        setting.setDisplay(false);
        float rorder = ap.seqPanel.seqCanvas.getFeatureRenderer().getOrder(
                key);
        if (rorder > -1)
        {
          setting.setOrder(rorder);
        }
        fs.addSetting(setting);
        settingsAdded.addElement(key);
      }
      en = ap.seqPanel.seqCanvas.getFeatureRenderer().featureGroups.keys();
      Vector groupsAdded = new Vector();
      while (en.hasMoreElements())
      {
        String grp = en.nextElement().toString();
        if (groupsAdded.contains(grp))
        {
          continue;
        }
        Group g = new Group();
        g.setName(grp);
        g
                .setDisplay(((Boolean) ap.seqPanel.seqCanvas
                        .getFeatureRenderer().featureGroups.get(grp))
                        .booleanValue());
        fs.addGroup(g);
        groupsAdded.addElement(grp);
      }
      jms.setFeatureSettings(fs);

    }

    if (av.hasHiddenColumns)
    {
      for (int c = 0; c < av.getColumnSelection().getHiddenColumns().size(); c++)
      {
        int[] region = (int[]) av.getColumnSelection().getHiddenColumns()
                .elementAt(c);
        HiddenColumns hc = new HiddenColumns();
        hc.setStart(region[0]);
        hc.setEnd(region[1]);
        view.addHiddenColumns(hc);
      }
    }

    jms.addViewport(view);

    object.setJalviewModelSequence(jms);
    object.getVamsasModel().addSequenceSet(vamsasSet);

    if (jout != null && fileName != null)
    {
      // We may not want to write the object to disk,
      // eg we can copy the alignViewport to a new view object
      // using save and then load
      try
      {
        JarEntry entry = new JarEntry(fileName);
        jout.putNextEntry(entry);
        PrintWriter pout = new PrintWriter(new OutputStreamWriter(jout,
                "UTF-8"));
        org.exolab.castor.xml.Marshaller marshaller = new org.exolab.castor.xml.Marshaller(
                pout);
        marshaller.marshal(object);
        pout.flush();
        jout.closeEntry();
      } catch (Exception ex)
      {
        // TODO: raise error in GUI if marshalling failed.
        ex.printStackTrace();
      }
    }
    return object;
  }

  private Sequence createVamsasSequence(String id, SequenceI jds)
  {
    return createVamsasSequence(true, id, jds, null);
  }

  private Sequence createVamsasSequence(boolean recurse, String id,
          SequenceI jds, SequenceI parentseq)
  {
    Sequence vamsasSeq = new Sequence();
    vamsasSeq.setId(id);
    vamsasSeq.setName(jds.getName());
    vamsasSeq.setSequence(jds.getSequenceAsString());
    vamsasSeq.setDescription(jds.getDescription());
    jalview.datamodel.DBRefEntry[] dbrefs = null;
    if (jds.getDatasetSequence() != null)
    {
      vamsasSeq.setDsseqid(seqHash(jds.getDatasetSequence()));
      if (jds.getDatasetSequence().getDBRef() != null)
      {
        dbrefs = jds.getDatasetSequence().getDBRef();
      }
    }
    else
    {
      vamsasSeq.setDsseqid(id); // so we can tell which sequences really are
                                // dataset sequences only
      dbrefs = jds.getDBRef();
    }
    if (dbrefs != null)
    {
      for (int d = 0; d < dbrefs.length; d++)
      {
        DBRef dbref = new DBRef();
        dbref.setSource(dbrefs[d].getSource());
        dbref.setVersion(dbrefs[d].getVersion());
        dbref.setAccessionId(dbrefs[d].getAccessionId());
        if (dbrefs[d].hasMap())
        {
          Mapping mp = createVamsasMapping(dbrefs[d].getMap(), parentseq,
                  jds, recurse);
          dbref.setMapping(mp);
        }
        vamsasSeq.addDBRef(dbref);
      }
    }
    return vamsasSeq;
  }

  private Mapping createVamsasMapping(jalview.datamodel.Mapping jmp,
          SequenceI parentseq, SequenceI jds, boolean recurse)
  {
    Mapping mp = null;
    if (jmp.getMap() != null)
    {
      mp = new Mapping();

      jalview.util.MapList mlst = jmp.getMap();
      int r[] = mlst.getFromRanges();
      for (int s = 0; s < r.length; s += 2)
      {
        MapListFrom mfrom = new MapListFrom();
        mfrom.setStart(r[s]);
        mfrom.setEnd(r[s + 1]);
        mp.addMapListFrom(mfrom);
      }
      r = mlst.getToRanges();
      for (int s = 0; s < r.length; s += 2)
      {
        MapListTo mto = new MapListTo();
        mto.setStart(r[s]);
        mto.setEnd(r[s + 1]);
        mp.addMapListTo(mto);
      }
      mp.setMapFromUnit(mlst.getFromRatio());
      mp.setMapToUnit(mlst.getToRatio());
      if (jmp.getTo() != null)
      {
        MappingChoice mpc = new MappingChoice();
        if (recurse
                && (parentseq != jmp.getTo() || parentseq
                        .getDatasetSequence() != jmp.getTo()))
        {
          mpc.setSequence(createVamsasSequence(false, seqHash(jmp.getTo()),
                  jmp.getTo(), jds));
        }
        else
        {
          String jmpid = "";
          SequenceI ps = null;
          if (parentseq != jmp.getTo()
                  && parentseq.getDatasetSequence() != jmp.getTo())
          {
            // chaining dbref rather than a handshaking one
            jmpid = seqHash(ps = jmp.getTo());
          }
          else
          {
            jmpid = seqHash(ps = parentseq);
          }
          mpc.setDseqFor(jmpid);
          if (!seqRefIds.containsKey(mpc.getDseqFor()))
          {
            jalview.bin.Cache.log.debug("creatign new DseqFor ID");
            seqRefIds.put(mpc.getDseqFor(), ps);
          }
          else
          {
            jalview.bin.Cache.log.debug("reusing DseqFor ID");
          }
        }
        mp.setMappingChoice(mpc);
      }
    }
    return mp;
  }

  String SetUserColourScheme(jalview.schemes.ColourSchemeI cs,
          Vector userColours, JalviewModelSequence jms)
  {
    String id = null;
    jalview.schemes.UserColourScheme ucs = (jalview.schemes.UserColourScheme) cs;

    if (!userColours.contains(ucs))
    {
      userColours.add(ucs);

      java.awt.Color[] colours = ucs.getColours();
      jalview.schemabinding.version2.UserColours uc = new jalview.schemabinding.version2.UserColours();
      jalview.schemabinding.version2.UserColourScheme jbucs = new jalview.schemabinding.version2.UserColourScheme();

      for (int i = 0; i < colours.length; i++)
      {
        jalview.schemabinding.version2.Colour col = new jalview.schemabinding.version2.Colour();
        col.setName(ResidueProperties.aa[i]);
        col.setRGB(jalview.util.Format.getHexString(colours[i]));
        jbucs.addColour(col);
      }
      if (ucs.getLowerCaseColours() != null)
      {
        colours = ucs.getLowerCaseColours();
        for (int i = 0; i < colours.length; i++)
        {
          jalview.schemabinding.version2.Colour col = new jalview.schemabinding.version2.Colour();
          col.setName(ResidueProperties.aa[i].toLowerCase());
          col.setRGB(jalview.util.Format.getHexString(colours[i]));
          jbucs.addColour(col);
        }
      }

      id = "ucs" + userColours.indexOf(ucs);
      uc.setId(id);
      uc.setUserColourScheme(jbucs);
      jms.addUserColours(uc);
    }

    return id;
  }

  jalview.schemes.UserColourScheme GetUserColourScheme(
          JalviewModelSequence jms, String id)
  {
    UserColours[] uc = jms.getUserColours();
    UserColours colours = null;

    for (int i = 0; i < uc.length; i++)
    {
      if (uc[i].getId().equals(id))
      {
        colours = uc[i];

        break;
      }
    }

    java.awt.Color[] newColours = new java.awt.Color[24];

    for (int i = 0; i < 24; i++)
    {
      newColours[i] = new java.awt.Color(Integer.parseInt(colours
              .getUserColourScheme().getColour(i).getRGB(), 16));
    }

    jalview.schemes.UserColourScheme ucs = new jalview.schemes.UserColourScheme(
            newColours);

    if (colours.getUserColourScheme().getColourCount() > 24)
    {
      newColours = new java.awt.Color[23];
      for (int i = 0; i < 23; i++)
      {
        newColours[i] = new java.awt.Color(Integer.parseInt(colours
                .getUserColourScheme().getColour(i + 24).getRGB(), 16));
      }
      ucs.setLowerCaseColours(newColours);
    }

    return ucs;
  }

  /**
   * DOCUMENT ME!
   * 
   * @param file
   *                DOCUMENT ME!
   */
  public AlignFrame LoadJalviewAlign(final String file)
  {
    uniqueSetSuffix = System.currentTimeMillis() % 100000 + "";

    jalview.gui.AlignFrame af = null;

    seqRefIds = new Hashtable();
    viewportsAdded = new Hashtable();
    frefedSequence = new Vector();
    Hashtable gatherToThisFrame = new Hashtable();

    String errorMessage = null;

    try
    {
      // UNMARSHALLER SEEMS TO CLOSE JARINPUTSTREAM, MOST ANNOYING
      URL url = null;

      if (file.startsWith("http://"))
      {
        url = new URL(file);
      }

      JarInputStream jin = null;
      JarEntry jarentry = null;
      int entryCount = 1;

      do
      {
        if (url != null)
        {
          jin = new JarInputStream(url.openStream());
        }
        else
        {
          jin = new JarInputStream(new FileInputStream(file));
        }

        for (int i = 0; i < entryCount; i++)
        {
          jarentry = jin.getNextJarEntry();
        }

        if (jarentry != null && jarentry.getName().endsWith(".xml"))
        {
          InputStreamReader in = new InputStreamReader(jin, "UTF-8");
          JalviewModel object = new JalviewModel();

          Unmarshaller unmar = new Unmarshaller(object);
          unmar.setValidation(false);
          object = (JalviewModel) unmar.unmarshal(in);

          af = LoadFromObject(object, file, true);
          if (af.viewport.gatherViewsHere)
          {
            gatherToThisFrame.put(af.viewport.getSequenceSetId(), af);
          }
          entryCount++;
        }
        else if (jarentry != null)
        {
          // Some other file here.
          entryCount++;
        }
      } while (jarentry != null);
      resolveFrefedSequences();
    } catch (java.io.FileNotFoundException ex)
    {
      ex.printStackTrace();
      errorMessage = "Couldn't locate Jalview XML file : " + file;
      System.err.println("Exception whilst loading jalview XML file : "
              + ex + "\n");
    } catch (java.net.UnknownHostException ex)
    {
      ex.printStackTrace();
      errorMessage = "Couldn't locate Jalview XML file : " + file;
      System.err.println("Exception whilst loading jalview XML file : "
              + ex + "\n");
    } catch (Exception ex)
    {
      System.err.println("Parsing as Jalview Version 2 file failed.");
      ex.printStackTrace(System.err);

      // Is Version 1 Jar file?
      try
      {
        af = new Jalview2XML_V1(raiseGUI).LoadJalviewAlign(file);
      } catch (Exception ex2)
      {
        System.err.println("Exception whilst loading as jalviewXMLV1:");
        ex2.printStackTrace();
        af = null;
      }

      if (Desktop.instance != null)
      {
        Desktop.instance.stopLoading();
      }
      if (af != null)
      {
        System.out.println("Successfully loaded archive file");
        return af;
      }
      ex.printStackTrace();

      System.err.println("Exception whilst loading jalview XML file : "
              + ex + "\n");
    }

    if (Desktop.instance != null)
    {
      Desktop.instance.stopLoading();
    }

    Enumeration en = gatherToThisFrame.elements();
    while (en.hasMoreElements())
    {
      Desktop.instance.gatherViews((AlignFrame) en.nextElement());
    }

    if (errorMessage != null)
    {
      final String finalErrorMessage = errorMessage;
      if (raiseGUI)
      {
        javax.swing.SwingUtilities.invokeLater(new Runnable()
        {
          public void run()
          {
            JOptionPane.showInternalMessageDialog(Desktop.desktop,
                    finalErrorMessage, "Error loading Jalview file",
                    JOptionPane.WARNING_MESSAGE);
          }
        });
      }
      else
      {
        System.err.println("Problem loading Jalview file: " + errorMessage);
      }
    }

    return af;
  }

  Hashtable alreadyLoadedPDB;

  String loadPDBFile(String file, String pdbId)
  {
    if (alreadyLoadedPDB == null)
      alreadyLoadedPDB = new Hashtable();

    if (alreadyLoadedPDB.containsKey(pdbId))
      return alreadyLoadedPDB.get(pdbId).toString();

    try
    {
      JarInputStream jin = null;

      if (file.startsWith("http://"))
      {
        jin = new JarInputStream(new URL(file).openStream());
      }
      else
      {
        jin = new JarInputStream(new FileInputStream(file));
      }

      JarEntry entry = null;
      do
      {
        entry = jin.getNextJarEntry();
      } while (!entry.getName().equals(pdbId));

      BufferedReader in = new BufferedReader(new InputStreamReader(jin));
      File outFile = File.createTempFile("jalview_pdb", ".txt");
      outFile.deleteOnExit();
      PrintWriter out = new PrintWriter(new FileOutputStream(outFile));
      String data;

      while ((data = in.readLine()) != null)
      {
        out.println(data);
      }
      try
      {
        out.flush();
      } catch (Exception foo)
      {
      }
      ;
      out.close();

      alreadyLoadedPDB.put(pdbId, outFile.getAbsolutePath());
      return outFile.getAbsolutePath();

    } catch (Exception ex)
    {
      ex.printStackTrace();
    }

    return null;
  }

  AlignFrame LoadFromObject(JalviewModel object, String file,
          boolean loadTreesAndStructures)
  {
    SequenceSet vamsasSet = object.getVamsasModel().getSequenceSet(0);
    Sequence[] vamsasSeq = vamsasSet.getSequence();

    JalviewModelSequence jms = object.getJalviewModelSequence();

    Viewport view = jms.getViewport(0);

    // ////////////////////////////////
    // LOAD SEQUENCES

    Vector hiddenSeqs = null;
    jalview.datamodel.Sequence jseq;

    ArrayList tmpseqs = new ArrayList();

    boolean multipleView = false;

    JSeq[] JSEQ = object.getJalviewModelSequence().getJSeq();
    int vi = 0; // counter in vamsasSeq array
    for (int i = 0; i < JSEQ.length; i++)
    {
      String seqId = JSEQ[i].getId() + "";

      if (seqRefIds.get(seqId) != null)
      {
        tmpseqs.add((jalview.datamodel.Sequence) seqRefIds.get(seqId));
        multipleView = true;
      }
      else
      {
        jseq = new jalview.datamodel.Sequence(vamsasSeq[vi].getName(),
                vamsasSeq[vi].getSequence());
        jseq.setDescription(vamsasSeq[vi].getDescription());
        jseq.setStart(JSEQ[i].getStart());
        jseq.setEnd(JSEQ[i].getEnd());
        jseq.setVamsasId(uniqueSetSuffix + seqId);
        seqRefIds.put(vamsasSeq[vi].getId() + "", jseq);
        tmpseqs.add(jseq);
        vi++;
      }

      if (JSEQ[i].getHidden())
      {
        if (hiddenSeqs == null)
        {
          hiddenSeqs = new Vector();
        }

        hiddenSeqs.addElement((jalview.datamodel.Sequence) seqRefIds
                .get(seqId));
      }

    }

    // /
    // Create the alignment object from the sequence set
    // ///////////////////////////////
    jalview.datamodel.Sequence[] orderedSeqs = new jalview.datamodel.Sequence[tmpseqs
            .size()];

    tmpseqs.toArray(orderedSeqs);

    jalview.datamodel.Alignment al = new jalview.datamodel.Alignment(
            orderedSeqs);

    // / Add the alignment properties
    for (int i = 0; i < vamsasSet.getSequenceSetPropertiesCount(); i++)
    {
      SequenceSetProperties ssp = vamsasSet.getSequenceSetProperties(i);
      al.setProperty(ssp.getKey(), ssp.getValue());
    }

    // /
    // SequenceFeatures are added to the DatasetSequence,
    // so we must create or recover the dataset before loading features
    // ///////////////////////////////
    if (vamsasSet.getDatasetId() == null || vamsasSet.getDatasetId() == "")
    {
      // older jalview projects do not have a dataset id.
      al.setDataset(null);
    }
    else
    {
      recoverDatasetFor(vamsasSet, al);
    }
    // ///////////////////////////////

    Hashtable pdbloaded = new Hashtable();
    if (!multipleView)
    {
      for (int i = 0; i < vamsasSeq.length; i++)
      {
        if (JSEQ[i].getFeaturesCount() > 0)
        {
          Features[] features = JSEQ[i].getFeatures();
          for (int f = 0; f < features.length; f++)
          {
            jalview.datamodel.SequenceFeature sf = new jalview.datamodel.SequenceFeature(
                    features[f].getType(), features[f].getDescription(),
                    features[f].getStatus(), features[f].getBegin(),
                    features[f].getEnd(), features[f].getFeatureGroup());

            sf.setScore(features[f].getScore());
            for (int od = 0; od < features[f].getOtherDataCount(); od++)
            {
              OtherData keyValue = features[f].getOtherData(od);
              if (keyValue.getKey().startsWith("LINK"))
              {
                sf.addLink(keyValue.getValue());
              }
              else
              {
                sf.setValue(keyValue.getKey(), keyValue.getValue());
              }

            }

            al.getSequenceAt(i).getDatasetSequence().addSequenceFeature(sf);
          }
        }
        if (vamsasSeq[i].getDBRefCount() > 0)
        {
          addDBRefs(al.getSequenceAt(i).getDatasetSequence(), vamsasSeq[i]);
        }
        if (JSEQ[i].getPdbidsCount() > 0)
        {
          Pdbids[] ids = JSEQ[i].getPdbids();
          for (int p = 0; p < ids.length; p++)
          {
            jalview.datamodel.PDBEntry entry = new jalview.datamodel.PDBEntry();
            entry.setId(ids[p].getId());
            entry.setType(ids[p].getType());
            if (ids[p].getFile() != null)
            {
              if (!pdbloaded.containsKey(ids[p].getFile()))
              {
                entry.setFile(loadPDBFile(file, ids[p].getId()));
              }
              else
              {
                entry.setFile(pdbloaded.get(ids[p].getId()).toString());
              }
            }

            al.getSequenceAt(i).getDatasetSequence().addPDBId(entry);
          }
        }
      }
    }

    // ///////////////////////////////
    // LOAD SEQUENCE MAPPINGS
    if (vamsasSet.getAlcodonFrameCount() > 0)
    {
      AlcodonFrame[] alc = vamsasSet.getAlcodonFrame();
      for (int i = 0; i < alc.length; i++)
      {
        jalview.datamodel.AlignedCodonFrame cf = new jalview.datamodel.AlignedCodonFrame(
                alc[i].getAlcodonCount());
        if (alc[i].getAlcodonCount() > 0)
        {
          Alcodon[] alcods = alc[i].getAlcodon();
          for (int p = 0; p < cf.codons.length; p++)
          {
            cf.codons[p] = new int[3];
            cf.codons[p][0] = (int) alcods[p].getPos1();
            cf.codons[p][1] = (int) alcods[p].getPos2();
            cf.codons[p][2] = (int) alcods[p].getPos3();
          }
        }
        if (alc[i].getAlcodMapCount() > 0)
        {
          AlcodMap[] maps = alc[i].getAlcodMap();
          for (int m = 0; m < maps.length; m++)
          {
            SequenceI dnaseq = (SequenceI) seqRefIds
                    .get(maps[m].getDnasq());
            // Load Mapping
            jalview.datamodel.Mapping mapping = null;
            // attach to dna sequence reference.
            if (maps[m].getMapping() != null)
            {
              mapping = addMapping(maps[m].getMapping());
            }
            if (dnaseq != null)
            {
              cf.addMap(dnaseq, mapping.getTo(), mapping.getMap());
            }
            else
            {
              // defer to later
              frefedSequence.add(new Object[]
              { maps[m].getDnasq(), cf, mapping });
            }
          }
        }
        al.addCodonFrame(cf);
      }

    }

    // ////////////////////////////////
    // LOAD ANNOTATIONS
    boolean hideQuality = true, hideConservation = true, hideConsensus = true;

    if (vamsasSet.getAnnotationCount() > 0)
    {
      Annotation[] an = vamsasSet.getAnnotation();

      for (int i = 0; i < an.length; i++)
      {
        if (an[i].getLabel().equals("Quality"))
        {
          hideQuality = false;
          continue;
        }
        else if (an[i].getLabel().equals("Conservation"))
        {
          hideConservation = false;
          continue;
        }
        else if (an[i].getLabel().equals("Consensus"))
        {
          hideConsensus = false;
          continue;
        }

        if (an[i].getId() != null
                && annotationIds.containsKey(an[i].getId()))
        {
          jalview.datamodel.AlignmentAnnotation jda = (jalview.datamodel.AlignmentAnnotation) annotationIds
                  .get(an[i].getId());
          if (an[i].hasVisible())
            jda.visible = an[i].getVisible();

          al.addAnnotation(jda);

          continue;
        }

        AnnotationElement[] ae = an[i].getAnnotationElement();
        jalview.datamodel.Annotation[] anot = null;

        if (!an[i].getScoreOnly())
        {
          anot = new jalview.datamodel.Annotation[al.getWidth()];

          for (int aa = 0; aa < ae.length && aa < anot.length; aa++)
          {
            if (ae[aa].getPosition() >= anot.length)
              continue;

            anot[ae[aa].getPosition()] = new jalview.datamodel.Annotation(

            ae[aa].getDisplayCharacter(), ae[aa].getDescription(), (ae[aa]
                    .getSecondaryStructure() == null || ae[aa]
                    .getSecondaryStructure().length() == 0) ? ' ' : ae[aa]
                    .getSecondaryStructure().charAt(0), ae[aa].getValue()

            );
            // JBPNote: Consider verifying dataflow for IO of secondary
            // structure annotation read from Stockholm files
            // this was added to try to ensure that
            // if (anot[ae[aa].getPosition()].secondaryStructure>' ')
            // {
            // anot[ae[aa].getPosition()].displayCharacter = "";
            // }
            anot[ae[aa].getPosition()].colour = new java.awt.Color(ae[aa]
                    .getColour());
          }
        }
        jalview.datamodel.AlignmentAnnotation jaa = null;

        if (an[i].getGraph())
        {
          jaa = new jalview.datamodel.AlignmentAnnotation(an[i].getLabel(),
                  an[i].getDescription(), anot, 0, 0, an[i].getGraphType());

          jaa.graphGroup = an[i].getGraphGroup();

          if (an[i].getThresholdLine() != null)
          {
            jaa.setThreshold(new jalview.datamodel.GraphLine(an[i]
                    .getThresholdLine().getValue(), an[i]
                    .getThresholdLine().getLabel(), new java.awt.Color(
                    an[i].getThresholdLine().getColour())));

          }

        }
        else
        {
          jaa = new jalview.datamodel.AlignmentAnnotation(an[i].getLabel(),
                  an[i].getDescription(), anot);
        }

        if (an[i].getId() != null)
        {
          annotationIds.put(an[i].getId(), jaa);
          jaa.annotationId = an[i].getId();
        }

        if (an[i].getSequenceRef() != null)
        {
          if (al.findName(an[i].getSequenceRef()) != null)
          {
            jaa.createSequenceMapping(al.findName(an[i].getSequenceRef()),
                    1, true);
            al.findName(an[i].getSequenceRef()).addAlignmentAnnotation(jaa);
          }
        }
        if (an[i].hasScore())
        {
          jaa.setScore(an[i].getScore());
        }

        if (an[i].hasVisible())
          jaa.visible = an[i].getVisible();

        al.addAnnotation(jaa);
      }
    }

    // ///////////////////////
    // LOAD GROUPS
    if (jms.getJGroupCount() > 0)
    {
      JGroup[] groups = jms.getJGroup();

      for (int i = 0; i < groups.length; i++)
      {
        ColourSchemeI cs = null;

        if (groups[i].getColour() != null)
        {
          if (groups[i].getColour().startsWith("ucs"))
          {
            cs = GetUserColourScheme(jms, groups[i].getColour());
          }
          else
          {
            cs = ColourSchemeProperty.getColour(al, groups[i].getColour());
          }

          if (cs != null)
          {
            cs.setThreshold(groups[i].getPidThreshold(), true);
          }
        }

        Vector seqs = new Vector();

        for (int s = 0; s < groups[i].getSeqCount(); s++)
        {
          String seqId = groups[i].getSeq(s) + "";
          jalview.datamodel.SequenceI ts = (jalview.datamodel.SequenceI) seqRefIds
                  .get(seqId);

          if (ts != null)
          {
            seqs.addElement(ts);
          }
        }

        if (seqs.size() < 1)
        {
          continue;
        }

        jalview.datamodel.SequenceGroup sg = new jalview.datamodel.SequenceGroup(
                seqs, groups[i].getName(), cs, groups[i].getDisplayBoxes(),
                groups[i].getDisplayText(), groups[i].getColourText(),
                groups[i].getStart(), groups[i].getEnd());

        sg
                .setOutlineColour(new java.awt.Color(groups[i]
                        .getOutlineColour()));

        sg.textColour = new java.awt.Color(groups[i].getTextCol1());
        sg.textColour2 = new java.awt.Color(groups[i].getTextCol2());
        sg.thresholdTextColour = groups[i].getTextColThreshold();

        if (groups[i].getConsThreshold() != 0)
        {
          jalview.analysis.Conservation c = new jalview.analysis.Conservation(
                  "All", ResidueProperties.propHash, 3, sg
                          .getSequences(null), 0, sg.getWidth() - 1);
          c.calculate();
          c.verdict(false, 25);
          sg.cs.setConservation(c);
        }

        al.addGroup(sg);
      }
    }

    // ///////////////////////////////
    // LOAD VIEWPORT

    AlignFrame af = new AlignFrame(al, view.getWidth(), view.getHeight());

    af.setFileName(file, "Jalview");

    for (int i = 0; i < JSEQ.length; i++)
    {
      af.viewport.setSequenceColour(af.viewport.alignment.getSequenceAt(i),
              new java.awt.Color(JSEQ[i].getColour()));
    }

    // If we just load in the same jar file again, the sequenceSetId
    // will be the same, and we end up with multiple references
    // to the same sequenceSet. We must modify this id on load
    // so that each load of the file gives a unique id
    String uniqueSeqSetId = view.getSequenceSetId() + uniqueSetSuffix;

    af.viewport.gatherViewsHere = view.getGatheredViews();

    if (view.getSequenceSetId() != null)
    {
      jalview.gui.AlignViewport av = (jalview.gui.AlignViewport) viewportsAdded
              .get(uniqueSeqSetId);

      af.viewport.sequenceSetID = uniqueSeqSetId;
      if (av != null)
      {

        af.viewport.historyList = av.historyList;
        af.viewport.redoList = av.redoList;
      }
      else
      {
        viewportsAdded.put(uniqueSeqSetId, af.viewport);
      }

      PaintRefresher.Register(af.alignPanel, uniqueSeqSetId);
    }
    if (hiddenSeqs != null)
    {
      for (int s = 0; s < JSEQ.length; s++)
      {
        jalview.datamodel.SequenceGroup hidden = new jalview.datamodel.SequenceGroup();

        for (int r = 0; r < JSEQ[s].getHiddenSequencesCount(); r++)
        {
          hidden.addSequence(al
                  .getSequenceAt(JSEQ[s].getHiddenSequences(r)), false);
        }
        af.viewport.hideRepSequences(al.getSequenceAt(s), hidden);
      }

      jalview.datamodel.SequenceI[] hseqs = new jalview.datamodel.SequenceI[hiddenSeqs
              .size()];

      for (int s = 0; s < hiddenSeqs.size(); s++)
      {
        hseqs[s] = (jalview.datamodel.SequenceI) hiddenSeqs.elementAt(s);
      }

      af.viewport.hideSequence(hseqs);

    }

    if ((hideConsensus || hideQuality || hideConservation)
            && al.getAlignmentAnnotation() != null)
    {
      int hSize = al.getAlignmentAnnotation().length;
      for (int h = 0; h < hSize; h++)
      {
        if ((hideConsensus && al.getAlignmentAnnotation()[h].label
                .equals("Consensus"))
                || (hideQuality && al.getAlignmentAnnotation()[h].label
                        .equals("Quality"))
                || (hideConservation && al.getAlignmentAnnotation()[h].label
                        .equals("Conservation")))
        {
          al.deleteAnnotation(al.getAlignmentAnnotation()[h]);
          hSize--;
          h--;
        }
      }
      af.alignPanel.adjustAnnotationHeight();
    }

    if (view.getViewName() != null)
    {
      af.viewport.viewName = view.getViewName();
      af.setInitialTabVisible();
    }
    af.setBounds(view.getXpos(), view.getYpos(), view.getWidth(), view
            .getHeight());

    af.viewport.setShowAnnotation(view.getShowAnnotation());
    af.viewport.setAbovePIDThreshold(view.getPidSelected());

    af.viewport.setColourText(view.getShowColourText());

    af.viewport.setConservationSelected(view.getConservationSelected());
    af.viewport.setShowJVSuffix(view.getShowFullId());
    af.viewport.rightAlignIds = view.getRightAlignIds();
    af.viewport.setFont(new java.awt.Font(view.getFontName(), view
            .getFontStyle(), view.getFontSize()));
    af.alignPanel.fontChanged();
    af.viewport.setRenderGaps(view.getRenderGaps());
    af.viewport.setWrapAlignment(view.getWrapAlignment());
    af.alignPanel.setWrapAlignment(view.getWrapAlignment());
    af.viewport.setShowAnnotation(view.getShowAnnotation());
    af.alignPanel.setAnnotationVisible(view.getShowAnnotation());

    af.viewport.setShowBoxes(view.getShowBoxes());

    af.viewport.setShowText(view.getShowText());

    af.viewport.textColour = new java.awt.Color(view.getTextCol1());
    af.viewport.textColour2 = new java.awt.Color(view.getTextCol2());
    af.viewport.thresholdTextColour = view.getTextColThreshold();

    af.viewport.setStartRes(view.getStartRes());
    af.viewport.setStartSeq(view.getStartSeq());

    ColourSchemeI cs = null;

    if (view.getBgColour() != null)
    {
      if (view.getBgColour().startsWith("ucs"))
      {
        cs = GetUserColourScheme(jms, view.getBgColour());
      }
      else if (view.getBgColour().startsWith("Annotation"))
      {
        // int find annotation
        for (int i = 0; i < af.viewport.alignment.getAlignmentAnnotation().length; i++)
        {
          if (af.viewport.alignment.getAlignmentAnnotation()[i].label
                  .equals(view.getAnnotationColours().getAnnotation()))
          {
            if (af.viewport.alignment.getAlignmentAnnotation()[i]
                    .getThreshold() == null)
            {
              af.viewport.alignment.getAlignmentAnnotation()[i]
                      .setThreshold(new jalview.datamodel.GraphLine(view
                              .getAnnotationColours().getThreshold(),
                              "Threshold", java.awt.Color.black)

                      );
            }

            if (view.getAnnotationColours().getColourScheme()
                    .equals("None"))
            {
              cs = new AnnotationColourGradient(af.viewport.alignment
                      .getAlignmentAnnotation()[i], new java.awt.Color(view
                      .getAnnotationColours().getMinColour()),
                      new java.awt.Color(view.getAnnotationColours()
                              .getMaxColour()), view.getAnnotationColours()
                              .getAboveThreshold());
            }
            else if (view.getAnnotationColours().getColourScheme()
                    .startsWith("ucs"))
            {
              cs = new AnnotationColourGradient(af.viewport.alignment
                      .getAlignmentAnnotation()[i], GetUserColourScheme(
                      jms, view.getAnnotationColours().getColourScheme()),
                      view.getAnnotationColours().getAboveThreshold());
            }
            else
            {
              cs = new AnnotationColourGradient(af.viewport.alignment
                      .getAlignmentAnnotation()[i], ColourSchemeProperty
                      .getColour(al, view.getAnnotationColours()
                              .getColourScheme()), view
                      .getAnnotationColours().getAboveThreshold());
            }

            // Also use these settings for all the groups
            if (al.getGroups() != null)
            {
              for (int g = 0; g < al.getGroups().size(); g++)
              {
                jalview.datamodel.SequenceGroup sg = (jalview.datamodel.SequenceGroup) al
                        .getGroups().elementAt(g);

                if (sg.cs == null)
                {
                  continue;
                }

                /*
                 * if
                 * (view.getAnnotationColours().getColourScheme().equals("None")) {
                 * sg.cs = new AnnotationColourGradient(
                 * af.viewport.alignment.getAlignmentAnnotation()[i], new
                 * java.awt.Color(view.getAnnotationColours(). getMinColour()),
                 * new java.awt.Color(view.getAnnotationColours().
                 * getMaxColour()),
                 * view.getAnnotationColours().getAboveThreshold()); } else
                 */
                {
                  sg.cs = new AnnotationColourGradient(
                          af.viewport.alignment.getAlignmentAnnotation()[i],
                          sg.cs, view.getAnnotationColours()
                                  .getAboveThreshold());
                }

              }
            }

            break;
          }

        }
      }
      else
      {
        cs = ColourSchemeProperty.getColour(al, view.getBgColour());
      }

      if (cs != null)
      {
        cs.setThreshold(view.getPidThreshold(), true);
        cs.setConsensus(af.viewport.hconsensus);
      }
    }

    af.viewport.setGlobalColourScheme(cs);
    af.viewport.setColourAppliesToAllGroups(false);

    if (view.getConservationSelected() && cs != null)
    {
      cs.setConservationInc(view.getConsThreshold());
    }

    af.changeColour(cs);

    af.viewport.setColourAppliesToAllGroups(true);

    if (view.getShowSequenceFeatures())
    {
      af.viewport.showSequenceFeatures = true;
    }

    if (jms.getFeatureSettings() != null)
    {
      af.viewport.featuresDisplayed = new Hashtable();
      String[] renderOrder = new String[jms.getFeatureSettings()
              .getSettingCount()];
      for (int fs = 0; fs < jms.getFeatureSettings().getSettingCount(); fs++)
      {
        Setting setting = jms.getFeatureSettings().getSetting(fs);

        af.alignPanel.seqPanel.seqCanvas.getFeatureRenderer().setColour(
                setting.getType(), new java.awt.Color(setting.getColour()));
        renderOrder[fs] = setting.getType();
        if (setting.hasOrder())
          af.alignPanel.seqPanel.seqCanvas.getFeatureRenderer().setOrder(
                  setting.getType(), setting.getOrder());
        else
          af.alignPanel.seqPanel.seqCanvas.getFeatureRenderer().setOrder(
                  setting.getType(),
                  fs / jms.getFeatureSettings().getSettingCount());
        if (setting.getDisplay())
        {
          af.viewport.featuresDisplayed.put(setting.getType(), new Integer(
                  setting.getColour()));
        }
      }
      af.alignPanel.seqPanel.seqCanvas.getFeatureRenderer().renderOrder = renderOrder;
      Hashtable fgtable;
      af.alignPanel.seqPanel.seqCanvas.getFeatureRenderer().featureGroups = fgtable = new Hashtable();
      for (int gs = 0; gs < jms.getFeatureSettings().getGroupCount(); gs++)
      {
        Group grp = jms.getFeatureSettings().getGroup(gs);
        fgtable.put(grp.getName(), new Boolean(grp.getDisplay()));
      }
    }

    if (view.getHiddenColumnsCount() > 0)
    {
      for (int c = 0; c < view.getHiddenColumnsCount(); c++)
      {
        af.viewport.hideColumns(view.getHiddenColumns(c).getStart(), view
                .getHiddenColumns(c).getEnd() // +1
                );
      }
    }

    af.setMenusFromViewport(af.viewport);

    Desktop.addInternalFrame(af, view.getTitle(), view.getWidth(), view
            .getHeight());

    // LOAD TREES
    // /////////////////////////////////////
    if (loadTreesAndStructures && jms.getTreeCount() > 0)
    {
      try
      {
        for (int t = 0; t < jms.getTreeCount(); t++)
        {

          Tree tree = jms.getTree(t);

          TreePanel tp = af.ShowNewickTree(new jalview.io.NewickFile(tree
                  .getNewick()), tree.getTitle(), tree.getWidth(), tree
                  .getHeight(), tree.getXpos(), tree.getYpos());

          tp.fitToWindow.setState(tree.getFitToWindow());
          tp.fitToWindow_actionPerformed(null);

          if (tree.getFontName() != null)
          {
            tp.setTreeFont(new java.awt.Font(tree.getFontName(), tree
                    .getFontStyle(), tree.getFontSize()));
          }
          else
          {
            tp.setTreeFont(new java.awt.Font(view.getFontName(), view
                    .getFontStyle(), tree.getFontSize()));
          }

          tp.showPlaceholders(tree.getMarkUnlinked());
          tp.showBootstrap(tree.getShowBootstrap());
          tp.showDistances(tree.getShowDistances());

          tp.treeCanvas.threshold = tree.getThreshold();

          if (tree.getCurrentTree())
          {
            af.viewport.setCurrentTree(tp.getTree());
          }
        }

      } catch (Exception ex)
      {
        ex.printStackTrace();
      }
    }

    // //LOAD STRUCTURES
    if (loadTreesAndStructures)
    {
      for (int i = 0; i < JSEQ.length; i++)
      {
        if (JSEQ[i].getPdbidsCount() > 0)
        {
          Pdbids[] ids = JSEQ[i].getPdbids();
          for (int p = 0; p < ids.length; p++)
          {
            for (int s = 0; s < ids[p].getStructureStateCount(); s++)
            {
              jalview.datamodel.PDBEntry jpdb = new jalview.datamodel.PDBEntry();

              jpdb.setFile(loadPDBFile(ids[p].getFile(), ids[p].getId()));
              jpdb.setId(ids[p].getId());

              int x = ids[p].getStructureState(s).getXpos();
              int y = ids[p].getStructureState(s).getYpos();
              int width = ids[p].getStructureState(s).getWidth();
              int height = ids[p].getStructureState(s).getHeight();

              java.awt.Component comp = null;

              JInternalFrame[] frames = null;
              do
              {
                try
                {
                  frames = Desktop.desktop.getAllFrames();
                } catch (ArrayIndexOutOfBoundsException e)
                {
                  // occasional No such child exceptions are thrown here...
                  frames = null;
                  try
                  {
                    Thread.sleep(10);
                  } catch (Exception f)
                  {
                  }
                  ;
                }
              } while (frames == null);
              for (int f = 0; f < frames.length; f++)
              {
                if (frames[f] instanceof AppJmol)
                {
                  if (frames[f].getX() == x && frames[f].getY() == y
                          && frames[f].getHeight() == height
                          && frames[f].getWidth() == width)
                  {
                    comp = frames[f];
                    break;
                  }
                }
              }

              Desktop.desktop.getComponentAt(x, y);

              String pdbFile = loadPDBFile(file, ids[p].getId());

              jalview.datamodel.SequenceI[] seq = new jalview.datamodel.SequenceI[]
              { (jalview.datamodel.SequenceI) seqRefIds.get(JSEQ[i].getId()
                      + "") };

              if (comp == null)
              {
                String state = ids[p].getStructureState(s).getContent();

                StringBuffer newFileLoc = new StringBuffer(state.substring(
                        0, state.indexOf("\"", state.indexOf("load")) + 1));

                newFileLoc.append(jpdb.getFile());
                newFileLoc.append(state.substring(state.indexOf("\"", state
                        .indexOf("load \"") + 6)));

                new AppJmol(pdbFile, ids[p].getId(), seq, af.alignPanel,
                        newFileLoc.toString(), new java.awt.Rectangle(x, y,
                                width, height));

              }
              else if (comp != null)
              {
                StructureSelectionManager.getStructureSelectionManager()
                        .setMapping(seq, null, pdbFile,
                                jalview.io.AppletFormatAdapter.FILE);

                ((AppJmol) comp).addSequence(seq);
              }
            }
          }
        }
      }
    }

    return af;
  }

  private void recoverDatasetFor(SequenceSet vamsasSet, Alignment al)
  {
    jalview.datamodel.Alignment ds = getDatasetFor(vamsasSet.getDatasetId());
    Vector dseqs = null;
    if (ds == null)
    {
      // create a list of new dataset sequences
      dseqs = new Vector();
    }
    for (int i = 0, iSize = vamsasSet.getSequenceCount(); i < iSize; i++)
    {
      Sequence vamsasSeq = vamsasSet.getSequence(i);
      ensureJalviewDatasetSequence(vamsasSeq, ds, dseqs);
    }
    // create a new dataset
    if (ds == null)
    {
      SequenceI[] dsseqs = new SequenceI[dseqs.size()];
      dseqs.copyInto(dsseqs);
      ds = new jalview.datamodel.Alignment(dsseqs);
      addDatasetRef(vamsasSet.getDatasetId(), ds);
    }
    // set the dataset for the newly imported alignment.
    if (al.getDataset() == null)
    {
      al.setDataset(ds);
    }
  }

  /**
   * 
   * @param vamsasSeq
   *                sequence definition to create/merge dataset sequence for
   * @param ds
   *                dataset alignment
   * @param dseqs
   *                vector to add new dataset sequence to
   */
  private void ensureJalviewDatasetSequence(Sequence vamsasSeq,
          AlignmentI ds, Vector dseqs)
  {
    // JBP TODO: Check this is called for AlCodonFrames to support recovery of
    // xRef Codon Maps
    jalview.datamodel.Sequence sq = (jalview.datamodel.Sequence) seqRefIds
            .get(vamsasSeq.getId());
    jalview.datamodel.SequenceI dsq = null;
    if (sq != null && sq.getDatasetSequence() != null)
    {
      dsq = (jalview.datamodel.SequenceI) sq.getDatasetSequence();
    }

    String sqid = vamsasSeq.getDsseqid();
    if (dsq == null)
    {
      // need to create or add a new dataset sequence reference to this sequence
      if (sqid != null)
      {
        dsq = (jalview.datamodel.SequenceI) seqRefIds.get(sqid);
      }
      // check again
      if (dsq == null)
      {
        // make a new dataset sequence
        dsq = sq.createDatasetSequence();
        if (sqid == null)
        {
          // make up a new dataset reference for this sequence
          sqid = seqHash(dsq);
        }
        dsq.setVamsasId(uniqueSetSuffix + sqid);
        seqRefIds.put(sqid, dsq);
        if (ds == null)
        {
          if (dseqs != null)
          {
            dseqs.addElement(dsq);
          }
        }
        else
        {
          ds.addSequence(dsq);
        }
      }
      else
      {
        if (sq != dsq)
        { // make this dataset sequence sq's dataset sequence
          sq.setDatasetSequence(dsq);
        }
      }
    }
    // TODO: refactor this as a merge dataset sequence function
    // now check that sq (the dataset sequence) sequence really is the union of
    // all references to it
    // boolean pre = sq.getStart() < dsq.getStart();
    // boolean post = sq.getEnd() > dsq.getEnd();
    // if (pre || post)
    if (sq != dsq)
    {
      StringBuffer sb = new StringBuffer();
      String newres = jalview.analysis.AlignSeq.extractGaps(
              jalview.util.Comparison.GapChars, sq.getSequenceAsString());
      if (!newres.equalsIgnoreCase(dsq.getSequenceAsString())
              && newres.length() > dsq.getLength())
      {
        // Update with the longer sequence.
        synchronized (dsq)
        {
          /*
           * if (pre) { sb.insert(0, newres .substring(0, dsq.getStart() -
           * sq.getStart())); dsq.setStart(sq.getStart()); } if (post) {
           * sb.append(newres.substring(newres.length() - sq.getEnd() -
           * dsq.getEnd())); dsq.setEnd(sq.getEnd()); }
           */
          dsq.setSequence(sb.toString());
        }
        // TODO: merges will never happen if we 'know' we have the real dataset
        // sequence - this should be detected when id==dssid
        System.err.println("DEBUG Notice:  Merged dataset sequence"); // ("
        // + (pre ? "prepended" : "") + " "
        // + (post ? "appended" : ""));
      }
    }
  }

  java.util.Hashtable datasetIds = null;

  private Alignment getDatasetFor(String datasetId)
  {
    if (datasetIds == null)
    {
      datasetIds = new Hashtable();
      return null;
    }
    if (datasetIds.containsKey(datasetId))
    {
      return (Alignment) datasetIds.get(datasetId);
    }
    return null;
  }

  private void addDatasetRef(String datasetId, Alignment dataset)
  {
    if (datasetIds == null)
    {
      datasetIds = new Hashtable();
    }
    datasetIds.put(datasetId, dataset);
  }

  private void addDBRefs(SequenceI datasetSequence, Sequence sequence)
  {
    for (int d = 0; d < sequence.getDBRefCount(); d++)
    {
      DBRef dr = sequence.getDBRef(d);
      jalview.datamodel.DBRefEntry entry = new jalview.datamodel.DBRefEntry(
              sequence.getDBRef(d).getSource(), sequence.getDBRef(d)
                      .getVersion(), sequence.getDBRef(d).getAccessionId());
      if (dr.getMapping() != null)
      {
        entry.setMap(addMapping(dr.getMapping()));
      }
      datasetSequence.addDBRef(entry);
    }
  }

  private jalview.datamodel.Mapping addMapping(Mapping m)
  {
    SequenceI dsto = null;
    // Mapping m = dr.getMapping();
    int fr[] = new int[m.getMapListFromCount() * 2];
    Enumeration f = m.enumerateMapListFrom();
    for (int _i = 0; f.hasMoreElements(); _i += 2)
    {
      MapListFrom mf = (MapListFrom) f.nextElement();
      fr[_i] = mf.getStart();
      fr[_i + 1] = mf.getEnd();
    }
    int fto[] = new int[m.getMapListToCount() * 2];
    f = m.enumerateMapListTo();
    for (int _i = 0; f.hasMoreElements(); _i += 2)
    {
      MapListTo mf = (MapListTo) f.nextElement();
      fto[_i] = mf.getStart();
      fto[_i + 1] = mf.getEnd();
    }
    jalview.datamodel.Mapping jmap = new jalview.datamodel.Mapping(dsto,
            fr, fto, (int) m.getMapFromUnit(), (int) m.getMapToUnit());
    if (m.getMappingChoice() != null)
    {
      MappingChoice mc = m.getMappingChoice();
      if (mc.getDseqFor() != null)
      {
        if (seqRefIds.containsKey(mc.getDseqFor()))
        {
          /**
           * recover from hash
           */
          jmap.setTo((SequenceI) seqRefIds.get(mc.getDseqFor()));
        }
        else
        {
          frefedSequence.add(new Object[]
          { mc.getDseqFor(), jmap });
        }
      }
      else
      {
        /**
         * local sequence definition
         */
        Sequence ms = mc.getSequence();
        jalview.datamodel.Sequence djs = null;
        String sqid = ms.getDsseqid();
        if (sqid != null && sqid.length() > 0)
        {
          /*
           * recover dataset sequence
           */
          djs = (jalview.datamodel.Sequence) seqRefIds.get(sqid);
        }
        else
        {
          System.err
                  .println("Warning - making up dataset sequence id for DbRef sequence map reference");
          sqid = ((Object) ms).toString(); // make up a new hascode for
                                            // undefined dataset sequence hash
                                            // (unlikely to happen)
        }

        if (djs == null)
        {
          /**
           * make a new dataset sequence and add it to refIds hash
           */
          djs = new jalview.datamodel.Sequence(ms.getName(), ms
                  .getSequence());
          djs.setStart(jmap.getMap().getToLowest());
          djs.setEnd(jmap.getMap().getToHighest());
          djs.setVamsasId(uniqueSetSuffix + sqid);
          jmap.setTo(djs);
          seqRefIds.put(sqid, djs);

        }
        jalview.bin.Cache.log.debug("about to recurse on addDBRefs.");
        addDBRefs(djs, ms);

      }
    }
    return (jmap);

  }

  public jalview.gui.AlignmentPanel copyAlignPanel(AlignmentPanel ap,
          boolean keepSeqRefs)
  {
    jalview.schemabinding.version2.JalviewModel jm = SaveState(ap, null,
            null);

    if (!keepSeqRefs)
    {
      clearSeqRefs();
      jm.getJalviewModelSequence().getViewport(0).setSequenceSetId(null);
    }
    else
    {
      uniqueSetSuffix = "";
    }

    viewportsAdded = new Hashtable();

    AlignFrame af = LoadFromObject(jm, null, false);
    af.alignPanels.clear();
    af.closeMenuItem_actionPerformed(true);

    /*
     * if(ap.av.alignment.getAlignmentAnnotation()!=null) { for(int i=0; i<ap.av.alignment.getAlignmentAnnotation().length;
     * i++) { if(!ap.av.alignment.getAlignmentAnnotation()[i].autoCalculated) {
     * af.alignPanel.av.alignment.getAlignmentAnnotation()[i] =
     * ap.av.alignment.getAlignmentAnnotation()[i]; } } }
     */

    return af.alignPanel;
  }

  /*
   * (non-Javadoc)
   * 
   * @see java.lang.Object#finalize()
   */
  protected void finalize() throws Throwable
  {
    // really make sure we have no buried refs left.
    clearSeqRefs();
    this.seqRefIds = null;
    this.seqsToIds = null;
    super.finalize();
  }

}
