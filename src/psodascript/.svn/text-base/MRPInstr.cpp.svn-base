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
#include "MRPInstr.h"
#include "Interpreter.h"
#include <sstream>
#include "RF.h"
#include "PsodaPrinter.h"
#include "PartitionTree.h"
#include "Dataset.h"

#include "HSearchInstr.h"

using namespace std;

MRPInstr::MRPInstr() : BuiltInCommand() {

  setDescription("Add a matrix representation for every tree in the repository.");
  initDefaultValue("start", "stepwise", "Where should the starting tree for the HSearch come from");
  initDefaultValue("criterion", "parsimony");
  initDefaultValue("nreps", 1);
  initDefaultValue("swap", "TBR", "What method should be used for swaping");
  initDefaultValue("wantRecursion", "FALSE");
  initDefaultValue("maxTrees", 1000);
  initDefaultValue("iterations", INT_MAX);
}

MRPInstr::~MRPInstr() {
  return;
}

void MRPInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void MRPInstr::execute(Environment* baseEnv) {

// copy taxa from current dataset
Dataset MatrixRepresentation;
Dataset* curDataset = Interpreter::getInstance()->dataset();
MatrixRepresentation.datatype() = Dataset::DNA_DATATYPE;
MatrixRepresentation.dataformat() = Dataset::ALIGNED_DATAFORMAT;
MatrixRepresentation.gapchar()  ='-';
MatrixRepresentation.missingchar() = '-';
for(int i=0;i<curDataset->ntaxa();i++)
{
	MatrixRepresentation.addName(curDataset->getTaxonName(i));
}

printf("Taxa names copied\n");

//build matrix representations
PartitionTree * tree = NULL;

//Go through the QTreeRepository
	deque<QTree *> tree_list = *(Interpreter::getInstance()->qtreeRepository()->getTrees());
	deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list
	for(treeIt = tree_list.begin(); treeIt != tree_list.end(); treeIt++)
	{
		//For each tree
		tree = new PartitionTree(*treeIt);
		tree->AddMatrixRepresentation(&MatrixRepresentation);
		delete tree;
		tree = NULL;
	}

//set current dataset to be matrix representation

printf("Dataset Built\n");


Interpreter::getInstance()->installDataset(&MatrixRepresentation);

//run hsearch with new dataset
HSearchInstr hs;
QTreeRepository repo;


hs.doHeuristicSearch(baseEnv,&repo);


//restore dataset

Interpreter::getInstance()->installDataset(curDataset);


//display consensus of best trees found
RF con_tree(&repo);
QTree* tree_p = con_tree.consensus(1.0);
tree_p->showTree();
delete tree_p;


}

string MRPInstr::getName() const {
  return "MRP";
}
