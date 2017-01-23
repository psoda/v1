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
 * Class JalviewUserColours.
 * 
 * @version $Revision: 1.11.2.2 $ $Date: 2008/08/29 11:08:45 $
 */
public class JalviewUserColours implements java.io.Serializable
{

  // --------------------------/
  // - Class/Member Variables -/
  // --------------------------/

  /**
   * Field _schemeName.
   */
  private java.lang.String _schemeName;

  /**
   * Field _colourList.
   */
  private java.util.Vector _colourList;

  // ----------------/
  // - Constructors -/
  // ----------------/

  public JalviewUserColours()
  {
    super();
    this._colourList = new java.util.Vector();
  }

  // -----------/
  // - Methods -/
  // -----------/

  /**
   * 
   * 
   * @param vColour
   * @throws java.lang.IndexOutOfBoundsException
   *                 if the index given is outside the bounds of the collection
   */
  public void addColour(final Colour vColour)
          throws java.lang.IndexOutOfBoundsException
  {
    this._colourList.addElement(vColour);
  }

  /**
   * 
   * 
   * @param index
   * @param vColour
   * @throws java.lang.IndexOutOfBoundsException
   *                 if the index given is outside the bounds of the collection
   */
  public void addColour(final int index, final Colour vColour)
          throws java.lang.IndexOutOfBoundsException
  {
    this._colourList.add(index, vColour);
  }

  /**
   * Method enumerateColour.
   * 
   * @return an Enumeration over all Colour elements
   */
  public java.util.Enumeration enumerateColour()
  {
    return this._colourList.elements();
  }

  /**
   * Method getColour.
   * 
   * @param index
   * @throws java.lang.IndexOutOfBoundsException
   *                 if the index given is outside the bounds of the collection
   * @return the value of the Colour at the given index
   */
  public Colour getColour(final int index)
          throws java.lang.IndexOutOfBoundsException
  {
    // check bounds for index
    if (index < 0 || index >= this._colourList.size())
    {
      throw new IndexOutOfBoundsException("getColour: Index value '"
              + index + "' not in range [0.."
              + (this._colourList.size() - 1) + "]");
    }

    return (Colour) _colourList.get(index);
  }

  /**
   * Method getColour.Returns the contents of the collection in an Array.
   * <p>
   * Note: Just in case the collection contents are changing in another thread,
   * we pass a 0-length Array of the correct type into the API call. This way we
   * <i>know</i> that the Array returned is of exactly the correct length.
   * 
   * @return this collection as an Array
   */
  public Colour[] getColour()
  {
    Colour[] array = new Colour[0];
    return (Colour[]) this._colourList.toArray(array);
  }

  /**
   * Method getColourCount.
   * 
   * @return the size of this collection
   */
  public int getColourCount()
  {
    return this._colourList.size();
  }

  /**
   * Returns the value of field 'schemeName'.
   * 
   * @return the value of field 'SchemeName'.
   */
  public java.lang.String getSchemeName()
  {
    return this._schemeName;
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
   */
  public void removeAllColour()
  {
    this._colourList.clear();
  }

  /**
   * Method removeColour.
   * 
   * @param vColour
   * @return true if the object was removed from the collection.
   */
  public boolean removeColour(final Colour vColour)
  {
    boolean removed = _colourList.remove(vColour);
    return removed;
  }

  /**
   * Method removeColourAt.
   * 
   * @param index
   * @return the element removed from the collection
   */
  public Colour removeColourAt(final int index)
  {
    java.lang.Object obj = this._colourList.remove(index);
    return (Colour) obj;
  }

  /**
   * 
   * 
   * @param index
   * @param vColour
   * @throws java.lang.IndexOutOfBoundsException
   *                 if the index given is outside the bounds of the collection
   */
  public void setColour(final int index, final Colour vColour)
          throws java.lang.IndexOutOfBoundsException
  {
    // check bounds for index
    if (index < 0 || index >= this._colourList.size())
    {
      throw new IndexOutOfBoundsException("setColour: Index value '"
              + index + "' not in range [0.."
              + (this._colourList.size() - 1) + "]");
    }

    this._colourList.set(index, vColour);
  }

  /**
   * 
   * 
   * @param vColourArray
   */
  public void setColour(final Colour[] vColourArray)
  {
    // -- copy array
    _colourList.clear();

    for (int i = 0; i < vColourArray.length; i++)
    {
      this._colourList.add(vColourArray[i]);
    }
  }

  /**
   * Sets the value of field 'schemeName'.
   * 
   * @param schemeName
   *                the value of field 'schemeName'.
   */
  public void setSchemeName(final java.lang.String schemeName)
  {
    this._schemeName = schemeName;
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
   * @return the unmarshaled jalview.binding.JalviewUserColours
   */
  public static jalview.binding.JalviewUserColours unmarshal(
          final java.io.Reader reader)
          throws org.exolab.castor.xml.MarshalException,
          org.exolab.castor.xml.ValidationException
  {
    return (jalview.binding.JalviewUserColours) Unmarshaller.unmarshal(
            jalview.binding.JalviewUserColours.class, reader);
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
