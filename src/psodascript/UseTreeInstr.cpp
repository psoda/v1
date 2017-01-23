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
#include "UseTreeInstr.h"
#include "Interpreter.h"
#include "QTree.h"
#include "QTreeRepository.h"
#include "StringLiteral.h"
#include "PsodaWarning.h"
#include <sstream>

using namespace std;

UseTreeInstr::UseTreeInstr() : BuiltInCommand() {
  setDescription("Uses a tree from the interpreter\'s tree map by adding a copy of it to the current tree repository");
  initDefaultValue("name", "*default", "The name of the tree to use");
  initDefaultValue("clearrep", "yes", "If this parameter is 'yes', clears the contents of the tree repository before adding the tree");
}

UseTreeInstr::~UseTreeInstr() {
  return;
}

void UseTreeInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) ) {
  execute(baseEnv);
}

void UseTreeInstr::execute(Environment* baseEnv) {
  string treeName = baseEnv->lookup("name").toString();
  bool clearRepository = (StringLiteral::toLowerCase(baseEnv->lookup("clearrep").toString()) == "yes");
  QTree* treeToUse = Interpreter::getInstance()->getTreeMap().get(treeName);
  QTreeRepository* repository = Interpreter::getInstance()->qtreeRepository();
  if (treeToUse && repository) {
    if (clearRepository) {
      repository->removeAll();
    }
    QTree* copyOfTreeToAdd = new QTree(treeToUse);
    repository->addTree(copyOfTreeToAdd);
  } else {
    ostringstream message;
    message << "Tree \"" << treeName << "\" does not exist in TreeMap";
    throw PsodaWarning(message.str());
  }
}

string UseTreeInstr::getName() const {
  return "UseTree";
}
