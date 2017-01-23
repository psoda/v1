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

import java.util.Hashtable;

import com.stevesoft.pat.Regex;

import jalview.datamodel.AlignmentI;
import jalview.datamodel.DBRefSource;
import jalview.ws.seqfetcher.DbSourceProxy;

public class EmblCdsSouce extends EmblXmlSource implements DbSourceProxy
{

  public EmblCdsSouce()
  {
    super();
    addDbSourceProperty(DBRefSource.CODINGSEQDB);
  }

  public String getAccessionSeparator()
  {
    return null;
  }

  public Regex getAccessionValidator()
  {
    // TODO Auto-generated method stub
    return null;
  }

  public String getDbSource()
  {
    return DBRefSource.EMBLCDS;
  }

  public String getDbVersion()
  {
    return "0"; // TODO : this is dynamically set for a returned record - not
                // tied to proxy
  }

  public AlignmentI getSequenceRecords(String queries) throws Exception
  {
    if (queries.indexOf(".") > -1)
    {
      queries = queries.substring(0, queries.indexOf("."));
    }
    return getEmblSequenceRecords(DBRefSource.EMBLCDS, queries);
  }

  public boolean isValidReference(String accession)
  {
    // TODO Auto-generated method stub
    return true;
  }

  /**
   * cDNA for LDHA_CHICK swissprot sequence
   */
  public String getTestQuery()
  {
    return "CAA37824";
  }

  public String getDbName()
  {
    return "EMBL (CDS)";
  }

}
