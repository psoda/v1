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
#ifndef PSODA_PARTITION_SPACE_SECTOR_SEARCH 
#define PSODA_PARTITION_SPACE_SECTOR_SEARCH

#include "QSearchBase.h"
#include "PartitionSectorSearchBase.h"
#include "EvaluatorBase.h"
#include "Dataset.h"
#include "QTreeRepository.h"
#include "Hypersphere.h"
#include "PartitionSector.h"
#include <map.h>
#include <stdio.h>

class PartitionSpaceSectorSearch : public PartitionSectorSearchBase
{
  public:
    PartitionSpaceSectorSearch();
    virtual ~PartitionSpaceSectorSearch();

    //VIRTUAL INTERFACE FUNCTIONS
	const char* name() const; 
    //GETTERS & SETTERS
protected:
	
	bool BuildSector(QTree* tree);
private:
	ProjectedTree DirectionFromCenter(ProjectedTree d);
	void CalculateCenter(ProjectedTree posiiton,ProjectedTree d);
	bool BuildDirectionSector(QTree* tree,ProjectedTree d);
	ProjectedTree mCenter;
	bool mCalculated;
	bool mTrees;
	FILE* sectors;
	FILE* directions;

};


#endif

