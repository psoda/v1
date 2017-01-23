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
#include "PsodaBlocks.h"

using namespace std;

PsodaBlocks::PsodaBlocks() : curIndex(0) {
  return;
}

PsodaBlocks::~PsodaBlocks() {
  //int size = blocks.size();
  while (blocks.size() > 0) {
    Block* thisBlockPtr = blocks.back();
    blocks.pop_back();
    delete thisBlockPtr;
  }
  curIndex = 0;
}

bool PsodaBlocks::hasNext() {
  return ((unsigned int)curIndex < blocks.size());
}
  
Block* PsodaBlocks::next() {
  return blocks[curIndex++];
}

Block* PsodaBlocks::top() {
  return blocks.back();
}

int PsodaBlocks::size() {
  return blocks.size();
}

void PsodaBlocks::push(Block* newBlock) {
  blocks.push_back(newBlock);
}

void PsodaBlocks::pop() {
  blocks.pop_back();
}
