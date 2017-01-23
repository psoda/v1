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
#include "DivisionOp.h"
#include "RealNumberLiteral.h"
#include "IntLiteral.h"
#include <assert.h>
#include <typeinfo>

using namespace std;

DivisionOp::DivisionOp() : Operator() {
  return;
}

DivisionOp::~DivisionOp() {
  return;
}

Literal& DivisionOp::operateOn(Environment* curEnv, Expression* operand1, Expression* operand2) {
  assert(operand1);
  assert(operand2);
  Literal* operand1Result = operand1->evaluate(curEnv);
  Literal* operand2Result = operand2->evaluate(curEnv);
  if (typeid(*operand1Result) == RealNumberLiteral::getType() ||
      typeid(*operand2Result) == RealNumberLiteral::getType()) {
    setResult(new RealNumberLiteral(operand1Result->toRealNumber() / operand2Result->toRealNumber()));
  } else {
    setResult(new IntLiteral(operand1Result->toInt() / operand2Result->toInt()));
  }
  return *getResult();
}

string DivisionOp::getOpSymbol() const {
  return "/";
}
