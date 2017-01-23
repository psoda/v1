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
#ifndef PSODA_VALIDATABLE_H
#define PSODA_VALIDATABLE_H

#include <string>

using namespace std;

/**
 * An object of this class can be validated
 */
class Validatable {

 private:

  void operator =(Validatable& rhs);

  string sourceCode;

 protected:

  /**
   * Constructors
   */
  Validatable();

  /**
   * Copy Constructor
   */
  Validatable(const Validatable& other);

 public:

  /**
   * Destructor
   */
  virtual ~Validatable();

  /**
   * Performs any necessary validatation on this object and its children (by default, does nothing)
   */
  virtual void validate();
  
};

#endif
