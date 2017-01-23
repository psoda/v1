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
#include "WhileLoop.h"
#include "PsodaBreak.h"
#include "Conditional.h"
#include <assert.h>
#include <sstream>

using namespace std;

WhileLoop::WhileLoop() : PsodaConstruct(), condition(NULL), body() {
  return;
}

WhileLoop::~WhileLoop() {
  if (condition) {
    delete condition;
    condition = NULL;
  }
}

void WhileLoop::addNode(ProgramGraphNode* newNode) {
  assert( newNode != NULL );
  body.addNode(newNode);
}

void WhileLoop::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void WhileLoop::execute(Environment* baseEnv) {
  while (getCondition()->evaluate(baseEnv)->toBool()) {
    try {
      body.execute(baseEnv);
    } catch (PsodaBreak b) {
      break;
    }
  }
}

void WhileLoop::validate() {
  getCondition()->validate();
  body.validate();
}

Expression* WhileLoop::getCondition() const {
  return condition;
}

void WhileLoop::setCondition(Expression* newCondition) {
  condition = newCondition;
}

string WhileLoop::toString() const {
  return toString(0);
}

string WhileLoop::toString(int depth, string preText) const {
  string tabs = getTabs(depth);
  stringstream returnString;
  returnString << preText << tabs << "while (" << getCondition()->toString() << ")" << endl;
  returnString << body.toString(depth, preText);
  returnString << getSpaces(preText) << tabs << "endWhile";
  return returnString.str();
}
