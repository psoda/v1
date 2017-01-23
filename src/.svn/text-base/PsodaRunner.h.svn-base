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
#ifndef PSODA_RUNNER_H
#define PSODA_RUNNER_H

#include "SignalHandlers.h"
#include "PsodaPrinter.h"
#include "PsodaBlocks.h"
#include "Interpreter.h"
#include "PsodaException.h"
#include "PsodaReturn.h"
#include "PsodaError.h"
#include "PsodaWarning.h"
#include "Environment.h"
#include <iostream>
#include <string>

using namespace std;

#define INCOMPLETE 0
#define VALID 1
#define INVALID 2

class PsodaRunner {

 private:

  static string getBaseDir(string pathToFile);
  
 public:

  static int run(const char* filename);
  static int parseString(const string& baseDir, const string& programCode, PsodaBlocks& blocksToPopulate, Environment* userDefinedCommandsToPopulate);
  static int parseFile(const string& filename, PsodaBlocks& blocksToPopulate, Environment* userDefinedCommandsToPopulate);


};

#endif

