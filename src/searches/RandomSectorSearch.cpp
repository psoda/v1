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
 #include "RandomSectorSearch.h"
 
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

#define REPLICATES 20
#define STEPWISE_REPLICATES 5

RandomSectorSearch::RandomSectorSearch()
{
}

RandomSectorSearch::~RandomSectorSearch()
{
}

const char* RandomSectorSearch::name() const
{
	return "Random Sector Search";
}
 
int RandomSectorSearch::BuildSectors(QTree* tree)
{
	return 1;
}

void RandomSectorSearch::addConstraints(QTree* tree,int sector_num)
{
	//return;
	tree->clearConstraints();
	tree->initNodeInfoIter();
	
	
	//pick a leaf node that has not been used recently
	int start_taxon = rand()%Interpreter::getInstance()->dataset()->ntaxa();
	
	QNodeInfo* info;
	tree->initNodeInfoIter();
	bool found = false;
	while(!found)
	{
		info = tree->nextNodeInfo();
		if(info==NULL) return;
		if(info->leaf())
		{
			if(info->nodeIndex()==start_taxon)
				found = true;
		}
	}
	
	int nodes_left = 97; //2n-3 branches in a tree 97 => 50 taxa tree after constraints
	list<QNode*> queue;
	queue.push_back(info->node());
	queue.push_back(info->node()->external());
	while(nodes_left > 0)
	{
		QNode* node = queue.front();
		queue.pop_front();
		if(!node->nodeInfo()->leaf())
		{
			queue.push_back(node->child1());
			queue.push_back(node->child2());
		}
		tree->addConstraint(node->nodeInfo());
		nodes_left--;
	}
}
