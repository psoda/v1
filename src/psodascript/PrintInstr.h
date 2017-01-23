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
#ifndef PSODA_PRINT_INSTR_H
#define PSODA_PRINT_INSTR_H

#include <vector>
#include "BuiltInCommand.h"

using namespace std;

class PrintInstr : public BuiltInCommand {

 private:

  //Prohibit copying of this object
  void operator =(PrintInstr&);
  PrintInstr(PrintInstr&);

 public:

  /**
   * Constructor
   */
  PrintInstr();

  /**
   * Destructor
   */
  virtual ~PrintInstr();

  /**
   * Call passParams() to set up the parameters in the interpreter
   */
  virtual void execute(Environment* baseEnv);
  virtual void execute(Environment* baseEnv, Literal*& returnVal);

  /**
   * Returns the name of this instruction
   */
  virtual string getName() const;

  /**
   * Returns a string containing the syntax for calling this command
   */
  virtual string getUsage() const;

  /**
   * Converts the object to its equivalent in psoda program code with the given paramNames and paramValues
   */
  virtual string toCode(const map<string, string>& params) const;

};

#endif

