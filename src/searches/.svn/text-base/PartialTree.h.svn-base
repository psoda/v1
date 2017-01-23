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
#ifndef QSODA_PARTIAL_TREE 
#define QSODA_PARTIAL_TREE

#include "Hypersphere.h"
#include "QAlgorithmBase.h"
#include "EvaluatorBase.h"
#include "QDeque.h"

class PartialTree
{
	public:
		PartialTree(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,QTree* tree);
		PartialTree(QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,QDeque<int> taxa);
		~PartialTree();
		double score();
		PartialTree* divide();
		bool join(PartialTree*& a, bool force=false);
		void refine(double&,double);
		double distanceWith(PartialTree*);
		QTree* GetTree();
		int size();
		void setMinSplit(int);
		void setMaxSplit(int);
	private:
		void Display(double mStartTime);
		QTree* doTBR(QTree* currentTree, double& bestScore);
		QNode* divisionPoint(QTree*);
		QTree* halfTree(QNode*);
		int findSize(QTree*);
		int findSizeRecursive(QNode*);
		void treeString(QNode*,char*);
		void treeStringTop(QNode*,char*);
		static ProjectedTree GetProjection(QTree*);
		PartialTree* sibling;
		ProjectedTree location;
		QTree* mTree;
		QAlgorithmBase* mSearch;
		EvaluatorBase* mEvaluator;
		double mScore;
		bool mUpdated;
		double otherTreeScore;
		int mSize;
};
#endif

