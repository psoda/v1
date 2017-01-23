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
#include "CatOp.h"
#include <assert.h>

using namespace std;

CatOp::CatOp() : Operator() {
  return;
}

CatOp::~CatOp() {
  return;
}

StringLiteral& CatOp::operateOn(Environment* curEnv, Expression* operand1, Expression* operand2) {
  assert(operand1);
  assert(operand2);
  string intResult = operand1->evaluate(curEnv)->toString() + operand2->evaluate(curEnv)->toString();
  result.setValue(intResult);
  return result;
}

string CatOp::getOpSymbol() const {
  return ".";
}
