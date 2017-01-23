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
#include "GraphInstr.h"
#include "Expression.h"
#include "Interpreter.h"

#ifdef GUI
#include "PSODA.h"
#endif

using namespace std;

GraphInstr::GraphInstr() : BuiltInCommand() {
  setDescription("Enable the graph in the GUI.");
  initDefaultValue("start", "no");
  initDefaultValue("stop", "no");
}

GraphInstr::~GraphInstr() {
  return;
}

void GraphInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void GraphInstr::execute(Environment* baseEnv
#ifndef GUI
			 __attribute__((unused))
#endif
) {
#ifdef GUI

  if (baseEnv->canRead("start")) {
    string start = baseEnv->lookupString("start");
    if (start == "yes") {
      Java_gui_PSODA_setGraphEnabled(true);
    }
  }

  if (baseEnv->canRead("stop")) {
    string stop = baseEnv->lookupString("stop");
    if (stop == "yes") {
      Java_gui_PSODA_setGraphEnabled(false);
    }
  }

#endif
}

string GraphInstr::getName() const {
  return "graph";
}
