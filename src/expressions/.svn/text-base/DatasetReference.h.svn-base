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
#ifndef PSODA_DATASET_REFERENCE_H
#define PSODA_DATASET_REFERENCE_H

#include "Literal.h"
#include "StringLiteral.h"
#include <string>
#include <typeinfo>

using namespace std;

class DatasetReference : public Literal {

 private:

  /**
   * The value of this int reference
   */
  StringLiteral value;

 public:

  /**
   * Contructor with initial value
   */
  DatasetReference(string initialValue);

  /**
   * Copy Constructor
   */
  DatasetReference(const Literal& other);

  /**
   * Assignment operator
   */
  virtual Literal& operator =(const Literal& rhs);

  /**
   * Destructor
   */
  virtual ~DatasetReference();
  
  /**
   * Returns an integer representation of the dataset
   */
  virtual string toString() const;

  /**
   * Creates a new copy of this Reference on the heap and returns it
   */
  virtual DatasetReference* copyToHeap() const;

  /**
   * Returns a reference to the class type for use in compares
   */
  static const type_info& getType();

};

#endif
