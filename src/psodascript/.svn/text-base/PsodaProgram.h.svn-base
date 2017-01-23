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
#ifndef PSODA_PROGRAM_H
#define PSODA_PROGRAM_H

#include "PsodaConstruct.h"
#include "PsodaException.h"
#include "PsodaWarning.h"
#include <map>
#include <stack>
#include <list>
#include <vector>

using namespace std;

class UserDefinedCommand;
class FunctionPointerLiteral;

class PsodaProgram : public PsodaConstruct {

 private:

  /**
   * The nodes that make up the body of this program
   */
  list<ProgramGraphNode*> body;

  /**
   * Keeps track of Node nesting
   */
  stack<ProgramGraphNode*> nodeStack;
  
  //Prohibit copying of this object
  PsodaProgram(PsodaProgram& other);
  void operator =(PsodaProgram& rhs);

 public:

  /**
   * Constructor
   */
  PsodaProgram();

  /**
   * Destructor
   */
  virtual ~PsodaProgram();

  /**
   * Add node to top stack Node body if not empty; if empty add to progBody; then push ptr on stack
   */
  virtual void beginNode(ProgramGraphNode* newNode);

  /**
   * Pop top node ptr
   */
  virtual void endNode();

  /**
   * Adds node to the node list as if completed (thus unmodifiable)
   * In other words, it performs a begin and endNode() all in one step
   * This is principally here so that a PsodaProgram can add nodes to
   * PsodaConstructs.
   */
  virtual void addNode(ProgramGraphNode* newNode);

  /**
   * Returns non-const ptr to top node on stack
   */
  virtual ProgramGraphNode* currentNode() const;

  /**
   * Calls execute on the current program
   *
   * @throws PsodaReturn if a ReturnIntruction is executed inside
   */
  virtual void execute(Environment* baseEnv);
  virtual void execute(Environment* baseEnv,Literal*&);

  /**
   * Executes the program nodes without pushing an extra environment on
   */
  void executeInSameEnv(Environment& baseEnv);

  /**
   * Performs possible validation in self and children
   */
  virtual void validate();

  /**
   * Prints the program source code to a string
   */
  virtual string toString() const;

  /**
   * Prints the program source code to a string at the given indentation depth
   */
  virtual string toString(int depth, string preText = "") const;

};

#endif
