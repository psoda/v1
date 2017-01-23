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
#ifndef PSODA_TREE_RESOLVE_H_
#define PSODA_TREE_RESOLVE_H_

#include "QSearchBase.h"
#include "EvaluatorBase.h"
#include "Dataset.h"
#include "QTreeRepository.h"
#include "QTree.h"

#include "ProjectedTree.h"
#include "PartitionTree.h"


#define HYPERSPHERE_LOOKUP_TABLE_SIZE 65535

class GradientDescentSearch : public QSearchBase
{
  public:
    GradientDescentSearch();
    virtual ~GradientDescentSearch();

    //VIRTUAL INTERFACE FUNCTIONS
    void search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int iterations);
	const char* name() const; 
    //GETTERS & SETTERS
	
	
	ProjectedTree map(QTree*) const;
	ProjectedTree map(const PartitionTree*) const;
	ProjectedTree lookup(const Partition) const;
	
	protected:
	
	list<QTree*> refine(QTree* source,ProjectedTree direction,int);
	
	
	void setup_tables(bool orthogonal = true);
	
	double xtable[HYPERSPHERE_LOOKUP_TABLE_SIZE];
	double ytable[HYPERSPHERE_LOOKUP_TABLE_SIZE];
	double ztable[HYPERSPHERE_LOOKUP_TABLE_SIZE];
	int accessed[HYPERSPHERE_LOOKUP_TABLE_SIZE];
};

#endif