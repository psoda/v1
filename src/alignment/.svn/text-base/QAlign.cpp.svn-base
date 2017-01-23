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
// To Do


#include "QAlign.h"
#include "Dist.h"
#include "QNode.h"
#include "QNodeInfo.h"
#include "QTree.h"
#include "Timer.h"
#include "PsodaPrinter.h"
#include "Globals.h"
#include "Interpreter.h"
#include <stdio.h>
#include <list>
#include <algorithm>
#include <float.h>
#include <assert.h>
#include <math.h>
#include <string.h>

/**
 * Set up a QAlign object.
 * \param gapOpenIn gap open penalty (negative values only).
 * \param gapExtIn gap extension penalty (negative value only).
 * \param subMatIn substition matrix to use for alignment.
 * \param penalizeLeadingGapsIn if set, penalize leading gaps (the gaps on the ends of the sequences)
 * \param useClassicPMSA if set, use the stock (unoptimized) progressivive sequence alignment methods of iterating through every character in one sub-alignment with every other character in the other alignment.  If no set, use the optimizied MultChars.
 * \param gapDistanceSetting penalize opening a new gap within this distance to another gap.
 * \param localWeight amount of contribution of local alignment wieghts.  Range form 0.0 to 1.0.  Set to 0.0 to ignore local alignment weights.
 */
QAlign::QAlign(float gapOpenIn, float gapExtIn, SubMat* subMatIn, bool penalizeLeadingGapsIn, bool useClassicPMSA, int gapDistanceSetting, float localWeight)
  : ntaxa(0),
    nchars(0),
    distMatrix(NULL),
    dpRowsAlloced(0),
    dpColsAlloced(0),
    mMatrix(NULL),
    upMatrix(NULL),
    leftMatrix(NULL),
    upLocalMatrix(NULL),
    leftLocalMatrix(NULL),
    localMatrix(NULL),
    mTraceback(NULL),
    upTraceback(NULL),
    leftTraceback(NULL),
    upGapDist(NULL),
    leftGapDist(NULL),
    mGapChar('-'),
    mEndChar('\0'),
    gapOpen(gapOpenIn),
    gapExt(gapExtIn),
    mGapDistanceSetting( gapDistanceSetting),
    gapLookup(NULL),
    penalizeLeadingGaps( penalizeLeadingGapsIn),
    guideTree(NULL),
    subMat(subMatIn),
    mUseClassicPMSA(useClassicPMSA),
    mLocalWeight(localWeight)
{
  Dataset* dataset = Interpreter::getInstance()->dataset();
  
  ntaxa = dataset->ntaxa();
  nchars = dataset->nchars();
  
  initDPMatrices( nchars, nchars);

  /* populate gap penalties based on distance from the last gap */
  if( mGapDistanceSetting < 1){
    mGapDistanceSetting = 1;
  }
  gapLookup = (float*) check_calloc( mGapDistanceSetting, sizeof( float));
  gapLookup[0] = gapExt;

  /* future: clustalw only sets the gapdist penalty for amino acids */
  for( int dist = 1; dist < mGapDistanceSetting; dist++){
    gapLookup[dist] = (2 + ((mGapDistanceSetting - dist)*2)/mGapDistanceSetting)*gapOpen;
  }

  mGapChar = dataset->gapchar();
  mEndChar = dataset->endchar();

  if( PSODA_DEBUG){
    PsodaPrinter::getInstance()->write( "QAlign(gop = %f, gep = %f, submat = %s, penalizeLeadingGapsIn = %s)\n", gapOpenIn, gapExtIn, subMat->getName().c_str(), penalizeLeadingGapsIn == true ? "true" : "false");
  }
}

QAlign::~QAlign()
{
  /* future: check to make sure they're alloced before deleting them */
  if( distMatrix){
    for (int i = 0; i < ntaxa; ++i)
      {
	delete [] distMatrix[i];
      }
  
    delete [] distMatrix;
  }

  if( gapLookup){
    free( gapLookup);
  }
  
  freeDPMatrices();
}


/**
 * The actual alignment method.
 * Use the tree repository if \a useExistingTree is set.
 * Write aligned dataset to \a alignedDataset.
 * \param qtreeRepository tree repository object.
 * \param useExistingTree if set, use a tree from the tree repository.
 * \param alignedDataset pointer to resulting (aligned) dataset.
 */
void QAlign::align(QTreeRepository &qtreeRepository, bool useExistingTree, Dataset** alignedDataset)
{
  QTreeRepository *tempRepository;
  PsodaPrinter* printBuffer = PsodaPrinter::getInstance();
  Dataset* dataset = Interpreter::getInstance()->dataset();
  /*
    vector<string>::iterator nameItr = dataset->taxonNames().begin();
    vector<string>::iterator nameEnd = dataset->taxonNames().end();
    int i = 0;
    for (; nameItr != nameEnd; nameItr++) {
    printf("name: %s\n", nameItr->c_str());
    printf("chars: %s\n", dataset->getCharacters(i++));
    }
  */
    printBuffer->write( "align()\n"); 
    printBuffer->write( "gap open: %f\ngap ext: %f\nsub mat: %s\n", gapOpen, gapExt, subMat->getName().c_str());
    if( fabs(mLocalWeight) < .001){
      printBuffer->write( "local weight: %f\n", mLocalWeight);
    }


  if( dataset->ntaxa() <= 1){
    char str[1024];
    sprintf( str, "ERROR: Can not perform alignment with %d taxa!", dataset->ntaxa());
    throw PsodaError(str);
  }
  
  // We need to make sure we don't have any gaps in the data
  dataset->stripGaps();
  
  Timer timer;
  
  QTree *qtree;
  if( useExistingTree == false)
  {
    // if there is no tree in the repository or we don't want to start from an existing tree, 
    // calculate a distance matrix and then a neighbor join tree
      timer.start();
    
    calculateDistMatrix(dataset);
    
      TimerSecondMicros sm1 = timer.getCurrentSecondMicros();
      printBuffer->write("Matrix calculation took %li.%li seconds\n" , sm1.seconds , sm1.micros );

    // Use distance matrix to calculate neighbor-join tree, and puts it in the tree repo
    DistSearch *dist = new DistSearch(ntaxa, distMatrix); 

      timer.start();
    tempRepository = new QTreeRepository;
    dist->search(*tempRepository, NULL, NULL, 1); /* 1 = number interations */

      TimerSecondMicros sm2 = timer.getCurrentSecondMicros();
      printBuffer->write("Neighbor-join tree generation took %li.%li seconds\n" , sm2.seconds , sm2.micros );
      qtree = tempRepository->getTree(); 
  }
  else
  {
      qtree = qtreeRepository.getTree(); 
  }
    
  //if( PSODA_DEBUG){ printBuffer->write("HY: Get the tree from the repository\n"); }
  // Get the tree from the repository and do a postorder traversal of the tree to create final multiple sequence alignment
  if( qtree == NULL){
    printBuffer->write("ERROR getting tree from tree repository\n");
    exit(-1);
  }
  if( PSODA_DEBUG){ fprintf(stderr, "Using %s for alignment\n", qtree->treeStr()); }

  // We need to create another QNode with appropriate QNodeInfo and pointers to create an artificial root
    timer.start();
    printBuffer->write("%s", "Progressive Alignment: ");
  QNodeInfo *rootinfo = new QNodeInfo(-1, ISNTLEAF);
  QNode *root = new QNode(*rootinfo, ISNTLEAF);
  root->child1() = qtree->root();
  root->child2() = qtree->root()->external();

  if( (*alignedDataset) != NULL){
    delete (*alignedDataset);
  }
  /* set parameters of new dataset */
  (*alignedDataset) = new Dataset();
  (*alignedDataset)->datatype() = dataset->datatype();
  (*alignedDataset)->dataformat() = Dataset::ALIGNED_DATAFORMAT;
  (*alignedDataset)->gapchar() = dataset->gapchar();
  //(*alignedDataset)->gapvalue() = dataset->gapvalue();
  (*alignedDataset)->missingchar() = dataset->missingchar();
  (*alignedDataset)->matchchar() = dataset->matchchar();
  (*alignedDataset)->setntaxa( dataset->ntaxa());
    

  /* the classic approach is a stock progressive multiple sequence alignment */
  if( mUseClassicPMSA){
    classicPMSA(root, dataset);
    Dataset* unOrderedAlignedDataset = root->nodeInfo()->mDataset;

    // re-order taxa
    (*alignedDataset)->setnchars( unOrderedAlignedDataset->nchars());
    for( int i = 0; i < dataset->ntaxa(); i++){
      (*alignedDataset)->addName( dataset->getTaxonName(i));
      (*alignedDataset)->addCharacters( unOrderedAlignedDataset->getCharacters( unOrderedAlignedDataset->getTaxonNumber( dataset->getTaxonName(i)),false));
    }

    delete unOrderedAlignedDataset;
    
  }else{
    PMSA(root, dataset);
      
    // Now we have to push the new gaps down through to the leaves.
    (*alignedDataset)->setnchars( dataset->nchars());
    for( int i = 0; i < dataset->ntaxa(); i++){
      (*alignedDataset)->addName( dataset->getTaxonName(i));
    }

    pushGapsDown(root, dataset, (*alignedDataset));

    //(*alignedDataset)->prologue();
  }
  
  cleanUpAlignment(*alignedDataset);
  
    printBuffer->write("%s", "\n");
    TimerSecondMicros sm3 = timer.getCurrentSecondMicros();
    printBuffer->write("Progressive alignment took %li.%li seconds\n" , sm3.seconds , sm3.micros );
    
  if( useExistingTree == false)
  {
      delete tempRepository;
  }

  // Be sure to traverse the tree and clean out all the mMultChar lists

  // delete the artificial root and its QNodeInfo
  //delete rootinfo; /* does this get deleted when root is deleted */
  delete root;
}

/**
 * Post-order traversal of the tree (starting at \a root) for dataset \a dataset, assigning the characters and gaps to \a alignedDataset.
 * \param root is the starting point for further recursion
 * \param dataset current dataset.
 * \param alignedDataset dataset to write aligned sequences to.
 * \sa pushGaps()
 */
void QAlign::pushGapsDown(QNode *root, Dataset* dataset, Dataset* alignedDataset)
{
  if (root->nodeInfo()->leaf())
    {

      if( PSODA_DEBUG){
	assert( dataset->getTaxonNumber(root->nodeInfo()->getTaxonName()) == root->nodeInfo()->nodeIndex());
      }

      //fprintf( stderr, "DEBUG: seq size = %d (\"%s\")\n", (int)root->nodeInfo()->mMultChar->size(), root->nodeInfo()->getTaxonName());


      /* pull sequence information out of the multChars and put it in the aligned dataset */
      char* seq = new char[root->nodeInfo()->mMultChar->size() + 1];

      int i = 0;
      list<MultChar *>::iterator a_it = root->nodeInfo()->mMultChar->begin();
      while (a_it != root->nodeInfo()->mMultChar->end())
        {
	  if( PSODA_DEBUG){
	    assert( (*a_it)->getCount() == 1);
	    //(*a_it)->print();
	  }
	  seq[i++] = (char)((*a_it)->getFirst());
 	  a_it++;
        }
      seq[i] = mEndChar;
      //fprintf( stderr, ", %d:\t%s\n", root->nodeInfo()->nodeIndex(), seq);
      alignedDataset->addCharacters( (const char*)seq, root->nodeInfo()->nodeIndex());
    }
  else
    {
      /* recurse */
      pushGaps(root);
      pushGapsDown(root->child1(), dataset, alignedDataset);
      pushGapsDown(root->child2(), dataset, alignedDataset);
    }
}



/**
 * Make the two child nodes of root to be the same length of root (by adding gaps).
 * \param root is the starting point for further recursion
 */
void QAlign::pushGaps(QNode *root)
{
    // Go through root's mMultChar list and each childs mMultChar list
    list<MultChar *>::iterator r_it, a_it, b_it;
    r_it = root->nodeInfo()->mMultChar->begin();
    a_it = root->child1()->nodeInfo()->mMultChar->begin();
    b_it = root->child2()->nodeInfo()->mMultChar->begin();
    int a = 0, b = 0, r = 0;

    /*
    fprintf(stderr, "Pushing gaps down for node %d my size is %d child1(%d) size is %d child2(%d) size is %d\n", 
            root->nodeInfo()->nodeIndex(), 
            (int)root->nodeInfo()->mMultChar->size(), 
            root->child1()->nodeInfo()->nodeIndex(), 
            (int)root->child1()->nodeInfo()->mMultChar->size(), 
            root->child2()->nodeInfo()->nodeIndex(), 
            (int)root->child2()->nodeInfo()->mMultChar->size());

    fprintf(stderr, "pushGaps(): (r: size: %d, a: size: %d, b: size: %d) (before)\n", (int)root->nodeInfo()->mMultChar->size(), (int)root->child1()->nodeInfo()->mMultChar->size(), (int)root->child2()->nodeInfo()->mMultChar->size());
    */
    while (r_it != root->nodeInfo()->mMultChar->end())
    {
        if (a_it == root->child1()->nodeInfo()->mMultChar->end() &&
	    b_it == root->child2()->nodeInfo()->mMultChar->end())
        {
	  /* insert gaps on the end of the child sequences */
	  root->child1()->nodeInfo()->mMultChar->push_back(new MultChar(mGapChar, true, 1, mGapChar));
            a_it--;
            //fprintf(stderr,"Reached end of list a --> Iterator is now at "); (*a_it)->print();
            //fprintf(stderr,"r_it is at "); (*r_it)->print(); fprintf(stderr,"\n");

            root->child2()->nodeInfo()->mMultChar->push_back(new MultChar(mGapChar, true, 1, mGapChar));
            b_it--;
            //fprintf(stderr, "Reached end of list b --> Iterator is now at "); (*b_it)->print();
            //fprintf(stderr,"r_it is at "); (*r_it)->print(); fprintf(stderr,"\n");

            r_it++;

            //fprintf(stderr,"\nComparing "); (*r_it)->print(); fprintf(stderr," to "); (*a_it)->print(); (*b_it)->print(); fprintf(stderr, "\n");
        }
        else if ((*r_it)->isMarked())
        {
	  /* insert gap into child sequences */
	  //fprintf(stderr,"Marked Gap in parent-->\n");
	  root->child1()->nodeInfo()->mMultChar->insert(a_it, new MultChar(mGapChar, true, 1, mGapChar));
	  root->child2()->nodeInfo()->mMultChar->insert(b_it, new MultChar(mGapChar, true, 1, mGapChar));
            r_it++;
            r++;
        }
        else
        {
            r_it++;
            a_it++;
            b_it++;
            r++;
            a++;
            b++;
        }
    }
    /*
    fprintf(stderr, "Finished pushing gaps down with r=%d, a=%d, b=%d\n", r, a, b);
    printf("Finished Pushing gaps down for node %d my size is %d child1(%d) size is %d child2(%d) size is %d\n", 
            root->nodeInfo()->nodeIndex(), 
	   (int)root->nodeInfo()->mMultChar->size(), 
            root->child1()->nodeInfo()->nodeIndex(), 
	   (int)root->child1()->nodeInfo()->mMultChar->size(), 
            root->child2()->nodeInfo()->nodeIndex(), 
	   (int)root->child2()->nodeInfo()->mMultChar->size());

    fprintf(stderr, "pushGaps(): (r: size: %d, a: size: %d, b: size: %d) (after)\n", (int)root->nodeInfo()->mMultChar->size(), (int)root->child1()->nodeInfo()->mMultChar->size(), (int)root->child2()->nodeInfo()->mMultChar->size());
    */
}

/**
 * Progressive Multiple Sequence Alignment.
 * Post-order traversal starting at root for dataset dataset.
 * The base case for recursion is if it's a leaf node (i.e., user inputted sequence data)
 * \param root is the starting point for further recursion
 * \param dataset current dataset.
 * \sa setupMultChar(), progressiveAlign()
 */
void QAlign::PMSA(QNode *root, Dataset *dataset)
{
    checkPausePSODA();
    if (root->nodeInfo()->leaf())
    {
        setupMultChar(root, dataset);
        //printf("Leaf %d my size is %d\n", root->nodeInfo()->nodeIndex(), root->nodeInfo()->mMultChar->size());
    }
    else
    {
      /* recurse */
        PMSA(root->child1(), dataset);
        PMSA(root->child2(), dataset);
        progressiveAlign(root);
    }

}


/**
 * Makes MultChar data structures for each character in the sequence for root.
 * \param root is the starting point for further recursion
 * \param dataset current dataset.
 */
void QAlign::setupMultChar(QNode *root, Dataset *dataset)
{
  assert( root);
  assert( root->nodeInfo());
  if( root->nodeInfo()->mMultChar){
    delete root->nodeInfo()->mMultChar;
  }
    
  const char *data = dataset->getCharacters( root->nodeInfo()->nodeIndex(),false);
  int dataLen = dataset->getSeqLen( root->nodeInfo()->nodeIndex());
  root->nodeInfo()->mMultChar = new list<MultChar*>();

  for (int i = 0; i < dataLen; i++)
    {
      root->nodeInfo()->mMultChar->push_back(new MultChar(data[i]));
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
 * Wrapper for multCharAlign().
 * \param root is the starting point for further recursion
 * \sa multCharAlign()
 */
void QAlign::progressiveAlign(QNode *root)
{
  //fprintf(stderr,"Progressively aligning %d and %d\n", root->child1()->nodeInfo()->nodeIndex(), root->child2()->nodeInfo()->nodeIndex());
  
    /*
    int i;
    fprintf(stderr,"\t %d: ", root->child1()->nodeInfo()->nodeIndex());
    for(i = 1, a_it = a->begin(); i <= sizea; a_it++, i++)
    {
      (*a_it)->print();
    }
    fprintf(stderr,"%s", "\n");
    fprintf(stderr,"\t %d: ", root->child2()->nodeInfo()->nodeIndex());
    for(i = 1, b_it = b->begin(); i <= sizeb; b_it++, i++)
    {
      (*b_it)->print();
    }
    fprintf(stderr,"%s", "\n");
    */
    if( PSODA_VERBOSE){ fprintf(stderr, "%s", "."); }
    
    multCharAlign(root);

    /*
    fprintf(stderr, "After alignment for node %d my size is %d child1(%d) size is %d child2(%d) size is %d\n", 
            root->nodeInfo()->nodeIndex(), 
            (int)root->nodeInfo()->mMultChar->size(), 
            root->child1()->nodeInfo()->nodeIndex(), 
            (int)root->child1()->nodeInfo()->mMultChar->size(), 
            root->child2()->nodeInfo()->nodeIndex(), 
            (int)root->child2()->nodeInfo()->mMultChar->size());
    */
}

/**
 * Align each of the children of root, comparing each of the MultChars.
 * \param root is the starting point for further recursion
 */
void QAlign::multCharAlign(QNode *root)
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
          matcost = (*a_it)->evalCost((*b_it), subMat);
	  //if( i == maxA ||
	  //    j == maxB){
	  //  /* prevents singleton characters at the end of the alignment */	    
	  //  matcost -= 0.1;
	  //}
	  setTraceback( i, j, matcost
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			, aPercentNotGaps, (*b_it)->getPercentNotGaps()
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			);
	}
    }

    if( penalizeLeadingGaps == false){
      /* allow for free gaps along the last row and column */

      i = sizea;
      a_it = a->end();
      a_it--;
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
      aPercentNotGaps = (*a_it)->getPercentNotGaps();
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
      for(j = 1, b_it = b->begin(); j <= maxB; b_it++, j++){
	  //fprintf(stderr, "Working on (%d,%d)", i, j);
          matcost = (*a_it)->evalCost((*b_it), subMat);
	  setTracebackLastRow( i, j, matcost
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			       , aPercentNotGaps
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			       );
      }
      
      j = sizeb;
      b_it = b->end();
      b_it--;
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
      bPercentNotGaps = (*b_it)->getPercentNotGaps();
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
      for(i = 1, a_it = a->begin(); i <= maxA; a_it++, i++){
	  //fprintf(stderr, "Working on (%d,%d)", i, j);
          matcost = (*a_it)->evalCost((*b_it), subMat);
	  setTracebackLastCol( i, j, matcost
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			       , bPercentNotGaps
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
      matcost = (*a_it)->evalCost((*b_it), subMat);
      setTracebackBottomRight( i, j, matcost);
    }

    //if( PSODA_DEBUG){
      //PsodaPrinter::getInstance()->write("multCharAlign score %f\n", DPMATRIX(sizea, sizeb));
      if( PSODA_DEBUG > 9){
	printDPMatrices(a, b);
      }
      //}

      /*
       * traceback
       */
     progressiveTraceback(root);
} /* END multCharAlign() */

/**
 * Do the traceback starting at the "bottom right" cell.
 * Begin at \a traceback[sizea][sizeb] and go back to \a traceback[1][1].
 * Put the results in parents MultChar list.
 * \param root is the starting point for further recursion
 */
void QAlign::progressiveTraceback(QNode* root){
  
    list<MultChar *> *a = root->child1()->nodeInfo()->mMultChar;
    list<MultChar *> *b = root->child2()->nodeInfo()->mMultChar;
    list<MultChar *>::iterator a_it, b_it;
    int sizea = a->size();
    int sizeb = b->size();
    
    root->nodeInfo()->mMultChar = new list<MultChar *>;
    Traceback *temp = getBottomRightTraceBack( sizea, sizeb);
    
    int taxaA = (*(a->begin()))->getTotal();
    int taxaB = (*(b->begin()))->getTotal();

    a_it = a->end();
    b_it = b->end();
    while(temp->next != NULL)
    {
      if( PSODA_DEBUG > 1){
	fprintf( stderr, "Considering a: ");
	if( a_it != a->begin()){
	  a_it--;
	  if( *a_it != NULL){
	    (*a_it)->print();
	  }
	  a_it++;
	}
	fprintf( stderr, ", b: ");
	if( b_it != b->begin()){
	  b_it--;
	  if( *b_it != NULL){
	    (*b_it)->print();
	  }
	  b_it++;
	}
	fprintf( stderr, ", ");
      }

      /* Traverse through matrix traceback values based, either moving diagonally (up 1 and to the left 1), up 1 or to the left 1 cell */ 
      MultChar *k = new MultChar();
      if (temp->direction == Diag)
        {
	  if( PSODA_DEBUG > 1){ fprintf( stderr, "Diag "); }
	  a_it--;
	  b_it--;
	  k->add((*a_it), (*b_it));
        }
      else if (temp->direction == Up)
        {
	  if( PSODA_DEBUG > 1){ fprintf( stderr, "Up "); }
	  // Mark this gap as one that needs to be propagated down - don't mark my parents copy
	  MultChar *g = new MultChar(mGapChar, true, taxaB, mGapChar);
	  b->insert(b_it, g);
	  b_it--;

	  a_it--;
	  k->add((*a_it), g);
        }
      else if (temp->direction == Left)
        {
	  if( PSODA_DEBUG > 1){ fprintf( stderr, "Left "); }
	  // Mark this gap as one that needs to be propagated down - don't mark my parents copy
	  MultChar *g = new MultChar(mGapChar, true, taxaA, mGapChar);
	  a->insert(a_it, g);
	  a_it--;

	  b_it--;
	  k->add((*b_it), g);
        }
      else
        {
	  //fprintf(stderr, "ERROR: Alignment DPMatrices Direction is equal to %d\n", (int)(temp->direction));
	  exit( 1);
        }
      root->nodeInfo()->mMultChar->push_front(k);
      temp = temp->next;
      if( PSODA_DEBUG > 1){ 	fprintf( stderr, " k: "); k->print(); fprintf(stderr, "\n"); }
    }

    if( PSODA_DEBUG){
      //fprintf( stderr, "\n");
      //printDPMatrices(sizea, sizeb);
      fprintf(stderr, "Aligned Sequences: (a: size: %d, b: size: %d)\n", (int)a->size(), (int)b->size());
      a_it = a->begin();
      while (a_it != a->end())
	{
	  (*a_it)->print();
	  a_it++;
	}
      fprintf(stderr,"\n");
      b_it = b->begin();
      while (b_it != b->end())
	{
	  (*b_it)->print();
	  b_it++;
	}
      fprintf(stderr,"\n");

      list<MultChar *> *k = root->nodeInfo()->mMultChar;
      list<MultChar *>::iterator k_it;
    
      fprintf(stderr,"Aligned seq: (size: %d)\n", (int)k->size());
      k_it = k->begin();
      while (k_it != k->end())
	{
	  (*k_it)->print();
	  k_it++;
	}
      fprintf(stderr,"\n");
    }
}

/**
 * Align \a a and \a b, comparing each of the \a MultChars.
 * Account for local alignment weights.
 * \param a is a list \a MultChars (equilvent to a profile)
 * \param b is a list \a MultChars (equilvent to a profile)
 */
void QAlign::multCharLocalAlign(list<MultChar *> *a, list<MultChar *> *b)
{
    int i,j;
    list<MultChar *>::iterator a_it, b_it;
    int sizea = a->size();
    int sizeb = b->size();
    double matcost;

    initDPMatrices( sizea, sizeb);

    int maxA = sizea;
    int maxB = sizeb;

    if( PSODA_DEBUG){
      fprintf(stderr, "\nSequences To (local) Align:\n");
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

    /* iterate over each of MultChars in one of the child sequences */
    for(i = 1, a_it = a->begin(); i <= maxA; a_it++, i++)
    {
      /* iterate over each of MultChars in the other child sequences */
      for(j = 1, b_it = b->begin(); j <= maxB; b_it++, j++)
	{
	  //fprintf(stderr, "Working on (%d,%d)", i, j);
          matcost = (*a_it)->evalCost((*b_it), subMat);
	  setLocalDPMatrics( i, j, matcost);
	}
    }

    if( PSODA_DEBUG){
      //PsodaPrinter::getInstance()->write("classicProgressiveAlign score %f\n", DPMATRIX(sizea, sizeb));
      if( PSODA_DEBUG > 9){
	printDPMatrices(a, b);
      }
    }
    
} /* END multCharLocalAlign() */



/**
 * Set the traceback values for cell (i,j) just for the diagonal case (for pairwise).
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 */
void QAlign::setTracebackDiagPairwise(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost){
  
  double diagM    =    M_MATRIX(i-1, j-1) + matCost; // coming from two aligned characters
  double diagUp   =   UP_MATRIX(i-1, j-1) + matCost; // previous character of one of the sequences was a gap
  double diagLeft = LEFT_MATRIX(i-1, j-1) + matCost; // previous character of one of the sequences was a gap
  
  //if( i == maxRow &&
  //    j == maxCol){
  //  /* discourage singletons on the end of sequences */
  //  diagUp += gapOpen;
  //  diagLeft += gapOpen;
  //}

  /* determine the maximum value */
  if( diagM > diagUp){
    if( diagM > diagLeft){
      M_MATRIX(i, j) = diagM;
      GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
      GAPLEFTDIST(i, j) = GAPLEFTDIST(i-1, j-1) + 1;
    }else{
      M_MATRIX(i, j) = diagLeft;
      GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
      GAPLEFTDIST(i, j) = 1; 
    }
  }else{
    if( diagUp > diagLeft){
      M_MATRIX(i, j) = diagUp;
      GAPUPDIST(i, j)   = 1; 
      GAPLEFTDIST(i, j) = GAPLEFTDIST(i-1, j-1) + 1;
    }else{
      M_MATRIX(i, j) = diagLeft;
      GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
      GAPLEFTDIST(i, j) = 1; 
    }
  }
}


/**
 * Set the traceback values for cell (i,j) (for pairwise--does not set the actual traceback pointers, just sets the scores).
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 */
void QAlign::setTracebackPairwise(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost){

  /*
   * M_MATRIX
   */

  setTracebackDiagPairwise(i, j, matCost);

  /*
   * UP_MATRIX (Ix)
   */

  double gapOpenPath =  M_MATRIX(i-1, j) + gapOpenPenalty( GAPUPDIST(i-1, j)); // open a new gap
  double gapExtPath  = UP_MATRIX(i-1, j) + gapExt; // extend an existing gap

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
  
  gapOpenPath =    M_MATRIX(i, j-1) + gapOpenPenalty( GAPLEFTDIST(i, j-1)); // open a new gap
  gapExtPath  = LEFT_MATRIX(i, j-1) + gapExt;  // extend an existing gap

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
  
} /* END setTracebackPairwise() */


/**
 * Set the traceback values for cell (i,j) for local alignment case (just for the diagonal case).
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 */
void QAlign::setLocalDPDiag(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost){
  
  double diagM    =    M_MATRIX(i-1, j-1) + matCost;
  double diagUp   =   UP_MATRIX(i-1, j-1) + matCost;
  double diagLeft = LEFT_MATRIX(i-1, j-1) + matCost;

  //if( i == maxRow &&
  //    j == maxCol){
  //  /* discourage singletons on the end of sequences */
  //  diagUp += gapOpen;
  //  diagLeft += gapOpen;
  //}
  
  /* determine the maximum value */
  if( diagM > diagUp){
    if( diagM > diagLeft){
      if( diagM > 0.0){
	M_MATRIX(i, j) = diagM;
	GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
	GAPLEFTDIST(i, j) = GAPLEFTDIST(i-1, j-1) + 1;
      }else{
	M_MATRIX(i, j) = 0.0;
	GAPUPDIST(i, j)   = mGapDistanceSetting;
	GAPLEFTDIST(i, j) = mGapDistanceSetting;
      }
    }else{
      if( diagLeft > 0.0){
	M_MATRIX(i, j) = diagLeft;
	GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
	GAPLEFTDIST(i, j) = 1;
      }else{
	M_MATRIX(i, j) = 0.0;
	GAPUPDIST(i, j)   = mGapDistanceSetting;
	GAPLEFTDIST(i, j) = mGapDistanceSetting;
      }
    }
  }else{
    if( diagUp > diagLeft){
      if( diagUp > 0.0){
	M_MATRIX(i, j) = diagUp;
	GAPUPDIST(i, j)   = 1; 
	GAPLEFTDIST(i, j) = GAPLEFTDIST(i-1, j-1) + 1;
      }else{
	M_MATRIX(i, j) = 0.0;
	GAPUPDIST(i, j)   = mGapDistanceSetting;
	GAPLEFTDIST(i, j) = mGapDistanceSetting;
      }
    }else{
      if( diagLeft > 0.0){
	M_MATRIX(i, j) = diagLeft;
	GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
	GAPLEFTDIST(i, j) = 1;
      }else{
	M_MATRIX(i, j) = 0.0;
	GAPUPDIST(i, j)   = mGapDistanceSetting;
	GAPLEFTDIST(i, j) = mGapDistanceSetting;
      }
    }
  }
  
} /* END setLocalDPDiag() */

/**
 * Set the traceback values for cell (i,j) for local alignment case
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 */
void QAlign::setLocalDPMatrics(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost){

  /*
   * M_MATRIX
   */

  setLocalDPDiag( i, j, matCost);
  
  /*
   * UP_MATRIX (Ix)
   */

  double gapOpenPath =  M_MATRIX(i-1, j) + gapOpenPenalty( GAPUPDIST(i-1, j)); // open a new gap
  double gapExtPath  = UP_MATRIX(i-1, j) + gapExt; // extend an existing gap

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
  
  gapOpenPath =    M_MATRIX(i, j-1) + gapOpenPenalty( GAPLEFTDIST(i, j-1)); // open a new gap
  gapExtPath  = LEFT_MATRIX(i, j-1) + gapExt; // extend an existing gap

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
 */
void QAlign::setTracebackLastRowPairwise(int i, int j, double matCost){

  /*
   * M_MATRIX
   */

  setTracebackDiagPairwise(i, j, matCost);
  
  /*
   * UP_MATRIX
   */

  double gapOpenPath =  M_MATRIX(i-1, j) + gapOpenPenalty( GAPUPDIST(i-1, j));
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
  
} /* END setTracebackLastRowPairwise() */


/**
 * Set the traceback values just for the last column (pairwise case only)
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 */
void QAlign::setTracebackLastColPairwise(int i, int j, double matCost){

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
  
  gapOpenPath =    M_MATRIX(i, j-1) + gapOpenPenalty( GAPLEFTDIST(i, j-1));
  gapExtPath  = LEFT_MATRIX(i, j-1) + gapExt;

  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
  }
  
} /* END setTracebackLastColPairwise() */


/**
 * Set the traceback values just for the bottom right cell of the matrix (pairwise case only)
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 */
void QAlign::setTracebackBottomRightPairwise(int i, int j, double matCost){

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
  
  gapOpenPath =    M_MATRIX(i, j-1);
  gapExtPath  = LEFT_MATRIX(i, j-1);

  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
  }
  
} /* END setTracebackBottomRightPairwise() */


/**
 * Set the traceback values for the diagional case (no gaps) for cell (i,j) of the traceback matrix.
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 */
void QAlign::setTracebackDiag(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost){

  if( mLocalWeight > 0.0){
    /* take in account local alignments */
    matCost += LOCAL_MATRIX(i, j);
  }
  
  double diagM    =    M_MATRIX(i-1, j-1) + matCost;
  double diagUp   =   UP_MATRIX(i-1, j-1) + matCost; /* closing a gap */
  double diagLeft = LEFT_MATRIX(i-1, j-1) + matCost; /* closing a gap */

  //if( i == maxRow &&
  //    j == maxCol){
  //  /* discourage singletons on the end of sequences */
  //  diagUp += gapOpen;
  //  diagLeft += gapOpen;
  //}

  /* determine the maximum value */
  if( diagM > diagUp){
    if( diagM > diagLeft){
      M_MATRIX(i, j) = diagM;
      M_TRACEBACK(i, j).next = &(M_TRACEBACK( i-1, j-1));
      GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
      GAPLEFTDIST(i, j) = GAPLEFTDIST(i-1, j-1) + 1;
    }else{
      M_MATRIX(i, j) = diagLeft;
      M_TRACEBACK(i, j).next = &(LEFT_TRACEBACK( i-1, j-1));
      GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
      GAPLEFTDIST(i, j) = 1; 
    }
  }else{
    if( diagUp > diagLeft){
      M_MATRIX(i, j) = diagUp;
      M_TRACEBACK(i, j).next = &(UP_TRACEBACK( i-1, j-1));
      GAPUPDIST(i, j)   = 1; 
      GAPLEFTDIST(i, j) = GAPLEFTDIST(i-1, j-1) + 1;
    }else{
      M_MATRIX(i, j) = diagLeft;
      M_TRACEBACK(i, j).next = &(LEFT_TRACEBACK( i-1, j-1));
      GAPUPDIST(i, j)   = GAPUPDIST(i-1, j-1) + 1;
      GAPLEFTDIST(i, j) = 1; 
    }
  }
} /* END setTracebackDiag() */


/**
 * Set the traceback values for cell (i,j).
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param percentNotGapsLeft the percentage of characters for the left sequences that are not gaps.  Note: only present when \a ADVANCED_PROGRESSIVE_GAP_PENALTIES is defined.
 * \param percentNotGapsUp the percentage of characters for the up sequence that are not gaps.  Note: only present when \a ADVANCED_PROGRESSIVE_GAP_PENALTIES is defined.
 */
void QAlign::setTraceback(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost
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
							  GAPUPDIST(i-1, j));
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
						     GAPLEFTDIST(i, j-1));
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
} /* END setTraceback() */


/**
 * Set the traceback values just for the last row.
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param percentNotGapsLeft the percentage of characters for the left sequences that are not gaps.  Note: only present when \a ADVANCED_PROGRESSIVE_GAP_PENALTIES is defined.
*/
void QAlign::setTracebackLastRow(int i, int j, double matCost
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
							  GAPUPDIST(i-1, j));
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
} /* END setTracebackLastRow() */


/**
 * Set the traceback values just for the last column.
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 * \param percentNotGapsUp the percentage of characters for the up sequence that are not gaps.  Note: only present when \a ADVANCED_PROGRESSIVE_GAP_PENALTIES is defined.
 */
void QAlign::setTracebackLastCol(int i, int j, double matCost
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
						     GAPLEFTDIST(i, j-1));
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
} /* END setTracebackLastCol() */


/**
 * Set the traceback values just for the bottom right value (doesn't account for gap open penalties).
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \param matCost match score.
 */
void QAlign::setTracebackBottomRight(int i, int j, double matCost){

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
  
  gapOpenPath =    M_MATRIX(i, j-1);
  gapExtPath  = LEFT_MATRIX(i, j-1);

  if( gapOpenPath > gapExtPath){
    LEFT_MATRIX(i, j) = gapOpenPath;
    LEFT_TRACEBACK(i, j).next = &(M_TRACEBACK(i, j-1));
  }else{
    LEFT_MATRIX(i, j) = gapExtPath;
    LEFT_TRACEBACK(i, j).next = &(LEFT_TRACEBACK(i, j-1));
  }
} /* END setTracebackBottomRight() */


/**
 * Returns the traceback for the bottom right cell.
 * \param i row index of traceback matrix.
 * \param j column index of traceback matrix.
 * \return a pointer to the \a Traceback object at the bottom right cell.  This position corresponds with the matrix fill alignment score.
 */
Traceback* QAlign::getBottomRightTraceBack(int i, int j){

  if( M_MATRIX( i, j) > UP_MATRIX(i, j)){
    if(  M_MATRIX( i, j) > LEFT_MATRIX(i, j)){
      return &( M_TRACEBACK(i, j));
    }else{
      return &( LEFT_TRACEBACK(i, j));
    }
  }else{
    if( UP_MATRIX( i, j) > LEFT_MATRIX(i, j)){
      return &( UP_TRACEBACK(i, j));
    }else{
      return &( LEFT_TRACEBACK(i, j));
    }
  }
} /* END getBottomRightTraceBack() */

/**
 * Align the dataset by recursively traversing starting from root.
 * Uses an unoptimized (stock) progressive multiple sequence alignment.
 * Compares every character of one sequence with every character of another.
 * \param root is the starting point for further recursion.
 * \param dataset one or more sequences to be aligned.
 * \sa classicProgressiveAlign()
 */
void QAlign::classicPMSA(QNode *root, Dataset *dataset)
{
    if (root->nodeInfo()->leaf())
    {
      setupLeafDataset(root, dataset); 
        //printf("Leaf %d my size is %d\n", root->nodeInfo()->nodeIndex(), root->nodeInfo()->mMultChar->size());
    }
    else
    {
      classicPMSA(root->child1(), dataset);
      classicPMSA(root->child2(), dataset);
      classicProgressiveAlign(root);
    }
} /* END classicPMSA() */

/**
 * Make a dataset (with a single sequence) for the leaf node root.
 * \param root is pointer to the leaf node containing a sequence.
 * \param dataset one or more sequences to be aligned.
 */
void QAlign::setupLeafDataset(QNode *root, Dataset *dataset)
{
  // copy single sequence to a Dataset

  assert( root);
  assert( root->nodeInfo());
  if( root->nodeInfo()->mDataset){
    fprintf(stderr, "\nHY: deleting mDataset in a nodeInfo - why?\n\n");
    delete root->nodeInfo()->mDataset;
  }
    
  root->nodeInfo()->mDataset = new Dataset();
  root->nodeInfo()->mDataset->datatype() = dataset->datatype();
  root->nodeInfo()->mDataset->dataformat() = Dataset::ALIGNED_DATAFORMAT;
  root->nodeInfo()->mDataset->gapchar() = dataset->gapchar();
  root->nodeInfo()->mDataset->missingchar() = dataset->missingchar();
  root->nodeInfo()->mDataset->matchchar() = dataset->matchchar();
  root->nodeInfo()->mDataset->addName( dataset->getTaxonName( root->nodeInfo()->nodeIndex()));
  root->nodeInfo()->mDataset->addCharacters( dataset->getCharacters( root->nodeInfo()->nodeIndex(),false ));

  if( PSODA_DEBUG){
    fprintf(stderr, "setupLeafDataset(%d): (ntaxa: %d, nchars: %d)\n",
	    root->nodeInfo()->nodeIndex(),
	    root->nodeInfo()->mDataset->ntaxa(),
	    root->nodeInfo()->mDataset->nchars() );
    root->nodeInfo()->mDataset->printSeqs( "phylip", "stderr");
  }
  
} /* END setupLeafDataset() */


/**
 * Align each of the children of root, comparing each character in the two sequences.
 * \param root pointer to the leaf node containing a sequence.
 */
void QAlign::classicProgressiveAlign(QNode *root)
{
  //fprintf(stderr,"Progressively aligning %d and %d\n", root->child1()->nodeInfo()->nodeIndex(), root->child2()->nodeInfo()->nodeIndex());
  if( PSODA_VERBOSE){ fprintf(stderr, "%s", "."); }

  int i,j;
  Dataset* a = root->child1()->nodeInfo()->mDataset;
  Dataset* b = root->child2()->nodeInfo()->mDataset;
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
      matcost = progressiveMatCost( i-1, a, j-1, b);
      //if( i == maxA ||
      //	  j == maxB){
      //	/* prevents singleton characters at the end of the alignment */	    
      //	matcost -= 0.1;
      //}
      setTraceback( i, j, matcost);
    }
  }

  if( penalizeLeadingGaps == false){
    /* allow for free gaps along the last row and column */

    i = sizea;
    for(j = 1; j <= maxB; j++){
      //fprintf(stderr, "Working on (%d,%d)", i, j);
      matcost = progressiveMatCost( i-1, a, j-1, b);
      setTracebackLastRow( i, j, matcost);
    }
    
    j = sizeb;
    for(i = 1; i <= maxA; i++){
      //fprintf(stderr, "Working on (%d,%d)", i, j);
      matcost = progressiveMatCost( i-1, a, j-1, b);
      setTracebackLastCol( i, j, matcost);
    }
    
    i = sizea;
    j = sizeb;
    //fprintf(stderr, "Working on (%d,%d)", i, j);
    matcost = progressiveMatCost( i-1, a, j-1, b);
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
  Traceback *temp = getBottomRightTraceBack( sizea, sizeb);

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
      }
      for(bIndex = 0; bIndex < b->ntaxa(); bIndex++){
	ab->appendACharacter( b->getCharacter(bIndex, j), k - kOffset, l++);
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
} /* END classicProgressiveAlign() */

/**
 * Calculate local alignments for sequences (stored in datasets) \a a & \a b.
 * \param a \a Dataset of one or more sequences forming a sub-alignment.
 * \param b \a Dataset of one or more sequences forming a sub-alignment.
 */
void QAlign::classicLocalAlign(Dataset* a, Dataset* b)
{
  //fprintf(stderr,"Progressively aligning %d and %d\n", root->child1()->nodeInfo()->nodeIndex(), root->child2()->nodeInfo()->nodeIndex());
  //fprintf(stderr, "%s", ".");

  int i,j;
  int sizea = a->nchars();
  int sizeb = b->nchars();
  double matcost;

  initDPMatrices( sizea, sizeb);

  int maxA = sizea;
  int maxB = sizeb;


  if( PSODA_DEBUG){
    fprintf(stderr, "\nSequences To (local) Align: (a: ntaxa: %d, nchars: %d; b: ntaxa: %d, nchars: %d)\n",
	    a->ntaxa(),
	    a->nchars(),
	    b->ntaxa(),
	    b->nchars() );
    fprintf(stderr,"a:\n");
    a->printSeqs( "phylip", "stderr");
    fprintf(stderr,"b:\n");
    b->printSeqs( "phylip", "stderr");
  }

  /* iterate over every character in sequence a */ 
  for(i = 1; i <= maxA; i++){
    /* iterate over every character in sequence b */ 
    for(j = 1; j <= maxB; j++){
      //fprintf(stderr, "Working on (%d,%d)", i, j);
      matcost = progressiveMatCost( i-1, a, j-1, b);
      setLocalDPMatrics( i, j, matcost);
    }
  }
	
  if( PSODA_DEBUG){
    //PsodaPrinter::getInstance()->write("classicProgressiveAlign score %f\n", DPMATRIX(sizea, sizeb));
    if( PSODA_DEBUG > 9){
      printDPMatrices(a, b);
    }
  }
} /* END classicLocalAlign() */


/**
 * Calculate the match cost for column \acolA of datasets \a a and column \a colB of dataset \a b.
 * \param colA column index for dataset \a a.
 * \param a dataset with one or more sequences.
 * \param colB column index for dataset \a b.
 * \param B dataset with one or more sequences.
 * \return match score for column \acolA of datasets \a a and column \a colB of dataset \a b.
 */
double QAlign::progressiveMatCost(int colA, Dataset* a, int colB, Dataset* b){
  double matCost = 0.0;

  int taxaA = a->ntaxa();
  int taxaB = b->ntaxa();

  /* iterate over every taxon in a */
  for(int aIndex = 0; aIndex < taxaA; aIndex++){
    char charA = a->getCharacter(aIndex, colA);
    /* iterate over every taxon in b */
    for(int bIndex = 0; bIndex < taxaB; bIndex++){
      matCost += subMat->get(charA, b->getCharacter(bIndex, colB));
    }
  }
  
  /* normalize */
  matCost /= taxaA * taxaB;

  //fprintf( stderr, "mC%lf\n", matCost);
  
  return matCost;
} /* END progressiveMatCost() */



/**
 * Set the values for the dynamic programming (DP) matrices via setTracebackPairwise() and similar functions.
 * \param a genetic sequence.
 * \param sizea number of characters in \a a.
 * \param b genetic sequence.
 * \param sizeb number of characters in \a b.
 * \return the alignment score.
 * \sa setTracebackPairwise()
 */
double QAlign::calculateDPMatrices(const char *a, int sizea, const char *b, int sizeb)
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
	  //matCost = subMat->get(a[i-1], b[j-1]);
	  //if( i == maxA ||
	  //    j == maxB){
	  //  /* prevents singleton characters at the end of the alignment */	    
	  //  matCost -= 0.1;
	  //}
	  setTracebackPairwise(i, j, subMat->get(a[i-1], b[j-1]));
	}
    }

	
  if( penalizeLeadingGaps == false){
    /* allow for free gaps along the last row and column */

    i = sizea;
    for(j = 1; j <= maxB; j++){
      setTracebackLastRowPairwise(i, j, subMat->get(a[i-1], b[j-1]));
    }
    
    j = sizeb;
    for(i = 1; i <= maxA; i++){
      setTracebackLastColPairwise(i, j, subMat->get(a[i-1], b[j-1]));
    }
	  
    i = sizea;
    j = sizeb; 
   setTracebackBottomRightPairwise(i, j, subMat->get(a[i-1], b[j-1]));
  }

  //if( PSODA_DEBUG > 9){ printDPMatrices( a, b); }
  
  return min3( M_MATRIX(sizea, sizeb), UP_MATRIX(sizea, sizeb), LEFT_MATRIX(sizea, sizeb));
}

// float QAlign::max3 (float a, float b, float c)
// {
// 	return max(max(a,b),c);
// }

/**
 * Post-order traversal of \a root
 * \param root pointer to the leaf node containing a sequence.
 * \param introstring debugging string.
 */
void QAlign::postorder(QNode *root, char *introstring)
{
    if (root->nodeInfo()->leaf())
        printf("%sChild Leaf %d\n", introstring, root->nodeInfo()->nodeIndex());
    else
    {
        postorder(root->child1(), introstring);
        postorder(root->child2(), introstring);
    }
}

/**
 * Calculates the Hamming distance between every pair of sequences in \a dataset.
 * Sets \a distMatrix with the distances.
 * \param dataset contains the sequences.
 * \sa calculateDPMatrices.
 */
void QAlign::calculateDistMatrix(Dataset *dataset)
{

  // allocate distMatrix memory for two dimensions
  distMatrix = new double*[ntaxa];
  for (int i = 0; i < ntaxa; ++i)
  {
    distMatrix[i] = new double[ntaxa];
  }
  
  double score;

  PsodaPrinter::getInstance()->write("%s", "Pairwise Alignment: "); 

  /* for each sequence */
	int dotiteration = 0;
  for (int i = 0; i < ntaxa; ++i)
  {
    distMatrix[i][i] = 0;
    
    PsodaPrinter::getInstance()->write( "%s", "."); 
		if((dotiteration++%80) == 79) {
			PsodaPrinter::getInstance()->write("\n");
		}
    /* with every other sequence */
    for (int j = i + 1; j < ntaxa; ++j)
    {
      checkPausePSODA();
      score = calculateDPMatrices(dataset->getCharacters(i,false), dataset->getSeqLen(i), dataset->getCharacters(j,false), dataset->getSeqLen(j));
      //fprintf(stderr,"DP Matrix for %d and %d\n", i, j);
      //printDPMatrices(dataset->getCharacters(i), dataset->getCharacters(j));

      distMatrix[j][i] = distMatrix[i][j] = score;
    }
  }
  PsodaPrinter::getInstance()->write("%s", "."); 
  
}

/**
 * MIN_INCREMENT is the threshold for the minumum amount for increasing the memory for the matrices
 */
#define MIN_INCREMENT    50

/**
 * Alloc and populate the static values for the Dynamic Programming matrices, if necessary.
 * The following main matrices are affected: \a mMatrix, \a upMatrix, \a leftMatrix, \a mTraceback, \a upTraceback, \a leftTraceback, \a upGapDist and \a leftGapDist.
 * Memory is increased for all of the matrices if the dimensions \a dpRowsRequested and \a dpColsRequested are larger than the variables \a dpRowsAlloced and \a dpColsAlloced.
 * The first row and column are statically set--no routines should modify them.
 * Therefore, the memory does not need to alloced each time and can be reused.
 * \param dpRowsRequested number of rows needed.
 * \param dpColsRequested number of columns needed.
 */
void QAlign::initDPMatrices( int dpRowsRequested, int dpColsRequested)
{

  if( dpRowsRequested + 1 < dpRowsAlloced &&
      dpColsRequested + 1 < dpColsAlloced){
    /* already have enough space */
    return;
  }

  freeDPMatrices();
  
  int rowsToAlloc = dpRowsAlloced; 
  int colsToAlloc = dpColsAlloced; 

  if( dpRowsAlloced == 0){
    /* first time*/
    rowsToAlloc = dpRowsRequested + 1;
  }else if( dpRowsRequested + 1 > dpRowsAlloced){
    rowsToAlloc = max( dpRowsRequested + 1, dpRowsAlloced + MIN_INCREMENT);
  }
  
  if( dpColsAlloced == 0){
    /* first time*/
    colsToAlloc = dpColsRequested + 1;
  }else if( dpColsRequested + 1 > dpColsAlloced){
    colsToAlloc = max( dpColsRequested + 1, dpColsAlloced + MIN_INCREMENT);
  }

  /* alloc new matricies */
  mMatrix       = (double*)    check_calloc( rowsToAlloc * colsToAlloc, sizeof( double));
  upMatrix      = (double*)    check_calloc( rowsToAlloc * colsToAlloc, sizeof( double));
  leftMatrix    = (double*)    check_calloc( rowsToAlloc * colsToAlloc, sizeof( double));
  mTraceback    = (Traceback*) check_calloc( rowsToAlloc * colsToAlloc, sizeof( struct Traceback));
  upTraceback   = (Traceback*) check_calloc( rowsToAlloc * colsToAlloc, sizeof( struct Traceback));
  leftTraceback = (Traceback*) check_calloc( rowsToAlloc * colsToAlloc, sizeof( struct Traceback));
  upGapDist     = (int*)       check_calloc( rowsToAlloc * colsToAlloc, sizeof( int));
  leftGapDist   = (int*)       check_calloc( rowsToAlloc * colsToAlloc, sizeof( int));
  
  if( mLocalWeight > 0.0){
    upLocalMatrix   = (double*)    check_calloc( rowsToAlloc * colsToAlloc, sizeof( double));
    leftLocalMatrix = (double*)    check_calloc( rowsToAlloc * colsToAlloc, sizeof( double));
    localMatrix     = (double*)    check_calloc( rowsToAlloc * colsToAlloc, sizeof( double));
  }

  /* complete transistion to new matricies */
  dpRowsAlloced = rowsToAlloc;
  dpColsAlloced = colsToAlloc;


  /* NOTE: NO initialization of the matrices beyond the first row and column */
  
  /* init first row & column */
  int row = 0;
  int col = 0;
  
  /* set traceback matrices */
  for(row = 0; row < rowsToAlloc; row++){
    for(col = 0; col < colsToAlloc; col++){
      M_TRACEBACK(row, col).direction = Diag;
      UP_TRACEBACK(row, col).direction = Up;
      LEFT_TRACEBACK(row, col).direction = Left;
    }
  }
  
  M_TRACEBACK(0, 0).direction = None;
  M_TRACEBACK(0, 0).next = NULL;
  
  UP_TRACEBACK(0, 0).direction = None;
  UP_TRACEBACK(0, 0).next = NULL;
  
  LEFT_TRACEBACK(0, 0).direction = None;
  LEFT_TRACEBACK(0, 0).next = NULL;

  M_MATRIX(0, 0) = 0;
  UP_MATRIX(0, 0) = NEG_INFINITY;
  LEFT_MATRIX(0, 0) = NEG_INFINITY;
  
    GAPUPDIST(0, 0) = mGapDistanceSetting; /* set it to something so that gapOpenPenalty() works */
  GAPLEFTDIST(0, 0) = mGapDistanceSetting; /* set it to something so that gapOpenPenalty() works */

  if( mLocalWeight > 0.0){
    LEFT_LOCAL_MATRIX(0, 0) = 0.0;
      UP_LOCAL_MATRIX(0, 0) = 0.0;
         LOCAL_MATRIX(0, 0) = 0.0;
  }
  
  /* initialize first row and col of matrices values */
  /* first row */
  row = 0;  
  for(col = 1; col < colsToAlloc; col++)
  {
    M_MATRIX(row, col) = NEG_INFINITY;
    UP_MATRIX(row, col) = NEG_INFINITY;
    
    if( penalizeLeadingGaps == true){
      LEFT_MATRIX(row, col) = gapOpen + (col - 1) * gapExt;
      /* discourages singleton characters at the beginnging of the alignment (when there's another traceback with the same score)*/
      //LEFT_MATRIX(row, col) = gapOpen + (col - 1) * gapExt + 0.1;
      GAPUPDIST(row, col) = 0;
      GAPLEFTDIST(row, col) = 0;
    }else{
      LEFT_MATRIX(row, col) = 0.0;
      GAPUPDIST(row, col) = mGapDistanceSetting; /* set it to something so that gapOpenPenalty() works */
      GAPLEFTDIST(row, col) = mGapDistanceSetting; /* set it to something so that gapOpenPenalty() works */
    }

    /* incorporate local alignment weights */
    if( mLocalWeight > 0.0){
      LEFT_LOCAL_MATRIX(row, col) = 0.0;
      LOCAL_MATRIX(row, col) = 0.0;
    }
    
    LEFT_TRACEBACK(row, col).next = &LEFT_TRACEBACK(row, col-1);
  }

  /* first column */
  col = 0;
  for(row = 1; row < rowsToAlloc; row++)
  {
    M_MATRIX(row, col) = NEG_INFINITY;
    LEFT_MATRIX(row, col) = NEG_INFINITY;
    
    if( penalizeLeadingGaps == true){
      UP_MATRIX(row, col) = gapOpen + (row - 1) * gapExt;
      /* discourages singleton characters at the beginnging of the alignment (when there's another traceback with the same score)*/
      //UP_MATRIX(row, col) = gapOpen + (row - 1) * gapExt + 0.1;
      GAPUPDIST(row, col) = 0;
      GAPLEFTDIST(row, col) = 0;
    }else{
      UP_MATRIX(row, col) = 0.0;
      GAPUPDIST(row, col) = mGapDistanceSetting; /* set it to something so that gapOpenPenalty() works */
      GAPLEFTDIST(row, col) = mGapDistanceSetting; /* set it to something so that gapOpenPenalty() works */
    }
    if( mLocalWeight > 0.0){
      UP_LOCAL_MATRIX(row, col) = 0.0;
      LOCAL_MATRIX(row, col) = 0.0;
    }
    
    UP_TRACEBACK(row, col).next = &UP_TRACEBACK(row-1, col);
  }
} /* END initDPMatrices() */


/**
 * Deallocate Dynamic Programming matrices: \a mMatrix, \a upMatrix, \a leftMatrix, \a mTraceback, \a upTraceback, \a leftTraceback, \a upGapDist and \aleftGapDist (and more for local alignment weights).
 */
void QAlign::freeDPMatrices(){
  /* delete old matricies */
  if( dpRowsAlloced * dpColsAlloced > 0){
    free( mMatrix);
    free( upMatrix);
    free( leftMatrix);
    free( mTraceback);
    free( upTraceback);
    free( leftTraceback);
    free( upGapDist);
    free( leftGapDist);

    if( mLocalWeight > 0.0){
      free( upLocalMatrix);
      free( leftLocalMatrix);
      free( localMatrix);
    }
  }
}


/**
 * Write the \a distMatrix matrix to STDOUT.
 */
void QAlign::printMatrix()
{
  if( distMatrix == NULL){
    return;
  }
  fprintf(stderr, "\n");
  for (int i = 0; i < ntaxa; ++i)
  {
    for (int j = 0; j < ntaxa; ++j)
    {
      fprintf( stderr, "%15.0f\t",distMatrix[i][j]);
    }
    fprintf( stderr, "\n");
  }
}

/**
 * Write the \a M_MATRIX, \a UP_MATRIX and LEFT_MATRIX matrics to STDOUT (and more for local alignment weights).
 * \param sizea first dimension of the matrices.
 * \param sizeb first dimension of the matrices.
 */
void QAlign::printDPMatrices(int sizea, int sizeb)
{
  fprintf(stderr, "mMatrix:\n");
  for (int i = 0; i < sizea + 1 && i < dpRowsAlloced; ++i)
  {
    for (int j = 0; j < sizeb + 1 && j < dpColsAlloced; ++j)
    {
      fprintf(stderr,"%15.0f\t",M_MATRIX(i, j));
    }
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "upMatrix:\n");
  for (int i = 0; i < sizea + 1 && i < dpRowsAlloced; ++i)
  {
    for (int j = 0; j < sizeb + 1 && j < dpColsAlloced; ++j)
    {
      fprintf(stderr,"%15.0f\t",UP_MATRIX(i, j));
    }
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "leftMatrix:\n");
  for (int i = 0; i < sizea + 1 && i < dpRowsAlloced; ++i)
  {
    for (int j = 0; j < sizeb + 1 && j < dpColsAlloced; ++j)
    {
      fprintf(stderr,"%15.0f\t",LEFT_MATRIX(i, j));
    }
    fprintf(stderr, "\n");
  }

  if( mLocalWeight > 0.0){
    fprintf(stderr, "upLocalMatrix:\n");
    for (int i = 0; i < sizea + 1 && i < dpRowsAlloced; ++i)
      {
	for (int j = 0; j < sizeb + 1 && j < dpColsAlloced; ++j)
	  {
	    fprintf(stderr,"%15.0f\t",UP_LOCAL_MATRIX(i, j));
	  }
	fprintf(stderr, "\n");
      }

    fprintf(stderr, "leftLocalMatrix:\n");
    for (int i = 0; i < sizea + 1 && i < dpRowsAlloced; ++i)
      {
	for (int j = 0; j < sizeb + 1 && j < dpColsAlloced; ++j)
	  {
	    fprintf(stderr,"%15.0f\t",LEFT_LOCAL_MATRIX(i, j));
	  }
	fprintf(stderr, "\n");
      }

    fprintf(stderr, "localmatrix:\n");
    for (int i = 0; i < sizea + 1 && i < dpRowsAlloced; ++i)
      {
	for (int j = 0; j < sizeb + 1 && j < dpColsAlloced; ++j)
	  {
	    fprintf(stderr,"%15.0f\t",LOCAL_MATRIX(i, j));
	  }
	fprintf(stderr, "\n");
      }
  }
}

/**
 * Write the \a M_MATRIX, \a UP_MATRIX and LEFT_MATRIX matrics to STDOUT with the sequences \a a and \a b (and more for local alignment weights).
 * \param a sequence of chacters.
 * \param b sequence of chacters.
 */
void QAlign::printDPMatrices(const char *a, const char *b)
{
  Dataset* dataset = Interpreter::getInstance()->dataset();
  int lenA = dataset->getLen( a);
  int lenB = dataset->getLen( b);
  
  fprintf(stderr, "mMatrix:\n");
  fprintf( stderr,"%15s\t\t", "");
  for (int j = 0; j < lenB && j < dpColsAlloced; ++j)
    {
      fprintf( stderr,"\t%15c", b[j]);
    }
  fprintf( stderr,"%s", "\n");

  for (int i = 0; i < lenA + 1 && i < dpRowsAlloced; ++i)
    {
      if( i == 0){
	fprintf( stderr, "%15s", "");
      }else{
	fprintf( stderr,"%15c", a[i - 1]);
      }
      for (int j = 0; j < lenB + 1 && j < dpColsAlloced; ++j)
	{
	  fprintf(stderr,"\t%15.0f", M_MATRIX(i, j));
	}
      fprintf(stderr, "\n");
    }
  
  fprintf(stderr, "upMatrix:\n");
  fprintf( stderr,"%15s\t\t", "");
  for (int j = 0; j < lenB && j < dpColsAlloced; ++j)
    {
      fprintf( stderr,"\t%15c", b[j]);
    }
  fprintf( stderr,"%s", "\n");

  for (int i = 0; i < lenA + 1 && i < dpRowsAlloced; ++i)
    {
      if( i == 0){
	fprintf( stderr, "%15s", "");
      }else{
	fprintf( stderr,"%15c", a[i - 1]);
      }
      for (int j = 0; j < lenB + 1 && j < dpColsAlloced; ++j)
	{
	  fprintf(stderr,"\t%15.0f", UP_MATRIX(i, j));
	}
      fprintf(stderr, "\n");
    }
  fprintf(stderr, "leftMatrix:\n");
  fprintf( stderr,"%15s\t\t", "");
  for (int j = 0; j < lenB && j < dpColsAlloced; ++j)
    {
      fprintf( stderr,"\t%15c", b[j]);
    }
  fprintf( stderr,"%s", "\n");

  for (int i = 0; i < lenA + 1 && i < dpRowsAlloced; ++i)
    {
      if( i == 0){
	fprintf( stderr, "%15s", "");
      }else{
	fprintf( stderr,"%15c", a[i - 1]);
      }
      for (int j = 0; j < lenB + 1 && j < dpColsAlloced; ++j)
	{
	  fprintf(stderr,"\t%15.0f", LEFT_MATRIX(i, j));
	}
      fprintf(stderr, "\n");
    }

  if( mLocalWeight > 0.0){
    fprintf(stderr, "upLocalMatrix:\n");
    fprintf( stderr,"%15s\t\t", "");
    for (int j = 0; j < lenB && j < dpColsAlloced; ++j)
      {
	fprintf( stderr,"\t%15c", b[j]);
      }
    fprintf( stderr,"%s", "\n");

    for (int i = 0; i < lenA + 1 && i < dpRowsAlloced; ++i)
      {
	if( i == 0){
	  fprintf( stderr, "%15s", "");
	}else{
	  fprintf( stderr,"%15c", a[i - 1]);
	}
	for (int j = 0; j < lenB + 1 && j < dpColsAlloced; ++j)
	  {
	    fprintf(stderr,"\t%15.0f", UP_LOCAL_MATRIX(i, j));
	  }
	fprintf(stderr, "\n");
      }

    fprintf(stderr, "leftLocalMatrix:\n");
    fprintf( stderr,"%15s\t\t", "");
    for (int j = 0; j < lenB && j < dpColsAlloced; ++j)
      {
	fprintf( stderr,"\t%15c", b[j]);
      }
    fprintf( stderr,"%s", "\n");

    for (int i = 0; i < lenA + 1 && i < dpRowsAlloced; ++i)
      {
	if( i == 0){
	  fprintf( stderr, "%15s", "");
	}else{
	  fprintf( stderr,"%15c", a[i - 1]);
	}
	for (int j = 0; j < lenB + 1 && j < dpColsAlloced; ++j)
	  {
	    fprintf(stderr,"\t%15.0f", LEFT_LOCAL_MATRIX(i, j));
	  }
	fprintf(stderr, "\n");
      }


    fprintf(stderr, "localMatrix:\n");
    fprintf( stderr,"%15s\t\t", "");
    for (int j = 0; j < lenB && j < dpColsAlloced; ++j)
      {
	fprintf( stderr,"\t%15c", b[j]);
      }
    fprintf( stderr,"%s", "\n");

    for (int i = 0; i < lenA + 1 && i < dpRowsAlloced; ++i)
      {
	if( i == 0){
	  fprintf( stderr, "%15s", "");
	}else{
	  fprintf( stderr,"%15c", a[i - 1]);
	}
	for (int j = 0; j < lenB + 1 && j < dpColsAlloced; ++j)
	  {
	    fprintf(stderr,"\t%15.0f", LOCAL_MATRIX(i, j));
	  }
	fprintf(stderr, "\n");
      }
  }
}

/**
 * Write the \a M_MATRIX, \a UP_MATRIX and LEFT_MATRIX matrics to STDOUT with the sequences/sub-alignments \a a and \a b (and more for local alignment weights).
 * \param a sequence/sub-alignment of chacters (as a list of \a MultChars).
 * \param b sequence/sub-alignment of chacters (as a list of \a MultChars).
 */
void QAlign::printDPMatrices( list<MultChar *> *a, list<MultChar *> *b)
{
  list<MultChar *>::iterator a_it, b_it;
  int i = 0;
  int j;
  
  fprintf(stderr, "mMatrix:\n");
  fprintf( stderr,"%s", "\t");
  for( b_it = b->begin(); b_it != b->end(); b_it++){
    fprintf( stderr,"%s", "\t");
    (*b_it)->print();
  }
  fprintf( stderr,"%s", "\n");
  i = 0;
  j = 0;
  fprintf(stderr,"\t%.1f", M_MATRIX(i, j));
  for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++){
    fprintf(stderr,"\t%.1f", M_MATRIX(i, j));
  }
  fprintf( stderr,"%s", "\n");
  
  for( i = 1, a_it = a->begin(); a_it != a->end() && i < dpRowsAlloced; ++i, a_it++)
    {
      (*a_it)->print();
      fprintf(stderr,"\t%.1f", M_MATRIX(i, 0));
      for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++)
	{
	  fprintf(stderr,"\t%.1f", M_MATRIX(i, j));
	}
      fprintf( stderr,"%s", "\n");
    }

  fprintf(stderr, "upMatrix:\n");
  fprintf( stderr,"%s", "\t");
  for( b_it = b->begin(); b_it != b->end(); b_it++){
    fprintf( stderr,"%s", "\t");
    (*b_it)->print();
  }
  fprintf( stderr,"%s", "\n");
  i = 0;
  j = 0;
  fprintf(stderr,"\t%.1f", UP_MATRIX(i, j));
  for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++){
    fprintf(stderr,"\t%.1f", UP_MATRIX(i, j));
  }
  fprintf( stderr,"%s", "\n");
  
  for( i = 1, a_it = a->begin(); a_it != a->end() && i < dpRowsAlloced; ++i, a_it++)
    {
      (*a_it)->print();
      fprintf(stderr,"\t%.1f", UP_MATRIX(i, 0));
      for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++)
	{
	  fprintf(stderr,"\t%.1f", UP_MATRIX(i, j));
	}
      fprintf( stderr,"%s", "\n");
    }
  
  fprintf(stderr, "leftMatrix:\n");
  fprintf( stderr,"%s", "\t");
  for( b_it = b->begin(); b_it != b->end(); b_it++){
    fprintf( stderr,"%s", "\t");
    (*b_it)->print();
  }
  fprintf( stderr,"%s", "\n");
  i = 0;
  j = 0;
  fprintf(stderr,"\t%.1f", LEFT_MATRIX(i, j));
  for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++){
    fprintf(stderr,"\t%.1f", LEFT_MATRIX(i, j));
  }
  fprintf( stderr,"%s", "\n");
  
  for( i = 1, a_it = a->begin(); a_it != a->end() && i < dpRowsAlloced; ++i, a_it++)
    {
      (*a_it)->print();
      fprintf(stderr,"\t%.1f", LEFT_MATRIX(i, 0));
      for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++)
	{
	  fprintf(stderr,"\t%.1f", LEFT_MATRIX(i, j));
	}
      fprintf( stderr,"%s", "\n");
    }

  fprintf(stderr, "upGapDist:\n");
  fprintf( stderr,"%s", "\t");
  for( b_it = b->begin(); b_it != b->end(); b_it++){
    fprintf( stderr,"%s", "\t");
    (*b_it)->print();
  }
  fprintf( stderr,"%s", "\n");
  i = 0;
  j = 0;
  fprintf(stderr,"\t%d", GAPUPDIST(i, j));
  for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++){
    fprintf(stderr,"\t%d", GAPUPDIST(i, j));
  }
  fprintf( stderr,"%s", "\n");
  
  for( i = 1, a_it = a->begin(); a_it != a->end() && i < dpRowsAlloced; ++i, a_it++)
    {
      (*a_it)->print();
      fprintf(stderr,"\t%d", GAPUPDIST(i, 0));
      for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++)
	{
	  fprintf(stderr,"\t%d", GAPUPDIST(i, j));
	}
      fprintf( stderr,"%s", "\n");
    }
  
  fprintf(stderr, "leftGapDist:\n");
  fprintf( stderr,"%s", "\t");
  for( b_it = b->begin(); b_it != b->end(); b_it++){
    fprintf( stderr,"%s", "\t");
    (*b_it)->print();
  }
  fprintf( stderr,"%s", "\n");
  i = 0;
  j = 0;
  fprintf(stderr,"\t%d", GAPLEFTDIST(i, j));
  for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++){
    fprintf(stderr,"\t%d", GAPLEFTDIST(i, j));
  }
  fprintf( stderr,"%s", "\n");
  
  for( i = 1, a_it = a->begin(); a_it != a->end() && i < dpRowsAlloced; ++i, a_it++)
    {
      (*a_it)->print();
      fprintf(stderr,"\t%d", GAPLEFTDIST(i, 0));
      for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++)
	{
	  fprintf(stderr,"\t%d", GAPLEFTDIST(i, j));
	}
      fprintf( stderr,"%s", "\n");
    }
  
  if( mLocalWeight > 0.0){
    fprintf(stderr, "upLocalMatrix:\n");
    fprintf( stderr,"%s", "\t");
    for( b_it = b->begin(); b_it != b->end(); b_it++){
      fprintf( stderr,"%s", "\t");
      (*b_it)->print();
    }
    fprintf( stderr,"%s", "\n");
    i = 0;
    j = 0;
    fprintf(stderr,"\t%.1f", UP_LOCAL_MATRIX(i, j));
    for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++){
      fprintf(stderr,"\t%.1f", UP_LOCAL_MATRIX(i, j));
    }
    fprintf( stderr,"%s", "\n");
  
    for( i = 1, a_it = a->begin(); a_it != a->end() && i < dpRowsAlloced; ++i, a_it++)
      {
	(*a_it)->print();
	fprintf(stderr,"\t%.1f", UP_LOCAL_MATRIX(i, 0));
	for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++)
	  {
	    fprintf(stderr,"\t%.1f", UP_LOCAL_MATRIX(i, j));
	  }
	fprintf( stderr,"%s", "\n");
      }
  
    fprintf(stderr, "leftLocalMatrix:\n");
    fprintf( stderr,"%s", "\t");
    for( b_it = b->begin(); b_it != b->end(); b_it++){
      fprintf( stderr,"%s", "\t");
      (*b_it)->print();
    }
    fprintf( stderr,"%s", "\n");
    i = 0;
    j = 0;
    fprintf(stderr,"\t%.1f", LEFT_LOCAL_MATRIX(i, j));
    for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++){
      fprintf(stderr,"\t%.1f", LEFT_LOCAL_MATRIX(i, j));
    }
    fprintf( stderr,"%s", "\n");
  
    for( i = 1, a_it = a->begin(); a_it != a->end() && i < dpRowsAlloced; ++i, a_it++)
      {
	(*a_it)->print();
	fprintf(stderr,"\t%.1f", LEFT_LOCAL_MATRIX(i, 0));
	for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++)
	  {
	    fprintf(stderr,"\t%.1f", LEFT_LOCAL_MATRIX(i, j));
	  }
	fprintf( stderr,"%s", "\n");
      }
  
    fprintf(stderr, "localMatrix:\n");
    fprintf( stderr,"%s", "\t");
    for( b_it = b->begin(); b_it != b->end(); b_it++){
      fprintf( stderr,"%s", "\t");
      (*b_it)->print();
    }
    fprintf( stderr,"%s", "\n");
    i = 0;
    j = 0;
    fprintf(stderr,"\t%.1f", LOCAL_MATRIX(i, j));
    for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++){
      fprintf(stderr,"\t%.1f", LOCAL_MATRIX(i, j));
    }
    fprintf( stderr,"%s", "\n");
  
    for( i = 1, a_it = a->begin(); a_it != a->end() && i < dpRowsAlloced; ++i, a_it++)
      {
	(*a_it)->print();
	fprintf(stderr,"\t%.1f", LOCAL_MATRIX(i, 0));
	for ( j = 1, b_it = b->begin(); b_it != b->end() && j < dpColsAlloced; ++j, b_it++)
	  {
	    fprintf(stderr,"\t%.1f", LOCAL_MATRIX(i, j));
	  }
	fprintf( stderr,"%s", "\n");
      }
  }
}


/**
 * Write the \a M_MATRIX, \a UP_MATRIX and LEFT_MATRIX matrics to STDOUT with the sequences/sub-alignments \a a and \a b (and more for local alignment weights).
 * \param a sequence/sub-alignment of chacters (stored in a \a Dataset).
 * \param b sequence/sub-alignment of chacters (stored in a \a Dataset).
 */
void QAlign::printDPMatrices( Dataset *a, Dataset *b)
{
  int taxaA = a->ntaxa();
  int taxaB = b->ntaxa();

  int charsA = a->nchars();
  int charsB = b->nchars();

  int rowA, colA;
  int rowB, colB;
  
  
  fprintf(stderr, "mMatrix:\n");
  for( rowB = 0; rowB < taxaB; rowB++){
    for( rowA = 0; rowA < taxaA - 1; rowA++){
      fprintf( stderr,"%s", "\t");
    }
    fprintf( stderr,"%s", b->getTaxonName(rowB));
    for( colB = 0; colB < charsB; colB++){
      fprintf( stderr,"\t%c", b->getCharacter(rowB, colB));
    }
    fprintf( stderr,"%s", "\n");
  }

  
  for( colA = 0; colA <= charsA; colA++){
    for( rowA = 0; rowA < taxaA; rowA++){
      fprintf( stderr,"%c\t", a->getCharacter(rowA, colA));
    }
    fprintf(stderr,"%.1f", 0.0);
    for( colB = 0; colB <= charsB; colB++){
      fprintf(stderr,"\t%.1f", M_MATRIX(colA, colB));
    }
    fprintf( stderr,"%s", "\n");
  }
  
  fprintf(stderr, "upMatrix:\n");
  for( rowB = 0; rowB < taxaB; rowB++){
    for( rowA = 0; rowA < taxaA - 1; rowA++){
      fprintf( stderr,"%s", "\t");
    }
    fprintf( stderr,"%s", b->getTaxonName(rowB));
    for( colB = 0; colB < charsB; colB++){
      fprintf( stderr,"\t%c", b->getCharacter(rowB, colB));
    }
    fprintf( stderr,"%s", "\n");
  }

  
  for( colA = 0; colA <= charsA; colA++){
    for( rowA = 0; rowA < taxaA; rowA++){
      fprintf( stderr,"%c\t", a->getCharacter(rowA, colA));
    }
    fprintf(stderr,"%.1f", 0.0);
    for( colB = 0; colB <= charsB; colB++){
      fprintf(stderr,"\t%.1f", UP_MATRIX(colA, colB));
    }
    fprintf( stderr,"%s", "\n");
  }
  
  fprintf(stderr, "leftMatrix:\n");
  for( rowB = 0; rowB < taxaB; rowB++){
    for( rowA = 0; rowA < taxaA - 1; rowA++){
      fprintf( stderr,"%s", "\t");
    }
    fprintf( stderr,"%s", b->getTaxonName(rowB));
    for( colB = 0; colB < charsB; colB++){
      fprintf( stderr,"\t%c", b->getCharacter(rowB, colB));
    }
    fprintf( stderr,"%s", "\n");
  }

  
  for( colA = 0; colA <= charsA; colA++){
    for( rowA = 0; rowA < taxaA; rowA++){
      fprintf( stderr,"%c\t", a->getCharacter(rowA, colA));
    }
    fprintf(stderr,"%.1f", 0.0);
    for( colB = 0; colB <= charsB; colB++){
      fprintf(stderr,"\t%.1f", LEFT_MATRIX(colA, colB));
    }
    fprintf( stderr,"%s", "\n");
  }

  if( mLocalWeight > 0.0){
    fprintf(stderr, "upLocalMatrix:\n");
    for( rowB = 0; rowB < taxaB; rowB++){
      for( rowA = 0; rowA < taxaA - 1; rowA++){
	fprintf( stderr,"%s", "\t");
      }
      fprintf( stderr,"%s", b->getTaxonName(rowB));
      for( colB = 0; colB < charsB; colB++){
	fprintf( stderr,"\t%c", b->getCharacter(rowB, colB));
      }
      fprintf( stderr,"%s", "\n");
    }

  
    for( colA = 0; colA <= charsA; colA++){
      for( rowA = 0; rowA < taxaA; rowA++){
	fprintf( stderr,"%c\t", a->getCharacter(rowA, colA));
      }
      fprintf(stderr,"%.1f", 0.0);
      for( colB = 0; colB <= charsB; colB++){
	fprintf(stderr,"\t%.1f", UP_LOCAL_MATRIX(colA, colB));
      }
      fprintf( stderr,"%s", "\n");
    }
    
    fprintf(stderr, "leftLocalMatrix:\n");
    for( rowB = 0; rowB < taxaB; rowB++){
      for( rowA = 0; rowA < taxaA - 1; rowA++){
	fprintf( stderr,"%s", "\t");
      }
      fprintf( stderr,"%s", b->getTaxonName(rowB));
      for( colB = 0; colB < charsB; colB++){
	fprintf( stderr,"\t%c", b->getCharacter(rowB, colB));
      }
      fprintf( stderr,"%s", "\n");
    }

  
    for( colA = 0; colA <= charsA; colA++){
      for( rowA = 0; rowA < taxaA; rowA++){
	fprintf( stderr,"%c\t", a->getCharacter(rowA, colA));
      }
      fprintf(stderr,"%.1f", 0.0);
      for( colB = 0; colB <= charsB; colB++){
	fprintf(stderr,"\t%.1f", LEFT_LOCAL_MATRIX(colA, colB));
      }
      fprintf( stderr,"%s", "\n");
    }
    
    fprintf(stderr, "localMatrix:\n");
    for( rowB = 0; rowB < taxaB; rowB++){
      for( rowA = 0; rowA < taxaA - 1; rowA++){
	fprintf( stderr,"%s", "\t");
      }
      fprintf( stderr,"%s", b->getTaxonName(rowB));
      for( colB = 0; colB < charsB; colB++){
	fprintf( stderr,"\t%c", b->getCharacter(rowB, colB));
      }
      fprintf( stderr,"%s", "\n");
    }

  
    for( colA = 0; colA <= charsA; colA++){
      for( rowA = 0; rowA < taxaA; rowA++){
	fprintf( stderr,"%c\t", a->getCharacter(rowA, colA));
      }
      fprintf(stderr,"%.1f", 0.0);
      for( colB = 0; colB <= charsB; colB++){
	fprintf(stderr,"\t%.1f", LOCAL_MATRIX(colA, colB));
      }
      fprintf( stderr,"%s", "\n");
    }
    
  }
} 


/**
 * Allocate memory, check the allocation and set the contents to zero 
 */
void* QAlign::check_calloc( size_t count, size_t size){
  void* ptr = malloc( count * size);
  if( ! ptr){
    throw PsodaError("Alloc error (QAlign)!");
  }
  memset( ptr, 0, count * size);
  
  return ptr;
}

/**
 * Calculate the percent identity for the default dataset.
 * The percent identity is the average hamming distance between all sequences.
 * \param printCounts if set, writes all of the pairwise identities to STDOUT.
 * \sa pairwisePercentIdentity()
 */
float QAlign::percentIdentity(bool printCounts){
  float percentID = 0.0;
  float percentIDSubMat = 0.0; // hamming distance
  float percentIDSubMatSum = 0.0; // use the values in a substitution matrix instead of similarity
  
  int pairwiseCounts[NUM_ASCII_VALUES][NUM_ASCII_VALUES];
  int pairwiseCountsSymmetrical[NUM_ASCII_VALUES][NUM_ASCII_VALUES];

  
  Dataset* dataset = Interpreter::getInstance()->dataset();
  
  // allocate distMatrix memory for two dimensions
  distMatrix = new double*[ntaxa];
  for (int i = 0; i < ntaxa; ++i)
  {
    distMatrix[i] = new double[ntaxa];
  }

  for( int i = 0; i < NUM_ASCII_VALUES; i++){
    for( int j = 0; j < NUM_ASCII_VALUES; j++){
      pairwiseCounts[i][j] = 0;
      pairwiseCountsSymmetrical[i][j] = 0;
    }
  }
  
  int counter = 0;
  
  if( PSODA_VERBOSE){ fprintf( stderr, "%s", "Pairwise: "); }

  /* for each sequence */
  for (int i = 0; i < ntaxa; ++i)
  {
    distMatrix[i][i] = 0;
    
    if( PSODA_VERBOSE){ fprintf( stderr, "%s", "."); }
    /* for every other sequence */
    for (int j = i + 1; j < ntaxa; ++j)
    {
      percentID += pairwisePercentIdentity(dataset->getCharacters(i,false), dataset->getSeqLen(i), dataset->getCharacters(j,false), dataset->getSeqLen(j), &percentIDSubMat, pairwiseCounts);
      percentIDSubMatSum += percentIDSubMat;
      //fprintf(stderr,"DP Matrix for %d and %d\n", i, j);
      //printDPMatrices(dataset->getCharacters(i), dataset->getCharacters(j));

      counter++;
    }
  }
  if( PSODA_VERBOSE){ fprintf( stderr, "%s", "\n"); }

  /* normalize */
  percentID /= counter;
  percentIDSubMatSum /= counter;
  
  PsodaPrinter::getInstance()->write("percent identity: %f\n", percentID);
  if( subMat->getName() != "identity"){
    PsodaPrinter::getInstance()->write("percent identity(%s): %f\n", subMat->getName().c_str(), percentIDSubMatSum);
  }

  if( printCounts){
    bool rowsAndColsToPrint[NUM_ASCII_VALUES];
    for( int i = 0; i < NUM_ASCII_VALUES; i++){
      rowsAndColsToPrint[i] = false;
    }
    for( int i = 0; i < NUM_ASCII_VALUES; i++){
      for( int j = 0; j < NUM_ASCII_VALUES; j++){
	if( pairwiseCounts[i][j] > 0){
	  rowsAndColsToPrint[i] = true;
	  rowsAndColsToPrint[j] = true;
	  if( i <= j){
	    pairwiseCountsSymmetrical[i][j] += pairwiseCounts[i][j];
	  }else{
	    pairwiseCountsSymmetrical[j][i] += pairwiseCounts[i][j];
	  }
	}
      }
    }

    printf("%s", "Pairwise Counts:\n");
    for( int j = 0; j < NUM_ASCII_VALUES; j++){
      if( rowsAndColsToPrint[j]){
	printf("\t%c", j);
      }
    }
    printf("%s", "\n");
    for( int i = 0; i < NUM_ASCII_VALUES; i++){
      if( rowsAndColsToPrint[i]){
	printf("%c", i);
	for( int j = 0; j < NUM_ASCII_VALUES; j++){
	  if( rowsAndColsToPrint[j]){
	    printf("\t%d", pairwiseCountsSymmetrical[i][j]);
	  }
	}
	printf("%s", "\n");
      }
    }
  }
  
  return percentID;
} /* END percentIdentity() */


/**
 * Calculate the percent identity between sequences \a a and \a b.
 * \param a a sequence.
 * \param sizea the length of \a a.
 * \param b a sequence.
 * \param sizeb the length of \a b.
 * \param percentIDSubMat the percent "identity" using a substitution matrix to scores differences.
 * \param pairwiseCounts 2D array of hamming scores between sequences.
 */
float QAlign::pairwisePercentIdentity(const char *a, int sizea, const char *b, int sizeb, float* percentIDSubMat, int pairwiseCounts[NUM_ASCII_VALUES][NUM_ASCII_VALUES])
{
  int i, j;
  //float matCost;

  initDPMatrices( sizea, sizeb);
	
  int maxA = (penalizeLeadingGaps == true) ? sizea : sizea - 1;
  int maxB = (penalizeLeadingGaps == true) ? sizeb : sizeb - 1;

  /* use a DP matrix to calculate the optimal alignment between a and b */

  /* for every character in the first sequence */
  for(i = 1; i <= maxA; i++)
    {
      /* for every character in the other sequence */
      for(j = 1; j <= maxB; j++)
	{
	  //matCost = subMat->get(a[i-1], b[j-1]);
	  //if( i == maxA ||
	  //    j == maxB){
	  //  /* prevents singleton characters at the end of the alignment */	    
	  //  matCost -= 0.1;
	  //}
	  setTraceback(i, j, subMat->get(a[i-1], b[j-1]));
	}
    }

	
  if( penalizeLeadingGaps == false){
    /* allow for free gaps along the last row and column */

    i = sizea;
    for(j = 1; j <= maxB; j++){
      setTracebackLastRow(i, j, subMat->get(a[i-1], b[j-1]));
    }
	  
    j = sizeb;
    for(i = 1; i <= maxA; i++){
      setTracebackLastCol(i, j, subMat->get(a[i-1], b[j-1]));
    }
    
    i = sizea;
    j = sizeb;
    setTracebackBottomRight(i, j, subMat->get(a[i-1], b[j-1]));
  }

  float percentID = 0.0;
  *percentIDSubMat = 0.0;
  
  // Now do the traceback. Begin at traceback[sizea][sizeb] and go back to traceback[1][1]
  // Save the path in an array, then walk down the array, calculate the percent id 
  Traceback *temp = getBottomRightTraceBack( sizea, sizeb);

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
  
  i = 0;
  j = 0;
  // walk down tracebackPath[]
  for(; k < sizea + sizeb; k++){
    if( tracebackPath[k] == Diag){
      if( PSODA_DEBUG > 1){ fprintf( stderr, "Diag "); }

      if( a[i] == b[j] &&
	  a[i] != mGapChar){
	percentID++;
      }
      *percentIDSubMat += subMat->get( a[i], b[j]);
      pairwiseCounts[ (int)a[i]][ (int)b[j]]++;
      i++;
      j++;
    }else if( tracebackPath[k] == Up){
      if( PSODA_DEBUG > 1){ fprintf( stderr, "Up "); }
      pairwiseCounts[ (int)a[i]][ (int)mGapChar]++;
      i++;
    }else{
      assert( tracebackPath[k] == Left);

      if( PSODA_DEBUG > 1){ fprintf( stderr, "Left "); }
      pairwiseCounts[ (int)mGapChar][ (int)b[j]]++;
      j++;
    }

  }
  
  percentID /= max( sizea, sizeb);
  *percentIDSubMat /= max( sizea, sizeb);
  //if( PSODA_DEBUG > 9){ printDPMatrices( a, b); }
  
  return percentID;
} /* END pairwisePercentIdentity() */



#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
/**
 * Calculate the gap open penalty based on the distance from the last gap (\a distFromLastGap).
 * \param percentNotGapsOther the percent of positions that are not gaps in the other sub-alignment.
 * \param distFromLastGap the distance from the last gap.
 * \return the gap open penalty.
 */
float QAlign::gapOpenPenalty( float percentNotGapsOther, int distFromLastGap){
  if( percentNotGapsOther > 0.0 &&
      percentNotGapsOther < 0.95){
    //return gapOpen * 0.3 * percentNotGapsOther; /* (Thompson, 1994) */
    return gapOpen * percentNotGapsOther;
  }else{
    if( distFromLastGap < mGapDistanceSetting){
      /* gapExt if distFromLastGap == 0, else, a scaled version of gapOpen to prevent opening a gap close to an existing gap*/
      return gapLookup[ distFromLastGap];
    }else{
      return gapOpen;
    }
  }
}

#else /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */

/**
 * Calculate the gap open penalty based on the distance from the last gap (\a distFromLastGap).
 * \param distFromLastGap the distance from the last gap.
 * \return the gap open penalty.
 */
float QAlign::gapOpenPenalty(int distFromLastGap){
  if( distFromLastGap < mGapDistanceSetting){
    /* gapExt if distFromLastGap == 0, else, a scaled version of gapOpen to prevent opening a gap close to an existing gap*/
    return gapLookup[ distFromLastGap];
  }else{
    return gapOpen;
  }
}

#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */


/**
 * Calculate the gap extension penalty.
 * \param percentNotGapsOther the percent of positions that are not gaps in the other sub-alignment.
 * \return the gap extension penalty.
 */
float QAlign::gapExtPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			    float percentNotGapsOther
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			    ){
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
  if( percentNotGapsOther > 0.0 &&
      percentNotGapsOther < 0.95){
    //return gapExt * 0.5; /* (Thompson, 1994) */
    return gapExt * percentNotGapsOther;
  }else{
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
    return gapExt;
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
  }
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
}


/**
 * Move 1 character from the ends of the sequences that are seperated by a sufficent number of gaps (\a gapLengthMinimum) to be contiguous with the rest of the sequences.
 * Using secondary structures for alignments tends to keep 1 character hanging out on the ends, which is not biologically plausible.
 * \param dataset the dataset to clean up.
 */
/* NOTE: code to handle the first 2 characters present, but commented out */
void QAlign::cleanUpAlignment(Dataset* dataset){
  int gapLengthMinimum = 3;  /* there needs to be at least 3 gaps after (before) a single residue before we'll move it */

  int nchars = dataset->nchars();
  
  /* adjust single residues in the first and last columns of the alignment */
  
  for( int i = 0; i < dataset->ntaxa(); i++){
    /*
     * First column
     */
    if( dataset->getCharacter( i, 0) != dataset->gapchar()){
      int secondNonGapPos = 1;
      while( secondNonGapPos < dataset->nchars() &&
	     dataset->getCharacter( i, secondNonGapPos) == dataset->gapchar()){
	secondNonGapPos++;
      }

      if( secondNonGapPos < dataset->nchars() &&
	  secondNonGapPos > gapLengthMinimum){

	if( PSODA_DEBUG > 8){ 
	  PsodaPrinter::getInstance()->write("Moving first column singleton '%c' for taxon %d (%s): (secondNonGapPos = %d)\n", dataset->getCharacter( i, 0), i, dataset->getTaxonName(i), secondNonGapPos);
	  PsodaPrinter::getInstance()->write("%s", "Before:\n");
	  for( int j = 0; j <= secondNonGapPos; j++){
	    PsodaPrinter::getInstance()->write("%c", dataset->getCharacter( i, j));
	  }
	  PsodaPrinter::getInstance()->write("%s", "\n");
	}
	dataset->setACharacter( dataset->getCharacter( i, 0), secondNonGapPos - 1, i);
	dataset->setACharacter( dataset->gapchar(), 0, i);
	
	if( PSODA_DEBUG > 8){ 
	  PsodaPrinter::getInstance()->write("%s", "After:\n");
	  for( int j = 0; j <= secondNonGapPos; j++){
	    PsodaPrinter::getInstance()->write("%c", dataset->getCharacter( i, j));
	  }
	  PsodaPrinter::getInstance()->write("%s", "\n");
	}
      }else if( secondNonGapPos == 1){
	///*
	// * first two positions are residues
	// */
	//
	//int nextNonGapPos = 2;
	//while( nextNonGapPos < dataset->nchars() &&
	//       dataset->getCharacter( i, nextNonGapPos) == dataset->gapchar()){
	//  nextNonGapPos++;
	//}
	//
	//if( nextNonGapPos < dataset->nchars() &&
	//    nextNonGapPos - 1 > gapLengthMinimum * 1.5){
	//
	//  if( PSODA_DEBUG > 8){ 
	//    PsodaPrinter::getInstance()->write("Moving first two column singletons '%c%c' for taxon %d (%s): (nextNonGapPos = %d)\n", dataset->getCharacter( i, 0), dataset->getCharacter( i, 1), i, dataset->getTaxonName(i), nextNonGapPos);
	//    PsodaPrinter::getInstance()->write("%s", "Before:\n");
	//    for( int j = 0; j <= nextNonGapPos; j++){
	//      PsodaPrinter::getInstance()->write("%c", dataset->getCharacter( i, j));
	//    }
	//    PsodaPrinter::getInstance()->write("%s", "\n");
	//  }
	//  dataset->setACharacter( dataset->getCharacter( i, 0), nextNonGapPos - 1 - 1, i);
	//  dataset->setACharacter( dataset->getCharacter( i, 1), nextNonGapPos - 1, i);
	//  dataset->setACharacter( dataset->gapchar(), 0, i);
	//  dataset->setACharacter( dataset->gapchar(), 1, i);
	//
	//  if( PSODA_DEBUG > 8){ 
	//    PsodaPrinter::getInstance()->write("%s", "After:\n");
	//    for( int j = 0; j <= nextNonGapPos; j++){
	//      PsodaPrinter::getInstance()->write("%c", dataset->getCharacter( i, j));
	//    }
	//    PsodaPrinter::getInstance()->write("%s", "\n");
	//  }
	//}
      }
    }

    /*
     * Last column
     */

    if( dataset->getCharacter( i, nchars - 1) != dataset->gapchar()){
      int secondNonGapPos = nchars - 2;
      while( secondNonGapPos > 0 &&
	     dataset->getCharacter( i, secondNonGapPos) == dataset->gapchar()){
	secondNonGapPos--;
      }

      if( secondNonGapPos > 0 &&
	  (nchars - 2) - secondNonGapPos > gapLengthMinimum){

	if( PSODA_DEBUG > 8){ 
	  PsodaPrinter::getInstance()->write("Moving last column singleton '%c' for taxon %d (%s): (secondNonGapPos = %d, nchars = %d)\n", dataset->getCharacter( i, nchars - 1), i, dataset->getTaxonName(i), secondNonGapPos, nchars);
	  PsodaPrinter::getInstance()->write("%s", "Before:\n");
	  for( int j = secondNonGapPos; j < nchars; j++){
	    PsodaPrinter::getInstance()->write("%c", dataset->getCharacter( i, j));
	  }
	  PsodaPrinter::getInstance()->write("%s", "\n");
	}
	dataset->setACharacter( dataset->getCharacter( i, nchars - 1), secondNonGapPos + 1, i);
	dataset->setACharacter( dataset->gapchar(), nchars - 1, i);
	
	if( PSODA_DEBUG > 8){ 
	  PsodaPrinter::getInstance()->write("%s", "After:\n");
	  for( int j = secondNonGapPos; j < nchars; j++){
	    PsodaPrinter::getInstance()->write("%c", dataset->getCharacter( i, j));
	  }
	  PsodaPrinter::getInstance()->write("%s", "\n");
	}
      }else if(secondNonGapPos == nchars - 2){
	///*
	// * last two positions are residues
	// */
	//
	//int nextNonGapPos = nchars - 2 - 1;
	//while( nextNonGapPos > 0 &&
	//       dataset->getCharacter( i, nextNonGapPos) == dataset->gapchar()){
	//  nextNonGapPos--;
	//}
	//
	//if( nextNonGapPos > 0 &&
	//    (nchars - 2 - 1) - nextNonGapPos > gapLengthMinimum * 1.5){
	//
	//  if( PSODA_DEBUG > 8){ 
	//    PsodaPrinter::getInstance()->write("Moving last two column singletons '%c%c' for taxon %d (%s): (nextNonGapPos = %d, nchars = %d)\n", dataset->getCharacter( i, nchars - 1), dataset->getCharacter( i, nchars - 1 - 1), i, dataset->getTaxonName(i), nextNonGapPos, nchars);
	//    PsodaPrinter::getInstance()->write("%s", "Before:\n");
	//    for( int j = nextNonGapPos; j < nchars; j++){
	//      PsodaPrinter::getInstance()->write("%c", dataset->getCharacter( i, j));
	//    }
	//    PsodaPrinter::getInstance()->write("%s", "\n");
	//  }
	//  dataset->setACharacter( dataset->getCharacter( i, nchars - 1 - 1), nextNonGapPos + 1, i);
	//  dataset->setACharacter( dataset->getCharacter( i, nchars - 1), nextNonGapPos + 1 + 1, i);
	//  dataset->setACharacter( dataset->gapchar(), nchars - 1 - 1, i);
	//  dataset->setACharacter( dataset->gapchar(), nchars - 1, i);
	//
	//  if( PSODA_DEBUG > 8){ 
	//    PsodaPrinter::getInstance()->write("%s", "After:\n");
	//    for( int j = nextNonGapPos; j < nchars; j++){
	//      PsodaPrinter::getInstance()->write("%c", dataset->getCharacter( i, j));
	//    }
	//    PsodaPrinter::getInstance()->write("%s", "\n");
	//  }
	//}
      }
    }
  }
}
