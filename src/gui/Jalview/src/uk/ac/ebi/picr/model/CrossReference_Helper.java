/**
 * CrossReference_Helper.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.2RC2 Nov 16, 2004 (12:19:44 EST) WSDL2Java emitter.
 */

package uk.ac.ebi.picr.model;

public class CrossReference_Helper
{
  // Type metadata
  private static org.apache.axis.description.TypeDesc typeDesc = new org.apache.axis.description.TypeDesc(
          CrossReference.class, true);

  static
  {
    typeDesc.setXmlType(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "CrossReference"));
    org.apache.axis.description.ElementDesc elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("accession");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "accession"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "string"));
    typeDesc.addFieldDesc(elemField);
    elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("accessionVersion");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "accessionVersion"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "string"));
    typeDesc.addFieldDesc(elemField);
    elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("databaseDescription");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "databaseDescription"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "string"));
    typeDesc.addFieldDesc(elemField);
    elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("databaseName");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "databaseName"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "string"));
    typeDesc.addFieldDesc(elemField);
    elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("dateAdded");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "dateAdded"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "dateTime"));
    typeDesc.addFieldDesc(elemField);
    elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("dateDeleted");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "dateDeleted"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "dateTime"));
    typeDesc.addFieldDesc(elemField);
    elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("deleted");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "deleted"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "boolean"));
    typeDesc.addFieldDesc(elemField);
    elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("gi");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "gi"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "string"));
    typeDesc.addFieldDesc(elemField);
    elemField = new org.apache.axis.description.ElementDesc();
    elemField.setFieldName("taxonId");
    elemField.setXmlName(new javax.xml.namespace.QName(
            "http://model.picr.ebi.ac.uk", "taxonId"));
    elemField.setXmlType(new javax.xml.namespace.QName(
            "http://www.w3.org/2001/XMLSchema", "string"));
    typeDesc.addFieldDesc(elemField);
  }

  /**
   * Return type metadata object
   */
  public static org.apache.axis.description.TypeDesc getTypeDesc()
  {
    return typeDesc;
  }

  /**
   * Get Custom Serializer
   */
  public static org.apache.axis.encoding.Serializer getSerializer(
          java.lang.String mechType, java.lang.Class _javaType,
          javax.xml.namespace.QName _xmlType)
  {
    return new org.apache.axis.encoding.ser.BeanSerializer(_javaType,
            _xmlType, typeDesc);
  }

  /**
   * Get Custom Deserializer
   */
  public static org.apache.axis.encoding.Deserializer getDeserializer(
          java.lang.String mechType, java.lang.Class _javaType,
          javax.xml.namespace.QName _xmlType)
  {
    return new org.apache.axis.encoding.ser.BeanDeserializer(_javaType,
            _xmlType, typeDesc);
  }

}
