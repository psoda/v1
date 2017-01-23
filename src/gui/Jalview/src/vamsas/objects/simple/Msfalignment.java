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
package vamsas.objects.simple;

public class Msfalignment implements java.io.Serializable
{
  private java.lang.String msf;

  private java.lang.String notes;

  public Msfalignment()
  {
  }

  public Msfalignment(java.lang.String msf, java.lang.String notes)
  {
    this.msf = msf;
    this.notes = notes;
  }

  /**
   * Gets the msf value for this Msfalignment.
   * 
   * @return msf
   */
  public java.lang.String getMsf()
  {
    return msf;
  }

  /**
   * Sets the msf value for this Msfalignment.
   * 
   * @param msf
   */
  public void setMsf(java.lang.String msf)
  {
    this.msf = msf;
  }

  /**
   * Gets the notes value for this Msfalignment.
   * 
   * @return notes
   */
  public java.lang.String getNotes()
  {
    return notes;
  }

  /**
   * Sets the notes value for this Msfalignment.
   * 
   * @param notes
   */
  public void setNotes(java.lang.String notes)
  {
    this.notes = notes;
  }

  private java.lang.Object __equalsCalc = null;

  public synchronized boolean equals(java.lang.Object obj)
  {
    if (!(obj instanceof Msfalignment))
    {
      return false;
    }
    Msfalignment other = (Msfalignment) obj;
    if (obj == null)
    {
      return false;
    }
    if (this == obj)
    {
      return true;
    }
    if (__equalsCalc != null)
    {
      return (__equalsCalc == obj);
    }
    __equalsCalc = obj;
    boolean _equals;
    _equals = true
            && ((this.msf == null && other.getMsf() == null) || (this.msf != null && this.msf
                    .equals(other.getMsf())))
            && ((this.notes == null && other.getNotes() == null) || (this.notes != null && this.notes
                    .equals(other.getNotes())));
    __equalsCalc = null;
    return _equals;
  }

  private boolean __hashCodeCalc = false;

  public synchronized int hashCode()
  {
    if (__hashCodeCalc)
    {
      return 0;
    }
    __hashCodeCalc = true;
    int _hashCode = 1;
    if (getMsf() != null)
    {
      _hashCode += getMsf().hashCode();
    }
    if (getNotes() != null)
    {
      _hashCode += getNotes().hashCode();
    }
    __hashCodeCalc = false;
    return _hashCode;
  }

}
