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
#ifndef PSODA_IDENT_EXPRESSION_H
#define PSODA_IDENT_EXPRESSION_H

#include "Expression.h"
#include "Literal.h"
#include <string>

using namespace std;

/**
 * Represents an identifier (like a variable)
 */
class IdentExpression : public Expression {

 private:
  
  /**
   * The name of this identifier
   */
  string name;
  
 public:

  /**
   * Default Constructor
   */
  IdentExpression();

  /**
   * Constructor
   *
   * @param _name The name of the identifier
   */
  IdentExpression(string _name);

  /**
   * Destructor
   */
  virtual ~IdentExpression();

  /**
   * Sets the name of this identifier
   *
   * @param _name The name of this identifier
   */
  void setName(string _name);

  /**
   * Evaluates the Expression under the Interpreter's current env
   *
   * @return a LitExpr with the value of this identifier
   */
  Literal* evaluate(Environment* curEnv);

  /**
   * Prints the expression to a string
   *
   * @return A string representation of the expression
   */
  virtual string toString() const;

};

#endif
