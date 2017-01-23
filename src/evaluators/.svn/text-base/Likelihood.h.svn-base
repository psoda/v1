#ifndef EVALUATOR_Likelihood 
#define EVALUATOR_Likelihood


#include "ArrayBase.h"
#include "EvaluatorBase.h"
#include "QTree.h"
#include "QNodeInfo.h"
#include "Dataset.h"

class QTree;
class Dataset;
//class Weights;
class Interpreter;
class TBREvaluateData;


class Likelihood : public EvaluatorBase
{
  //typedef Array<LikelihoodNodeData> LikelihoodNodeDataArray;
  public:
  Likelihood();
  private:
  Likelihood( const Likelihood& Likelihood );
  Likelihood& operator=( const Likelihood& Likelihood );
  public:
  virtual ~Likelihood();

  //Weights*& weightsOrig() { return mWeights; }
  //LikelihoodNodeDataArray& nodeDatas() { return mNodeDatas; }

  double branchScore(QNodeInfo*);
  void qpreProcess( QTree* tree);
  double qevaluateTree( QTree* tree, QNodeInfo *node1, QNodeInfo *node2 );
  //void qassign( QTree *tree, QNode *destNode, LikelihoodSiteDataIndex dest, QNode *srcNode, LikelihoodSiteDataIndex src, int nchars );
  void qinitNodeDatas(QTree *tree);
  void qpostOrderPass( QTree* tree, QNode *root);
  void qpreOrderPass( QTree* tree, QNode *root );
  int qfinalScoreNode( QTree *tree, QNode* node );
  int qpreOrderPassRecurse( QTree *tree, QNode* node, QNode* sibling );
  int qpostOrderPassRecurse( QTree * tree, QNode* node );
  double qscoreTree( QTree* tree);

  void init( );
  void init( Interpreter* interpreter );
  void initLikelihoodVariables();
  void freeLikelihoodVariables();

  void treeEvaluate (QTree *tr);       /* Evaluate a user tree */

  public: 
  static const char* LikelihoodDatasetKey;

  protected:
  //parents NOT DESTRUCTED
  //Weights* mWeights;
  
};


#endif

