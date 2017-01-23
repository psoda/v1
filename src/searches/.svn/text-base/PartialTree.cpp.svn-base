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
#include "PartialTree.h"
#include "NewickTreeParser.h"
#include "PsodaPrinter.h"
#include "Interpreter.h"

int MinSplitSize = 30;
int MaxSplitSize = 60;

#define MIN_RATCHET_SIZE 150

#define MAXTREES 1
#define MAXEQUAL 0

#define MAX_TREE_STRING 10000

PartialTree::PartialTree(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, QTree* tree)
:sibling(NULL)
,mTree(tree)
,mSearch(qsearchAlgorithm)
,mEvaluator(evaluator)
,mScore(evaluator->qscoreTree(tree))
,mUpdated(false)
,otherTreeScore(0.0)
,mSize(findSize(tree))
{
}

PartialTree::PartialTree(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,QDeque <int> taxa)
:sibling(NULL)
,mTree(NULL)
,mSearch(qsearchAlgorithm)
,mEvaluator(evaluator)
,mScore(DBL_MAX)
,mUpdated(false)
,otherTreeScore(0.0)
,mSize(taxa.size())
{
	list<QTree*> trees;
	list<QNodeInfo *> nodeInfoList;
	list <QNodeInfo *>::iterator nodeInfoIter;
    QNodeInfo *nodeInfo;
	int index;
	
	PsodaPrinter* printBuffer = PsodaPrinter::getInstance();

	mTree = new QTree();
	taxa.randomize(); //taxa may be randomized already, but they may be selected systematically
	nodeInfoList.clear();
	//Create Nodeinfo structures for all given leaves
	for(index = 0;index <taxa.size();index++)
	{
		nodeInfo = new QNodeInfo(taxa[index],ISLEAF);
		nodeInfoList.push_front(nodeInfo);
		mTree->nnodes()++;
	}
	
	// Start at the beginning of the list
        nodeInfoIter = nodeInfoList.begin();

        // Get the first 3 NodeInfo entries
        QNodeInfo *nodeInfo1 = *nodeInfoIter++; // Get the next value from the iterator
        mTree->insertNodeInfo(nodeInfo1); // Insert the internal node on the tree list
        QNodeInfo *nodeInfo2 = *nodeInfoIter++; // Get the next value from the iterator
        mTree->insertNodeInfo(nodeInfo2); // Insert the internal node on the tree list
        QNodeInfo *nodeInfo3 = *nodeInfoIter++; // Get the next value from the iterator
        mTree->insertNodeInfo(nodeInfo3); // Insert the internal node on the tree list

        // Create 3 Nodes and hook them up to the NodeInfos
        QNode *node1 = new QNode(*nodeInfo1,ISLEAF);
        nodeInfo1->node() = node1;
        QNode *node2 = new QNode(*nodeInfo2,ISLEAF);
        nodeInfo2->node() = node2;
        QNode *node3 = new QNode(*nodeInfo3,ISLEAF);
        nodeInfo3->node() = node3;

        // Now create an internal node and hook everyone to it
        QNodeInfo *internalNodeInfo = new QNodeInfo(index++, ISNTLEAF);
        mTree->insertNodeInfo(internalNodeInfo); // Insert the internal node on the tree list
        QNode *internalNode = new QNode(*internalNodeInfo, ISNTLEAF); 
        internalNodeInfo->node() = internalNode;
        mTree->nnodes()++;

        internalNode->connectExternal(internalNode->internal1(), node2); // Connect the first child
        internalNode->connectExternal(internalNode->internal2(), node3); // Connect the second child
        internalNode->connectExternal(internalNode->internal3(), node1); // Connect the third child

        // Give it a root
        mTree->root() = internalNode;
		
		int dotiteration = 0;
		printBuffer->write("\n");
		while (nodeInfoIter != nodeInfoList.end())
        {
            // Check to see if we need to pause
            checkPausePSODA();

            nodeInfo = *nodeInfoIter;
            // Create a new node for the next taxa
            QNode *node = new QNode(*nodeInfo,ISLEAF);
            nodeInfo->node() = node;
            mTree->insertNodeInfo(nodeInfo); // Insert the new node on the tree list
            // We dont need to increment nnodes because we did this previously

            // Set the score to the maximum value
            mTree->resetScore();
			mTree->subTree1() = mTree->root();
            mTree->subTree2() = node;
            // Then kick off the copy
            mTree->makeNodeLists();

            // Tell TBR that it is already split so he wont try to split it at the next split point
            mTree->isSplit() = true;
            mTree->earlyPrune() = INT_MAX; // Get rid of any early prune data

            // Now find the best rearrangement
            mTree->nodeDataInitialized() = false;

            trees.clear();
			qsearchAlgorithm->rearrange(mTree, evaluator, MAXTREES, MAXEQUAL, DBL_MAX, false, trees); // Last parameter is whether to create a new tree or reuse the current one
            QTree *treep;
            treep = trees.front();
			mTree = treep; // Now use the new tree
			printBuffer->write(".");
						if((dotiteration++%80) == 79) {
              printBuffer->write("\n");
						}
            nodeInfoIter++;
        }
		mScore = mTree->getScore();
}

PartialTree::~PartialTree()
{
	delete mTree;
}

void PartialTree::setMinSplit(int size)
{
	MinSplitSize = size;
}
void PartialTree::setMaxSplit(int size)
{
	MaxSplitSize = size;
}

int PartialTree::size()
{
	return mSize;
}

double PartialTree::score()
{
	if(mSize < 2) return 0.0;
	return mScore;
}

QTree* PartialTree::GetTree()
{
	NewickTreeParser ntp;
	QTree* qt = ntp.parseBuffer(mTree->treeStr(), strlen(mTree->treeStr()), false);
	return qt;
}

PartialTree* PartialTree::divide()
{
	location = GetProjection(mTree);
	if(mSize < MinSplitSize) return NULL; //don't divide
	//find division point (a QNode*)
	QNode* divideAt = divisionPoint(mTree);
	if(divideAt==NULL) return NULL;
	
	//printf("\n--------------------------------------------\n\n");
	//mTree->showTree(false,false);
	
	//build 2 new Partial Trees
	//printf("Dividing A\n");
	QTree* TreeA = halfTree(divideAt);
	if(TreeA == NULL)
	{
		printf("ERROR: Could Not Divide\n");
		return NULL;
	}
	//printf("Dividing B\n");
	QTree* TreeB = halfTree(divideAt->external());
	if(TreeB == NULL)
	{
		printf("ERROR: Could Not Divide\n");
		return NULL;
	}
	//printf("Dividing Tree\n");
	PartialTree* newPT = new PartialTree(mSearch,mEvaluator,TreeA);
	
	newPT->sibling = this;
	newPT->location = location;
	sibling = newPT;
	delete mTree;
	mTree = TreeB;
	mSize = findSize(mTree);
	mScore = mEvaluator->qscoreTree(mTree);
	
	//mTree->showTree(false,false);
	//newPT->mTree->showTree(false,false);
	
	//printf("\n--------------------------------------------\n\n");
	
	return newPT;
}

QNode* PartialTree::divisionPoint(QTree* qt)
{
	//examine all NodeInfos
	
	//choose NodeInfo with largest final -(pre + post) score;
	double best_score = -1.0;
	QNode* best = NULL;
	for(QDeque<QNodeInfo*>::iterator iter = qt->nodeInfoList()->begin();iter != qt->nodeInfoList()->end();iter++)
	{
			double cur_score = mEvaluator->branchScore(*iter);
			if(!(*iter)->leaf())
			{
			if(!(*iter)->node()->child1()->nodeInfo()->leaf())
			if(!(*iter)->node()->child2()->nodeInfo()->leaf())
			if(!(*iter)->node()->external()->nodeInfo()->leaf())
			{
				if(cur_score > best_score)
				{
					best_score = cur_score;
					best = (*iter)->node();
				}
			}
			}
	} 
	return best;
}

QTree* PartialTree::halfTree(QNode* root)
{
	
	NewickTreeParser ntp;
	//build treestring for half tree (do not follow roots external()
	char treeStr[MAX_TREE_STRING];
	memset(treeStr,0,MAX_TREE_STRING);
	 treeStringTop(root,treeStr);
	//parse string
	//printf("%s\n",treeStr);
	QTree* qt = ntp.parseBuffer(treeStr, strlen(treeStr), false);
	return qt;
	
}

void PartialTree::treeString(QNode* root,char* buffer)
{
	if(root->nodeInfo()->leaf())
	{
		sprintf(buffer,"%d",root->nodeInfo()->nodeIndex()+1);
	}
	else
	{
		char child1[MAX_TREE_STRING];
		char child2[MAX_TREE_STRING];
		treeString(root->child1(),child1);
		treeString(root->child2(),child2);
		sprintf(buffer,"(%s,%s)",child1,child2);
	}
}

void PartialTree::treeStringTop(QNode* root,char* buffer)
{
	if(root->nodeInfo()->leaf())
	{
		sprintf(buffer,"(%d)",root->nodeInfo()->nodeIndex()+1);
	}
	else
	{
		char child1[MAX_TREE_STRING];
		char child2[MAX_TREE_STRING];
		char child3[MAX_TREE_STRING];
		if(!root->child1()->nodeInfo()->leaf())
		{
			treeString(root->child1()->child1(),child1);
			treeString(root->child1()->child2(),child2);
			treeString(root->child2(),child3);
		}
		else if(!root->child2()->nodeInfo()->leaf())
		{
			treeString(root->child1(),child1);
			treeString(root->child2()->child1(),child2);
			treeString(root->child2()->child2(),child3);

		}
		else
		{
			treeString(root->child1(),child1);
			treeString(root->child2(),child2);
			sprintf(buffer,"(%s,%s)",child1,child2);
			return;
		}
		sprintf(buffer,"(%s,%s,%s)",child1,child2,child3);
	}
}

int PartialTree::findSize(QTree* qt)
{
	return findSizeRecursive(qt->root())+findSizeRecursive(qt->root()->external());
}

int PartialTree::findSizeRecursive(QNode* root)
{
	if(root->nodeInfo()->leaf())
		return 1;
	else
		return findSizeRecursive(root->child1())+findSizeRecursive(root->child2());
}

bool PartialTree::join(PartialTree*& a,bool force)
{
	if(!force) 
	{
		//Unless force is true don't rejoin siblings, or join if the result is too large
		if(a->mSize + mSize > MaxSplitSize) return false;
		if(a==sibling) return false;
	}
	NewickTreeParser ntp;
	list<QTree*> trees;
	QDeque<QNodeInfo*>::iterator nodeInfoIter;
	
	//add all nodes from a to this
	for(nodeInfoIter = a->mTree->nodeInfoList()->begin();nodeInfoIter != a->mTree->nodeInfoList()->end();nodeInfoIter++)
	{
		mTree->insertNodeInfo(*nodeInfoIter);
	}
	//set up subtrees
	mTree->subTree1() = mTree->root();
	mTree->subTree2() = a->mTree->root();
	mTree->makeNodeLists();
	mTree->isSplit() = true;
	mTree->earlyPrune() = INT_MAX;
	
	//use TBR to join
	trees.clear();
	mSearch->rearrange(mTree, mEvaluator, MAXTREES, MAXEQUAL, DBL_MAX, true, trees); // Last parameter is whether to create a new tree or reuse the current one
	QTree* qt = trees.front();
	
	//delete(mTree);
	mTree = ntp.parseBuffer(qt->treeStr(), strlen(qt->treeStr()), false);
	mSize += a->mSize;
	a = NULL;
	mScore = mEvaluator->qscoreTree(mTree);
	//printf("%f\n",mScore);
	return true;
}

void PartialTree::refine(double& total_score,double startTime)
{
	NewickTreeParser ntp;
	char* treestr;
	otherTreeScore = total_score - mScore;
	if(mSize > MIN_RATCHET_SIZE)
	{
		//for(int progress_limit = mSize/4;progress_limit<=mSize;progress_limit*=2)
		//{
			int progress_limit = mSize+1;
			int no_progress = progress_limit;
			while((no_progress > 0) && (mTree->split()))
			{
				mTree = doTBR(mTree,mScore);
				Display(startTime);
				if(mScore <= mTree->getScore())
				{
					no_progress--;
				}
				else
				{
					no_progress = progress_limit;
				}
				mScore = mTree->getScore();
			}
			Interpreter::getInstance()->dataset()->randomizeWeights(0.15);
			treestr = mTree->treeStr();
			free(mTree);
			mTree = ntp.parseBuffer(treestr,strlen(treestr),false);
			no_progress = progress_limit;
			while((no_progress > 0) && (mTree->split()))
			{
				mTree = doTBR(mTree,mScore);
				Display(startTime);
				if(mScore <= mTree->getScore())
				{
					no_progress--;
				}
				else
				{
					no_progress = progress_limit;
				}
				mScore = mTree->getScore();
			}
			Interpreter::getInstance()->dataset()->resetWeights();
			treestr = mTree->treeStr();
			free(mTree);
			mTree = ntp.parseBuffer(treestr,strlen(treestr),false);
		//}
		
		while(mTree->split())
		{
			mTree = doTBR(mTree,mScore);
			Display(startTime);
			mScore = mTree->getScore();
		}	
		total_score = otherTreeScore + mScore;
	}
	else
	{
		while(1)
		{
			if(!mTree->split())
			{
				total_score = otherTreeScore + mScore;
				return;
			}
			mTree = doTBR(mTree,mScore);
			Display(startTime);
			mScore = mTree->getScore();
		}
	}
}

double PartialTree::distanceWith(PartialTree* pt)
{
	if(pt->mSize + mSize > MaxSplitSize)
		return -1.0;
	
	ProjectedTree A;
	ProjectedTree B;
	A = GetProjection(mTree);
	B = GetProjection(pt->mTree);
	ProjectedTree diff = (A + B) - location;
	return diff.length();
}

QTree* PartialTree::doTBR(QTree* currentTree, double& bestScore)
{
	list<QTree*> trees;
	
    QTree* newTree = NULL;
	
		// Perform the search algorithm.  Since you dont return the same tree that was
        // passed in, you may not return any trees that are better than or equal to the current tree
        // the second to last parameter tells the algorithm to create a new tree 

        trees.clear();
		mSearch->rearrange(currentTree, mEvaluator, MAXTREES, MAXEQUAL, bestScore, true, trees);
        //        printf("search returned trees %d\n",trees.size());
        // Now rejoin the split tree so we can either split it again, or delete it
        currentTree->join();
        // Now print out progress
		
        // rearrange wont return trees if there is no currentSpace and no better trees were found.
        // It will also not return trees if no equal trees were found
        if(trees.size() == 0)
        {
            //                printf("rearrange didnt return any trees\n");
            return currentTree;
        }
        newTree = trees.front();  // Get a tree out of the list
        trees.pop_front();
        //printf("after rearrange %f\n",evaluator->qscoreTree(currentTree, dataset ));


        // If we found a new tree that is better than the best score
        // Flush the tree repository
        if(newTree->getScore() < bestScore)
        {
			mUpdated = true;
            bestScore = newTree->getScore();

            // Nuke the tree repository

            delete(currentTree);
			currentTree = newTree;
		}   
       
        while(trees.size())
        {
            newTree = trees.front();  // Get a tree out of the list
            trees.pop_front();
			//temp = ntp.parseBuffer(newTree->treeStr(),strlen(newTree->treeStr()),false);
			//consense.addTree(temp);
            delete(newTree);
        }

	return currentTree;
}

void PartialTree::Display(double mStartTime)
{
	static int now = 0;
	static int lastprint = 0;
	static int lastmenu = 0;

	now = (int) When();
		
	//display
	if ((now-lastmenu) > 100)
	{
		PsodaPrinter::getInstance()->write("\nElapsed\tSize\t     This Score\tOther Score\tTotal\n");
		PsodaPrinter::getInstance()->write("   time\t\n");
		PsodaPrinter::getInstance()->write("------------------------------------------\n");
		lastmenu = now;
	}
	if ((mUpdated && ((now-lastprint) > 0)) || ((now-lastprint) > 10))
	{
		int seconds = (int) (When()-mStartTime);
		int hours = seconds / (60*60);
		int minutes = (seconds / 60)%60;
		seconds = seconds % 60;
		
		PsodaPrinter::getInstance()->write("%02i:%02i:%02i",hours, minutes, seconds);
		PsodaPrinter::getInstance()->write("\t%d",mSize);
		PsodaPrinter::getInstance()->write("\t%14.1f\t%14.1f\t%14.1f",mScore,otherTreeScore,mScore+otherTreeScore);
		PsodaPrinter::getInstance()->write("\n");

		lastprint = now;
		mUpdated = false;
	}
}


ProjectedTree PartialTree::GetProjection(QTree* qt)
{
	static Hypersphere map;
	return map.lookupTree(qt);
}

