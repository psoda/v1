/*
 *  ParticleTree.h
 *  
 *
 *  Created by Kenneth Sundberg on 5/7/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PSODA_PARTICLE_TREE_H_
#define PSODA_PARTICLE_TREE_H_

#include "GradientDescent.h"

class ParticleTree
{
public:
	ParticleTree(GradientDescentSearch*);
	~ParticleTree();
	
	void initStepwise();
	void initTree(QTree*);
	bool move(double x,double y,double z);
	void consenseMove(ParticleTree*,int);
	void explore();
	double score() const;
	double x();
	double y();
	double z();
	QTree* tree();
private:
		PartitionTree* unresolve(double x,double y,double z,int n);
		PartitionTree* unresolveByScore(int n);
		void resolve(PartitionTree*,int);
		void ecr(PartitionTree*,int cur,bool*,Partition*,int npartitions,int partitions_needed);

		QTree* curTree;
		char* curTreeStr;
		GradientDescentSearch* mGDS;
		ProjectedTree mPosition;
		
		bool improved;
		
};

#endif
