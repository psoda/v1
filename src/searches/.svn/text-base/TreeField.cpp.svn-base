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
/*
 *  TreeField.cpp
 *  
 *
 *  Created by Kenneth Sundberg on 3/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TreeField.h"
#include "PartitionTree.h"
#include "gnuPlotTerminal.h"
#include "NewickTreeParser.h"
#include "ResolutionTree.h"
#include "Interpreter.h"
#include "Parsimony.h"
#include "QStep.h"
#include "QTBR.h"


#define MAX_NODE_DEGREE 10 /*Note this is a soft constraint*/

#define X_VOXELS 30
#define Y_VOXELS 30
#define Z_VOXELS 30

TreeField::TreeField(GradientDescentSearch* gds):gds_pt(gds)
{
}

void TreeField::display()
{
	gnuPlotTerminal term;
	term.setOutput("TreeField.dat");
	term.display(points);
}

PartitionTree* TreeField::GenerateTree(int taxa,int branches)
{
	//printf("Generating Tree\n");
	PartitionTree* pt;
	pt = new PartitionTree(taxa);
	//Generate Root Branch
	bool* partition_array = (bool*)malloc(sizeof(bool)*taxa);
	for(int i=0;i<taxa;i++)
		if( ((float)rand()/INT_MAX) > 0.5)
			partition_array[i] = true;
		else
			partition_array[i] = false;
	
	/*printf("True side\n");
	for(int i=0;i<taxa;i++)
		if(partition_array[i])
			printf("X");
		else
			printf("-");
	printf("\n");
	*/
	
	int true_side = GenerateSide(taxa,branches/2,pt,partition_array,true);
	
	//printf("False side\n");
	/*int false_side =*/ GenerateSide(taxa,branches-true_side,pt,partition_array,false);
	
	//char* tree_str = pt->treeString();
	//printf("Generated Tree: %s\n",tree_str);
	return pt;
}


int TreeField::GenerateSide(int taxa,int branches_remaining,PartitionTree* tree,bool* partition,bool side)
{
	//printf("%d branches remaining\n",branches_remaining);
	if(branches_remaining <=0) return 0;
	
	//copy partition data
	bool* partition_data = (bool*)malloc(sizeof(bool)*taxa);
	int branches = (rand()%MAX_NODE_DEGREE)-1;
	if(branches < 2) branches = 2;
	if (branches > branches_remaining) branches = branches_remaining;
	int* partitions = (int*)malloc(sizeof(int)*taxa);
	for(int i=0;i<taxa;i++)
	{
		if(partition[i] == side)
			partitions[i] = 0;  //the 0th subtree is what the parent has set
		else
			if(branches > 1)
				partitions[i] = rand()%branches + 1; //+1 to avoid exceeding MAX_NODE_DEGREE
			else
				partitions[i] = rand()%(branches+1);  //only one branch can be added so the MAX_NODE_DEGREE may be violated
	}

	int assigned_branches = 0;

	for(int j=1;j<=branches;j++)
	{
		if(assigned_branches < branches_remaining)
		{
			int partition_size = 0;
			for(int i=0;i<taxa;i++)
			{
				if(partitions[i] == j)
				{
					partition_data[i] = !side;
					partition_size++;
				}
				else
					partition_data[i] = side;
			}
			
			if(partition_size > 1) //don't bother with trivial partitions
			{
				Partition* part = new Partition(taxa,partition_data);
				if(tree->isCompatible(*part))
				{
					tree->add(*part);
					//part->print();
					//printf("\n");
					assigned_branches++;
				}
				else
				{
					//printf("ERROR: Generated Incompatible Branch!\n");
				}
				delete(part);
			
				if(partition_size > 2) //don't bother recursing into simple clades
					assigned_branches += GenerateSide(taxa,branches_remaining-assigned_branches,tree, partition_data, side);
			}
		}
	}
	
	free(partition_data);
	free(partitions);
	return assigned_branches;
}

void TreeField::Clear()
{
	points.clear();
	trees.clear();
}

void TreeField::GenerateStepwiseTrees(int n)
{
	QStepwiseAdditionSearch stepwise;
	QTBR search_algorithim;
	Parsimony evaluator;
	QTreeRepository cur_tree;
	QTree* qt;

	//start with a set of stepwise parsimony trees
	for(int i=0;i<n;i++)
	{
			stepwise.search(cur_tree,&search_algorithim,&evaluator,1);
			qt = cur_tree.popTree();
			char* tree_string = strdup(qt->treeStr());
			ProjectedTree position = gds_pt->map(qt);
			position.score = qt->getScore();
			points.push_front(position);
			trees.push_front(tree_string);
	}
}

void TreeField::GenerateTrees(int taxa,int prefered_branches,double min_x,double max_x,double min_y,double max_y,double min_z,double max_z)
{
	int voxels[X_VOXELS][Y_VOXELS][Z_VOXELS];
	const int TREES_PER_VOXEL = 10;
	
	for(int i=0;i<X_VOXELS;i++)
	for(int j=0;j<Y_VOXELS;j++)
	for(int k=0;k<Z_VOXELS;k++)
		voxels[i][j][k] = 0;
		
	int trees_to_generate = X_VOXELS*Y_VOXELS*Z_VOXELS;
	
	int x_branches = (int)((double)(max_x - min_x) / X_VOXELS);
	int y_branches = (int)((double)(max_y - min_y) / Y_VOXELS);
	int z_branches = (int)((double)(max_z - min_z) / Z_VOXELS);
	
	int branches = x_branches;
	if(y_branches < branches) branches = y_branches;
	if(z_branches < branches) branches = z_branches;
	//each tree should resolve to a volumne no smaller than its assigned voxel.
	
	branches = ((taxa - 3) - branches) / 2;
	
	branches = prefered_branches;
	
	printf("Generating trees with %d branches (%d,%d,%d)\n",branches,x_branches,y_branches,z_branches);
	
	int trees_generated = 0;
	PartitionTree* pt = NULL;
	while((trees_generated < 10000)&&(trees_to_generate > 0))
	{
		pt = GenerateTree(taxa,branches);
		char* tree_string = strdup(pt->treeString());
		
		ProjectedTree position = gds_pt->map(pt);
		trees_generated++;
		
		//printf("%.3f\t%.3f\t%.3f\n",position(0),position(1),position(2));
		
		if(!((position(0) > max_x)||(position(0) < min_x)||(position(1) > max_y)||(position(1) < min_y)||(position(2) > max_z)||(position(2) < min_z)))
		{
			
			int vox_i = (int)((X_VOXELS*position(0))/(max_x-min_x));
			int vox_j = (int)((Y_VOXELS*position(1))/(max_y-min_y));
			int vox_k = (int)((Z_VOXELS*position(2))/(max_z-min_z));
			
			
			if(voxels[vox_i][vox_j][vox_k] < TREES_PER_VOXEL)
			{
				voxels[vox_i][vox_j][vox_k] += 1;
				trees_to_generate--;
				//printf("%d: %s\n",trees_generated,tree_string);
				points.push_front(position);
				trees.push_front(tree_string);
			}
			else
			{
				delete tree_string;
			}
		}
		else
		{
			delete tree_string;
		}
		/*if(trees_generated%100 == 0)
			printf("Generated: %d\t Needed: %d\n",trees_generated,trees_to_generate);
		*/	
		
	//printf("Generated: %d\t Needed: %d\n",trees_generated,trees_to_generate);	
	}
	
	/*
	for(int i=0;i<X_VOXELS;i++)
	{
		for(int j=0;j<Y_VOXELS;j++)
		{
			for(int k=0;k<Z_VOXELS;k++)
			{
				if(voxels[i][j][k])
					printf("X ");
				else
					printf("- ");
			}
			printf("\n");
		}
		
		printf("\n");
	}*/

	//printf("Generated: %d\t Needed: %d\n",trees_generated,trees_to_generate);
}
		
void TreeField::EvaluateTrees()
{
	//Interpreter::getInstance()->dataset()->convertToCompressedBinaryMatrix();
    NewickTreeParser* ntp;
	Parsimony p;
	//make a QTree from each tree in list and evaluate
	int tree_num = 0;
	FILE* fout;
	fout = fopen("TreeField.tre","w");
	printf("Evaluating Trees\n");
	//list<char*>::iterator i = trees.begin();
	
	for(list<char*>::iterator i = trees.begin();i != trees.end();i++)
	{
		printf("Before: %s\n",*i);
		ntp = new NewickTreeParser();
		QTree * qt = ntp->parseBuffer(*i, strlen(*i), false);
		delete ntp;
		qt->setTreeStr(*i);
		ResolutionTree* rt = new ResolutionTree(qt);
		//delete qt;
		qt = rt->getQTree();
		qt->updateTreeString();
		printf("Scoring Full Tree\n");
		double score = p.qscoreTree(qt);
		printf("score %f\n",score);
		fprintf(fout, "tree TreeName%d = [&U]  %s;\n",tree_num++,qt->treeStr());
		delete qt;
	}
}
