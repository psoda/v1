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
#ifndef PSODA_BLOCKS_H
#define PSODA_BLOCKS_H

using namespace std;

#include "Block.h"
#include <vector>

class PsodaBlocks {
  
 private:

  vector<Block*> blocks;
  int curIndex;

 public:

  /**
   * Constructor
   */
  PsodaBlocks();

  /**
   * Destructor
   */
  virtual ~PsodaBlocks();

  /**
   * Returns true if curIndex < blocks.size
   * 
   * @returns Whether or not there are more blocks to be gotten
   */
  bool hasNext();
  
  /**
   * Returns a pointer to the next block
   */ 
  Block* next();

  /**
   * Returns a pointer to the last added block
   */
  Block* top();

  /**
   * Returns the number of blocks
   */
  int size();

  /**
   * Pushes the given block pointer on to the top
   */
  void push(Block* newBlock);

  /**
   * Pops the top block pointer
   */
  void pop();

};

#endif
