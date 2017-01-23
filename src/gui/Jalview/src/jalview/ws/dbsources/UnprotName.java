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

/**
 * Canonical Uniprot fetcher instance specifically retrieving UP_NAME
 * references.
 * 
 * @author JimP
 * 
 */
public class UnprotName extends Uniprot implements
        jalview.ws.seqfetcher.DbSourceProxy
{

  /*
   * (non-Javadoc)
   * 
   * @see jalview.ws.dbsources.Uniprot#getDbSource()
   */
  public String getDbSource()
  {
    return jalview.datamodel.DBRefSource.UP_NAME;
  }

}
