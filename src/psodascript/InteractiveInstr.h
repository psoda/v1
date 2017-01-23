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
#ifndef PSODA_INTERACTIVE_INSTR_H
#define PSODA_INTERACTIVE_INSTR_H

#include "BuiltInCommand.h"
#include "PsodaBlocks.h"
#include <string>

using namespace std;

class InteractiveInstr : public BuiltInCommand {

 private:

  //Prohibit copying of this object
  void operator =(InteractiveInstr&);
  InteractiveInstr(InteractiveInstr&);
  static bool hasReachedEnd(const string& programText);

  /**
   * Checks a global variable and breaks out of the current readline if it is true
   */
  static int checkForBreakInput();

  /**
   * This is the actual meat of the command
   */
  static void interpret(Environment* baseEnv);

 public:

  /**
   * Constructor
   */
  InteractiveInstr();

  /**
   * Destructor
   */
  virtual ~InteractiveInstr();

  /**
   * Call passParams() to set up the parameters in the interpreter
   */
  virtual void execute(Environment* baseEnv);
  virtual void execute(Environment* baseEnv, Literal*& returnVal);

  /**
   * Executes the program statements provided in the given string (the preface and final end are added inside of the method
   */
  static bool executeCode(const string& programText, Environment* baseEnv = NULL);

  /**
   * Returns the name of this instruction
   */
  virtual string getName() const;

  /**
   * Run the top block of the the provided blocks iff it is a program block
   * Catch and print any exceptions
   * If a baseEnv is provided the program will be run in that same environment (without pushing on an extra environment)
   * otherwise, the program will be interpreted as normal with a NULL base environment
   *
   * @return true on a successful run and false otherwise
   */
  static bool runTopProgramOfBlocks(PsodaBlocks& blocks, Environment* baseEnv);
  
};

#endif

