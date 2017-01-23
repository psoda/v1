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
#ifndef PSODA_WHILE_LOOP_H
#define PSODA_WHILE_LOOP_H

#include "PsodaConstruct.h"
#include "Expression.h"
#include "PsodaProgram.h"

using namespace std;

class WhileLoop : public PsodaConstruct {

 private:

  //Prohibit copying of this object
  WhileLoop(WhileLoop& other);
  void operator =(WhileLoop& rhs);

  /**
   * The condition of the conditional (ie. while <condition> ... )
   */
  Expression* condition;

  PsodaProgram body; //do

 public:

  /**
   * Constructor
   */
  WhileLoop();

  /**
   * Destructor
   */
  virtual ~WhileLoop();

  /**
   * Add the given element to the loop body
   */
  void addNode(ProgramGraphNode* newNode);

  /**
   * Returns a pointer to the condition expr
   */
  Expression* getCondition() const;

  /**
   * Sets the condition
   */
  void setCondition(Expression* newCondition);

  /**
   * Check condition and execute loop body while still true
   */
  virtual void execute(Environment* baseEnv);
  virtual void execute(Environment* baseEnv, Literal*& returnVal);
  /**
   * Performs possible validation in self and children
   */
  virtual void validate();

  /**
   * Prints the WhileLoop source code to a string
   */
   string toString() const;

  /**
   * Prints the source code of the WhileLoop to a string at the given indentation depth
   */
   string toString(int depth, string preText = "") const;

};

#endif
