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
#include "StringLiteral.h"
#include "InvalidCastException.h"
#include <sstream>
#include <ctype.h>

using namespace std;

#define DEFAULT_STRING_VALUE ""

StringLiteral::StringLiteral() : Literal(), value(DEFAULT_STRING_VALUE) {
  //do nothing
}

StringLiteral::StringLiteral(string initialValue) : Literal(), value(initialValue) {
  //do nothing
}

StringLiteral::StringLiteral(const Literal& other) : Literal(), value(other.toString()) {
  //do nothing
}

Literal& StringLiteral::operator =(const Literal& rhs) {
  if (&rhs != this) {
    value = rhs.toString();
  }
  return *this;
}

StringLiteral& StringLiteral::operator =(const string& rhs) {
  value = rhs;
  return *this;
}

StringLiteral::~StringLiteral() {
  //do nothing
}
  
void StringLiteral::setValue(string newValue) {
  value = newValue;
}

bool StringLiteral::isValidNum(string value) {
  int length = value.length();
  bool hasSeenDecimalPoint = false;
  int i = 0;
  if (value[i] == '-') {
    i++;
  }
  for (; i < length; i++) {
    // check for decimal point
    if (value[i] == '.') {
      if (hasSeenDecimalPoint) {
	// don't accept it if there are multiple decimal points
	return false;
      } else {
	hasSeenDecimalPoint = true;
      }
    } else if (!isdigit(value[i])) {
      // don't accept it if there are non digit/decimal point characters
      return false;
    }
  }
  return true;
}

bool StringLiteral::toBool() const {
  // return false is emptystring
  return (bool)value.compare("");
}

string StringLiteral::toCode() const {
  string returnString = "\"";
  for (unsigned int i = 0; i < value.length(); i++) {
    switch (value[i]) {
    case '\n':
      returnString += "\\n";
      break;
    case '\t':
      returnString += "\\t";
      break;
    case '\"':
      returnString += "\\\"";
      break;
    case '\'':
      returnString += "\\\'";
      break;
    case '\r':
      returnString += "\\r";
      break;
    default:
      returnString += value[i];
      break;
    }
  }
  returnString += "\"";
  return returnString;
}

double StringLiteral::toRealNumber() const {

  if (!StringLiteral::isValidNum(value)) {
    ostringstream message;
    message << "The String \"" << value << "\" does not hold a valid double value";
    throw InvalidCastException(message.str());
  }

  //it passed, so return the integer
  return atof(value.c_str());
}

int StringLiteral::toInt() const {

  if (!StringLiteral::isValidNum(value)) {
    ostringstream message;
    message << "The String \"" << value << "\" does not hold a valid integer value";
    throw InvalidCastException(message.str());
  }

  //it passed, so return the integer
  return atoi(value.c_str());
}

string StringLiteral::toString() const {
  return value;
}

string StringLiteral::toLowerCase() {
  return StringLiteral::toLowerCase(value);
}

StringLiteral* StringLiteral::copyToHeap() const {
  return new StringLiteral(value);
}

string StringLiteral::toLowerCase(const string& inString) {
  int length = inString.size();
  string returnString = inString;
  for (int i = 0; i < length; i++) {
    if (isupper(returnString[i])) {
      returnString[i] = tolower(returnString[i]);
    }
  }
  return returnString;
}

string StringLiteral::replaceNewLines(const string& inString) {
  int length = inString.size();
  string returnString = inString;
  for (int i = 0; i < length; i++) {
    if (returnString[i] == '\n') {
      returnString[i] = ' ';
    }
  }
  for (int i = length - 1; i >= 0; i--) {
    if (isspace(returnString[i])) length--;
    else break;
  }
  return returnString.substr(0, length);
}

const type_info& StringLiteral::getType() {
  static StringLiteral stringLit;
  static const type_info& info = typeid(stringLit);
  return info;
}
