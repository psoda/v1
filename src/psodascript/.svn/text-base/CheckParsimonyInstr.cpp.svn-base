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
#include "CheckParsimonyInstr.h"
#include <ctime>
#include "Interpreter.h"
#include "HSearchInstr.h"
#include "PsodaPrinter.h"
#include "ConstructAncestorStatesInstr.h"
#include <assert.h>

/*

	Parameters this class can use:
		longestTaxa	-- (string) specifies which node to replace with the ancestor
		cpSeed		-- (int | "time") used to seed the random number generator used to sample from ancestral states
		construct	-- (string - "parsimony" | "sampling") specifies whether to use the parsimony constructed or sampled ancestor states
		sub_frequency  (double) specifies the substitution frequency for sampling ancestor states

*/
// Private
void CheckParsimonyInstr::operator=(CheckParsimonyInstr&) {

}
CheckParsimonyInstr::CheckParsimonyInstr(CheckParsimonyInstr& old) : BuiltInCommand(old) {

}

// Public
/**
 *  Constructor
 */
CheckParsimonyInstr::CheckParsimonyInstr() : BuiltInCommand() {
  setDescription("Parsimony validation test.");
	initDefaultValue("longestTaxa", "_calculate_", "Unless specified by the user, the longest taxa will be calculated using parsimony criteria.");
	initDefaultValue("lbshorten_method", "substitution","Method to shorten long branch"); // Or "NJ" to use NJ comparison to validate parsimony tree
	initDefaultValue("construct", "sampling", "Use ancestor state constructed by sampling or parsimony"); // Or "parsimony" to use ancestor state constructed by parsimony
  addParamOption("construct", "parsimony");
	initDefaultValue("cpSeed", "time", "Random seed can be specified, otherwise, use the current time");		/* Random seed will be used unless specified by the user. */
	initDefaultValue("sub_frequency", "0.75", "Substitution Frequency");

	initDefaultValue("start", "stepwise", "Start Tree generation method");
	initDefaultValue("criterion", "parsimony", "Optimality criteria");
	initDefaultValue("nreps", 1, "Number of Replicates");
	initDefaultValue("swap", "TBR", "Swapping method");
	initDefaultValue("wantRecursion", "FALSE", "Recursion (Not currently Implemented)");
	initDefaultValue("maxTrees", INT_MAX, "Maximum number of trees to keep in the repository");
	initDefaultValue("iterations", INT_MAX, "Number of iterations to perform");
}

/**
 *  Destructor
 */
CheckParsimonyInstr::~CheckParsimonyInstr() {
	return;
}

/**
 *  Executes the Parsimony Checking (validating) algorithm
 */
 
void CheckParsimonyInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
} 
 
void CheckParsimonyInstr::execute(Environment* baseEnv) {
	doCheckParsimony(baseEnv);

	/* Road Map: hsearch -> find longest taxa in tree -> 
		replace it with it's external node's sequence -> 
		run hsearch -> compare trees */
}

/**
 *  Returns the name of this instruction
 */
string CheckParsimonyInstr::getName() const {
	return "checkParsimony";
}

/**
 * This Method runs the Parsimony Validating algorithm.
 * It compares the results of hsearch with the results of
 * hsearch on a shortened-longest-branch dataset.
 */
void CheckParsimonyInstr::doCheckParsimony(Environment* env) {
#ifdef undef

  assert(env);
	/* Road Map: hsearch -> find longest taxa in tree -> 
		replace it with it's external node's sequence -> 
		run hsearch -> compare trees */
	
	/* hsearch & save results */
	HSearchInstr hs;
	hs.doHeuristicSearch(env,Interpreter::getInstance()->qtreeRepository());
	char* oldTreeName = new char[1024];
	strcpy(oldTreeName,Interpreter::getInstance()->qtreeRepository()->getTree()->treeStr());

	/* Construct ancestral states */
	ConstructAncestorStatesInstr ConAnc;
	ConAnc.doConstructAncestorStates(env);

	/* Find longest taxa in tree */
	string taxon = env->lookupString("longestTaxa");
	if (taxon.compare("calculate") == 0) {
		// Calculate the longest taxa
		PsodaPrinter::getInstance()->write("Calculating longest taxon.\n");
		taxon = Interpreter::getInstance()->dataset()->getLongestTaxon();
		PsodaPrinter::getInstance()->write("Using %s as the longest taxon.\n",taxon.c_str());
	} 
	QNode* leaf = Interpreter::getInstance()->qtreeRepository()->getTree()->findLeaf(taxon);
	if (leaf == NULL) {
		printf("%s::%i - Error - Leaf not found.\n",__FILE__,__LINE__);
	}
	if (!leaf->nodeInfo()->leaf()) { printf("%s::%i - Not a leaf!\n",__FILE__,__LINE__);}
	QNode* ancestor = leaf->external();

	Interpreter::getInstance()->dataset()->printPairWiseDistances();

	// *** Validate Parsimony Tree *** //
	QNode* brother = leaf->sibling();
	int brotherNumber = Interpreter::getInstance()->dataset()->getTaxonNumber(brother->nodeInfo()->getTaxonName());
	PsodaPrinter::getInstance()->write("Longest branch: %s\n",leaf->nodeInfo()->getTaxonName());
	PsodaPrinter::getInstance()->write("Using as brother: %s\n",brother->nodeInfo()->getTaxonName());	

	string method = env->lookupString("lbshorten_method");
	if (method.compare("substitution") == 0) {
		/* Modify dataset */
		PsodaPrinter::getInstance()->write("Shortening longest branch\n");
		const char* leafSeq = leaf->nodeInfo()->getSeqData();
		int taxonNumber = Interpreter::getInstance()->dataset()->getTaxonNumber(taxon.c_str());

		string seed = env->lookupString("cpSeed");
		if (seed.compare("time")==0) {
			srand(time(NULL));
			PsodaPrinter::getInstance()->write("%s - Using time as seed for ancestor state sampling.\n",__FILE__);
		} else {
			istringstream tmp(seed);
			int s;
			if (! (tmp >> s)) {
				s = time(NULL);
				PsodaPrinter::getInstance()->write("%s - Seed parameter incorrect: %s. Using %i.\n",__FILE__,seed.c_str(),s);
			}
			srand(s);
		}
	
		string construct = env->lookupString("construct");
		string frequency = env->lookupString("sub_frequency");
		double freq = 0.5;
		istringstream iss(frequency);
		if (! (iss >> freq)) {
			freq = 0.5;
			PsodaPrinter::getInstance()->write("%s, %i - Substitution frequency parameter incorrect: %s. Using %f.\n",__FILE__,__LINE__,frequency.c_str(),freq);
		}
		PsodaPrinter::getInstance()->write("Using substitution frequency: %f.\n",freq);
		if (construct.compare("parsimony") == 0) {
			Interpreter::getInstance()->dataset()->replaceCharacters(ancestor->nodeInfo()->AncState(),taxonNumber);
		}
		else {
			int n = Interpreter::getInstance()->dataset()->nchars();
			char* newSeq = new char[n+1];
			newSeq[n] = 0;
			for (int i = 0; i < n; i++) {
				if ((0.0 + rand()) / RAND_MAX < freq) {
					int rand_node;
					while( (rand_node = (rand() % Interpreter::getInstance()->dataset()->ntaxa())) == taxonNumber || rand_node == brotherNumber){};
						//printf("%s, %i - rand_node = %i\n",__FILE__,__LINE__,rand_node);
					newSeq[i] = Interpreter::getInstance()->dataset()->getCharacter(rand_node,i);
				} else {
					newSeq[i] = leafSeq[i];
				}
			}
			Interpreter::getInstance()->dataset()->replaceCharacters(newSeq,taxonNumber);
		}
		
		Interpreter::getInstance()->dataset()->printPairWiseDistances();
		/* hsearch & compare */
			Interpreter::getInstance()->dataset()->printMatrix();
		
	hs.doHeuristicSearch(env,Interpreter::getInstance()->qtreeRepository());
		char* newTreeName = Interpreter::getInstance()->qtreeRepository()->getTree()->treeStr();
		Interpreter::getInstance()->dataset()->printTranslationMatrix();
		PsodaPrinter::getInstance()->write("tree ORIG = [&U]  %s;\n", oldTreeName);
		PsodaPrinter::getInstance()->write("tree LBS = [&U]  %s;\n", newTreeName);
			//*/
	
		// Declare Result
		// PsodaPrinter::getInstance()->write(">>>%s\n",(strcmp(oldTreeName,newTreeName)==0)?"TRUE":"FALSE");
	
	} else if (method.compare("NJ") == 0) {
	// Neighbor Joining method
	//  If longest branch and sibling are not neighbors, suggest MaxL
		
		// Find distance to brother
		const char* leafseq = leaf->nodeInfo()->getSeqData();
		const char* brotherseq = brother->nodeInfo()->getSeqData();
		int sibling_dist = 0;
		for (int i = 0; i < Interpreter::getInstance()->dataset()->nchars(); i++) {
			if (leafseq[i] != brotherseq[i]){ sibling_dist++; }
		}
		PsodaPrinter::getInstance()->write("Distance from %s to %s = %i\n",leaf->nodeInfo()->getTaxonName(),brother->nodeInfo()->getTaxonName(),sibling_dist);

		// Compare distance to all other leaves
		bool siblings_are_neighbors = true;
		vector<string>& names = Interpreter::getInstance()->dataset()->taxonNames();
		for (vector<string>::iterator gerbil = names.begin(); gerbil != names.end() && siblings_are_neighbors; gerbil++) {
			if (gerbil->compare(leaf->nodeInfo()->getTaxonName()) == 0) {
				continue;
			}
			int dist = 0;
			const char* tax_seq = Interpreter::getInstance()->dataset()->getCharacters(Interpreter::getInstance()->dataset()->getTaxonNumber(gerbil->c_str()));
			for (int rat = 0; rat < Interpreter::getInstance()->dataset()->nchars(); rat++) {
				if (tax_seq[rat] != leafseq[rat]) { dist++; }
			}
			siblings_are_neighbors = (dist >= sibling_dist);
			if (!siblings_are_neighbors) {
				PsodaPrinter::getInstance()->write("Closest neighbor to %s is %s (dist = %i), not %s.\n",leaf->nodeInfo()->getTaxonName(),gerbil->c_str(),
																											dist,brother->nodeInfo()->getTaxonName());
			}
		}

		// Report result
		Interpreter::getInstance()->dataset()->printTranslationMatrix();
		PsodaPrinter::getInstance()->write("tree ORIG = [&U] %s;\n",oldTreeName);
		if (siblings_are_neighbors) {
			PsodaPrinter::getInstance()->write(">>> 1\nParsimony constructed tree passed NJ test.\n");
		} else {
			PsodaPrinter::getInstance()->write(">>> 0\nParsimony constructed tree failed NJ test.\n");
		}

	} else {
		// We don't support any other methods...
		PsodaPrinter::getInstance()->write("We don't support this option: %s = %s\n","lbshorten_method",method.c_str());
	}
	delete[] oldTreeName;
	
#endif
}

