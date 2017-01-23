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
#include "LBSTestInstr.h"
#include "HSearchInstr.h"
#include "PsodaPrinter.h"
#include "Interpreter.h"
#include "ConstructAncestorStatesInstr.h"


void LBSTestInstr::operator=(LBSTestInstr& other __attribute__((unused))) {}
LBSTestInstr::LBSTestInstr(LBSTestInstr& other __attribute__((unused))) : BuiltInCommand() {}

/**
 *  Constructor
 */
LBSTestInstr::LBSTestInstr() : BuiltInCommand() {
  setDescription("Long Branch Shortening.");
	initDefaultValue("alltrees", "false");
	initDefaultValue("longestTaxa", "_calculate_"); // Unless specified by the user, the longest taxa will be calculated using parsimony criteria. 
	initDefaultValue("lbshorten_method", "substitution"); // Or "NJ" to use NJ comparison to validate parsimony tree
	initDefaultValue("construct", "sampling"); // Or "parsimony" to use ancestor state constructed by parsimony
	initDefaultValue("cpSeed", "time");		// Random seed will be used unless specified by the user. 
	initDefaultValue("sub_frequency", "0.25");
	initDefaultValue("start", "stepwise");
	initDefaultValue("criterion", "parsimony");
	initDefaultValue("nreps", 1);
	initDefaultValue("swap", "TBR");
	initDefaultValue("wantRecursion", "FALSE");
	initDefaultValue("maxTrees", INT_MAX);
	initDefaultValue("iterations", INT_MAX);
}

/**
 *  Destructor
 */
 LBSTestInstr::~LBSTestInstr() {

}

/**
 *  Executes the Parsimony Checking (validating) algorithm
 */
   void LBSTestInstr::execute(Environment* baseEnv) {
	doLBSTest(baseEnv);
  }
   void LBSTestInstr::execute(Environment* baseEnv , Literal*& returnVal __attribute__((unused))) {
	execute(baseEnv);
  }

/**
 *  Returns the name of this instruction
 */
 string LBSTestInstr::getName() const {
	return "lbstest";
}


bool LBSTestInstr::doLBSTest(Environment* env) {
#ifdef undef
  assert(env);
	PsodaPrinter* printer = PsodaPrinter::getInstance();
	printer->write("\nDoing Long Branch Shortening Test...\n");

	QTreeRepository* orig_repository = Interpreter::getInstance()->qtreeRepository();
	Interpreter::getInstance()->qtreeRepository() = new QTreeRepository();

	string scope = env->lookupString("alltrees");
	bool alltrees = (scope.compare("true") == 0);
	if (alltrees) {
		int count = orig_repository->numTrees();
		printer->write("Testing %i tree%s.\n",count,(count!=1?"s":""));

	}
	bool test_result = true;

	const deque<QTree*>* trees = orig_repository->getTrees();
	_Deque_iterator<QTree*,QTree* const&,QTree* const*> tree = trees->begin();
	do {
		const char* oldTreeName = (*tree)->treeStr();
		printer->write("\nTesting %s\n",oldTreeName);

		Dataset* data = Interpreter::getInstance()->dataset();
		data->printPairWiseDistances();
	
		// Find longest taxon
		string longest_taxon_name = data->getLongestTaxon();
		QNode* longest_taxon = (*tree)->findLeaf(longest_taxon_name);
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
	
		// Sample from everyone else
		printer->write("Shortening longest branch\n");
		const char* longest_taxon_seq = longest_taxon->nodeInfo()->getSeqData();

		string seed = env->lookupString("cpSeed");
		if (seed.compare("time")==0) {
			srand(time(NULL));
			printer->write("%s - Using time as seed for ancestor state sampling.\n",__FILE__);
		} else {
			istringstream tmp(seed);
			int s;
			if (! (tmp >> s)) {
				s = time(NULL);
				printer->write("%s - Seed parameter incorrect: %s. Using %i.\n",__FILE__,seed.c_str(),s);
			}
			srand(s);
		}

		string construct = env->lookupString("construct");
		
		if (construct.compare("parsimony") == 0) {
			data->replaceCharacters(longest_taxon->external()->nodeInfo()->AncState(),data->getTaxonNumber(longest_taxon_name.c_str()));

		} else {
			// Define substitution frequency
			string frequency = env->lookupString("sub_frequency");
			double freq = 0.5;
			istringstream iss(frequency);
			if (! (iss >> freq)) {
				freq = 0.5;
				printer->write("%s, %i - Substitution frequency parameter incorrect: %s. Using %f.\n",__FILE__,__LINE__,frequency.c_str(),freq);
			}
			printer->write("Using substitution frequency: %f.\n",freq);

			// Prepare sampling set
			vector<string>& names = data->taxonNames();
			vector<int> numbers;
			string sib = sibling->nodeInfo()->getTaxonName();
			string anc = longest_taxon->external()->nodeInfo()->getTaxonName();

			for (vector<string>::iterator iter = names.begin(); iter != names.end(); iter++) {
				if (iter->c_str() == NULL || iter->compare(longest_taxon_name) * iter->compare(sib) * iter->compare(anc) == 0) {
					continue;
				}
				numbers.push_back(data->getTaxonNumber(iter->c_str()));
			}

			// Gather pairwise distances
			//map<string,int>& distances = data->getPairWiseDistances();

			int n = data->nchars();
			char* newSeq = new char[n+1];
			newSeq[n] = '\0';

			for (int i = 0; i < n; i++) {
				// Randomly substitute nucleotides
				if ((0.0 + rand()) / RAND_MAX < freq) {

					// Choose other node to sample from
					int rand_node = rand() % numbers.size();
					newSeq[i] = data->getCharacter(numbers.at(rand_node),i);

					/*while( (rand_node = (rand() % data()->ntaxa())) == taxonNumber || rand_node == brotherNumber){};
						//printf("%s, %i - rand_node = %i\n",__FILE__,__LINE__,rand_node);
					newSeq[i] = data()->getCharacter(rand_node,i);*/
				} else {
					newSeq[i] = longest_taxon_seq[i];
				}
			}
			data->replaceCharacters(newSeq,data->getTaxonNumber(longest_taxon_name.c_str()));
		}

		data->printPairWiseDistances();
		//data->printMatrix();

		// Run hsearch again and compare
		HSearchInstr hs;
		
	hs.doHeuristicSearch(env,Interpreter::getInstance()->qtreeRepository());
		char* newTreeName = Interpreter::getInstance()->qtreeRepository()->getTree()->treeStr();
		data->printTranslationMatrix();
		printer->write("tree ORIG = [&U]  %s;\n", oldTreeName);
		printer->write("tree LBS = [&U]  %s;\n", newTreeName);
			//*/
	
		// Declare Result
		bool lbs_test = (strcmp(oldTreeName,newTreeName)==0);
	
		if (lbs_test) {
			printer->write(">>> 1\nTree %s passed the LBS test.\n",(*tree)->treeStr());
		} else {
			printer->write(">>> 0\nTree %s failed the LBS test.\n",(*tree)->treeStr());
		}
		test_result = (test_result && lbs_test);

		// Clean up
		data->replaceCharacters(longest_taxon_seq,data->getTaxonNumber(longest_taxon_name.c_str()));
		Interpreter::getInstance()->qtreeRepository()->removeAll();

		tree++;
	} while (tree != trees->end() && alltrees);

	delete Interpreter::getInstance()->qtreeRepository();
	Interpreter::getInstance()->qtreeRepository() = orig_repository;

	return test_result;
	
#endif
}
