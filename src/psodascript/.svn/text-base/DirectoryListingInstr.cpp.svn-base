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
#include "DirectoryListingInstr.h"
#include "ProgramBlock.h"
#include "PsodaBlocks.h"
#include "PsodaPrinter.h"
#include "PsodaRunner.h"
#include "StringLiteral.h"
#include "FileTools.h"
#include "Interpreter.h"
#include "CommandNode.h"

using namespace std;

DirectoryListingInstr::DirectoryListingInstr() : BuiltInCommand() {
  setDescription("Prints out the contents of the current directory");
}

DirectoryListingInstr::~DirectoryListingInstr() {
  return;
}

void DirectoryListingInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) ) {
  execute(baseEnv);
}

void DirectoryListingInstr::execute(Environment* baseEnv) {
  string filename = baseEnv->lookup("file").toString();
  Location* location = (Location*)baseEnv->lookup("*loc").getData();
  PsodaPrinter::getInstance()->write("Directory listing for %s:\n", location->getPathToFile().c_str());
  
  set<
  string currentPathToFile = location->getPathToFile();
  string dir = FileTools::getBaseDir(currentPathToFile);
  string newPathToFile = FileTools::buildPath(dir, filename);
  if (CommandNode::isExecutingFile(newPathToFile, baseEnv)) {
    if (PSODA_VERBOSE) {
    }
  } else {
    try {
      Environment* currentUserDefinedCommandsEnv = Interpreter::getInstance()->getUserDefinedCommandEnv();
      PsodaBlocks blocks;
      int errorCode = PsodaRunner::parseFile(newPathToFile, blocks, currentUserDefinedCommandsEnv);
      if (errorCode) {
	return;
      } else {
	//If there was no error take the first datablock
	while (blocks.hasNext() && OKToKeepRunning) {
	  Block* nextBlock = blocks.next();
	  // We only keep the first datablock that we find
	  if (typeid(*nextBlock) == ProgramBlock::getType()) {
	    nextBlock->interpretBlock();
	  }
	}
      }
    } catch (PsodaException& e) {
      PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
    }
  }
}

string DirectoryListingInstr::getName() const {
  return "DirectoryListing";
}
