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
#include "Literal.h"
#include "PsodaException.h"
#include <assert.h>

using namespace std;

Literal::Literal() {
  return;
}

#ifdef NODEF
Literal& Literal::operator =(const Literal& /*rhs*/) {
  //This should never be called;
  assert(0);
  throw PsodaException("Literal operator= Called");
  //return *this; //Function is noreturn
}
#endif

Literal::~Literal() {
  return;
}

string Literal::toCode() const {
  return toString();
}

bool Literal::toBool() const {
  throw PsodaException("Unsupport Type Conversion to Bool");
}

double Literal::toRealNumber() const {
  throw PsodaException("Unsupport Type Conversion to Real Number");
}

int Literal::toInt() const {
  throw PsodaException("Unsupport Type Conversion to Int");
}

string Literal::toString() const {
  throw PsodaException("Unsupport Type Conversion to String");
}

QTree* Literal::toTree() const {
  throw PsodaException("Unsupport Type Conversion to Tree");
}

void* Literal::getData() const {
  return NULL;
}
