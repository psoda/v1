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
#include "PsodaRunner.h"
#include "PsodaSourceParser.h"
#include "InteractiveInstr.h"
#include "FileTools.h"
#include "Environment.h"
#include "CommandNode.h"
#include "PsodaStopException.h"
#include <stdlib.h>
#include <time.h>
#ifndef WIN32
#include <signal.h>
#endif
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>

using namespace std;

int PsodaRunner::parseFile(const string& pathToFile, PsodaBlocks& blocksToPopulate, Environment* userDefinedCommandsToPopulate) {
  string dir = FileTools::getBaseDir(pathToFile);
  string filename = FileTools::getFilename(pathToFile);
  return PsodaSourceParser::parseFilename(dir.c_str(), filename.c_str(), blocksToPopulate, userDefinedCommandsToPopulate);
}

int PsodaRunner::parseString(const string& baseDir, const string& programCode, PsodaBlocks& blocksToPopulate, Environment* userDefinedCommandsToPopulate) {
  int returnCode = INVALID;
  /*
  string tempName = baseDir;
  tempName += "/interactiveprogram";
  ostringstream filenameStream;
  
  // find the first file that is not taken
  for (int i = 0; i < INT_MAX; i++) {
    filenameStream.str("");
    filenameStream << tempName << i << ".nex~";

    if (access(filenameStream.str().c_str(), F_OK) < 0) {
      break;
    }
  }

  ofstream myOutFile(filenameStream.str().c_str());
  myOutFile << "#NEXUS\nBEGIN PSODA;" << endl;
  myOutFile << programCode;
  myOutFile << "END;" << endl;
  myOutFile.close();
  */
  int errorCode = 0;
  int parsed = true;
  try {
    errorCode = PsodaSourceParser::parseBuffer(baseDir.c_str(), programCode, blocksToPopulate, userDefinedCommandsToPopulate);
  } catch (PsodaException& e) {
    parsed = false;
    // Only print out an error message if the error wasn't because of a premature end
    static string okayError1 = "syntax error, unexpected ENDSY";
    static int okayError1Length = okayError1.length();
    static string okayError2 = "expecting ENDSY";
    static string okayError3 = "expecting SEMISY";
    string givenErrorMessage = e.what();
    int messageLength = givenErrorMessage.length();
    if (messageLength >= okayError1Length && !givenErrorMessage.substr(0, okayError1Length).compare(okayError1) ||
    	givenErrorMessage.rfind(okayError2) != string::npos ||
    	givenErrorMessage.rfind(okayError3) != string::npos) {
      returnCode = INCOMPLETE;
    } else {
      PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
    }
  }
  if (!errorCode && parsed) {
    returnCode = VALID;
  }
  /*
  myOutFile.close();

  if (remove(filenameStream.str().c_str()) < 0) {
    PsodaPrinter::getInstance()->write("Warning: %s\n", strerror(errno));
  }
  */
  return returnCode;
}

int PsodaRunner::run(const char* pathToFile) {
  setDebug(0,0);  
  bool parsedFile = false;
  //PsodaPrinter::getInstance()->write("PsodaRunner Running on %s\n", pathToFile);
  try {
    //Startup Initialization
    if (!(AlgorithmFlags & BATCHMODE)) {
      RegisterSignalHandlers();
    }
    //    Interpreter::getInstance()->clear();
    srand( time (NULL) );
    
    if(!strcmp(pathToFile, "")) {
      CommandNode interactiveCommand;
      interactiveCommand.setCommandName("interactive");
      interactiveCommand.execute(Interpreter::getInstance()->getGlobalEnv());
    } else {
      PsodaBlocks blocks;
      string dir = FileTools::getBaseDir(pathToFile);
      int errorCode = parseFile(pathToFile, blocks, Interpreter::getInstance()->getUserDefinedCommandEnv());
      //    int errorCode = PsodaParser::parse(pathToFile, blocks);
      if (errorCode) {
        exit( -1 );
      } else {
        //If there was no error, interpret the blocks
        parsedFile = true;
        Interpreter::getInstance()->setBaseDir(dir);
        Interpreter::getInstance()->interpret(&blocks);
#ifndef GUI
        CommandNode interactiveCommand;
        interactiveCommand.setCommandName("interactive");
        interactiveCommand.execute(Interpreter::getInstance()->getGlobalEnv());
#endif
      }
    }
  } catch (PsodaReturn& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  } 
  catch (PsodaStopException& e)
  {
	PsodaPrinter::getInstance()->write("PSODA Stopped\n");
  }
#ifndef WIN32
  catch (PsodaError& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
    if (parsedFile) kill(0, SIGILL);
  } 
#endif
  catch (PsodaWarning& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  } catch (PsodaException& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  }
  
  //fprintf(stderr, "Completed PsodaRunner:run()\n");
  return 0;
}
