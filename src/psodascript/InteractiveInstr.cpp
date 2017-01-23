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
#include "InteractiveInstr.h"
#include "PsodaRunner.h"
#include "PsodaException.h"
#include "PsodaProgram.h"
#include "ProgramBlock.h"
#include "FileTools.h"
#include "Location.h"
#include "StringLiteral.h"
#include "UndefinedVariableException.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <stdio.h>

#ifdef USEREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#else
#include <stdio.h>
#endif

using namespace std;

#define START_COMMAND_PROMPT "> "
#define MID_COMMAND_PROMPT "*   "

InteractiveInstr::InteractiveInstr() : BuiltInCommand() {
  setDescription("Execute an interactive instruction.");
  initDefaultValue("code", "", "The code that you pass in via this parameter will be executed by a PSODA interpreter");
  initDefaultValue("echoCode", true, "If true, the code entered throught the \'code\' parameter will be echoed to the console");
}

InteractiveInstr::~InteractiveInstr() {
  return;
}

bool InteractiveInstr::runTopProgramOfBlocks(PsodaBlocks& blocks, Environment* baseEnv) {
  ProgramBlock* top = NULL;
  if (blocks.size() > 0) {
    if (typeid(*blocks.top()) == ProgramBlock::getType()) {
      top = ((ProgramBlock*)blocks.top());
    } else {
      return false;
    }
    try {
      if (baseEnv) {
	top->getProgram().executeInSameEnv(*baseEnv);
      } else {
	top->getProgram().execute(baseEnv);
      }
    } catch (PsodaError& e) {
      PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
      return false;
    } catch (PsodaWarning& e) {
      PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
      return false;
    }
  }
  return true;
}

void InteractiveInstr::execute(Environment* baseEnv, Literal*& returnVal) {
  try {
    interpret(baseEnv);
  } catch (PsodaReturn r) {
    returnVal = r.getReturnVal();
  }
}

void InteractiveInstr::execute(Environment* baseEnv) {
  Literal* returnVal = NULL;
  execute(baseEnv, returnVal);
}

int InteractiveInstr::checkForBreakInput() {
  return 1;
}

void InteractiveInstr::interpret(Environment* baseEnv) {
  string code = baseEnv->lookup("code").toString();
  bool echoCode = baseEnv->lookup("echoCode").toBool();
  string thisLine;
  bool firstTime = true;

#ifdef USEREADLINE
  rl_event_hook = &checkForBreakInput;
#endif
  
  try {
    while (true) {
      ostringstream programText;
      if (!firstTime) cout << endl;
      bool attemptedRun = false;
      bool doneWithCommand = false;
      while (!doneWithCommand) {
	string commandPrompt = "";
	if (attemptedRun) commandPrompt = MID_COMMAND_PROMPT;
	else if (!firstTime || (code != "" && echoCode)) commandPrompt = START_COMMAND_PROMPT;
	
	// Use the code passed in as a parameter the first time
	if (firstTime) {
	  if (echoCode) cout << commandPrompt << code;
	  thisLine = code;
	  firstTime = false;
	} else {

#ifdef USEREADLINE
	  char* fromCmdLine = NULL;
	  break_readline_input = false;
	  fromCmdLine = readline(commandPrompt.c_str());
	  if (break_readline_input) {
	    programText.str("");
	    if (fromCmdLine) {
	      free(fromCmdLine);
	      fromCmdLine = NULL;
	    }
	    break;
	  } else {
	    thisLine = fromCmdLine;
	    if (fromCmdLine) {
	      free(fromCmdLine);
	      fromCmdLine = NULL;
	    }
	  }
#else
	  cout << commandPrompt;
	  getline(cin, thisLine);
#endif
	}
      
	// Don't bother executing if they didn't enter anything
	if (thisLine != "") {
	  attemptedRun = true;
	  programText << thisLine << endl;
	  doneWithCommand = executeCode(programText.str(), baseEnv);
	}
      }
    }
  } catch (PsodaReturn& e) {
    throw e;
  } catch (PsodaException& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  }
}

bool InteractiveInstr::executeCode(const string& programText, Environment* baseEnv) {
  bool doneWithCommand = false;
  Environment* varEnv = Interpreter::getInstance()->getDefaultVarEnv();
  Location newLocation(".");
  Location* location = &newLocation;
  if (baseEnv) {
    try {
      varEnv = getVariableEnv(baseEnv);
      location = (Location*)baseEnv->lookup("*loc").getData();
    } catch (UndefinedVariableException& e) {
      varEnv = baseEnv;
    }
  }
  string baseDir = FileTools::getBaseDir(location->getPathToFile());
  const string preface = "#NEXUS\nBEGIN PSODA;\n";
  // try to run the current text if there is no error in parsing
  PsodaBlocks blocksToPopulate;
  Environment userDefinedCommandsToPopulate;
  int parseCode = PsodaRunner::parseString(baseDir, preface + programText + "\nEND;\n", blocksToPopulate, &userDefinedCommandsToPopulate);
  if (parseCode == VALID) {
    Interpreter::getInstance()->getUserDefinedCommandEnv()->setAll(&userDefinedCommandsToPopulate);
    runTopProgramOfBlocks(blocksToPopulate, varEnv);
    doneWithCommand = true;
  } else if (parseCode == INCOMPLETE) {
    doneWithCommand = false;
  } else {
    doneWithCommand = true;
  }

#ifdef USEREADLINE
  if (doneWithCommand) add_history(StringLiteral::replaceNewLines(programText.c_str()).c_str());
#endif

  return doneWithCommand;
}

string InteractiveInstr::getName() const {
  return "interactive";
}
