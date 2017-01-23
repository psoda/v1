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
import java.util.Hashtable;
import java.util.Iterator;
import java.util.StringTokenizer;

import com.stevesoft.pat.Regex;

import jalview.datamodel.Alignment;
import jalview.datamodel.AlignmentI;
import jalview.datamodel.DBRefSource;
import jalview.datamodel.SequenceI;
import jalview.datamodel.xdb.embl.EmblEntry;
import jalview.ws.ebi.EBIFetchClient;
import jalview.ws.seqfetcher.DbSourceProxy;
import jalview.ws.seqfetcher.DbSourceProxyImpl;

/**
 * @author JimP
 * 
 */
public class EmblSource extends EmblXmlSource implements DbSourceProxy
{

  public EmblSource()
  {
    addDbSourceProperty(DBRefSource.DNASEQDB);
    addDbSourceProperty(DBRefSource.CODINGSEQDB);
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getAccessionSeparator()
   */
  public String getAccessionSeparator()
  {
    // TODO Auto-generated method stub
    return null;
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getAccessionValidator()
   */
  public Regex getAccessionValidator()
  {
    // TODO Auto-generated method stub
    return null;
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getDbSource()
   */
  public String getDbSource()
  {
    return DBRefSource.EMBL;
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getDbVersion()
   */
  public String getDbVersion()
  {
    // TODO Auto-generated method stub
    return "0";
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#getSequenceRecords(java.lang.String[])
   */
  public AlignmentI getSequenceRecords(String queries) throws Exception
  {
    return getEmblSequenceRecords(DBRefSource.EMBL, queries);
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.DbSourceProxy#isValidReference(java.lang.String)
   */
  public boolean isValidReference(String accession)
  {
    // TODO Auto-generated method stub
    return false;
  }

  /**
   * return LHD_CHICK coding gene
   */
  public String getTestQuery()
  {
    return "X53828";
  }

  public String getDbName()
  {
    return "EMBL"; // getDbSource();
  }
}
