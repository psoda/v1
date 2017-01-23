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
#include "LScoresInstr.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"


using namespace std;

LScoresInstr::LScoresInstr() : BuiltInCommand() {
  setDescription("Generate Likelihood scores for trees in the repository.");
  initDefaultValue("file", "", "File to save the likelihood scores to");
  return;
}

LScoresInstr::~LScoresInstr() {
  return;
}

void LScoresInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void LScoresInstr::execute(Environment* baseEnv) {
  Interpreter::getInstance()->lscores(baseEnv);
#ifdef GUI
      PsodaPrinter::getInstance()->write("## LScores Completed Successfully\n");
#endif
}

void LScoresInstr::addTree(int newTree) {
  treeList.push_back(newTree);
}

string LScoresInstr::treesString() const {
  stringstream returnStream;
  list<int>::const_iterator treeItr = treeList.begin();
  list<int>::const_iterator treeEnd = treeList.end();
  for (; treeItr != treeEnd; treeItr++) {
    int thisTree = *treeItr;
    returnStream << " ";
    if (thisTree == -1) returnStream << "all";
    else returnStream << thisTree;
  }
  if (treeList.size()) {
    returnStream << " /";
  }
  return returnStream.str();
}

string LScoresInstr::getName() const {
  return "lscores";
}
