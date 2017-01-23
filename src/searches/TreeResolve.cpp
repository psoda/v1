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
 *  TreeResolve.cpp
 *  
 *
 *  Created by Kenneth Sundberg on 1/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TreeResolve.h"
#include "ResolutionTree.h"
#include "RF.h"

TreeResolveSearch::TreeResolveSearch()
{
}

TreeResolveSearch::~TreeResolveSearch()
{
}



const char* TreeResolveSearch::name() const
{
	return "Tree Resolution";
}

void TreeResolveSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm __attribute__((unused)),  EvaluatorBase *evaluator __attribute__((unused)), int nreps __attribute__((unused)))
{
	printf("In TreeResolve Search\n");

	//Compute Consensus tree
	printf("Computing Consensus\n");
	RF con_tree(&qtreeRepository);
	//Resolve Consensus tree
	QTree* con_result = con_tree.consensus(1.0);
	//con_result->showTree();
	printf("Resolving Tree\n");
	
	ResolutionTree res_tree(con_result);
	QTree* result = res_tree.getQTree();
	//set repository to result
	qtreeRepository.removeAll();
	qtreeRepository.addTree(result);
}


