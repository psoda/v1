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
#ifndef QSUM_OF_PAIRS
#define QSUM_OF_PAIRS

#include "Dataset.h"
#include "SSDataset.h"
#include "SubMat.h"
#include "PsodaPrinter.h"

class QSumOfPairs  /*: public QMetricBase */
{
 protected:
  int ntaxa;                /* copy of value from dataset */
  int nchars;               /* copy of value from dataset */
  
  float gapOpen;              /* penalty value */
  float gapExt;               /* penalty value */
  //float gapLookup[2];         /* used for simplfied affine gap penalty */
  bool penalizeLeadingGaps;   /* if set, penalize leading gaps (the gaps on the ends of the sequences) */
  //char* guideTree;
  SubMat* subMatA;            /* substitution matrix for Alpha-helices */
  SubMat* subMatB;            /* substitution matrix for beta-sheets */
  SubMat* subMatL;            /* substitution matrix for loops */
  SubMat* subMat;             /* substitution matrix for default case (i.e., none of the above */
  SubMat* subMatSS;           /* substitution matrix for Secondary Structure elements */

  Dataset* mDataset;          /* principle dataset */
  Dataset* mRefDataset;       /* reference dataset for the principle dataset */
  SSDataset* mSSDataset;      /* secondary structure dataset */
  SSDataset* mRefSSDataset;   /* reference dataset for the secondary structure dataset */

  PsodaPrinter* mPrintBuffer;

  double ssMatCost(char a, char ssA, char b, char ssB);
  
 public:
  QSumOfPairs(float gapOpenIn, float gapExtIn, SubMat* subMatAIn, SubMat* subMatBIn, SubMat* subMatLIn, SubMat* subMatIn, SubMat* subMatSSIn, bool penalizeLeadingGapsIn);
  virtual ~QSumOfPairs();

  float selfSumOfPairs();            /* metric for a single data set */
  float referenceSumOfPairs();       /* requires a reference data set (RefData block) */
  float selfSSSumOfPairs();          /* metric for a single data set (using secondary structure) (uses ssalign's (mis)match function)*/
  float referenceSSSumOfPairs();     /* requires a reference data set (RefData block), SS and RefSS blocks (uses ssalign's (mis)match function)*/
  float justReferenceSSSumOfPairs(); /* reference sum of pairs on just the secondary structure using a single substitution matrix */
};
#endif /* QSUM_OF_PAIRS */

