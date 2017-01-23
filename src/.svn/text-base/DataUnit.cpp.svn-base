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
#include "DataUnit.h"
#include "Dataset.h"
#include "PsodaError.h"

//#include <fstream>
//#include <errno.h>

using namespace std;

int dataUnitStr2Num(string dataUnit, Dataset::Datatype datatype){
  /* future: error checking */
  if( datatype == Dataset::CODON_DATATYPE){
    /* future: merge with Dataset */
    if( dataUnit.length() != 3){
      char str[1024];
      sprintf( str, "Error: dataUnitStr2Num(%s, %d), string has %d characters instead of 3 (for codons)", dataUnit.c_str(), datatype, (int)dataUnit.length());
      throw PsodaError(str);
    }
    if( dataUnit[0] == '-'){
      return GAP_LOC;
    }else{
      /* modeled after Dataset.h::convertCodon() */
      int codonNum = 0;
      for( int i = 0; i < CHARS_PER_CODON; i++){
	char codonChar;
	
	switch( toupper( dataUnit[i])){
	case 'A': codonChar = A_NUM; break;
	case 'C': codonChar = C_NUM; break;
	case 'G': codonChar = G_NUM; break;
	case 'T': codonChar = T_NUM; break;
	default:
	  char str[1024];
	  sprintf( str, "Error: dataUnitStr2Num(%s, %d) found non-nucleotide character: %c", dataUnit.c_str(), datatype, dataUnit[i]);
	  throw PsodaError(str);
	};

	codonNum |= codonChar << (i * 2);
	//fprintf( stderr, "dataUnitStr2Num(%s, %d): %d: codonChar = %d, codonNum = %d\n", dataUnit.c_str(), datatype, i, codonChar, codonNum);
      }
      return codonNum;
    }
  }else{
    return (char)dataUnit[0];
  }
}


int dataUnitNum2OtherCase(int dataUnit, Dataset::Datatype datatype){

  if( datatype == Dataset::CODON_DATATYPE){
    return dataUnit;
  }else{
    if( dataUnit >= 'a' &&
	dataUnit <= 'z'){
      return dataUnit - ((int)'a' - (int)'A');
    }else{
      return dataUnit + ((int)'a' - (int)'A');
    }
  }
}


string dataUnitNum2String(int dataUnitNum, Dataset::Datatype datatype){
  char buffer[4];
  string s;
  
  if( datatype == Dataset::CODON_DATATYPE){
    /* future: merge with Dataset */
    /* modeled after Dataset::setGeneticCode() */
    
    //fprintf( stderr, "dataUnitNum2String(%d, %d):\n", dataUnitNum, datatype);
      
    buffer[4] = '\0';
    
    for( int pos = 0; pos < CHARS_PER_CODON; pos++){
      int codonNum = (dataUnitNum >> (pos*BITS_PER_CHAR)) & CHAR_MASK;
	
      switch( codonNum){
	case A_NUM: buffer[pos] = 'A'; break;
	case C_NUM: buffer[pos] = 'C'; break;
	case G_NUM: buffer[pos] = 'G'; break;
	case T_NUM: buffer[pos] = 'T'; break;
	default:
	  char str[1024];
	  sprintf( str, "Error: dataUnitNum(%d, %d) found non-nucleotide character: %d (decimal)", dataUnitNum, datatype, codonNum);
	  throw PsodaError(str);
	};

      //fprintf( stderr, "dataUnitNum2String(%d, %d): pos: %d, codonNum = %d, buffer = %s\n", dataUnitNum, datatype, pos, codonNum, buffer);
      
    }
  }else{
    buffer[0] = dataUnitNum;
    buffer[1] = '\0';
  }
  s.assign(buffer);

  return s;
}

