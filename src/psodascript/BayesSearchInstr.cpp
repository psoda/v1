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
#include "BayesSearchInstr.h"
#include "Interpreter.h"
#include "bayes.h"
#include "PsodaPrinter.h"
#include <assert.h>

using namespace std;

BayesSearchInstr::BayesSearchInstr() : BuiltInCommand() {
  setDescription("Use mrbayes to perform a search.  This calls an external program to perform the search and then inserts the resulting tree into the repository.");
  initDefaultValue("bayesargs","lset nst=6 rates=invgamma;\n", "The parameters passed to mrbayes.  See the documentation for Mrbayes for details");
  initDefaultValue("iterations","0", "The number of iterations.  If zero, then continue until the average standard deviation of split frequencies is less than 0.1");
}

BayesSearchInstr::~BayesSearchInstr() {
  return;
}

void BayesSearchInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

/**
 * Runs a BayesSearch
 */
void BayesSearchInstr::execute(Environment* baseEnv) {
  assert(baseEnv);
	int iterations = atoi(baseEnv->lookupString("iterations").c_str());
	string bayesargs = baseEnv->lookupString("bayesargs");

	bayes_search(*(Interpreter::getInstance()->qtreeRepository()), iterations, bayesargs);
}

string BayesSearchInstr::getName() const {
  return "BayesSearch";
}

/*
void BayesSearchInstr::doBayesSearch(Environment* env) {
  assert(env);
  // Set parameter defaults in the BayesSearchInstr constructor
  // (src/psodaProgramGraph/src/BayesSearchInstr.cpp)
  // Right now, the BayesSearchInstr execute method simply calls this method
  // Put Bayes Search Code Here ...
  PsodaPrinter::getInstance()->write("Doing Bayes Search...\n");
	int iterations = atoi(env->lookupString("iterations").c_str());
	string bayesargs = env->lookupString("bayesargs");
  bayes_search(*qtreeRepository(), iterations, bayesargs);
}*/
