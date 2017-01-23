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
#ifndef PSODA_EXPRESSION_H
#define PSODA_EXPRESSION_H

#include "LocatableObject.h"
#include "Validatable.h"
#include "Codeable.h"
#include "Environment.h"
#include "Literal.h"

using namespace std;

class Expression : public LocatableObject, public Codeable, public Validatable {

 public:

  /**
   * Default Constructor
   */  
  Expression();

  /**
   * Destructor
   */  
  virtual ~Expression();

  /**
   * Evaluates the Expression under the Interpreter's current environment
   *
   * @return a Literal with the evaluated value
   */
   virtual Literal* evaluate(Environment* curEnv) = 0;

   /**
    * Prints the expression to a string
    *
    * @return A string representation of the expression
    */
   virtual string toString() const = 0;

};

#endif
