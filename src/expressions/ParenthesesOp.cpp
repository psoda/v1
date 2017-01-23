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
#include "ParenthesesOp.h"
#include <assert.h>

using namespace std;

ParenthesesOp::ParenthesesOp() : Operator() {
  return;
}

ParenthesesOp::~ParenthesesOp() {
  return;
}

Literal& ParenthesesOp::operateOn(Environment* curEnv, Expression* operand1, Expression* operand2 __attribute__((unused)) ) {
  assert(operand1);
  return *(operand1->evaluate(curEnv));
}

string ParenthesesOp::getOpSymbol() const {
  return "()";
}

string ParenthesesOp::toString(Expression* operand1, Expression* operand2 __attribute__((unused))) const {
  string returnString = "";
  returnString += "(";
  if (operand1) {
    returnString += operand1->toString();
  }
  returnString += ")";
  return returnString;
}
