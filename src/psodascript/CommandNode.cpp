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
#include "CommandNode.h"
#include "PsodaPrinter.h"
#include "Interpreter.h"
#include "UndefinedVariableException.h"
#include "StringLiteral.h"
#include "IntLiteral.h"
#include "Data.h"
#include "LiteralExpression.h"
#include "NameExpression.h"
#include "PsodaCommand.h"
#include "IdentReference.h"
#include <typeinfo>
#include <iostream>

using namespace std;

CommandNode::CommandNode() : ProgramGraphNode() {
  //  readOnlyEnv.extendEnv(&paramsEnv);
}

CommandNode::~CommandNode() {
  map<string, Expression*>::iterator paramItr = params.begin();
  map<string, Expression*>::iterator paramEnd = params.end();
  for (; paramItr != paramEnd; paramItr++) {
    delete paramItr->second;
  }
  params.clear();
}

void CommandNode::setCommandName(string newCommandName) {
  commandName = newCommandName;
}

string CommandNode::getCommandName() const {
  return commandName;
}

void CommandNode::validate() {
  PsodaCommand* thisCommand = getCommand();
  if (thisCommand == NULL) {
    //print an warning if the command wasn't found
    if (PSODA_VERBOSE) {
      PsodaPrinter::getInstance()->write("Warning (%s): Command not found (\"%s\"); command will be skipped\n", getLocation().toString().c_str(), commandName.c_str());
    }
  } else {
    // Validate parameter names
    map<string, Expression*>::const_iterator paramsItr = params.begin();
    map<string, Expression*>::const_iterator paramsEnd = params.end();
    for (; paramsItr != paramsEnd; paramsItr++) {
      string paramName = paramsItr->first;
      if (PSODA_VERBOSE) {
	if (!thisCommand->hasParam(paramName)) {
	  // Print a warning if a paramter is given that the command doesn't accept
	  PsodaPrinter::getInstance()->write("Warning (%s): Unused parameter (\"%s\") passed to command (\"%s\"); parameter will be ignored\n", getLocation().toString().c_str(), paramName.c_str(), thisCommand->getName().c_str());
	}
      }
      paramsItr->second->validate();
    }
  }
}

PsodaCommand* CommandNode::getCommand() const {
  PsodaCommand* thisCommand = Interpreter::getInstance()->getFunction(StringLiteral::toLowerCase(commandName));
  return thisCommand;
}

void CommandNode::execute(Environment* baseEnv) {
  Literal* myReturn;
  doExecute(baseEnv, myReturn, false);
}

void CommandNode::execute(Environment* baseEnv, Literal*& returnVal) {
  doExecute(baseEnv, returnVal, true);
}

void CommandNode::doExecute(Environment* baseEnv, Literal*& returnVal, bool shouldReturn) {
  PsodaCommand* thisCommand = getCommand();
  if (thisCommand == NULL) {
    //print an error if the command wasn't found
    PsodaPrinter::getInstance()->write("Warning (%s): Command not found (\"%s\"); skipping command\n", getLocation().toString().c_str(), commandName.c_str());
    return;
  }

  ReadOnlyEnvironment readOnlyEnv;
  Environment paramsEnv;

  //evaluate the params before changing the scope
  map<string, Expression*>::const_iterator paramsItr = params.begin();
  map<string, Expression*>::const_iterator paramsEnd = params.end();

  for (; paramsItr != paramsEnd; paramsItr++) {
    string paramName = StringLiteral::toLowerCase(paramsItr->first);
    Expression* thisValueExpr = paramsItr->second;

    bool passAsReference = false;
    // if the parameter is being passed by reference and if the expression is an ident expression, add the reference to the environment
    if (thisCommand->isReference(paramName)) {
      if (typeid(*thisValueExpr) == NameExpression::getType()) {
	string varName = StringLiteral::toLowerCase(thisValueExpr->toString());
	if (baseEnv->canUpdate(varName)) {
	  //	  printf("Assigning reference to %s\n", varName.c_str());
	  string varName = StringLiteral::toLowerCase(thisValueExpr->toString());
	  IdentReference thisReference(varName, baseEnv);
	  paramsEnv.setReference(paramName, &thisReference);
	  passAsReference = true;
	} 
      }
    }
    if (!passAsReference) {
      // otherwise, add the param to the environment
      Literal* lit = thisValueExpr->evaluate(baseEnv);
      paramsEnv.set(paramName, lit);
    }
  }

  // Put a read only env to protect the default environment
  readOnlyEnv.extendEnv(thisCommand->getDefaultEnvironment());

  // Extend the parameter results env with the defaults for the command
  paramsEnv.extendEnv(&readOnlyEnv);

  // Pass a pointer to the base environment
  Data baseEnvData(baseEnv);
  paramsEnv.set("*env", &baseEnvData);
  // Pass a pointer to the location of this command node
  Data locationData((void*)&getLocation());
  paramsEnv.set("*loc", &locationData);

  if (shouldReturn) {
    thisCommand->execute(&paramsEnv, returnVal);
  } else {
    thisCommand->execute(&paramsEnv);
  }
}

bool CommandNode::isExecutingFile(const string& pathToFile, Environment* baseEnv) {
  if (baseEnv) {
    try {
      Location* location = (Location*) baseEnv->lookup("*loc").getData();
      if (location->getPathToFile() == pathToFile) return true;
    } catch (UndefinedVariableException& e) {
      // do nothing
    }
    try {
      Environment* varEnv = (Environment*) baseEnv->lookup("*env").getData();
      return isExecutingFile(pathToFile, varEnv);
    } catch (UndefinedVariableException& e) {
      // do nothing
    } 
  }
  return false;
}

void CommandNode::buildStackTrace(ostringstream& stackTrace, Environment* baseEnv) {
  if (baseEnv) {
    try {
      Location* location = (Location*) baseEnv->lookup("*loc").getData();
      stackTrace << "\t" << location->toString() << endl;
    } catch (UndefinedVariableException& e) {
      // do nothing
    }
    try {
      Environment* varEnv = (Environment*) baseEnv->lookup("*env").getData();
      buildStackTrace(stackTrace, varEnv);
    } catch (UndefinedVariableException& e) {
      return;
    } 
  }
}

string CommandNode::paramsString() const {
  map<string, Expression*>::const_iterator paramsItr = params.begin();
  map<string, Expression*>::const_iterator paramsEnd = params.end();
  string returnString = " (";
  bool isFirst = true;
  while (paramsItr != paramsEnd) {
    if (isFirst) {
      isFirst = false;
    } else {
      returnString += ", ";
    }

    returnString += paramsItr->first;

    if (paramsItr->second != 0) {
      returnString += " = ";
      Expression* thisValueExpr = paramsItr->second;
      returnString += thisValueExpr->toString();
    }
    paramsItr++;
  }
  returnString += ")";
  return returnString;
}

void CommandNode::setParamName(string name) {
  lastAddedParamName = name;
}

void CommandNode::unsetParam(string name) {
  string lowerCaseName = StringLiteral::toLowerCase(name);
  Expression* thisVal = params[lowerCaseName];
  if (thisVal) {
    delete thisVal;
  }
  params.erase(lowerCaseName);
}

bool CommandNode::paramIsSet(string name) const {
  return (params.find(name) != params.end());
}

void CommandNode::setParamValue(string value) {
  setParameter(lastAddedParamName, value);
}

void CommandNode::setParamValue(Expression* value) {
  setParameter(lastAddedParamName, value);
}

void CommandNode::setParameter(string name, string value) {
  setParameter(name, new StringLiteral(value));
}

void CommandNode::setParameter(string name, int value) {
  setParameter(name, new IntLiteral(value));
}

void CommandNode::setParameter(string name, Literal* value) {
  setParameter(name, new LiteralExpression(value));
}

void CommandNode::setParameter(string name, Expression* value) {
  string lowerCaseName = StringLiteral::toLowerCase(name);
  if (params.find(lowerCaseName) != params.end()) {
    delete params[lowerCaseName];
    params[lowerCaseName] = NULL;
  }
  params[lowerCaseName] = value;
}

string CommandNode::toString() const {
  string returnString = commandName;
  returnString += paramsString();
  return returnString;
}

string CommandNode::toString(int depth, string preText) const {
  string returnString = preText;
  returnString += getTabs(depth);
  returnString += toString();
  return returnString;
}
