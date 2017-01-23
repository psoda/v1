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
#ifndef PSODA_POST_DECR_OP_H
#define PSODA_POST_DECR_OP_H

#include "Operator.h"

using namespace std;

/**
 * Represents a negation operator
 */
class PostDecrOp : public Operator {

 public:

  /**
   * Default Constructor
   */
  PostDecrOp();

  /**
   * Destructor
   */
  virtual ~PostDecrOp();

  /**
   * Returns the post decremented value of operand1 (operand2 is ignored)
   *
   * @param operand1, operand2 The operands on which to operate
   * @return an IntLiteral with the value of the negation of operand1
   */
  Literal& operateOn(Environment* curEnv, Expression* operand1, Expression* operand2);

  /**
   * Prints the operator to a string
   *
   * @return A string representation of the operator
   */
  virtual string getOpSymbol() const;

  /**
   * Prints a representation of the operator operating on the given operands to a string
   *
   * @return A string representation of the operator operating on the given operands
   */
  virtual string toString(Expression* operand1, Expression* operand2) const;

};

#endif
