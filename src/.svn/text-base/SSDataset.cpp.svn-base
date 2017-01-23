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
#include "SSDataset.h"
#include "PsodaPrinter.h"
#include "Globals.h"
#include <assert.h>

#include <fstream> /* DEBUGGING */

#undef PADDING




const int SSDataset::translateSSASCII2Num[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SS_CHAR_GAP_INDEX,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SS_CHAR_GAP_CODONS_INDEX,0,0,0,0,SS_CHAR_BETA_STRAND_INDEX,0,0,SS_CHAR_HELIX_INDEX,0,0,0,SS_CHAR_LOOP_INDEX,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SS_CHAR_BETA_STRAND_INDEX,0,0,SS_CHAR_HELIX_INDEX,0,0,0,SS_CHAR_LOOP_INDEX,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const char SSDataset::translateSSNum2ASCII2[SS_CHAR_NUMBER_OF_INDICES] = {
  SS_CHAR_HELIX,
  SS_CHAR_BETA_STRAND,
  SS_CHAR_LOOP,
  SS_CHAR_GAP,
  SS_CHAR_GAP_CODONS};




SSDataset::SSDataset()
  : Dataset()
{}

SSDataset::SSDataset( const SSDataset& orig )
  : Dataset( orig)
{}

SSDataset::~SSDataset()
{
}

SSDataset& SSDataset::operator=(const SSDataset& rhs)
{
  Dataset::operator=(rhs);
  return *this;
}




void SSDataset::convertTo3State(){
  char* convertTable = new char[ ASCII_TABLE_SIZE];

  for( int i = 0; i < ASCII_TABLE_SIZE; i++){
    convertTable[i] = UNDEFINED_SECONDARY_STRUCTURE;
  }
  
  convertTable[(int)'A'] = convertTable[(int)'a'] = SS_CHAR_HELIX;
  convertTable[(int)'B'] = convertTable[(int)'b'] = SS_CHAR_BETA_STRAND;
  convertTable[(int)'C'] = convertTable[(int)'c'] = SS_CHAR_LOOP;
  convertTable[(int)'E'] = convertTable[(int)'e'] = SS_CHAR_BETA_STRAND;
  convertTable[(int)'G'] = convertTable[(int)'g'] = SS_CHAR_HELIX;
  convertTable[(int)'H'] = convertTable[(int)'h'] = SS_CHAR_HELIX;
  convertTable[(int)'I'] = convertTable[(int)'i'] = SS_CHAR_HELIX;
  convertTable[(int)'L'] = convertTable[(int)'l'] = SS_CHAR_LOOP;
  convertTable[(int)'S'] = convertTable[(int)'s'] = SS_CHAR_LOOP;
  convertTable[(int)'T'] = convertTable[(int)'t'] = SS_CHAR_LOOP;
  convertTable[(int)' ']                          = SS_CHAR_LOOP;
  convertTable[(int)'-']                          = SS_CHAR_GAP;
  
  char ssChar;

	/*Create mMatrix from mASCIIData in Dataset*/
  mNCharsAlloced = mNCharacters+1;
  mMatrix = (char*)malloc(sizeof(char)*mNCharsAlloced*mNTaxa);
  mBinaryData = (char**)malloc(sizeof(char*)*mNTaxa);
  for(int i=0;i<mNTaxa;i++)
  {
	mBinaryData[i] = (char*)malloc(sizeof(char)*mNCharsAlloced);
	for(int j=0;j<mNCharacters;j++)
	{
		mMatrix[i*mNCharsAlloced+j] = mASCIIData[i][j];
		mBinaryData[i][j] = 0;
	}
	mBinaryData[i][mNCharsAlloced] = 0;
	mMatrix[(i+1)*mNCharsAlloced-1] = 0;
  }

  for( int i = 0; i < mNTaxa; i++){
    for( int j = 0; j < getSeqLen(i); j++){
      ssChar = convertTable[ (int)ACCESS_ARRAY(mMatrix, i, j, mNCharsAlloced)];
	  mBinaryData[i][j] = ssChar;
      if( ssChar == UNDEFINED_SECONDARY_STRUCTURE){
	char str[1024];
	sprintf( str, "Unrecognized secondary structure '%c' (detected in convertTo3State())", ACCESS_ARRAY(mMatrix, i, j, mNCharsAlloced));
	throw PsodaError( str);
      }
      ACCESS_ARRAY(mMatrix, i, j, mNCharsAlloced) = ssChar;
    }
	//printf("%dth SS: %s\n",i,mBinaryData[i]);
  }
  mSequenceChanged = false;
  delete[] convertTable;
}


bool SSDataset::printCombinedSeqs(string type, string filename, Dataset* dataset){

  assert( dataset);
  if( matches( dataset) == false){
    throw PsodaError("Error: Asked to print combined secondary structure output alignment, but the two datasets do not match");
  }

  bool converted = false;
  if( mDatatype == CODON_DATATYPE){
    convertCodonsToDNA();
    converted = true;
  }
  
  if( filename == "none"){
    return false;
  }

  if( type == "UNKNOWN"){
    fprintf( stderr, "ERROR: Unknown format for printing seqences\n");
    return false;
  }


  //char* outputFileName, char** seqs, int numSeqs, int maxChars, char** seqsNames, enum formatType type){
  FILE* fp;
   int i = 0;

   if( filename == "stdout" ||
       filename == "STDOUT" ||
       filename == ""){
     fp = stdout;
   }else if( filename == "stderr" ||
       filename == "STDERR"){
     fp = stderr;
   }else{
     fp = fopen( filename.c_str(), "w");
     if( ! fp){
       char str[1024];
       sprintf( str, "Error: opening output sequence file \"%s\" (errno = %d(%s))\n", filename.c_str(), errno, strerror(errno));
       throw PsodaError(str);
     }
   }

  
  /* phylip format */
  if( type == "phylip" ||
      type == "PHYLIP"){
    fprintf( fp, "%d %d\n", mNTaxa, mNCharacters);
    for( i = 0; i < mNTaxa; i++){
      fprintf( fp, "%-20s %s\n", getTaxonName(i), getCharacters(i,false));
      fprintf( fp, "%-20s %s\n", getTaxonName(i), dataset->getCharacters(dataset->getTaxonNumber( getTaxonName(i)),false));
    }
    //fprintf( fp, "\n");
  }
  if( type == "phylip interleaved" ||
      type == "PHYLIP INTERLEAVED"){
    printf("PHYLIP INTERLEAVED - Not Yet Implemented\n");
    //printSeqsPhylipInterleaved( fp, seqs, numSeqs, maxChars, seqsNames);
  }
  /* fasta format */
  else if( type == "fasta" ||
	   type == "FASTA"){
    for( i = 0; i < mNTaxa; i++){
      fprintf( fp, ">%s\n%s\n%s\n", getTaxonName(i), getCharacters(i,false), dataset->getCharacters(dataset->getTaxonNumber( getTaxonName(i)),false));
    }
    //fprintf( fp, "\n");
  }
  /* NEXUS format */
  else if( type == "nexus" ||
	   type == "NEXUS"){
    fprintf( fp, "NEXUS format not entirely supported yet\n");
    fprintf( stderr, "NEXUS format not entirely supported yet\n");
    return false;
    /*     for( i = 0; i < numSeqs; i++){ */
    /*       fprintf( fp, ">%s\n%s\n", seqsNames[i], seqs[i]); */
    /*     } */
    /*     fprintf( fp, "%s", "\n"); */
  }

  
   if( filename != "stdout" &&
       filename != "STDOUT" &&
       filename != "stderr" &&
       filename != "STDERR" &&
       filename != ""){
     fclose( fp);
   }


   if( converted == true){
     convertDNAToCodons();
   }
   
   return 0;
} /* END printCombinedSeqs() */

  void SSDataset::convertDNAToCodons()
  {
	printf("SSDataset::convertDNAToCodons not implemented\n");
	}
  
  void SSDataset::convertCodonsToDNA()
 {
 printf("SSDataset::convertCodonsToDNA not implemented\n");
 }