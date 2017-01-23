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
#include "InlineUserDefinedCommand.h"
#include "PsodaReturn.h"
#include "ReadOnlyEnvironment.h"
#include "Environment.h"

using namespace std;

InlineUserDefinedCommand::InlineUserDefinedCommand() : UserDefinedCommand() {
  setDescription("Execute a user defined command.");
  return;
}

InlineUserDefinedCommand::~InlineUserDefinedCommand() {
  return;
}

void InlineUserDefinedCommand::execute(Environment* baseEnv) {
  Literal* returnVal;
  execute(baseEnv, returnVal);
}

void InlineUserDefinedCommand::execute(Environment* baseEnv, Literal*& returnVal) {
  // First we will copy all baseEnv variables into the new environment
  Environment newEnv;
  newEnv.setAllReferences(baseEnv);

  // Now we will create a new read only environment and extend it with the variable environment from outside the command call (this allows the inline command to see but not change the values of defined variables).

  ReadOnlyEnvironment readOnlyEnv;
  Environment* varEnv = getVariableEnv(baseEnv);
  readOnlyEnv.extendEnv(varEnv);

  // Now we will extend the new enviornment with the read only variable environment
  newEnv.extendEnv(&readOnlyEnv);

  // Now execute the user defined command with the new environment
  UserDefinedCommand::execute(&newEnv, returnVal);
}

string InlineUserDefinedCommand::getBegin() const {
  return "begin*";
}
