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
#ifndef PSODA_ELEMENT_H
#define PSODA_ELEMENT_H

#include "LocatableObject.h"
#include "Literal.h"
#include "Environment.h"
#include <string>

using namespace std;

class PsodaElement {

 protected:

  //Prohibit copying of this object
  PsodaElement(PsodaElement& other);
  void operator =(PsodaElement& rhs);

  /**
   * Returns a string with the given number of tabs
   */
  string getTabs(int depth) const;

  /**
   * Returns a string of spaces with the same length as the given text
   */
  string getSpaces(string text) const;

public:

  /**
   * Constructor
   */
  PsodaElement();

  /**
   * Destructor
   */
  virtual ~PsodaElement();

 /**
   * All inheriting classes must implement an execute function
   */
  virtual void execute(Environment* baseEnv) = 0;

 /**
   * Execute the instruction and get back any return value through returnVal
   */
  virtual void execute(Environment* baseEnv, Literal*& returnVal) = 0;

  /**
   * All inheriting classes must implement a toString function
   */
  virtual string toString() const = 0;

  /**
   * Prints the element out at the given indentation depth
   */
  virtual string toString(int depth, string preText) const = 0;

};

#endif
