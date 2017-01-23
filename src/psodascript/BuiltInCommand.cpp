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
#include "BuiltInCommand.h"
#include "StringLiteral.h"
#include <iostream>
#include <fstream>

using namespace std;

//#define COMMAND_HELP_DIR "doc/help/commands/"
//#define INPUT_BUF_SIZE 1024

BuiltInCommand::BuiltInCommand() : PsodaCommand(), result(NULL) {
  return;
}

BuiltInCommand::BuiltInCommand(BuiltInCommand& old):PsodaCommand(old) {
  return;
}

BuiltInCommand::~BuiltInCommand() {
  setResult(NULL);
}

void BuiltInCommand::setResult(Literal* newResult) {
  if (result) {
    delete result;
    result = NULL;
  }
  result = newResult;
}

Literal* BuiltInCommand::getResult() {
  return result;
}

string BuiltInCommand::getHelp() const {
  ostringstream helpStream;
  helpStream << PsodaCommand::getHelp();
  /*
  string helpFilename = COMMAND_HELP_DIR + StringLiteral::toLowerCase(getName()) + ".txt";
  ifstream helpFile(helpFilename.c_str(), ios::in);
  if (helpFile.good()) {
    char inBuf[INPUT_BUF_SIZE];
    do {
      helpFile.read(inBuf, INPUT_BUF_SIZE);
      int numRead = helpFile.gcount();
      inBuf[numRead] = '\0';
      helpStream << inBuf;
    } while (helpFile.good());
    helpFile.close();
  }
  */
  return helpStream.str();
}

