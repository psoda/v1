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
#ifndef PSODA_OPERATION_EXPRESSION_H
#define PSODA_OPERATION_EXPRESSION_H

#include "Expression.h"
#include "Operator.h"
#include "Literal.h"
#include <string>

using namespace std;

class OperationExpression : public Expression {

 private:

  /**
   * The operands for this expression
   */
  Expression* operand1;
  Expression* operand2;

  /**
   * The operator for this expression
   */
  Operator* op;

 public:

  /**
   * Constructor
   */
  OperationExpression();

  /**
   * Destructor
   */
  virtual ~OperationExpression();

  /**
   * Evaluates the Expression under the Interpreter's current env
   *
   * @return a Literal with the evaluated value
   */
  Literal* evaluate(Environment* curEnv);

  /**
   * Sets the operator
   *
   * @param operator The new operator
   */
  void setOperator(Operator* newOp);

  /**
   * Sets operand1
   *
   * @param newOperand The new operand1
   */
  void setOperand1(Expression* newOperand);

  /**
   * Sets operand2
   *
   * @param newOperand The new operand2
   */
  void setOperand2(Expression* newOperand);

  /**
   * Prints the expression to a string
   *
   * @return A string representation of the expression
   */
  virtual string toString() const;

};

#endif
