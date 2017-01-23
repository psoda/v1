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
#include "UserDefinedCommand.h"
#include "PsodaReturn.h"

using namespace std;

UserDefinedCommand::UserDefinedCommand() : PsodaCommand(), LocatableObject(), Codeable(), name("undef"), body() {
  return;
}

UserDefinedCommand::~UserDefinedCommand() {
  return;
}

void UserDefinedCommand::setName(string newCommandName) {
  name = newCommandName;
}

string UserDefinedCommand::getName() const {
  return name;
}

PsodaProgram& UserDefinedCommand::getBody() {
  return body;
}

void UserDefinedCommand::execute(Environment* baseEnv) {
  Literal* returnVal;
  execute(baseEnv, returnVal);
}

void UserDefinedCommand::execute(Environment* baseEnv, Literal*& returnVal) {
  returnVal = NULL;
  try {
    body.execute(baseEnv);
  } catch (PsodaReturn r) {
    returnVal = r.getReturnVal();
  }
}

string UserDefinedCommand::toString() const {
  return toString(0);
}

string UserDefinedCommand::toString(int depth, string preText) const {
  string tabs = getTabs(depth);
  string spaces = getSpaces(preText);
  string returnString = preText + tabs;
  returnString += getBegin();
  returnString += " ";
  returnString += PsodaCommand::toString();
  returnString += "\n";
  returnString += body.toString(depth);
  returnString += spaces + tabs;
  returnString += "end;\n";
  return returnString;
}

string UserDefinedCommand::getBegin() const {
  return "begin";
}

