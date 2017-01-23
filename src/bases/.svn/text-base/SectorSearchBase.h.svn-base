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
#ifndef PSODA_SECTOR_SEARCH_BASE
#define PSODA_SECTOR_SEARCH_BASE

#include "QSearchBase.h"
#include "EvaluatorBase.h"
#include "Dataset.h"
#include "QTreeRepository.h"
#include "Hypersphere.h"
#include "PartitionSector.h"

class SectorSearchBase : public QSearchBase
{
  public:
    SectorSearchBase();
    virtual ~SectorSearchBase();

    //VIRTUAL INTERFACE FUNCTIONS
    void search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int iterations);
	virtual const char* name() const=0; 
    //GETTERS & SETTERS
	unsigned int& replicates();
	unsigned int replicates() const;
	int& stepwise_replicates();
	int stepwise_replicates() const;
	int& global_tbr_length();
	int global_tbr_length() const;
	int& global_perturbed_tbr_length();
	int global_perturbed_tbr_length() const;
	int& repo_size();
	int repo_size() const;
	
	bool sector_keep_tree() const;
	bool& sector_keep_tree();
	bool global_keep_tree() const;
	bool& global_keep_tree();
	bool perturbed_keep_tree() const;
	bool& perturbed_keep_tree();
	bool fast_exit_sector_search() const;
	bool& fast_exit_sector_search();
	bool ptm_start() const;
	bool& ptm_start();
	int ptm_min() const;
	int& ptm_min();
	int ptm_max() const;
	int& ptm_max();
	
protected:
	
	double mStartTime;
	bool mUpdated;
	int mStepwiseReplicates;
	unsigned int mReplicates;
	int mGlobalTBRLength;
	int mGlobalPerturbedTBRLength;
	int mRepoSize;
	bool mSectorKeepCur;
	bool mGlobalKeepCur;
	bool mPerturbedKeepCur;
	bool mFastSectorExit;
	bool mPTMStart;
	int mPTMMinSize;
	int mPTMMaxSize;
	
	double mBestScore;
	QTree* mBestTree;

	void DisplayScore(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,double bestScore);
    void PartitionSearch(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,bool keepCurTree = false);
	bool GlobalSearch(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,int timelimit,bool keepCurTree = false);

	double FuseTrees(QTreeRepository& dest,QTreeRepository& src, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, bool use_best);
	
	virtual int BuildSectors(QTree*)=0;
	virtual void addConstraints(QTree*,int)=0;

	QTree* doTBR(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,QTree* currentTree,double& bestScore,bool keepCurTree = false);
};


#endif

