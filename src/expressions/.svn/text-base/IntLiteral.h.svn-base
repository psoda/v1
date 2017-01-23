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
#ifndef PSODA_INT_LITERAL_H
#define PSODA_INT_LITERAL_H

#include "Literal.h"
#include <string>

using namespace std;

class IntLiteral : public Literal {

 private:

  /**
   * The value of this int literal
   */
  int value;

 public:

  /**
   * Default Contructor
   */
  IntLiteral();

  /**
   * Copy Contructor
   */
  IntLiteral(const Literal& other);

  /**
   * Contructor with initial value
   */
  IntLiteral(int initialValue);

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
  virtual ~IntLiteral();
  
  /**
   * Updates the internal value
   *
   * @param value The new value
   */
  void setValue(int newValue);

  /**
   * Returns an integer representation of the literal
   */
  virtual bool toBool() const;

  /**
   * Returns an integer representation of the literal
   */
  virtual int toInt() const;

  /**
   * Returns an integer representation of the literal
   */
  virtual double toRealNumber() const;

  /**
   * Returns an integer representation of the string
   */
  virtual string toString() const;

  /**
   * Creates a new copy of this Literal on the heap and returns it
   */
  virtual IntLiteral* copyToHeap() const;

  /**
   * Returns an IntLiteral with the minimum possible IntLiteral value
   */
  static const IntLiteral& getMin();

  /**
   * Returns an IntLiteral with the maximum possible IntLiteral value
   */
  static const IntLiteral& getMax();

};

#endif
