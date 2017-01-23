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
#ifndef QSODA_TREEREPOSITORY 
#define QSODA_TREEREPOSITORY

#include "QTree.h"
#include "EvaluatorBase.h"
#include <deque>

#define NEXUS_TREE_FORMAT 1
#define NEWICK_TREE_FORMAT 2

class EvaluatorBase;
class QTreeRepository;
class  QTreeRepository
{
  public:
    QTreeRepository();
    QTreeRepository(bool filterIsomorphic);
  public:
    ~QTreeRepository();

    //VIRTUAL INTERFACE FUNCTIONS
    void addTree(QTree*&);
    QTree* getSearchableTree(bool print=true);
    QTree* getTree();
	QTree* popTree();
    QTree** getBestTrees(int*);
    void rescore(EvaluatorBase *evaluator);
    void print();
    void print(std::ostream &output);
    void printNewick(std::ostream &output);
    void removeAll();
	// The number of trees in the tree repository that still need to be swapped
    int numTreesToSwap();
	// The number of trees in the tree repository
    int numTrees() const;
		void  setMaxTrees(int maxTreeVal, int ntaxa);
    unsigned int  maxTrees() const;
    const deque<QTree *>* getTrees();

    /**
     * Sets earlyPrune on all trees in the repository
     */
    void setEarlyPrune(int newEarlyPruneScore);

    /**
     * Sets the visited() on all trees and node to false
     */
    void clearVisited();
	double rescoreTrees(EvaluatorBase *evaluator);
    void randomMorph();

  protected:
    deque<QTree *> mList; //STL list of trees
    unsigned int mMaxTrees;
		bool mFilterIsomorphic;
};
#endif
