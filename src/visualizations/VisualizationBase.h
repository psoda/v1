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
#ifndef _PSODA_VISUALIZATION_H_
#define _PSODA_VISUALIZATION_H_

#include <vector>
#include "QTree.h"
#include "QTreeRepository.h"
#include "ProjectedTree.h"

class VisualizationBase
{
public:
	VisualizationBase();
	virtual ~VisualizationBase();
	void AddTrees(QTreeRepository*);
	void AddTree(QTree*);
	virtual list<ProjectedTree> visualization() = 0;
protected:
	QTreeRepository mTrees;
	
};

#endif