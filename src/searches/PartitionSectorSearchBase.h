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
#ifndef PSODA_PARTITION_SECTOR_SEARCH_BASE 
#define PSODA_PARTITION_SECTOR_SEARCH_BASE

#include "QSearchBase.h"
#include "SectorSearchBase.h"
#include "EvaluatorBase.h"
#include "Dataset.h"
#include "QTreeRepository.h"
#include "Hypersphere.h"
#include "PartitionSector.h"
#include <map.h>

class PartitionSectorSearchBase : public SectorSearchBase
{
  public:
    PartitionSectorSearchBase();
    virtual ~PartitionSectorSearchBase();

protected:
	
	//VIRTUAL INTERFACE FUNCTIONS
	int BuildSectors(QTree*);
	void addConstraints(QTree*,int);
	virtual bool BuildSector(QTree* tree) = 0;
	
	
	bool BuildRandomSector(QTree* tree);
	

	void StartNewSector();
	void AddNodeToCurrentSector(QNode* root);
	Partition buildPartitionMap(QNode* root);
	
	Hypersphere projection;
	list<int> curSector;
	int curSector_size;
	int curSector_leaves;
	list< list <int> > mSectors;
	map<QNodeInfo*,ProjectedTree > mPartitions;
	map<QNodeInfo*,bool > mUsedPartitions;
	ProjectedTree mDirectionSearched;

};


#endif

