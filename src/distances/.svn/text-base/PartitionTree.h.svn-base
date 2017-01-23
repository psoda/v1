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
#ifndef PSODA_PARTITION_TREE_H
#define PSODA_PARTITION_TREE_H

#include "Partition.h"
#include "QTree.h"
#include <queue>

/**
 * Class partition tree represents a tree as a set of partitions
 */
class PartitionTree
{
	public:
	PartitionTree(int);
	PartitionTree(QTree*);
	~PartitionTree();
	
	void add(const Partition&,double branch_length = -1.0);
	char* treeString();
	
	void AddMatrixRepresentation(Dataset*) const;
	
	std::list<Partition>::const_iterator begin() const;
	std::list<Partition>::const_iterator end() const;

	bool isCompatible(const Partition&) const;
	
	void compatiblePartitions(int& num_partitions,Partition*& partitions);
	
	int partitionsNeededToResolve();
	
	protected:
	void join(int,int,bool treestring = true);
	int find(int);
	
	char* treestringjoin(char*,char*);
	char* treestringparens(char*,double branch_length = -1.0);
	
	bool maybeCompatible(int,bool*) const;
	
	Partition AddPartition(QNode*);
	
	int mNPartitions;
	int mNtaxa;
	std::priority_queue <Partition, std::vector<Partition>, std::greater<Partition> > mPartitionSet; // Min-heap for Partitions
	std::list<Partition> mPartitions; //List of partitions for interested parties
	int* ufinfo; //Array for union-find
	bool* dont_care;
	char** partialTreeString;  //Partial treestrings
	void considerAllPartitions(int ntaxa,bool* taxa,list<Partition>*);
	void considerPartitions(list<Partition>*);
	void generatePossiblePartitions(list<int>& to_consider,list<Partition>* partitions,int cur, int size,int included,bool* included_taxa);
	
};

#endif
