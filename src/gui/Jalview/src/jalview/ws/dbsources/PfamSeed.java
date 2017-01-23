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

import jalview.ws.seqfetcher.DbSourceProxy;

/**
 * flyweight class specifying retrieval of Seed alignments from PFAM
 * 
 * @author JimP
 * 
 */
public class PfamSeed extends Pfam implements DbSourceProxy
{
  public PfamSeed()
  {
    super();
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.dbsources.Pfam#getPFAMURL()
   */
  protected String getPFAMURL()
  {
    return "http://pfam.sanger.ac.uk/family/alignment/download/format?alnType=seed&format=stockholm&order=t&case=l&gaps=default&entry=";
  }

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.seqfetcher.DbSourceProxy#getDbName()
   */
  public String getDbName()
  {
    return "PFAM (Seed)";
  }

  public String getDbSource()
  {
    return jalview.datamodel.DBRefSource.PFAM; // archetype source
  }

  public String getTestQuery()
  {
    return "PF03760";
  }

}
