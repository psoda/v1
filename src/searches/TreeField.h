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
 *  TreeField.h
 *  
 *
 *  Created by Kenneth Sundberg on 3/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TREE_FIELD_H_
#define _TREE_FIELD_H_

#include "QTree.h"
#include "GradientDescent.h"
#include <list>

class TreeField
{
	public:
		TreeField(GradientDescentSearch*);
		
		void GenerateStepwiseTrees(int n);
		void GenerateTrees(int taxa,int branches,double min_x,double max_x,double min_y,double max_y,double min_z,double max_z);
		void display();
		void EvaluateTrees();
		void Clear();
		
	private:
		//bounding box
		double min_x,max_x;
		double min_y,max_y;
		double min_z,max_z;
		
		PartitionTree* GenerateTree(int taxa,int branches);
		int GenerateSide(int taxa,int branches,PartitionTree*,bool*,bool);
		GradientDescentSearch* gds_pt;
		
		list<ProjectedTree> points;
		list<char*> trees;
};

#endif