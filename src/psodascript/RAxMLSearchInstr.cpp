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
#include "RAxMLSearchInstr.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include "raxml.h"

using namespace std;

RAxMLSearchInstr::RAxMLSearchInstr() : BuiltInCommand() {
  setDescription("Execute a raxml search.");
  initDefaultValue("raxargs", "-m GTRGAMMA","These are raxml parameters.  See the raxml manual for details.");
}

RAxMLSearchInstr::~RAxMLSearchInstr() {
  return;
}

void RAxMLSearchInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void RAxMLSearchInstr::execute(Environment* baseEnv) {
  doRAxMLSearch(baseEnv);
#ifdef GUI
      PsodaPrinter::getInstance()->write("## RAxMLSearch Completed Successfully\n");
#endif
}

string RAxMLSearchInstr::getName() const {
  return "RAxMLSearch";
}

/**
 * Runs a RAxMLSearch
 */
void RAxMLSearchInstr::doRAxMLSearch(Environment* env) {
  assert(env);
  // Set parameter defaults in the RAxMLSearchInstr constructor
  // (src/psodaProgramGraph/src/RAxMLSearchInstr.cpp)
  // Right now, the RAxMLSearchInstr execute method simply calls this method
  // Put RAxML Search Code Here ...
  PsodaPrinter::getInstance()->write("Doing RAxML Search...\n");
	int iterations = atoi(env->lookupString("iterations").c_str());
	string raxargs = env->lookupString("raxargs");
  raxml_search(*Interpreter::getInstance()->qtreeRepository(), iterations, raxargs);
}
