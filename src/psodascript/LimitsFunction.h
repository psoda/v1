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
#ifndef PSODA_LIMITS_FUNCTION_H
#define PSODA_LIMITS_FUNCTION_H

#include "BuiltInCommand.h"

using namespace std;

/**
 * Function returns the portion of the provided string between the start and end
 */
class LimitsFunction : public BuiltInCommand {

 public:

  /**
   * Constructor
   */
  LimitsFunction();

  /**
   * Destructor
   */
  virtual ~LimitsFunction();

  /**
   * Execute the function and get the result through returnVal
   *
   * @param returnVal after calling this mehod, returnVal will point to the return value of the function
   * If nothing was returned, returnVal is set to NULL
   */

  virtual void execute(Environment* baseEnv);
  virtual void execute(Environment* baseEnv, Literal*& returnVal);

  /**
   * Returns the name of this instruction
   */
  virtual string getName() const;

};

#endif
