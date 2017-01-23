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
#include "CommonCladeRefinement.h"
#include "PsodaPrinter.h"
#include "Interpreter.h"
#include "Dataset.h"
#include "PsodaPrinter.h"

#include <sys/time.h>
#include <sys/types.h>
#include <float.h>
#include <math.h>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#define sleep(s) Sleep(s*1000)
#endif
#include "Globals.h"
#include "NewickTreeParser.h"

CommonCladeRefinement::CommonCladeRefinement():CladeMap() 
{
}

const char* CommonCladeRefinement::name() const
{
	return "Common Clade Refinement";
}

/**
 * Perform the Retained Results Search
 */
void CommonCladeRefinement::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm  __attribute__((unused)),  EvaluatorBase *evaluator, int iterations __attribute__((unused)))
{
	mNtaxa = Interpreter::getInstance()->dataset()->ntaxa();
	QTree* currentTree = NULL;
	// Clear the visited status on the repository.  We may be scoring using a new metric.  This actually deletes all of the trees and creates new ones.
    qtreeRepository.clearVisited();

	//Clear the map
	CladeMap.clear();

	//printf("Building CCR map\n");
	
	double best_score = DBL_MAX;
	int* weights = NULL;
	int nchar;
	//build map of clades
	for(int i=0;i<qtreeRepository.numTrees();i++)
	{

		currentTree = qtreeRepository.popTree();
		if(!weights)
		{
			weights = currentTree->weights();
			nchar = currentTree->nchars();
		}
		currentTree->split();
		currentTree->join();
		
		evaluator->qpreProcess(currentTree);
		if(currentTree->root()->nodeInfo()->nodeData()->score(PSDI_FINAL) < best_score)
			best_score = currentTree->root()->nodeInfo()->nodeData()->score(PSDI_FINAL);
		/*
		double cur_score = evaluator->qscoreTree(currentTree);
		printf("cur_score = %.0f\n",cur_score);
		if(cur_score < best_score) best_score = cur_score;
		*/
		AddTreeToMap(currentTree);
		qtreeRepository.addTree(currentTree);
	}
	checkPausePSODA();
	//printf("Examining Map\n");
	//examine each tree for common clades
	//printf("%.0f = best score\n",best_score);
	int common_partition = 0;
	for(map<Partition,list<QNode*> >::iterator iter = CladeMap.begin();iter != CladeMap.end();iter++)
	{
		common_partition++;
		if((*iter).second.size() > 1) //only interested in partitions that exist in multiple trees
		{
			//printf("Checking partition %d of %d in %d trees",(int)common_partition,CladeMap.size(),(int)(*iter).second.size());
			for(list<QNode*>::iterator node_iter1 = (*iter).second.begin();node_iter1 != (*iter).second.end();node_iter1++)
			{
				for(list<QNode*>::iterator node_iter2 = node_iter1;node_iter2 != (*iter).second.end();node_iter2++)
				{
					checkPausePSODA();
					ConsiderCombination(*node_iter1,*node_iter2,qtreeRepository,nchar,best_score,evaluator);
					ConsiderCombination((*node_iter1)->external(),*node_iter2,qtreeRepository,nchar,best_score,evaluator);
					ConsiderCombination(*node_iter1,(*node_iter2)->external(),qtreeRepository,nchar,best_score,evaluator);
					ConsiderCombination((*node_iter1)->external(),(*node_iter2)->external(),qtreeRepository,nchar,best_score,evaluator);
				}
			}
			//printf("\n");
		}
	}
} 


void CommonCladeRefinement::ConsiderCombination(QNode* subtree1,QNode* subtree2,QTreeRepository& qtreeRepository,int nchar,double& best_score,EvaluatorBase* evaluator)
{
static int dotiteration = 0;
static int iteration = 0;
//consider recombining part of tree 1 and tree 2
if( subtree1->nodeInfo()->nodeData()->score(PSDI_POST) == subtree2->nodeInfo()->nodeData()->score(PSDI_POST))
{
	iteration++;
	if(iteration%10==0)
	{
		PsodaPrinter::getInstance()->write(".");
		if((dotiteration++%80) == 79) 
		{
			PsodaPrinter::getInstance()->write("\n");
		}
	}
	return; //there is very little chance of this being better
}
//if(score < best_score )
{
	//printf("Possible tree - partial score = %.1f\t",score);
//	score += combine(subtree1->nodeInfo()->nodeData()->siteData(PSDI_POST), subtree2->nodeInfo()->nodeData()->siteData(PSDI_POST),nchar);
	//printf("total score = %.1f\n",score);
//	if(score < best_score)
	{
		//is the tree complete?
		//then update best score and create tree
		QTree* qt = BuildTree(subtree1,subtree2);
		if(qt != NULL)
		{
			double eval_score = evaluator->qscoreTree(qt); 
			//printf("%.0f\t%.0f\n",eval_score,best_score);
/*			if(eval_score != score)
			{ 
				printf("\nERROR: Eval score = %.1f, score = %.1f\n",eval_score,score);
				DebugTreeScore(subtree1,subtree2,nchar);
				printf("\nERROR: Eval score = %.1f, score = %.1f\n",eval_score,score);
				
				printf("Pre     = %.1f\nPost    = %.1f\nCombine = %d\n"
					,subtree1->nodeInfo()->nodeData()->score(PSDI_POST)
					,subtree2->nodeInfo()->nodeData()->score(PSDI_POST)
					,combine(subtree1->nodeInfo()->nodeData()->siteData(PSDI_POST), subtree2->nodeInfo()->nodeData()->siteData(PSDI_POST),nchar));
				for(int i=0;i<nchar;i++)
				{
					if(!((subtree1->nodeInfo()->nodeData()->siteData(PSDI_POST))[i]&(subtree2->nodeInfo()->nodeData()->siteData(PSDI_POST))[i]))
					printf("[%x]\t[%x]\n",(subtree1->nodeInfo()->nodeData()->siteData(PSDI_POST))[i],(subtree2->nodeInfo()->nodeData()->siteData(PSDI_POST))[i]);
				}
			}
*/
			if(eval_score <= best_score)
			{
				best_score = eval_score;
				qt->setScore(eval_score);
				if(eval_score < best_score)
				{
					qtreeRepository.removeAll();
					printf("Better Tree Found %.1f\n",eval_score);
				}
				qtreeRepository.addTree(qt);
			}
			else
			{
				delete qt;
				qt = NULL;
			}
		}
		else
		{
			//printf("Incomplete Tree found %.1f\n",score);

		}
	}
}
}

void CommonCladeRefinement::DebugTreeScore(QNode* subtree1,QNode* subtree2,int nchar)
{
	char buf1[10000];
	char buf2[10000];
	double score1 = DebugTreeScoreRecursive(subtree1,buf1,nchar);
	double score2 = DebugTreeScoreRecursive(subtree2,buf2,nchar);
	double score = combine(buf1,buf2,nchar);
	printf("Tree should have score %.1f\n",score+score1+score2);
}

double CommonCladeRefinement::DebugTreeScoreRecursive(QNode* root,char* return_buf,int nchar)
{
	char buf1[10000];
	char buf2[10000];
	if(root->nodeInfo()->leaf())
	{
		int leaf_index = root->nodeInfo()->nodeIndex();
		memcpy(return_buf,root->nodeInfo()->nodeData()->siteData(PSDI_POST),nchar*sizeof(char));
		return 0.0;
	}
	double score1 = DebugTreeScoreRecursive(root->child1(),buf1,nchar);
	double score2 = DebugTreeScoreRecursive(root->child2(),buf2,nchar);
	double score = combineDest(return_buf,buf1,buf2,nchar);
	
	//compare with post data
	for(int i=0;i<nchar;i++)
	{
		if((score+score1+score2) != root->nodeInfo()->nodeData()->score(PSDI_POST))
		{
			printf("ERROR: Different post score found (%.1f + %.1f +%.1f) %.1f != ",score,score1,score2,(score+score1+score2));
			printf("%.1f (%.1f pre) ",root->nodeInfo()->nodeData()->score(PSDI_POST),root->nodeInfo()->nodeData()->score(PSDI_PRE));
			score = combine(root->child1()->nodeInfo()->nodeData()->siteData(PSDI_POST),root->child2()->nodeInfo()->nodeData()->siteData(PSDI_POST),nchar);
			printf("= %.1f + %.1f + %.1f\n",score,root->child1()->nodeInfo()->nodeData()->score(PSDI_POST),root->child2()->nodeInfo()->nodeData()->score(PSDI_POST));
		}
		if(return_buf[i]!=root->nodeInfo()->nodeData()->siteData(PSDI_POST)[i])
		{
			printf("ERROR: Different post data found character %d, %x != %x\n",i,return_buf[i],root->nodeInfo()->nodeData()->siteData(PSDI_POST)[i]);
		}
	}
	
	return score1+score2+score;
}

void CommonCladeRefinement::AddTreeToMap(QTree* tree)
{
	AddNodeToMap(tree->root());
	AddNodeToMap(tree->root()->external());
}

Partition CommonCladeRefinement::AddNodeToMap(QNode* root)
{
	
	if(root->nodeInfo()->leaf())
	{
		int leaf_index = root->nodeInfo()->nodeIndex();
		return Partition(mNtaxa,leaf_index);
	}
	Partition left = AddNodeToMap(root->child1());
	Partition right = AddNodeToMap(root->child2());
	Partition center = left + right;

	
	CladeMap[center].push_front(root);
	return center;

}

int CommonCladeRefinement::combineDest(char* dest,char* src1, char* src2,int nchar)
{
	int differences = 0;
	int * weights = Interpreter::getInstance()->dataset()->weights();

	  for (int i = 0; i < nchar; i++)
	  {
		  if ( ! ( *dest = *src1 & *src2 ) )
		  {
			  differences += weights[i] ;
			  //differences ++;
			  *dest = *src1 | *src2 ;
		  }
		  dest++;
		  src1++;
		  src2++;
	  }

  return differences ;
}

int CommonCladeRefinement::combine(char* src1, char* src2,int nchar)
{

	int differences = 0;
	int * weights = Interpreter::getInstance()->dataset()->weights();

  for (int i = 0; i < nchar; i++)
  {
	  if ( ! ( *src1 & *src2 ) )
      {
          differences += weights[i] ;
		  //printf("Incrementing differences by %d\n",weights[i]);
	  }
	  src1++;
	  src2++;
  }
  return differences;
}

const int MAX_STRING_SIZE = 10000;

/**
 * BuildTree, checks that the two subtrees contain all the taxa and if so builds a tree from them.  Otherwise it returns NULL
 **/
QTree* CommonCladeRefinement::BuildTree(QNode* subtree1,QNode* subtree2)
{
	NewickTreeParser ntp;
	char treeStr[MAX_STRING_SIZE];
	
	char treeStr1[MAX_STRING_SIZE];
	char treeStr2[MAX_STRING_SIZE];
	
	int ntaxa = Interpreter::getInstance()->dataset()->ntaxa();
	
	bool* included = (bool*)malloc(sizeof(bool)*ntaxa);
	for(int i=0;i<ntaxa;i++)
		included[i] = false;
	BuildString(subtree1,included,treeStr1);
	BuildString(subtree2,included,treeStr2);
	
	//printf("%s\n\n%s\n\n**************",treeStr1,treeStr2);
	
	for(int i=0;i<ntaxa;i++)
		if(!included[i])
		{
			//this is not a valid tree
			free(included);
			return NULL;
		}
		
	sprintf(treeStr,"(%s,%s)",treeStr1,treeStr2);
	QTree* qt = ntp.parseBuffer(treeStr,strlen(treeStr),false);
	qt->setTreeStr(treeStr);
	free(included);
	
	return qt;
}

char* CommonCladeRefinement::BuildString(QNode* root,bool* included,char* root_str)
{
	char child1[MAX_STRING_SIZE];
	char child2[MAX_STRING_SIZE];
	if(root->nodeInfo()->leaf())
	{
		sprintf(root_str,"%d",root->nodeInfo()->nodeIndex()+1);
		included[root->nodeInfo()->nodeIndex()] = true;
		return root_str;
	}
	memset(child1,0,MAX_STRING_SIZE);
	memset(child2,0,MAX_STRING_SIZE);
	BuildString(root->child1(),included,child1);
	BuildString(root->child2(),included,child2);
	sprintf(root_str,"(%s,%s)",child1,child2);
	return root_str;
}
