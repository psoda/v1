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
#ifndef PSODA_RESOLUTION_TREE_H_
#define PSODA_RESOLUTION_TREE_H_

/*
 *  ResolutionTree.h
 *  
 *
 *  Created by Kenneth Sundberg on 1/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "QTree.h"
#include "Dataset.h"
#include <list>
#include "Environment.h"

class ResolutionTree
{
	public:
		ResolutionTree(QTree*, bool TBR=false);
		~ResolutionTree();
	
		QTree* getQTree();
	
	private:
		ResolutionTree();
	
		ResolutionTree* BuildNode(QNode*,int level);
		char* partialTreeStr();
	 
		char* partialTreeStringFromNode(QNode*);
		QNode* findNode(int id, QNode* root);	 
		void Resolve(const char* pre_seq,bool TBR);
	
		bool mResolved;
		bool mLeaf;
		Dataset* mDataset;
		QTree* mResolution; 
		int mNchars;
		char* mSequenceData;
		list<ResolutionTree*> children;
		int taxon;
		char TreeStr[10000];
		
};
#endif