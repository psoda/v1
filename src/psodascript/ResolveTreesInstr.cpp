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
#include "ResolveTreesInstr.h"
#include "ResolutionTree.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"
using namespace std;

ResolveTreesInstr::ResolveTreesInstr() : BuiltInCommand() {
  setDescription("Given an unresolved tree, resolve it in all possible ways and put the trees in the repository.");
/*
  initDefaultValue("start", "stepwise", "Where should the starting tree for the HSearch come from");
  initDefaultValue("criterion", "parsimony");
  initDefaultValue("nreps", 1);
  initDefaultValue("swap", "TBR", "What method should be used for swaping");
  initDefaultValue("wantRecursion", "FALSE");
  initDefaultValue("maxTrees", 1000);
  initDefaultValue("iterations", INT_MAX);
*/
}

ResolveTreesInstr::~ResolveTreesInstr() {
  return;
}

void ResolveTreesInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);	
}

void ResolveTreesInstr::execute(Environment* baseEnv __attribute__((unused))) 
{
QTreeRepository* trees = Interpreter::getInstance()->qtreeRepository();
//iterate through repository
//Go through the QTreeRepository
	deque<QTree *> tree_list = *(trees->getTrees());
	deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list
	for(treeIt = tree_list.begin(); treeIt != tree_list.end(); treeIt++)
	{
		ResolutionTree* rt = new ResolutionTree(*treeIt);
		QTree* qt = rt->getQTree();
		trees->popTree();
		trees->addTree(qt);
		delete rt;
	}

#ifdef GUI
      PsodaPrinter::getInstance()->write("## Resolve Trees Completed Successfully\n");
#endif
}

string ResolveTreesInstr::getName() const {
  return "ResolveTrees";
}
