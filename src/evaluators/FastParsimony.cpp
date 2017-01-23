#include "FastParsimony.h"
#include "Interpreter.h"
#include "Dataset.h"
#include "QNodeInfo.h"
#include "QTree.h"
#include "Globals.h"
#include "FastParsimonyNodeData.h"

#undef verboseScoring
#undef VERBOSE_FAST_PARSIMONY_SCOREING

double GlobalDouble;

int FPverbosescoring = 1;

const char* FastParsimony::parsimonyDatasetKey = "default";

FastParsimony::FastParsimony()
{
#ifdef notdef
	for(int i=0;i<16;i++)
	{
		zero.c[i] = 0;
		ones.c[i] = 0xFF;
	}
	init();
#endif
}


double  FastParsimony::branchScore(QNodeInfo* info)
{
#ifdef notdef
	return info->nodeDataFP()->score(PSDI_FINAL) - (info->nodeDataFP()->score(PSDI_PRE)+info->nodeDataFP()->score(PSDI_POST));
#endif
}


FastParsimony::FastParsimony( const FastParsimony& orig ):EvaluatorBase(orig)
{
#ifdef notdef
	for(int i=0;i<16;i++)
	{
		zero.c[i] = 0;
		ones.c[i] = 0xFF;
	}
#endif
}

FastParsimony& FastParsimony::operator=( const FastParsimony& orig )
{
#ifdef notdef
  if ( this != &orig )
  {
  }
  return *this;
#endif
}

FastParsimony::~FastParsimony()
{
}



void FastParsimony::init()
{
#ifdef notdef
#ifdef verboseScoring
if(FPverbosescoring) printf("init\n");
#endif
  mSize = Interpreter::getInstance()->dataset()->nchars() / 16;
  if(Interpreter::getInstance()->dataset()->nchars() % 16 != 0)
	mSize++;
	
	mWeights = (c16vector*)malloc(sizeof(c16vector)*mSize);
	memset(mWeights,0,sizeof(c16vector)*mSize);
	int* weights = Interpreter::getInstance()->dataset()->weights();
	mFallback = false;
	for(int i=0;i<mSize;i++)
	{
		for(int j=0;j<16;j++)
		{
			if(i*16+j < Interpreter::getInstance()->dataset()->nchars())
				if(!weights)
				{
					mWeights[i].c[i] = 1;
				}
				else if(weights[i*16+j] >= 255)
				{
					mFallback = true;
				}
				else
				{
					mWeights[i].c[j] = weights[i*16+j];
				}
		}
	}
	if(mFallback)
	{
		free (mWeights);  //we will not be able to use any vector instructions.
		printf("Fast Parsimony Fallback\n");
	}
	else
	{
		printf("Fast Parsimony\n");
	}
#endif
}

void FastParsimony::qassign( QTree * tree __attribute__ ((unused)), QNode *destNode, ParsimonySiteDataIndex dest, QNode *srcNode, ParsimonySiteDataIndex src, int nchars __attribute__ ((unused)) )
{
#ifdef notdef
#ifdef verboseScoring
if(FPverbosescoring) printf("qassign destnode %d srcnode %d\n",destNode->nodeInfo()->nodeIndex(),srcNode->nodeInfo()->nodeIndex());
#endif
  memcpy ( destNode->nodeInfo()->nodeDataFP()->siteData(dest), srcNode->nodeInfo()->nodeDataFP()->siteData(src), mSize * sizeof(c16vector) );
  destNode->nodeInfo()->nodeDataFP()->score(dest) = srcNode->nodeInfo()->nodeDataFP()->score(src);
#endif
}

void FastParsimony::qinitNodeDatas(QTree *tree)
{
#ifdef notdef
	Dataset* dataset = Interpreter::getInstance()->dataset();

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
        if((*nodePointer)->nodeDataFP())
            continue;
        (*nodePointer)->nodeDataFP() = new FastParsimonyNodeData();
        if ( (*nodePointer)->leaf() )
        {
            (*nodePointer)->nodeDataFP()->init( dataset->getCharacters((*nodePointer)->nodeIndex(),true), dataset->nchars() );
        }
        else
        {
            (*nodePointer)->nodeDataFP()->init(dataset->nchars() );
        }
    } 
#ifdef verboseScoring
if(FPverbosescoring)     
    for(nodePointer = nodes->begin(); nodePointer != nodes->end(); nodePointer++)
    {
        c16vector* siteData = (*nodePointer)->nodeDataFP()->siteData(PSDI_POST);
        //printf("qinitNodeData: nodeData[%d]", (*nodePointer)->nodeIndex());
        if ((*nodePointer)->leaf())
        {
            printf("qinitNodeData: nodeData[%d]", (*nodePointer)->nodeIndex());
            printf(" = %s\n", dataset->getTaxonName((*nodePointer)->nodeIndex()));
            for(int column = 0; column < tree->nchars(); column++)
            {
                printf("[%d]=%x",column,siteData[column/16].c[column%16]);
            }
            printf("\n");
        }
        else
        {
            printf("qinitNodeData: nodeData[%d]\n", (*nodePointer)->nodeIndex());
            for(int column = 0; column < tree->nchars(); column++)
            {
                printf("[%d]=%x",column,siteData[column/16].c[column%16]);
            }
            printf("\n");
        }
    }
#endif
#endif
}



// Preprocess both of the subtrees so that they can be easily scored
// Mark the calculation root during preorderpass, so we will know which link should be 
//  split to get this score.
void FastParsimony::qpreProcess( QTree* tree)
{
#ifdef notdef
	if(mFallback)
	{
		mSlow.qpreProcess(tree);
		return;
	}
  //Dataset* dataset = Interpreter::getInstance()->dataset();
#ifdef verboseScoring
if(FPverbosescoring) printf("qpreProcess root %d\n",tree->root()->nodeInfo()->nodeIndex());
#endif
    // Allocate space for the nodedatas first
    qinitNodeDatas(tree);

#ifdef verboseScoring
if(FPverbosescoring) printf("qpreProcess tree1 %d\n",tree->subTree1()->nodeInfo()->nodeIndex());
#endif
    // Preprocess subtree 1
    qpostOrderPass( tree, tree->subTree1() );
    qpreOrderPass( tree, tree->subTree1() );
    tree->subTreeScore1() = tree->subTree1()->nodeInfo()->nodeDataFP()->score(PSDI_FINAL);

#ifdef verboseScoring
if(FPverbosescoring) printf("qpreProcess tree2 %d\n",tree->subTree2()->nodeInfo()->nodeIndex());
#endif
    // Preprocess subtree 2
    qpostOrderPass( tree, tree->subTree2() );
    qpreOrderPass( tree, tree->subTree2() );
    tree->subTreeScore2() = tree->subTree2()->nodeInfo()->nodeDataFP()->score(PSDI_FINAL);
#endif
}

// This is called in the same way as singlePassEvaluate
double FastParsimony::qscoreTree( QTree* tree )
{
#ifdef notdef
	if(mFallback)
	{
		return mSlow.qscoreTree(tree);
	}
  //Dataset* dataset = Interpreter::getInstance()->dataset();
//#ifdef verboseScoring
if(FPverbosescoring) printf("qscoreTree (SinglePassevaluate on %d\n",tree->root()->nodeInfo()->nodeIndex());
//#endif
  qinitNodeDatas(tree);
  qpostOrderPass( tree, tree->root() );
#ifdef notdef // For some reason singlepass doesnt do this so we wont either
  qpreOrderPass( tree, tree->root() );
#endif

  GlobalDouble = tree->root()->nodeInfo()->nodeDataFP()->score(PSDI_FINAL);

  tree->setScore( GlobalDouble );
  printf("%f\n", GlobalDouble);
  return tree->getScore();
#endif
}

void FastParsimony::qpreOrderPass( QTree* tree, QNode *root )
{
#ifdef notdef
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
if(FPverbosescoring) printf("qpreOrderPass top for me %d top %d\n",node->nodeInfo()->nodeIndex(), top->nodeInfo()->nodeIndex());
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
if(FPverbosescoring) printf("qpreOrderPass at least 2 nodes for me %d child1 %d child2 %d\n",node->nodeInfo()->nodeIndex(), child1->nodeInfo()->nodeIndex(), child2->nodeInfo()->nodeIndex());
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
if(FPverbosescoring) printf("qpreOrderPass single node for me %d \n",node->nodeInfo()->nodeIndex());
#endif
        qassign( tree, node, PSDI_FINAL, node, PSDI_POST, nchars );
        node->nodeInfo()->nodeDataFP()->score(PSDI_PRE) = 0 ;
        node->nodeInfo()->nodeDataFP()->score(PSDI_FINAL) = 0 ;
    }
  }
#endif
}

bool FastParsimony::PreChanged(QNode *node, QNode *sibling)
{
#ifdef notdef
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
#endif
}


void FastParsimony::qpreOrderPassRecurse( QTree *tree, QNode* node, QNode* sibling )
{
#ifdef notdef
#ifdef verboseScoring
if(FPverbosescoring) printf("qpreOrderPassRecurse node %d sibling %d\n",node->nodeInfo()->nodeIndex(),sibling->nodeInfo()->nodeIndex());
#endif
  int differences = 0 ;

  c16vector* subTree0SiteData = node->nodeInfo()->nodeDataFP()->siteData(PSDI_PRE);          /* SELF */
  c16vector* subTree1SiteData = node->external()->nodeInfo()->nodeDataFP()->siteData(PSDI_PRE);    /* PARENT */
  c16vector* subTree2SiteData = sibling->nodeInfo()->nodeDataFP()->siteData(PSDI_POST);  /* SIBLING */

  int differences1 = (int) node->external()->nodeInfo()->nodeDataFP()->score(PSDI_PRE);
  int differences2 = (int) sibling->nodeInfo()->nodeDataFP()->score(PSDI_POST);
#ifdef verboseScoring
if(FPverbosescoring)   printf("preorder calculating for %d and %d\n",node->nodeInfo()->nodeIndex(),node->external()->nodeInfo()->nodeIndex());
#endif

   int differences3;
//#define PREQUINN
#ifdef PREQUINN
	if (PreChanged(node, sibling))
	{
		differences3 = calculateParsimonyScore( tree->nchars(), subTree0SiteData, subTree1SiteData, subTree2SiteData, mWeights ) ;
		node->nodeInfo()->preDifferences() = differences3;
	}
	else
	{
		printf("No pre calc needed\n");
		differences3 = node->nodeInfo()->preDifferences();
	}
#else
	differences3 = calculateParsimonyScore(subTree0SiteData, subTree1SiteData, subTree2SiteData, mWeights ) ;
#endif

  differences = differences1 + differences2 + differences3;
  node->nodeInfo()->nodeDataFP()->score(PSDI_PRE) = differences ;
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
#endif
}

int FPprunetimer = 0;
// Score the tree if the subtrees were joined at these two nodes
// The old code used to call tbrevaluate
// Returns -1 if pruned, otherwise a positive number
double FastParsimony::qevaluateTree( QTree* tree, QNodeInfo *node1, QNodeInfo *node2 )
{
#ifdef notdef
	if(mFallback)
	{
		return mSlow.qevaluateTree(tree,node1,node2);
	}
#ifdef verboseScoring
if(FPverbosescoring)
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

  c16vector* subTree1SiteData = node1->nodeDataFP()->siteData(PSDI_FINAL);
  c16vector* subTree2SiteData = node2->nodeDataFP()->siteData(PSDI_FINAL);

  //ParsimonyPrintNode ( parsimony , nodeNumberTree0 ) ;
  //ParsimonyPrintNode ( parsimony , nodeNumberTree1 ) ;


  // Add up the scores looking both ways from the join nodes
  // Then add in the differences between the two nodes
  differences1 = (int)  node1->nodeDataFP()->score(PSDI_FINAL);
  differences2 = (int)  node2->nodeDataFP()->score(PSDI_FINAL);
  differences = differences1 + differences2;

#ifdef notdef
  if(FPprunetimer >= 1000000) {
    printf("earlyprune %d, %d, %d earlyprune %d\n",differences1, differences2, differences, tree->earlyPrune());
    FPprunetimer = 0;
  } else {
    FPprunetimer++;
  }
#endif
  if((AlgorithmFlags & EARLYPRUNE) && (differences > tree->earlyPrune()))
  {
//printf("earlyprune %d, %d, %d earlyprune %d\n",differences1, differences2, differences, tree->earlyPrune());
    return -1;
  }


  // Calculate max difference allowed for this tree
  int maxdiff = tree->earlyPrune() - differences2 - differences1;
  //printf("Maxdiff = %d = %d - %d - %d\n",maxdiff, tree->earlyPrune(), differences2, differences1);
  differences3 += calculateParsimonyScoreOnly(  subTree1SiteData, subTree2SiteData, mWeights, maxdiff);
  differences = differences1 + differences2 + differences3 ;

#ifdef verboseScoring
if(FPverbosescoring)   printf("QevaluateTree, differences1 %d, differences2 %d, differences3 %d differences %d\n",differences1, differences2, differences3, differences);
#endif

  if(differences < tree->earlyPrune())
      tree->earlyPrune() = differences;

  printf("qevaluateTree()\n");
  printf("%d\n",differences);
  double dbl_diff = differences;
  printf("%f\n",dbl_diff);
  
  return dbl_diff ;
#endif
}

int FastParsimony::qfinalScoreNode( QTree *tree, QNode* node )
{
#ifdef notdef
#ifdef verboseScoring
if(FPverbosescoring) printf("qfinalScoreNode\n");
#endif
  int differences = 0;

  c16vector* subTree0SiteData = node->nodeInfo()->nodeDataFP()->siteData(PSDI_FINAL);
  c16vector* subTree1SiteData = node->nodeInfo()->nodeDataFP()->siteData(PSDI_POST);
  c16vector* subTree2SiteData = node->nodeInfo()->nodeDataFP()->siteData(PSDI_PRE);

  int differences1 = (int) node->nodeInfo()->nodeDataFP()->score(PSDI_POST);
  int differences2 = (int) node->nodeInfo()->nodeDataFP()->score(PSDI_PRE);

  int differences3 = calculateParsimonyScore(  subTree0SiteData, subTree1SiteData, subTree2SiteData, mWeights) ;
#ifdef verboseScoring
if(FPverbosescoring)   printf("Q FinalScoreNode calculating for %d differences1(Post)=%d, differences2(Pre)=%d differences3(scan)=%d\n",node->nodeInfo()->nodeIndex(),differences1, differences2, differences3);
#endif
  
  differences = differences1 + differences2 + differences3;
  node->nodeInfo()->nodeDataFP()->score(PSDI_FINAL) = differences ;

  return differences ;
#endif
}

void FastParsimony::qpostOrderPass( QTree* tree, QNode *root)
{
#ifdef notdef
#ifdef verboseScoring
if(FPverbosescoring) printf("qpostOrderPass\n");
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
#endif
}

bool FastParsimony::PostChanged(QNode *node)
{
#ifdef notdef
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
#endif
}

int FastParsimony::qpostOrderPassRecurse( QTree * tree, QNode* node )
{
#ifdef notdef
#ifdef verboseScoring
if(FPverbosescoring) printf("qpostOrderPassRecurse\n");
#endif
  int differences = 0 ;

  if ( !(node->nodeInfo()->leaf()))
  {
    c16vector* subTree0SiteData = node->nodeInfo()->nodeDataFP()->siteData(PSDI_POST) ;
    c16vector* subTree1SiteData = node->child1()->nodeInfo()->nodeDataFP()->siteData(PSDI_POST) ;
    c16vector* subTree2SiteData = node->child2()->nodeInfo()->nodeDataFP()->siteData(PSDI_POST) ;

    int differences1 = qpostOrderPassRecurse( tree, node->child1() ) ;
    int differences2 = qpostOrderPassRecurse( tree, node->child2() ) ;
	int differences3;
	
#define QUINN
#ifdef QUINN
	if (PostChanged(node))
	{
		differences3 = calculateParsimonyScore( subTree0SiteData, subTree1SiteData, subTree2SiteData, mWeights ) ;
		node->nodeInfo()->postDifferences() = differences3;
	}
	else
	{
		//printf("No calc needed\n");
		differences3 = node->nodeInfo()->postDifferences();
	}
#else
	differences3 = calculateParsimonyScore( subTree0SiteData, subTree1SiteData, subTree2SiteData, mWeights ) ;
#endif
	
	differences = differences1 + differences2 + differences3;
	
    // printf("me %d nchars %d differences1 %d differences2 %d differences3 %d differences %d child1 %d child2 %d\n",node->nodeInfo()->nodeIndex(), tree->nchars(),differences1,differences2,differences3,differences,node->child1()->nodeInfo()->nodeIndex(),node->child2()->nodeInfo()->nodeIndex());

    node->nodeInfo()->nodeDataFP()->score(PSDI_POST) = differences ;
#ifdef verboseScoring
 if(FPverbosescoring)  printf("postorder calculating for %s[%d] using %s[%d] and %s[%d] returning %d\n",
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
#endif
}


int FastParsimony::calculateParsimonyScoreOnly(c16vector* src1 , c16vector* src2 , c16vector* weights, int maxdiff )
{
#ifdef notdef
	int i;
	int score = 0;
	c16vector and_res;
	c16vector mask;
	c16vector score_vect;

#ifdef VERBOSE_FAST_PARSIMONY_SCOREING	
	int j;
	printf("calculateParsimonyScoreOnly -- maxdiff = %d\n",maxdiff);
	printf("src1:\t\t");
	for(i=0;i<mSize;i++)
	{
		for(j=0;j<16;j++)
			printf("%8x ",src1[i].c[j]);
	}
	printf("\n");
	printf("src2:\t\t");
	for(i=0;i<mSize;i++)
	{
		for(j=0;j<16;j++)
			printf("%8x ",src2[i].c[j]);
	}
	printf("\n");
	printf("weights:\t");
	for(i=0;i<mSize;i++)
	{
		for(j=0;j<16;j++)
			printf("%8x ",weights[i].c[j]);
	}
	printf("\n");
	printf("score:\t\t");
#endif	
	
	for(i=0;i<mSize;i++)
	{
		//compute the intersection of these characters
		and_res.v = src1[i].v & src2[i].v;
		
		//find which characters had an empty intersection
		mask.half[0] = __builtin_ia32_pcmpeqb(and_res.half[0],zero.half[0]);
		mask.half[1] = __builtin_ia32_pcmpeqb(and_res.half[1],zero.half[1]);
		
		//put value of weights in each byte field that required a union rather than intersection
		mask.v &= weights[i].v;
		
#ifdef VERBOSE_FAST_PARSIMONY_SCOREING	
	
		for(j=0;j<16;j++)
			printf("%8x ",mask.c[j]);
	
#endif
		
		//sum weights and add to score
		score_vect.v = mask.v;
		
		mask.sf = __builtin_ia32_shufps(mask.sf,mask.sf,0xB1); //exchange abcd =>badc
		mask.v += score_vect.v;
		score_vect.v = mask.v;
		
		mask.sf = __builtin_ia32_shufps(mask.sf,mask.sf,0x4E); //exchange abcd =>badc
		mask.v += score_vect.v;
		score_vect.v = mask.v;
		
		score += score_vect.c[0];
		score += score_vect.c[1];
		score += score_vect.c[2];
		score += score_vect.c[3];
		
		//exit early if we have already exceeded the best tree so far
		if(score > maxdiff)
		{
			printf("\n\n");
			return score;
		}
	}
	printf("\n\n");
	return score;
#endif
}

int FastParsimony::calculateParsimonyScore(c16vector* dest , c16vector* src1 , c16vector* src2 , c16vector* weights )
{
#ifdef notdef
	int score = 0;
	int i;
	c16vector and_res;
	c16vector or_res;

	c16vector mask;
	c16vector move_mask;
	c16vector score_vect;
#ifdef VERBOSE_FAST_PARSIMONY_SCOREING	
	int j;
	printf("calculateParsimonyScore\n");
	printf("src1:\t\t");
	for(i=0;i<mSize;i++)
	{
		for(j=0;j<16;j++)
			printf("%8x ",src1[i].c[j]);
	}
	printf("\n");
	printf("src2:\t\t");
	for(i=0;i<mSize;i++)
	{
		for(j=0;j<16;j++)
			printf("%8x ",src2[i].c[j]);
	}
	printf("\n");
	printf("weights:\t");
	for(i=0;i<mSize;i++)
	{
		for(j=0;j<16;j++)
			printf("%8x ",weights[i].c[j]);
	}
	printf("\n");
	printf("dest:\t\t");
#endif	
	for(i=0;i<mSize;i++)
	{
		//compute the intersection of these characters
		and_res.v = src1[i].v & src2[i].v;
		or_res.v = src1[i].v | src2[i].v;
		
		//find which characters had an empty intersection
		mask.half[0] = __builtin_ia32_pcmpeqb(and_res.half[0],zero.half[0]);
		mask.half[1] = __builtin_ia32_pcmpeqb(and_res.half[1],zero.half[1]);
		
		move_mask.v = mask.v;
		
		//fill in dest vectors
		dest[i].v = move_mask.v & or_res.v;
		
		move_mask.half[0] = __builtin_ia32_pxor(move_mask.half[0],ones.half[0]);
		move_mask.half[1] = __builtin_ia32_pxor(move_mask.half[1],ones.half[1]);
		
		
		dest[i].v |= (move_mask.v & and_res.v);
		
		//put value of weights in each byte field that required a union rather than intersection
		mask.v &= weights[i].v;
	
#ifdef VERBOSE_FAST_PARSIMONY_SCOREING	
	
		for(j=0;j<16;j++)
			if(mask.c[j] == 0)
				printf("%8x ",dest[i].c[j]);
			else
				printf("%8x*",dest[i].c[j]);
	
#endif
		
		//sum weights and add to score
		score_vect.v = mask.v;
		
		mask.sf = __builtin_ia32_shufps(mask.sf,mask.sf,0xB1); //exchange abcd =>badc
		mask.v += score_vect.v;
		score_vect.v = mask.v;
		
		mask.sf = __builtin_ia32_shufps(mask.sf,mask.sf,0x4E); //exchange abcd =>badc
		mask.v += score_vect.v;
		score_vect.v = mask.v;
		
		score += score_vect.c[0];
		score += score_vect.c[1];
		score += score_vect.c[2];
		score += score_vect.c[3];
	}
#ifdef VERBOSE_FAST_PARSIMONY_SCOREING	
	printf("\n\n");
#endif
	return score;
#endif
}
