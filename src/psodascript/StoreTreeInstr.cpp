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
#include "StoreTreeInstr.h"
#include "Interpreter.h"
#include "TreeMap.h"
#include "QTree.h"
#include "PsodaWarning.h"
#include <sstream>

using namespace std;

StoreTreeInstr::StoreTreeInstr() : BuiltInCommand() {
  setDescription("Takes a tree from the current tree repository and stores a copy of it in the interpreter\'s tree map.");
  initDefaultValue("name", "*default", "The name by which the stored tree will be identified in the tree map");
}

StoreTreeInstr::~StoreTreeInstr() {
  return;
}

void StoreTreeInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) ) {
  execute(baseEnv);
}

void StoreTreeInstr::execute(Environment* baseEnv) {
  string treeName = baseEnv->lookup("name").toString();
  QTree* treeToStore = Interpreter::getInstance()->qtreeRepository()->getTree();
  if (!treeToStore) {
    ostringstream message;
    message << "There are no trees in the current repository that can be stored. Not storing tree \"" << treeName << "\".";
    throw PsodaWarning(message.str());
  }
  QTree* treeCopy = new QTree(treeToStore);
  Interpreter::getInstance()->getTreeMap().store(treeName, treeCopy);
}

string StoreTreeInstr::getName() const {
  return "StoreTree";
}
