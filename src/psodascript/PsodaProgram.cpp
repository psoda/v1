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
#include "PsodaProgram.h"
#include "PsodaPrinter.h"
#include "Interpreter.h"
#include "UndefinedVariableException.h"
#include "InvalidCastException.h"
#include "InvalidParameterListException.h"
#include "Interpreter.h"
#include "PsodaBreak.h"
#include "PsodaError.h"
#include "Globals.h"
#include "Data.h"
#include "UserDefinedCommand.h"
#include <sstream>
#include <assert.h>

using namespace std;

PsodaProgram::PsodaProgram() : PsodaConstruct(), body(), nodeStack() {
  return;
}

PsodaProgram::~PsodaProgram() {

  /* Clean up the other nodes */
  list<ProgramGraphNode*>::iterator bodyItr = body.begin();
  list<ProgramGraphNode*>::iterator bodyEnd = body.end();
  for(; bodyItr != bodyEnd; bodyItr++) {
    delete *bodyItr;
  }
  body.clear();

}

void PsodaProgram::beginNode(ProgramGraphNode* newNode) {
  assert( newNode != NULL );
  addNode(newNode);
  nodeStack.push(newNode); //keeps the node "open" for future changes
}

void PsodaProgram::endNode() {
  assert( nodeStack.size() > 0 );
  nodeStack.pop(); //"closes" an node (no more changes)
}

void PsodaProgram::addNode(ProgramGraphNode* newNode) {
  assert( newNode != NULL );
  if(nodeStack.size() > 0) {
    ((PsodaConstruct*)nodeStack.top())->addNode(newNode);
  }
  else {
    body.push_back(newNode);
  }
}

ProgramGraphNode* PsodaProgram::currentNode() const {
  if (nodeStack.size() < 1) {
    return NULL;
  } else {
    return nodeStack.top();
  }
}

void PsodaProgram::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void PsodaProgram::executeInSameEnv(Environment& newEnv) {
  //  newEnv.set("*env", new Data(&newEnv));
  
  //Check if you should execute before you start
  //if (!OKToKeepRunning) throw PsodaException("Forcing Quit...", getLocation());
  ProgramGraphNode* currentNode = NULL;
  list<ProgramGraphNode*>::const_iterator node = body.begin();
  list<ProgramGraphNode*>::const_iterator bodyEnd = body.end();
  for(; node != bodyEnd; node++ ) {
    currentNode = *node;
    try {
      ADD_LOCATION_TO_EXCEPTIONS(currentNode->execute(&newEnv);, currentNode->getLocation());
    } catch (PsodaWarning w) {
      string errorMessage = w.toString();
      PsodaPrinter::getInstance()->write("\n%s\n", w.toString().c_str());
    }
    //if (!OKToKeepRunning) throw PsodaException("Forcing Quit...", currentNode->getLocation());
  }
}

void PsodaProgram::execute(Environment* baseEnv) {
  Environment newEnv;
  newEnv.extendEnv(baseEnv);
  executeInSameEnv(newEnv);
}

void PsodaProgram::validate() {
  list<ProgramGraphNode*>::const_iterator node = body.begin();
  list<ProgramGraphNode*>::const_iterator bodyEnd = body.end();
  for(; node != bodyEnd; node++ ) {
    (*node)->validate();
  }
}

string PsodaProgram::toString() const {
  return toString(0);
}

string PsodaProgram::toString(int depth, string preText) const {
  ostringstream returnString;
  returnString.str(preText);

  /*  
  // Print out the functions
  map<string, UserDefinedCommand*>::const_iterator functionsItr = functions.begin();
  map<string, UserDefinedCommand*>::const_iterator functionsEnd = functions.end();
  for(; functionsItr != functionsEnd; functionsItr++ ) {
    UserDefinedCommand* thisFunction = functionsItr->second;
    assert(thisFunction);
    string thisString = thisFunction->toString(1);
    returnString << thisString;
    returnString << "\n";
  }
  */

  // Print out the other nodes
  list<ProgramGraphNode*>::const_iterator node = body.begin();
  list<ProgramGraphNode*>::const_iterator bodyEnd = body.end();
  for(; node != bodyEnd; node++ ) {
    ostringstream lineString;
    lineString.str();
    lineString << (*node)->getLocation().getFirstLine();
    lineString << ": ";
    returnString << (*node)->toString(depth + 1, lineString.str());
    returnString << ";\n";
  }
  return returnString.str();
}

