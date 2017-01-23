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
#ifndef BUILT_IN_COMMAND_H
#define BUILT_IN_COMMAND_H

#include "PsodaCommand.h"
#include "Environment.h"
#include "Literal.h"

using namespace std;

class BuiltInCommand : public PsodaCommand {
  
 private:

  /**
   * If this command returns a value, the result will be held here until the next time this command is called
   */
  Literal* result;

 protected:
  //Prohibit copying of this object
  BuiltInCommand(BuiltInCommand& other);
  void operator =(BuiltInCommand& rhs);

  /**
   * If there was a previous result, deletes and replaces it
   */
  void setResult(Literal* newResult);

  /**
   * Gets the value of result
   */
  Literal* getResult();

  /**
   * Returns a string containing the user documentation for this command
   */
  virtual string getHelp() const;

 public:

  /**
   * Constructor
   */
  BuiltInCommand();

  /**
   * Destructor
   */
  virtual ~BuiltInCommand();

};

#endif
