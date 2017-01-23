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
#include "QNode.h"
#include "QNodeInfo.h"
#include "QTree.h"
#include "Timer.h"
#include "PsodaPrinter.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "Interpreter.h"
#include <string.h>

#define NOT_CALCULATED   (-1.0)  /* used for percentNotGaps */

/* debugging flag used to debug just MultChar routines */
bool localDebug = false;


/** Constructor
 */
MultChar::MultChar() :
  mCount(0),
  mTotal(0),
  marked( false),
  head(NULL),
  percentNotGaps(NOT_CALCULATED)
{
}  
  

/** Constructor
 * \param residue single genetic character (e.g., mucleotide, amino acid)
 * \param ismarked sets the \a marked flag
 * \param total the number of tuples
 * \param ssElement secondary structure element (optional)
 */
MultChar::MultChar(char residue, bool ismarked, int total, char ssElement) :
  mCount(1),
  mTotal(total),
  marked( ismarked),
  head( newNode(residue, total, ssElement)),
  percentNotGaps(NOT_CALCULATED)
{
}

/** Destructor
 */
MultChar::~MultChar()
{
  charNode* tmp;
  charNode* next = head;
  while( next != NULL){
    tmp = next;
    next = next->next;
    free( tmp);
  }
}

/** Marks a tuple
 */
void MultChar::mark()
{
    marked = true;
}

bool MultChar::isMarked()
{
    return marked;
}

void MultChar::print()
{
  int gapChar = Interpreter::getInstance()->dataset()->gapchar();
  
  fprintf(stderr,"[");
  charNode* next = head;
  while( next != NULL){
    if( next->residue == gapChar){
      fprintf(stderr, "%s", "-");
    }else if( next->residue < 'A'){
      fprintf(stderr,"(%d)", next->residue);
    }else{
      fprintf(stderr,"%c", next->residue);
    }
    if( next->ssElement != UNDEFINED_SS_ELEMENT){
      fprintf(stderr, ":%c", next->ssElement);
    }
    if( next->next != NULL){
      fprintf(stderr, "%c", ',');
    }
    next = next->next;
  }
  //if( PSODA_DEBUG > 1){
    //fprintf(stderr,"(%d)]", mTotal);
  //}else{
    fprintf(stderr,"%s", "]");
    //}
}

int MultChar::getCount()
{
    return mCount;
}

int MultChar::getTotal()
{
    return mTotal;
}

int MultChar::getFirst()
{
  if( head){
    return head->residue;
  }else{
    return '\0';
  }
}

int MultChar::getFirstSS()
{
  if( head){
    return head->ssElement;
  }else{
    return UNDEFINED_SS_ELEMENT;
  }
}


/**
 * Calculates the most common secondary structure element.
 */
char MultChar::mostCommonSS()
{
  /* Future: cache results, clearing the cache in other methods */

  /* Future: initize in for loop */
  int totals[ SS_CHAR_NUMBER_OF_INDICES] = { 0, 0, 0, 0, 0};
  
  charNode* next = head;
  while( next != NULL){
    totals[ SSDataset::translateSSASCII2Num[ (int) next->ssElement] ]++;
    next = next->next;
  }

  int mostCommonSSIndex = 0;
  for( int i =  mostCommonSSIndex + 1; i < SS_CHAR_NUMBER_OF_INDICES; i++){
    if( totals[i] > totals[mostCommonSSIndex]){
      mostCommonSSIndex = i;
    }
  }
  
  return SSDataset::translateSSNum2ASCII2[mostCommonSSIndex];
  
} /* END mostCommonSS() */
  
/**
 * Returns the percent that are not gaps.
 */
float MultChar::getPercentNotGaps()
{
  if( fabs(percentNotGaps - NOT_CALCULATED) < 0.001){
    int gapChar = Interpreter::getInstance()->dataset()->gapchar();
    charNode* next = head;
    int nonGapCount = 0;
    while( next != NULL){
      if( next->residue == gapChar){
	nonGapCount++;
      }
      next = next->next;
    }
    assert( mCount);
    percentNotGaps = (float) nonGapCount / mCount;
  }
  
  return percentNotGaps;
}


/**
 * Copy the MultChar c.
 */
void MultChar::copy(MultChar *c)
{

  /* 4 scenarios:
   * 1) empty list
   * 2) first item
   * 3) middle item
   * 4) last item
   */
  
  if( c->head == NULL){
    /* empty list */
    /* nop */
    return;
  }

  /* first item */
  head = newNode( c->head);

  charNode* nextC = c->head->next;
  //charNode* next = head;
  charNode* prev = head;
  
  while( nextC != NULL){
    /* middle item */
    /* last item */
    prev->next = newNode( nextC);
    nextC = nextC->next;
    prev = prev->next;
  }
  
  mCount = c->mCount;
  mTotal = c->mTotal;

  if( localDebug){
    fprintf(stderr, "%s\n", "copy()");
    print();
  }
  
} /* END copy() */

/**
 * Returns the number of characters that match the residue and secondary structure elements.
 */
int MultChar::getChar(char residue, char ssElement)
{
  charNode* next = head;

  /* look for existing char, first by residue, then by ssElement */
  while( next != NULL &&
	 next->residue < residue){
    next = next->next;
  }
  if( next != NULL &&
      next->residue == residue){
    while( next != NULL &&
	   next->ssElement < ssElement){
      next = next->next;
    }
  }

  if( next != NULL &&
      next->residue == residue &&
      next->ssElement == ssElement){
    return next->numberOfChars;
  }else{
    return -1;
  }
} /* END getChar() */



/**
 * Increments the number of characters that match the residue and secondary structure element if the tuple of residue and ssElement already exists.
 * Otherwise, make a new MultChar of residue and ssElement that represents numberOfChars characters.
 */
void MultChar::setChar(char residue, int numberOfChars, char ssElement)
{
  charNode* next = head;
  charNode* prev = NULL;

  /* look for existing char, first by residue, then by ssElement */
  while( next != NULL &&
	 next->residue < residue){
    prev = next;
    next = next->next;
  }
  if( next != NULL &&
      next->residue == residue){
    while( next != NULL &&
	   next->ssElement < ssElement){
      prev = next;
      next = next->next;
    }
  }

  if( next != NULL &&
      next->residue == residue &&
      next->ssElement == ssElement){
    next->numberOfChars++;
  }else{
    /* new character */
  
    /* 4 scenarios:
     * 1) empty list
     * 2) first item
     * 3) middle item
     * 4) last item
     */
  
    if( next == head){
      if( head == NULL){
	/* empty list */
	head = newNode( residue, numberOfChars, ssElement);
      }else{
	/* first item */
	charNode* tmp = head;
	head = newNode( residue, numberOfChars, ssElement);
	head->next = tmp;
      }
    }else{
      /* middle item */
      /* last item */
      prev = newNode( residue, numberOfChars, ssElement);
      prev->next = next;
    }

    mCount++;
  }

  mTotal += numberOfChars;

  if( localDebug){
    fprintf(stderr, "%s\n", "setChar()");
    print();
  }

} /* END setChar() */

/**
 * Increments the number of characters that match the residue and secondary structure element if the tuple of residue and ssElement already exists.
 * Otherwise, make a new MultChar of residue and ssElement that represents a single tuple.
 */
void MultChar::incChar(char residue, char ssElement)
{

  charNode* next = head;
  charNode* prev = NULL;
  /* look for existing char, first by residue, then by ssElement */
  while( next != NULL &&
	 next->residue < residue){
    prev = next;
    next = next->next;
  }
  if( next != NULL &&
      next->residue == residue){
    while( next != NULL &&
	   next->ssElement < ssElement){
      prev = next;
      next = next->next;
    }
  }

  if( next != NULL &&
      next->residue == residue &&
      next->ssElement == ssElement){
    next->numberOfChars++;
  }else{
    /* new character */
  
    /* 4 scenarios:
     * 1) empty list
     * 2) first item
     * 3) middle item
     * 4) last item
     */
  
    if( next == head){
      if( head == NULL){
	/* empty list */
	head = newNode( residue, 1, ssElement);
      }else{
	/* first item */
	charNode* tmp = head;
	head = newNode( residue, 1, ssElement);
	head->next = tmp;
      }
    }else{
      /* middle item */
      /* last item */
      prev = newNode( residue, 1, ssElement);
      prev->next = next;
    }

    mCount++;
  }

  mTotal++;

  if( localDebug){
    fprintf(stderr, "%s\n", "incChar()");
    print();
  }
} /*  END incChar() */

/**
 * Combine the MultChars a and b into self.
 */
void MultChar::add(MultChar *a, MultChar *b)
{

  if (a == NULL){
    copy( b);
  }else{
    charNode* nextA = a->head;
    charNode* nextB = b->head;
    charNode* prev = head;
  
    /* Scenarios:
       a ? b | action
       --------------
         =   | a++, b++
         <   | a++
         >   | b++

       a in range? | b in range? | action
       ----------------------------------
       yes         | yes         | see above
       yes         | no          | a++
       no          | yes         | b++
       no          | no          | done
     */
    
    /* first item */
    if( (nextA->residue == nextB->residue) &&
	(nextA->ssElement == nextB->ssElement)){
      head = newNode( nextA->residue, nextA->numberOfChars + nextB->numberOfChars, nextA->ssElement);
      nextA = nextA->next;
      nextB = nextB->next;
    }else if( ((nextA->residue == nextB->residue) &&
	       (nextA->ssElement < nextB->ssElement)) ||
	      (nextA->residue < nextB->residue)){
      head = newNode( nextA);
      nextA = nextA->next;
    }else{
     head = newNode( nextB);
     nextB = nextB->next;
    }
    mCount++;
    
    prev = head;
    
    while( nextA != NULL || nextB != NULL){
      if( (nextA != NULL && nextB != NULL) &&
	  (nextA->residue == nextB->residue) &&
	  (nextA->ssElement == nextB->ssElement)){
	prev->next = newNode( nextA->residue, nextA->numberOfChars + nextB->numberOfChars, nextA->ssElement);
	nextA = nextA->next;
	nextB = nextB->next;
	prev = prev->next;
      }else if( ((nextA != NULL && nextB != NULL) &&
		 ((nextA->residue < nextB->residue) ||
		  ((nextA->residue == nextB->residue) &&
		   (nextA->ssElement < nextB->ssElement)))) ||
		(nextA != NULL && nextB == NULL)){
	prev->next = newNode( nextA);
	nextA = nextA->next;
	prev = prev->next;
      }else{
	prev->next = newNode( nextB);
	nextB = nextB->next;
	prev = prev->next;
      }
      mCount++;
    }
    mTotal = a->mTotal + b->mTotal;
  }

  if( localDebug){
    fprintf(stderr, "%s\n", "add()");
    print();
  }
} /* END add() */

/**
 * Return the combined cost of matching this MultChar with f, using substitution matrix submat.
 */
double MultChar::evalCost(MultChar *f, SubMat* subMat)
{
    double cost = 0.0;
    int tot = 0;

    charNode* next = head;
    charNode* nextF;

    /* iterate through each tuple */
    while( next != NULL){
      nextF = f->head;
      /* iterate through each tuple of MultChar f */
      while( nextF != NULL){
	//fprintf( stderr, "evalCost() %c (%d) and %c (%d)\n", mShortList[i], mCharValues[ mShortList[i]], f->mShortList[j], f->mCharValues[ f->mShortList[j]]);
        cost += next->numberOfChars * nextF->numberOfChars * subMat->get( next->residue, nextF->residue);
	tot  += next->numberOfChars * nextF->numberOfChars;
	nextF = nextF->next;
      }
      next = next->next;
    }

    return (cost/tot);
}

/**
 * Return the combined cost of matching this MultChar with f, using substitution matrices submat{,A,B,L,SS}.
 */
double MultChar::evalSSCost(MultChar *f, SubMat* subMat, SubMat* subMatA, SubMat* subMatB, SubMat* subMatL, SubMat* subMatSS)
{
    double cost = 0.0;
    int tot = 0;

    charNode* next = head;
    charNode* nextF;

    while( next != NULL){
      nextF = f->head;
      while( nextF != NULL){
	//fprintf( stderr, "evalCost() %c (%d) and %c (%d)\n", mShortList[i], mCharValues[ mShortList[i]], f->mShortList[j], f->mCharValues[ f->mShortList[j]]);
	cost += next->numberOfChars * nextF->numberOfChars * ssCost( next->residue, next->ssElement, nextF->residue, nextF->ssElement, subMat, subMatA, subMatB, subMatL, subMatSS);
	tot  += next->numberOfChars * nextF->numberOfChars;
	nextF = nextF->next;
      }
      next = next->next;
    }

    return (cost/tot);
}


/**
 * Return the match score for characters a and b (and their secondary structures ssA and ssB), using substitution matrices submat{,A,B,L,SS}.
 * \sa QSSAlign::ssMatCost()
 */
float MultChar::ssCost(char a, char ssA, char b, char ssB, SubMat* subMat, SubMat* subMatA, SubMat* subMatB, SubMat* subMatL, SubMat* subMatSS){
  double ssMatCost = 0.0;
  
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
    ssMatCost = subMat->get(a, b);
  }

  return ((ssMatCost + subMatSS->get(ssA, ssB)) / 2.0);
}


/**
 * Make a new node with numberOfChars character residue and secondary structure element tuples.
 */
charNode* MultChar::newNode(char residue, int numberOfChars, char ssElement){
  charNode* newNode = (charNode*) check_calloc( 1, sizeof( charNode));
  newNode->residue = residue;
  newNode->ssElement = ssElement;
  newNode->numberOfChars = numberOfChars;
  newNode->next = NULL;

  return newNode;
}


/**
 * Make a new node based on nodeToCopy.
 */
charNode* MultChar::newNode(charNode* nodeToCopy){
  charNode* newNode = (charNode*) check_calloc( 1, sizeof( charNode));
  newNode->residue = nodeToCopy->residue;
  newNode->ssElement = nodeToCopy->ssElement;
  newNode->numberOfChars = nodeToCopy->numberOfChars;
  newNode->next = NULL;

  return newNode;
}


/**
 * Allocs memory, checks the allocation and sets the contents to zero 
 */
void* MultChar::check_calloc( size_t count, size_t size){
  void* ptr = malloc( count * size);
  if( ! ptr){
    throw PsodaError("Alloc error (MultChar)!");
  }
  
  memset( ptr, 0, count * size);
  
  return ptr;
}


