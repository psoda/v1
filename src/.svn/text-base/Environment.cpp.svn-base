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
#include "Environment.h"
#include "UndefinedVariableException.h"
#include "StringLiteral.h"
#include "IdentReference.h"
#include <set>
#include <typeinfo>

using namespace std;

Environment::Environment(Environment* _baseEnv) : currEnv(), baseEnv(_baseEnv) {
  return;
}

Environment::Environment() : currEnv(), baseEnv(NULL) {
  return;
}

Environment::Environment(const Environment& a) : currEnv(a.currEnv), baseEnv(a.baseEnv) {
  return;
}

Environment& Environment::operator=(const Environment& a) {
  currEnv=a.currEnv;
  baseEnv=a.baseEnv;
  return *this;
}

Environment::~Environment() {
  clear();
}

void Environment::extendEnv(Environment* _baseEnv) {
  baseEnv = _baseEnv;
}

Environment* Environment::getBaseEnv() const {
  return baseEnv;
}

bool Environment::canRead(string ident) const {
  try {
    lookup(ident);
  } catch (UndefinedVariableException& e) {
    return false;
  }
  return true;
}

bool Environment::isShallow(string ident) const {
  string lowerCaseIdent = StringLiteral::toLowerCase(ident);
  map<string, Literal*>::const_iterator itr = currEnv.find(lowerCaseIdent);
  if ( itr != currEnv.end() ) {
    return true;
  } else {
    return false;
  }
}

bool Environment::canUpdate(string ident) const {
  string lowerCaseIdent = StringLiteral::toLowerCase(ident);
  map<string, Literal*>::const_iterator itr = currEnv.find(lowerCaseIdent);
  if ( itr != currEnv.end() ) {
    //found it in the currEnv
    Literal* curLit = itr->second;
    if (isReference(curLit)) {
      IdentReference* ref = (IdentReference*)curLit;
      return ref->getEnv()->canUpdate(ref->getVar());
    } else {
      return true;
    }
  }
  else if ( baseEnv != NULL ) {
    //not found in currEnv, check the baseEnv
    return baseEnv->canUpdate(lowerCaseIdent);
  }
  else {
    return false;
  }
}

Literal& Environment::lookup(string ident) const {
  string lowerCaseIdent = StringLiteral::toLowerCase(ident);
  /*  For Debug
  map<string, Literal*>::const_iterator varItr = currEnv.begin();
  map<string, Literal*>::const_iterator varEnd = currEnv.end();
  for (; varItr != varEnd; varItr++) {
    printf("var: %s = %s\n", varItr->first.c_str(), varItr->second->toString().c_str());
  }
  */
  map<string, Literal*>::const_iterator itr = currEnv.find(lowerCaseIdent);
  if ( itr != currEnv.end() ) {
    //found it in the currEnv
    return *itr->second;
  }
  else if ( baseEnv != NULL ) {
    //not found in currEnv, check the baseEnv
    return baseEnv->lookup(lowerCaseIdent);
  }
  else {
    //not in currEnv and no baseEnv exists, so throw an exception
    string errorMessage = "Undefined Variable: ";
    errorMessage += lowerCaseIdent;
    throw UndefinedVariableException(errorMessage);
  }
}

string Environment::lookupString(string ident) const {
  if (canRead(ident)) {
    const Literal* value = &lookup(ident);
    return value->toString();
  } else {
    return "";
  }
}

void Environment::setAll(const Environment* toCopyFrom) {
  std::set<string> names;
  toCopyFrom->getAllReadableNames(names);
  std::set<string>::const_iterator namesItr = names.begin();
  std::set<string>::const_iterator namesEnd = names.end();
  for (; namesItr != namesEnd; namesItr++) {
    set(*namesItr, &(toCopyFrom->lookup(*namesItr)));
  }
}

void Environment::setAllReferences(const Environment* toCopyFrom) {
  std::set<string> names;
  toCopyFrom->getAllReadableNames(names);
  std::set<string>::const_iterator namesItr = names.begin();
  std::set<string>::const_iterator namesEnd = names.end();
  for (; namesItr != namesEnd; namesItr++) {
    set(*namesItr, &(toCopyFrom->lookup(*namesItr)), false);
  }
}

void Environment::setReference(string ident, const Literal* reference) {
  set(ident, reference, false);
}

void Environment::set(string ident, const Literal* lit, bool dereferenceLitIfReference) {
  if (dereferenceLitIfReference && isReference(lit)) {
    const IdentReference* reference = (const IdentReference*) lit;
    set(ident, &(reference->getEnv()->lookup(reference->getVar())), dereferenceLitIfReference);
  } else {
    string lowerCaseIdent = StringLiteral::toLowerCase(ident);
    map<string, Literal*>::iterator itr = currEnv.find(lowerCaseIdent);
    
    bool isRef = false;
    // check for and delete previous value
    if (itr != currEnv.end()) {
      // if the current literal is a reference, do the thing to the object of the reference
      Literal* curLit = itr->second;
      if (isReference(curLit)) {
	IdentReference* ref = (IdentReference*)curLit;
	ref->getEnv()->set(ref->getVar(), lit);
	isRef = true;
      } else {
	delete currEnv[lowerCaseIdent];
	currEnv.erase(itr);
      }
    }
    if (!isRef) {
      currEnv[lowerCaseIdent] = lit->copyToHeap();
    }
  }
}

void Environment::setString(string ident, string stringLitValue) {
  StringLiteral stringLit(stringLitValue);
  set(ident, &stringLit);
}

void Environment::update(string ident, const Literal* lit) {
  string lowerCaseIdent = StringLiteral::toLowerCase(ident);
  map<string, Literal*>::const_iterator itr = currEnv.find(lowerCaseIdent);
  if ( itr != currEnv.end() ) {
    //found it in the currEnv
    Literal* curLit = itr->second;
    // if it is a reference, update ther reference
    if (isReference(curLit)) {
      IdentReference* ref = (IdentReference*)curLit;
      ref->getEnv()->update(ref->getVar(), lit);
    } else {
      set(lowerCaseIdent, lit);
    }
  } else if ( baseEnv != NULL ) {
    //not found in currEnv, check the baseEnv
    baseEnv->update(lowerCaseIdent, lit);
  } else {
    //not in currEnv and no baseEnv exists, so throw an exception
    string errorMessage = "Undefined Variable: ";
    errorMessage += lowerCaseIdent;
    throw UndefinedVariableException(errorMessage);
  }
}

void Environment::unset(string ident) {
  string lowerCaseIdent = StringLiteral::toLowerCase(ident);

  map<string, Literal*>::iterator itr = currEnv.find(lowerCaseIdent);
  if ( itr != currEnv.end() ) {
    delete itr->second;
    currEnv.erase(itr);
  } else if ( baseEnv != NULL ) {
    baseEnv->unset(lowerCaseIdent);
  }
}

void Environment::getAllReadableNames(std::set<string>& names) const {
  map<string, Literal*>::const_iterator envItr = currEnv.begin();
  map<string, Literal*>::const_iterator envEnd = currEnv.end();
  for (; envItr != envEnd; envItr++) {
    if (canRead(envItr->first)) names.insert(envItr->first);
  }
  if (baseEnv) {
    baseEnv->getAllReadableNames(names);
  }
}

string Environment::toString() const {
  ostringstream returnStream("");
  map<string, Literal*>::const_iterator envItr = currEnv.begin();
  map<string, Literal*>::const_iterator envEnd = currEnv.end();
  while (envItr != envEnd) {
    returnStream << envItr->first << " = " << envItr->second->toCode();
    envItr++;
    if (envItr != envEnd) {
      returnStream << ", ";
    }
  }
  return returnStream.str();
}

void Environment::clear() {
  map<string, Literal*>::const_iterator envItr = currEnv.begin();
  map<string, Literal*>::const_iterator envEnd = currEnv.end();
  while (envItr != envEnd) {
    delete envItr->second; 
    envItr++;
  }
  currEnv.clear();
}

bool Environment::isReference(const Literal* lit) {
  if (lit) {
    bool returnVal = (typeid(*lit) == IdentReference::getType());
    return returnVal;
  } else {
    return false;
  }
}
