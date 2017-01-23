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
#include "DeclareVarInstr.h"
#include "Data.h"
#include "Interpreter.h"
#include "UndefinedVariableException.h"
#include "VariableRedeclarationException.h"

using namespace std;

DeclareVarInstr::DeclareVarInstr() : BuiltInCommand() {
  // Just so that we know that these parameters are used
  setDescription("Declare a variable\n");
  initDefaultValue("*name", "");
  initDefaultValue("*value", "");
}

DeclareVarInstr::~DeclareVarInstr() {
  return;
}

void DeclareVarInstr::checkParam(string name __attribute__ ((unused)) ) const {
  // don't do anything
  return; 
}

void DeclareVarInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
  execute(baseEnv);
}

void DeclareVarInstr::execute(Environment* baseEnv) {
  Environment* varEnv = getVariableEnv(baseEnv);
  string name = baseEnv->lookup("*name").toString();
  Literal* value = (Literal*) &(baseEnv->lookup("*value"));
  if (varEnv->isShallow(name)) {
    varEnv->set(name, value);
    ostringstream message;
    message << "Variable \"" << name << "\" was already defined";
    throw VariableRedeclarationException(message.str());
  } else {
    varEnv->set(name, value);
  }
}

string DeclareVarInstr::getName() const {
  return "*var";
}
