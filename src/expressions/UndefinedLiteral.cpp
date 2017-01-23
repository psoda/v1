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
#include "UndefinedLiteral.h"
#include "UndefinedVariableException.h"

using namespace std;

UndefinedLiteral::UndefinedLiteral() : Literal() {
  return;
}

UndefinedLiteral::UndefinedLiteral(const Literal& other __attribute__((unused)) ) : Literal() {
/*TODO copy constructor not defined*/
  return;
}

Literal& UndefinedLiteral::operator=(const Literal& rhs __attribute__((unused)) ) {
/*TODO = operator not defined*/
  return *this;
}

UndefinedLiteral::~UndefinedLiteral() {
  return;
}

UndefinedLiteral* UndefinedLiteral::getInstance() {
  static UndefinedLiteral instance;
  return &instance;
}

bool UndefinedLiteral::toBool() const {
  return false;
}

int UndefinedLiteral::toInt() const {
  throw UndefinedVariableException();
}

string UndefinedLiteral::toString() const {
  throw UndefinedVariableException();
}

string UndefinedLiteral::toCode() const {
  return "undef";
}

UndefinedLiteral* UndefinedLiteral::copyToHeap() const {
  return new UndefinedLiteral();
}

const type_info& UndefinedLiteral::getType() {
  static UndefinedLiteral undefinedLit;
  static const type_info& info = typeid(undefinedLit);
  return info;
}
