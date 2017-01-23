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
#ifndef PSODA_RF_H_
#define PSODA_RF_H_

#include "QTreeRepository.h"
#include "QTree.h"
#include <map>
#include "Partition.h"
#include "PartitionTree.h"

/**
 * The RF class computes the all-to-all Robinson-Foulds distances for a set of trees
 */
class RF
{
	public:
		RF(QTreeRepository*);
		~RF();
		void print(float cutoff = 0.05) const;
		int numTrees() const;
		int operator()(int,int);
		QTree* consensus(float);
		void conflate (int unresolution_degree,PartitionTree* conTree);
		double resolution(float);
	protected:
		
		void CalculateDistanceMatrix();
		void BuildMap(QTreeRepository*);
		void AddTree(QTree,int);
		void AddPartitionToMap(Partition,int);
		int mNtrees;
		map<Partition,list<int> > partition_map;
		int** distances;
		int mNtaxa;
};

#endif

