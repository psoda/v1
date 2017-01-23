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
#ifndef QSODA_QALIGN 
#define QSODA_QALIGN

#include "QNode.h"
#include "Dataset.h"
#include "QTreeRepository.h"
#include "SubMat.h"


/**
 * \a Path directions for the traceback matrix.
 */
typedef enum direction_t {None, Up, Left, Diag} Direction;

/**
 * Stores the necessary information for the traceback matrix.
 */
struct Traceback
{
  Direction direction;   /* direction of best path from the respective location */
  Traceback *next;       /* pointer to the previous character in the best path from the respective location */
};


//#define NO_GAP                   0
//#define IS_GAP                   1

#define M_MATRIX(row,col)                  (mMatrix[((row) * dpColsAlloced) + (col)])
#define UP_MATRIX(row,col)                (upMatrix[((row) * dpColsAlloced) + (col)])
#define LEFT_MATRIX(row,col)            (leftMatrix[((row) * dpColsAlloced) + (col)])
				     
// only used for local alignment     
#define UP_LOCAL_MATRIX(row,col)     (upLocalMatrix[((row) * dpColsAlloced) + (col)])
#define LEFT_LOCAL_MATRIX(row,col) (leftLocalMatrix[((row) * dpColsAlloced) + (col)])
#define LOCAL_MATRIX(row,col)          (localMatrix[((row) * dpColsAlloced) + (col)])
				     
#define M_TRACEBACK(row,col)            (mTraceback[((row) * dpColsAlloced) + (col)])
#define UP_TRACEBACK(row,col)          (upTraceback[((row) * dpColsAlloced) + (col)])
#define LEFT_TRACEBACK(row,col)      (leftTraceback[((row) * dpColsAlloced) + (col)])

#define GAPUPDIST(row,col)               (upGapDist[((row) * dpColsAlloced) + (col)])
#define GAPLEFTDIST(row,col)           (leftGapDist[((row) * dpColsAlloced) + (col)])


#define max(a,b)     (((a) > (b)) ? (a) : (b))
#define max3(a,b,c)  (max(max((a),(b)),(c)))

#define min(a,b)     (((a) < (b)) ? (a) : (b))
#define min3(a,b,c)  (min(min((a),(b)),(c)))


#define NEG_INFINITY ((double)(INT_MIN * 0.5)) /* "half" of negative infinity to avoid overflow */


/**
 * Progressive multiples sequence alignment.
 */
class QAlign
{
  protected:
  int ntaxa;                 /* copy of value from dataset */
  int nchars;                /* copy of value from dataset */
  double** distMatrix;       /* row size = ntaxa, col size = ntaxa */
  
  int dpRowsAlloced;         /* number of dynamic programming rows allocated for the following matrices: mMatrix, upMatrix, leftMatrix, upLocalMatrix, leftLocalMatrix, localMatrix, mTraceback, upTraceback, leftTraceback, upGapDist and leftGapDist */
  int dpColsAlloced;         /* number of dynamic programming columns allocated for the following matrices: mMatrix, upMatrix, leftMatrix, upLocalMatrix, leftLocalMatrix, localMatrix, mTraceback, upTraceback, leftTraceback, upGapDist and leftGapDist */
  double* mMatrix;         /* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix for alignments ending in a matching two sub-alignment columns */
  double* upMatrix;        /* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix for alignments ending with a gap in the "up" sequence */
  double* leftMatrix;      /* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix for alignments ending with a gap in the "left" sequence */

  double* upLocalMatrix;   /* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix for alignments ending in a matching two sub-alignment columns */
  double* leftLocalMatrix; /* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix for alignments ending with a gap in the "up" sequence */	
  double* localMatrix;     /* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix for alignments ending with a gap in the "left" sequence */	
  
  Traceback* mTraceback;   /* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix of traceback pointers for alignments ending in a matching two sub-alignment columns */
  Traceback* upTraceback;  /* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix of traceback pointers for alignments ending with a gap in the "up" sequence */	
  Traceback* leftTraceback;/* 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced; matrix of traceback pointers for alignments ending with a gap in the "left" sequence */	
  
  int* upGapDist;          /* The distance from the last gap in the "up"   sequence/profile; 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced */
  int* leftGapDist;        /* The distance from the last gap in the "left" sequence/profile; 2D array stored in a 1D configuration; rows = dpRowsAlloced, cols = dpColsAlloced */
  
  char mGapChar;           /* '-' unless aligning codons, then GAP_LOC */
  char mEndChar;           /* '\0' unless aligning codons, then -1 */
  
  float gapOpen;           /* penalty value */
  float gapExt;            /* penalty value */
  int mGapDistanceSetting; /* penalize starting a new gap with gapDistanceSetting of the last gap */
  float* gapLookup;        /* used for simplfied affine gap penalty */

  /** \def ADVANCED_PROGRESSIVE_GAP_PENALTIES ClustalW style gap penalties for progressive alignment.
   * Namely, if a sub-alignment already has a certain threshold of gaps, then make it easier for gaps in the other sub-alignment.
   */
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
  float gapOpenPenalty( float percentNotGapsOther, int distFromLastGap);
#else /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
  float gapOpenPenalty(int distFromLastGap);         /* used for simplfied affine gap penalty */
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
  
  float gapExtPenalty(
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
		      float percentNotGapsOther
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
		      );
  bool penalizeLeadingGaps;  /* if set, penalize leading gaps (the gaps on the ends of the sequences) */
  char* guideTree;           /* newick guide tree */ 
  SubMat* subMat;            /* substition matrix to use for alignment */
  bool mUseClassicPMSA;      /* if set, use the stock (unoptimized) progressivive sequence alignment methods of iterating through every character in one sub-alignment with every other character in the other alignment.  If no set, use the optimizied MultChars. */
  float mLocalWeight;          /* if none zero, calculate a local alignment and add DP weights to mat cost */
  
  void initDPMatrices( int sizeTop, int sizeLeft);
  void freeDPMatrices();

  void setTracebackDiagPairwise(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost);
  void setTracebackPairwise(int i, int j, double matCost);
  void setTracebackLastRowPairwise(int i, int j, double matCost);
  void setTracebackLastColPairwise(int i, int j, double matCost);
  void setTracebackBottomRightPairwise(int i, int j, double matCost);
  
  void setTracebackDiag(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost);
  void setTraceback(int i, int j, double matCost
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
		    , float percentNotGapsLeft = 1.0, float percentNotGapsUp = 1.0
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
		    );  /* maxRow and maxCol are the indices of the largest row and column respectively */
  void setTracebackLastRow(int i, int j, double matCost
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			   , float percentNotGapsLeft = 1.0
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			   );
  void setTracebackLastCol(int i, int j, double matCost
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			   , float percentNotGapsUp = 1.0
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			   );
  void setTracebackBottomRight(int i, int j, double matCost);

  void setLocalDPDiag(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost);
  void setLocalDPMatrics(int i, int j, double matCost);
  Traceback* getBottomRightTraceBack(int i, int j);


  
  void* check_calloc( size_t count, size_t size);  

  void calculateDistMatrix(Dataset *dataset);
  double calculateDPMatrices(const char *a, int sizea, const char *b, int sizeb); /* pairwise alignment */
  //float max3(float a, float b, float c);
    
  void PMSA(QNode *root, Dataset *dataset);
  void postorder(QNode *root, char *introstring);
  void pushGapsDown(QNode *root, Dataset* dataset, Dataset* alignedDataset);
  void pushGaps(QNode *root);

  void setupMultChar(QNode *root, Dataset *dataset);
  void progressiveAlign(QNode *root);
  void multCharAlign(QNode *root); /* final profile alignment */
  void progressiveTraceback(QNode* root);
  
  void multCharLocalAlign(list<MultChar *> *a, list<MultChar *> *b);


  void classicPMSA(QNode *root, Dataset *dataset);
  void setupLeafDataset(QNode *root, Dataset *dataset);
  void classicProgressiveAlign(QNode *root);
  double progressiveMatCost(int colA, Dataset* a, int colB, Dataset* b);

  void classicLocalAlign(Dataset* a, Dataset* b);
  
  void printMatrix(void);
  void printDPMatrices(const char *a, const char *b);
  void printDPMatrices(int sizea, int sizeb);
  void printDPMatrices( list<MultChar *> *a, list<MultChar *> *b);
  void printDPMatrices( Dataset *a, Dataset *b);

  float pairwisePercentIdentity(const char *a, int sizea, const char *b, int sizeb, float* percentIDSubMat, int pairwiseCounts[NUM_ASCII_VALUES][NUM_ASCII_VALUES]);

  void cleanUpAlignment(Dataset* dataset);
 
 public:
  QAlign(float gapOpenIn, float gapExtIn, SubMat* subMatIn, bool penalizeLeadingGapsIn, bool useClassicPMSA, int gapDistanceSetting, float localWeight);
    virtual ~QAlign();

    void align(QTreeRepository &qtreeRepository, bool useExistingTree, Dataset** alignedDataset);

    float percentIdentity( bool printCounts);
    
};
#endif

