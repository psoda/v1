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
#include "TimeFunction.h"
#include "IntLiteral.h"
#include <time.h>
#include <assert.h>

using namespace std;

TimeFunction::TimeFunction() : BuiltInCommand() {
  setDescription("Get the current time.");
  return;
}

TimeFunction::~TimeFunction() {
  return;
}

void TimeFunction::execute(Environment* baseEnv)
{
	Literal* temp = NULL;
	execute(baseEnv,temp);
}

void TimeFunction::execute(Environment* baseEnv __attribute__ ((unused)) , Literal*& returnVal) {
  int returnTime = time(NULL);
  setResult(new IntLiteral(returnTime));
  returnVal = getResult();
}

string TimeFunction::getName() const {
  return "time";
}
