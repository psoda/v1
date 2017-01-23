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
#include "NeighborTestInstr.h"
#include <ctime>
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include "ConstructAncestorStatesInstr.h"

/*

	This function should only be used after hsearch has been performed.

*/
// Private
void NeighborTestInstr::operator=(NeighborTestInstr&) {

}
NeighborTestInstr::NeighborTestInstr(NeighborTestInstr& old) : BuiltInCommand(old) {

}

// Public
/**
 *  Constructor
 */
NeighborTestInstr::NeighborTestInstr() : BuiltInCommand() {
  setDescription("Test neighbor joining");
	initDefaultValue("alltrees", "false"); // Set to true to do the Neighbor test on all trees in the repository.
}

/**
 *  Destructor
 */
NeighborTestInstr::~NeighborTestInstr() {
	return;
}

/**
 *  Executes the Parsimony Checking (validating) algorithm
 */
 
void NeighborTestInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
} 
 
void NeighborTestInstr::execute(Environment* baseEnv) {
	doNeighborTest(baseEnv);

	/* Road Map: hsearch -> find longest taxa in tree -> 
		replace it with it's external node's sequence -> 
		run hsearch -> compare trees */
}

/**
 *  Returns the name of this instruction
 */
string NeighborTestInstr::getName() const {
	return "neighborTest";
}


bool NeighborTestInstr::doNeighborTest(Environment* env) {
#ifdef undef
  assert(env);
	PsodaPrinter* printer = PsodaPrinter::getInstance();
	printer->write("Doing Neighbor Test...\n");

	string scope = env->lookupString("alltrees");
	bool alltrees = (scope.compare("true") == 0);
	if (alltrees) {
		int count = Interpreter::getInstance()->qtreeRepository()->numTrees();
		printer->write("Testing %i tree%s.\n",count,(count>1?"s":""));

	}
	bool test_result = true;
	do {
		QTree* tree = Interpreter::getInstance()->qtreeRepository()->popTree();
	
		Interpreter::getInstance()->dataset()->printPairWiseDistances();
	
		// Find longest taxon
		string longest_taxon_name = Interpreter::getInstance()->dataset()->getLongestTaxon();
		QNode* longest_taxon = tree->findLeaf(longest_taxon_name);
		if (longest_taxon == NULL) {
			printer->write("%s::%i - ERROR -- Longest taxon leaf not found!\n",__FILE__,__LINE__);
		} else {
			printer->write("Longest taxon: %s\n", longest_taxon_name.c_str());
		}
	
		// Find sibling
		ConstructAncestorStatesInstr ConAnc;
		ConAnc.doConstructAncestorStates(env);
		
		
		QNode* sibling = longest_taxon->sibling();
		printer->write("Sibling: %s\n",sibling->nodeInfo()->getTaxonName());
	
		// Check distance between longest and sibling
		int sibling_dist = 0;
		const char* ltaxon_seq = longest_taxon->nodeInfo()->getSeqData();
		const char* sibling_seq;
		if (sibling->nodeInfo()->leaf()) {
			sibling_seq = sibling->nodeInfo()->getSeqData();	
		} else {
			sibling_seq = sibling->nodeInfo()->AncState();
		}
		for (int i = 0; i < Interpreter::getInstance()->dataset()->nchars(); i++) {
			if (ltaxon_seq[i] != sibling_seq[i]) { sibling_dist++; }
		}
		printer->write("Distance from %s to %s: %i\n",longest_taxon_name.c_str(),sibling->nodeInfo()->getTaxonName(),sibling_dist);
	
		// Search tree for shorter distance
		bool neighbor_test = Interpreter::getInstance()->dataset()->doNeighborTest(longest_taxon_name.c_str(),sibling_dist);
	
		Interpreter::getInstance()->dataset()->printTranslationMatrix();
		printer->write("tree PARS = [&U] %s;\n",tree->treeStr());
		if (neighbor_test) {
			printer->write(">>> 1\nTree %s passed the neighbor test.\n",tree->treeStr());
		} else {
			printer->write(">>> 0\nTree %s failed the neighbor test.\n",tree->treeStr());
		}
		test_result = (test_result && neighbor_test);
	} while (Interpreter::getInstance()->qtreeRepository()->numTrees() > 0 && alltrees);
	//printer->write("Num trees: %i & alltrees = %s\n",Interpreter::getInstance()->qtreeRepository()->numTrees(),(alltrees?"true":"false"));
	return test_result;
#endif
}
