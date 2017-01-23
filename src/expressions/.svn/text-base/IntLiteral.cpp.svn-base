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
#include "IntLiteral.h"
#include <sstream>

using namespace std;

#define DEFAULT_INT_VALUE 0

IntLiteral::IntLiteral() : Literal(), value(DEFAULT_INT_VALUE) {
  return;
}

IntLiteral::IntLiteral(int initialValue) : Literal(), value(initialValue) {
  return;
}

IntLiteral::IntLiteral(const Literal& other) : Literal(), value(other.toInt()) {
  return;
}

Literal& IntLiteral::operator =(const Literal& rhs) {
  if (&rhs != this) {
    value = rhs.toInt();
  }
  return *this;
}

IntLiteral::~IntLiteral() {
  return;
}

void IntLiteral::setValue(int newValue) {
  value = newValue;
}

bool IntLiteral::toBool() const {
  return (bool)value;
}

int IntLiteral::toInt() const {
  return value;
}

double IntLiteral::toRealNumber() const {
  return value;
}

string IntLiteral::toString() const {
  ostringstream returnString;
  returnString.str("");
  returnString << value;
  return returnString.str();
}

IntLiteral* IntLiteral::copyToHeap() const {
  return new IntLiteral(value);
}

const IntLiteral& IntLiteral::getMin() {
  static IntLiteral minLiteral(INT_MIN);
  return minLiteral;
}

const IntLiteral& IntLiteral::getMax() {
  static IntLiteral maxLiteral(INT_MAX);
  return maxLiteral;
}
