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
#include "SectorStep.h"
#include "QNode.h"
#include "QNodeInfo.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <float.h>
#include "QDeque.h"
#include "Dataset.h"

#include "NewickTreeParser.h"

#define MAXTREES 1
#define MAXEQUAL 0

#define MIN_SECTOR 50


QSectorStepwiseAdditionSearch::QSectorStepwiseAdditionSearch():mStartTime(0.0),mUpdated(false)
{
}

QSectorStepwiseAdditionSearch::~QSectorStepwiseAdditionSearch()
{
}

const char* QSectorStepwiseAdditionSearch::name() const
{
	return "Sector Stepwise Addition";
}


void QSectorStepwiseAdditionSearch::DisplayScore()
{
	static int now = 0;
	static int lastprint = 0;
	static int lastmenu = 0;

	now = (int) When();
		
	//display
	if ((now-lastmenu) > 100)
	{
		PsodaPrinter::getInstance()->write("\nElapsed\tSubtrees\t     Min Score\t\tMax Score\n");
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
		
		double minScore=0.0;
		double maxScore=0.0;
		for(int i=0;i<mNSubTrees;i++)
		{
			minScore += mSubScores[i];
		}
		maxScore = minScore + (mNSubTrees-1) * Interpreter::getInstance()->dataset()->nchars();
		if(minScore > Interpreter::getInstance()->dataset()->nchars() * Interpreter::getInstance()->dataset()->ntaxa())
		{
			PsodaPrinter::getInstance()->write("\t%d",mNSubTrees);
		PsodaPrinter::getInstance()->write("\n");
		}
		else
		{
		PsodaPrinter::getInstance()->write("\t%d\t%14.1f\t%14.1f",mNSubTrees,minScore,maxScore);
		PsodaPrinter::getInstance()->write("\n");
		}
		lastprint = now;
		mUpdated = false;
	}
}

QTree* QSectorStepwiseAdditionSearch::partialTree(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,QDeque<int> taxa)
{
	list<QTree*> trees;
	list<QNodeInfo *> nodeInfoList;
	list <QNodeInfo *>::iterator nodeInfoIter;
    QNodeInfo *nodeInfo;
	int index;
	
	PsodaPrinter* printBuffer = PsodaPrinter::getInstance();

	QTree *tree = new QTree();
	taxa.randomize(); //taxa may be randomized already, but they may be selected systematically
	nodeInfoList.clear();
	//Create Nodeinfo structures for all given leaves
	for(index = 0;index <taxa.size();index++)
	{
		nodeInfo = new QNodeInfo(taxa[index],ISLEAF);
		nodeInfoList.push_front(nodeInfo);
		tree->nnodes()++;
	}
	
	// Start at the beginning of the list
        nodeInfoIter = nodeInfoList.begin();

        // Get the first 3 NodeInfo entries
        QNodeInfo *nodeInfo1 = *nodeInfoIter++; // Get the next value from the iterator
        tree->insertNodeInfo(nodeInfo1); // Insert the internal node on the tree list
        QNodeInfo *nodeInfo2 = *nodeInfoIter++; // Get the next value from the iterator
        tree->insertNodeInfo(nodeInfo2); // Insert the internal node on the tree list
        QNodeInfo *nodeInfo3 = *nodeInfoIter++; // Get the next value from the iterator
        tree->insertNodeInfo(nodeInfo3); // Insert the internal node on the tree list

        // Create 3 Nodes and hook them up to the NodeInfos
        QNode *node1 = new QNode(*nodeInfo1,ISLEAF);
        nodeInfo1->node() = node1;
        QNode *node2 = new QNode(*nodeInfo2,ISLEAF);
        nodeInfo2->node() = node2;
        QNode *node3 = new QNode(*nodeInfo3,ISLEAF);
        nodeInfo3->node() = node3;

        // Now create an internal node and hook everyone to it
        QNodeInfo *internalNodeInfo = new QNodeInfo(index++, ISNTLEAF);
        tree->insertNodeInfo(internalNodeInfo); // Insert the internal node on the tree list
        QNode *internalNode = new QNode(*internalNodeInfo, ISNTLEAF); 
        internalNodeInfo->node() = internalNode;
        tree->nnodes()++;

        internalNode->connectExternal(internalNode->internal1(), node2); // Connect the first child
        internalNode->connectExternal(internalNode->internal2(), node3); // Connect the second child
        internalNode->connectExternal(internalNode->internal3(), node1); // Connect the third child

        // Give it a root
        tree->root() = internalNode;
		
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
            tree->insertNodeInfo(nodeInfo); // Insert the new node on the tree list
            // We dont need to increment nnodes because we did this previously

            // Set the score to the maximum value
            tree->resetScore();
			tree->subTree1() = tree->root();
            tree->subTree2() = node;
            // Then kick off the copy
            tree->makeNodeLists();

            // Tell TBR that it is already split so he wont try to split it at the next split point
            tree->isSplit() = true;
            tree->earlyPrune() = INT_MAX; // Get rid of any early prune data

            // Now find the best rearrangement
            tree->nodeDataInitialized() = false;

            trees.clear();
			qsearchAlgorithm->rearrange(tree, evaluator, MAXTREES, MAXEQUAL, DBL_MAX, false, trees); // Last parameter is whether to create a new tree or reuse the current one
            QTree *treep;
            treep = trees.front();
			tree = treep; // Now use the new tree
			printBuffer->write(".");
						if((dotiteration++%80) == 79) {
              printBuffer->write("\n");
						}
            nodeInfoIter++;
        }
		return tree;
} 

void QSectorStepwiseAdditionSearch::BuildSubTrees(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator)
{
	//Divide taxa into groups of 50-100 taxa
	//How many groups?
	mNSubTrees = 1;
	float subtreeSize = Interpreter::getInstance()->dataset()->ntaxa();
	while(subtreeSize > MIN_SECTOR * 2)
	{
		mNSubTrees *=2;
		subtreeSize /= 2;
	}
	//Randomize taxa (This could perhaps be replaced with a NJ tree or some other clustering)
	QDeque<int> vi;
	QDeque<int> sub_vi;
    for (int i = 0; i < Interpreter::getInstance()->dataset()->ntaxa(); i++)
    {
        vi.push_back(i);
    }
	vi.randomize();
	mSubScores = new double[mNSubTrees];
	mSubTree = new QTree*[mNSubTrees];
	//For each group build a stepwise tree
	for(int i=0;i<mNSubTrees;i++)
	{
		sub_vi.clear();
		//add subtreeSize taxa (or all remaining taxa if at end)
		for(int j=((int)subtreeSize)*i;j<((int)subtreeSize)*(i+1);j++)
		{
			sub_vi.push_back(vi[j]);
		}
		if(i==mNSubTrees-1)
		{
			for(int j=((int)subtreeSize)*(i+1);j<Interpreter::getInstance()->dataset()->ntaxa();j++)
			{
				sub_vi.push_back(vi[j]);
			}
		}
		
		mSubTree[i] = partialTree(qsearchAlgorithm,evaluator,sub_vi);
		mSubScores[i] = mSubTree[i]->getScore();
	}
}

QTree* QSectorStepwiseAdditionSearch::doTBR(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,QTree* currentTree, double& bestScore,bool keepCurTree)
{
	list<QTree*> trees;
	
    QTree* newTree = NULL;
	
		// Perform the search algorithm.  Since you dont return the same tree that was
        // passed in, you may not return any trees that are better than or equal to the current tree
        // the second to last parameter tells the algorithm to create a new tree 

        trees.clear();
		qsearchAlgorithm->rearrange(currentTree, evaluator, MAXTREES, MAXEQUAL, bestScore, true, trees);
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

void QSectorStepwiseAdditionSearch::refine(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,int index)
{
	//PsodaPrinter::getInstance()->write("Refining %d\n",index);
	while(1)
	{
		if(!mSubTree[index]->split()) //All splits have been searched
		{
			return;
		}
		mSubTree[index] = doTBR(qsearchAlgorithm,evaluator,mSubTree[index],mSubScores[index],true);
		mSubScores[index] = mSubTree[index]->getScore();
		DisplayScore();
	}
}

void QSectorStepwiseAdditionSearch::join(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,int i,int j)
{
	
	NewickTreeParser ntp;
	list<QTree*> trees;
	QDeque<QNodeInfo*>::iterator nodeInfoIter;
	//PsodaPrinter::getInstance()->write("Joining %d and %d\n",i,j);
	// add all nodes from j to i
	for(nodeInfoIter = mSubTree[j]->nodeInfoList()->begin();nodeInfoIter != mSubTree[j]->nodeInfoList()->end();nodeInfoIter++)
	{
		mSubTree[i]->insertNodeInfo(*nodeInfoIter);
	}
	//set up subtrees
	mSubTree[i]->subTree1() = mSubTree[i]->root();
	mSubTree[i]->subTree2() = mSubTree[j]->root();
	mSubTree[i]->makeNodeLists();
	mSubTree[i]->isSplit() = true;
	mSubTree[i]->earlyPrune() = INT_MAX;
	
	//use TBR to join
	trees.clear();
	qsearchAlgorithm->rearrange(mSubTree[i], evaluator, MAXTREES, MAXEQUAL, DBL_MAX, true, trees); // Last parameter is whether to create a new tree or reuse the current one
	QTree* qt = trees.front();
	
	//PsodaPrinter::getInstance()->write("%f+%f = ",mSubScores[i],mSubScores[j]);
	mSubScores[i] = mSubTree[i]->getScore();
	
	delete(mSubTree[i]);
	mSubTree[i] = ntp.parseBuffer(qt->treeStr(), strlen(qt->treeStr()), false);
	//PsodaPrinter::getInstance()->write("%f\n",mSubScores[i]);
	//clean up
	mSubTree[j] = NULL;
	mNSubTrees--;
	
	mUpdated = true;
}
 
void QSectorStepwiseAdditionSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int nreps)
{
	NewickTreeParser ntp;
	QTree* qt;
    mStartTime = When();
	BuildSubTrees(qsearchAlgorithm,evaluator);
	DisplayScore();
	//For each tree, refine with TBR
	for(int i=0;i<mNSubTrees;i++)
		refine(qsearchAlgorithm,evaluator,i);
	//Join with TBR
	int size = 1;
	while(mNSubTrees>1)
	{
		size*=2;
		int half = mNSubTrees/2;
		for(int i=0;i<half;i++)
		{
			join(qsearchAlgorithm,evaluator,i,mNSubTrees-1);
			refine(qsearchAlgorithm,evaluator,i);
			/*
			if(size>3)
			{
				
			//PsodaPrinter::getInstance()->write("Ratchet\n");
				Interpreter::getInstance()->dataset()->randomizeWeights(0.15);
				qt = mSubTree[i];
				mSubTree[i] = ntp.parseBuffer(qt->treeStr(), strlen(qt->treeStr()), false);
				
				refine(qsearchAlgorithm,evaluator,i);
				
			//PsodaPrinter::getInstance()->write("Reset\n");
				Interpreter::getInstance()->dataset()->resetWeights();
				
				qt = mSubTree[i];
				mSubTree[i] = ntp.parseBuffer(qt->treeStr(), strlen(qt->treeStr()), false);
				refine(qsearchAlgorithm,evaluator,i);
			}
			*/
		}
		
	}
	
	
	
	qtreeRepository.addTree(mSubTree[0]);

} 



