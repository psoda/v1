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
#include "PostIncrOp.h"
#include "IncrementInstr.h"
#include "IntLiteral.h"
#include <assert.h>

using namespace std;

PostIncrOp::PostIncrOp() : Operator() {
  return;
}

PostIncrOp::~PostIncrOp() {
  return;
}

Literal& PostIncrOp::operateOn(Environment* curEnv, Expression* operand1, Expression* operand2 __attribute__((unused)) ) {
  assert(operand1);
  setResult(new IntLiteral(operand1->evaluate(curEnv)->toInt()));
  IncrementInstr::incrementVar(curEnv, operand1->toString());
  return *getResult();
}

string PostIncrOp::getOpSymbol() const {
  return "++";
}

string PostIncrOp::toString(Expression* operand1, Expression* operand2 __attribute__((unused)) ) const {
  string returnString = "";
  if (operand1) {
    returnString += operand1->toString();
  }
  returnString += getOpSymbol();
  return returnString;
}
