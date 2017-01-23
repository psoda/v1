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
#ifndef PSODA_DATA_H
#define PSODA_DATA_H

#include "Literal.h"
#include <string>

using namespace std;

class Data : public Literal {

 private:

  /**
   * The data stored in this literal
   */
  void* value;

  /**
   * Destroys the void* inside if there is one
   */
  void clean();

 public:

  /**
   * Default Contructor
   */
  Data();

  /**
   * Contructor with initial value
   */
  Data(void* initialValue);

  /**
   * Assignment Operator
   *
   * @return Reference to the resulting Literal
   * @throws InvalidCastException if the rhs cannot be converted to an int
   */
  virtual Literal& operator =(const Literal& rhs);

  /**
   * Destructor
   */
  virtual ~Data();
  
  /**
   * Updates the internal value
   *
   * @param value The new value
   */
  void setData(void* newValue);

  /**
   * Get the raw data
   */
  void* getData() const;

  /**
   * Get the raw data
   */
  virtual string toCode() const;

  /**
   * Copy this data pointer to the heap
   */
  virtual Data* copyToHeap() const;

};

#endif
