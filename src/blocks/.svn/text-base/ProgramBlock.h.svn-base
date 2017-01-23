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
#ifndef PSODA_PROGRAM_BLOCK_H
#define PSODA_PROGRAM_BLOCK_H

#include "Block.h"
#include "PsodaProgram.h"
#include <typeinfo>

using namespace std;

class ProgramBlock : public Block {
  
 private:
  
  /**
   * The program represented by this program block
   */
  PsodaProgram programGraph;

 public:

  /**
   * Constructor
   */
  ProgramBlock();

  /**
   * Destructor
   */
  virtual ~ProgramBlock();

  /**
   * Get the program graph
   *
   * @return A reference to the PsodaProgramGraph represented by this data block
   */
  PsodaProgram& getProgram();

  /**
   * Uses the Singleton Interpreter to run the PsodaProgram
   */
  virtual void interpretBlock();

  /**
   * Returns a reference to the class type for use in compares
   */
  static const type_info& getType();

};

#endif
