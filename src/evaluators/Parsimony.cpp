#include "Parsimony.h"
#include "Interpreter.h"
#include "Dataset.h"
#include "QNodeInfo.h"
#include "QTree.h"
#include "Globals.h"

#undef verboseScoring

int verbosescoring = 1;

const char* Parsimony::parsimonyDatasetKey = "default";

Parsimony::Parsimony()
{
}


Parsimony::Parsimony( const Parsimony& orig ):EvaluatorBase(orig)
{
}

double  Parsimony::branchScore(QNodeInfo* info)
{
	return info->nodeData()->score(PSDI_FINAL) - (info->nodeData()->score(PSDI_PRE)+info->nodeData()->score(PSDI_POST));
}

Parsimony& Parsimony::operator=( const Parsimony& orig )
{
  if ( this != &orig )
  {
  }
  return *this;
}

Parsimony::~Parsimony()
{
}



void Parsimony::init()
{
#ifdef verboseScoring
if(verbosescoring) printf("init\n");
#endif
  weights() = Interpreter::getInstance()->dataset()->weights();
  //int n2_1 = Interpreter::getInstance()->dataset()->n2_1();
}

void Parsimony::qassign( QTree * tree __attribute__ ((unused)), QNode *destNode, ParsimonySiteDataIndex dest, QNode *srcNode, ParsimonySiteDataIndex src, int nchars )
{
#ifdef verboseScoring
if(verbosescoring) printf("qassign destnode %d srcnode %d\n",destNode->nodeInfo()->nodeIndex(),srcNode->nodeInfo()->nodeIndex());
#endif
  memcpy ( destNode->nodeInfo()->nodeData()->siteData(dest), srcNode->nodeInfo()->nodeData()->siteData(src), nchars );
  destNode->nodeInfo()->nodeData()->score(dest) = srcNode->nodeInfo()->nodeData()->score(src);
}

void Parsimony::qinitNodeDatas(QTree *tree)
{
	Dataset* dataset = Interpreter::getInstance()->dataset();
#ifdef verboseScoring
if(verbosescoring) printf("qinitNodeDatas tree %d dataset %X\n",tree->root()->nodeInfo()->nodeIndex(),dataset);
#endif
//    printf("nchars is %d\n",tree->nchars());

// Turn this optimization on once scoring is working
#ifdef notdef
    if (tree->nodeDataInitialized())
    {
        return;
    }
    tree->nodeDataInitialized() = true;
#endif

    //set up nodeDatas with initial data
    QDeque <QNodeInfo *> *nodes = tree->nodeInfoList();
    QDeque<QNodeInfo *>::iterator nodePointer; 

    for(nodePointer = nodes->begin(); nodePointer != nodes->end(); nodePointer++)
    {
        // If data for this node has already bee allocated
        if((*nodePointer)->nodeData())
            continue;
        (*nodePointer)->nodeData() = new ParsimonyNodeData();
        if ( (*nodePointer)->leaf() )
        {
            (*nodePointer)->nodeData()->init( dataset->getCharacters((*nodePointer)->nodeIndex(),true), dataset->nchars() );
        }
        else
        {
            (*nodePointer)->nodeData()->init(dataset->nchars() );
        }
    } 
#ifdef verboseScoring
if(verbosescoring)     
    for(nodePointer = nodes->begin(); nodePointer != nodes->end(); nodePointer++)
    {
        char* siteData = (*nodePointer)->nodeData()->siteData(PSDI_POST);
        //printf("qinitNodeData: nodeData[%d]", (*nodePointer)->nodeIndex());
        if ((*nodePointer)->leaf())
        {
            printf("qinitNodeData: nodeData[%d]", (*nodePointer)->nodeIndex());
            printf(" = %s\n", dataset->getTaxonName((*nodePointer)->nodeIndex()));
            for(int column = 0; column < tree->nchars(); column++)
            {
                printf("[%d]=%x",column,siteData[column]);
            }
            printf("\n");
        }
        else
        {
            printf("qinitNodeData: nodeData[%d]\n", (*nodePointer)->nodeIndex());
            for(int column = 0; column < tree->nchars(); column++)
            {
                printf("[%d]=%x",column,siteData[column]);
            }
            printf("\n");
        }
    }
#endif
}



// Preprocess both of the subtrees so that they can be easily scored
// Mark the calculation root during preorderpass, so we will know which link should be 
//  split to get this score.
void Parsimony::qpreProcess( QTree* tree)
{
  //Dataset* dataset = Interpreter::getInstance()->dataset();
#ifdef verboseScoring
if(verbosescoring) printf("qpreProcess root %d\n",tree->root()->nodeInfo()->nodeIndex());
#endif
    // Allocate space for the nodedatas first
    qinitNodeDatas(tree);

#ifdef verboseScoring
if(verbosescoring) printf("qpreProcess tree1 %d\n",tree->subTree1()->nodeInfo()->nodeIndex());
#endif
    // Preprocess subtree 1
    qpostOrderPass( tree, tree->subTree1() );
    qpreOrderPass( tree, tree->subTree1() );
    tree->subTreeScore1() = tree->subTree1()->nodeInfo()->nodeData()->score(PSDI_FINAL);

#ifdef verboseScoring
if(verbosescoring) printf("qpreProcess tree2 %d\n",tree->subTree2()->nodeInfo()->nodeIndex());
#endif
    // Preprocess subtree 2
    qpostOrderPass( tree, tree->subTree2() );
    qpreOrderPass( tree, tree->subTree2() );
    tree->subTreeScore2() = tree->subTree2()->nodeInfo()->nodeData()->score(PSDI_FINAL);
}

// This is called in the same way as singlePassEvaluate
double Parsimony::qscoreTree( QTree* tree )
{
  //Dataset* dataset = Interpreter::getInstance()->dataset();
#ifdef verboseScoring
if(verbosescoring) printf("qscoreTree (SinglePassevaluate on %d\n",tree->root()->nodeInfo()->nodeIndex());
#endif
  qinitNodeDatas(tree);
  qpostOrderPass( tree, tree->root() );
#ifdef notdef // For some reason singlepass doesnt do this so we wont either
  qpreOrderPass( tree, tree->root() );
#endif
  tree->setScore( tree->root()->nodeInfo()->nodeData()->score(PSDI_FINAL) );
  return tree->getScore();
}

void Parsimony::qpreOrderPass( QTree* tree, QNode *root )
{
  QNode* node = root;
  QNode* top = root->external();
  int nchars = tree->nchars();

  // If this subtree has an external
  if ( top )
  {
    /* copy top POST data to self PRE and 
     * self POST to top PRE */
    qassign( tree, node, PSDI_PRE, top, PSDI_POST, nchars );
    qassign( tree, top, PSDI_PRE, node, PSDI_POST, nchars );
    // Mark this node (out of the tri) as the one to break
    // in order get the tree this score corresponds to.
    root->nodeInfo()->node() = root;
    top->nodeInfo()->node() = top;

#ifdef verboseScoring
if(verbosescoring) printf("qpreOrderPass top for me %d top %d\n",node->nodeInfo()->nodeIndex(), top->nodeInfo()->nodeIndex());
#endif

    /* compute FINAL from POST and PRE */
    qfinalScoreNode( tree, node );
    // This guy is hosed -- the second entry has all zeros
    qfinalScoreNode( tree, top );

    /* recurse on SELF */
    // If not a leaf
    if ( !(node->nodeInfo()->leaf()) )
    {
      qpreOrderPassRecurse( tree, node->child1(), node->child2() );
      qpreOrderPassRecurse( tree, node->child2(), node->child1() );
    }

    /* recurse on TOP child */
    // If not a leaf
    if ( !(top->nodeInfo()->leaf()) )
    {
      qpreOrderPassRecurse( tree, top->child1(), top->child2() );
      qpreOrderPassRecurse( tree, top->child2(), top->child1() );
    }
  }
  else
  {
    /* at least two nodes */
    if ( !(node->nodeInfo()->leaf()) )
    {
      QNode* child1 = node->child1(); // LEFT child
      QNode* child2 = node->child2(); // RIGHT child

#ifdef verboseScoring
if(verbosescoring) printf("qpreOrderPass at least 2 nodes for me %d child1 %d child2 %d\n",node->nodeInfo()->nodeIndex(), child1->nodeInfo()->nodeIndex(), child2->nodeInfo()->nodeIndex());
#endif
      /* copy left POST data to right PRE and 
       * right POST to LEFT PRE */
      qassign( tree, child1, PSDI_PRE, child2, PSDI_POST, nchars );
      qassign( tree, child2, PSDI_PRE, child1, PSDI_POST, nchars );

      /* compute FINAL from POST and PRE */
      qfinalScoreNode( tree, child1 );
      qfinalScoreNode( tree, child2 );

      /* recurse on LEFT child */
      if ( !(child1->nodeInfo()->leaf()) )
      {
        qpreOrderPassRecurse( tree, child1->child1(), child1->child2() );
        qpreOrderPassRecurse( tree, child1->child2(), child1->child1() );
      }

      /* recurse on RIGHT child */
      if ( !(child2->nodeInfo()->leaf()) )
      {
        qpreOrderPassRecurse( tree, child2->child1(), child2->child2() );
        qpreOrderPassRecurse( tree, child2->child2(), child2->child1() );
      }
    }
  // This is a single taxa (stepwise addition)
    else
    {
#ifdef verboseScoring
if(verbosescoring) printf("qpreOrderPass single node for me %d \n",node->nodeInfo()->nodeIndex());
#endif
        qassign( tree, node, PSDI_FINAL, node, PSDI_POST, nchars );
        node->nodeInfo()->nodeData()->score(PSDI_PRE) = 0 ;
        node->nodeInfo()->nodeData()->score(PSDI_FINAL) = 0 ;
    }
  }
}

bool Parsimony::PreChanged(QNode *node, QNode *sibling)
{
	bool retval;
	if (node->nodeInfo()->preChild1() == node->external()->nodeInfo()->nodeIndex() && 
		node->nodeInfo()->preChild2() == sibling->nodeInfo()->nodeIndex() && 
		node->external()->nodeInfo()->preChanged() == false &&
		sibling->nodeInfo()->postChanged() == false)
	{
		retval = false;
	}
	else
	{
		retval = true;
		node->nodeInfo()->preChild1() = node->external()->nodeInfo()->nodeIndex();
		node->nodeInfo()->preChild2() = sibling->nodeInfo()->nodeIndex();
		node->nodeInfo()->preChanged() = true;	
		/*
		 printf("PostChanged child1[%d != %d] or child2[%d != %d] or child1Changed=%d or child2Changed=%d\n",
				node->nodeInfo()->postChild1(), node->child1()->nodeInfo()->nodeIndex(),
				node->nodeInfo()->postChild2(), node->child2()->nodeInfo()->nodeIndex(), 
				node->child1()->nodeInfo()->postChanged(),
				node->child2()->nodeInfo()->postChanged());
		 */
	}
	return retval;
}


void Parsimony::qpreOrderPassRecurse( QTree *tree, QNode* node, QNode* sibling )
{
#ifdef verboseScoring
if(verbosescoring) printf("qpreOrderPassRecurse node %d sibling %d\n",node->nodeInfo()->nodeIndex(),sibling->nodeInfo()->nodeIndex());
#endif
  int differences = 0 ;

  char* subTree0SiteData = node->nodeInfo()->nodeData()->siteData(PSDI_PRE);          /* SELF */
  char* subTree1SiteData = node->external()->nodeInfo()->nodeData()->siteData(PSDI_PRE);    /* PARENT */
  char* subTree2SiteData = sibling->nodeInfo()->nodeData()->siteData(PSDI_POST);  /* SIBLING */

  int differences1 = (int) node->external()->nodeInfo()->nodeData()->score(PSDI_PRE);
  int differences2 = (int) sibling->nodeInfo()->nodeData()->score(PSDI_POST);
#ifdef verboseScoring
if(verbosescoring)   printf("preorder calculating for %d and %d\n",node->nodeInfo()->nodeIndex(),node->external()->nodeInfo()->nodeIndex());
#endif

   int differences3;
//#define PREQUINN
#ifdef PREQUINN
	if (PreChanged(node, sibling))
	{
		differences3 = calculateParsimonyScore( tree->nchars(), subTree0SiteData, subTree1SiteData, subTree2SiteData, tree->weights() ) ;
		node->nodeInfo()->preDifferences() = differences3;
	}
	else
	{
		printf("No pre calc needed\n");
		differences3 = node->nodeInfo()->preDifferences();
	}
#else
	differences3 = calculateParsimonyScore( tree->nchars(), subTree0SiteData, subTree1SiteData, subTree2SiteData, tree->weights() ) ;
#endif

  differences = differences1 + differences2 + differences3;
  node->nodeInfo()->nodeData()->score(PSDI_PRE) = differences ;
    // Mark this node (out of the tri) as the one to break
    // in order get the tree this score corresponds to.
  node->nodeInfo()->node() = node;

  // Not a leaf
  if ( !(node->nodeInfo()->leaf()) )
  {
    qpreOrderPassRecurse ( tree, node->child1(), node->child2() );
    qpreOrderPassRecurse ( tree, node->child2(), node->child1() );
  }

  qfinalScoreNode( tree, node );
}

int prunetimer = 0;
// Score the tree if the subtrees were joined at these two nodes
// The old code used to call tbrevaluate
// Returns -1 if pruned, otherwise a positive number
double Parsimony::qevaluateTree( QTree* tree, QNodeInfo *node1, QNodeInfo *node2 )
{
#ifdef verboseScoring
if(verbosescoring)
{
    int maxindex = Interpreter::getInstance()->dataset()->ntaxa() * 2 -1;
    printf("qevaluateTree root %d node1 %d node2 %d\n",tree->root()->nodeInfo()->nodeIndex(),node1->nodeIndex(),node2->nodeIndex());
    if(node1->nodeIndex() > maxindex)
        printf("ALERT: node1 is not valid!, index greater than %d\n",maxindex);
    if(node2->nodeIndex() > maxindex)
        printf("ALERT: node2 is not valid!, index greater than %d\n",maxindex);
}
#endif
  int differences = 0;
  int differences1 = 0;
  int differences2 = 0;
  int differences3 = 0;
 // int qdifferences3 = 0;

  char* subTree1SiteData = node1->nodeData()->siteData(PSDI_FINAL);
  char* subTree2SiteData = node2->nodeData()->siteData(PSDI_FINAL);

  //ParsimonyPrintNode ( parsimony , nodeNumberTree0 ) ;
  //ParsimonyPrintNode ( parsimony , nodeNumberTree1 ) ;


  // Add up the scores looking both ways from the join nodes
  // Then add in the differences between the two nodes
  differences1 = (int)  node1->nodeData()->score(PSDI_FINAL);
  differences2 = (int)  node2->nodeData()->score(PSDI_FINAL);
  differences = differences1 + differences2;

#ifdef notdef
  if(prunetimer >= 1000000) {
    printf("earlyprune %d, %d, %d earlyprune %d\n",differences1, differences2, differences, tree->earlyPrune());
    prunetimer = 0;
  } else {
    prunetimer++;
  }
#endif
  if((AlgorithmFlags & EARLYPRUNE) && (differences > tree->earlyPrune()))
  {
//printf("earlyprune %d, %d, %d earlyprune %d\n",differences1, differences2, differences, tree->earlyPrune());
    return -1;
  }


  // Calculate max difference allowed for this tree
  int maxdiff = tree->earlyPrune() - differences2 - differences1;
  differences3 += calculateParsimonyScoreOnly( tree->nchars(), NULL , subTree1SiteData, subTree2SiteData, tree->weights(), maxdiff);
  differences = differences1 + differences2 + differences3 ;

#ifdef verboseScoring
if(verbosescoring)   printf("QevaluateTree, differences1 %d, differences2 %d, differences3 %d differences %d\n",differences1, differences2, differences3, differences);
#endif

  if(differences < tree->earlyPrune())
      tree->earlyPrune() = differences;

  return differences ;
}

int Parsimony::qfinalScoreNode( QTree *tree, QNode* node )
{
#ifdef verboseScoring
if(verbosescoring) printf("qfinalScoreNode\n");
#endif
  int differences = 0;

  char* subTree0SiteData = node->nodeInfo()->nodeData()->siteData(PSDI_FINAL);
  char* subTree1SiteData = node->nodeInfo()->nodeData()->siteData(PSDI_POST);
  char* subTree2SiteData = node->nodeInfo()->nodeData()->siteData(PSDI_PRE);

  int differences1 = (int) node->nodeInfo()->nodeData()->score(PSDI_POST);
  int differences2 = (int) node->nodeInfo()->nodeData()->score(PSDI_PRE);

  int differences3 = calculateParsimonyScore( tree->nchars(), subTree0SiteData, subTree1SiteData, subTree2SiteData, tree->weights()) ;
#ifdef verboseScoring
if(verbosescoring)   printf("Q FinalScoreNode calculating for %d differences1(Post)=%d, differences2(Pre)=%d differences3(scan)=%d\n",node->nodeInfo()->nodeIndex(),differences1, differences2, differences3);
#endif
  
  differences = differences1 + differences2 + differences3;
  node->nodeInfo()->nodeData()->score(PSDI_FINAL) = differences ;

  return differences ;
}

void Parsimony::qpostOrderPass( QTree* tree, QNode *root)
{
#ifdef verboseScoring
if(verbosescoring) printf("qpostOrderPass\n");
#endif
  int nchars = tree->nchars();
    /* rootless */
    if ( root->external() )
    {

    /* copy top POST data to self PRE and 
     * self POST to top PRE */
      if ( !(root->nodeInfo()->leaf()) )
      {
        qpostOrderPassRecurse( tree, root );
      }
      // If the external is not a leaf, recurse
      if ( !(root->external()->nodeInfo()->leaf()) )
      {
        qpostOrderPassRecurse( tree, root->external() );
      }
      
      qassign( tree, root, PSDI_PRE, root->external(), PSDI_POST, nchars );
      qfinalScoreNode( tree , root );
      
    }
    /* rooted, only has children so score them */
    else
    {
      qpostOrderPassRecurse( tree, root );
    }
}

bool Parsimony::PostChanged(QNode *node)
{
	bool retval;
	if (node->nodeInfo()->postChild1() == node->child1()->nodeInfo()->nodeIndex() && 
		node->nodeInfo()->postChild2() == node->child2()->nodeInfo()->nodeIndex() && 
		node->child1()->nodeInfo()->postChanged() == false &&
		node->child2()->nodeInfo()->postChanged() == false)
	{
		retval = false;
	}
	else
	{
		retval = true;
		node->nodeInfo()->postChild1() = node->child1()->nodeInfo()->nodeIndex();
		node->nodeInfo()->postChild2() = node->child2()->nodeInfo()->nodeIndex();
		node->nodeInfo()->postChanged() = true;	
		/*
		printf("PostChanged child1[%d != %d] or child2[%d != %d] or child1Changed=%d or child2Changed=%d\n",
			   node->nodeInfo()->postChild1(), node->child1()->nodeInfo()->nodeIndex(),
			   node->nodeInfo()->postChild2(), node->child2()->nodeInfo()->nodeIndex(), 
			   node->child1()->nodeInfo()->postChanged(),
			   node->child2()->nodeInfo()->postChanged());
		*/
	}
	return retval;
}

int Parsimony::qpostOrderPassRecurse( QTree * tree, QNode* node )
{
#ifdef verboseScoring
if(verbosescoring) printf("qpostOrderPassRecurse\n");
#endif
  int differences = 0 ;

  if ( !(node->nodeInfo()->leaf()))
  {
    char* subTree0SiteData = node->nodeInfo()->nodeData()->siteData(PSDI_POST) ;
    char* subTree1SiteData = node->child1()->nodeInfo()->nodeData()->siteData(PSDI_POST) ;
    char* subTree2SiteData = node->child2()->nodeInfo()->nodeData()->siteData(PSDI_POST) ;

    int differences1 = qpostOrderPassRecurse( tree, node->child1() ) ;
    int differences2 = qpostOrderPassRecurse( tree, node->child2() ) ;
	int differences3;
	
#define QUINN
#ifdef QUINN
	if (PostChanged(node))
	{
		differences3 = calculateParsimonyScore( tree->nchars(), subTree0SiteData, subTree1SiteData, subTree2SiteData, tree->weights() ) ;
		node->nodeInfo()->postDifferences() = differences3;
	}
	else
	{
		//printf("No calc needed\n");
		differences3 = node->nodeInfo()->postDifferences();
	}
#else
	differences3 = calculateParsimonyScore( tree->nchars(), subTree0SiteData, subTree1SiteData, subTree2SiteData, tree->weights() ) ;
#endif
	
	differences = differences1 + differences2 + differences3;
    // printf("me %d nchars %d differences1 %d differences2 %d differences3 %d differences %d child1 %d child2 %d\n",node->nodeInfo()->nodeIndex(), tree->nchars(),differences1,differences2,differences3,differences,node->child1()->nodeInfo()->nodeIndex(),node->child2()->nodeInfo()->nodeIndex());

    node->nodeInfo()->nodeData()->score(PSDI_POST) = differences ;
#ifdef verboseScoring
 if(verbosescoring)  printf("postorder calculating for %s[%d] using %s[%d] and %s[%d] returning %d\n",
         Interpreter::getInstance()->dataset()->getTaxonName(node->nodeInfo()->nodeIndex()), node->nodeInfo()->nodeIndex(),
         Interpreter::getInstance()->dataset()->getTaxonName(node->child1()->nodeInfo()->nodeIndex()), node->child1()->nodeInfo()->nodeIndex(),
         Interpreter::getInstance()->dataset()->getTaxonName(node->child2()->nodeInfo()->nodeIndex()), node->child2()->nodeInfo()->nodeIndex(),differences);
#endif
    return differences ;
  }
  else
  {
    return 0 ;
  }
}


int Parsimony::calculateParsimonyScoreOnly( int nchar , char* dest , char* src1 , char* src2 , int* weights, int maxdiff )
{
  int differences = 0 ;
#ifdef verboseScoring
if(verbosescoring)   printf("CPSO: %d dest %X\n",nchar,dest);
#endif

  for ( int i = 0 ; i < nchar ; i ++ )
  {
      if((AlgorithmFlags & REALYEARLYPRUNE) && (differences > maxdiff))
          break;
      if (!dest)
      {
	differences += !( ( src1[i] & src2[i] ) && 1 ) * weights[i];
//	differences += !( ( src1[i] & src2[i] ) && 1 );
      }
      else
      {
          dest[i] = !( ( src1[i] & src2[i] ) && 1 );
          differences += dest[i] * weights[i];
//          differences += dest[i];
      }
#ifdef verboseScoring
if(verbosescoring)       
{
      if(dest)
	printf("(%d)([%x,%x]=%x)*%d=%d\n",i,src1[i],src2[i],dest[i],weights[i],differences);
      else
	printf("(%d)([%x,%x])*%d=%d\n",i,src1[i],src2[i],weights[i],differences);
}
#endif
  }
#ifdef verboseScoring
if(verbosescoring)       
{
    printf("returning differences %d\n",differences);
}
#endif
  return differences ;
}

int Parsimony::calculateParsimonyScore( int nchar , char* dest , char* src1 , char* src2 , int* weights )
{
  int differences = 0 ;
#ifdef verboseScoring
if(verbosescoring)   printf("CPS: nchars %d dest %X\n",nchar,dest);
#endif

/* calculate intermediate data for this node */


  for (int i = 0; i < nchar; i++)
  {
	  if ( ! ( *dest = *src1 & *src2 ) )
      {
          differences += weights[i] ;
          //differences ++;
		  *dest = *src1 | *src2 ;
	  }
	  dest++;
	  src1++;
	  src2++;
  }

  return differences ;
}
