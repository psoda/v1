#ifndef EVALUATOR_FAST_PARSIMONY 
#define EVALUATOR_FAST_PARSIMONY

//#include "ArrayBase.h"
#include "EvaluatorBase.h"
#include "FastParsimonyNodeData.h"
#include "QTree.h"
#include "QNodeInfo.h"
#include "QNode.h"
#include "Parsimony.h"

class Node;
class Dataset;
class Interpreter;
class TBREvaluateData;

typedef char v16qi __attribute__ ((vector_size (16)));
typedef char v8qi __attribute__ ((vector_size (8)));
typedef float v4sf __attribute__ ((vector_size (16)));



class FastParsimony : public EvaluatorBase
{
  //typedef Array<ParsimonyNodeData> ParsimonyNodeDataArray;
  public:
  FastParsimony();
  private:
  FastParsimony( const FastParsimony& parsimony );
  FastParsimony& operator=( const FastParsimony& parsimony );
  public:
  virtual ~FastParsimony();

  c16vector*& weights() { return mWeights; }
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
  int calculateParsimonyScoreOnly( c16vector* src1 , c16vector* src2 , c16vector* weights, int maxdiff );
  int calculateParsimonyScore( c16vector* dest , c16vector* src1 , c16vector* src2 , c16vector* weights );
  double QinitializeTree(QTree* tree );
  void QinitNodeDatas(QTree *tree);

  protected:
  //parents NOT DESTRUCTED
  c16vector* mWeights;
  c16vector zero;
  c16vector ones;
  int mSize;
  bool mFallback;  //if any weight is greater than 255 regular parsimony must be used
  Parsimony mSlow;  //for use if mFallback is true
  //children DESTRUCT
  //ParsimonyNodeDataArray mNodeDatas;

};
#endif

