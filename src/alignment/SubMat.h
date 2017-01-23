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
#ifndef PSODA_SUBMAT_H
#define PSODA_SUBMAT_H

#include <string>
#include <iostream>

#include "Dataset.h"
#include <string>
#include <algorithm>
#include <vector>

#define NUM_ASCII_VALUES           128

using namespace std;

class Dataset;

/** SubMat Description:
 *
 * SubMat is a substitution matrix.
 * The indices are ascii values.
 * The constructor can take either a hard-coded sub. mat. (see matrices.h) or a filename (which is read in and stored)
*/

class SubMat
{

 public:
  /* constuctors & destructors */
  SubMat( string fileName, Dataset* dataset, bool setGapPenaltiesToLowest = true);
  ~SubMat();
  void printMatrix();
  
  
  //GETTERS AND SETTERS

  double get(int from, int to);
  string getName(){ return subMatDescription; }

 protected:
  string subMatDescription; /* description (e.g., name) of substitution matrix */
  double subMat[NUM_ASCII_VALUES][NUM_ASCII_VALUES]; /* the actual substitution matrix */
  Dataset::Datatype mDatatype;           // the type of data in the dataset DNA, PROTEIN, etc
  int mGapChar;                          // value of a gapchar (from Dataset)
  
 private:

  bool mSetGapPenaltiesToLowest;
    
  bool readSubMatrixFile( string matrixFileName);
  void loadIdentitySubMatrix();
  void loadIdentityNegativeSubMatrix();
  void loadDefaultSubMatrix( string defaultMatrix);
  void tokenizeString(const string& str,
			      vector<string>& tokens,
			      const string& delimiters = " ,\t");
  void formatForDefaultSubMats();
  void setGapPenalties();

};
#endif /* PSODA_SUBMAT_H */
