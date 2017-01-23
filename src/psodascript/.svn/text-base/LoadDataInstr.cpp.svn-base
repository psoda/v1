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
#include "LoadDataInstr.h"
#include "DataBlock.h"
#include "PsodaBlocks.h"
#include "PsodaPrinter.h"
#include "PsodaRunner.h"
#include "StringLiteral.h"
#include "FileTools.h"

using namespace std;

LoadDataInstr::LoadDataInstr() : BuiltInCommand() {
  setDescription("Loads a dataset from a file.  Note that only the first datablock from the file will be kept.");
  initFileDefaultValue("file", "", "The file from which the data should be read");
  initDefaultValue("name", "", "The dataset will be stored in the interpreter\'s map with the given name if name is not empty");
  initDefaultValue("makeCurrent", "yes", "If \'yes\', the loaded dataset will become the current dataset in the interpreter");
}

LoadDataInstr::~LoadDataInstr() {
  return;
}

void LoadDataInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) ) {
  execute(baseEnv);
}

void LoadDataInstr::execute(Environment* baseEnv) 
{
  string filename = baseEnv->lookup("file").toString();
  Location* location = (Location*)baseEnv->lookup("*loc").getData();
  string dir = FileTools::getBaseDir(location->getPathToFile());
  string newPathToFile = FileTools::buildPath(dir, filename);
  string dataName = baseEnv->lookup("name").toString();
  bool makeCurrent = (StringLiteral::toLowerCase(baseEnv->lookup("makecurrent").toString()) == "yes");
  try {
    Environment dummyEnv;
    PsodaBlocks blocks;
    int errorCode = PsodaRunner::parseFile(newPathToFile, blocks, &dummyEnv);
    bool foundDataBlock = false;
    //    int errorCode = PsodaParser::parse(pathToFile, blocks);
    if (errorCode) {
      return;
    } else {
      //If there was no error take the first datablock
      while (blocks.hasNext()) {
	Block* nextBlock = blocks.next();
	// We only keep the first datablock that we find
	if (!foundDataBlock && typeid(*nextBlock) == DataBlock::getType()) {
	  Dataset* thisDataset = ((DataBlock*) nextBlock)->popDataset();
	  if (dataName != "") {
	    Interpreter::getInstance()->getDatasetMap().setD(thisDataset, dataName);
	  }
	  if (makeCurrent) {
	    Interpreter::getInstance()->getDatasetMap().setCurrent(thisDataset);
	  }
	  break;
	}
      }
    }
  } catch (PsodaException& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  }
#ifdef GUI
      PsodaPrinter::getInstance()->write("## LoadData Completed Successfully\n");
#endif

}

string LoadDataInstr::getName() const {
  return "LoadData";
}
