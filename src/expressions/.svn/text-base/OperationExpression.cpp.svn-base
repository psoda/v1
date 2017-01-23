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
#include "OperationExpression.h"
#include "UndefinedVariableException.h"
#include "InvalidCastException.h"
#include "InvalidParameterListException.h"
#include "PsodaBreak.h"
#include "PsodaReturn.h"
#include "PsodaWarning.h"
#include "PsodaReturn.h"
#include "PsodaError.h"
#include "UndefinedLiteral.h"
#include <iostream>
#include <assert.h>

using namespace std;

OperationExpression::OperationExpression() : Expression(), operand1(NULL), operand2(NULL), op(NULL) {
  return;
}

OperationExpression::~OperationExpression() {
  if (op != NULL) {
    delete op;
    op = NULL;
  }
  if (operand1 != NULL) {
    delete operand1;
    operand1 = NULL;
  }
  if (operand2 != NULL) {
    delete operand2;
    operand2 = NULL;
  }
}

Literal* OperationExpression::evaluate(Environment* curEnv) {
  assert(operand1);
  if (!op) {
    ADD_LOCATION_TO_EXCEPTIONS(return operand1->evaluate(curEnv);, getLocation());
  } else {
    ADD_LOCATION_TO_EXCEPTIONS(return &(op->operateOn(curEnv, operand1, operand2));, getLocation());
  }
}

void OperationExpression::setOperator(Operator* newOperator) {
  op = newOperator;
}

void OperationExpression::setOperand1(Expression* newOperand) {
  operand1 = newOperand;
}

void OperationExpression::setOperand2(Expression* newOperand) {
  operand2 = newOperand;
}

string OperationExpression::toString() const {
  string returnString = "";
  if (!op) {
    if (operand1) {
      returnString += operand1->toString();
    }
  } else {
    returnString += op->toString(operand1, operand2);
  }
  return returnString;
}
