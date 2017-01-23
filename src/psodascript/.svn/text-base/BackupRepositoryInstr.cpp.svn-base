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
#include "BackupRepositoryInstr.h"
#include "Interpreter.h"
#include "QTreeRepository.h"
#include "QTree.h"

using namespace std;

BackupRepositoryInstr::BackupRepositoryInstr() : BuiltInCommand() {
  setDescription("Make a copy of the tree repository into the accumulator.");
  return;
}

BackupRepositoryInstr::~BackupRepositoryInstr() {
  return;
}

void BackupRepositoryInstr::execute(Environment* baseEnv __attribute__((unused))) {
    QTreeRepository* repository = Interpreter::getInstance()->qtreeRepository();
    QTreeRepository* accumulator = Interpreter::getInstance()->getAccumulator();
    accumulator->removeAll();
    const deque<QTree*>* trees = repository->getTrees();
    deque<QTree*>::const_iterator treeItr = trees->begin();
    deque<QTree*>::const_iterator treeEnd = trees->end();
    for (; treeItr != treeEnd; treeItr++) {
      QTree* thisTree = *treeItr;
	  QTree* copy = new QTree(thisTree);
      accumulator->addTree(copy);
    }
}

void BackupRepositoryInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)))
{
	execute(baseEnv);
}

string BackupRepositoryInstr::getName() const {
  return "BackupRepository";
}
