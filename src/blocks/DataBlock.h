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
#ifndef PSODA_DATA_BLOCK_H
#define PSODA_DATA_BLOCK_H

#include "Block.h"
#include "Dataset.h"
#include <typeinfo>

using namespace std;

class DataBlock : public Block {
  
 public:

  /**
   * Constructor
   */
  DataBlock(BlockType type = UNDEFINED_BLOCK);

  /**
   * Destructor
   */
  virtual ~DataBlock();

  /**
   * Get the dataset
   *
   * @return A reference to the Dataset represented by this data block
   */
  Dataset& getDataset();

  /**
   * Returns the internal dataset pointer (which is allocated on the headp) and sets the internal pointer to NULL
   * Whoever calls this method is responsible for freeing the memory also, the datablock will not be worth anything
   * after this method call
   */
  Dataset* popDataset();

  /**
   * Sets up the dataset in the Singleton Interpreter
   */
  virtual void interpretBlock();

  /**
   * Returns a reference to the class type for use in compares
   */
  static const type_info& getType();

 private:
  
  /**
   * The dataset represented by this data block
   */
  Dataset* dataset;

  /**
   * If the internal dataset is installed in the DatasetMap, this Boolean value is set to true
   * This indicates that the dataset memory will be managed by the DatasetMap and we shouldn't delete it.
   */
  bool wasUsed;

};

#endif
