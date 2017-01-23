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
#include "TreeFusion.h"
#include "QStep.h"
#include "QTBR.h"
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
#include "ResolutionTree.h"

#include "CommonCladeRefinement.h"

#define TF_REPLICATES 1

TreeFusionSearch::TreeFusionSearch()
{
}

TreeFusionSearch::~TreeFusionSearch()
{
}

const char* TreeFusionSearch::name() const
{
	return "Tree Fusion Search";
}


void TreeFusionSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int nreps)
{
	QStepwiseAdditionSearch stepwise;
	QTreeRepository trees[TF_REPLICATES];
	QTBR search_algorithim;
	double bestScore[TF_REPLICATES];
	int i;
	int now=0, lastmenu=0, lastprint=0;
	QTree* currentTree;
	list<QTree*> tree_list;
	for(i=0;i<TF_REPLICATES;i++)
	{
		//initialize each repository with a stepwise tree
		stepwise.search(trees[i],&search_algorithim,evaluator,1);
		//printf("%d has %d trees\n",i,trees[i].numTrees());
		bestScore[i] = DBL_MAX;
	}
	mStartTime = When();
	now=(int)When();
	//while(!gQuit&&(now=When())-mStartTime>60)
	while((now-mStartTime)<60)
	{
		//printf("Considering each respository (%d sec left)\n",now-mStartTime);
		for(i=0;i<TF_REPLICATES;i++) //consider each repository seperately
		{
			//printf("\tRepo %d (%d trees)\n",i,trees[i].numTrees());
			currentTree = trees[i].getSearchableTree();
			if(currentTree == NULL) //this repository has finished
			{
				continue;
			}
			//printf("Got tree from repository %d\n",i);
#undef SECT_SEARCH
#ifndef SECT_SEARCH
			while(1)
			{
				if(!currentTree->split()) //all splits have been searched
				{
					currentTree->visited() = true;
					trees[i].addTree(currentTree);
					break; //move on to next repository
				}
				tree_list.clear(); //clear tree list for TBR
				int currentSpace = trees[i].maxTrees()-trees[i].numTrees();
				search_algorithim.rearrange(currentTree,evaluator,trees[i].maxTrees(),currentSpace,bestScore[i],true,tree_list);
				currentTree->join();
				if(tree_list.size() == 0)
				{
					continue;	//try a different split point
				}
				QTree* newTree = tree_list.front();  // Get a tree out of the list
				tree_list.pop_front();
				if(newTree->getScore() < bestScore[i])
				{
					bestScore[i] = newTree->getScore();
					trees[i].removeAll(); //nuke this repository
					trees[i].addTree(newTree);
					
					while(tree_list.size())
					{
						newTree = tree_list.front();
						tree_list.pop_front();
						trees[i].addTree(newTree);
					}
					delete currentTree;
					break;  //move on to next repository
				}
				while(tree_list.size())
				{
					newTree = tree_list.front();
					tree_list.pop_front();
					trees[i].addTree(newTree);
				}
			}
#else
			while(1)
			{
				QTree* rss = new QTree(currentTree);
				//rss->join();
				printf("Selecting node to unresolve\n");
				//choose node to unresolve
				QNode* root = rss->root();
				for(int j=0;j<50;j++)
				{
					double rand_val = (double)rand()/INT_MAX;
					if(rand_val < 0.33)
						root = root->external();
					else if(rand_val < 0.66)
						root = root->child1();
					else
						root = root->child2();
				}
				root->nodeInfo()->setLabel("Unresolve here");
				//unresolve randomly
				printf("Unresolving Node\n");
				int degree = 3;
				while(degree < 5)
				{
					printf("current degree = %d\n",degree);
					int child = rand()%degree;
					QNode* child_p = root;
					for(int k=0;k<child;k++)
						child_p = child_p->internal1();
					QNode* child_rem = child_p->internal1();
					QNode* child_next = child_rem->internal1();
					QNode* child_orig = child_rem;
					while(child_rem->external()->nodeInfo()->leaf()||child_rem==root)
					{
						printf(".");
						fflush(NULL);
						child_p = child_p->internal1();
						child_rem = child_rem->internal1();
						child_next = child_next->internal1();
						if(child_rem==child_orig)
							break;	//the tree is too small
					}
					child_rem->external()->nodeInfo()->setLabel("Remove this branch");
					printf("Removing selected branch\n");
					
					child_p->internal1() = child_rem->external()->internal1();
					child_rem->external()->internal2() = child_next;
					for(QNode* cur = child_p->internal1();cur!=child_next;cur=cur->internal1())
					{
						cur->nodeInfo() = root->nodeInfo();
					}
					//printf("Deleting unused QNodes\n");
					//delete child_rem->external();
					//delete child_rem;
					degree++;
				
				}
				rss->showTree();
				exit(0);
				printf("Resolving Node\n");
				ResolutionTree* rt = new ResolutionTree(rss,true);
				QTree* qt = rt->getQTree();
				delete rt;
				delete rss;
				double score = evaluator->qscoreTree(qt);
				if(score < bestScore[i])
				{
					bestScore[i] = score;
					trees[i].addTree(qt);
					delete rss;
					break;
				}
				delete qt;
				
			}
#endif
		}
		
		now=(int)When();
		//display
		if((now-lastmenu)>100)
		{
			lastmenu = now;
			PsodaPrinter::getInstance()->write("\nTime\t\t\t\t\tBest Score in Repository\n");
			PsodaPrinter::getInstance()->write("Elapsed       Rearr.\t");
			for(i=0;i<TF_REPLICATES;i++)
				PsodaPrinter::getInstance()->write("%d\t",i+1);
			PsodaPrinter::getInstance()->write("\n");
			PsodaPrinter::getInstance()->write("---------------------------------------------------------------------------------------------------------\n");
		}
		if((now-lastprint)>10)
		{
			lastprint = now;
			int seconds = (int) (When()-mStartTime);
            int hours = seconds / (60*60);
            int minutes = (seconds / 60)%60;
            seconds = seconds % 60;
			PsodaPrinter::getInstance()->write("%02i:%02i:%02i %12.0f\t",hours,minutes,seconds,search_algorithim.rearrangements());
			for(i=0;i<TF_REPLICATES;i++)
				PsodaPrinter::getInstance()->write("%.0f\t",bestScore[i]);
			PsodaPrinter::getInstance()->write("\n");
		}
		
		
	}
	
	//Tree Fuse into repository 0
	
		for(int source = 1;source<TF_REPLICATES;source++)
		{
			printf("Fusing %d\n",source);
			currentTree=trees[source].getSearchableTree();
			if(currentTree!=NULL)
			{
				QTree* qt = new QTree(currentTree);
				trees[0].addTree(qt);
				trees[source].addTree(currentTree);
				CommonCladeRefinement ccr;
				ccr.search(trees[0],qsearchAlgorithm,evaluator,nreps);
			}
		}
	
	//put best trees in given repository
	double bestScoreAll = DBL_MAX;
	for(i=0;i<TF_REPLICATES;i++)
	{
		if(bestScore[i] < bestScoreAll)
		{
			bestScoreAll = bestScore[i];
			qtreeRepository.removeAll();
			while((currentTree=trees[i].getSearchableTree())!=NULL)
			{
				qtreeRepository.addTree(currentTree);
			}
		}
		
	}
	
	gQuit=false;
}

