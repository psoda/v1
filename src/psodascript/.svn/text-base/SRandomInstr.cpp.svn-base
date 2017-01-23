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
#include "SRandomInstr.h"
#include "StringLiteral.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

#ifdef WIN32
#define srandom srand
#define random rand
#endif

SRandomInstr::SRandomInstr() : BuiltInCommand() {
  setDescription("Seed the random number generator.");
  initDefaultValue("seed", "clock");
}

SRandomInstr::~SRandomInstr() {
  return;
}

void SRandomInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void SRandomInstr::execute(Environment* baseEnv) {
  int seed = 0;
  //seed with the current clock
  seed = time(NULL);
  
  const Literal& seedLit = baseEnv->lookup("seed");
  if (StringLiteral::toLowerCase(seedLit.toString()) != "clock") {
    seed = seedLit.toInt();
  }
  srand(seed);
  srandom(seed);
}

string SRandomInstr::getName() const {
  return "srandom";
}
