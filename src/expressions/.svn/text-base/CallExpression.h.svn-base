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
#ifndef PSODA_CALL_EXPRESSION_H
#define PSODA_CALL_EXPRESSION_H

#include "Expression.h"
#include "Operator.h"
#include "Literal.h"
#include "CommandNode.h"
#include <vector>
#include <string>

using namespace std;

class CallExpression : public Expression {
  
 private:

  /**
   * The instruction to be used
   */
  CommandNode* commandNode;

 public:

  /**
   * Constructor
   */
  CallExpression();

  /**
   * Destructor
   */
  virtual ~CallExpression();

  /**
   * Evaluates the Expression under the Interpreter's current env
   *
   * @return a Literal with the evaluated value
   */
  Literal* evaluate(Environment* curEnv);

  /**
   * Performs possible validation in self and children
   */
  virtual void validate();

  /**
   * Sets instruction to be executed
   *
   * @param newInstruction The instruction to be executed
   */
  void setCommandNode(CommandNode* newCommandNode);

  /**
   * Prints the expression to a string
   *
   * @return A string representation of the expression
   */
  virtual string toString() const;

};

#endif
