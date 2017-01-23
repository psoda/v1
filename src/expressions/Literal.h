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
#ifndef PSODA_LITERAL_H
#define PSODA_LITERAL_H

#include "QTree.h"
#include <string>

using namespace std;

class Literal {

 protected:

  /**
   * Contructor
   */
  Literal();

 public:

  /**
   * Assignment Operator
   */
  virtual Literal& operator =(const Literal& rhs) = 0  ;

  /**
   * Destructor
   */
  virtual ~Literal();
  
  /**
   * Returns a string representation of the code that made the literal
   *
   * The default is to call toString();
   */
  virtual string toCode() const;

  /**
   * Returns a real number representation of the literal
   */
  virtual double toRealNumber() const __attribute__((noreturn)) ;

  /**
   * Returns a boolean representation of the literal
   */
  virtual bool toBool() const __attribute__((noreturn)) ;

  /**
   * Returns an integer representation of the literal
   */
  virtual int toInt() const __attribute__((noreturn)) ;

  /**
   * Returns a string representation of the literal
   */
  virtual string toString() const __attribute__((noreturn)) ;

  /**
   * Returns the QTree pointer stored in this literal (if it is a TreeLiteral)
   */
  virtual QTree* toTree() const __attribute__((noreturn)) ;

  /**
   * Returns the QTree pointer stored in this literal (if it is a TreeLiteral)
   */
  virtual void* getData() const;

  /**
   * Creates a new copy of this Literal on the heap and returns it
   */
  virtual Literal* copyToHeap() const = 0;

};

#endif
