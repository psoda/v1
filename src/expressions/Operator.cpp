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
#include "Operator.h"

using namespace std;

Operator::Operator() : result(NULL) {
  return;
}

Operator::~Operator() {
  cleanResult();
}

void Operator::cleanResult() {
  if (result) {
    delete result;
    result = NULL;
  }
}

void Operator::setResult(Literal* newResult) {
  cleanResult();
  result = newResult;
}

Literal* Operator::getResult() {
  return result;
}

string Operator::toString(Expression* operand1, Expression* operand2) const {
  string returnString = "";
  if (operand1) {
    returnString += operand1->toString();
  }
  returnString += " ";
  returnString += getOpSymbol();
  returnString += " ";
  if (operand2) {
    returnString += operand2->toString();
  }
  return returnString;
}
