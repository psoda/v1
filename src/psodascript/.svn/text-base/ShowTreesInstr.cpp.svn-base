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
#include "ShowTreesInstr.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"

using namespace std;

ShowTreesInstr::ShowTreesInstr() : BuiltInCommand() {
  setDescription("Print out the trees in the repository");
  return;
}

ShowTreesInstr::~ShowTreesInstr() {
  return;
}

void ShowTreesInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}


/**
 * Displays the trees in the current tree repository
 */
void ShowTreesInstr::execute(Environment* baseEnv) {
  assert(baseEnv);
  // Set parameter defaults in the ConstructAncestorStatesInstr constructor
  // (src/psodascript/ConstructAncestorStatesInstr.cpp)
  // Right now, the ConstructAncestorStatesInstr execute method simply calls this method
  // Put Construct Ancestor States Code Here ...
	QTree* tree;
	string displayType = baseEnv->lookupString("all");
	int i;
	
	if(displayType == "yes")
	{
		for(i=0;i<Interpreter::getInstance()->qtreeRepository()->numTrees();i++)
		{
		PsodaPrinter::getInstance()->write("Tree %d:\n",i+1);
		tree=Interpreter::getInstance()->qtreeRepository()->popTree();
		tree->showTree();
		Interpreter::getInstance()->qtreeRepository()->addTree(tree);
		}
	}
	else
	{
		tree = Interpreter::getInstance()->qtreeRepository()->getTree();
		if (!tree) return;
		PsodaPrinter::getInstance()->write("Tree 1:\n");
		tree->showTree();
	}
	
#ifdef GUI
        PsodaPrinter::getInstance()->write("## ShowTrees Completed Successfully\n");
#endif
	
  
}

string ShowTreesInstr::getName() const {
  return "showtrees";
}
