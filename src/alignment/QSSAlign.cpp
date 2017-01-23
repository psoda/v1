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
#include "QSSAlign.h"
#include "Timer.h"
#include "Interpreter.h"
#include "Dist.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

/*
#include "QNode.h"
#include "QNodeInfo.h"
#include "QTree.h"
#include "Globals.h"
#include <stdio.h>
#include <list>
#include <algorithm>
#include <float.h>
#include <errno.h>
*/

/**
 * Constructor.
 * \param gapOpenIn gap open penalty.
 * \param gapOpenHelixIn gap open penalty when the most common secondary structure is a helix.
 * \param gapOpenBetaStrandIn gap open penalty when the most common secondary structure is a beta-strand.
 * \param gapOpenLoopIn gap open penalty when the most common secondary structure is a loop.
 * \param gapExtIn gap extension penalty.
 * \param subMatAIn substitution matrix for Alpha-helices.
 * \param subMatBIn substitution matrix for Beta-sheets.
 * \param subMatLIn substitution matrix for loops.
 * \param subMatIn substitution matrix for the default case (e.g., none of the above).
 * \param subMatSSIn substitution matrix for secondary structures.
 * \param penalizeLeadingGapsIn if set, penalize leading gaps (the gaps on the ends of the sequences)
 * \param useClassicPMSA if set, use the stock (unoptimized) progressivive sequence alignment methods of iterating through every character in one sub-alignment with every other character in the other alignment.  If no set, use the optimizied MultChars.
 * \param gapDistanceSetting penalize opening a new gap within this distance to another gap.
 * \param localWeight amount of contribution of local alignment wieghts.  Range form 0.0 to 1.0.  Set to 0.0 to ignore local alignment weights.
 */
QSSAlign::QSSAlign(float gapOpenIn, float gapOpenHelixIn, float gapOpenBetaStrandIn, float gapOpenLoopIn, float gapExtIn, SubMat* subMatAIn, SubMat* subMatBIn, SubMat* subMatLIn, SubMat* subMatIn, SubMat* subMatSSIn, bool penalizeLeadingGapsIn, bool useClassicPMSA, int gapDistanceSetting, float localWeight)
  : QAlign( gapOpenIn, gapExtIn, subMatIn, penalizeLeadingGapsIn, useClassicPMSA, gapDistanceSetting, localWeight),
    mGapCharSS('-'),
    mEndCharSS('\0'),
    subMatA(subMatAIn),
    subMatB(subMatBIn),
    subMatL(subMatLIn),
    subMat(subMatIn),
    subMatSS(subMatSSIn),
    mDataset( NULL),
    mSSDataset( NULL),
    mPrintBuffer( NULL),
    gapOpenHelix( gapOpenHelixIn),
    gapOpenBetaStrand( gapOpenBetaStrandIn),
    gapOpenLoop( gapOpenLoopIn),
    gapLookupHelix( NULL),
    gapLookupBetaStrand( NULL),
    gapLookupLoop( NULL)
{

  gapLookupHelix = (float*) check_calloc( mGapDistanceSetting, sizeof( float));
  gapLookupHelix[0] = gapExt;

  gapLookupBetaStrand = (float*) check_calloc( mGapDistanceSetting, sizeof( float));
  gapLookupBetaStrand[0] = gapExt;

  gapLookupLoop = (float*) check_calloc( mGapDistanceSetting, sizeof( float));
  gapLookupLoop[0] = gapExt;

  /* future: clustalw only sets the gapdist penalty for amino acids */
  for( int dist = 1; dist < mGapDistanceSetting; dist++){
    gapLookupHelix[dist]      = (2 + ((mGapDistanceSetting - dist)*2)/mGapDistanceSetting)*gapOpenHelix;
    gapLookupBetaStrand[dist] = (2 + ((mGapDistanceSetting - dist)*2)/mGapDistanceSetting)*gapOpenBetaStrand;
    gapLookupLoop[dist]       = (2 + ((mGapDistanceSetting - dist)*2)/mGapDistanceSetting)*gapOpenLoop;
  }
}

/** Destructor
 */
QSSAlign::~QSSAlign()
{}


/**
 * The actual alignment method.
 * Use the tree repository if \a useExistingTree is set.
 * Write aligned datasets to \a alignedDataset and \a alignedSSDataset.
 * \param qtreeRepository tree repository object.
 * \param useExistingTree if set, use a tree from the tree repository.
 * \param alignedDataset pointer to resulting (aligned) dataset.
 */
void QSSAlign::ssalign(QTreeRepository &qtreeRepository, bool useExistingTree, Dataset** alignedDataset, SSDataset** alignedSSDataset)
{
  QTreeRepository *tempRepository;
  mDataset = Interpreter::getInstance()->dataset();
  mSSDataset = Interpreter::getInstance()->ssDataset();
  mPrintBuffer = PsodaPrinter::getInstance();

  if( mSSDataset == NULL){
    throw PsodaError("\nERROR: No SSData detected for ssalign()!\n\n");
  }
  
  if( mSSDataset->matches( mDataset) == false){
    throw PsodaError( "\nERROR: Secondary structure data set does not match sequences data set (detected in QSSAlign())!\n\n");
  }

  if( mDataset->ntaxa() <= 1){
    char str[1024];
    sprintf( str, "ERROR: Can not perform alignment with %d taxa!", mDataset->ntaxa());
    throw PsodaError(str);
  }

  // We need to make sure we don't have any gaps in the data
  mDataset->stripGaps();

    fprintf(stderr,"Before convertto3State()\n");
  /* mandate that secondary structures are in 3-state form */
  mSSDataset->convertTo3State();
    fprintf(stderr,"After convertto3State()\n");
  
  mGapCharSS = mSSDataset->gapchar();
  mEndCharSS = mSSDataset->endchar();
  
  /*
  vector<string>::iterator nameItr = mDataset->taxonNames().begin();
  vector<string>::iterator nameEnd = mDataset->taxonNames().end();
  int i = 0;
  for (; nameItr != nameEnd; nameItr++) {
    printf("name: %s\n", nameItr->c_str());
    printf("chars: %s\n", mDataset->getCharacters(i++));
  }
  */
  if( PSODA_VERBOSE || PSODA_DEBUG){
    mPrintBuffer->write( "ssalign()\n"); 
    mPrintBuffer->write( "gap open A: %f\ngap open B: %f\ngap open L: %f\ngap open (default): %f\ngap ext: %f\nsub mat: %s\n", gapOpenHelix, gapOpenBetaStrand, gapOpenLoop, gapOpen, gapExt, subMat->getName().c_str());
    if( fabs(mLocalWeight) < .001){
      mPrintBuffer->write( "local weight: %f\n", mLocalWeight);
    }
  }
  
  Timer timer;


  QTree *qtree;
  if( useExistingTree == false)
  {
      // if there is no tree in the repository, calculate a distance matrix and then a neighbor join tree
      timer.start();
      calculateDistMatrix(mDataset, mSSDataset);
      TimerSecondMicros sm1 = timer.getCurrentSecondMicros();
      mPrintBuffer->write("Matrix calculation took %li.%li seconds\n" , sm1.seconds , sm1.micros );

      // Use distance matrix to calculate neighbor-join tree, and puts it in the tree repo
      tempRepository = new QTreeRepository;
      DistSearch *dist = new DistSearch(ntaxa, distMatrix); 
      timer.start();
      dist->search(*tempRepository, NULL, NULL, 1); /* 1 = number interations */

      TimerSecondMicros sm2 = timer.getCurrentSecondMicros();
      mPrintBuffer->write("Neighbor-join tree generation took %li.%li seconds\n" , sm2.seconds , sm2.micros );
      qtree = tempRepository->getTree(); 
    }
    else
    {
        if( PSODA_DEBUG){ mPrintBuffer->write("HY: Get the tree from the repository\n"); }
        // Get the tree from the repository and Do a postorder traversal of the tree to create final multiple sequence alignment
        qtree = qtreeRepository.getTree(); /* future: not guaranteed to give you the neighbor-joining tree (e.g., there's a better scoring tree already in the repository) */
    }
    if( qtree == NULL){
      mPrintBuffer->write("ERROR getting tree from tree repository\n");
      exit(-1);
    }
    if( PSODA_DEBUG){ printf("Using %s for alignment\n", qtree->treeStr()); }

    // We need to create another QNode with appropriate QNodeInfo and pointers to create an artificial root
    timer.start();
    fprintf(stderr, "%s", "Progressive: ");
    QNodeInfo *rootinfo = new QNodeInfo(-1, ISNTLEAF);
    QNode *root = new QNode(*rootinfo, ISNTLEAF);
    root->child1() = qtree->root();
    root->child2() = qtree->root()->external();
    
    if( *alignedDataset != NULL){
      delete *alignedDataset;
    }
    *alignedDataset = new Dataset();
    (*alignedDataset)->datatype() = mDataset->datatype();
    (*alignedDataset)->dataformat()= Dataset::ALIGNED_DATAFORMAT;
    (*alignedDataset)->gapchar()= mDataset->gapchar();
    //(*alignedDataset)->gapvalue() = mDataset->gapvalue();
    (*alignedDataset)->missingchar() = mDataset->missingchar();
    (*alignedDataset)->matchchar() = mDataset->matchchar();
    (*alignedDataset)->setntaxa( mDataset->ntaxa());


    if( (*alignedSSDataset) != NULL){
      delete (*alignedSSDataset);
    }
    (*alignedSSDataset) = new SSDataset();
    (*alignedSSDataset)->datatype()= mSSDataset->datatype();
    (*alignedSSDataset)->dataformat()= Dataset::ALIGNED_DATAFORMAT;
    (*alignedSSDataset)->gapchar()= mSSDataset->gapchar();
    //(*alignedSSDataset)->gapvalue() = mSSDataset->gapvalue();
    (*alignedSSDataset)->missingchar() = mSSDataset->missingchar();
    (*alignedSSDataset)->matchchar() = mSSDataset->matchchar();
    (*alignedSSDataset)->setntaxa( mSSDataset->ntaxa());

    if( mUseClassicPMSA){
      classicSSPMSA(root);
      Dataset* unOrderedAlignedDataset = root->nodeInfo()->mDataset;
      Dataset* unOrderedAlignedSSDataset = root->nodeInfo()->mSSDataset;

      // re-order taxa
      (*alignedDataset)->setnchars( unOrderedAlignedDataset->nchars());
      (*alignedSSDataset)->setnchars( unOrderedAlignedDataset->nchars());
      for( int i = 0; i < mDataset->ntaxa(); i++){
	(*alignedDataset)->addName( mDataset->getTaxonName(i));
	(*alignedSSDataset)->addName( mDataset->getTaxonName(i));
	int unOrderedTaxonNum = unOrderedAlignedDataset->getTaxonNumber( mDataset->getTaxonName(i));
      
	(*alignedDataset)->addCharacters( unOrderedAlignedDataset->getCharacters( unOrderedTaxonNum,false));
	(*alignedSSDataset)->addCharacters( unOrderedAlignedSSDataset->getCharacters( unOrderedTaxonNum,true));
      }

      delete unOrderedAlignedDataset;
      delete unOrderedAlignedSSDataset;
    }else{
      ssPMSA(root);
      
      // Now we have to push the new gaps down through to the leaves.
      (*alignedDataset)->setnchars( mDataset->nchars());
      (*alignedSSDataset)->setnchars( mDataset->nchars());
      for( int i = 0; i < mDataset->ntaxa(); i++){
	(*alignedDataset)->addName( mDataset->getTaxonName(i));
	(*alignedSSDataset)->addName( mDataset->getTaxonName(i));
      }

      pushGapsDownSS(root, *alignedDataset, *alignedSSDataset);

      if( PSODA_DEBUG > 1){
	fprintf( stderr, "alignedDataset:\n");
	(*alignedDataset)->printSeqs("PHYLIP", "stderr");
	fprintf( stderr, "alignedSSDataset:\n");
	(*alignedSSDataset)->printSeqs("PHYLIP", "stderr");
      }
      //(*alignedDataset)->prologue();
    }

    cleanUpAlignment(*alignedDataset);
    cleanUpAlignment(*alignedSSDataset);

    if( useExistingTree == false)
    {
        delete tempRepository;
    }
    
    fprintf(stderr, "%s", "\n");
    TimerSecondMicros sm3 = timer.getCurrentSecondMicros();
    mPrintBuffer->write("Progressive alignment took %li.%li seconds\n" , sm3.seconds , sm3.micros );
    
    // delete the artificial root and its QNodeInfo
    //delete rootinfo; /* does this get deleted when root is deleted */
    delete root;
}


/**
 * Post-order traversal of the tree (starting at \a root), assigning the characters and gaps to \a alignedDataset and \a alignedSSDataset.
 * \param root is the starting point for further recursion
 * \param alignedDataset dataset to write aligned sequences to.
 * \param alignedSSDataset dataset to write aligned secondary structure sequences to.
 * \sa pushGaps(), pushGapsDown()
 */
void QSSAlign::pushGapsDownSS(QNode *root, Dataset* alignedDataset, SSDataset* alignedSSDataset)
{
  if (root->nodeInfo()->leaf())
    {

      if( PSODA_DEBUG){
	assert( mDataset->getTaxonNumber(root->nodeInfo()->getTaxonName()) == root->nodeInfo()->nodeIndex());
      }

      //fprintf( stderr, "HY: seq size = %d (\"%s\")\n", (int)root->nodeInfo()->mMultChar->size(), root->nodeInfo()->getTaxonName());
      
      char* seq   = new char[root->nodeInfo()->mMultChar->size() + 1];
      char* ssSeq = new char[root->nodeInfo()->mMultChar->size() + 1];

      int i = 0;
      list<MultChar *>::iterator a_it = root->nodeInfo()->mMultChar->begin();
      while (a_it != root->nodeInfo()->mMultChar->end())
        {
	  if( PSODA_DEBUG > 1){
	    assert( (*a_it)->getCount() == 1);
	    (*a_it)->print();
	  }
	  seq[i]   = (char)((*a_it)->getFirst());
	  ssSeq[i] = (char)((*a_it)->getFirstSS());
	  if( ssSeq[i] == mGapChar){
	    ssSeq[i] = mGapCharSS;
	  }
	  i++;
 	  a_it++;
        }
      seq[i] = mEndChar;
      ssSeq[i] = mEndCharSS;
      //fprintf( stderr, ", %d:\t%s\n", root->nodeInfo()->nodeIndex(), seq);
      alignedDataset->addCharacters( (const char*)seq, root->nodeInfo()->nodeIndex());
      alignedSSDataset->addCharacters( (const char*)ssSeq, root->nodeInfo()->nodeIndex());

      delete[] seq;
      delete[] ssSeq;
    }
  else
    {
      /* recurse */
      pushGaps(root);
      pushGapsDownSS(root->child1(), alignedDataset, alignedSSDataset);
      pushGapsDownSS(root->child2(), alignedDataset, alignedSSDataset);
    }
}

void QSSAlign::ssPMSA(QNode *root)
{
    if (root->nodeInfo()->leaf())
    {
        setupMultCharSS(root);
        //printf("Leaf %d my size is %d\n", root->nodeInfo()->nodeIndex(), root->nodeInfo()->mMultChar->size());
    }
    else
    {
        ssPMSA(root->child1());
        ssPMSA(root->child2());
        progressiveSSAlign(root);
    }
}

/**
 * Makes MultChar data structures for each character in the sequence for root.
 * \param root is the starting point for further recursion
 */
void QSSAlign::setupMultCharSS(QNode *root)
{
  assert( root);
  assert( root->nodeInfo());
  if( root->nodeInfo()->mMultChar){
    delete root->nodeInfo()->mMultChar;
  }
    
    const char *data = mDataset->getCharacters( root->nodeInfo()->nodeIndex(),false);
    const char *ssData = mSSDataset->getCharacters( root->nodeInfo()->nodeIndex(),true);
    int dataLen = mDataset->getSeqLen( root->nodeInfo()->nodeIndex());
    root->nodeInfo()->mMultChar = new list<MultChar*>();

    for (int i = 0; i < dataLen; i++)
    {
      root->nodeInfo()->mMultChar->push_back(new MultChar(data[i], false, 1, ssData[i] ));
    }
//     fprintf(stderr, "Node %d has %d characters\n", root->nodeInfo()->nodeIndex(), strlen(data));

//     list<MultChar *>::iterator a_it = root->nodeInfo()->mMultChar->begin();
//     while (a_it != root->nodeInfo()->mMultChar->end())
//       {
// 	(*a_it)->print();
// 	a_it++;
//       }
    
}


/**
 * Wrapper for multCharAlignSS().
 * \param root is the starting point for further recursion
 * \sa multCharAlignSS(), progressiveAlign()
 */
void QSSAlign::progressiveSSAlign(QNode *root)
{
    fprintf(stderr, "%s", ".");
    multCharAlignSS(root);
} /* END progressiveSSAlign() */


/**
 * Align each of the children of root, comparing each of the MultChars.
 * The only difference between multCharAlign() and multCharAlignSS() is the call to evalCost() instead of evalSSCost()
 * \param root is the starting point for further recursion
 */
void QSSAlign::multCharAlignSS(QNode *root)
{
  int i,j;
  list<MultChar *> *a = root->child1()->nodeInfo()->mMultChar;
  list<MultChar *> *b = root->child2()->nodeInfo()->mMultChar;
  list<MultChar *>::iterator a_it, b_it;
  int sizea = a->size();
  int sizeb = b->size();
  double matcost;

#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
  float aPercentNotGaps;
  float bPercentNotGaps;
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
    
  initDPMatrices( sizea, sizeb);

  int maxA = (penalizeLeadingGaps == true) ? sizea : sizea - 1;
  int maxB = (penalizeLeadingGaps == true) ? sizeb : sizeb - 1;

  if( PSODA_DEBUG){
    fprintf(stderr, "\nSequences To Align:\n");
    a_it = a->begin();
    while (a_it != a->end()){
      (*a_it)->print();
      a_it++;
    }
    fprintf(stderr,"\n");
    b_it = b->begin();
    while (b_it != b->end()){
      (*b_it)->print();
      b_it++;
    }
    fprintf(stderr,"\n");
  }

  /* include local alignment weights */
  if( mLocalWeight > 0.0){
    multCharLocalAlign(a, b);
  }
    
    /* iterate over each of MultChars in one of the child sequences */
  for(i = 1, a_it = a->begin(); i <= maxA; a_it++, i++)
    {
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
      aPercentNotGaps = (*a_it)->getPercentNotGaps();
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
      /* iterate over each of MultChars in the other child sequences */
      for(j = 1, b_it = b->begin(); j <= maxB; b_it++, j++)
	{
	  //fprintf(stderr, "Working on (%d,%d)", i, j);
	  matcost = (*a_it)->evalSSCost((*b_it), subMat, subMatA, subMatB, subMatL, subMatSS);
	  setTracebackSS( i, j, matcost, (*a_it)->mostCommonSS(), (*b_it)->mostCommonSS()
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			, aPercentNotGaps, (*b_it)->getPercentNotGaps()
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			);
	}
    }

  if( penalizeLeadingGaps == false){
    /* allow for free gaps along the last row and column */

    j = sizeb;
    b_it = b->end();
    b_it--;
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
    bPercentNotGaps = (*b_it)->getPercentNotGaps();
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
    for(i = 1, a_it = a->begin(); i <= maxA; a_it++, i++){
      //fprintf(stderr, "Working on (%d,%d)", i, j);
      matcost = (*a_it)->evalSSCost((*b_it), subMat, subMatA, subMatB, subMatL, subMatSS);
      setTracebackSSLastCol( i, j, matcost, (*b_it)->mostCommonSS()
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			   , bPercentNotGaps
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			   );
    }
	  
    i = sizea;
    a_it = a->end();
    a_it--;
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
    aPercentNotGaps = (*a_it)->getPercentNotGaps();
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
    for(j = 1, b_it = b->begin(); j <= maxB; b_it++, j++){
      //fprintf(stderr, "Working on (%d,%d)", i, j);
      matcost = (*a_it)->evalSSCost((*b_it), subMat, subMatA, subMatB, subMatL, subMatSS);
      setTracebackSSLastRow( i, j, matcost, (*a_it)->mostCommonSS()
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			   , aPercentNotGaps
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			   );
    }
    i = sizea;
    a_it = a->end();
    a_it--;
    j = sizeb;
    b_it = b->end();
    b_it--;
    //fprintf(stderr, "Working on (%d,%d)", i, j);
    matcost = (*a_it)->evalSSCost((*b_it), subMat, subMatA, subMatB, subMatL, subMatSS);
    setTracebackBottomRight( i, j, matcost);
  }
	
  if( PSODA_DEBUG){
    //PsodaPrinter::getInstance()->write("multCharAlign score %f\n", DPMATRIX(sizea, sizeb));
    if( PSODA_DEBUG > 9){
      printDPMatrices(a, b);
    }
  }

  /*
   * traceback
   */
  progressiveTraceback(root);
} /* END multCharAlignSS() */



/**
 * Align the dataset by recursively traversing starting from root.
 * Uses an unoptimized (stock) progressive multiple sequence alignment.
 * Compares every character and its secondary structure element of one sequence with every character and its secondary structure element of another.
 * \param root is the starting point for further recursion.
 * \sa classicPMSA(), classicProgressiveSSAlign()
 */
void QSSAlign::classicSSPMSA(QNode *root){
    if (root->nodeInfo()->leaf())
    {
      setupLeafSSDataset(root);
      //printf("Leaf %d my size is %d\n", root->nodeInfo()->nodeIndex(), root->nodeInfo()->mMultChar->size());
    }
    else
    {
      classicSSPMSA(root->child1());
      classicSSPMSA(root->child2());
      classicProgressiveSSAlign(root);
    }

}

/**
 * Make two datasets---one for the primary sequence and another for the secondary sequence---each with just a single sequence, for the leaf node root.
 * \param root is pointer to the leaf node containing a sequence.
 */
void QSSAlign::setupLeafSSDataset(QNode *root)
{
  // copy single sequence to a Dataset

  assert( root);
  assert( root->nodeInfo());
  if( root->nodeInfo()->mDataset){
    fprintf(stderr, "\nHY: deleting mDataset in a nodeInfo - why?\n\n");
    delete root->nodeInfo()->mDataset;
    delete root->nodeInfo()->mSSDataset;
  }
    
  root->nodeInfo()->mDataset = new Dataset();
  root->nodeInfo()->mDataset->datatype() = mDataset->datatype();
  root->nodeInfo()->mDataset->dataformat()= Dataset::ALIGNED_DATAFORMAT;
  root->nodeInfo()->mDataset->gapchar()= mDataset->gapchar();
  root->nodeInfo()->mDataset->missingchar() = mDataset->missingchar();
  root->nodeInfo()->mDataset->matchchar() = mDataset->matchchar();
  root->nodeInfo()->mDataset->addName( mDataset->getTaxonName( root->nodeInfo()->nodeIndex()));
  root->nodeInfo()->mDataset->addCharacters( mDataset->getCharacters( root->nodeInfo()->nodeIndex(),false));

  // copy secondary structure sequence
  root->nodeInfo()->mSSDataset = new SSDataset();
  root->nodeInfo()->mSSDataset->datatype()=  mSSDataset->datatype();
  root->nodeInfo()->mSSDataset->dataformat()= Dataset::ALIGNED_DATAFORMAT;
  root->nodeInfo()->mSSDataset->gapchar()= mSSDataset->gapchar();
  root->nodeInfo()->mSSDataset->missingchar() = mSSDataset->missingchar();
  root->nodeInfo()->mSSDataset->matchchar() = mSSDataset->matchchar();
  root->nodeInfo()->mSSDataset->addName( mSSDataset->getTaxonName( root->nodeInfo()->nodeIndex()));
  root->nodeInfo()->mSSDataset->addCharacters( mSSDataset->getCharacters( root->nodeInfo()->nodeIndex(),false));

  
  if( PSODA_DEBUG){
    fprintf(stderr, "setupLeafDataset(%d): (ntaxa: %d, nchars: %d)\n",
	    root->nodeInfo()->nodeIndex(),
	    root->nodeInfo()->mDataset->ntaxa(),
	    root->nodeInfo()->mDataset->nchars() );
    root->nodeInfo()->mDataset->printSeqs( "phylip", "stderr");
  }
  
} /* END setupLeafSSDataset() */



/**
 * Align each of the children of root, comparing each character in the two sequences.
 * \param root pointer to the leaf node containing a sequence.
 */
void QSSAlign::classicProgressiveSSAlign(QNode *root){
  //fprintf(stderr,"Progressively aligning %d and %d\n", root->child1()->nodeInfo()->nodeIndex(), root->child2()->nodeInfo()->nodeIndex());
  fprintf(stderr, "%s", ".");

  int i,j;
  Dataset* a = root->child1()->nodeInfo()->mDataset;
  Dataset* b = root->child2()->nodeInfo()->mDataset;
  SSDataset* ssA = root->child1()->nodeInfo()->mSSDataset;
  SSDataset* ssB = root->child2()->nodeInfo()->mSSDataset;
  int sizea = a->nchars();
  int sizeb = b->nchars();
  double matcost;

  initDPMatrices( sizea, sizeb);

  int maxA = (penalizeLeadingGaps == true) ? sizea : sizea - 1;
  int maxB = (penalizeLeadingGaps == true) ? sizeb : sizeb - 1;


  if( PSODA_DEBUG){
    fprintf(stderr, "\nSequences To Align: (a: ntaxa: %d, nchars: %d; b: ntaxa: %d, nchars: %d)\n",
	    a->ntaxa(),
	    a->nchars(),
	    b->ntaxa(),
	    b->nchars() );
    fprintf(stderr,"a:\n");
    a->printSeqs( "phylip", "stderr");
    fprintf(stderr,"b:\n");
    b->printSeqs( "phylip", "stderr");
  }

  /* get local alignment of a & b */
  if( mLocalWeight > 0.0){
    classicLocalAlign(a, b);
  }
    
  /* iterate over every character */
  for(i = 1; i <= maxA; i++){
    /* iterate over every character */
    for(j = 1; j <= maxB; j++){
      //fprintf(stderr, "Working on (%d,%d)", i, j);
      matcost = progressiveSSMatCost( i-1, a, ssA, j-1, b, ssB);
      setTracebackSS( i, j, matcost, ssA, ssB);
    }
  }

  if( penalizeLeadingGaps == false){
    /* allow for free gaps along the last row and column */

    j = sizeb;
    for(i = 1; i <= maxA; i++){
      //fprintf(stderr, "Working on (%d,%d)", i, j);
      matcost = progressiveSSMatCost( i-1, a, ssA, j-1, b, ssB);
      setTracebackSSLastCol( i, j, matcost, ssB);
    }
	  
    i = sizea;
    for(j = 1; j <= maxB; j++){
      //fprintf(stderr, "Working on (%d,%d)", i, j);
      matcost = progressiveSSMatCost( i-1, a, ssA, j-1, b, ssB);
      setTracebackSSLastRow( i, j, matcost, ssA);
    }
    i = sizea;
    j = sizeb;
    //fprintf(stderr, "Working on (%d,%d)", i, j);
    matcost = progressiveSSMatCost( i-1, a, ssA, j-1, b, ssB);
    setTracebackBottomRight( i, j, matcost);
  }
	
  if( PSODA_DEBUG){
    //PsodaPrinter::getInstance()->write("classicProgressiveAlign score %f\n", DPMATRIX(sizea, sizeb));
    if( PSODA_DEBUG > 9){
      printDPMatrices(a, b);
    }
  }
    
  // Now do the traceback. Begin at traceback[sizea][sizeb] and go back to traceback[1][1]
  // Save the path in an array, then walk down the array, building the profile alignment (Dataset)
  // Put the results in parent's Dataset
  Traceback *temp = getBottomRightTraceBack(sizea, sizeb);

  Direction tracebackPath[sizea + sizeb];

  int k; /* index of tracebackPath[] */
  // initialize tracebackPath[]
  /* not neccessary
     for( k = 0; k < sizea + sizeb; k++){
     tracebackPath[k] = None;
     }
  */
  k = sizea + sizeb - 1;
    
  while(temp->next != NULL){
    tracebackPath[k--] = temp->direction;
    temp = temp->next;
  }
  k++;
  int kOffset = k;

  Dataset* ab = new Dataset();
  ab->datatype() = a->datatype();
  ab->dataformat() = Dataset::ALIGNED_DATAFORMAT;
  ab->gapchar() = a->gapchar();
  ab->missingchar() = a->missingchar();
  ab->matchchar() = a->matchchar();
  //ab->gapvalue() = a->gapvalue();
  ab->setntaxa( a->ntaxa() + b->ntaxa());

  SSDataset* ssAB = new SSDataset();
  ssAB->datatype()= ssA->datatype();
  ssAB->dataformat()= Dataset::ALIGNED_DATAFORMAT;
  ssAB->gapchar()= ssA->gapchar();
  ssAB->missingchar() = ssA->missingchar();
  ssAB->matchchar() = ssA->matchchar();
  //ssAB->gapvalue() = ssA->gapvalue();
  ssAB->setntaxa( ssA->ntaxa() + ssB->ntaxa());

  int aIndex = 0;
  int bIndex = 0;
  int l = 0; /* taxon index of profile alignment */
  for(aIndex = 0; aIndex < a->ntaxa(); aIndex++){
    ab->addName( a->getTaxonName(aIndex));
  }
  for(bIndex = 0; bIndex < b->ntaxa(); bIndex++){
    ab->addName( b->getTaxonName(bIndex));
  }
    
  i = 0;
  j = 0;
  // walk down tracebackPath[] and build the profile alignment Dataset
  for(; k < sizea + sizeb; k++){
    if( tracebackPath[k] == Diag){
      if( PSODA_DEBUG > 1){ fprintf( stderr, "Diag "); }
      l = 0;
      for(aIndex = 0; aIndex < a->ntaxa(); aIndex++){
	ab->appendACharacter( a->getCharacter(aIndex, i), k - kOffset, l++);
	ssAB->appendACharacter( ssA->getCharacter(aIndex, i), k - kOffset, l++);
      }
      for(bIndex = 0; bIndex < b->ntaxa(); bIndex++){
	ab->appendACharacter( b->getCharacter(bIndex, j), k - kOffset, l++);
	ssAB->appendACharacter( ssB->getCharacter(bIndex, j), k - kOffset, l++);
      }
      i++;
      j++;
    }else if( tracebackPath[k] == Up){
      if( PSODA_DEBUG > 1){ fprintf( stderr, "Up "); }
      l = 0;
      for(aIndex = 0; aIndex < a->ntaxa(); aIndex++){
	ab->appendACharacter( a->getCharacter(aIndex, i), k - kOffset, l++);
      }
      for(bIndex = 0; bIndex < b->ntaxa(); bIndex++){
	ab->appendACharacter( b->gapchar(), k - kOffset, l++);
      }
      i++;
    }else{
      assert( tracebackPath[k] == Left);

      if( PSODA_DEBUG > 1){ fprintf( stderr, "Left "); }
      l = 0;
      for(aIndex = 0; aIndex < a->ntaxa(); aIndex++){
	ab->appendACharacter( a->gapchar(), k - kOffset, l++);
      }
      for(bIndex = 0; bIndex < b->ntaxa(); bIndex++){
	ab->appendACharacter( b->getCharacter(bIndex, j), k - kOffset, l++);
      }
      j++;
    }
  }
  
  root->nodeInfo()->mDataset = ab;
    
  if( PSODA_DEBUG){
    //fprintf( stderr, "\n");
    //printDPMatrices(sizea, sizeb);
    fprintf(stderr, "Aligned Sequences: (ntaxa: %d, nchars: %d)\n",
	    ab->ntaxa(),
	    ab->nchars() );
    ab->printSeqs( "phylip", "stderr");
    fprintf(stderr,"\n");
  }

  /*
  fprintf(stderr, "After alignment for node %d my size is %d child1(%d) size is %d child2(%d) size is %d\n", 
	  root->nodeInfo()->nodeIndex(), 
	  (int)root->nodeInfo()->mDataset->ntaxa(), 
	  root->child1()->nodeInfo()->nodeIndex(), 
	  (int)root->child1()->nodeInfo()->mDataset->ntaxa(), 
	  root->child2()->nodeInfo()->nodeIndex(), 
	  (int)root->child2()->nodeInfo()->mDataset->ntaxa());
  */
}



/**
 * Calculate the match cost for column \acolA of datasets \a a and column \a colB of dataset \a b, and their secondary structure elements.
 * \param colA column index for dataset \a a.
 * \param a dataset with one or more sequences.
 * \param ssA secondary structure dataset with one or more sequences.
 * \param colB column index for dataset \a b.
 * \param B dataset with one or more sequences.
 * \param ssB secondary structure dataset with one or more sequences.
 * \return match score for column \acolA of datasets \a a and column \a colB of dataset \a b.
 */
double QSSAlign::progressiveSSMatCost(int colA, Dataset* a, SSDataset* ssA, int colB, Dataset* b, SSDataset* ssB){
  double matCost = 0.0;

  int taxaA = a->ntaxa();
  int taxaB = b->ntaxa();
  
  /* iterate over every taxon in a */
  for(int aIndex = 0; aIndex < taxaA; aIndex++){
    char charA = a->getCharacter(aIndex, colA);
    char ssCharA = ssA->getCharacter(aIndex, colA);
    /* iterate over every taxon in b */
    for(int bIndex = 0; bIndex < taxaB; bIndex++){
      matCost += ssMatCost(charA, ssCharA, b->getCharacter(bIndex, colB), ssB->getCharacter(bIndex, colB));
    }
  }
  /* normalize */
  matCost /= taxaA * taxaB;

  //fprintf( stderr, "mC%lf,%d\n", matCost, taxaA * taxaB);
  
  return matCost;
} /* END progressiveMatCost() */


/**
 * Set the values for the dynamic programming (DP) matrices via setTracebackSSPairwise() and similar functions.
 * \param a genetic sequence.
 * \param sizea number of characters in \a a.
 * \param b genetic sequence.
 * \param sizeb number of characters in \a b.
 * \return the alignment score.
 * \sa setTracebackSSPairwise()
 */
double QSSAlign::calculateDPMatrices(const char *a, const char* ssA, int sizea, const char *b, const char* ssB, int sizeb)
{
  int i, j;

  initDPMatrices( sizea, sizeb);
	
  int maxA = (penalizeLeadingGaps == true) ? sizea : sizea - 1;
  int maxB = (penalizeLeadingGaps == true) ? sizeb : sizeb - 1;

  /* for every character in a */
  for(i = 1; i <= maxA; i++)
    {
      /* for every character in b */
      for(j = 1; j <= maxB; j++)
	{
	  setTracebackSSPairwise(i, j, ssMatCost(a[i-1], ssA[i-1], b[j-1], ssB[j-1]), ssA[i-1], ssB[j-1]);
	}
    }

	
  if( penalizeLeadingGaps == false){
    /* allow for free gaps along the last row and column */

    j = sizeb;
    for(i = 1; i <= maxA; i++){
      setTracebackSSLastColPairwise(i, j, ssMatCost(a[i-1], ssA[i-1], b[j-1], ssB[j-1]), ssB[j-1]);
    }
	  
    i = sizea;
    for(j = 1; j <= maxB; j++){
      setTracebackSSLastRowPairwise(i, j, ssMatCost(a[i-1], ssA[i-1], b[j-1], ssB[j-1]), ssA[i-1]);
    }
    
    i = sizea;
    j = sizeb;
    setTracebackBottomRightPairwise(i, j, ssMatCost(a[i-1], ssA[i-1], b[j-1], ssB[j-1]));
  }
	
  return min3( M_MATRIX(sizea, sizeb), UP_MATRIX(sizea, sizeb), LEFT_MATRIX(sizea, sizeb));
}

/**
 * Return the match score for characters a and b (and their secondary structures ssA and ssB), using substitution matrices submat{,A,B,L,SS}.
 * \sa  MultChar::ssCost()
 */

double QSSAlign::ssMatCost(char a, char ssA, char b, char ssB){
  double ssMatCost = 0.0;
  
  if( ssA != SS_CHAR_GAP &&
      ssA != SS_CHAR_GAP_CODONS &&
      ssA == ssB){
    if( ssA == SS_CHAR_LOOP){
      ssMatCost = subMatA->get(a, b);
    }else if( ssA == SS_CHAR_BETA_STRAND){
      ssMatCost = subMatB->get(a, b);
    }else{
      if( ssA != SS_CHAR_HELIX){
	char str[1024];
	sprintf( str, "ERROR: ssMatCost(a: %c, ssA: %c, b: %c, ssB: %c) found non-3state SS char '%c'!", a, ssA, b, ssB, ssA);
	throw PsodaError( str);
      }
      ssMatCost = subMatL->get(a, b);
    }
  }else{
    ssMatCost = subMat->get(a, b);
  }

  return ((ssMatCost + subMatSS->get(ssA, ssB)) / 2.0);
}


/**
 * Calculates the Hamming distance between every pair of sequences in \a dataset.
 * Sets \a distMatrix with the distances.
 * \param dataset contains the primary sequences.
 * \param dataset contains the secondary structure sequences.
 * \sa calculateDPMatrices.
 */
void QSSAlign::calculateDistMatrix(Dataset *dataset, SSDataset* ssDataset)
{

  // allocate distMatrix memory for two dimensions
  distMatrix = new double*[ntaxa];
  for (int i = 0; i < ntaxa; ++i)
  {
    distMatrix[i] = new double[ntaxa];
  }
  
  double score;

  if( PSODA_VERBOSE){ fprintf( stderr, "%s", "Pairwise: "); }
  
  /* for each sequence */
  for (int i = 0; i < ntaxa; ++i)
  {
    distMatrix[i][i] = 0;
    
    if( PSODA_VERBOSE){ fprintf(stderr, "%s", "."); }
    /* with every other sequence */
    for (int j = i + 1; j < ntaxa; ++j)
    {
      score = calculateDPMatrices(dataset->getCharacters(i,false), ssDataset->getCharacters(i,true), dataset->getSeqLen(i), dataset->getCharacters(j,false), ssDataset->getCharacters(j,true), dataset->getSeqLen(j));
      //fprintf(stderr,"DP Matrix for %d and %d\n", i, j);
      //printDPMatrices(dataset->getCharacters(i), dataset->getCharacters(j));

      distMatrix[j][i] = distMatrix[i][j] = score;
    }
  }
  if( PSODA_VERBOSE){ fprintf(stderr, "%s", "."); }
}



/**
 * Set the traceback values for cell (i,j) (for pairwise--does not set the actual traceback pointers, just sets the scores).
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param mostCommonSSLeft the most common secondary structure element in the left sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set)
 * \param mostCommonSSUp the most common secondary structure element in the up sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set)
 */
void QSSAlign::setTracebackSSPairwise(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost, char mostCommonSSLeft, char mostCommonSSUp){

  /*
   * M_MATRIX
   */

  setTracebackDiagPairwise(i, j, matCost);

  /*
   * UP_MATRIX (Ix)
   */

  double gapOpenPath =  M_MATRIX(i-1, j) + gapOpenPenalty( GAPUPDIST(i-1, j), mostCommonSSLeft);
  double gapExtPath  = UP_MATRIX(i-1, j) + gapExt;

  ///* discourage singleton characters at the beginning */
  //if( j == 1){
  //  gapOpenPath += gapOpenPenalty( GAPUPDIST(i-1, j));
  //}
  
  /* choose the maximum value */
  if( gapOpenPath > gapExtPath){
    UP_MATRIX(i, j) = gapOpenPath;
  }else{
    UP_MATRIX(i, j) = gapExtPath;
  }
  
  /*
   * LEFT_MATRIX (Iy)
   */
  
  gapOpenPath =    M_MATRIX(i, j-1) + gapOpenPenalty( GAPLEFTDIST(i, j-1), mostCommonSSUp);
  gapExtPath  = LEFT_MATRIX(i, j-1) + gapExt;

  ///* discourage singleton characters at the beginning */
  //if( i == 1){
  //  gapOpenPath += gapOpenPenalty( GAPUPDIST(i, j-1));
  //}
  
  /* choose the maximum value */
  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
  }
  
} /* END setTracebackSSPairwise() */




/**
 * Set the traceback values for cell (i,j) for local alignment case
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param mostCommonSSLeft the most common secondary structure element in the left sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set).
 * \param mostCommonSSUp the most common secondary structure element in the up sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set).
 */
void QSSAlign::setLocalDPMatrics(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost, char mostCommonSSLeft, char mostCommonSSUp){

  /*
   * M_MATRIX
   */

  setLocalDPDiag( i, j, matCost);
  
  /*
   * UP_MATRIX (Ix)
   */

  double gapOpenPath =  M_MATRIX(i-1, j) + gapOpenPenalty( GAPUPDIST(i-1, j), mostCommonSSLeft);
  double gapExtPath  = UP_MATRIX(i-1, j) + gapExt;

  ///* discourage singleton characters at the beginning */
  //if( j == 1){
  //  gapOpenPath += gapOpenPenalty( GAPUPDIST(i-1, j));
  //}
  
  if( gapOpenPath > gapExtPath){
    UP_MATRIX(i, j) = max(gapOpenPath, 0.0);
  }else{
    UP_MATRIX(i, j) = max(gapExtPath, 0.0);
  }
  
  /*
   * LEFT_MATRIX (Iy)
   */
  
  gapOpenPath =    M_MATRIX(i, j-1) + gapOpenPenalty( GAPLEFTDIST(i, j-1), mostCommonSSUp);
  gapExtPath  = LEFT_MATRIX(i, j-1) + gapExt;

  ///* discourage singleton characters at the beginning */
  //if( i == 1){
  //  gapOpenPath += gapOpenPenalty( GAPUPDIST(i, j-1));
  //}

  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = max(gapOpenPath, 0.0);
  }else{
    LEFT_MATRIX(i, j) = max(gapExtPath, 0.0);
  }

  LOCAL_MATRIX(i, j) = max3( M_MATRIX(i, j), UP_MATRIX(i, j), LEFT_MATRIX(i, j)) * mLocalWeight;
  
} /* END setLocalDPMatrics() */


/**
 * Set the traceback values just for the last row (pairwise case only)
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param mostCommonSSLeft the most common secondary structure element in the left sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set).
 */
void QSSAlign::setTracebackSSLastRowPairwise(int i, int j, double matCost, char mostCommonSSLeft){

  /*
   * M_MATRIX
   */

  setTracebackDiagPairwise(i, j, matCost);
  
  /*
   * UP_MATRIX
   */

  double gapOpenPath =  M_MATRIX(i-1, j) + gapOpenPenalty( GAPUPDIST(i-1, j), mostCommonSSLeft);
  double gapExtPath  = UP_MATRIX(i-1, j) + gapExt;

  if( gapOpenPath > gapExtPath){
    UP_MATRIX(i, j) = gapOpenPath;
  }else{
    UP_MATRIX(i, j) = gapExtPath;
  }
  
  /*
   * LEFT_MATRIX
   */
  
  gapOpenPath =    M_MATRIX(i, j-1);
  gapExtPath  = LEFT_MATRIX(i, j-1);

  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
  }
  
} /* END setTracebackSSLastRowPairwise() */


/**
 * Set the traceback values just for the last column (pairwise case only)
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param mostCommonSSUp the most common secondary structure element in the up sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set).
 */
void QSSAlign::setTracebackSSLastColPairwise(int i, int j, double matCost, char mostCommonSSUp){

  /*
   * M_MATRIX
   */

  setTracebackDiagPairwise(i, j, matCost);
  
  /*
   * UP_MATRIX
   */

  double gapOpenPath =  M_MATRIX(i-1, j);
  double gapExtPath  = UP_MATRIX(i-1, j);

  if( gapOpenPath > gapExtPath){
    UP_MATRIX(i, j) = gapOpenPath;
  }else{
    UP_MATRIX(i, j) = gapExtPath;
  }
  
  /*
   * LEFT_MATRIX
   */
  
  gapOpenPath =    M_MATRIX(i, j-1) + gapOpenPenalty( GAPLEFTDIST(i, j-1), mostCommonSSUp);
  gapExtPath  = LEFT_MATRIX(i, j-1) + gapExt;

  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
  }
  
} /* END setTracebackSSLastColPairwise() */



/**
 * Set the traceback values for cell (i,j).
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param mostCommonSSLeft the most common secondary structure element in the left sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set).
 * \param mostCommonSSUp the most common secondary structure element in the up sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set).
 * \param percentNotGapsLeft the percentage of characters for the left sequences that are not gaps.  Note: only present when \a ADVANCED_PROGRESSIVE_GAP_PENALTIES is defined.
 * \param percentNotGapsUp the percentage of characters for the up sequence that are not gaps.  Note: only present when \a ADVANCED_PROGRESSIVE_GAP_PENALTIES is defined.
 */
void QSSAlign::setTracebackSS(int i, int j, double matCost, char mostCommonSSLeft, char mostCommonSSUp
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			  , float percentNotGapsLeft, float percentNotGapsUp
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			      ){

  /*
   * M_MATRIX
   */

  setTracebackDiag(i, j, matCost);
  
  /*
   * UP_MATRIX
   */

  /* open a new gap */
  double gapOpenPath =  M_MATRIX(i-1, j) + gapOpenPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
							  percentNotGapsLeft,
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
							  GAPUPDIST(i-1, j), mostCommonSSLeft);
  /* extend existing gap */
  double gapExtPath  = UP_MATRIX(i-1, j) + gapExtPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
							 percentNotGapsLeft
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
							 );
  ///* discourage singleton characters at the beginning */
  //if( j == 1){
  //  gapOpenPath += gapOpenPenalty( GAPUPDIST(i-1, j));
  //}
  
  /* determine maximum value */
  if( gapOpenPath > gapExtPath){
    UP_MATRIX(i, j) = gapOpenPath;
    UP_TRACEBACK(i, j).next = &(M_TRACEBACK(i-1, j));
  }else{
    UP_MATRIX(i, j) = gapExtPath;
    UP_TRACEBACK(i, j).next = &(UP_TRACEBACK(i-1, j));
  }
  
  /*
   * LEFT_MATRIX
   */
  
  /* open a new gap */
  gapOpenPath =    M_MATRIX(i, j-1) + gapOpenPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
						     percentNotGapsUp,
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
						     GAPLEFTDIST(i, j-1), mostCommonSSUp);
  /* extend existing gap */
  gapExtPath  = LEFT_MATRIX(i, j-1) + gapExtPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
						    percentNotGapsUp
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
						    );

  ///* discourage singleton characters at the beginning */
  //if( i == 1){
  //  gapOpenPath += gapOpenPenalty( GAPUPDIST(i, j-1));
  //}

  /* determine maximum value */
  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
    LEFT_TRACEBACK(i, j).next = &(M_TRACEBACK(i, j-1));
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
    LEFT_TRACEBACK(i, j).next = &(LEFT_TRACEBACK(i, j-1));
  }
} /* END setTracebackSS() */


/**
 * Set the traceback values just for the last row.
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param mostCommonSSLeft the most common secondary structure element in the left sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set).
 * \param percentNotGapsLeft the percentage of characters for the left sequences that are not gaps.  Note: only present when \a ADVANCED_PROGRESSIVE_GAP_PENALTIES is defined.
*/
void QSSAlign::setTracebackSSLastRow(int i, int j, double matCost, char mostCommonSSLeft
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
				 , float percentNotGapsLeft
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
				 ){

  /*
   * M_MATRIX
   */

  setTracebackDiag(i, j, matCost);

  
  /*
   * UP_MATRIX
   */

  double gapOpenPath =  M_MATRIX(i-1, j) + gapOpenPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
							  percentNotGapsLeft,
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
							  GAPUPDIST(i-1, j), mostCommonSSLeft);
  double gapExtPath  = UP_MATRIX(i-1, j) + gapExtPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
							 percentNotGapsLeft
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
							 );

  if( gapOpenPath > gapExtPath){
    UP_MATRIX(i, j) = gapOpenPath;
    UP_TRACEBACK(i, j).next = &(M_TRACEBACK(i-1, j));
  }else{
    UP_MATRIX(i, j) = gapExtPath;
    UP_TRACEBACK(i, j).next = &(UP_TRACEBACK(i-1, j));
  }
  
  /*
   * LEFT_MATRIX
   */
  
  gapOpenPath =    M_MATRIX(i, j-1);
  gapExtPath  = LEFT_MATRIX(i, j-1);

  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
    LEFT_TRACEBACK(i, j).next = &(M_TRACEBACK(i, j-1));
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
    LEFT_TRACEBACK(i, j).next = &(LEFT_TRACEBACK(i, j-1));
  }
} /* END setTracebackSSLastRow() */


/**
 * Set the traceback values just for the last column.
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param mostCommonSSUp the most common secondary structure element in the up sequences (used when ADVANCED_PROGRESSIVE_GAP_PENALTIES is set).
  * \param percentNotGapsUp the percentage of characters for the up sequence that are not gaps.  Note: only present when \a ADVANCED_PROGRESSIVE_GAP_PENALTIES is defined.
 */
void QSSAlign::setTracebackSSLastCol(int i, int j, double matCost, char mostCommonSSUp
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
				 , float percentNotGapsUp
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
				 ){  

  /*
   * M_MATRIX
   */

  setTracebackDiag(i, j, matCost);
  
  /*
   * UP_MATRIX
   */

  double gapOpenPath =  M_MATRIX(i-1, j);
  double gapExtPath  = UP_MATRIX(i-1, j);

  if( gapOpenPath > gapExtPath){
    UP_MATRIX(i, j) = gapOpenPath;
    UP_TRACEBACK(i, j).next = &(M_TRACEBACK(i-1, j));
  }else{
    UP_MATRIX(i, j) = gapExtPath;
    UP_TRACEBACK(i, j).next = &(UP_TRACEBACK(i-1, j));
  }
  
  /*
   * LEFT_MATRIX
   */
  
  gapOpenPath =    M_MATRIX(i, j-1) + gapOpenPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
						     percentNotGapsUp,
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
						     GAPLEFTDIST(i, j-1), mostCommonSSUp);
  gapExtPath  = LEFT_MATRIX(i, j-1) + gapExtPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
						    percentNotGapsUp
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
						    );

  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
    LEFT_TRACEBACK(i, j).next = &(M_TRACEBACK(i, j-1));
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
    LEFT_TRACEBACK(i, j).next = &(LEFT_TRACEBACK(i, j-1));
  }
} /* END setTracebackSSLastCol() */



/**
 * Set the traceback values for cell (i,j).
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param ssA secondary structure dataset for the left sequence.
 * \param ssB secondary structure dataset for the up sequence.
 */
void QSSAlign::setTracebackSS(int i, int j, double matCost, SSDataset* ssA, SSDataset* ssB){
  char mostCommonSSA = '\0';
  char mostCommonSSB = '\0';

  char str[1024];
  sprintf( str, "%s", "ERROR: function not yet implementedn\n");
  throw PsodaError( str);

  
  if( ssA != NULL &&
      ssB != NULL){
  setTracebackSS(i, j, matCost, mostCommonSSA, mostCommonSSB);
  }
} /* END setTracebackSS() */


/**
 * Set the traceback values just for the last row.
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param ssA secondary structure dataset for the left sequence.
*/
void QSSAlign::setTracebackSSLastRow(int i, int j, double matCost, SSDataset* ssA){
  char mostCommonSSA = '\0';
  
  char str[1024];
  sprintf( str, "%s", "ERROR: function not yet implementedn\n");
  throw PsodaError( str);
  
  if( ssA != NULL){
  setTracebackSSLastRow(i, j, matCost, mostCommonSSA);
  }
} /* END setTracebackSSLastRow() */


/**
 * Set the traceback values just for the last column.
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param ssB secondary structure dataset for the up sequence.
 */
void QSSAlign::setTracebackSSLastCol(int i, int j, double matCost, SSDataset* ssB){ 
  char mostCommonSSB = '\0';
  
  char str[1024];
  sprintf( str, "%s", "ERROR: function not yet implementedn\n");
  throw PsodaError( str);
  
  if( ssB != NULL){
  setTracebackSSLastCol(i, j, matCost, mostCommonSSB);
  }
} /* END setTracebackSSLastCol() */



/**
 * Calculate the gap open penalty based on the distance from the last gap (\a distFromLastGap).
 * \param distFromLastGap the distance from the last gap.
 * \param mostCommonSSInOther the most common secondary structure element in the other sequence
 * \return the gap open penalty.
 * \sa QAlign::gapOpenPenalty()
 */
float QSSAlign::gapOpenPenalty(int distFromLastGap, char mostCommonSSInOther){
  if( distFromLastGap < mGapDistanceSetting){
    /* gapExt if distFromLastGap == 0, else, a scaled version of gapOpen to prevent opening a gap close to an existing gap*/

    if( mostCommonSSInOther != SS_CHAR_GAP &&
	mostCommonSSInOther != SS_CHAR_GAP_CODONS){
      if( mostCommonSSInOther == SS_CHAR_LOOP){
	return gapLookupLoop[ distFromLastGap];
      }else if( mostCommonSSInOther == SS_CHAR_HELIX){
	return gapLookupHelix[ distFromLastGap];
      }else{
	if( mostCommonSSInOther != SS_CHAR_BETA_STRAND){
	  char str[1024];
	  sprintf( str, "ERROR: QSSAlign::gapOpenPenalty(distFromLastGap: %d, mostCommonSSInOther: %c) found non-3state SS char '%c'!", distFromLastGap, mostCommonSSInOther, mostCommonSSInOther);
	  throw PsodaError( str);
	}
	return gapLookupBetaStrand[ distFromLastGap];
      }
    }else{
      return gapLookup[ distFromLastGap];
    }
  }else{
    if( mostCommonSSInOther != SS_CHAR_GAP &&
	mostCommonSSInOther != SS_CHAR_GAP_CODONS){
      if( mostCommonSSInOther == SS_CHAR_LOOP){
	return gapOpenLoop;
      }else if( mostCommonSSInOther == SS_CHAR_HELIX){
	return gapOpenHelix;
      }else{
	if( mostCommonSSInOther != SS_CHAR_BETA_STRAND){
	  char str[1024];
	  sprintf( str, "ERROR: QSSAlign::gapOpenPenalty(distFromLastGap: %d, mostCommonSSInOther: %c) found non-3state SS char '%c'!", distFromLastGap, mostCommonSSInOther, mostCommonSSInOther);
	  throw PsodaError( str);
	}
	return gapOpenBetaStrand;
      }
    }else{
      return gapOpen;
    }
  }
}

