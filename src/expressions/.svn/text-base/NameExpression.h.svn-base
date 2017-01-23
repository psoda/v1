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
#ifndef PSODA_NAME_EXPRESSION_H
#define PSODA_NAME_EXPRESSION_H

#include "Expression.h"
#include "IdentExpression.h"
#include "CallExpression.h"
#include "StringLiteral.h"
#include <string>
#include <typeinfo>

using namespace std;

/**
 * Represents an identifier (like a variable) or a string with out quotes (in that order)
 */
class NameExpression : public Expression {

 private:
  
  /**
   * The name of this nameifier
   */
  string name;
  
  /**
   * If the name is in the environment, evaluation is passed on to the identExpression
   */
  IdentExpression identExpression;

  /**
   * If the name is not in the environment the value is treated as a string
   */
  StringLiteral stringLit;

 public:

  /**
   * Constructor
   *
   * @param _name The name of the nameifier
   */
  NameExpression(string _name);

  /**
   * Destructor
   */
  virtual ~NameExpression();

  /**
   * Evaluates the Expression under the Interpreter's current env
   *
   * @return a LitExpr with the value of this identifier or the default return value
   * the function with this name
   */
  Literal* evaluate(Environment* curEnv);

  /**
   * Prints the expression to a string
   *
   * @return A string representation of the expression
   */
  virtual string toString() const;

  /**
   * Returns a reference to the class type for use in compares
   */
  static const type_info& getType();

};

#endif
