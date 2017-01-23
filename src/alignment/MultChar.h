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
#ifndef QSODA_MULTCHAR 
#define QSODA_MULTCHAR

#include "QNode.h"
#include "Dataset.h"
#include "SubMat.h"

/** Indicates an unused Secondary Structure element.  Especially used when not incorporating secondary structures. */
#define UNDEFINED_SS_ELEMENT      (-1)


/**
 * A tuple of a genetic character and its secondary structure element (if included).  MultChars are comprised of list of charNodes.  
 */
typedef struct charNode{
  char residue;       /* nucleotide or amino acid */
  char ssElement;     /* secondary structure element */
  int numberOfChars;  /* the number of residues */
  charNode* next;     /* pointer to the next tuple */
} charNode; 


class MultChar 
{
  private:
    int mCount; /* the number of unique chars (e.g., (1 G, 4 Cs, 1 T) = 3; length of the linked list*/
    int mTotal; /* total number of chars (e.g., (1 G, 4 Cs, 1 T) = 6; sum of all the ______ values in the linked list */
    bool marked;/* tracks inserted gaps during pushGaps(). \sa pushGaps() */

    charNode* head;  /* first tuple in the list */

    float percentNotGaps;  /* a cached value; values can be -1.0 (NOT_CALCULATED) or between 0.0 - 1.0 */

    charNode* newNode(char residue, int numberOfChars, char ssElement = UNDEFINED_SS_ELEMENT);
    charNode* newNode(charNode* nodeToCopy);
    void* check_calloc( size_t count, size_t size);

    float ssCost(char a, char ssA, char b, char ssB, SubMat* subMat, SubMat* subMatA, SubMat* subMatB, SubMat* subMatL, SubMat* subMatSS);

  public:
    MultChar();
    MultChar(char residue, bool ismarked = false, int total = 1, char ssElement = UNDEFINED_SS_ELEMENT);
    virtual ~MultChar();

    void setChar(char residue, int numberOfChars, char ssElement = UNDEFINED_SS_ELEMENT);
    int getChar(char residue, char ssElement);
    void incChar(char residue, char ssElement);
    int getCount();
    int getTotal();
    int getFirst();
    int getFirstSS();
    char mostCommonSS(); /* returns the most common ssElement (preference for ties are (in order) ... */
    float getPercentNotGaps();
  
    void mark();
    bool isMarked();

/*     bool equalsGap(); */
/*     bool equalsGapOnly(); */

    void copy(MultChar *c);
    void add(MultChar *a, MultChar *b);
    double evalCost(MultChar *f, SubMat* subMat);
    double evalSSCost(MultChar *f, SubMat* subMat, SubMat* subMatA, SubMat* subMatB, SubMat* subMatL, SubMat* subMatSS);

    void print();
};
#endif

