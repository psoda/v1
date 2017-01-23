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
#ifndef QALGORITHM_RTBR 
#define QALGORITHM_RTBR

#include "QAlgorithmBase.h"
#include "QTree.h"

class QTree;
class QAlgorithmBase;

class RTBR : public QAlgorithmBase
{
  public:
    RTBR();
    virtual ~RTBR();

    /**
     * @param treeList This list is populated by rearrange (rather than returning a new list)
     */
    void rearrange(QTree* startTree, EvaluatorBase  *eval, int numtrees, int numequal, double bestScore, bool saveOrigTree, list<QTree*>& treeList);
  private:
  
	QNode* optimizeTree(QNode* root,int level);
	void recurseCopy( QNode*& dest, const QNode* src);
	void Split(QNode* splitpoint,QNode*& left,QNode*& right);
	QNode* TBRJoin(QNode* left, QNode* right,double& score,int level);
	void AddToNodeInfoList(QTree* tree, QNode* root);
	void recursiveAddToList(list<QNode*>& mList, QNode* root);
	EvaluatorBase* mEval;
};
#endif

