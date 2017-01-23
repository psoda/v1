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
#ifndef PSODA_QSSPREDICT 
#define PSODA_QSSPREDICT

#include "Dataset.h"
#include "SSDataset.h"
#include "EvaluatorBase.h"
#include "QTreeRepository.h"
#include "PsodaPrinter.h"

extern "C" {
	#include <floatfann.h>
	#include <fann.h>
}

/**
 * Secondary structure (SS) predictor.
 */
class QSSPredict
{
 protected:
  struct fann** anns;
  int numAnns;
 
  Dataset* mDataset;          /* principal dataset */

  PsodaPrinter* mPrintBuffer;
  
  struct fann* init_ann_from_file(char* filename);		/* must be called before sequences can be converted */
  int seq2structure(const char* seq, char* structure_out);
  
  
 public:
  QSSPredict(char** netFiles, int numNets);
  virtual ~QSSPredict();
  
  void sspredict(SSDataset** predictedSSDataset);
};
#endif /* PSODA_QSSALIGN */

