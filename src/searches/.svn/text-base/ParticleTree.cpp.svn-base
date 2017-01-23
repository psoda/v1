/*
 *  ParticleTree.cpp
 *  
 *
 *  Created by Kenneth Sundberg on 5/7/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleTree.h"
#include "PartitionTree.h"
#include "NewickTreeParser.h"
#include "Interpreter.h"
#include "Parsimony.h"
#include "QStep.h"
#include "RF.h"

#include "QTBR.h"

ParticleTree::ParticleTree(GradientDescentSearch* p):curTree(NULL),curTreeStr(NULL),mGDS(p)
{
}

ParticleTree::~ParticleTree()
{
}

double ParticleTree::x()
{
	return mPosition(0);
}


double ParticleTree::y()
{
	return mPosition(1);
}

double ParticleTree::z()
{
	return mPosition(2);
}

void ParticleTree::initStepwise()
{
	QStepwiseAdditionSearch stepwise;
	QTBR search_algorithim;
	Parsimony evaluator;
	QTreeRepository cur_tree;
	
	curTree = NULL;
	stepwise.search(cur_tree,&search_algorithim,&evaluator,1);
	curTree = cur_tree.popTree();
	mPosition = mGDS->map(curTree);
	mPosition.score = curTree->getScore();
	//curTree->showTree();
	if(curTreeStr)
	{
		free(curTreeStr);
	}
	curTreeStr = strdup(curTree->treeStr());
}

void ParticleTree::initTree(QTree* qt)
{
	mPosition = mGDS->map(qt);
	mPosition.score = qt->getScore();
	curTree = NULL;
	if(curTreeStr)
	{
		free(curTreeStr);
	}
	curTreeStr = strdup(qt->treeStr());
	printf("%s\n",curTreeStr);
}

double ParticleTree::score() const
{
	return mPosition.score;
}

QTree* ParticleTree::tree()
{
	NewickTreeParser* ntp = new NewickTreeParser();
	QTree* qt = ntp->parseBuffer(curTreeStr,strlen(curTreeStr),false);
	delete ntp;
	return qt;
}

/* Explore trys to improve a tree without knowledge of a direction to a better tree */
void ParticleTree::explore()
{
	delete curTree;
	NewickTreeParser* ntp = new NewickTreeParser();
	curTree = ntp->parseBuffer(curTreeStr,strlen(curTreeStr),false);
	delete ntp;
	resolve(unresolveByScore(6),6);
}

void ParticleTree::consenseMove(ParticleTree* other,int nTaxa)
{
	delete curTree;
	NewickTreeParser* ntp = new NewickTreeParser();
	curTree = ntp->parseBuffer(curTreeStr,strlen(curTreeStr),false);
	QTree* otherTree = ntp->parseBuffer(other->curTreeStr,strlen(other->curTreeStr),false);
	
	QTreeRepository repo(false);
  int ntaxa = Interpreter::getInstance()->dataset()->ntaxa();
	repo.setMaxTrees(2, ntaxa);
	repo.addTree(curTree);
	repo.addTree(otherTree);
	
	RF conTree(&repo);
	PartitionTree* pt = new PartitionTree(nTaxa);
	conTree.conflate(6,pt);
	
	int size = pt->partitionsNeededToResolve();
	if(size > 6)
	{
		printf("WARNING: Resolving %d branches\n",size);
		return;
	}
	resolve(pt,size);
	delete ntp;
}

bool ParticleTree::move(double x,double y,double z)
{
	delete curTree;
	NewickTreeParser* ntp = new NewickTreeParser();
	curTree = ntp->parseBuffer(curTreeStr,strlen(curTreeStr),false);
	delete ntp;
	//printf("Moving %s\n",curTree->treeStr());
	//curTree->showTree();
	improved = false;
	resolve(unresolve(x,y,z,6),6);
	return improved;
}

PartitionTree* ParticleTree::unresolveByScore(int n)
{
	PartitionTree* pt = new PartitionTree(curTree);
	double score;
	int nPartitions = 0;
	//consider each partition and sort them by score contribution
	std::priority_queue <pair<double,const Partition*>, std::vector< pair<double,const Partition*> > > PartitionSet; // Max-heap for scores w/ Partitions
	while(!PartitionSet.empty())
	{
		//printf("DEBUG: PartitionSet not EMPTY!\n");
		PartitionSet.pop();
	}
	for(std::list<Partition>::const_iterator i=pt->begin();i!=pt->end();i++)
	{
		score = rand(); //TODO get correct score
		pair<double,const Partition*> result;
		result.first = score;
		result.second = &(*i);
		PartitionSet.push(result);
		nPartitions++;
		//i->print();
		//printf(" X\n");
		i++; //there are two of each partition in the list
		//i->print();
		//printf("\n");
	}
	
	//printf("DEBUG nPartitions = %d\n",nPartitions);
	
	int ct = 0;
	
	//printf("DEBUG: Adding Partitions\n");
	
	//add all but the n worst partitions
	//TODO this isn't quite right
	PartitionTree* new_pt= new PartitionTree(Interpreter::getInstance()->dataset()->ntaxa());
	for(int j=0;j<nPartitions - n;j++)
	{
		pair<double,const Partition*> value = PartitionSet.top();
		PartitionSet.pop();
		if(new_pt->isCompatible(*(value.second)))
		{
			ct++;
			//(value.second)->print();
			new_pt->add(*(value.second));
			//printf(" X\n");
			/*
			printf("%d:\t",j);
			(*(value.second)).print();
			printf("\t%.3f\n",value.first);
			*/
		}
		else
		{
			
			//(value.second)->print();
			//printf("\n");
			/*
			printf("X%d:\t",j);
			(*(value.second)).print();
			printf("\t%.3f\n",value.first);
			*/
		}

	}
	
	delete curTree;
	delete pt;
	NewickTreeParser* ntp = new NewickTreeParser();
	char* treestring = new_pt->treeString();
	curTree = ntp->parseBuffer(treestring,strlen(treestring),false);
	/*printf("%s\n%d partitions\n",treestring,ct);
	if(ct < 17)
	{
	 throw "Too Many Unresolutions!\n";
	}*/
	//curTree->showTree();
	return new_pt;
}

PartitionTree* ParticleTree::unresolve(double x,double y,double z,int n)
{
	PartitionTree* pt = new PartitionTree(curTree);
	ProjectedTree part;
	double score;
	int nPartitions = 0;
	//consider each partition and sort them by magnitude along x,y,z
	std::priority_queue <pair<double,const Partition*>, std::vector< pair<double,const Partition*> > > PartitionSet; // Max-heap for scores w/ Partitions
	while(!PartitionSet.empty())
	{
		//printf("DEBUG: PartitionSet not EMPTY!\n");
		PartitionSet.pop();
	}
	for(std::list<Partition>::const_iterator i=pt->begin();i!=pt->end();i++)
	{
		part = mGDS->lookup(*i);
		score = x * part(0) + y * part(1) + z * part(2);
		score *= -1; //we want partitions which face opposite the given direction
		pair<double,const Partition*> result;
		result.first = score;
		result.second = &(*i);
		PartitionSet.push(result);
		nPartitions++;
		//i->print();
		//printf(" X\n");
		i++; //there are two of each partition in the list
		//i->print();
		//printf("\n");
	}
	
	//printf("DEBUG nPartitions = %d\n",nPartitions);
	
	int ct = 0;
	
	//printf("DEBUG: Adding Partitions\n");
	
	//add all but the n worst partitions
	//TODO this isn't quite right
	PartitionTree* new_pt= new PartitionTree(Interpreter::getInstance()->dataset()->ntaxa());
	for(int j=0;j<nPartitions - n;j++)
	{
		pair<double,const Partition*> value = PartitionSet.top();
		PartitionSet.pop();
		if(new_pt->isCompatible(*(value.second)))
		{
			ct++;
			//(value.second)->print();
			new_pt->add(*(value.second));
			//printf(" X\n");
			/*
			printf("%d:\t",j);
			(*(value.second)).print();
			printf("\t%.3f\n",value.first);
			*/
		}
		else
		{
			
			//(value.second)->print();
			//printf("\n");
			/*
			printf("X%d:\t",j);
			(*(value.second)).print();
			printf("\t%.3f\n",value.first);
			*/
		}

	}
	
	delete curTree;
	delete pt;
	NewickTreeParser* ntp = new NewickTreeParser();
	char* treestring = new_pt->treeString();
	curTree = ntp->parseBuffer(treestring,strlen(treestring),false);
	/*printf("%s\n%d partitions\n",treestring,ct);
	if(ct < 17)
	{
	 throw "Too Many Unresolutions!\n";
	}*/
	//curTree->showTree();
	return new_pt;
}

void ParticleTree::resolve(PartitionTree* unresolved,int partitions_needed)
{
	//find unresolved nodes
	

	//form list of possible partitions to resolve with
	int npartitions;
	Partition* partitions = NULL;
	unresolved->compatiblePartitions(npartitions,partitions);
	/*printf("Possible Partitions\n");
	for(int i=0;i<npartitions;i++)
	{
		partitions[i].print();
		printf("\n");
	}
	printf("%d compatiblePartitions\n",npartitions);*/
	bool* include = (bool*)malloc(sizeof(bool)*npartitions);
	ecr(unresolved,0,include,partitions,npartitions,partitions_needed);
	delete unresolved;
	
}



void ParticleTree::ecr(PartitionTree* resolved,int cur,bool* include,Partition* partitions,int npartitions,int partitions_needed)
{
	int i;
	
	/*for(i=0;i<cur;i++)
		if(include[i])
			printf("*");
		else
			printf(".");
	for(i=cur;i<npartitions;i++)
		printf("-");
	printf("\n");*/
	
	if(cur == npartitions)
	{
		//is this a fully resolved tree?
		{
			if(partitions_needed > 0) return;
		}
		//base case, construct tree
		PartitionTree tree(Interpreter::getInstance()->dataset()->ntaxa());
		for(std::list<Partition>::const_iterator iter = resolved->begin();iter != resolved->end();iter++)
			tree.add(*iter);
		for(i=0;i<npartitions;i++)
			if(include[i])
				tree.add(partitions[i]);
		char* treestring = tree.treeString();
		NewickTreeParser* ntp = new NewickTreeParser();
		QTree* qt = ntp->parseBuffer(treestring,strlen(treestring),false);
		//score tree
		
		Parsimony p_eval;
		double score = p_eval.qscoreTree(qt);
		//printf("Consider Tree - score %.0f\n",score);
		//qt->showTree();
		//if better then current best, replace
		if(mPosition.score > score)
		{
			improved = true;
			printf("Better Local Tree - score %.0f\n",score);
			qt->setTreeStr(treestring);
			mPosition = mGDS->map(qt);
			mPosition.score = qt->getScore();
			delete curTree;
			curTree = qt;
			printf("%s\n",curTree->treeStr());
			if(curTreeStr)
			{
				free(curTreeStr);
			}
			curTreeStr = strdup(treestring);
		}
		else
		{
			delete qt;
		}
	}
	else
	{
		//consider cur partition not in tree
		include[cur] = false;
		ecr(resolved,cur+1,include,partitions,npartitions,partitions_needed);
		//consider cur partition in tree
		PartitionTree conflict(Interpreter::getInstance()->dataset()->ntaxa());
		for(i=0;i<cur;i++)
			if(include[i])
				conflict.add(partitions[i]);
		if(conflict.isCompatible(partitions[cur])) //otherwise no valid trees on this branch
		{
			include[cur] = true;
			ecr(resolved,cur+1,include,partitions,npartitions,partitions_needed - 1);
		}
		
	}
}
