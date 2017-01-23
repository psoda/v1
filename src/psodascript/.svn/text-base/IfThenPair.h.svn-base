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
#ifndef PSODA_IF_THEN_PAIR_H
#define PSODA_IF_THEN_PAIR_H

#include "Expression.h"
#include "PsodaProgram.h"

using namespace std;

class IfThenPair {

 private:

  Expression* cond; //if
  PsodaProgram body; //then

 public:

  /**
   * Constructor
   */
  IfThenPair();

  /**
   * Destructor
   */
  virtual ~IfThenPair();

  /**
   * Frees the memory on the previous condition and sets the new condition
   *
   * @param newCond The new condition for this pair
   */
  void setCondition(Expression* newCond);

  /**
   * Returns a pointer to the condition
   */
  Expression* getCondition() const;

  /**
   * Returns a reference to the body
   */
  PsodaProgram& getBody();

};

#endif
