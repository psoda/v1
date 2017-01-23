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
#ifndef PSODA_DATASET_H
#define PSODA_DATASET_H

#include <string>
#include <vector>
#include <errno.h>
#include <stdio.h>

#define ASCII_TABLE_SIZE   123 /* largest ASCII value used + 1 */

#define NUM_AA                     20  /* number of amino acids residues */
#define NUM_AA_ALL       (NUM_AA + 1)  /* number of amino acids (not 20 to account for stop/start (*) */

/* codon globals */
#define NUM_CODONS 64
#define CHARS_PER_CODON 3
#define CHARS_PER_RESIDUE_STR   (CHARS_PER_CODON + 1)
#define FIRST_POS  4
#define SECOND_POS 2

/* convertCodon() takes 3 DNA characters numbers (like A_NUM - T_NUM) and returns the 0-63 codon code */
#define convertCodon(first,second,third) (((first) << FIRST_POS) | ((second) << SECOND_POS) | (third))


#define T_NUM           0x0
#define C_NUM           0x1
#define A_NUM           0x2
#define G_NUM           0x3
#define U_NUM           T_NUM
#define GAP_LOC         NUM_CODONS  /* codon number index for gap */

#define FIRST_CHAR      T_NUM
#define LAST_CHAR       G_NUM

#define BITS_PER_CHAR   2
#define CHAR_MASK       0x3


typedef enum { UNDEFINED_BLOCK, SS_BLOCK /* secondary structure */, REF_SS_BLOCK /* secondary structure reference data set */, REF_BLOCK /* reference data set */ } BlockType;

  
using namespace std;

class Dataset
{
 public:

/**
 * Datatype identifies the type of dataset (e.g., nucleotide, amino acid or codons) 
 */

typedef enum { UNDEFINED_DATATYPE, NUCLEOTIDE_DATATYPE, DNA_DATATYPE, STANDARD_DATATYPE, RNA_DATATYPE, PROTEIN_DATATYPE, PROTEIN_CODING_DNA_DATATYPE /* untranslated codons */, CODON_DATATYPE, SECONDARY_STRUCTURE_DATATYPE, SECONDARY_STRUCTURE_3_STATE_DATATYPE } Datatype ;

/**
 * DataFormat identifies the format of the dataset (e.g., aligned, unaligned)
 */

typedef enum { UNDEFINED_DATAFORMAT, UNALIGNED_DATAFORMAT, ALIGNED_DATAFORMAT } DataFormat;

	
  Dataset(BlockType blockType = UNDEFINED_BLOCK);
  //Dataset( const Dataset& dataset );

  //Dataset& operator=(const Dataset&);

  virtual ~Dataset();

 
	
  
			
 
  

  

/* METHODS FOR CONVERTING BETWEEN TAXA NAME AND TAXA NUMBER */
	
	const char* getTaxonName( int taxonNumber );
	int getTaxonNumber(const char *taxonName);
  		 	
  /*METHODS FOR BUILDING A DATASET*/
  	
  void addName( const char* taxonName);
  void addCharacters( const char* taxonCharacters, int pos = -1 );
  void appendACharacter( const char character, int col, int pos = -1 );
  
  
  void setACharacter(char character, int col, int pos);

  void setntaxa( int ntaxa );	/*TRY TO ELIMINATE*/
  void setnchars( int ncharacters ); /*TRY TO ELIMINATE*/

  /* UTILITY METHODS*/
  void convertDNAToCodons();
  void convertCodonsToDNA();
  
  char convertToBinary(char) const;
  static char convertToASCII(char);
  	
  bool matches(Dataset* otherDataset, bool namesOnly = false);
  void stripGaps();
  
  int getLen(const char* str);
  int getSeqLen( int taxonNumber);
	  
	 /*GETTERS AND SETTERS*/


  char getCharacter( int taxonNumber, int col ) const;
  char* getCharacters( int taxonNumber,bool binary );
  vector<string>& taxonNames();	
	 
	  int* weights() const;
  void resetWeights();
  void randomizeWeights(double percent=0.25);
  
	 int nchars() const;
	int ntaxa() const;
	 
	  char gapchar() const;
	  char& gapchar();
	  
	  char missingchar() const;
	  char& missingchar();
	  
	  Datatype& datatype();
	  Datatype datatype() const;
	  
	  DataFormat dataformat() const;
	  DataFormat& dataformat();
		
	 char endchar() const;
	  
	  //  char gapvalue() const;
	  //  char& gapvalue();
	  
	  char& matchchar();
	  char matchchar() const; 

	BlockType& blocktype();
	BlockType blocktype() const;
	
	 /*DISPLAY METHODS
	  future: rename and consolidate to print() */
  void printTNT(char* filename);
  void printMatrix ();
  void printMatrix(string filename);
  //void printMatrixNexus(string filename);
  void printMatrixNexus(string filename, bool fixforbayes);
  bool printSeqs(string type, string filename);
  //void printTranslationMatrix();
  //void printPairWiseDistances();

protected:

  int mNTaxa;                   // number of taxa
  
  int mNCharacters;             // (aligned) number of characters per taxon (unaligned) max number of characters
	char mGapChar;                // the gap character for this dataset
  char mMissingChar;            // the missing character for this dataset
    char mMatchChar;              // the match character for this dataset
	char mEndChar;                // the end (terminal) character for this dataset
  
  Datatype mDatatype;           // the type of data in the dataset DNA, PROTEIN, etc
  DataFormat mDataFormat;       // the inter data storage format MATRIX, SEQUENCES, etc
  BlockType mBlockType;           // the type of data block that was parsed
  
  vector<string> mTaxonNames;   // list of taxon names in the same order as they appear in the dataset
  int* mWeights;               // array of integral weights.  Indices are the columns of the sequences.  Used primarily to compress columns.
  int* mWeightsBak;            // backup of mWeights 

  bool mSequenceChanged;		//dirty bit for ASCII sequence, used to determine when compression/conversion is required
  int mCurSequence;				//current sequence to add charater to
  char** mASCIIData;			// sequence data in printable form
  char** mBinaryData;			// sequence data in binary form
  char** mCodonData;			// codon data
  
  void calculateCompressedBinaryMatrix(); 
  char charsToCodon(char first, char second, char third);
  
};


#endif /* PSODA_DATASET_H */
