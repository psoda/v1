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
#include "IfElse.h"
#include "LiteralExpression.h"
#include "BoolLiteral.h"
#include <assert.h>

using namespace std;

IfElse::IfElse() : PsodaConstruct(), cases() {
  IfThenPair* initialCase = new IfThenPair;
  initialCase->setCondition(new LiteralExpression(new BoolLiteral(true)));
  cases.push_back(initialCase);
}

IfElse::~IfElse() {
  //iterate through cases and destroy each case
  list<IfThenPair*>::iterator casesItr = cases.begin();
  list<IfThenPair*>::iterator casesEnd = cases.end();
  while (casesItr != casesEnd) {
    IfThenPair* thisIfThenPair = *casesItr;
    delete thisIfThenPair;
    thisIfThenPair = NULL;
    casesItr++;
  }
}

void IfElse::addNode(ProgramGraphNode* newNode) {
  assert( newNode != NULL );
  cases.back()->getBody().addNode(newNode);
}
  
void IfElse::beginElsif() {
  beginElse();
}
  
void IfElse::beginElse() {
  IfThenPair* newCase = new IfThenPair;
  newCase->setCondition(new LiteralExpression(new BoolLiteral(true)));
  cases.push_back(newCase);
}

void IfElse::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void IfElse::execute(Environment* baseEnv) {
  list<IfThenPair*>::const_iterator casesItr = cases.begin();
  list<IfThenPair*>::const_iterator casesEnd = cases.end();
  //IfThenPair* lastPair = cases.back();
  //IfThenPair* firstPair = cases.front();
  for (; casesItr != casesEnd; casesItr++) {
    IfThenPair* ifThen = *casesItr;
    if (ifThen->getCondition()->evaluate(baseEnv)->toBool()) {
      ifThen->getBody().execute(baseEnv);
      return;
    }
  }
}

void IfElse::validate() {
  list<IfThenPair*>::const_iterator casesItr = cases.begin();
  list<IfThenPair*>::const_iterator casesEnd = cases.end();
  //IfThenPair* lastPair = cases.back();
  //IfThenPair* firstPair = cases.front();
  for (; casesItr != casesEnd; casesItr++) {
    IfThenPair* ifThen = *casesItr;
    ifThen->getCondition()->validate();
    ifThen->getBody().validate();
  }
}

string IfElse::toString() const {
  return toString(0);
}

string IfElse::toString(int depth, string preText) const {
  string returnString = "";
  string tabs = getTabs(depth);
  string spaces = getSpaces(preText);
  list<IfThenPair*>::const_iterator casesItr = cases.begin();
  list<IfThenPair*>::const_iterator casesEnd = cases.end();
  IfThenPair* lastPair = cases.back();
  IfThenPair* firstPair = cases.front();
  while (casesItr != casesEnd) {
    IfThenPair* ifThen = *casesItr;
    if( (ifThen == lastPair) && (cases.size() > 1) ) {
      returnString += spaces + tabs;
      returnString += "else\n";
      returnString += ifThen->getBody().toString(depth);
      returnString += spaces + tabs;
      returnString += "endif";

      return returnString;
    } else if(ifThen == firstPair) {
      returnString += preText + tabs;
    } else {
      returnString += spaces + tabs;
    }

    //add the "els" to all if's after the first
    if( ifThen != firstPair ) {
      returnString += "els";
    }
    returnString += "if (";
    returnString += ifThen->getCondition()->toString();
    returnString += ")\n";
    returnString += ifThen->getBody().toString(depth);
    casesItr++;
  }

  returnString += spaces + tabs;
  returnString += "endif";

  return returnString;
}

void IfElse::setCondition(Expression* newCondition) {
  cases.back()->setCondition(newCondition);
}

Expression* IfElse::getCondition() const {
  return cases.back()->getCondition();
}
