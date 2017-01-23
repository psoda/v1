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
#include "PartialTreeSearch.h"
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
#include <queue>
#include "NewickTreeParser.h"

#define MAXTREES 1
#define MAXEQUAL 0


PartialTreeSearch::PartialTreeSearch():mStartTime(0.0),mUpdated(false),minSectSize(30),maxSectSize(60)
{
}

PartialTreeSearch::~PartialTreeSearch()
{
}

const char* PartialTreeSearch::name() const
{
	return "Partial Tree Search";
}

void PartialTreeSearch::setMinSize(int size)
{
	minSectSize = size;
}

void PartialTreeSearch::setMaxSize(int size)
{
	maxSectSize = size;
}

int distance_between_taxa(int i,int j)
{
	char* A;
	char* B;
	A = Interpreter::getInstance()->dataset()->getCharacters(i,false);
	B = Interpreter::getInstance()->dataset()->getCharacters(j,false);
	int score =0;
	for(int k=0;k<Interpreter::getInstance()->dataset()->nchars();k++)
	{
		if(A[k]!=B[k]) score++;
	}
	return score;
}

void PartialTreeSearch::Initialize( QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator)
{
//Divide taxa into groups of 50-100 taxa
	//How many groups?
	mNSubTrees = 1;
	printf("min:%d\tmax:%d\n",minSectSize,maxSectSize);
	float subtreeSize = Interpreter::getInstance()->dataset()->ntaxa();
	while(subtreeSize > (minSectSize + 2 * (maxSectSize - minSectSize) / 3) * 2)
	{
		mNSubTrees *=2;
		subtreeSize /= 2;
	}
	//Randomize taxa (This could perhaps be replaced with a NJ tree or some other clustering)
	QDeque<int> vi;
	QDeque<int> sub_vi;
	
	std::priority_queue< std::pair< int, int>, std::vector<std::pair< int, int> > >taxa_by_distance;
	for (int i = 0; i < Interpreter::getInstance()->dataset()->ntaxa(); i++)
    {
		taxa_by_distance.push(make_pair(distance_between_taxa(0,i),i));
	}
    for (int i = 0; i < Interpreter::getInstance()->dataset()->ntaxa(); i++)
    {
        vi.push_back(taxa_by_distance.top().second);
		taxa_by_distance.pop();
    }
	//vi.randomize();
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
		
		mSubTrees.push_back(new PartialTree(qsearchAlgorithm,evaluator,sub_vi));
	}
	mSubTrees.front()->setMinSplit(minSectSize);
	mSubTrees.front()->setMaxSplit(maxSectSize);
}
 
void PartialTreeSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int nreps)
{
	nreps = 3; //TODO this magic is not good
	mStartTime = When();
	Initialize(qsearchAlgorithm,evaluator);
	for(int i=0;i<nreps;i++)
	{
		//printf("%d/%d\n",i,nreps);
		double total_score = 0.0;
		for(list<PartialTree*>::iterator iter = mSubTrees.begin();iter != mSubTrees.end();iter++)
		{
			total_score += (*iter)->score();
			//printf("%f\n",total_score);
		}
		//refine partial trees
		int tree_num = 1;
		int total_trees = mSubTrees.size();
		for(list<PartialTree*>::iterator iter = mSubTrees.begin();iter != mSubTrees.end();iter++)
		{
			//printf("PartialTree %d / %d:\n",tree_num,total_trees);
			tree_num++;
			(*iter)->refine(total_score,mStartTime);
		}
		//allow partial trees to divide
		
		for(list<PartialTree*>::iterator iter = mSubTrees.begin();iter != mSubTrees.end();iter++)
		{
			PartialTree* pt = (*iter)->divide();
			if(pt != NULL)
				mSubTrees.push_back(pt);
		}
		//printf("%d trees divided into %d trees\n",total_trees,mSubTrees.size());
		total_trees = mSubTrees.size();
		//determine parings
		//allow partial trees to join
		bool joined = true;
		int join_attempts = mSubTrees.size();
		//for each partial tree
		for(int i=0;i<join_attempts;i++)
		{
			//find tree that will move it the farthest distance from where it was
			PartialTree* pt = mSubTrees.front();
			mSubTrees.pop_front();
			list<PartialTree*>::iterator best;
			double distance = -0.5;
			best = mSubTrees.begin();
			for(list<PartialTree*>::iterator iter = mSubTrees.begin();iter != mSubTrees.end();iter++)
			{
				double cur_distance = (*iter)->distanceWith(pt);
				if(cur_distance > distance)
				{
					best = iter;
					distance = cur_distance;
				}
			}
			joined = (*best)->join(pt);
			if(!joined) mSubTrees.push_back(pt); //tree did not join, put it back on the list
		}
		
		/*
		while(join_attempts > 0)
		{
			join_attempts--;
			PartialTree* pt = mSubTrees.front();
			mSubTrees.pop_front();
			for(list<PartialTree*>::iterator iter = mSubTrees.begin();iter != mSubTrees.end();iter++)
			{
				joined = (*iter)->join(pt);
				if(joined) break;
			}
			if(!joined) mSubTrees.push_back(pt); // tree did not join with any, put it back on the list
		}*/
		
		//printf("%d trees joined into %d trees\n",total_trees,mSubTrees.size());
		
		int total_size = 0;
		for(list<PartialTree*>::iterator iter = mSubTrees.begin();iter != mSubTrees.end();iter++)
		{
			total_size += (*iter)->size();
		}
		//printf("%d taxa\n",total_size);
		if(total_size > Interpreter::getInstance()->dataset()->ntaxa())
		{
			throw PsodaError("Too many taxa\n");
		}
		
	}
	
	
	double combined_score=0.0;
	for(list<PartialTree*>::iterator iter = mSubTrees.begin();iter != mSubTrees.end();iter++)
	{
		combined_score += (*iter)->score();
	}
	
	PartialTree* sum = mSubTrees.front();
	mSubTrees.pop_front();
	
	int tree_num = 1;
	int total_trees = mSubTrees.size();
	while(!mSubTrees.empty())
	{
		tree_num++;
		PartialTree* pt = mSubTrees.front();
		mSubTrees.pop_front();
		combined_score-=pt->score();
		combined_score-=sum->score();
		//printf("Joining PartialTree %d, %d to join:\n",tree_num,mSubTrees.size());
		sum->join(pt,true);
		combined_score+=sum->score();
		
		
		sum->refine(combined_score,mStartTime);
		mSubTrees.push_back(sum);
		
		/*printf("Running Combined Score: %f\n",combined_score);
		combined_score=0.0;
		for(list<PartialTree*>::iterator iter = mSubTrees.begin();iter != mSubTrees.end();iter++)
		{
			combined_score += (*iter)->score();
		}
		printf("New Combined Score: %f\n",combined_score);
		*/
		
		sum=mSubTrees.front();
		mSubTrees.pop_front();
	}
	QTree* qt = sum->GetTree();
	qtreeRepository.addTree(qt);
}


