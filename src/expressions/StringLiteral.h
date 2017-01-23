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
#ifndef PSODA_STRING_LITERAL_H
#define PSODA_STRING_LITERAL_H

#include "Literal.h"
#include <string>
#include <typeinfo>

using namespace std;

class StringLiteral : public Literal {

 private:

  /**
   * The value of this int literal
   */
  string value;

 public:

  /**
   * Default Contructor
   */
  StringLiteral();

  /**
   * Copy Constructor
   */
  StringLiteral(const Literal& other);

  /**
   * Contructor with initial value
   */
  StringLiteral(string initialValue);

  /**
   * Checks the string value to see if it holds a valid number
   *
   * @return True if the string has only digits other than at most one decimal point,
   *         and false otherwise.
   */
  static bool isValidNum(string value);

  /**
   * Assignment operator
   */
  virtual Literal& operator =(const Literal& rhs);
  StringLiteral& operator =(const string& rhs);

  /**
   * Destructor
   */
  virtual ~StringLiteral();
  
  /**
   * Updates the internal value
   *
   * @param value The new value
   */
  void setValue(string newValue);

  /**
   * Returns a string with special characters escaped
   */
  virtual string toCode() const;

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
   * Get the value of the string in lower case
   */
  string toLowerCase();

  /**
   * Creates a new copy of this Literal on the heap and returns it
   */
  virtual StringLiteral* copyToHeap() const;

  /**
   * Get the value of the string in lower case
   */
  static string toLowerCase(const string& inString);

  /**
   * Replace all newline characters with single spaces
   */
  static string replaceNewLines(const string& inString);

  /**
   * Returns a reference to the class type for use in compares
   */
  static const type_info& getType();

};

#endif
