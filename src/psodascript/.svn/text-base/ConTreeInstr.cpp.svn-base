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
#include "ConTreeInstr.h"
#include "Interpreter.h"
#include <sstream>
#include "RF.h"
#include "PsodaPrinter.h"


using namespace std;

ConTreeInstr::ConTreeInstr() : BuiltInCommand() {
  setDescription("Compute the Consensus tree");
  initDefaultValue("showtree", "true");
  initDefaultValue("indices", "false");
  initDefaultValue("strict", "true");
  initDefaultValue("majrule", "false");
  initDefaultValue("percent", 100);
  initDefaultValue("grpfreq", "false");
  initDefaultValue("usetreewts", "false");
  initDefaultValue("replace","false");
}

ConTreeInstr::~ConTreeInstr() {
  return;
}

void ConTreeInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void ConTreeInstr::execute(Environment* baseEnv) {

	float fraction = 1.0;
	
	if(baseEnv->lookupString("strict") == "true")
		fraction = 1.0;
	if(baseEnv->lookupString("majrule") == "true")
		fraction = 0.5;
	if (baseEnv->lookupString("percent") != "")
		fraction = float (float(atoi(baseEnv->lookupString("percent").c_str())) / 100.0);
	   
	QTreeRepository* treeRepository = Interpreter::getInstance()->qtreeRepository();
	RF con_tree(treeRepository);
	QTree* tree = con_tree.consensus(fraction);
	printf("%2.0f%% Consensus tree %s\n",fraction * 100,tree->treeStr());
	tree->showTree();
	con_tree.print(0.0);
	if(baseEnv->lookupString("replace") == "true")
	{
		treeRepository->removeAll();
		treeRepository->addTree(tree);
	}
	else
	{
		delete tree;
	}
#ifdef GUI
        PsodaPrinter::getInstance()->write("## ConTree Completed Successfully\n");
#endif
}

string ConTreeInstr::getName() const {
  return "contree";
}
