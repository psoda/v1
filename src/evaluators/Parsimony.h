#ifndef EVALUATOR_PARSIMONY 
#define EVALUATOR_PARSIMONY

//#include "ArrayBase.h"
#include "EvaluatorBase.h"
//#include "ParsimonyNodeData.h"
#include "QTree.h"
#include "QNode.h"
#include "QNodeInfo.h"

class Node;
class Dataset;
class Interpreter;
class TBREvaluateData;

class Parsimony : public EvaluatorBase
{
  //typedef Array<ParsimonyNodeData> ParsimonyNodeDataArray;
  public:
  Parsimony();
  private:
  Parsimony( const Parsimony& parsimony );
  Parsimony& operator=( const Parsimony& parsimony );
  public:
  virtual ~Parsimony();

  int*& weights() { return mWeights; }
  //ParsimonyNodeDataArray& nodeDatas() { return mNodeDatas; }

  double branchScore(QNodeInfo*);
  void qpreProcess( QTree* tree );
  double qevaluateTree( QTree* tree, QNodeInfo *node1, QNodeInfo *node2 );
  void qassign( QTree *tree, QNode *destNode, ParsimonySiteDataIndex dest, QNode *srcNode, ParsimonySiteDataIndex src, int nchars );
  void qinitNodeDatas(QTree *tree);
  void qpostOrderPass( QTree* tree, QNode *root);
  void qpreOrderPass( QTree* tree, QNode *root );
  int qfinalScoreNode( QTree *tree, QNode* node );
  void qpreOrderPassRecurse( QTree *tree, QNode* node, QNode* sibling );
  int qpostOrderPassRecurse( QTree * tree, QNode* node );
  double qscoreTree( QTree* tree);

  void init();
  
  bool PostChanged(QNode *node);
  bool PreChanged(QNode *node, QNode *sibling);

  public: 
  static const char* parsimonyDatasetKey;

  protected:
  int calculateParsimonyScoreOnly( int nchar , char* dest , char* src1 , char* src2 , int* weights, int maxdiff );
  int calculateParsimonyScore( int nchar , char* dest , char* src1 , char* src2 , int* weights );
  double QinitializeTree(QTree* tree );
  void QinitNodeDatas(QTree *tree);

  protected:
  //parents NOT DESTRUCTED
  int* mWeights;

  //children DESTRUCT
  //ParsimonyNodeDataArray mNodeDatas;

};
#endif

