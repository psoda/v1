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
#include "SetInstr.h"
#include "Interpreter.h"

using namespace std;

#define SET_COMMAND_PARAM "*command"
#define SET_NAME_PARAM "*name"
#define SET_VALUE_PARAM "*value"
#define SET_FOR_ALL_COMMANDS "*all"

SetInstr::SetInstr() : BuiltInCommand() {
  // Just so that we know that these parameters are used
  setDescription("Set global parameters for instructions");
  initDefaultValue(SET_NAME_PARAM, "", "The name of the default parameter to be set");
  initDefaultValue(SET_VALUE_PARAM, "", "The value to which the parameter should be set");
  initDefaultValue(SET_COMMAND_PARAM, SET_FOR_ALL_COMMANDS, "The default value for the given parameter will be set on this command (by default, the set is done on all commands that have the given parameter)");
}

SetInstr::~SetInstr() {
  return;
}

void SetInstr::checkParam(string name __attribute__((unused)) ) const {
  // don't do anything
  return; 
}

void SetInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
  execute(baseEnv);
}

void SetInstr::execute(Environment* baseEnv) {
  //Environment* varEnv = getVariableEnv(baseEnv);
  string command = baseEnv->lookup(SET_COMMAND_PARAM).toString();
  string name = baseEnv->lookup(SET_NAME_PARAM).toString();
  Literal* value = (Literal*) &(baseEnv->lookup(SET_VALUE_PARAM));
  if (command == SET_FOR_ALL_COMMANDS) {
    Interpreter::getInstance()->doSet(name, value);
  } else {
    Interpreter::getInstance()->doSet(command, name, value);
  }
}

string SetInstr::getName() const {
  return "set";
}

string SetInstr::toCode(const map<string, string>& params) const {
  string returnString = getName();
  map<string, string>::const_iterator commandNameFoundItr = params.find(SET_COMMAND_PARAM);
  if (commandNameFoundItr != params.end() && commandNameFoundItr->second != SET_FOR_ALL_COMMANDS) {
    returnString += "{";
    returnString += commandNameFoundItr->second;
    returnString += "}";
  }
  returnString += " (";
  map<string, string>::const_iterator nameFoundItr = params.find(SET_NAME_PARAM);
  map<string, string>::const_iterator valueFoundItr = params.find(SET_VALUE_PARAM);
  if (nameFoundItr != params.end() && valueFoundItr != params.end()) {
    returnString += nameFoundItr->second;
    returnString += "=";
    returnString += valueFoundItr->second;
  }
  returnString += ");";
  return returnString;
}

