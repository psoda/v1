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
 * Class Annotation.
 * 
 * @version $Revision$ $Date$
 */
public class Annotation implements java.io.Serializable
{

  // --------------------------/
  // - Class/Member Variables -/
  // --------------------------/

  /**
   * Field _graph.
   */
  private boolean _graph;

  /**
   * keeps track of state for field: _graph
   */
  private boolean _has_graph;

  /**
   * Field _graphType.
   */
  private int _graphType;

  /**
   * keeps track of state for field: _graphType
   */
  private boolean _has_graphType;

  /**
   * Field _sequenceRef.
   */
  private java.lang.String _sequenceRef;

  /**
   * Field _graphColour.
   */
  private int _graphColour;

  /**
   * keeps track of state for field: _graphColour
   */
  private boolean _has_graphColour;

  /**
   * Field _graphGroup.
   */
  private int _graphGroup;

  /**
   * keeps track of state for field: _graphGroup
   */
  private boolean _has_graphGroup;

  /**
   * Field _id.
   */
  private java.lang.String _id;

  /**
   * Field _scoreOnly.
   */
  private boolean _scoreOnly = false;

  /**
   * keeps track of state for field: _scoreOnly
   */
  private boolean _has_scoreOnly;

  /**
   * Field _score.
   */
  private double _score;

  /**
   * keeps track of state for field: _score
   */
  private boolean _has_score;

  /**
   * Field _visible.
   */
  private boolean _visible;

  /**
   * keeps track of state for field: _visible
   */
  private boolean _has_visible;

  /**
   * Field _annotationElementList.
   */
  private java.util.Vector _annotationElementList;

  /**
   * Field _label.
   */
  private java.lang.String _label;

  /**
   * Field _description.
   */
  private java.lang.String _description;

  /**
   * Field _thresholdLine.
   */
  private jalview.schemabinding.version2.ThresholdLine _thresholdLine;

  // ----------------/
  // - Constructors -/
  // ----------------/

  public Annotation()
  {
    super();
    this._annotationElementList = new java.util.Vector();
  }

  // -----------/
  // - Methods -/
  // -----------/

  /**
   * 
   * 
   * @param vAnnotationElement
   * @throws java.lang.IndexOutOfBoundsException
   *                 if the index given is outside the bounds of the collection
   */
  public void addAnnotationElement(
          final jalview.schemabinding.version2.AnnotationElement vAnnotationElement)
          throws java.lang.IndexOutOfBoundsException
  {
    this._annotationElementList.addElement(vAnnotationElement);
  }

  /**
   * 
   * 
   * @param index
   * @param vAnnotationElement
   * @throws java.lang.IndexOutOfBoundsException
   *                 if the index given is outside the bounds of the collection
   */
  public void addAnnotationElement(
          final int index,
          final jalview.schemabinding.version2.AnnotationElement vAnnotationElement)
          throws java.lang.IndexOutOfBoundsException
  {
    this._annotationElementList.add(index, vAnnotationElement);
  }

  /**
   */
  public void deleteGraph()
  {
    this._has_graph = false;
  }

  /**
   */
  public void deleteGraphColour()
  {
    this._has_graphColour = false;
  }

  /**
   */
  public void deleteGraphGroup()
  {
    this._has_graphGroup = false;
  }

  /**
   */
  public void deleteGraphType()
  {
    this._has_graphType = false;
  }

  /**
   */
  public void deleteScore()
  {
    this._has_score = false;
  }

  /**
   */
  public void deleteScoreOnly()
  {
    this._has_scoreOnly = false;
  }

  /**
   */
  public void deleteVisible()
  {
    this._has_visible = false;
  }

  /**
   * Method enumerateAnnotationElement.
   * 
   * @return an Enumeration over all
   *         jalview.schemabinding.version2.AnnotationElement elements
   */
  public java.util.Enumeration enumerateAnnotationElement()
  {
    return this._annotationElementList.elements();
  }

  /**
   * Method getAnnotationElement.
   * 
   * @param index
   * @throws java.lang.IndexOutOfBoundsException
   *                 if the index given is outside the bounds of the collection
   * @return the value of the jalview.schemabinding.version2.AnnotationElement
   *         at the given index
   */
  public jalview.schemabinding.version2.AnnotationElement getAnnotationElement(
          final int index) throws java.lang.IndexOutOfBoundsException
  {
    // check bounds for index
    if (index < 0 || index >= this._annotationElementList.size())
    {
      throw new IndexOutOfBoundsException(
              "getAnnotationElement: Index value '" + index
                      + "' not in range [0.."
                      + (this._annotationElementList.size() - 1) + "]");
    }

    return (jalview.schemabinding.version2.AnnotationElement) _annotationElementList
            .get(index);
  }

  /**
   * Method getAnnotationElement.Returns the contents of the collection in an
   * Array.
   * <p>
   * Note: Just in case the collection contents are changing in another thread,
   * we pass a 0-length Array of the correct type into the API call. This way we
   * <i>know</i> that the Array returned is of exactly the correct length.
   * 
   * @return this collection as an Array
   */
  public jalview.schemabinding.version2.AnnotationElement[] getAnnotationElement()
  {
    jalview.schemabinding.version2.AnnotationElement[] array = new jalview.schemabinding.version2.AnnotationElement[0];
    return (jalview.schemabinding.version2.AnnotationElement[]) this._annotationElementList
            .toArray(array);
  }

  /**
   * Method getAnnotationElementCount.
   * 
   * @return the size of this collection
   */
  public int getAnnotationElementCount()
  {
    return this._annotationElementList.size();
  }

  /**
   * Returns the value of field 'description'.
   * 
   * @return the value of field 'Description'.
   */
  public java.lang.String getDescription()
  {
    return this._description;
  }

  /**
   * Returns the value of field 'graph'.
   * 
   * @return the value of field 'Graph'.
   */
  public boolean getGraph()
  {
    return this._graph;
  }

  /**
   * Returns the value of field 'graphColour'.
   * 
   * @return the value of field 'GraphColour'.
   */
  public int getGraphColour()
  {
    return this._graphColour;
  }

  /**
   * Returns the value of field 'graphGroup'.
   * 
   * @return the value of field 'GraphGroup'.
   */
  public int getGraphGroup()
  {
    return this._graphGroup;
  }

  /**
   * Returns the value of field 'graphType'.
   * 
   * @return the value of field 'GraphType'.
   */
  public int getGraphType()
  {
    return this._graphType;
  }

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
   * Returns the value of field 'label'.
   * 
   * @return the value of field 'Label'.
   */
  public java.lang.String getLabel()
  {
    return this._label;
  }

  /**
   * Returns the value of field 'score'.
   * 
   * @return the value of field 'Score'.
   */
  public double getScore()
  {
    return this._score;
  }

  /**
   * Returns the value of field 'scoreOnly'.
   * 
   * @return the value of field 'ScoreOnly'.
   */
  public boolean getScoreOnly()
  {
    return this._scoreOnly;
  }

  /**
   * Returns the value of field 'sequenceRef'.
   * 
   * @return the value of field 'SequenceRef'.
   */
  public java.lang.String getSequenceRef()
  {
    return this._sequenceRef;
  }

  /**
   * Returns the value of field 'thresholdLine'.
   * 
   * @return the value of field 'ThresholdLine'.
   */
  public jalview.schemabinding.version2.ThresholdLine getThresholdLine()
  {
    return this._thresholdLine;
  }

  /**
   * Returns the value of field 'visible'.
   * 
   * @return the value of field 'Visible'.
   */
  public boolean getVisible()
  {
    return this._visible;
  }

  /**
   * Method hasGraph.
   * 
   * @return true if at least one Graph has been added
   */
  public boolean hasGraph()
  {
    return this._has_graph;
  }

  /**
   * Method hasGraphColour.
   * 
   * @return true if at least one GraphColour has been added
   */
  public boolean hasGraphColour()
  {
    return this._has_graphColour;
  }

  /**
   * Method hasGraphGroup.
   * 
   * @return true if at least one GraphGroup has been added
   */
  public boolean hasGraphGroup()
  {
    return this._has_graphGroup;
  }

  /**
   * Method hasGraphType.
   * 
   * @return true if at least one GraphType has been added
   */
  public boolean hasGraphType()
  {
    return this._has_graphType;
  }

  /**
   * Method hasScore.
   * 
   * @return true if at least one Score has been added
   */
  public boolean hasScore()
  {
    return this._has_score;
  }

  /**
   * Method hasScoreOnly.
   * 
   * @return true if at least one ScoreOnly has been added
   */
  public boolean hasScoreOnly()
  {
    return this._has_scoreOnly;
  }

  /**
   * Method hasVisible.
   * 
   * @return true if at least one Visible has been added
   */
  public boolean hasVisible()
  {
    return this._has_visible;
  }

  /**
   * Returns the value of field 'graph'.
   * 
   * @return the value of field 'Graph'.
   */
  public boolean isGraph()
  {
    return this._graph;
  }

  /**
   * Returns the value of field 'scoreOnly'.
   * 
   * @return the value of field 'ScoreOnly'.
   */
  public boolean isScoreOnly()
  {
    return this._scoreOnly;
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
   * Returns the value of field 'visible'.
   * 
   * @return the value of field 'Visible'.
   */
  public boolean isVisible()
  {
    return this._visible;
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
  public void removeAllAnnotationElement()
  {
    this._annotationElementList.clear();
  }

  /**
   * Method removeAnnotationElement.
   * 
   * @param vAnnotationElement
   * @return true if the object was removed from the collection.
   */
  public boolean removeAnnotationElement(
          final jalview.schemabinding.version2.AnnotationElement vAnnotationElement)
  {
    boolean removed = _annotationElementList.remove(vAnnotationElement);
    return removed;
  }

  /**
   * Method removeAnnotationElementAt.
   * 
   * @param index
   * @return the element removed from the collection
   */
  public jalview.schemabinding.version2.AnnotationElement removeAnnotationElementAt(
          final int index)
  {
    java.lang.Object obj = this._annotationElementList.remove(index);
    return (jalview.schemabinding.version2.AnnotationElement) obj;
  }

  /**
   * 
   * 
   * @param index
   * @param vAnnotationElement
   * @throws java.lang.IndexOutOfBoundsException
   *                 if the index given is outside the bounds of the collection
   */
  public void setAnnotationElement(
          final int index,
          final jalview.schemabinding.version2.AnnotationElement vAnnotationElement)
          throws java.lang.IndexOutOfBoundsException
  {
    // check bounds for index
    if (index < 0 || index >= this._annotationElementList.size())
    {
      throw new IndexOutOfBoundsException(
              "setAnnotationElement: Index value '" + index
                      + "' not in range [0.."
                      + (this._annotationElementList.size() - 1) + "]");
    }

    this._annotationElementList.set(index, vAnnotationElement);
  }

  /**
   * 
   * 
   * @param vAnnotationElementArray
   */
  public void setAnnotationElement(
          final jalview.schemabinding.version2.AnnotationElement[] vAnnotationElementArray)
  {
    // -- copy array
    _annotationElementList.clear();

    for (int i = 0; i < vAnnotationElementArray.length; i++)
    {
      this._annotationElementList.add(vAnnotationElementArray[i]);
    }
  }

  /**
   * Sets the value of field 'description'.
   * 
   * @param description
   *                the value of field 'description'.
   */
  public void setDescription(final java.lang.String description)
  {
    this._description = description;
  }

  /**
   * Sets the value of field 'graph'.
   * 
   * @param graph
   *                the value of field 'graph'.
   */
  public void setGraph(final boolean graph)
  {
    this._graph = graph;
    this._has_graph = true;
  }

  /**
   * Sets the value of field 'graphColour'.
   * 
   * @param graphColour
   *                the value of field 'graphColour'.
   */
  public void setGraphColour(final int graphColour)
  {
    this._graphColour = graphColour;
    this._has_graphColour = true;
  }

  /**
   * Sets the value of field 'graphGroup'.
   * 
   * @param graphGroup
   *                the value of field 'graphGroup'.
   */
  public void setGraphGroup(final int graphGroup)
  {
    this._graphGroup = graphGroup;
    this._has_graphGroup = true;
  }

  /**
   * Sets the value of field 'graphType'.
   * 
   * @param graphType
   *                the value of field 'graphType'.
   */
  public void setGraphType(final int graphType)
  {
    this._graphType = graphType;
    this._has_graphType = true;
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
   * Sets the value of field 'label'.
   * 
   * @param label
   *                the value of field 'label'.
   */
  public void setLabel(final java.lang.String label)
  {
    this._label = label;
  }

  /**
   * Sets the value of field 'score'.
   * 
   * @param score
   *                the value of field 'score'.
   */
  public void setScore(final double score)
  {
    this._score = score;
    this._has_score = true;
  }

  /**
   * Sets the value of field 'scoreOnly'.
   * 
   * @param scoreOnly
   *                the value of field 'scoreOnly'.
   */
  public void setScoreOnly(final boolean scoreOnly)
  {
    this._scoreOnly = scoreOnly;
    this._has_scoreOnly = true;
  }

  /**
   * Sets the value of field 'sequenceRef'.
   * 
   * @param sequenceRef
   *                the value of field 'sequenceRef'.
   */
  public void setSequenceRef(final java.lang.String sequenceRef)
  {
    this._sequenceRef = sequenceRef;
  }

  /**
   * Sets the value of field 'thresholdLine'.
   * 
   * @param thresholdLine
   *                the value of field 'thresholdLine'.
   */
  public void setThresholdLine(
          final jalview.schemabinding.version2.ThresholdLine thresholdLine)
  {
    this._thresholdLine = thresholdLine;
  }

  /**
   * Sets the value of field 'visible'.
   * 
   * @param visible
   *                the value of field 'visible'.
   */
  public void setVisible(final boolean visible)
  {
    this._visible = visible;
    this._has_visible = true;
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
   * @return the unmarshaled jalview.schemabinding.version2.Annotation
   */
  public static jalview.schemabinding.version2.Annotation unmarshal(
          final java.io.Reader reader)
          throws org.exolab.castor.xml.MarshalException,
          org.exolab.castor.xml.ValidationException
  {
    return (jalview.schemabinding.version2.Annotation) Unmarshaller
            .unmarshal(jalview.schemabinding.version2.Annotation.class,
                    reader);
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
