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
#include "PrintInstr.h"
#include "PsodaPrinter.h"
#include "PsodaException.h"
#include <typeinfo>

using namespace std;

#define PRINT_TEXT_PARAM "*text"

PrintInstr::PrintInstr() : BuiltInCommand() {
  setDescription("Evaluates the given expression and prints the string value to the console (note: the usage for this command differs from the typical name=value paradigm");
  initDefaultValue(PRINT_TEXT_PARAM, "\n", "any valid PSODA expression");
}

PrintInstr::~PrintInstr() {
  return;
}

void PrintInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
  execute(baseEnv);
}

void PrintInstr::execute(Environment* baseEnv) {
  PsodaPrinter::getInstance()->write("%s", baseEnv->lookup(PRINT_TEXT_PARAM).toString().c_str());
}

string PrintInstr::getName() const {
  return "print";
}

string PrintInstr::toCode(const map<string, string>& params) const {
  string returnString = getName();
  returnString += " (";
  map<string, string>::const_iterator textFoundItr = params.find(PRINT_TEXT_PARAM);
  if (textFoundItr != params.end()) {
    returnString += textFoundItr->second;
  }
  returnString += ");";
  return returnString;
}

string PrintInstr::getUsage() const {
  string usage = getName() + " ";
  usage += PRINT_TEXT_PARAM;
  usage += ";";
  return usage;
}
