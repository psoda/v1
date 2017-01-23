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
package jalview.ws.dbsources;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import org.exolab.castor.xml.Unmarshaller;

import com.stevesoft.pat.Regex;

import jalview.datamodel.Alignment;
import jalview.datamodel.AlignmentI;
import jalview.datamodel.DBRefEntry;
import jalview.datamodel.DBRefSource;
import jalview.datamodel.PDBEntry;
import jalview.datamodel.SequenceFeature;
import jalview.datamodel.SequenceI;
import jalview.datamodel.UniprotEntry;
import jalview.datamodel.UniprotFile;
import jalview.io.FormatAdapter;
import jalview.io.IdentifyFile;
import jalview.ws.DBRefFetcher;
import jalview.ws.ebi.EBIFetchClient;
import jalview.ws.seqfetcher.DbSourceProxy;
import jalview.ws.seqfetcher.DbSourceProxyImpl;

/**
 * @author JimP
 * 
 */
public class Uniprot extends DbSourceProxyImpl implements DbSourceProxy
{
  public Uniprot()
  {
    super();
    addDbSourceProperty(DBRefSource.SEQDB, DBRefSource.SEQDB);
    addDbSourceProperty(DBRefSource.PROTSEQDB);
    // addDbSourceProperty(DBRefSource.MULTIACC, new Integer(50));
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getAccessionSeparator()
   */
  public String getAccessionSeparator()
  {
    return null; // ";";
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getAccessionValidator()
   */
  public Regex getAccessionValidator()
  {
    return null;
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getDbSource()
   */
  public String getDbSource()
  {
    return DBRefSource.UNIPROT;
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getDbVersion()
   */
  public String getDbVersion()
  {
    return "0"; // we really don't know what version we're on.
  }

  private EBIFetchClient ebi = null;

  public Vector getUniprotEntries(File file)
  {
    UniprotFile uni = new UniprotFile();
    try
    {
      // 1. Load the mapping information from the file
      org.exolab.castor.mapping.Mapping map = new org.exolab.castor.mapping.Mapping(
              uni.getClass().getClassLoader());
      java.net.URL url = getClass().getResource("/uniprot_mapping.xml");
      map.loadMapping(url);

      // 2. Unmarshal the data
      Unmarshaller unmar = new Unmarshaller(uni);
      unmar.setIgnoreExtraElements(true);
      unmar.setMapping(map);

      uni = (UniprotFile) unmar.unmarshal(new FileReader(file));
    } catch (Exception e)
    {
      System.out.println("Error getUniprotEntries() " + e);
    }

    return uni.getUniprotEntries();
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getSequenceRecords(java.lang.String[])
   */
  public AlignmentI getSequenceRecords(String queries) throws Exception
  {
    startQuery();
    try
    {
      Alignment al = null;
      ebi = new EBIFetchClient();
      StringBuffer result = new StringBuffer();
      // uniprotxml parameter required since december 2007
      File file = ebi.fetchDataAsFile("uniprot:" + queries, "uniprotxml",
              null);
      Vector entries = getUniprotEntries(file);

      if (entries != null)
      {
        // First, make the new sequences
        Enumeration en = entries.elements();
        while (en.hasMoreElements())
        {
          UniprotEntry entry = (UniprotEntry) en.nextElement();

          StringBuffer name = new StringBuffer(">UniProt/Swiss-Prot");
          Enumeration en2 = entry.getAccession().elements();
          while (en2.hasMoreElements())
          {
            name.append("|");
            name.append(en2.nextElement());
          }
          en2 = entry.getName().elements();
          while (en2.hasMoreElements())
          {
            name.append("|");
            name.append(en2.nextElement());
          }

          if (entry.getProtein() != null
                  && entry.getProtein().getName() != null)
          {
            for (int nm = 0, nmSize = entry.getProtein().getName().size(); nm < nmSize; nm++)
            {
              name.append(" " + entry.getProtein().getName().elementAt(nm));
            }
          }

          result.append(name + "\n"
                  + entry.getUniprotSequence().getContent() + "\n");

        }

        // Then read in the features and apply them to the dataset
        al = parseResult(result.toString());
        if (al != null)
        {
          // Decorate the alignment with database entries.
          addUniprotXrefs(al, entries);
        }
        else
        {
          results = result;
        }
      }
      stopQuery();
      return al;
    } catch (Exception e)
    {
      stopQuery();
      throw (e);
    }
  }

  /**
   * add an ordered set of UniprotEntry objects to an ordered set of seuqences.
   * 
   * @param al -
   *                a sequence of n sequences
   * @param entries
   *                a seuqence of n uniprot entries to be analysed.
   */
  public void addUniprotXrefs(Alignment al, Vector entries)
  {
    for (int i = 0; i < entries.size(); i++)
    {
      UniprotEntry entry = (UniprotEntry) entries.elementAt(i);
      Enumeration e = entry.getDbReference().elements();
      Vector onlyPdbEntries = new Vector();
      Vector dbxrefs = new Vector();
      while (e.hasMoreElements())
      {
        PDBEntry pdb = (PDBEntry) e.nextElement();
        DBRefEntry dbr = new DBRefEntry();
        dbr.setSource(pdb.getType());
        dbr.setAccessionId(pdb.getId());
        dbr.setVersion(DBRefSource.UNIPROT + ":" + getDbVersion());
        dbxrefs.addElement(dbr);
        if (!pdb.getType().equals("PDB"))
        {
          continue;
        }

        onlyPdbEntries.addElement(pdb);
      }
      SequenceI sq = al.getSequenceAt(i);
      while (sq.getDatasetSequence() != null)
      {
        sq = sq.getDatasetSequence();
      }

      Enumeration en2 = entry.getAccession().elements();
      while (en2.hasMoreElements())
      {
        // we always add as uniprot if we retrieved from uniprot or uniprot name
        sq.addDBRef(new DBRefEntry(DBRefSource.UNIPROT, getDbVersion(), en2
                .nextElement().toString()));
      }
      en2 = dbxrefs.elements();
      while (en2.hasMoreElements())
      {
        // we always add as uniprot if we retrieved from uniprot or uniprot name
        sq.addDBRef((DBRefEntry) en2.nextElement());

      }
      sq.setPDBId(onlyPdbEntries);
      if (entry.getFeature() != null)
      {
        e = entry.getFeature().elements();
        while (e.hasMoreElements())
        {
          SequenceFeature sf = (SequenceFeature) e.nextElement();
          sf.setFeatureGroup("Uniprot");
          sq.addSequenceFeature(sf);
        }
      }
    }
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#isValidReference(java.lang.String)
   */
  public boolean isValidReference(String accession)
  {
    return true;
  }

  /**
   * return LDHA_CHICK uniprot entry
   */
  public String getTestQuery()
  {
    return "P00340";
  }

  public String getDbName()
  {
    return "Uniprot"; // getDbSource();
  }
}
