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
#ifndef INLINE_USER_DEFINED_COMMAND_H
#define INLINE_USER_DEFINED_COMMAND_H

#include "UserDefinedCommand.h"

using namespace std;

class InlineUserDefinedCommand : public UserDefinedCommand {
  
 private:

  //Prohibit copying of this object
  InlineUserDefinedCommand(InlineUserDefinedCommand& other);
  void operator =(InlineUserDefinedCommand& rhs);

 public:

  /**
   * Constructor
   */
  InlineUserDefinedCommand();

  /**
   * Destructor
   */
  virtual ~InlineUserDefinedCommand();

  /**
   * Executes the body of the function
   */
  virtual void execute(Environment* baseEnv);

  /**
   * Execute the function and get the result through returnVal
   *
   * @param returnVal after calling this mehod, returnVal will point to the return value of the function
   * If nothing was returned, it will point to a singleton instance of UndefinedLiteral
   */
  virtual void execute(Environment* baseEnv, Literal*& returnVal);

  /**
   * Prints out the begin keyword (with any special markings to show that this is a macro)
   */
  virtual string getBegin() const;

};

#endif
