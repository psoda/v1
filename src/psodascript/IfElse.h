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
#ifndef PSODA_IF_ELSE_H
#define PSODA_IF_ELSE_H

#include "PsodaConstruct.h"
#include "Expression.h"
#include "IfThenPair.h"
#include <list>

using namespace std;

class IfElse : public PsodaConstruct  {

 private:

  //Prohibit copying of this object
  IfElse(IfElse& other);
  void operator =(IfElse& rhs);

  /**
   * These will be checked in order first to last, execute first body whose condition holds
   * All new IfThenPairs are given default condition of true
   */
  list<IfThenPair*> cases;

 public:

  /**
   * Constructor
   */
  IfElse();

  /**
   * Destructor
   */
  virtual ~IfElse();

  /**
   * Add the given node to the last added IfThenPair
   */
  virtual void addNode(ProgramGraphNode* newNode);
  
  /**
   * Create and add a new IfThenPair to the end of the cases
   */
  void beginElsif();
  
  /**
   * Alias for beginElsif
   */
  void beginElse();

  /**
   * Iterate through cases and execute body of first case whose condition holds
   */
  virtual void execute(Environment* baseEnv);
  virtual void execute(Environment* baseEnv,Literal*& result);
  
  /**
   * Performs possible validation in self and children
   */
  virtual void validate();

  /**
   * Prints the source code of the IfElse to a string
   */
  virtual string toString() const;

  /**
   * Prints the source code of the IfElse to a string at the given indentation depth
   */
  virtual string toString(int depth, string preText = "") const;

  /**
   * Sets the condition of the current case
   *
   * @param newCondition The new condition for the current case
   */
  virtual void setCondition(Expression* newCondition);

  /**
   * Gets the condition of the current case
   */
  virtual Expression* getCondition() const;

};

#endif
