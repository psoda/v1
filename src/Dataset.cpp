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
#include "Dataset.h"
#include "Globals.h"
#include "PsodaPrinter.h"

#undef VERBOSE_DATASET							  /*Turns debug statments on and off*/

/**
 * CONSTRUCTORS AND DESTRUCTORS
 */

Dataset::Dataset(BlockType blockType):
mNTaxa(0),
mNCharacters(0),

mGapChar('-'),
mMissingChar('?'),
mMatchChar(0),
mEndChar('\0'),

mDatatype(UNDEFINED_DATATYPE),
mDataFormat(UNDEFINED_DATAFORMAT),
mBlockType(blockType),

mTaxonNames(),
mWeights(NULL),
mWeightsBak(NULL),

mSequenceChanged(false),
mCurSequence(0),
mASCIIData(NULL),
mBinaryData(NULL),
mCodonData(NULL)
{}

Dataset::~Dataset()
{
	int i;
	if(mBinaryData != NULL)
	{
		for(i=0;i<mNTaxa;i++)
			if(mBinaryData[i] != NULL)
				free(mBinaryData[i]);
		free(mBinaryData);
	}
	if(mASCIIData != NULL)
	{
		for(i=0;i<mNTaxa;i++)
			if(mASCIIData[i] != NULL)
				free(mASCIIData[i]);
		free(mASCIIData);
	}
	if(mWeights != NULL)
		free(mWeights);
	if(mWeightsBak != NULL)
		free(mWeightsBak);
}

/**
 * METHODS FOR CONVERTING BETWEEN TAXON NAME AND NUMBER
 */

/**
 * Returns a pointer to the character string of the taxon taxonNumber
 */

const char* Dataset::getTaxonName( int taxonNumber )
{
	if( taxonNumber >= mNTaxa)
	{
		return NULL;
	}
	return mTaxonNames.at(taxonNumber).data();
}


/**
 * Returns the taxon number (ID) for taxonName
 */
int Dataset::getTaxonNumber(const char *taxonName)
{
	for (int i = 0; i < mNTaxa; i++)
	{
		if (strcmp(taxonName, mTaxonNames.at(i).c_str()) == 0)
			return i;
	}
	return -1;
}

vector<string>& Dataset::taxonNames(){
  return mTaxonNames;
}

/**
 * METHODS FOR BUILDING A DATASET
 */

/** Set the number of taxa
 * Utilizes resizeMatrix() and updates mCurrentTaxaIndex (used while parsing an input file) (DEPRECATED)
 */
void Dataset::setntaxa( int ntaxa )
{
//mNTaxa = ntaxa;
//printf("Dataset::setntaxa is a no-op\n");
}


/**
 * Set the (largest) number of characters for any taxon (DEPRECATED)
 */
void Dataset::setnchars( int ncharacters)
{
	mNCharacters = ncharacters;
}


/**
 *  Adds a taxa name to the dataset
 */

void Dataset::addName( const char* taxonName)
{
	mTaxonNames.push_back(taxonName);
//check to see if mASCIIData has been alocated and if so adjust it's size
	if(mASCIIData != NULL)
	{
		char** temp = (char**)malloc(sizeof(char*)*(mNTaxa+1));
		for(int i=0;i<mNTaxa;i++)
			temp[i] = mASCIIData[i];
		temp[mNTaxa] = NULL;
		free(mASCIIData);
		mASCIIData = temp;
	}
//clear mBinaryData as it is out of date
	if(mBinaryData != NULL)
	{
		for(int i=0;i<mNTaxa;i++)
			if(mBinaryData[i] != NULL)
				free(mBinaryData[i]);
		free(mBinaryData);
		mBinaryData = NULL;
		mSequenceChanged = true;
	}
	mNTaxa++;

#ifdef VERBOSE_DATASET
	printf("Added %s, taxa %d\n",taxonName,mNTaxa);
#endif
}


/**
 * Adds characters to the end of the current ASCII sequence for taxon pos
 *
 * A better name would be appendCharacters
 */

void Dataset::addCharacters( const char* taxonCharacters, int pos)
{
	mSequenceChanged = true;
	if (pos == -1)
	{
//set position to the current position and advance the current sequence
		mCurSequence%=mNTaxa;
		pos = mCurSequence;
		mCurSequence++;

	}
	if((pos > mNTaxa)||(pos < 0))
	{
		return;
	}
	if(mASCIIData == NULL)
	{
		mASCIIData = (char**)malloc(sizeof(char*)*mNTaxa);
//Initialize all elements to NULL
		for(int i=0;i<mNTaxa;i++)
			mASCIIData[i] = NULL;
	}
	if(mASCIIData[pos] == NULL)
	{
		mASCIIData[pos] = strdup(taxonCharacters);
	}
	else
	{
		char* temp = (char*)malloc(sizeof(char)*(strlen(mASCIIData[pos])+strlen(taxonCharacters)+1));
		memcpy(temp,mASCIIData[pos],strlen(mASCIIData[pos]) +1);
		strcat(temp,taxonCharacters);
		free(mASCIIData[pos]);
		mASCIIData[pos] = temp;
	}
	int nchar=strlen(mASCIIData[pos]);
	if(nchar > mNCharacters)
		mNCharacters = nchar;

//Replace match characters
	for(int i=0;i<nchar;i++)
		if(mASCIIData[pos][i] == mMatchChar)
			mASCIIData[pos][i] = mASCIIData[0][i];
#ifdef VERBOSE_DATASET
	printf("Added to taxon %d: %s\ntaxon%d=%s\n",pos,taxonCharacters,pos,mASCIIData[pos]);
#endif
}


/**
 * Insert a single character at column col for taxon number pos
 *
 * A better name would be insertCharacter
 */

void Dataset::appendACharacter( const char character, int col, int pos)
{
	mSequenceChanged = true;
	printf("Dataset::appendACharacter not implemented\n");
}


/**
 * Replace the character in the col column of taxon number pos with character
 */
void Dataset::setACharacter(char character, int col, int pos)
{
	mSequenceChanged = true;
	printf("Dataset::setACharacter not implemented\n");
}


/**
 * GETTERS AND SETTERS
 */

/**
 * Return a pointer to the weights arrays
 */
int* Dataset::weights() const
{
	if (mDataFormat == ALIGNED_DATAFORMAT)
	{
		return mWeights;
	}
	else
	{
//PsodaPrinter::getInstance()->write("\nALERT: Attempting to get the weights for unaligned data\n\n");
		return NULL;
	}
}


/* Note: doesn't actually reset the weights, justs copies them for certain scenarios */
void Dataset::resetWeights()
{
	if (mDataFormat == ALIGNED_DATAFORMAT)
	{
		if( mWeightsBak != NULL)
		{
			memcpy(mWeights, mWeightsBak, mNCharacters * sizeof(int));
		}
	}
	else
	{
//PsodaPrinter::getInstance()->write("ALERT: Attempting to reset weights for unaligned data\n");
	}
}

void Dataset::randomizeWeights(double percent)
{
	if (mDataFormat == ALIGNED_DATAFORMAT)
	{
		for(int i=0;i<mNCharacters;i++)
		{
			if((double)rand()/INT_MAX < percent)
				mWeights[i]++;  //increase weight by one on percent of the characters
		}
	}
}


/**
 * Returns the number of characters:
 *	For aligned data, it is the length of all of the sequences
 *	For unaligned data, it is the length of the longest sequence
 */
int Dataset::nchars() const
{
	return mNCharacters;
}


/**
 * Returns the number of sequences
 */
int Dataset::ntaxa()     const
{
	return mNTaxa;
}


/**
 * Getter function for gap character
 */
char Dataset::gapchar() const
{
	return mGapChar;
}


/**
 * Setter function for gap character
 */
char& Dataset::gapchar()
{
	return mGapChar;
}


/**
 * Getter function for missing character
 */
char Dataset::missingchar() const
{
	return mMissingChar;
}


/**
 * Setter function for missing character
 */
char& Dataset::missingchar()
{
	return mMissingChar;
}


/**
 * Getter function for match character
 */
char Dataset::matchchar() const
{
	return mMatchChar;
}


/**
 * Setter function for match character
 */
char& Dataset::matchchar()
{
	return mMatchChar;
}


/**
 * Getter function for end character
 */
char Dataset::endchar() const
{
	return mEndChar;
}


/**
 * Getter function for dataformat (aligned/unaligned)
 */
Dataset::DataFormat Dataset::dataformat() const
{
	return mDataFormat;
}


/**
 * Setter function for dataformat (aligned/unaligned)
 */
Dataset::DataFormat& Dataset::dataformat()
{
	return mDataFormat;
}


/**
 * Getter function for datatype
 */
Dataset::Datatype Dataset::datatype() const
{
	return mDatatype;
}


/**
 * Setter function for datatype
 */
Dataset::Datatype& Dataset::datatype()
{
	return mDatatype;
}


/**
 * Getter function for blocktype
 */
BlockType Dataset::blocktype() const
{
	return mBlockType;
}


/**
 * Setter function for blocktype
 */
BlockType& Dataset::blocktype()
{
	return mBlockType;
}


/**
 * Returns the sequence for the given taxonNumber in binary or ASCII format
 */
char* Dataset::getCharacters( int taxonNumber, bool binary )
{
	if(binary)
	{
		if(mSequenceChanged) calculateCompressedBinaryMatrix();
		return mBinaryData[taxonNumber];
	}
	else
	{
/* It may be best to return a copy of mASCIIData instead of a direct pointer*/
		return mASCIIData[taxonNumber];
	}
}


/**
 * Returns the colth character for taxon taxonNumber
 */
char Dataset::getCharacter( int taxonNumber, int col ) const
{
	return mASCIIData[taxonNumber][col];
}


/**
 * UTILITY FUNCTIONS
 */

/** Replaces dataset encoded as DNA with encoding using codons
 */
void Dataset::convertDNAToCodons()
{

	int i;
	if( mDatatype != PROTEIN_CODING_DNA_DATATYPE)
	{
		return;
	}

//free mCodonData
	if(mCodonData)
	{
		for(i=0;i<mNTaxa;i++)
			if(mCodonData[i])
				free(mCodonData[i]);
		free(mCodonData);
	}

	int codonNCharacters = (int)(mNCharacters / CHARS_PER_CODON);

//allocate mCodonData
	mCodonData = (char**)malloc(sizeof(char*)*mNTaxa);
	for(i=0;i<mNTaxa;i++)
		mCodonData[i] = (char*)malloc(sizeof(char)*(codonNCharacters+1));

//convert DNA to codons
	for( i = 0; i < mNTaxa; i++)
	{
		for(int j=0;j<codonNCharacters;j++)
		{
			if(mASCIIData[i][j*CHARS_PER_CODON] == '\0') break;
			if(mASCIIData[i][j*CHARS_PER_CODON+1] == '\0') break;
			if(mASCIIData[i][j*CHARS_PER_CODON+2] == '\0') break;
			mCodonData[i][j] = charsToCodon(mASCIIData[i][j*CHARS_PER_CODON],mASCIIData[i][j*CHARS_PER_CODON+1],mASCIIData[i][j*CHARS_PER_CODON+2]);
		}
	}

	mDatatype = CODON_DATATYPE;

}


/**
 *  Return the codon value of the three DNA characters: first, second and third.
 */
char Dataset::charsToCodon(char first, char second, char third)
{
	unsigned char retVal = 0x0;

	const unsigned char T_val = 0x0;
	const unsigned char C_val = 0x1;
	const unsigned char A_val = 0x2;
	const unsigned char G_val = 0x3;

	if( first  == mGapChar &&
		second == mGapChar &&
		third  == mGapChar )
	{
		return GAP_LOC;
	}

	if( first  == '\0' &&
		second == '\0' &&
		third  == '\0' )
	{
		return mEndChar;
	}

	switch(first)
	{
		case 'A':
		case 'a':
			retVal |= A_val << 4;
			break;

		case 'C':
		case 'c':
			retVal |= C_val << 4;
			break;
		case 'G':
		case 'g':
			retVal |= G_val << 4;
			break;
		case 'T':
		case 't':
		case 'U':
		case 'u':
			retVal |= T_val << 4;
			break;
		default:
			PsodaPrinter::getInstance()->write("Unsupported character found while coverting dna to codons (probably an ambiguous character) first: %c\n",first);
	}

	switch(second)
	{
		case 'A':
		case 'a':
			retVal |= A_val << 2;
			break;

		case 'C':
		case 'c':
			retVal |= C_val << 2;
			break;
		case 'G':
		case 'g':
			retVal |= G_val << 2;
			break;
		case 'T':
		case 't':
		case 'U':
		case 'u':
			retVal |= T_val << 2;
			break;
		default:
			PsodaPrinter::getInstance()->write("Unsupported character found while coverting dna to codons (probably an ambiguous character) second: %c\n",second);
	}
	switch(third)
	{
		case 'A':
		case 'a':
			retVal |= A_val;
			break;

		case 'C':
		case 'c':
			retVal |= C_val;
			break;
		case 'G':
		case 'g':
			retVal |= G_val;
			break;
		case 'T':
		case 't':
		case 'U':
		case 'u':
			retVal |= T_val;
			break;
		default:
			PsodaPrinter::getInstance()->write("Unsupported character found while coverting dna to codons (probably an ambiguous character) third: %c\n",third);
	}

	return retVal;
}


/**
 * Replaces dataset encoded as codons with encoding using DNA
 * This code is neither called, nor implemented, but an outline is provided in comments
 */
void Dataset::convertCodonsToDNA()
{
/*
  if( mDatatype != CODON_DATATYPE){
	return;
  }

  int dnaNCharacters = (int)(mNCharacters * CHARS_PER_CODON);
  int newNCharsAlloced = dnaNCharacters + 1;

  char* newMatrix = (char*) check_calloc( mNTaxa * newNCharsAlloced, sizeof( char));

  for( int i = 0; i < mNTaxa; i++){
//fprintf(stderr, "convertCodonsToDNA() A: mSeqLens[%d]: %d\n", i, mSeqLens[i]);
convertCodonSeqToDNA(i, &(ACCESS_ARRAY( newMatrix, i, 0, newNCharsAlloced)));
//fprintf(stderr, "convertCodonsToDNA() B: mSeqLens[%d]: %d\n", i, mSeqLens[i]);
}

setDatatype( PROTEIN_CODING_DNA_DATATYPE);

switchOutMatrix( newMatrix, mNTaxa, dnaNCharacters, mNTaxa, newNCharsAlloced);
*/
}


/**
 * Converts an ASCII character to a binary representation
 */
char Dataset::convertToBinary(char toConvert) const
{
	unsigned char conversionArray[128];
	unsigned char defaultValue = 0x1f;

	memset( conversionArray, defaultValue, 128 );
	conversionArray[(int)'a'] = conversionArray[(int)'A'] = 0x1 << 0 ;
	conversionArray[(int)'c'] = conversionArray[(int)'C'] = 0x1 << 1 ;
	conversionArray[(int)'g'] = conversionArray[(int)'G'] = 0x1 << 2 ;
	conversionArray[(int)'u'] = conversionArray[(int)'U'] = conversionArray[(int)'t'] = conversionArray[(int)'T'] = 0x1 << 3 ;

	conversionArray[(int)'r'] = conversionArray[(int)'R'] = conversionArray[(int)'A'] | conversionArray[(int)'G'];
	conversionArray[(int)'y'] = conversionArray[(int)'Y'] = conversionArray[(int)'C'] | conversionArray[(int)'T'];
	conversionArray[(int)'m'] = conversionArray[(int)'M'] = conversionArray[(int)'A'] | conversionArray[(int)'C'];
	conversionArray[(int)'k'] = conversionArray[(int)'K'] = conversionArray[(int)'G'] | conversionArray[(int)'T'];
	conversionArray[(int)'s'] = conversionArray[(int)'S'] = conversionArray[(int)'C'] | conversionArray[(int)'G'];
	conversionArray[(int)'w'] = conversionArray[(int)'W'] = conversionArray[(int)'A'] | conversionArray[(int)'T'];
	conversionArray[(int)'h'] = conversionArray[(int)'H'] = conversionArray[(int)'A'] | conversionArray[(int)'C'] | conversionArray[(int)'T'];
	conversionArray[(int)'b'] = conversionArray[(int)'B'] = conversionArray[(int)'C'] | conversionArray[(int)'G'] | conversionArray[(int)'T'];
	conversionArray[(int)'v'] = conversionArray[(int)'V'] = conversionArray[(int)'A'] | conversionArray[(int)'C'] | conversionArray[(int)'G'];
	conversionArray[(int)'d'] = conversionArray[(int)'D'] = conversionArray[(int)'A'] | conversionArray[(int)'G'] | conversionArray[(int)'T'];
	conversionArray[(int)'n'] = conversionArray[(int)'N'] = conversionArray[(int)'G'] | conversionArray[(int)'A'] | conversionArray[(int)'T'] | conversionArray[(int)'C'];
	conversionArray[(int)mGapChar] = 0x1f ;		  /* ( 0x1 << 3 ) || ( 0x1 << 2 ) || ( 0x1 << 1 ) || ( 0x1 << 0 ) */
	conversionArray[(int)'?'] = 0x1f ;

	return conversionArray[(int)toConvert];
}


/**
 * Converts a binary representation to an ASCII character
 */
char Dataset::convertToASCII(char toConvert)
{
	switch(toConvert)
	{
		case 0: return '.';
		case 1: return 'A';
		case 2: return 'C';
		case 3: return 'M';
		case 4: return 'G';
		case 5: return 'R';
		case 6: return 'S';
		case 7: return 'V';
		case 8: return 'T';
		case 9: return 'W';
		case 10: return 'Y';
		case 11: return 'H';
		case 12: return 'K';
		case 13: return 'D';
		case 14: return 'B';
		case 15: return 'N';
		case 0x1f: return '-';
		default: return '-';
	}
}


/**
 * Functions like strlen(), except that correctly identifies the end character of the sequences
 */

int Dataset::getLen(const char* str)
{

	int len = 0;
	while( str[len] != mEndChar)
	{
		len++;
	}
	return len;
}


/**
 * Returns the length of the given ASCII sequence, using selected end character for termation rather than '\0'
 */
int Dataset::getSeqLen( int taxonNumber)
{
	return getLen(getCharacters(taxonNumber,false));
}


/**
 * Compares this dataset with otherDataset (in terms of number of taxa and characters, the format, and the taxon names and their order
 */

bool Dataset::matches(Dataset* otherDataset, bool namesOnly)
{
	if( mNTaxa != otherDataset->mNTaxa)
	{
		if( PSODA_DEBUG){ PsodaPrinter::getInstance()->write("Difference in datasets: NTaxa: %d vs %d\n", mNTaxa, otherDataset->mNTaxa); }
		return false;
	}

/* check names (and order) */
	for( int i = 0; i < mNTaxa; i++)
	{
		if( otherDataset->getTaxonNumber( getTaxonName(i)) != i)
		{
			if( PSODA_DEBUG){ PsodaPrinter::getInstance()->write("Difference in datasets: Taxon Name: i: %d, %s vs %s\n", i, getTaxonName(i), otherDataset->getTaxonName(i)); }
			return false;
		}
	}

	if( namesOnly)
	{
		return true;
	}

/* check data format*/
	if( mDataFormat != otherDataset->mDataFormat)
	{
		if( PSODA_DEBUG){ PsodaPrinter::getInstance()->write("Difference in datasets: DataFormat: %d vs %d\n", mDataFormat, otherDataset->mDataFormat); }
		return false;
	}

/* check length of taxa for aligned format*/
	if( mDataFormat == ALIGNED_DATAFORMAT)
	{
		if( mNCharacters != otherDataset->mNCharacters)
		{
			if( PSODA_DEBUG){ PsodaPrinter::getInstance()->write("Difference in datasets: NCharacters: %d vs %d\n", mNCharacters, otherDataset->mNCharacters); }
			return false;
		}
	}
	else
	{
		for( int i = 0; i < mNTaxa; i++)
		{
			if( getSeqLen(i) != otherDataset->getSeqLen(i))
			{
				if( PSODA_DEBUG){ PsodaPrinter::getInstance()->write("Difference in datasets: Seq %d (%s) seqlen: %d vs %d\n",i, getTaxonName(i), getSeqLen(i), otherDataset->getSeqLen(i)); }
				if( PSODA_DEBUG){ PsodaPrinter::getInstance()->write("dataset1: %s\n", getCharacters(i,false)); }
				if( PSODA_DEBUG){ PsodaPrinter::getInstance()->write("dataset2: %s\n", otherDataset->getCharacters(i,false)); }
				return false;
			}
		}
	}

	return true;
}


/**
 * Remove any current gaps from dataset, as a preprocess step for alignment
 */

void Dataset::stripGaps()
{
	mSequenceChanged = true;
	int i,j,k;

// remove gap characters from each taxon
	for(i=0;i<mNTaxa;i++)
	{
		if(mASCIIData[i] != NULL)
		{
			for(j=0,k=0;mASCIIData[i][j] != '\0' ;j++)
			{
				if(mASCIIData[i][j] != mGapChar)
				{
					mASCIIData[i][k] = mASCIIData[i][j];
					k++;
				}
			}
//Insure null termination of new sequence;
			mASCIIData[i][k] = '\0';
		}
	}
}


/**
 * Converts data set to a binary representation, also compresses equivalant rows
 */

void Dataset::calculateCompressedBinaryMatrix()
{
	int i,j;
	mSequenceChanged = false;
/*Free any previous matrix*/
	if(mBinaryData != NULL)
	{
		for(i=0;i<mNTaxa;i++)
			if(mBinaryData[i] != NULL)
				free(mBinaryData[i]);
		free(mBinaryData);
	}

/* Create Temporary Binary Matrix and Weights*/
	char** tempBinaryData = (char**)malloc(sizeof(char*)*mNTaxa);

	for(i=0;i<mNTaxa;i++)
	{
		tempBinaryData[i] = (char*)malloc(sizeof(char)*mNCharacters);
		for(int j=0;j<mNCharacters;j++)
			tempBinaryData[i][j] = convertToBinary(mASCIIData[i][j]);
	}
	int* tempWeights = (int*)malloc(sizeof(int)*mNCharacters);
	for(i=0;i<mNCharacters;i++)
		tempWeights[i] = 1;

/* Compress Temporary Binary Matrix*/

/* Copy Compressed Matrix to mBinaryData*/
	mBinaryData = tempBinaryData;
	mWeights = tempWeights;
	
	mWeightsBak = (int*)malloc(sizeof(int)*mNCharacters);
	memcpy(mWeightsBak, mWeights, mNCharacters * sizeof(int));
}


/**
 * DISPLAY FUNCTIONS
 */

void Dataset::printTNT(char* filename)
{
	FILE* fp = fopen(filename,"w");
	if(mDataFormat == ALIGNED_DATAFORMAT)
	{
		int nrows = ntaxa();
		int ncolumns = nchars();
		//fprintf(fp,"xread\n %d %d\n",ncolumns,nrows);
		for ( int i = 0 ; i < nrows ; i ++ )
		{
			unsigned char *rowptr = (unsigned char *)getCharacters(i,false);
			//fprintf(fp,"%s ",getTaxonName(i));
			fprintf(fp,"t%d\n",i);
			for ( int j = 0 ; j < ncolumns ; j ++ )
			{
				fprintf(fp,"%c",rowptr[j]);
			/*
				switch(rowptr[j])
				{
				case 'A':
				case 'a':
					fprintf(fp,"0");
					break;
				case 'G':
				case 'g':
					fprintf(fp,"1");
					break;
				case 'C':
				case 'c':
					fprintf(fp,"2");
					break;
				case 'T':
				case 't':
				case 'U':
				case 'u':
					fprintf(fp,"3");
					break;
				case 'R':
				case 'r':
					fprintf(fp,"[01]");
					break;
				case 'Y':
				case 'y':
					fprintf(fp,"[23]");
					break;
				case 'M':
				case 'm':
					fprintf(fp,"[02]");
					break;
				case 'K':
				case 'k':
					fprintf(fp,"[13]");
					break;
				case 'W':
				case 'w':
					fprintf(fp,"[03]");
					break;
				case 'S':
				case 's':
					fprintf(fp,"[12]");
					break;
				case 'B':
				case 'b':
					fprintf(fp,"[123]");
					break;
				case 'D':
				case 'd':
					fprintf(fp,"[013]");
					break;
				case 'H':
				case 'h':
					fprintf(fp,"[023]");
					break;
				case 'V':
				case 'v':
					fprintf(fp,"[012]");
					break;
				case 'N':
				case 'n':
				case '?':
					fprintf(fp,"[0123]");
					break;
				case '-':
					fprintf(fp,"4");
					break;
				default:
					printf("%c",rowptr[j]);
				}
			*/
			}
			fprintf(fp,"\n");
		}
	}
	fprintf(fp,";");
	fclose(fp);
}

/**
 * This Display function called by Interpreter. It uses PsodaPrinter
 */
void Dataset::printMatrix()
{
	if (mDataFormat == ALIGNED_DATAFORMAT &&
		(mDatatype == DNA_DATATYPE || mDatatype == NUCLEOTIDE_DATATYPE ))
	{
		int nrows = ntaxa();
		int ncolumns = nchars();
		PsodaPrinter::getInstance()->write("%d %d\n",nrows,ncolumns);
		for ( int i = 0 ; i < nrows ; i ++ )
		{
			unsigned char *rowptr = (unsigned char *)getCharacters(i,false);
			PsodaPrinter::getInstance()->write("%s ",getTaxonName(i));
			for ( int j = 0 ; j < ncolumns ; j ++ )
			{
				PsodaPrinter::getInstance()->write("%c",rowptr[j]);
			}
			PsodaPrinter::getInstance()->write("\n");
		}
	}
	else
	{
		printf("Ignoring request to print ");
		switch(mDataFormat)
		{
			case UNDEFINED_DATAFORMAT:
				printf("undefined alignment ");
				break;
			case UNALIGNED_DATAFORMAT:
				printf("unaligned ");
		}
		switch(mDatatype)
		{
			case UNDEFINED_DATATYPE:
				printf("undefined data");
				break;
			case NUCLEOTIDE_DATATYPE:
				printf("nucleotide data");
				break;
			case STANDARD_DATATYPE:
				printf("standard data");
				break;
			case RNA_DATATYPE:
				printf("rna data");
				break;
			case PROTEIN_DATATYPE:
				printf("protein data");
				break;
			case PROTEIN_CODING_DNA_DATATYPE:
				printf("protein coding dna data");
				break;
			case CODON_DATATYPE:
				printf("codon data");
				break;
			case SECONDARY_STRUCTURE_DATATYPE:
				printf("secondary structure data");
				break;
			case SECONDARY_STRUCTURE_3_STATE_DATATYPE:
				printf("3 state secondary structure data");
				break;
		}
		printf("\n");
	}
}


/**
 * This Display function is used by RAxML Search, It outputs to a filename
 */
void Dataset::printMatrix(string filename)
{
	if (mDataFormat == ALIGNED_DATAFORMAT &&
		mDatatype == DNA_DATATYPE )
	{

		if( filename == "none")
		{
			return;
		}

		FILE* fp;

		if( filename == "stdout" ||
			filename == "STDOUT" ||
			filename == "")
		{
/* use stdout */
			fp = stdout;
		}else if( filename == "stderr" ||
			filename == "STDERR")
		{
/* use stderr */
			fp = stderr;
		}
		else
		{
			fp = fopen( filename.c_str(), "w");
			if( ! fp)
			{
				char str[1024];
				sprintf( str, "Error: opening output sequence file \"%s\" (errno = %d(%s))\n", filename.c_str(), errno, strerror(errno));
				throw PsodaError(str);
			}
		}

		int nrows = ntaxa();
		int ncolumns = nchars();
		fprintf( fp, "%d %d\n",nrows,ncolumns);
		for ( int i = 0 ; i < nrows ; i ++ )
		{
			char *rowptr = getCharacters(i,false);
			fprintf( fp, "%s ",getTaxonName(i));
			for ( int j = 0 ; j < ncolumns ; j ++ )
			{
				fprintf( fp, "%c",rowptr[j]);
			}
			fprintf( fp, "\n");
		}

		if( filename != "stdout" &&
			filename != "STDOUT" &&
			filename != "stderr" &&
			filename != "STDERR" &&
			filename != "")
		{
			fclose( fp);
		}

	}
	else
	{
		printf("Ignoring request to print unaligned data and/or non-DNA (NYI)\n");
	}
}


/**
 * This function is used for MrBayes search, it outputs a nexus format
 */
void Dataset::printMatrixNexus(string filename, bool fixForBayes)
{

	if (mDataFormat == ALIGNED_DATAFORMAT &&
		mDatatype == DNA_DATATYPE )
	{

		if( filename == "none")
		{
			return;
		}

		FILE* fp;

		if( filename == "stdout" ||
			filename == "STDOUT" ||
			filename == "")
		{
/* use stdout */
			fp = stdout;
		}else if( filename == "stderr" ||
			filename == "STDERR")
		{
/* use stderr */
			fp = stderr;
		}
		else
		{
			fp = fopen( filename.c_str(), "w");
			if( ! fp)
			{
				char str[1024];
				sprintf( str, "Error: opening output sequence file \"%s\" (errno = %d(%s))\n", filename.c_str(), errno, strerror(errno));
				throw PsodaError(str);
			}
		}

		int nrows = ntaxa();
		int ncolumns = nchars();

		fprintf(fp, "BEGIN DATA;\ndimensions ntax=%d nchar=%d;\nformat interleave=no datatype=DNA;\n\nmatrix\n",nrows, ncolumns);

		for ( int i = 0 ; i < nrows ; i ++ )
		{
			char *rowptr = getCharacters(i,false);
			fprintf( fp, "%s\t",getTaxonName(i));
			for ( int j = 0 ; j < ncolumns ; j ++ )
			{
				char cprint;

				cprint = rowptr[j];

				if(fixForBayes)					  // Bayes cant handle ? or -
				{
					if((cprint == '-') || (cprint == '?'))
					{
						cprint = 'N';
					}
				}
				fprintf( fp, "%c",cprint);
			}
			fprintf( fp, "\n");
		}

		fprintf(fp, "\n;\nEnd;");
		fprintf(fp, "\nBEGIN TREES;\nTRANSLATE\n");

		for (int i = 0; i < nrows - 1 ; i ++)
		{
			fprintf(fp, "\t%d\t%s,\n",i+1,getTaxonName(i));
		}
		fprintf(fp, "\t%d\t%s\n;",nrows + 1,getTaxonName(nrows - 1));

		if( filename != "stdout" &&
			filename != "STDOUT" &&
			filename != "stderr" &&
			filename != "STDERR" &&
			filename != "")
		{
			fclose( fp);
		}

	}
	else
	{
		printf("Ignoring request to print unaligned data and/or non-DNA (NYI)\n");
	}
}


bool Dataset::printSeqs(string type, string filename)
{
	FILE* fp;
	int i = 0;

	if( filename == "stdout" ||
		filename == "STDOUT" ||
		filename == "")
	{
/* use stdout */
		fp = stdout;
	}
	else if( filename == "stderr" ||
		filename == "STDERR")
	{
/* use stderr */
		fp = stderr;
	}
	else
	{
		fp = fopen( filename.c_str(), "w");
		if( ! fp)
		{
			char str[1024];
			sprintf( str, "Error: opening output sequence file \"%s\" (errno = %d(%s))\n", filename.c_str(), errno, strerror(errno));
			throw PsodaError(str);
		}
	}

	if( type == "fasta" || type == "FASTA")
	{
		for( i = 0; i < mNTaxa; i++)
		{
			if (filename == "stdout" || filename == "STDOUT" || filename == "")
				PsodaPrinter::getInstance()->write(">%s\n%s\n", getTaxonName(i), getCharacters(i,false));
			else
				fprintf( fp, ">%s\n%s\n", getTaxonName(i), getCharacters(i,false));
		}
	}
	else if(type == "phylip" || type == "PHYLIP")
	{
		if (filename == "stdout" || filename == "STDOUT" || filename == "")
			PsodaPrinter::getInstance()->write("%d %d\n",mNTaxa,mNCharacters);
		else
			fprintf( fp,"%d %d\n",mNTaxa,mNCharacters);
	
		for( i = 0; i < mNTaxa; i++)
		{
			if (filename == "stdout" || filename == "STDOUT" || filename == "")
				PsodaPrinter::getInstance()->write("%10.10s%s\n", getTaxonName(i), getCharacters(i,false));
			else
				fprintf( fp, "%10.10s%s\n", getTaxonName(i), getCharacters(i,false));
		}
	}
	else
	{
		printf("Dataset::printSeqs(%s,%s) not implemented\n",type.c_str(),filename.c_str());
	}

	if( filename != "stdout" &&
		filename != "STDOUT" &&
		filename != "stderr" &&
		filename != "STDERR" &&
		filename != "")
	{
		fclose( fp);
	}
	return false;
}
