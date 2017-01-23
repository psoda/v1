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

#include "BoolLiteral.h"
using namespace std;

#define DEFAULT_BOOL_VALUE false

BoolLiteral::BoolLiteral() : Literal(), value(DEFAULT_BOOL_VALUE) {
  return;
}

BoolLiteral::BoolLiteral(bool initialValue) : Literal(), value(initialValue) {
  return;
}

BoolLiteral::BoolLiteral(const Literal& other) : Literal(), value(other.toBool()) {
  return;
}

Literal& BoolLiteral::operator =(const Literal& rhs) {
  if (&rhs != this) {
    value = rhs.toBool();
  }
  return *this;
}

BoolLiteral::~BoolLiteral() {
  return;
}

void BoolLiteral::setValue(bool newValue) {
  value = newValue;
}

bool BoolLiteral::toBool() const {
  return value;
}

int BoolLiteral::toInt() const {
  if (value) return 1;
  else return 0;
}

double BoolLiteral::toRealNumber() const {
  if (value) return 1.0;
  else return 0.0;
}

string BoolLiteral::toString() const {
  string returnString;
  if (value) {
    returnString = "true";
  } else {
    returnString = "false";
  }
  return returnString;
}

BoolLiteral* BoolLiteral::copyToHeap() const {
  return new BoolLiteral(value);
}
