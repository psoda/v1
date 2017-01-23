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
#include "DataBlock.h"
#include "Interpreter.h"
#ifdef GUI
#include "PSODA.h"
#endif

using namespace std;

DataBlock::DataBlock(BlockType blockType) : Block(), dataset(new Dataset(blockType)), wasUsed(false) {
  //  printf("Creating a datablock\n");
  /*
    if( mBlockType == SS_BLOCK){
    Interpreter::getInstance()->setSSDataset((SSDataset*) &dataset);
    }else if( mBlockType == REF_SS_BLOCK){
    Interpreter::getInstance()->setRefSSDataset((SSDataset*) &dataset);
  }else if( mBlockType == REF_BLOCK){
  Interpreter::getInstance()->setRefDataset(&dataset);
  }else{
  Interpreter::getInstance()->setDataset(&dataset);
  }
  */
}

DataBlock::~DataBlock() {
  if (dataset && !wasUsed) {
    delete dataset;
    dataset = NULL;
  }
}

Dataset& DataBlock::getDataset() {
  return *dataset;
}

Dataset* DataBlock::popDataset() {
  Dataset* tempDataset = dataset;
  dataset = NULL;
  return tempDataset;
}

void DataBlock::interpretBlock() {
  BlockType type = dataset->blocktype();
  if( type == SS_BLOCK){
    Interpreter::getInstance()->installSSDataset((SSDataset*) dataset);
  }else if( type == REF_SS_BLOCK){
    Interpreter::getInstance()->installRefSSDataset((SSDataset*) dataset);
  }else if( type == REF_BLOCK){
    Interpreter::getInstance()->installRefDataset(dataset);
  }else{
    Interpreter::getInstance()->installDataset(dataset);
  }
  wasUsed = true;
  //  printf("Dataset: \n%s\n", Interpreter::getInstance()->dataset()->getSequenceData()->toString().c_str());
}

const type_info& DataBlock::getType() {
  static DataBlock dataBlock;
  static const type_info& info = typeid(dataBlock);
  return info;
}
