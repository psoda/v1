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
#include "ConstructAncestorStatesInstr.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include "PsodaWarning.h"
#include <assert.h>

using namespace std;

ConstructAncestorStatesInstr::ConstructAncestorStatesInstr() : BuiltInCommand() {
  setDescription("Construct the ancestral character states for a tree\n");
  initDefaultValue("display", "yes", "display states");
  initDefaultValue("assign", "yes", "assign the computed ancestral states to ancester data structures");
}

ConstructAncestorStatesInstr::~ConstructAncestorStatesInstr() {
  return;
}

void ConstructAncestorStatesInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void ConstructAncestorStatesInstr::execute(Environment* baseEnv) {
  doConstructAncestorStates(baseEnv);
}

string ConstructAncestorStatesInstr::getName() const {
  return "ConstructAncestorStates";
}


/**
 * Constructs Ancestor States ... (whoever implements this should write a better description)
 */
void ConstructAncestorStatesInstr::doConstructAncestorStates(Environment* env) {
  assert(env);
  // Set parameter defaults in the ConstructAncestorStatesInstr constructor
  // (src/psodascript/ConstructAncestorStatesInstr.cpp)
  // Right now, the ConstructAncestorStatesInstr execute method simply calls this method
  // Put Construct Ancestor States Code Here ...
	QTree* tree;
	string displayType = env->lookupString("display");
	string assignType = env->lookupString("assign");
	int i;
	int chars;
 
	
	PsodaPrinter::getInstance()->write("Constructing Ancestor States...\n");
	
	for(i=0;i<Interpreter::getInstance()->qtreeRepository()->numTrees();i++)
		{
			tree = Interpreter::getInstance()->qtreeRepository()->popTree();
			
			if (!tree)
			{
				throw PsodaWarning("Cannot reconstruct ancestral states without a tree.");
			}
			
			
			chars = tree->nchars();
			PsodaPrinter::getInstance()->write("nchars = %d\n",chars);
			if(chars==0)
			{
				throw PsodaWarning("Trees have 0 characters of data.");
			}
			
			tree->calculateAncestralStates();
			if(assignType == "yes")
			{
				tree->assignAncestor();
			}
			if(displayType == "yes")
			{
			
				PsodaPrinter::getInstance()->write("Displaying Ancestor States...\n");
			
				PsodaPrinter::getInstance()->write("Tree %d:\n",i+1);
				tree->displayAncestralStates();
			}
			Interpreter::getInstance()->qtreeRepository()->addTree(tree);
		}
  
  
}

