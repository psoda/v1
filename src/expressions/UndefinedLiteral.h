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
#ifndef PSODA_UNDEFINED_LITERAL_H
#define PSODA_UNDEFINED_LITERAL_H

#include "Literal.h"
#include <string>
#include <typeinfo>

using namespace std;

class UndefinedLiteral : public Literal {

 public:

  /**
   * Default Contructor
   */
  UndefinedLiteral();

  /**
   * Copy Contructor
   */
  UndefinedLiteral(const Literal& other);

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
  virtual ~UndefinedLiteral();
  
  /**
   * Access a Singleton UndefinedLiteral
   */
  static UndefinedLiteral* getInstance();

  /**
   * Throws UndefinedVariableException
   */
  virtual bool toBool() const;

  /**
   * Throws UndefinedVariableException
   */
  virtual int toInt() const __attribute__((noreturn)) ;

  /**
   * Throws UndefinedVariableException
   */
  virtual string toString() const __attribute__((noreturn)) ;

  /**
   * Returns undef
   */
  virtual string toCode() const;

  /**
   * Creates a new copy of this Literal on the heap and returns it
   */
  virtual UndefinedLiteral* copyToHeap() const;

  /**
   * Returns a reference to the class type for use in compares
   */
  static const type_info& getType();

};

#endif
