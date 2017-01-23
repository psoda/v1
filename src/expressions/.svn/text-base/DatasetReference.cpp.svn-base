/*
 * Copyright (C) <2009> <Quinn Snell, Mark Clement>
 *
 * This program is free software; you can redistribute it and/or modify it under the 
 * terms of the GNU General Public License as published by the Free Software Foundation; 
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program (gpl.txt); 
 * if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * More information can be obtained by accessing http://dna.cs.byu.edu/psoda
 */
#include "DatasetReference.h"
#include "InvalidCastException.h"
#include "Interpreter.h"
#include <sstream>

using namespace std;

#define DEFAULT_STRING_VALUE ""

DatasetReference::DatasetReference(string initialValue) : Literal(), value(initialValue) {
  Interpreter::getInstance()->getDatasetMap().incrementRefCount(initialValue);
}

DatasetReference::DatasetReference(const Literal& other) : Literal(), value(other.toString()) {
  Interpreter::getInstance()->getDatasetMap().incrementRefCount(value.toString());
}

Literal& DatasetReference::operator =(const Literal& rhs) {
  if (&rhs != this) {
    string newId = rhs.toString();
    Interpreter::getInstance()->getDatasetMap().incrementRefCount(newId);
    Interpreter::getInstance()->getDatasetMap().decrementRefCount(value.toString());
    value.setValue(newId);
  }
  return *this;
}

DatasetReference::~DatasetReference() {
  Interpreter::getInstance()->getDatasetMap().decrementRefCount(value.toString());
}
  
string DatasetReference::toString() const {
  return value.toString();
}

DatasetReference* DatasetReference::copyToHeap() const {
  return new DatasetReference(value.toString());
}

const type_info& DatasetReference::getType() {
  static DatasetReference datasetReference("");
  static const type_info& info = typeid(datasetReference);
  return info;
}
