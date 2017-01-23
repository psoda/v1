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
package jalview.schemabinding.version2;

// ---------------------------------/
// - Imported classes and packages -/
// ---------------------------------/

import org.exolab.castor.xml.Marshaller;
import org.exolab.castor.xml.Unmarshaller;

/**
 * Class Mapping.
 * 
 * @version $Revision$ $Date$
 */
public class Mapping extends jalview.schemabinding.version2.MapListType
        implements java.io.Serializable
{

  // --------------------------/
  // - Class/Member Variables -/
  // --------------------------/

  /**
   * Internal choice value storage
   */
  private java.lang.Object _choiceValue;

  /**
   * Field _mappingChoice.
   */
  private jalview.schemabinding.version2.MappingChoice _mappingChoice;

  // ----------------/
  // - Constructors -/
  // ----------------/

  public Mapping()
  {
    super();
  }

  // -----------/
  // - Methods -/
  // -----------/

  /**
   * Returns the value of field 'choiceValue'. The field 'choiceValue' has the
   * following description: Internal choice value storage
   * 
   * @return the value of field 'ChoiceValue'.
   */
  public java.lang.Object getChoiceValue()
  {
    return this._choiceValue;
  }

  /**
   * Returns the value of field 'mappingChoice'.
   * 
   * @return the value of field 'MappingChoice'.
   */
  public jalview.schemabinding.version2.MappingChoice getMappingChoice()
  {
    return this._mappingChoice;
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
   * Sets the value of field 'mappingChoice'.
   * 
   * @param mappingChoice
   *                the value of field 'mappingChoice'.
   */
  public void setMappingChoice(
          final jalview.schemabinding.version2.MappingChoice mappingChoice)
  {
    this._mappingChoice = mappingChoice;
    this._choiceValue = mappingChoice;
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
   * @return the unmarshaled jalview.schemabinding.version2.MapListType
   */
  public static jalview.schemabinding.version2.MapListType unmarshal(
          final java.io.Reader reader)
          throws org.exolab.castor.xml.MarshalException,
          org.exolab.castor.xml.ValidationException
  {
    return (jalview.schemabinding.version2.MapListType) Unmarshaller
            .unmarshal(jalview.schemabinding.version2.Mapping.class, reader);
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
