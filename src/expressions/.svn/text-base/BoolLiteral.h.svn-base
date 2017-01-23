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
#ifndef PSODA_BOOL_LITERAL_H
#define PSODA_BOOL_LITERAL_H

#include "Literal.h"
#include <string>

using namespace std;

class BoolLiteral : public Literal {

 private:

  /**
   * The value of this bool literal
   */
  bool value;

 public:

  /**
   * Default Contructor
   */
  BoolLiteral();

  /**
   * Copy Contructor
   */
  BoolLiteral(const Literal& other);

  /**
   * Contructor with initial value
   */
  BoolLiteral(bool initialValue);

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
  virtual ~BoolLiteral();
  
  /**
   * Updates the internal value
   *
   * @param value The new value
   */
  void setValue(bool newValue);

  /**
   * Returns an integer representation of the literal
   */
  virtual bool toBool() const;

  /**
   * Returns 'true' if value and 'false' otherwise
   */
  virtual string toString() const;

  /**
   * Returns an integer representation of the literal
   */
  virtual int toInt() const;

  /**
   * Returns an integer representation of the literal
   */
  virtual double toRealNumber() const;

  /**
   * Creates a new copy of this Literal on the heap and returns it
   */
  virtual BoolLiteral* copyToHeap() const;

};

#endif
