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
#ifndef QALGORITHM_SECTARIAN 
#define QALGORITHM_SECTARIAN

#include "QAlgorithmBase.h"
#include "QTree.h"

class QTree;
class QAlgorithmBase;

class SectarianSearch : public QAlgorithmBase
{
  public:
    SectarianSearch();
    virtual ~SectarianSearch();

    /**
     * @param treeList This list is populated by rearrange (rather than returning a new list)
     */
    void rearrange(QTree* startTree, EvaluatorBase  *eval, int numtrees, int numequal, double bestScore, bool saveOrigTree, list<QTree*>& treeList);
	bool fast_return;
	private:
	
	
	bool refine(char* A,int A_score, char* B,int B_score, char* C,int C_score, char* D,int D_score, char* E,int E_score,int nchar, int* weights, QNodeInfo* node, list<char*>& treeList, double& best_score);
	int combine(char* dest, char* src1, char* src2,int nchar, int* weights);
	void buildTreeString(QNode* root,char* treeStr);

};
#endif

