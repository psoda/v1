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
#ifndef PSODA_QSSALIGN 
#define PSODA_QSSALIGN

#include "QAlign.h"
#include "Dataset.h"
#include "SSDataset.h"
#include "SubMat.h"
#include "QNode.h"
#include "EvaluatorBase.h"
#include "QTreeRepository.h"
#include "PsodaPrinter.h"

/**
 * Progressive multiple sequence alignment that incorporates secondary structures (SS).
 */
class QSSAlign : public QAlign
{
 protected:
  char mGapCharSS;            /* '-' unless aligning codons, then GAP_LOC */
  char mEndCharSS;            /* '\0' unless aligning codons, then -1 */
  
  SubMat* subMatA;            /* substitution matrix for Alpha-helices */
  SubMat* subMatB;            /* substitution matrix for beta-sheets */
  SubMat* subMatL;            /* substitution matrix for loops */
  SubMat* subMat;             /* substitution matrix for default case (i.e., none of the above */
  SubMat* subMatSS;           /* substitution matrix for Secondary Structure elements */

  Dataset* mDataset;          /* principle dataset */
  SSDataset* mSSDataset;      /* secondary structure dataset */

  PsodaPrinter* mPrintBuffer;

  float gapOpenHelix;
  float gapOpenBetaStrand;
  float gapOpenLoop;

  float* gapLookupHelix;       /* used for simplfied affine gap penalty */
  float* gapLookupBetaStrand;  /* used for simplfied affine gap penalty */
  float* gapLookupLoop;        /* used for simplfied affine gap penalty */

  
  double ssMatCost(char a, char ssA, char b, char ssB);


  void calculateDistMatrix(Dataset *dataset, SSDataset* ssDataset);
  double calculateDPMatrices(const char *a, const char* ssA, int sizea, const char *b, const char* ssB, int sizeb); /* pairwise alignment */
    
  void classicSSPMSA(QNode *root);

  void classicProgressiveSSAlign(QNode *root);  /* final profile alignment */
  //void multCharAlign(QNode *root);
  void setupLeafSSDataset(QNode *root);
  double progressiveSSMatCost(int colA, Dataset* a, SSDataset* ssA, int colB, Dataset* b, SSDataset* ssB);

  void ssPMSA(QNode *root);
  void progressiveSSAlign(QNode *root);
  void setupMultCharSS(QNode *root);
  void pushGapsDownSS(QNode *root, Dataset* alignedDataset, SSDataset* alignedSSDataset);
  void multCharAlignSS(QNode *root);



  void setTracebackSSPairwise(int i, int j, double matCost, char mostCommonSSLeft, char mostCommonSSUp);
  void setLocalDPMatrics(int i, /*int maxRow,*/ int j, /*int maxCol,*/ double matCost, char mostCommonSSLeft, char mostCommonSSUp);
  void setTracebackSSLastRowPairwise(int i, int j, double matCost, char mostCommonSSLeft);
  void setTracebackSSLastColPairwise(int i, int j, double matCost, char mostCommonSSUp);
  
  void setTracebackSS(int i, int j, double matCost, char mostCommonSSLeft, char mostCommonSSUp
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
		      , float percentNotGapsLeft = 1.0, float percentNotGapsUp = 1.0
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
		      );  /* maxRow and maxCol are the indices of the largest row and column respectively */
  void setTracebackSSLastRow(int i, int j, double matCost, char mostCommonSSLeft
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			     , float percentNotGapsLeft = 1.0
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			     ) ;
  void setTracebackSSLastCol(int i, int j, double matCost, char mostCommonSSUp
#ifdef ADVANCED_PROGRESSIVE_GAP_PENALTIES
			     , float percentNotGapsUp = 1.0
#endif /* ADVANCED_PROGRESSIVE_GAP_PENALTIES */
			     );

  float gapOpenPenalty(int distFromLastGap, char mostCommonSSInOther);



  void setTracebackSS(int i, int j, double matCost, SSDataset* ssA, SSDataset* ssB) __attribute__((noreturn));
  void setTracebackSSLastRow(int i, int j, double matCost, SSDataset* ssA) __attribute__((noreturn));
  void setTracebackSSLastCol(int i, int j, double matCost, SSDataset* ssB) __attribute__((noreturn));
  
  
 public:
  QSSAlign(float gapOpenIn, float gapOpenHelixIn, float gapOpenBetaStrandIn, float gapOpenLoopIn, float gapExtIn, SubMat* subMatAIn, SubMat* subMatBIn, SubMat* subMatLIn, SubMat* subMatIn, SubMat* subMatSSIn, bool penalizeLeadingGapsIn, bool useExistingTree, int gapDistanceSetting, float localWeight);
  virtual ~QSSAlign();
  
  void ssalign(QTreeRepository &qtreeRepository, bool useExistingTree, Dataset** alignedDataset, SSDataset** alignedSSDataset);
};
#endif /* PSODA_QSSALIGN */

