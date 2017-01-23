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
#include "UseDataInstr.h"
#include "Interpreter.h"

using namespace std;

UseDataInstr::UseDataInstr() : BuiltInCommand() {
  setDescription("Uses a dataset from the interpreter\'s dataset map.");
  initDefaultValue("name", "", "The name of the dataset to use");
}

UseDataInstr::~UseDataInstr() {
  return;
}

void UseDataInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) ) {
  execute(baseEnv);
}

void UseDataInstr::execute(Environment* baseEnv) {
  string dataName = baseEnv->lookup("name").toString();
  if (dataName != "") {
    Interpreter::getInstance()->getDatasetMap().setCurrent(dataName);
  }
}

string UseDataInstr::getName() const {
  return "UseData";
}
