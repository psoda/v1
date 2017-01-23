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
package jalview.datamodel;

/**
 * A set of feature property constants used by jalview
 * 
 * @author JimP
 * 
 */
public class FeatureProperties
{

  public static final String EXONPOS = "exon number";

  public static final String EXONPRODUCT = "product";

  /**
   * lookup feature type for a particular database to see if its a coding region
   * feature
   * 
   * @param dbrefsource
   * @param string
   * @return
   */
  public static boolean isCodingFeature(String dbrefsource, String type)
  {
    return ((dbrefsource == null
            || dbrefsource.equalsIgnoreCase(DBRefSource.EMBL) || dbrefsource
            .equalsIgnoreCase(DBRefSource.EMBLCDS)) && type
            .equalsIgnoreCase("CDS"));
  }
}
