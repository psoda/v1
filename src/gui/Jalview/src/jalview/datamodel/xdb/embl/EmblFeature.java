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
package jalview.datamodel.xdb.embl;

import java.util.Hashtable;
import java.util.Vector;

public class EmblFeature
{
  String name;

  Vector dbRefs;

  Vector qualifiers;

  Vector locations;

  /**
   * @return the dbRefs
   */
  public Vector getDbRefs()
  {
    return dbRefs;
  }

  /**
   * @param dbRefs
   *                the dbRefs to set
   */
  public void setDbRefs(Vector dbRefs)
  {
    this.dbRefs = dbRefs;
  }

  /**
   * @return the locations
   */
  public Vector getLocations()
  {
    return locations;
  }

  /**
   * @param locations
   *                the locations to set
   */
  public void setLocations(Vector locations)
  {
    this.locations = locations;
  }

  /**
   * @return the name
   */
  public String getName()
  {
    return name;
  }

  /**
   * @param name
   *                the name to set
   */
  public void setName(String name)
  {
    this.name = name;
  }

  /**
   * @return the qualifiers
   */
  public Vector getQualifiers()
  {
    return qualifiers;
  }

  /**
   * @param qualifiers
   *                the qualifiers to set
   */
  public void setQualifiers(Vector qualifiers)
  {
    this.qualifiers = qualifiers;
  }
}
