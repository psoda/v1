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
package jalview.binding;

// ---------------------------------/
// - Imported classes and packages -/
// ---------------------------------/

import org.exolab.castor.xml.Marshaller;
import org.exolab.castor.xml.Unmarshaller;

/**
 * Class UserColours.
 * 
 * @version $Revision: 1.15.2.2 $ $Date: 2008/08/29 11:08:44 $
 */
public class UserColours implements java.io.Serializable
{

  // --------------------------/
  // - Class/Member Variables -/
  // --------------------------/

  /**
   * Field _id.
   */
  private java.lang.String _id;

  /**
   * Field _userColourScheme.
   */
  private jalview.binding.UserColourScheme _userColourScheme;

  // ----------------/
  // - Constructors -/
  // ----------------/

  public UserColours()
  {
    super();
  }

  // -----------/
  // - Methods -/
  // -----------/

  /**
   * Returns the value of field 'id'.
   * 
   * @return the value of field 'Id'.
   */
  public java.lang.String getId()
  {
    return this._id;
  }

  /**
   * Returns the value of field 'userColourScheme'.
   * 
   * @return the value of field 'UserColourScheme'.
   */
  public jalview.binding.UserColourScheme getUserColourScheme()
  {
    return this._userColourScheme;
  }

  /**
   * Method isValid.
   * 
   * @return true if this object is valid according to the schema
   */
  public boolean isValid()
  {
    try
    {
      validate();
    } catch (org.exolab.castor.xml.ValidationException vex)
    {
      return false;
    }
    return true;
  }

  /**
   * 
   * 
   * @param out
   * @throws org.exolab.castor.xml.MarshalException
   *                 if object is null or if any SAXException is thrown during
   *                 marshaling
   * @throws org.exolab.castor.xml.ValidationException
   *                 if this object is an invalid instance according to the
   *                 schema
   */
  public void marshal(final java.io.Writer out)
          throws org.exolab.castor.xml.MarshalException,
          org.exolab.castor.xml.ValidationException
  {
    Marshaller.marshal(this, out);
  }

  /**
   * 
   * 
   * @param handler
   * @throws java.io.IOException
   *                 if an IOException occurs during marshaling
   * @throws org.exolab.castor.xml.ValidationException
   *                 if this object is an invalid instance according to the
   *                 schema
   * @throws org.exolab.castor.xml.MarshalException
   *                 if object is null or if any SAXException is thrown during
   *                 marshaling
   */
  public void marshal(final org.xml.sax.ContentHandler handler)
          throws java.io.IOException,
          org.exolab.castor.xml.MarshalException,
          org.exolab.castor.xml.ValidationException
  {
    Marshaller.marshal(this, handler);
  }

  /**
   * Sets the value of field 'id'.
   * 
   * @param id
   *                the value of field 'id'.
   */
  public void setId(final java.lang.String id)
  {
    this._id = id;
  }

  /**
   * Sets the value of field 'userColourScheme'.
   * 
   * @param userColourScheme
   *                the value of field 'userColourScheme'
   */
  public void setUserColourScheme(
          final jalview.binding.UserColourScheme userColourScheme)
  {
    this._userColourScheme = userColourScheme;
  }

  /**
   * Method unmarshal.
   * 
   * @param reader
   * @throws org.exolab.castor.xml.MarshalException
   *                 if object is null or if any SAXException is thrown during
   *                 marshaling
   * @throws org.exolab.castor.xml.ValidationException
   *                 if this object is an invalid instance according to the
   *                 schema
   * @return the unmarshaled jalview.binding.UserColours
   */
  public static jalview.binding.UserColours unmarshal(
          final java.io.Reader reader)
          throws org.exolab.castor.xml.MarshalException,
          org.exolab.castor.xml.ValidationException
  {
    return (jalview.binding.UserColours) Unmarshaller.unmarshal(
            jalview.binding.UserColours.class, reader);
  }

  /**
   * 
   * 
   * @throws org.exolab.castor.xml.ValidationException
   *                 if this object is an invalid instance according to the
   *                 schema
   */
  public void validate() throws org.exolab.castor.xml.ValidationException
  {
    org.exolab.castor.xml.Validator validator = new org.exolab.castor.xml.Validator();
    validator.validate(this);
  }

}
