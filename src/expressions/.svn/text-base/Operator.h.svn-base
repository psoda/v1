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
#ifndef PSODA_OPERATOR_H
#define PSODA_OPERATOR_H

#include "Expression.h"
#include "Literal.h"

using namespace std;

/**
 * Represents an identifier (like a variable)
 */
class Operator {

 private:

  /**
   * Hold the last result of using the operator
   */
  Literal* result;

 protected:

  /**
   * Deletes the result Literal if there is one
   */
  void cleanResult();

  /**
   * Deletes the old result if there was one and puts the newResult in its place
   */
  void setResult(Literal* newResult);
  
  /**
   * Returns the internal result
   */
  Literal* getResult();

 public:

  /**
   * Default Constructor
   */
  Operator();

  /**
   * Destructor
   */
  virtual ~Operator();

  /**
   * Uses this operator to operate on the vector of expressions under the
   *  passed in environment
   *
   * @param operand1, operand2 The operands on which to operate
   * (operand2 will be ignored when passed to a unary operator)
   * @param env The environment underwhich to evaluate the expression
   * @return a LitExpr with the value of this identifier
   */
  virtual Literal& operateOn(Environment* curEnv, Expression* operand1, Expression* operand2) = 0;

  /**
   * Prints the operator to a string
   *
   * @return A string representation of the operator
   */
  virtual string getOpSymbol() const = 0;

  /**
   * Prints a representation of the operator operating on the given operands to a string
   *
   * @return A string representation of the operator operating on the given operands
   */
  virtual string toString(Expression* operand1, Expression* operand2) const;

};

#endif
