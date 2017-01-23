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
#include <stdio.h>
#include "QTBR.h"
#include "Interpreter.h"
#include "QNode.h"
#include "QTree.h"
#include "Dataset.h" 

using namespace std;

QTBR::QTBR()
{
    mRearrangements = 0;
}

QTBR::~QTBR()
{
}

/**
 * Split the tree, then try all of the possible join points on both subtrees.
 * Returns an array of tree pointers of all the most optimal trees found
 */
void QTBR::rearrange(QTree* startTree, EvaluatorBase  *eval, int numtrees, int numequal, double bestScore, bool saveOrigTree, list<QTree*>& treeList)
{
    double bScore=bestScore;
    QNodeInfo *nodemin = NULL;
    QNodeInfo *node1, *node2;
    vector<pair<QNodeInfo *,QNodeInfo *> > jPoints;
    int localRearrangements = 0;
    //bool quitBetter = false;
    int niter = 0;

	//Dataset* dset = Interpreter::getInstance()->dataset();

    // If there were no available split points return 
    if(!startTree->isSplit())  // Stepwise will pass us a tree that is already split 
    {
        if(!startTree->split()) 
        {
	  // Return an empty list if there are no more split points left to explore
	  return;
        }
    }

    // Build the Preprocess data in the NodeInfo structure
    eval->qpreProcess(startTree);

    startTree->initNodeInfoIter1(); // Initialize the iterator
    while ((node1 = startTree->nextNodeInfo1()) != NULL)
    {
        startTree->initNodeInfoIter2(); // Initialize the iterator
        while ((node2 = startTree->nextNodeInfo2()) != NULL)
        {
            double newTreeScore = 0 ;
            // This will actually always be the first pair of nodeinfos, but just in case we change something, check
            if((node1 == startTree->subTree1()->nodeInfo()) && (node2 == startTree->subTree2()->nodeInfo()))
            {
                // This was the original tree, so dont score it
                continue;
            }
            niter++;
			//printf("Calling qevaluate\n");
            newTreeScore = eval->qevaluateTree(startTree, node1, node2);
#ifdef notdef
			// print tree string of all examined trees
			QTree *test_tree = startTree->createSpecificTree(node1,node2, true);
			printf("PSSS*: %s\n",test_tree->treeStr());
			delete test_tree;
#endif
			
			//printf("qevaluate result %d =  %f - %f\n",niter,newTreeScore,bScore);
			
            if((AlgorithmFlags & PRINTALLTBR)&&(saveOrigTree))
                printf("+ %f\n",newTreeScore);
#ifdef notdef
// Debug Score by rescoring each one
QTree *test_tree = startTree->createSpecificTree(node1,node2, saveOrigTree);

double treeScore = eval->qscoreTree(test_tree);
printf("\nRescore %f\n",treeScore);
if(treeScore != newTreeScore)
{
    printf("Score DIFFERENCE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! swap %f, rescore %f best %f\n",newTreeScore,treeScore,bScore);
    startTree->print();
printf("canonicalizing test\n");
    test_tree->canonicalize();
    test_tree->updateTreeString();
    test_tree->print();
}
delete test_tree;

// END Debug Score by rescoring each one
#endif

            // We need to add an optimal score to the list
            if ((newTreeScore >= 0) &&(newTreeScore <= bScore))
            {
                if (newTreeScore < bScore)
                {
#ifdef QUITONBETTER
                    // I think we should return here.  There is no reason to continue searching a suboptimal tree
                    // But this seemed to make things a bit slower
                    if(saveOrigTree)
                        quitBetter = true;
#endif
//                    printf("found a better score was %f new %f\n",bScore,newTreeScore);
                    bScore = newTreeScore;
                    jPoints.clear();
                }
                pair<QNodeInfo *,QNodeInfo *> temp;
                nodemin = node1;
                temp.first = node1;
                temp.second = node2;
                // If the score was better, then we want to add up to numtrees to the repository, if it was equal, then 
                //   we add at most numequal trees 
                if (newTreeScore < bestScore)
                {
                    if(jPoints.size() < (unsigned int)numtrees)
                        jPoints.push_back(temp);
                }
                else // The score was equal
                {
                    if(jPoints.size() < (unsigned int)numequal)
                        jPoints.push_back(temp);
                }
            }
#ifdef notdef
           printf("\n");
#endif
            localRearrangements++;
#ifdef QUITONBETTER
            // If we found a better tree, quit and search it
            if(quitBetter)
                break;
#endif
        }
        // If we found a better tree, quit and search it
#ifdef QUITONBETTER
        if(quitBetter)
            break;
#endif
    }
    if((AlgorithmFlags & PRINTALLTBR)&&(saveOrigTree))
        printf("QTBR niter %d\n",niter);

    // Accounting 
	rearrangements() += localRearrangements;

    vector<pair<QNodeInfo *,QNodeInfo *> >::iterator it = jPoints.begin();
    while (it!=jPoints.end())
    {
//        printf("Q joining tree first %d, second %d score %f\n",(*it).first->nodeIndex(),(*it).second->nodeIndex(),bScore);
#ifdef notdef
#endif
#ifdef TESTFORMEMORYLEAK
        if(saveOrigTree)
        {
            for(int i = 0; i < 1000000000; i++)
            {
                QTree *tree = startTree->createSpecificTree((*it).first,(*it).second, saveOrigTree);
                delete(tree);
            }
            exit(0);
        }
#endif
        QTree *tree = startTree->createSpecificTree((*it).first,(*it).second, saveOrigTree);
		tree->setScore(bScore);
//	printf("Tree * = %x\n",tree);
        // add the tree to the list
        treeList.push_back(tree);
        it++;
    }
    return;
}

