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
 
#include "OrthoSectorSearch.h"
 
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

OrthoSectorSearch::OrthoSectorSearch()
{
	
	sectors=fopen("ORTHO_sectors.dat","w");
	directions=fopen("ORTHO_directions.dat","w");
}

OrthoSectorSearch::~OrthoSectorSearch()
{
	fclose(sectors);
	fclose(directions);
}

const char* OrthoSectorSearch::name() const
{
	return "Orthogonal Sector Search";
}
 
double dot2(ProjectedTree a, ProjectedTree b)
{
	double cur_dot = 0.0;
	for(int i=0;i<3;i++)
			cur_dot += a(i)*b(i);
	if(cur_dot < 0) return -cur_dot;
	return cur_dot;
}

bool OrthoSectorSearch::BuildOrthoSector(QTree* tree)
{
	ProjectedTree d;
	ProjectedTree position = projection.lookupTree(tree);
	ProjectedTree sector = position + mDirectionSearched;
	d=mDirectionSearched;
	
	//choose start node
	QNodeInfo* best = NULL;
	double best_dot = DBL_MAX;
	for(map<QNodeInfo*,ProjectedTree>::iterator iter = mPartitions.begin();iter!=mPartitions.end();iter++)
	{
		if(!mUsedPartitions[iter->first])
		{
			double cur_dot = dot2(iter->second,d);
			if(best_dot > cur_dot)  //find most orthogonal dimension
			{
				best_dot = cur_dot;
				best = iter->first;
			}
		}
	}
	
	if(best == NULL) return false;
	
	//expand into a sector
	int nodes_left = 97; //2n-3 branches in a tree 97 => 50 taxa tree after constraints
	std::priority_queue< std::pair< double, QNode*> > queue;
	queue.push(pair< double, QNode*> (dot2(mPartitions[best],d) ,best->node()));
	queue.push(pair< double, QNode*> (dot2(mPartitions[best->node()->external()->nodeInfo()],d),best->node()->external()));
	while(nodes_left > 0)
	{
		QNode* node = queue.top().second;
		queue.pop();
		
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
					queue.push(pair< double, QNode*> (dot2(mPartitions[node->child1()->nodeInfo()],d) ,node->child1()));
				}
				
				if(node->child2()->nodeInfo()->leaf())
				{
					AddNodeToCurrentSector(node->child2());
					nodes_left--;
				}
				else
				{
					queue.push(pair< double, QNode*> (dot2(mPartitions[node->child2()->nodeInfo()],d) ,node->child2()));
				}
				
				AddNodeToCurrentSector(node);
				nodes_left--;
			}
		}
		if(queue.empty())
		{
			sector = position - mDirectionSearched;
			fprintf(sectors,"%.2f\t%.2f\t%.2f\n",sector(0),sector(1),sector(2));
			fprintf(directions,"%.2f\t%.2f\t%.2f\n",mDirectionSearched(0) - d(0),mDirectionSearched(1) - d(1),mDirectionSearched(2) -d(2));
			return false;
		}
	}
	sector = position - mDirectionSearched;
	fprintf(sectors,"%.2f\t%.2f\t%.2f\n",sector(0),sector(1),sector(2));
	fprintf(directions,"%.2f\t%.2f\t%.2f\n",mDirectionSearched(0) - d(0),mDirectionSearched(1) - d(1),mDirectionSearched(2) -d(2));
	return true;
}

bool OrthoSectorSearch::BuildSector(QTree* tree)
{
	if(mDirectionSearched.length() < 0.1) //search is balanced, choose a random sector
		return BuildRandomSector(tree);
	else
		return BuildOrthoSector(tree);
	
}

