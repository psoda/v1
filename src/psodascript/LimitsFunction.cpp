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
#include "LimitsFunction.h"
#include "StringLiteral.h"
#include "IntLiteral.h"
#include "RealNumberLiteral.h"
#include "UndefinedLiteral.h"
#include <stdlib.h>
#include <assert.h>

using namespace std;

LimitsFunction::LimitsFunction() : BuiltInCommand() {
  setDescription("Set limits for variables.");
  initDefaultValue("bound", "max", "Which bound is being requested (either \'max\' or \'min\')");
  initDefaultValue("type", "int", "Which data type is begin described (either \'int\' or \'double\')");
}

LimitsFunction::~LimitsFunction() {
  return;
}

void LimitsFunction::execute(Environment* baseEnv)
{
  Literal* temp = NULL;
  execute(baseEnv,temp);
}

void LimitsFunction::execute(Environment* baseEnv, Literal*& returnVal) {
  string bound = StringLiteral::toLowerCase(baseEnv->lookup("bound").toString());
  string type = StringLiteral::toLowerCase(baseEnv->lookup("type").toString());
  if (type == "int" && bound == "max") setResult(IntLiteral::getMax().copyToHeap());
  else if (type == "int" && bound == "min") setResult(IntLiteral::getMin().copyToHeap());
  else if (type == "double" && bound == "max") setResult(RealNumberLiteral::getMax().copyToHeap());
  else if (type == "double" && bound == "min") setResult(RealNumberLiteral::getMin().copyToHeap());
  else setResult(new UndefinedLiteral);
  returnVal = getResult();
}

string LimitsFunction::getName() const {
  return "Limits";
}
