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
#include "ReturnInstr.h"
#include "IntLiteral.h"
#include "Environment.h"
#include "Interpreter.h"
#include "PsodaReturn.h"
#include "Data.h"
#include "Expression.h"
#include "UndefinedVariableException.h"
#include "UndefinedLiteral.h"

using namespace std;

ReturnInstr::ReturnInstr() : BuiltInCommand() {
  // Just so that we know that these parameters are used
  setDescription("Return from a function");
  initDefaultValue("*value", "");
}

ReturnInstr::~ReturnInstr() {
  return;
}

void ReturnInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void ReturnInstr::execute(Environment* baseEnv) {
  //Environment* varEnv = getVariableEnv(baseEnv);
  try {
    Literal* value = (Literal*) &(baseEnv->lookup("*value"));
    throw PsodaReturn(value->copyToHeap());
  } catch (UndefinedVariableException& e) {
    throw PsodaReturn(UndefinedLiteral::getInstance()->copyToHeap());
  }
}

string ReturnInstr::getName() const {
  return "return";
}
