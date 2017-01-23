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
#include "DPTBR.h"
#include "QTBR.h"


DPTBR::DPTBR()
{
}

DPTBR::~DPTBR()
{
}

const char* DPTBR::name() const
{
	return "Dynamic Programming TBR";
}


void DPTBR::search(QTreeRepository & qtreeRepository , QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int nreps __attribute__((unused)))
{
	Dataset* dataset = Interpreter::getInstance()->dataset();
	QDeque<int> vi;
	QTree*** DParray;
	int i,j,k;
	int ntaxa = dataset->ntaxa();
	double rearrangments = 0;
	QNode* node1;
	QNode* node2;
	QNode* node3;
	QNode* internalNode;
	
	QNodeInfo* nodeInfo1;
	QNodeInfo* nodeInfo2;
	QNodeInfo* nodeInfo3;
	QNodeInfo* internalNodeInfo;
	
    for (i = 0; i < ntaxa; i++)
    {
        vi.push_back(i);
    }
	
	vi.randomize(); // Called with no arguments causes the QDeque random number generator to be seeded by the clock

	DParray = (QTree***) malloc(sizeof(QTree**) * ntaxa);
	for (i = 0; i < ntaxa; i++)
    {
		DParray[i] = (QTree**) malloc(sizeof(QTree*) * ntaxa);
	} 
	
	
	for (i = 0; i < ntaxa; i++)
    for (j = 0; j < ntaxa; j++)
    {
		DParray[i][j] = NULL;
	}
	
	for(j=0;j<ntaxa;j++)
	{
		for(i=j;i>=0;i--)
		{
			//printf("Tree for taxa %d - %d\n",i,j);
			if(j-i < 3)
			{
				DParray[i][j] = new QTree();
				switch(j-i)
				{
					case 0:
						nodeInfo1 = new QNodeInfo(vi[i],ISLEAF);
						DParray[i][i]->root() = new QNode(*nodeInfo1,ISLEAF);
						nodeInfo1->node() = DParray[i][i]->root();
						DParray[i][i]->root()->external() = NULL;
						DParray[i][i]->insertNodeInfo(nodeInfo1);
						//Initialize parsimony data
						
						break;
					case 1:
						nodeInfo1 = new QNodeInfo(vi[i],ISLEAF);
						node1 = new QNode(*nodeInfo1,ISLEAF);
						nodeInfo1->node() = node1;
						
						nodeInfo2 = new QNodeInfo(vi[j],ISLEAF);
						node2 = new QNode(*nodeInfo2,ISLEAF);
						nodeInfo2->node() = node2;
						
						node1->external() = node2;
						node2->external() = node1;
						
						DParray[i][j]->root() = node1;
						DParray[i][j]->insertNodeInfo(nodeInfo1);
						DParray[i][j]->insertNodeInfo(nodeInfo2);
						//Initialize parsimony data
						break;
					case 2:
						nodeInfo1 = new QNodeInfo(vi[i],ISLEAF);
						node1 = new QNode(*nodeInfo1,ISLEAF);
						nodeInfo1->node() = node1;
						
						nodeInfo2 = new QNodeInfo(vi[i+1],ISLEAF);
						node2 = new QNode(*nodeInfo2,ISLEAF);
						nodeInfo2->node() = node2;
						
						nodeInfo3 = new QNodeInfo(vi[j],ISLEAF);
						node3 = new QNode(*nodeInfo3,ISLEAF);
						nodeInfo3->node() = node3;
						
						internalNodeInfo = new QNodeInfo(ntaxa,ISNTLEAF);
						internalNode = new QNode(*internalNodeInfo,ISNTLEAF);
						
						internalNode->connectExternal(internalNode->internal1(), node2); // Connect the first child
						internalNode->connectExternal(internalNode->internal2(), node3); // Connect the second child
						internalNode->connectExternal(internalNode->internal3(), node1); // Connect the third child
						
						DParray[i][j]->root() = internalNode;
						
						
						DParray[i][j]->insertNodeInfo(nodeInfo1);
						DParray[i][j]->insertNodeInfo(nodeInfo2);
						DParray[i][j]->insertNodeInfo(nodeInfo3);
						DParray[i][j]->insertNodeInfo(internalNodeInfo);
						//Initialize parsimony data
						break;
				}
				evaluator->qscoreTree(DParray[i][j]);
				//DParray[i][j]->updateTreeString();
				//printf("\tSubtree (%d,%d) with score %f\n\t",i,j,DParray[i][j]->getScore());
				//DParray[i][j]->print();
			}
			else
			{
			int best_score = INT_MAX;
				for(k=i;k<j;k++)
				{
					//Check DParray[i][k] + DParray[k+1][j]
					if(DParray[i][k]->getScore() + DParray[k+1][j]->getScore() < best_score)  //early cutout
					{
						QTree* tree = new QTree();
						tree->root() = DParray[i][k]->root();
						tree->subTree1() = tree->root();
						tree->subTree2() = DParray[k+1][j]->root();

						
						AddToNodeInfoList(tree,tree->subTree1());
						AddToNodeInfoList(tree,tree->subTree1()->external());
						AddToNodeInfoList(tree,tree->subTree2());
						AddToNodeInfoList(tree,tree->subTree2()->external());
						
						
						tree->isSplit() = true;
						tree->makeNodeLists();
						tree->earlyPrune() = best_score;
						tree->nodeDataInitialized() = false;
						list<QTree*> trees;
						//QTBR* qsearchAlgorithm = new QTBR();
						tree->updateTreeString();  //This is needed or CreateSpecific Tree in QTBR has a bus error
						qsearchAlgorithm->rearrange(tree, evaluator, 1, 1, DBL_MAX, true, trees); // Last parameter is whether to create a new tree or reuse the current one
						rearrangments += qsearchAlgorithm->rearrangements();
						tree = trees.front();  

						evaluator->qscoreTree(tree);

						if(tree->getScore() < best_score)
						{
							if(DParray[i][j])
							{
								delete(DParray[i][j]);
								//delete current tree
							}
							DParray[i][j] = tree;
						}
						else
						{
							delete(tree);
						}
					}
					else
					{
						printf("Early cutout\n");
					}
							
				}
				//DParray[i][j]->updateTreeString();
				//printf("\tSubtree (%d,%d) with score %f\n\t",i,j,DParray[i][j]->getScore());
				//DParray[i][j]->print();
			}
		}
	}
	printf("Tree found with score %.2f\n",DParray[0][ntaxa-1]->getScore());
	
	DParray[0][ntaxa-1]->updateTreeString();
	DParray[0][ntaxa-1]->print();
	
	printf("%f TBR rearrangments tried\n",rearrangments);
	//qtreeRepository.removeAll();
	printf("Adding to repository\n");
	qtreeRepository.addTree(DParray[0][ntaxa-1]);
	printf("Done Adding to repository\n");
	return;
} 


void DPTBR::AddToNodeInfoList(QTree* tree, QNode* root)
{
	if(root)
	{
		tree->insertNodeInfo(root->nodeInfo());
		if(!root->nodeInfo()->leaf())
		{
			AddToNodeInfoList(tree,root->child1());
			AddToNodeInfoList(tree,root->child2());
		}
	}
}
