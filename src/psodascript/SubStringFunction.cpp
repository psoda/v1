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
#include "SubStringFunction.h"
#include "InvalidParameterListException.h"
#include "StringLiteral.h"

using namespace std;

SubStringFunction::SubStringFunction() : BuiltInCommand() {
  setDescription("Find a substring");
  initDefaultValue("string", "", "The string to take from");
  initDefaultValue("first", 0, "The index of the first character to take");
  initDefaultValue("length", "", "The resulting substring will be no longer than this length, but it may be shorter if the end of the input string is reached.  Passing an empty string as this parameter will result in going to the end of the string.");
}

SubStringFunction::~SubStringFunction() {
  return;
}

void SubStringFunction::execute(Environment* baseEnv)
{
  Literal* temp = NULL;
  execute(baseEnv,temp);
}

void SubStringFunction::execute(Environment* baseEnv, Literal*& returnVal) {
  string initialString = baseEnv->lookup("string").toString();
  int firstIndex = baseEnv->lookup("first").toInt();
  Literal& subStringLengthLiteral = baseEnv->lookup("length");
  string subStringLengthString = subStringLengthLiteral.toString();
  int subStringLength;
  if (subStringLengthString == "") {
    subStringLength = initialString.length() - firstIndex;
  } else {
    subStringLength = subStringLengthLiteral.toInt();
  }
  if (subStringLength < 0) subStringLength = 0;
  setResult(new StringLiteral(initialString.substr(firstIndex, subStringLength)));
  returnVal = getResult();
}

string SubStringFunction::getName() const {
  return "SubString";
}
