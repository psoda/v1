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
 #include "PostOrderSectorSearch.h"
 
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

#define HARD_MAX_SECT_SIZE 127 /*This is a sector with 65 leaf nodes*/
#define MAX_SECT_SIZE 97  /*This is a sector with 50 leaf nodes*/
#define MIN_SECT_SIZE 67 /*This is a sector with 35 leaf nodes*/

PostOrderSectorSearch::PostOrderSectorSearch()
{
	curSector_size = 0;
	curSector_leaves = 0;
}

PostOrderSectorSearch::~PostOrderSectorSearch()
{
}

const char* PostOrderSectorSearch::name() const
{
	return "Post Order Sector Search";
}
 
int PostOrderSectorSearch::BuildSectors(QTree* tree)
{
	mSectors.clear();
	PostOrderTraversal(tree->root());
	StartNewSector();
	PostOrderTraversal(tree->root()->external());
	return mSectors.size();
}

int PostOrderSectorSearch::PostOrderTraversal(QNode* root)
{
	if(root->nodeInfo()->leaf())
	{
		AddNodeToCurrentSector(root);
		return 1; //parent has at least this one child
	}
	int child1 = PostOrderTraversal(root->child1());
	int sect_size = child1;
	
	if(sect_size > MIN_SECT_SIZE)
	{
		StartNewSector();
		sect_size = 0; //parent has no children in the current sector
	}
	AddNodeToCurrentSector(root);
	int child2 = PostOrderTraversal(root->child2());
	
	sect_size += child2+1; //size of current sector
	if(sect_size > MAX_SECT_SIZE)
	{
		StartNewSector();
		return 0; //parent has no children in the current sector
	}
	return sect_size; //parent has sect_size children in the current sector
}

void PostOrderSectorSearch::AddNodeToCurrentSector(QNode* root)
{
	curSector_size++;
	if(root->nodeInfo()->leaf())
		curSector_leaves++;
	curSector.push_back(root->nodeInfo()->nodeIndex());
}

void PostOrderSectorSearch::StartNewSector()
{
	//printf("Sector Size %d/%d\n",curSector_leaves,curSector_size);
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

void PostOrderSectorSearch::addConstraints(QTree* tree,int sector_num)
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
