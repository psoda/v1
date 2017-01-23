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
#include "NameExpression.h"
#include "Interpreter.h"
#include "Environment.h"
#include "UndefinedLiteral.h"
#include "UndefinedVariableException.h"
#include <assert.h>

using namespace std;

NameExpression::NameExpression(string _name) : Expression() {
  name = StringLiteral::toLowerCase(_name);
  identExpression.setName(StringLiteral::toLowerCase(_name));
  stringLit.setValue(StringLiteral::toLowerCase(_name));
}

NameExpression::~NameExpression() {
  return;
}

Literal* NameExpression::evaluate(Environment* curEnv) {
  Literal* returnVal = NULL;
  try {
    returnVal = identExpression.evaluate(curEnv);
    //    if (typeid(*returnVal) == UndefinedLiteral::getType()) throw UndefinedVariableException();
    return returnVal;
  } catch (UndefinedVariableException e) {
    return &stringLit;
  }
  return returnVal;
}

string NameExpression::toString() const {
  return name;
}

const type_info& NameExpression::getType() {
  static NameExpression nameExpression("");
  static const type_info& info = typeid(nameExpression);
  return info;
}
