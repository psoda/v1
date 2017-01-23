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
#ifndef PSODA_COMMON_CLADE_REFINEMENT 
#define PSODA_COMMON_CLADE_REFINEMENT

#include "QSearchBase.h"
#include "QAlgorithmBase.h"
#include "EvaluatorBase.h"
#include "QTree.h"
#include "QTreeRepository.h"
#include "Globals.h"
#include <vector>
#include "Partition.h"
#include <map>
#include <list>

class QTree;

class QAlgorithmBase;
class EvaluatorBase;

class CommonCladeRefinement : public QSearchBase
{
  public:
    CommonCladeRefinement();
    //VIRTUAL INTERFACE FUNCTIONS
    void search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int iterations);
	const char* name() const; 
    //GETTERS & SETTERS

  protected:
	  void ConsiderCombination(QNode* subtree1,QNode* subtree2,QTreeRepository& qtreeRepository,int nchar,double& best_score,EvaluatorBase* evaluator);
  
	  QTree* BuildTree(QNode*,QNode*);
	  void DebugTreeScore(QNode*,QNode*,int nchar);
	  double DebugTreeScoreRecursive(QNode*,char*,int nchar);
	  char* BuildString(QNode* root,bool* included,char*);
	  
	  int combine(char* src1, char* src2,int nchar);
	  int combineDest(char* dest,char* src1, char* src2,int nchar);
	  void AddTreeToMap(QTree* tree);
	  Partition AddNodeToMap(QNode* root);
	  map<Partition,list<QNode*> > CladeMap;
	  //multimap<Partition,pair<double,double> > CladeMap;
	  int mNtaxa;
};
#endif

