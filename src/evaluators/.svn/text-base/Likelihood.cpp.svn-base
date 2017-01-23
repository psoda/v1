#include "Likelihood.h"
#include "Interpreter.h"
#include "Dataset.h"
#include "QNodeInfo.h"
#include "QTree.h"
//#include "Weights.h"
#include "QTreeRepository.h"
#include "raxml.h"


const char* Likelihood::LikelihoodDatasetKey = "default";

Likelihood::Likelihood()
{
}

Likelihood::Likelihood( const Likelihood& orig ):EvaluatorBase(orig)
{
}

Likelihood& Likelihood::operator=( const Likelihood& orig )
{
  if ( this != &orig )
  {
  }
  return *this;
}

Likelihood::~Likelihood()
{
}

double Likelihood::branchScore(QNodeInfo*)
{
	return 0.0;
}

void Likelihood::init( Interpreter* /*interpreter*/ )
{

}


void Likelihood::qinitNodeDatas(QTree * /*treet*/)
{
	
}


// Preprocess both of the subtrees so that they can be easily scored
void Likelihood::qpreProcess( QTree* /*tree*/ )
{

}

// Score tree from scratch
double Likelihood::qscoreTree( QTree* qtree)
{
  //printf("qscoreTree\n");  
  treeEvaluate(qtree);
  //printf("Done. Score is %lf\n\n", qtree->score());
  return qtree->getScore();
}

// Score the tree if the subtrees were joined at these two nodes
// The old code used to call tbrevaluate
// Returns -1 if pruned, otherwise a positive number
double Likelihood::qevaluateTree( QTree* tree, QNodeInfo *node1, QNodeInfo *node2 )
{
	//First, join the tree at those nodes
    QTree *newTree = tree->createSpecificTree(node1,node2,true);
    // Now we have a new tree we can mess with

    treeEvaluate(newTree);
    double score = newTree->getScore();
    //	printf("Done. Score is %lf\n\n", score);
	
	//Now delete the joined tree
    delete(newTree);
	
	return (score);
	
}

void Likelihood::treeEvaluate( QTree* qtree )
{
  raxml_score(qtree);
}
