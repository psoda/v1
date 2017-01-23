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
 #include "PartitionSectorSearchBase.h"
 
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include "QStep.h"
#include "QTBR.h"

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
#include "CommonCladeRefinement.h"
#include <queue>

#define HARD_MAX_SECT_SIZE 127 /*This is a sector with 65 leaf nodes*/
#define MAX_SECT_SIZE 97  /*This is a sector with 50 leaf nodes*/
#define MIN_SECT_SIZE 67 /*This is a sector with 35 leaf nodes*/

PartitionSectorSearchBase::PartitionSectorSearchBase()
{
	curSector_size = 0;
	curSector_leaves = 0;
	
	for(int i=0;i<3;i++)
		mDirectionSearched(i) = 0;
}

PartitionSectorSearchBase::~PartitionSectorSearchBase()
{
}

 
int PartitionSectorSearchBase::BuildSectors(QTree* tree)
{
	mSectors.clear();
	mPartitions.clear();
	buildPartitionMap(tree->root());
	buildPartitionMap(tree->root()->external());
	
	while(BuildSector(tree))
	{
		StartNewSector();
	}
	
	/*
	mGlobalTBRLength =(int)(mGlobalTBRLength * 1.1);
	mGlobalPerturbedTBRLength =(int)(mGlobalPerturbedTBRLength * 1.15);
	*/
	return mSectors.size();
}




bool PartitionSectorSearchBase::BuildRandomSector(QTree* tree)
{
	//printf("Random Sector\n");
	//pick a leaf node that has not been used recently
	int start_taxon = rand()%Interpreter::getInstance()->dataset()->ntaxa();
	
	QNodeInfo* info;
	tree->initNodeInfoIter();
	bool found = false;
	while(!found)
	{
		info = tree->nextNodeInfo();
		if(info==NULL) return false;
		if(info->leaf())
		{
			if(info->nodeIndex()==start_taxon)
				found = true;
		}
	}
	
	//printf("Found Center Node\n");
	
	int nodes_left = 97; //2n-3 branches in a tree 97 => 50 taxa tree after constraints
	list<QNode*> queue;
	queue.push_back(info->node());
	queue.push_back(info->node()->external());
		
	while(nodes_left > 0)
	{
		QNode* node = queue.front();
		queue.pop_front();
		
		if(node->nodeInfo()->leaf()) //We want leaf nodes in the sector to improve phylogenetic signal
		{
			AddNodeToCurrentSector(node);
			nodes_left--;
		}
		else
		{
			
			if(!mUsedPartitions[node->nodeInfo()])
			{
				mUsedPartitions[node->nodeInfo()] = true;
				mDirectionSearched += mPartitions[node->nodeInfo()];
				if(node->child1()->nodeInfo()->leaf())
				{
					AddNodeToCurrentSector(node->child1());
					nodes_left--;
				}
				else
				{
					queue.push_back(node->child1());
				}
				
				if(node->child2()->nodeInfo()->leaf())
				{
					AddNodeToCurrentSector(node->child2());
					nodes_left--;
				}
				else
				{
					queue.push_back(node->child2());
				}
				
				AddNodeToCurrentSector(node);
				nodes_left--;
			}
		}
		if(queue.empty())
		{
			return false;
		}
	}
	return true;
	
}


Partition PartitionSectorSearchBase::buildPartitionMap(QNode* root)
{
	if(root->nodeInfo()->leaf())
	{
		int leaf_index = root->nodeInfo()->nodeIndex();
		return Partition(Interpreter::getInstance()->dataset()->ntaxa(),leaf_index);
	}
	Partition left = buildPartitionMap(root->child1());
	Partition right = buildPartitionMap(root->child2());
	Partition center = left + right;
	
	mPartitions[root->nodeInfo()] = projection.lookupPartition(center);
	mUsedPartitions[root->nodeInfo()] = false;
	
	return center;
}


void PartitionSectorSearchBase::AddNodeToCurrentSector(QNode* root)
{
	curSector_size++;
	if(root->nodeInfo()->leaf())
		curSector_leaves++;
	curSector.push_back(root->nodeInfo()->nodeIndex());
}

void PartitionSectorSearchBase::StartNewSector()
{
	printf("Sector Size %d/%d\n",curSector_leaves,curSector_size);
	if(curSector_size < MIN_SECT_SIZE)
	{
		curSector.clear();
	}
	else
	{
		curSector.sort();
		mSectors.push_back(curSector);
		curSector.clear();
	}
	curSector_size = 0;
	curSector_leaves = 0;
}

void PartitionSectorSearchBase::addConstraints(QTree* tree,int sector_num)
{
	//return;
	tree->clearConstraints();
	
	list< list<int> >::iterator sector = mSectors.begin();
	for(int i=1;i<sector_num;i++)
		sector++;
	//printf("Bulding Sector %d (%d nodes)\n",sector_num,(int) sector->size());
	for(list<int>::iterator iter = sector->begin();iter!=sector->end();iter++)
	{
		tree->addConstraint(*iter);
		//printf("%d ",*iter);
	}
	//printf("\n");
}
