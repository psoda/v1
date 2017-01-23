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
#ifndef PSODA_LSCORES_INSTR_H
#define PSODA_LSCORES_INSTR_H

#include "BuiltInCommand.h"
#include <list>

using namespace std;

class LScoresInstr : public BuiltInCommand {

 private:

  //Prohibit copying of this object
  void operator =(LScoresInstr&);
  LScoresInstr(LScoresInstr&);

 protected:
  
  list<int> treeList;

 public:

  /**
   * Constructor
   */
  LScoresInstr();

  /**
   * Destructor
   */
  virtual ~LScoresInstr();

  /**
   * Call passParams() to set up the parameters in the interpreter
   */
  virtual void execute(Environment* baseEnv);
  virtual void execute(Environment* baseEnv, Literal*& returnVal);

  /**
   * Takes the number of a tree and adds it to treeList
   */
  void addTree(int newTree);

  /**
   * Returns the treeList as a string
   */
  string treesString() const;

  /**
   * Returns the name of this instruction
   */
  virtual string getName() const;

};

#endif

