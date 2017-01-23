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
#ifndef PSODA_SSDATASET_H
#define PSODA_SSDATASET_H

#include "Dataset.h"
#include <string>
#include <vector>

/* 3 state characters */
#define SS_CHAR_HELIX             ('H')
#define SS_CHAR_BETA_STRAND       ('E')
#define SS_CHAR_LOOP              ('L')
#define SS_CHAR_GAP               ('-')
#define SS_CHAR_GAP_CODONS        (NUM_CODONS)

#define SS_CHAR_HELIX_INDEX       (0)
#define SS_CHAR_BETA_STRAND_INDEX (1)
#define SS_CHAR_LOOP_INDEX        (2)
#define SS_CHAR_GAP_INDEX         (3)
#define SS_CHAR_GAP_CODONS_INDEX  (4)
#define SS_CHAR_MAX_INDEX         (SS_CHAR_GAP_CODONS_INDEX)
#define SS_CHAR_NUMBER_OF_INDICES (SS_CHAR_MAX_INDEX + 1)

using namespace std;

class SSDataset : public Dataset
{
 public:
  typedef enum { UNDEFINED_SECONDARY_STRUCTURE, ALPHA_HELIX_SECONDARY_STRUCTURE, BETA_SHEET_SECONDARY_STRUCTURE, LOOP_SECONDARY_STRUCTURE, GAP_SECONDARY_STRUCTURE } threeStateSecondaryStructures;
	
  SSDataset();
  SSDataset( const SSDataset& ssDataset );

  SSDataset& operator=(const SSDataset&);

  ~SSDataset();

  void convertTo3State();
  bool printCombinedSeqs(string type, string filename, Dataset* dataset);
  
  static const int  translateSSASCII2Num[128];
  static const char translateSSNum2ASCII2[SS_CHAR_NUMBER_OF_INDICES];

 protected:
 
  void convertDNAToCodons();
  void convertCodonsToDNA();
  char* mMatrix; //These belong in Dataset, and should be replaced by their equivalents
  int mNCharsAlloced; //These belong in Dataset, and should be replaced by their equivalents
  
 private:

  
};

#endif /* PSODA_SSDATASET_H */
