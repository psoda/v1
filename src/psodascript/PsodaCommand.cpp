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
#include "PsodaCommand.h"
#include "LiteralExpression.h"
#include "StringLiteral.h"
#include "IntLiteral.h"
#include "RealNumberLiteral.h"
#include "Interpreter.h"
#include "Environment.h"
#include "PsodaPrinter.h"
#include <cctype>
#include <string>
#include <algorithm>

using namespace std;

PsodaCommand::PsodaCommand() : PsodaElement(), currentDefaults(), readOnlyEnv(), initialDefaults(), paramDescriptions(), referencedParams() {
  setDescription("Execute a psoda command");
  currentDefaults.extendEnv(&readOnlyEnv);
  readOnlyEnv.extendEnv(&initialDefaults);
  initialDefaults.extendEnv(Interpreter::getInstance()->getGlobalEnv());
}

PsodaCommand::PsodaCommand(PsodaCommand & old): PsodaElement(old)
{
  // This should not be called
  return;
}

PsodaCommand::~PsodaCommand() {
  paramDescriptions.clear();
  referencedParams.clear();
}

void PsodaCommand::declareReference(string name) {
  referencedParams.insert(StringLiteral::toLowerCase(name));
}

const set<string>& PsodaCommand::getReferences() const {
  return referencedParams;
}

bool PsodaCommand::isReference(string name) const {
  return (referencedParams.find(StringLiteral::toLowerCase(name)) != referencedParams.end());
}

void PsodaCommand::initFileDefaultValue(string name, string filename, string description) {
  StringLiteral myLit(filename);
  initDefaultValue(name, &myLit, description);
  fileParamNames.insert(name);
}

void PsodaCommand::initBooleanDefaultValue(string name, bool value, string description) {
  const char* trueString = "yes";
  const char* falseString = "no";
  StringLiteral myLit(value ? trueString : falseString);
  initDefaultValue(name, &myLit, description);
  addParamOption(name, (!value) ? trueString : falseString);
}

void PsodaCommand::initDefaultValue(string name, double value, string description) {
  RealNumberLiteral myLit(value);
  initDefaultValue(name, &myLit, description);
}

void PsodaCommand::initDefaultValue(string name, string value, string description) {
  StringLiteral myLit(value);
  initDefaultValue(name, &myLit, description);
}

void PsodaCommand::initDefaultValue(string name, int value, string description) {
  IntLiteral myLit(value);
  initDefaultValue(name, &myLit, description);
}

void PsodaCommand::initDefaultValue(string name, const Literal* value, string description) {
  initialDefaults.set(name, value);
  paramDescriptions[name] = description;
  addParamOption(name, value);
}

void PsodaCommand::addParamOption(const string& name, double value) {
  RealNumberLiteral myLit(value);
  addParamOption(name, &myLit);
}

void PsodaCommand::addParamOption(const string& name, int value) {
  IntLiteral myLit(value);
  addParamOption(name, &myLit);
}

void PsodaCommand::addParamOption(const string& name, const string& value) {
  StringLiteral myLit(value);
  addParamOption(name, &myLit);
}

void PsodaCommand::addParamOption(const string& name, const Literal* value) {
  if (!value) return;
  string valueString = value->toCode();
  map<string, set<string> >::iterator optionsFound = paramOptions.find(name);
  if (optionsFound != paramOptions.end()) {
    optionsFound->second.insert(valueString);
  } else {
    set<string> options;
    options.insert(valueString);
    paramOptions[name] = options;
  }
}

void PsodaCommand::setDefaultValue(string name, Literal* value) {
  currentDefaults.set(name, value);
}

void PsodaCommand::resetDefaultValue(string name) {
  currentDefaults.unset(name);
}

Literal* PsodaCommand::getDefaultValue(string name) const {
  return (Literal*) &(currentDefaults.lookup(name));
}

vector<string> PsodaCommand::getDefaultValues() const {
  vector<string> returnVector;
  map<string, string>::const_iterator descriptionItr = paramDescriptions.begin();
  map<string, string>::const_iterator descriptionEnd = paramDescriptions.end();
  for (; descriptionItr != descriptionEnd; descriptionItr++) {
    returnVector.push_back(getDefaultValue(descriptionItr->first)->toCode());
  }
  return returnVector;
}

string PsodaCommand::getParamDescription(string paramName) const {
  map<string, string>::const_iterator descriptionFound = paramDescriptions.find(paramName);
  if (descriptionFound != paramDescriptions.end()) {
    return descriptionFound->second;
  } else {
    return "";
  }
}

vector<string> PsodaCommand::getParamDescriptions() const {
  vector<string> returnVector;
  map<string, string>::const_iterator descriptionItr = paramDescriptions.begin();
  map<string, string>::const_iterator descriptionEnd = paramDescriptions.end();
  for (; descriptionItr != descriptionEnd; descriptionItr++) {
    returnVector.push_back(descriptionItr->second);
  }
  return returnVector;
}

Environment* PsodaCommand::getDefaultEnvironment() {
  return &currentDefaults;
}

bool PsodaCommand::hasParam(string paramName) const {
  return initialDefaults.canRead(paramName);
}

vector<string> PsodaCommand::getParamNames() const {
  vector<string> returnVector;
  map<string, string>::const_iterator descriptionItr = paramDescriptions.begin();
  map<string, string>::const_iterator descriptionEnd = paramDescriptions.end();
  for (; descriptionItr != descriptionEnd; descriptionItr++) {
    returnVector.push_back(descriptionItr->first);
  }
  return returnVector;
}

set<string> PsodaCommand::getParamOptions(const string& paramName) const {
  map<string, set<string> >::const_iterator optionsFound = paramOptions.find(paramName);
  if (optionsFound != paramOptions.end()) {
    return optionsFound->second;
  } else {
    set<string> returnSet;
    return returnSet;
  }
}

set<string> PsodaCommand::getFileParamNames() const {
  return fileParamNames;
}

string PsodaCommand::toString() const {
  string returnString = getName();
  returnString += " (";
  returnString += initialDefaults.toString();
  returnString += ");";
  return returnString;
}

string PsodaCommand::toCode(const map<string, string>& params) const {
  string returnString = getName();
  returnString += " (";
  map<string, string>::const_iterator paramsItr = params.begin();
  map<string, string>::const_iterator paramsEnd = params.end();
  bool isFirst = true;
  for (; paramsItr != paramsEnd; paramsItr++) {
    //    if (paramsItr->second == getDefaultValue(paramsItr->first)->toCode()) continue; // Don't print the value if it is just the default value
    if (isFirst) {
      isFirst = false;
    } else {
      returnString += ", "; 
    }
    returnString += paramsItr->first;
    returnString += "="; 
		// If it is a filename, we need to duplicate backslashes
		string tmpstr = paramsItr->first;
		// Transform to upper case so we deal with all kinds of file directives
		transform(tmpstr.begin(),tmpstr.end(), tmpstr.begin(), (int(*)(int))toupper);
		string paramvalue = paramsItr->second;
		if(tmpstr == "FILE") {
		// This is the filename, so duplicate backslashes
			size_t lookHere = 0;
			size_t foundHere = 0;
			// Replace all single backslashes with double backslashes
			string from = "\\";
			string to = "\\\\";
			while((foundHere = paramvalue.find(from,lookHere)) != string::npos) {
				paramvalue.replace(foundHere, from.size(), to);
				lookHere = foundHere + to.size();
			}
		}
    returnString += paramvalue;
  }
  returnString += ");";
  return returnString;
}

string PsodaCommand::toString(int depth, string preText) const {
  string returnString = preText;
  returnString += getTabs(depth);
  returnString += toString();
  return returnString;
}

string PsodaCommand::getUsage() const {
  return toString();
}

void PsodaCommand::setDescription(const string& newDescription) {
  commandDescription = newDescription;
}

string PsodaCommand::getDescription() const {
  return commandDescription;
}

string PsodaCommand::getHelp() const {
  ostringstream helpStream;
  helpStream << "Help (" << getName() << ")" << endl;
  helpStream << "\tDescription: " << (commandDescription == "" ? "<No Description Provided>" : commandDescription) << endl;
  helpStream << "\tUsage: " << getUsage() << endl;
  helpStream << "\tParameters: " << endl;
  map<string, string>::const_iterator descriptionItr = paramDescriptions.begin();
  map<string, string>::const_iterator descriptionEnd = paramDescriptions.end();
  for (; descriptionItr != descriptionEnd; descriptionItr++) {
    string description;
    if (descriptionItr->second == "") {
      description = "<No Description Provided>";
    } else {
      description = descriptionItr->second;
    }
    helpStream << "\t\t" << descriptionItr->first << "\t:\t" << description << endl;
  }
  return helpStream.str();
}

Environment* PsodaCommand::getVariableEnv(Environment* baseEnv) {
  return (Environment*) baseEnv->lookup("*env").getData();
}
