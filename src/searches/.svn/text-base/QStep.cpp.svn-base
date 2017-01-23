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
#include "QStep.h"
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

#undef COMMON_CLADE_REFINED_STEPWISE
//COMMON_CLADE_REFINED_STEPWISE doesn't appear to help
	
#ifdef COMMON_CLADE_REFINED_STEPWISE

#include "CommonCladeRefinement.h"
#endif

QStepwiseAdditionSearch::QStepwiseAdditionSearch()
{
}

QStepwiseAdditionSearch::~QStepwiseAdditionSearch()
{
}

const char* QStepwiseAdditionSearch::name() const
{
	return "Stepwise Addition";
}

// In order to debug scoring, this code used the same order as the old TBR
#ifdef notdef
int taxaOrder[1000];
double taxaScore[1000];
#endif
/**
 * Perform the stepwise search:
 * 1) Create NodeInfo structures for all of the taxa and put them in a linked list.  
 *   Randomly assign taxa from the file to these structures
 * 2) Add the taxa in NodeInfo order (random order)
 * 3) Use TBR to find the optimal addition place
 *
 * This routine needs the Dataset so it can add the nodes to the tree.  Other searches shouldnt need this ...
 */
void QStepwiseAdditionSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int nreps)
{
    list<QTree*> trees;
    QNodeInfo *nodeInfo;
    //int randval;
    int index;
    list<QNodeInfo *> nodeInfoList;  // STL list of NodeInfo Objects for all taxa
    list <QNodeInfo *>::iterator nodeInfoIter; // Iterator to use in acccessing list
    double bestScore = DBL_MAX;
	PsodaPrinter* printBuffer = PsodaPrinter::getInstance();
    Dataset* dataset = Interpreter::getInstance()->dataset();

    if(dataset->ntaxa() < 4) {
        printBuffer->write("Stepwise Search unnecessary with less than 4 taxa\n");
        exit(-1);
    }

    // First create a vector of all of the leaf numbers
    using std::vector;
    using std::random_shuffle;
    //vector<int> vi;
	QDeque<int> vi;
    for (int i = 0; i < dataset->ntaxa(); i++)
    {
        vi.push_back(i);
    }
	
    // Repeat for number of replicates
    while( nreps--) {
        QTree *tree = new QTree();

        //random_shuffle(vi.begin(), vi.end()); /* shuffle elements */

	vi.randomize(); // Called with no arguments causes the QDeque random number generator to be seeded by the clock

        // Clear the list so we start clean
        nodeInfoList.clear();
        // Now create Nodeinfo structures for all the leaves in random order
        for(index = 0; index < dataset->ntaxa(); index++) {
            nodeInfo = new QNodeInfo(vi[index], ISLEAF);
            {
                nodeInfoList.push_front(nodeInfo);
            }
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

#ifdef notdef
        tree->print();
#endif
				int dotiteration = 0;
        // each iteration adds an additional taxa to the tree
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

            //printf("adding %d\n",nodeInfo->nodeIndex());
#ifdef notdef
#endif
            // Copy all of the existing nodes to subtree Node lists
            // First initialize the subtrees
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
#define MAXTREES 1
#define MAXEQUAL 0
	    //printf("Size before rearrange %d\n",trees.size());
            qsearchAlgorithm->rearrange(tree, evaluator, MAXTREES, MAXEQUAL, DBL_MAX, false, trees); // Last parameter is whether to create a new tree or reuse the current one
            QTree *treep;
            treep = trees.front();  // Get the only tree out of the list
            //printf("Size before pop %d\n",trees.size());
	    //TODO .pop_front calls the destructor of the poped element, this is probably not right
	   
	    //trees.pop_front(); // Remove it from the list so it wont be destroyed when we delete the list
            //printf("Size after pop %d\n",trees.size());
	    // Only delete the tree if we passed true to rearrange to have him create a new tree
            tree = treep; // Now use the new tree
            printBuffer->write(".");
						if((dotiteration++%80) == 79) {
              printBuffer->write("\n");
						}
            nodeInfoIter++;
        }
		if(tree->getScore() < bestScore)
		{
			printBuffer->write("\n Stepwise Complete: rearrangements %f score %f best %f\n", qsearchAlgorithm->rearrangements(), tree->getScore(),tree->getScore());
		}
		else
		{
			printBuffer->write("\n Stepwise Complete: rearrangements %f score %f best %f\n", qsearchAlgorithm->rearrangements(), tree->getScore(),bestScore);
		}
#ifndef COMMON_CLADE_REFINED_STEPWISE
        // tree->print();
        // Now put the best tree into the tree repository if it is better
        if(tree->getScore() < bestScore)
        {
            // nuke the tree repository
            qtreeRepository.removeAll();
			
            // update the best tree
            bestScore = tree->getScore();
            // add the better tree
            qtreeRepository.addTree(tree);
        }
        else
        {
            delete(tree);
        }
#else
		if(tree->getScore() < bestScore)
        {
            // update the best tree
			
            //bestScore = tree->getScore();
			bestScore = evaluator->qscoreTree(tree);
		}
		//add tree to repository
		qtreeRepository.addTree(tree);
#endif
    }
	
#ifdef COMMON_CLADE_REFINED_STEPWISE
	CommonCladeRefinement ccr;
	printf("Stepwise Best Tree %.0f\n",bestScore);
	ccr.search(qtreeRepository,qsearchAlgorithm,evaluator,nreps);
#endif
} 

