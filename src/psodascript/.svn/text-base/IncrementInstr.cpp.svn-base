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
#include "IncrementInstr.h"
#include "IntLiteral.h"
#include "Environment.h"
#include "Interpreter.h"
#include "UndefinedVariableException.h"

using namespace std;

IncrementInstr::IncrementInstr() : BuiltInCommand() {
  // Just so that we know that this parameter is used
  setDescription("Increment a value.");
  initDefaultValue("*name", "");
}

IncrementInstr::~IncrementInstr() {
  return;
}

int IncrementInstr::incrementVar(Environment* baseEnv, string name) {
  Environment* varEnv = getVariableEnv(baseEnv);
  Literal& lit = varEnv->lookup(name);
  IntLiteral newValue(lit.toInt() + 1);
  try {
    varEnv->update(name, &newValue);
  } catch (UndefinedVariableException& e) {
    varEnv->set(name, &newValue);
  }
  return newValue.toInt();
}

void IncrementInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) ) {
  execute(baseEnv);
}

void IncrementInstr::execute(Environment* baseEnv) {
  IncrementInstr::incrementVar(baseEnv, baseEnv->lookup("*name").toString());
}

string IncrementInstr::getName() const {
  return "*increment";
}
