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
#include "GetBestScoreFunction.h"
#include "IntLiteral.h"
#include "RealNumberLiteral.h"
#include "Interpreter.h"
#include "QTreeRepository.h"
#include "QTree.h"
#include <limits.h>
#include <assert.h>
#include "PsodaPrinter.h"

using namespace std;

GetBestScoreFunction::GetBestScoreFunction() : BuiltInCommand() {
  setDescription("Get the score of the best tree in the Tree Repository");
	initDefaultValue("eliminate","True","If true, all but the best tree are eliminated from the tree repository");
  return;
}

GetBestScoreFunction::~GetBestScoreFunction() {
  return;
}

void GetBestScoreFunction::execute(Environment* baseEnv)
{
	Literal* temp = NULL;

	execute(baseEnv,temp);
#ifdef GUI
		PsodaPrinter::getInstance()->write("Best Score %s \n",temp->toString().c_str());
		PsodaPrinter::getInstance()->write("## GetBestScore Completed Successfully\n");
#endif
}

void GetBestScoreFunction::execute(Environment* baseEnv, Literal*& returnVal) {
  //Dataset* defaultDataset;
  //IntLiteral* returnPtr = 0;
	bool eliminate;
	if( baseEnv->lookupString("eliminate") == "True") {
		eliminate = true;
	} else {
		eliminate = false;
	}
		
	double realReturnVal = getScore(eliminate);
  setResult(new RealNumberLiteral(realReturnVal));
  returnVal = getResult();
}

string GetBestScoreFunction::getName() const {
  return "getBestScore";
}

double GetBestScoreFunction::getScore(bool eliminate) { // A value of true means eliminate all but the best trees
  QTreeRepository* repository = Interpreter::getInstance()->qtreeRepository();
  double realReturnVal = INT_MAX;
  double bestscore = INT_MAX;
  QTree *treeptr;

  if (repository) {
		if(eliminate) { // If the eliminate flag is on, take everything out of the repository and put back the best trees.
			deque<QTree*> tempDeque;
			while((treeptr = repository->popTree())) {
				if(treeptr->getScore() < bestscore) {
					bestscore = treeptr->getScore();
				}
        tempDeque.push_back(treeptr);
			}
			while(!tempDeque.empty()) {
				treeptr = tempDeque.front();
				tempDeque.pop_front();
				if(treeptr->getScore() <= bestscore) {
					repository->addTree(treeptr);
				}
			}
		}

    QTree* bestTree = repository->getTree();
    if (bestTree) {
      double bestScore = bestTree->getScore();
      if (bestScore >= 0) {
				realReturnVal = bestScore;
      }
    }
  }
  return(realReturnVal);
}
