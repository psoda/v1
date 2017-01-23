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
#include "QSumOfPairs.h"
#include "Interpreter.h"
#include <assert.h>

#include <math.h>
#include <stdio.h>

/**
 * Constructor.
 * \param gapOpenIn gap open penalty.
 * \param gapExtIn gap extension penalty.
 * \param subMatAIn substitution matrix for Alpha-helices.
 * \param subMatBIn substitution matrix for Beta-sheets.
 * \param subMatLIn substitution matrix for loops.
 * \param subMatIn substitution matrix for the default case (e.g., none of the above).
 * \param subMatSSIn substitution matrix for secondary structures.
 * \param penalizeLeadingGapsIn if set, penalize leading gaps (the gaps on the ends of the sequences)
 */
QSumOfPairs::QSumOfPairs(float gapOpenIn, float gapExtIn, SubMat* subMatAIn, SubMat* subMatBIn, SubMat* subMatLIn, SubMat* subMatIn, SubMat* subMatSSIn, bool penalizeLeadingGapsIn)
  : /* QMetricBase( gapOpenIn, gapExtIn, subMatIn, penalizeLeadingGapsIn),*/
  ntaxa(0),
  nchars(0),
  gapOpen( gapOpenIn),
  gapExt( gapExtIn),
  penalizeLeadingGaps( penalizeLeadingGapsIn),
  subMatA(subMatAIn),
  subMatB(subMatBIn),
  subMatL(subMatLIn),
  subMat(subMatIn),
  subMatSS(subMatSSIn),
  mDataset( Interpreter::getInstance()->dataset()),
  mRefDataset( Interpreter::getInstance()->refDataset()),
  mSSDataset( Interpreter::getInstance()->ssDataset()),
  mRefSSDataset( Interpreter::getInstance()->refSSDataset()),
  mPrintBuffer( PsodaPrinter::getInstance())
{

  if( mDataset->dataformat() == Dataset::UNALIGNED_DATAFORMAT){
    throw PsodaError( "ERROR: Can not calculate sum-of-pairs on unaligned data!");
  }

  ntaxa = mDataset->ntaxa();
  nchars = mDataset->nchars();


  if( ntaxa == 0 ||
      nchars == 0){
    throw PsodaError( "ERROR: Empty data set (detected in QSumOfPairs())!");
  }
  
  if( subMat == NULL){
    subMat = new SubMat( "identity", mDataset, false);
  }
  
  if( mRefDataset != NULL){
    if( mRefDataset->matches( mDataset, true) == false){
      throw PsodaError( "ERROR: Reference data set does not match sequences data set (detected in QSumOfPairs())!");
    }
  }
  
  if( mSSDataset != NULL){
    if( mSSDataset->matches( mDataset, true) == false){
      throw PsodaError( "ERROR: Secondary structure data set does not match sequences data set (detected in QSumOfPairs())!");
    }

    mSSDataset->convertTo3State();
  }
  
  if( mRefSSDataset != NULL){
    if( mRefSSDataset->matches( mSSDataset, true) == false){
      throw PsodaError( "ERROR: Secondary structure reference data set does not match the secondary structure data set (detected in QSumOfPairs())!");
    }

    mRefSSDataset->convertTo3State();
  }
}

/** Destructor
 */
QSumOfPairs::~QSumOfPairs()
{}

/**
 * Calculate the normalized average percent identity between every pair of sequences.
 * \return the normalized average percent identity between every pair of sequences
 */
float QSumOfPairs::selfSumOfPairs(){
  
  assert( mDataset != NULL &&
	  subMat != NULL);
  
  float score = 0.0;
  const char* taxonStr1;
  const char* taxonStr2;
  int counter = 0;

  /* for every sequence */
  for( int taxon1 = 0; taxon1 < ntaxa; taxon1++){
    taxonStr1 = mDataset->getCharacters( taxon1,false);
    /* for every other sequence */
    for( int taxon2 = taxon1 + 1; taxon2 < ntaxa; taxon2++){
      taxonStr2 = mDataset->getCharacters( taxon2,false);
      /* for every character */
      for( int col = 0; col < nchars; col++){
	score += subMat->get( taxonStr1[col], taxonStr2[col]);
	if( isnan( score)){
	  char str[1024];
	  sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
	  throw PsodaError( str);
	}
  	counter++;
      }
    }
  }
  /* normalize */
  score /= counter;
  if( isnan( score)){
    char str[1024];
    sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
    throw PsodaError( str);
  }

  PsodaPrinter::getInstance()->write("selfSumOfPairs(%s) = %f\n", subMat->getName().c_str(), score );

  return score;
} /* END selfSumOfPairs() */


/**
 * Calculate the normalized average percent identity between every pair of sequences.
 * \return the normalized average percent identity between every pair of sequences
 */
float QSumOfPairs::selfSSSumOfPairs(){

  assert( mDataset != NULL &&
	  mSSDataset != NULL &&
	  subMat != NULL &&
	  subMatA != NULL &&
	  subMatB != NULL &&
	  subMatL != NULL &&
	  subMatSS != NULL);
  
  float score = 0.0;
  const char* taxonStr1;
  const char* taxonStr2;
  const char* taxonStrSS1;
  const char* taxonStrSS2;
  int counter = 0;
  
  for( int taxon1 = 0; taxon1 < ntaxa; taxon1++){
    taxonStr1 = mDataset->getCharacters( taxon1,false);
    taxonStrSS1 = mSSDataset->getCharacters( taxon1,false);
    for( int taxon2 = taxon1 + 1; taxon2 < ntaxa; taxon2++){
      taxonStr2 = mDataset->getCharacters( taxon2,false);
      taxonStrSS2 = mSSDataset->getCharacters( taxon2,false);
      for( int col = 0; col < nchars; col++){
	score += ssMatCost( taxonStr1[col], taxonStrSS1[col], taxonStr2[col], taxonStrSS2[col]);
	if( isnan( score)){
	  char str[1024];
	  sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
	  throw PsodaError( str);
	}
  	counter++;
      }
    }
  }
  score /= counter;
  if( isnan( score)){
    char str[1024];
    sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
    throw PsodaError( str);
  }

  PsodaPrinter::getInstance()->write("selfSSSumOfPairs(%s) = %f\n", subMat->getName().c_str(), score );

  return score;
} /* END selfSSSumOfPairs() */


/**
 * Calculates the match score for \a and \b (and their secondary structure elements \a ssA and \a ssB)
 * \param a genetic character
 * \param ssA secondary structure element associated with \a a
 * \param b genetic character
 * \param ssB secondary structure element associated with \a b
 * \return the match score
 */
double QSumOfPairs::ssMatCost(char a, char ssA, char b, char ssB){
  double ssMatCost = 0.0;

  /* check if ss elements match; if they do, use the respective substitution matrix */
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
    /* use the default substitution matrix */
    ssMatCost = subMat->get(a, b);
  }

  /* average match score and secondary structure match score */
  return ((ssMatCost + subMatSS->get(ssA, ssB)) / 2.0);
}


/**
 * Calculate the reference sum of pairs score.
 * Namely, the normalized average hamming distance between every sequence and it's analogous reference sequence.
 * \return the reference sum of pairs score.  The range of the score is between 0.0 and 1.0 when the identity matrix is used.
 */
float QSumOfPairs::referenceSumOfPairs(){
  

  assert( mDataset != NULL &&
	  mRefDataset != NULL &&
	  subMat != NULL);
  
  float score = 0.0;
  const char* taxonStr1;
  const char* taxonStr2;
  int counter = 0;

  int ncharsRef = mRefDataset->nchars();
  int minNChars = nchars;
  if( ncharsRef < minNChars){
    minNChars = ncharsRef;
  }


  fprintf( stderr, "referenceSumOfPairs(%s):\n", subMat->getName().c_str());

  /* for every taxon */
  for( int taxon1 = 0; taxon1 < ntaxa; taxon1++){
    taxonStr1 = mDataset->getCharacters( taxon1,false);
    taxonStr2 = mRefDataset->getCharacters( taxon1,false);
    /* for every column of the sequence */
    for( int col = 0; col < minNChars; col++){
      score += subMat->get( taxonStr1[col], taxonStr2[col]);
      fprintf( stderr, "\t%c, %c: score: %f\n", taxonStr1[col], taxonStr2[col], score);
      if( isnan( score)){
	char str[1024];
	sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
	throw PsodaError( str);
      }
      counter++;
    }
  }
  /* normalize */
  score /= counter;
      fprintf( stderr, "\tscore: %f (final)\n", score);
  if( isnan( score)){
    char str[1024];
    sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
    throw PsodaError( str);
  }
  
  PsodaPrinter::getInstance()->write("referenceSumOfPairs(%s) = %f\n", subMat->getName().c_str(), score );
  
  return score;
} /* END referenceSumOfPairs() */



/**
 * Calculate the reference sum of pairs score using the secondary structure match score.
 * Namely, the normalized average hamming distance between every sequence and it's analogous reference sequence.
 * \return the reference sum of pairs score.  The range of the score is between 0.0 and 1.0 when the identity matrix is used.
 */
float QSumOfPairs::referenceSSSumOfPairs(){

  assert( mDataset != NULL &&
	  mRefDataset != NULL &&
	  mSSDataset != NULL &&
	  mRefSSDataset != NULL &&
	  subMat != NULL &&
	  subMatA != NULL &&
	  subMatB != NULL &&
	  subMatL != NULL &&
	  subMatSS != NULL);

  float score = 0.0;
  const char* taxonStr1;
  const char* taxonStr2;
  const char* taxonStrSS1;
  const char* taxonStrSS2;
  int counter = 0;

  int ncharsRef = mRefDataset->nchars();
  int minNChars = nchars;
  if( ncharsRef < minNChars){
    minNChars = ncharsRef;
  }
  
  /* for every taxon */
  for( int taxon1 = 0; taxon1 < ntaxa; taxon1++){
    taxonStr1 = mDataset->getCharacters( taxon1,false);
    taxonStr2 = mRefDataset->getCharacters( taxon1,false);
    
    taxonStrSS1 = mSSDataset->getCharacters( taxon1,false);
    taxonStrSS2 = mRefSSDataset->getCharacters( taxon1,false);
    
    /* for every column of the sequence */
    for( int col = 0; col < minNChars; col++){
      score += ssMatCost( taxonStr1[col], taxonStrSS1[col], taxonStr2[col], taxonStrSS2[col]);
      //score += subMat->get( taxonStr1[col], taxonStr2[col]);
      if( isnan( score)){
	char str[1024];
	sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
	throw PsodaError( str);
      }
      counter++;
    }
  }
  /* normalize */
  score /= counter;
  if( isnan( score)){
    char str[1024];
    sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
    throw PsodaError( str);
  }
  
  PsodaPrinter::getInstance()->write("referenceSSSumOfPairs(%s) = %f\n", subMat->getName().c_str(), score );
  
  return score;
} /* END referenceSSSumOfPairs() */


/**
 * Calculate the reference sum of pairs score using just the secondary structure substitution matrix, \a subMatSS..
 * Namely, the normalized average hamming distance between every sequence and it's analogous reference sequence.
 * \return the reference sum of pairs score.  The range of the score is between 0.0 and 1.0 when the identity matrix is used.
 */
float QSumOfPairs::justReferenceSSSumOfPairs(){

  assert( mSSDataset != NULL &&
	  mRefSSDataset != NULL &&
	  subMatSS != NULL);

  float score = 0.0;
  const char* taxonStrSS1;
  const char* taxonStrSS2;
  int counter = 0;

  int ncharsRef = mRefSSDataset->nchars();
  int minNChars = mSSDataset->nchars();
  if( ncharsRef < minNChars){
    minNChars = ncharsRef;
  }
  
  /* for every taxon */
  for( int taxon1 = 0; taxon1 < ntaxa; taxon1++){
    taxonStrSS1 = mSSDataset->getCharacters( taxon1,false);
    taxonStrSS2 = mRefSSDataset->getCharacters( taxon1,false);
    
    /* for every column of the sequence */
    for( int col = 0; col < minNChars; col++){
      score += subMatSS->get( taxonStrSS1[col], taxonStrSS2[col]);
      if( isnan( score)){
	char str[1024];
	sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
	throw PsodaError( str);
      }
      counter++;
    }
  }
  /* normalize */
  score /= counter;
  if( isnan( score)){
    char str[1024];
    sprintf(str, "ERROR: score = %lf (nan) (counter = %d)!", score, counter);
    throw PsodaError( str);
  }
  
  PsodaPrinter::getInstance()->write("justReferenceSSSumOfPairs(%s) = %f\n", subMat->getName().c_str(), score );
  
  return score;
} /* END referenceSSSumOfPairs() */

