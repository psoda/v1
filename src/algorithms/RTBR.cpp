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
#include "RTBR.h"
#include <stdio.h>
#include "QTBR.h"
#include "Interpreter.h"
#include "QNode.h"
#include "QTree.h"
#include "Dataset.h" 
#include "QRetainedResultSearch.h"

using namespace std;

#define MAX_CHILD 4
#define MAX_RECURSION 1
#define MAXTREES 1
#define MAXEQUAL 0

#undef VERBOSE_RTBR

RTBR::RTBR()
{
    mRearrangements = 0;
}

RTBR::~RTBR()
{
}

/**
 * Split the tree, then try all of the possible join points on both subtrees.
 * Returns an array of tree pointers of all the most optimal trees found
 */
void RTBR::rearrange(QTree* startTree, EvaluatorBase  *eval, int numtrees __attribute__((unused)) , int numequal __attribute__((unused)), double bestScore __attribute__((unused)), bool saveOrigTree __attribute__((unused)), list<QTree*>& treeList)
{
	QTree* stree = startTree->createSpecificTree(startTree->subTree1()->nodeInfo(),startTree->subTree2()->nodeInfo(),true); 
	QTree* tree= new QTree();

	mEval = eval;
    //determine sizes of subtrees
	//findSubtreeSize(stree->root());
	//findSubtreeSize(stree->root()->external());
	//start Recursive TBR
	QNode* r = optimizeTree(stree->root(),0);
	tree->root() = r;
	tree->subTree1() = tree->root();
	tree->isSplit() = false;
	

	tree->updateTreeString();
	eval->qscoreTree(tree);

	treeList.push_back(tree);
	return;
}


QNode* RTBR::optimizeTree(QNode* root,int level)
{
	list<QNode*> splitpoints;
	QNode* best = NULL;
	QNode* cur = NULL;
	QNode* left = NULL;
	QNode* right = NULL;
	double best_score = DBL_MAX;
	double cur_score = DBL_MAX;

	//find current score
	QTree* stree = new QTree();
	stree->root() = root;
	AddToNodeInfoList(stree,root);
	AddToNodeInfoList(stree,root->external());
	mEval->qscoreTree(stree);
	
	best = root;
	best_score = stree->getScore();
	

	//create list of splitpoints
	recursiveAddToList(splitpoints,root);
	recursiveAddToList(splitpoints,root->external());
	
	int split_size = splitpoints.size();

	if(split_size <= MAX_CHILD)
	{

		return root;
	}	
	//foreach splitpoint
	int count;
	count = 0;
	while(count < split_size)
	{
		QNode* split_point = splitpoints.front();
		count++;
		
		splitpoints.pop_front();
		//create two subtrees
		left = NULL;
		right = NULL;
		Split(split_point,left,right);
		//optimize subtrees
		if(level < MAX_RECURSION)
		{
			optimizeTree(left,level+1);
			optimizeTree(right,level+1);
		}
		//TBR join trees
		cur_score = best_score;
		cur = TBRJoin(left,right,cur_score,level);
		if(cur_score < best_score)
		{
		
			best_score = cur_score;
			best = cur;
			return best;
		}
		else
		{
			//delete cur;
		}
	}
	return best;
}

void RTBR::recursiveAddToList(std::list<QNode*, std::allocator<QNode*> >& mList, QNode* root)
{
	if(!root) return;
	if(root->nodeInfo()->leaf())
	{
		mList.push_back(root);
		return;
	}
	else
	{
		mList.push_back(root);
		recursiveAddToList(mList,root->child1());
		recursiveAddToList(mList,root->child2());
	}
}

void RTBR::recurseCopy( QNode*& dest, const QNode* src)
{
	QNodeInfo *newNodeInfo;
	if ( src )
	{
		if ( src->nodeInfo()->leaf() )
		{
			newNodeInfo = new QNodeInfo(src->nodeInfo()->nodeIndex(), ISLEAF);
			dest = new QNode(*newNodeInfo, ISLEAF); // Create a leaf
			newNodeInfo->node() = dest;
			dest->internal1() = dest;
		}
		else
		{
			newNodeInfo = new QNodeInfo(src->nodeInfo()->nodeIndex(), ISNTLEAF);
			dest = new QNode(*newNodeInfo, ISNTLEAF); // Create an internal
			newNodeInfo->node() = dest;

			// The copy will point my external at the child
			recurseCopy( dest->child1(), src->child1());
			// Now point the childs external at me
			dest->child1()->external() = dest->internal1();

			// The copy will point my external at the child
			recurseCopy( dest->child2(), src->child2());
			// Now point the childs external at me
			dest->child2()->external() = dest->internal2();
		}
	}
}

void NodePrint(QNode* root,int level)
{
	if(!root)
	{
		printf("NULL root\n");
		return;
	}
	for(int i=0;i<level;i++)
		printf(" ");
	root->nodeInfo()->print();
	if(!root->nodeInfo()->leaf())
	{
		NodePrint(root->child1(),level+1);
		NodePrint(root->child2(),level+1);
		
	}
}

void RTBR::Split(QNode* splitpoint,QNode*& left,QNode*& right)
{
	QNode* copy;
	QNode* copy_ext;
	//Deep copy splitpoint into copy
	recurseCopy(copy,splitpoint);
	recurseCopy(copy_ext,splitpoint->external());
	//divide copy into left and right trees;


	//detach right subtree
	if(copy_ext->nodeInfo()->leaf())
	{
		right = copy_ext;
		right->external() = NULL;  //make a singleton
	}
	else
	{
		right = copy_ext->child1();
		right->external() = copy_ext->child2();
		copy_ext->child2()->external() = right;
	}

	//detach left subtree
	if(copy->nodeInfo()->leaf())
	{
		left = copy;
		left->external() = NULL; //make a singleton
	}
	else
	{
		left = copy->child1();
		left->external()=copy->child2();
		copy->child2()->external() = left;
	}
	
	
	
}

QNode* RTBR::TBRJoin(QNode* left, QNode* right, double& score,int level)
{
	level =0;
	QTree* tree = new QTree();
	AddToNodeInfoList(tree,left);
	AddToNodeInfoList(tree,left->external());
	AddToNodeInfoList(tree,right);
	AddToNodeInfoList(tree,right->external());
	tree->resetScore();
	tree->isSplit() = true;
	tree->subTree1() = left;
	tree->root() = left;
	tree->subTree2() = right;
	tree->makeNodeLists();
	tree->earlyPrune() = INT_MAX;
	tree->nodeDataInitialized() = false;
	list<QTree*> trees;
	trees.clear();
	QTBR* qsearchAlgorithm = new QTBR();
	qsearchAlgorithm->rearrange(tree, mEval, MAXTREES, MAXEQUAL, DBL_MAX, false, trees); // Last parameter is whether to create a new tree or reuse the current one
	rearrangements() += qsearchAlgorithm->rearrangements();
	delete qsearchAlgorithm;
	delete tree;
	tree = trees.front();
	score = tree->getScore();     
	//tree->updateTreeString(); 

	tree->print();
	QNode* ret_val = tree->root();
	/*Delete QTree without deleting QNodes beneath it*/
	tree->root() = NULL;
	tree->nodeInfoList()->clear();

	return ret_val;
}

void RTBR::AddToNodeInfoList(QTree* tree, QNode* root)
{
	if(root)
	{
		tree->insertNodeInfo(root->nodeInfo());
		tree->nnodes()++;
		if(!root->nodeInfo()->leaf())
		{
			AddToNodeInfoList(tree,root->child1());
			AddToNodeInfoList(tree,root->child2());
		}
	}
}
