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
#ifndef PSODA_REAL_NUMBER_LITERAL_H
#define PSODA_REAL_NUMBER_LITERAL_H

#include "Literal.h"
#include <string>
#include <typeinfo>

using namespace std;

class RealNumberLiteral : public Literal {

 private:

  /**
   * The value of this int literal
   */
  double value;

 public:

  /**
   * Default Contructor
   */
  RealNumberLiteral();

  /**
   * Copy Contructor
   */
  RealNumberLiteral(const Literal& other);

  /**
   * Contructor with initial value
   */
  RealNumberLiteral(double initialValue);

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
  virtual ~RealNumberLiteral();
  
  /**
   * Updates the internal value
   *
   * @param value The new value
   */
  void setValue(double newValue);

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
  virtual RealNumberLiteral* copyToHeap() const;

  /**
   * Returns a reference to the class type for use in compares
   */
  static const type_info& getType();

  /**
   * Returns a RealNumberLiteral with the minimum possible value
   */
  static const RealNumberLiteral& getMin();

  /**
   * Returns a RealNumberLiteral with the maximum possible value
   */
  static const RealNumberLiteral& getMax();

};

#endif
