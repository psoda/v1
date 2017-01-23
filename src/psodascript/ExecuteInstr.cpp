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
#include "ExecuteInstr.h"
#include "InvalidParameterListException.h"
#include "Interpreter.h"
#include <vector>
#include "Data.h"
#include "Expression.h"

#ifndef WIN32
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace std;

ExecuteInstr::ExecuteInstr() : BuiltInCommand() {
  setDescription("Execute an external program\n");
  initDefaultValue("command", "");
}

ExecuteInstr::~ExecuteInstr() {
  return;
}

void parseCommand(string command, vector<string>& paramList) {
  int commandLength = command.length();
  string currentString = "";
  bool quoteOn = false;
  for (int i = 0; i < commandLength; i++) {
    char thisChar = command[i];
    if (thisChar == '\"') {
      //      currentString += command[i];
      if (quoteOn) {
	quoteOn = false;
	paramList.push_back(currentString);
	currentString = "";
      } else {
	quoteOn = true;
      }
    } else if (thisChar == '\\') {
      i++;
      if (i < commandLength) {
	currentString += command[i];
      }
    } else if (thisChar == ' ' && !quoteOn) {
	paramList.push_back(currentString);
	currentString = "";
    } else {
      currentString += command[i];
    }
  }
}

void ExecuteInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void ExecuteInstr::execute(Environment* baseEnv __attribute__((unused)) ) {
#ifndef WIN32
  string command = baseEnv->lookup("command").toString();

  vector<string> paramList;
  parseCommand(command, paramList);
  vector<string>::iterator paramItr = paramList.begin();
  vector<string>::iterator paramEnd = paramList.end();

  /*
  for (; paramItr != paramEnd; paramItr++) {
    printf("[%s]\n", paramItr->c_str());
  }
  */

  int argc = paramList.size();
  if (argc < 1) {
    throw InvalidParameterListException("Wrong Number of Parameters");
  }
 
  string dir = Interpreter::getInstance()->getBaseDir();
  chdir(dir.c_str());
  string file = "";
  //  file += (*expressionsToExecute)[0]->evaluate(baseEnv)->toString();
  file += paramList[0];
  char* path = strdup(file.c_str());

  char** argv = new char*[argc + 1];
  char* env = 0;
  argv[0] = path;
  for (int i = 1; i < argc; i++) {
    argv[i] = strdup(paramList[i].c_str());
    //    printf("argv[%d] = %s\n", i, argv[i]);
  }
  argv[argc] = NULL;
  //  int pipeHandles[2];
  //  pipe(pipeHandles);
  pid_t pid = fork();
  if (pid == 0) {
    //    printf("starting\n");
    if (execve(path, argv, &env) < 0) {
      //execute failed
    }
  } else {
    int status;
    while (waitpid(pid, &status, 0) <= 0);
    //    printf("done.\n");
  }

  for (int i = 1; i < argc; i++) {
    delete argv[i];
  }
  delete[] argv;

#endif
}

string ExecuteInstr::getName() const {
  return "execute";
}
