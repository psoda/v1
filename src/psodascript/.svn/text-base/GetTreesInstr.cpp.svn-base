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
#include "GetTreesInstr.h"
#include "PsodaPrinter.h"
#include "Interpreter.h"

using namespace std;

GetTreesInstr::GetTreesInstr() : BuiltInCommand() {
  setDescription("Get trees from a file and put them in the tree repository");
  initFileDefaultValue("file", "", "The file from which the trees should be read");
}

GetTreesInstr::~GetTreesInstr() {
  return;
}

void GetTreesInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
  execute(baseEnv);
}

void GetTreesInstr::execute(Environment* baseEnv) {
  Interpreter::getInstance()->getTrees(baseEnv);
#ifdef GUI
      PsodaPrinter::getInstance()->write("## GetTrees Completed Successfully\n");
#endif
}

string GetTreesInstr::getName() const {
  return "gettrees";
}
