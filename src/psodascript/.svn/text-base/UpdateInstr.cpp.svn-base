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
#include "UpdateInstr.h"
#include "Data.h"
#include "Interpreter.h"
#include "UndefinedVariableException.h"

using namespace std;

#define UPDATE_NAME_PARAM "*name"
#define UPDATE_VALUE_PARAM "*value"

UpdateInstr::UpdateInstr() : BuiltInCommand() {
  // Just so that we know that these parameters are used
  setDescription("Change the value of a variable.");
  initDefaultValue(UPDATE_NAME_PARAM, "", "The variable name that will be given a value");
  initDefaultValue(UPDATE_VALUE_PARAM, "", "The value that should be assigned to the variable");
}

UpdateInstr::~UpdateInstr() {
  return;
}

void UpdateInstr::checkParam(string name __attribute__ ((unused)) ) const {
  // don't do anything
  return; 
}

void UpdateInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
  execute(baseEnv);
}

void UpdateInstr::execute(Environment* baseEnv) {
  Environment* varEnv = getVariableEnv(baseEnv);
  string name = baseEnv->lookup(UPDATE_NAME_PARAM).toString();
  const Literal* value = (Literal*) &(baseEnv->lookup(UPDATE_VALUE_PARAM));
  try {
    varEnv->update(name, value);
  } catch (UndefinedVariableException& e) {
    varEnv->set(name, value);
  }
}

string UpdateInstr::getName() const {
  return "*update";
}

string UpdateInstr::toCode(const map<string, string>& params) const {
  string returnString = "";
  map<string, string>::const_iterator nameFoundItr = params.find(UPDATE_NAME_PARAM);
  map<string, string>::const_iterator valueFoundItr = params.find(UPDATE_VALUE_PARAM);
  if (nameFoundItr != params.end() && valueFoundItr != params.end()) {
    returnString += nameFoundItr->second;
    returnString += "=";
    returnString += valueFoundItr->second;
  }
  returnString += ";";
  return returnString;
}
