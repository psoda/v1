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
#include "SubMat.h"
#include "DataUnit.h"
#include "PsodaError.h"
#include "Globals.h"  /* for DEBUG */
#include "defaultSubMats.h"
#include <fstream>
#include <errno.h>
#include <cctype>
#include <string.h>


#define DEFAULT_SEPERATOR        '\t'  /* tab character */

using namespace std;

/** Constuctor
 * \param filename file name or description of substitution matrix.  Several keywords indicate hard-coded sub. mats (e.g., BLOSUM62).
 * \param dataset current dataset (used to determine the datatype).
 * \param setGapPenaltiesToLowest if set, look for the lowest values in the substitution matrix to set to the gap penalty.
 */
SubMat::SubMat( string fileName, Dataset* dataset, bool setGapPenaltiesToLowest) :
  subMatDescription( fileName),
  mDatatype( dataset->datatype()),
  mGapChar( dataset->gapchar()),
  mSetGapPenaltiesToLowest( setGapPenaltiesToLowest)  
{
  if( PSODA_DEBUG){ cerr << "HY: SubMat(" << subMatDescription.c_str() << ", " << mDatatype << ")" << endl; }

  /* check for a hard-coded substition matrics */
  if( subMatDescription == "IUB" ||
      subMatDescription == "iub"){
    loadDefaultSubMatrix( "IUB");
  }else if( subMatDescription == "BLOSUM62" ||
	    subMatDescription == "blosum62"){
    loadDefaultSubMatrix( "BLOSUM62");
  }else if( subMatDescription == "SCHNEIDER" ||
	    subMatDescription == "schneider"){
    loadDefaultSubMatrix( "SCHNEIDER");
  }else if( subMatDescription == "LuthyA" ||
	    subMatDescription == "LüthyA" ||
	    subMatDescription == "luthya"){
    loadDefaultSubMatrix( "LuthyA");
  }else if( subMatDescription == "LuthyB" ||
	    subMatDescription == "LüthyB" ||
	    subMatDescription == "luthyb"){
    loadDefaultSubMatrix( "LuthyB");
  }else if( subMatDescription == "LuthyL" ||
	    subMatDescription == "LüthyL" ||
	    subMatDescription == "luthyl"){
    loadDefaultSubMatrix( "LuthyL");
  }else if( subMatDescription == "identity"){
    loadIdentitySubMatrix();
  }else if( subMatDescription == "identity-negative"){
    loadIdentityNegativeSubMatrix();
  }else if( subMatDescription == ""){
    if( mDatatype == Dataset::NUCLEOTIDE_DATATYPE ||
	mDatatype == Dataset::DNA_DATATYPE ||
	/* mDatatype == Dataset::STANDARD_DATATYPE || */
	mDatatype == Dataset::RNA_DATATYPE){
      loadDefaultSubMatrix( "IUB");
    }else if( mDatatype == Dataset::PROTEIN_DATATYPE){
      loadDefaultSubMatrix( "BLOSUM62");
    }else if( mDatatype == Dataset::CODON_DATATYPE){
      /*
       * future: replace with something more intelligent
       */
      //loadIdentitySubMatrix();
      loadDefaultSubMatrix( "SCHNEIDER");
    }else{
      throw PsodaError( "\nUnrecognized datatype for a \"default\" substitution matrix!\n\n");
    }
  }else{
    readSubMatrixFile( subMatDescription);
  }

  if( PSODA_DEBUG > 11){
    formatForDefaultSubMats();
  }
}


SubMat::~SubMat(){}

/**
 * Returns the value in the substitution matrix for the entries \a from and \a to.
 */
double SubMat::get(int from, int to){
  if( from >= NUM_ASCII_VALUES ||
      to >= NUM_ASCII_VALUES){
    cerr << "ERROR: Invalid indicies for a SubMat substitution matrix!" << endl;
    // fail or return something?
  }
  return subMat[from][to];
}



/**
 * Open and parse a substitution matrix from a file.
 * \param matrixFileName file name of substitution matrix.
 * NOTE: assumes that if "*" columns and rows exists that they are the last ones respectively
 */
bool SubMat::readSubMatrixFile( string matrixFileName){
  //char seperator = '\t';
  int residueTranslations[NUM_ASCII_VALUES];
  int residueTranslationsOther[NUM_ASCII_VALUES];
  //char line[LINE_LEN];
  int residue1, residue2;
  int residuesFound = 0;
  //int upperToLowerDiff = (int)'a' - (int)'A';

  vector<string> tokens;

//  if( seperator == '\0'){
//    seperator = DEFAULT_SEPERATOR;
//  }
//
  ifstream matrixFile( matrixFileName.c_str());
  
  if( ! matrixFile){
    cerr << "\nError: could not open substitution matrix file \"" << matrixFileName << "\" (errno = " << errno << "(" << strerror(errno) << "))\n" << endl;
    exit(-1);
  }

  /* initialize residue translation array */
  for( residue1 = 0; residue1 < NUM_ASCII_VALUES; residue1++){
    residueTranslations[residue1] = -1;
    residueTranslationsOther[residue1] = -1;
  }

  /* "zero" out matrix2D */
  for( residue1 = 0; residue1 < NUM_ASCII_VALUES; residue1++){
    for( residue2 = 0; residue2 < NUM_ASCII_VALUES; residue2++){
      subMat[residue1][residue2] = 0.0;
    }
  }
  
  string line;
  int skippedColumns = 0;
  /*
   * get header
   */
  
  /* skip comment lines */
  while( getline(matrixFile, line) && ( line.compare(0, 1, "#") == 0)){
    //cout << "Skipping sub. mat. line " << line << endl;
  }

  if( line.empty()){
    cerr << "Error reading even the residue column headers from \"" << matrixFileName << "\"!" << endl;
    exit( -1);
  }

  tokenizeString((const string)line, tokens);
  
  vector<string>::iterator iter;
  
  for(iter = tokens.begin(); iter != tokens.end(); iter++){
    if( *iter == "*"){
      skippedColumns++;
      continue;
    }
    residueTranslations[residuesFound] = dataUnitStr2Num( *iter, mDatatype);
    
    if( residueTranslations[residuesFound] == -1){
        cerr << "ERROR: Could not parse the dataUnit \"" << *iter << "\" (dataUnitStr2Num() returned "<< residueTranslations[residuesFound] << ")!\n";
	exit( -1);
    }
    residueTranslationsOther[ residuesFound] = dataUnitNum2OtherCase( residueTranslations[ residuesFound], mDatatype);
    
    //cout << "DEBUG: dataUnitStr2Num(" << *iter << ", " << mDatatype << ") = " << residueTranslations[residuesFound] << ")\n";
    
    residuesFound++;
  }
  
  if( residuesFound >= NUM_ASCII_VALUES){
    cerr << "ERROR, found " << residuesFound << " residues, but the size of the substitution matrix is only " << NUM_ASCII_VALUES << endl;
    exit( -1);
  }


  /*
   * read substitution matrix one line at a time
   */
  for( residue1 = 0; residue1 < residuesFound; residue1++){
    //fprintf( stderr, "DEBUG: residueTranslations[%d] =  %s\n", residue1, dataUnitNum2String( residueTranslations[residue1], mDatatype));
   }


  /* read substitution matrix one line at a time */
  for( residue1 = 0; residue1 < residuesFound; residue1++){
    if( getline(matrixFile, line) == 0){
      cerr << "ERROR: Reading row #" << residue1 + 1 << " of " << residuesFound << " in the substitution matrix file  \"" << matrixFileName << "\" (errno = " << errno << "(" << strerror(errno) << "))" << endl;
      exit( -1);
    }

    tokenizeString((const string)line, tokens);

    if( tokens.size() - 1 != (unsigned int) residuesFound +  skippedColumns){
       cerr << "ERROR: mal-formed substitution matrix row label: \"" << line << "\" (found " << tokens.size() - 1 << " entries (instead of " << residuesFound +  skippedColumns << "))\n";
       exit(-1);
     }
    
    vector<string>::iterator iter;
    iter = tokens.begin();
    //cerr << "DEBUG: iter = \"" << *iter << "\"" << endl;
    if( *iter == "*"){
      continue;
    }else if( residueTranslations[residue1] != dataUnitStr2Num( *iter, mDatatype)){
      cerr << "ERROR: mal-formed substitution matrix row label \""<< *iter<< "\" (expecting \""<<  residueTranslations[residue1] << "\" (line = \"" << line << "\")\n";
      exit(-1);
    }
    
    iter++;
    int residue2 = 0;
    float value;
    for(; iter != tokens.end(); iter++){
      value = atof( (*iter).c_str());
      subMat[ residueTranslations[ residue1]][ residueTranslations[ residue2]] = value;
      subMat[ residueTranslations[ residue1]][ residueTranslationsOther[ residue2]] = value;
      subMat[ residueTranslationsOther[ residue1]][ residueTranslations[ residue2]] = value;
      subMat[ residueTranslationsOther[ residue1]][ residueTranslationsOther[ residue2]] = value;
      //cerr << "DEBUG: assigning \"" << *iter << "\" to ({"<< dataUnitNum2String( residueTranslations[ residue1], mDatatype) << "," << dataUnitNum2String( residueTranslationsOther[ residue1], mDatatype) << "},{"<< dataUnitNum2String( residueTranslations[ residue2], mDatatype) << "," << dataUnitNum2String( residueTranslationsOther[ residue2], mDatatype) << "})" << endl;
      residue2++;
    }
  }

  if( PSODA_DEBUG >= 9){
    cout << "subMat:"<<endl;
    printMatrix();
  }

  if( mSetGapPenaltiesToLowest){
    setGapPenalties();
  }
  
  return true;

} /* END readSubMatrixFile() */


/**
 * Helper function to tokenize strings.
 * \param str string to tokenize.
 * \param tokens found tokens are added to this vector.
 * \param delimiters characters to seperate tokens.
 */
void SubMat::tokenizeString(const string& str,
			    vector<string>& tokens,
			    const string& delimiters){
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    tokens.clear();
    
    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

/**
 * Write the substitution matrix to STDOUT.
 */
void SubMat::printMatrix(){
  int residue1, residue2;
  cout << " \t";
  for( residue2 = 0; residue2 < NUM_ASCII_VALUES; residue2++){
    if( residue2 >= 'A'){
      cout << (char) residue2 << "\t";
    }else{
      cout << " \t";
    }
  }
  cout << endl;
  
  for( residue1 = 0; residue1 < NUM_ASCII_VALUES; residue1++){
    if( residue1 >= 'A'){
      cout << (char) residue1 << "\t";
    }else{
      cout << " \t";
    }
    for( residue2 = 0; residue2 < NUM_ASCII_VALUES; residue2++){
      cout << subMat[residue1][residue2] << "\t";
    }
    cout << endl;
  }
}

/**
 * Write the substitution matrix to STDOUT in the format for a .h file to be used for hard-coded matrics.
 */
void SubMat::formatForDefaultSubMats(){
  int residue1, residue2;
  
  fprintf(stderr, "\nextern const double %s[NUM_ASCII_VALUES][NUM_ASCII_VALUES] = {\n", getName().c_str());
	  
  fprintf(stderr, "%s", "/*\t");
  for( residue2 = 0; residue2 < NUM_ASCII_VALUES; residue2++){
    fprintf(stderr, ",%c", residue2);
    if( residue2 == mGapChar){
      fprintf(stderr, "%s", "(gap)");
    }
  }
  fprintf(stderr, "%s", " */\n");
  
  for( residue1 = 0; residue1 < NUM_ASCII_VALUES; residue1++){
    fprintf(stderr, "/* %c*/ {", residue1);
    for( residue2 = 0; residue2 < NUM_ASCII_VALUES; residue2++){
      fprintf(stderr, ",%f", subMat[residue1][residue2]);
    }
    fprintf(stderr, "%s", "}");
    if( residue1 < NUM_ASCII_VALUES - 1){
      fprintf(stderr, "%s", ",");
    }
    fprintf(stderr, "%s", "\n");
  }
  fprintf(stderr, "%s", "};\n\n");
}

/**
 * Set substitution matrix to be the identity matrix (i.e., if two characters then = 1, otherwise 0)
 */
void SubMat::loadIdentitySubMatrix(){
  for( int i = 0; i < NUM_ASCII_VALUES; i++){
    for( int j = 0; j < NUM_ASCII_VALUES; j++){
      //subMat[i][j] = -1.0;
      subMat[i][j] = 0.0;
    }
    //if( i == '-') {fprintf(stderr, "%c(%d) tolower: %c(%d), toupper: %c(%d)\n", i, i, tolower(i), tolower(i), toupper(i), toupper(i));}
    subMat[i][tolower((char)i)] = 1.0;
    subMat[i][toupper((char)i)] = 1.0;
  }
}

/**
 * Set substitution matrix to be the identity matrix (i.e., if two characters then = 1, otherwise -1)
 */
void SubMat::loadIdentityNegativeSubMatrix(){
  for( int i = 0; i < NUM_ASCII_VALUES; i++){
    for( int j = 0; j < NUM_ASCII_VALUES; j++){
      subMat[i][j] = -1.0;
    }
    //fprintf(stderr, "%c(%d) tolower: %c(%d), toupper: %c(%d)\n", i, i, tolower(i), tolower(i), toupper(i), toupper(i));
    subMat[i][tolower((char)i)] = 1.0;
    subMat[i][toupper((char)i)] = 1.0;
  }
}


/**
 * Set the substitution matrix to one of the hard-coded matrices
 */
void SubMat::loadDefaultSubMatrix( string defaultMatrix){
  subMatDescription = defaultMatrix;
  
  if( subMatDescription == "IUB"){
    memcpy(subMat, iub, sizeof(double) * NUM_ASCII_VALUES * NUM_ASCII_VALUES);
  }else if( subMatDescription == "BLOSUM62"){
    memcpy(subMat, blosum62, sizeof(double) * NUM_ASCII_VALUES * NUM_ASCII_VALUES);
  }else if( subMatDescription == "SCHNEIDER"){
    memcpy(subMat, schneider, sizeof(double) * NUM_ASCII_VALUES * NUM_ASCII_VALUES);
  }else if( subMatDescription == "LuthyA"){
    memcpy(subMat, luthya, sizeof(double) * NUM_ASCII_VALUES * NUM_ASCII_VALUES);
  }else if( subMatDescription == "LuthyB"){
    memcpy(subMat, luthyb, sizeof(double) * NUM_ASCII_VALUES * NUM_ASCII_VALUES);
  }else if( subMatDescription == "LuthyL"){
    memcpy(subMat, luthyl, sizeof(double) * NUM_ASCII_VALUES * NUM_ASCII_VALUES);
  }else{
      throw PsodaError( "\nUnrecognized default substitution matrix \"defaultMatrix\"!\n\n");
  }
  
  if( mSetGapPenaltiesToLowest){
    setGapPenalties();
  }
}

/**
 * Sets gap entries of the substitution matrix to the lowest values in the matrix.
 */
void SubMat::setGapPenalties(){
  double lowest = subMat[0][0];
  for( int i = 0; i < NUM_ASCII_VALUES; i++){
    for( int j = 0; j < NUM_ASCII_VALUES; j++){
      if( subMat[i][j] < lowest){
	lowest = subMat[i][j];
      }
    }
  }


  if( PSODA_DEBUG > 9){ fprintf(stderr, "SubMat(): lowest = %lf\n", lowest); }
  
  for( int i = 0; i < NUM_ASCII_VALUES; i++){
    subMat[mGapChar][i] = lowest;
    subMat[i][mGapChar] = lowest;
  }

  subMat[mGapChar][mGapChar] = 0.0;  /* lowest? */
}
