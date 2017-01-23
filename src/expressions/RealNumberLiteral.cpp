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
#include "RealNumberLiteral.h"
#include <sstream>
#include <math.h>
#include <float.h>

using namespace std;

#define DEFAULT_REAL_NUMBER_VALUE 0.0

RealNumberLiteral::RealNumberLiteral() : Literal(), value(DEFAULT_REAL_NUMBER_VALUE) {
  return;
}

RealNumberLiteral::RealNumberLiteral(double initialValue) : Literal(), value(initialValue) {
  return;
}

RealNumberLiteral::RealNumberLiteral(const Literal& other) : Literal(), value(other.toRealNumber()) {
  return;
}

Literal& RealNumberLiteral::operator =(const Literal& rhs) {
  if (&rhs != this) {
    value = rhs.toRealNumber();
  }
  return *this;
}

RealNumberLiteral::~RealNumberLiteral() {
  return;
}

void RealNumberLiteral::setValue(double newValue) {
  value = newValue;
}

bool RealNumberLiteral::toBool() const { /*Close to 0 = false, non-zero = true*/
  if (fabs(value) < 0.001) return false;
  return true; 
}

int RealNumberLiteral::toInt() const {
  return (int) value;
}

double RealNumberLiteral::toRealNumber() const {
  return value;
}

string RealNumberLiteral::toString() const {
  ostringstream returnString;
  returnString.str("");
  returnString << value;
  return returnString.str();
}

RealNumberLiteral* RealNumberLiteral::copyToHeap() const {
  return new RealNumberLiteral(value);
}

const type_info& RealNumberLiteral::getType() {
  static RealNumberLiteral realNumber;
  static const type_info& info = typeid(realNumber);
  return info;
}

const RealNumberLiteral& RealNumberLiteral::getMin() {
  static RealNumberLiteral minLiteral(DBL_MAX);
  return minLiteral;
}

const RealNumberLiteral& RealNumberLiteral::getMax() {
  static RealNumberLiteral maxLiteral(DBL_MAX);
  return maxLiteral;
}

