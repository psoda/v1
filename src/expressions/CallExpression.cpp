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
#include "CallExpression.h"
#include "UndefinedLiteral.h"
#include "Environment.h"
#include "Interpreter.h"
#include "PsodaException.h"
#include <iostream>
#include <assert.h>

using namespace std;

CallExpression::CallExpression() : Expression(), commandNode(NULL) {
  return;
}

CallExpression::~CallExpression() {
  if (commandNode) {
    delete commandNode;
    commandNode = NULL;
  }
}

Literal* CallExpression::evaluate(Environment* curEnv) {
  Literal* result = NULL;
  commandNode->execute(curEnv, result);

  if (!result) {
    result = UndefinedLiteral::getInstance();
  }

  return result;
}

void CallExpression::validate() {
  if (commandNode) {
    commandNode->validate();
  }
}

void CallExpression::setCommandNode(CommandNode* newCommandNode) {
  if (commandNode) {
    delete commandNode;
    commandNode = NULL;
  }
  commandNode = newCommandNode;
}

string CallExpression::toString() const {
  string returnString = "";
  if (commandNode) {
    returnString += commandNode->toString();
  }
  return returnString;
}
