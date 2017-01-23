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
#ifndef USER_DEFINED_COMMAND_H
#define USER_DEFINED_COMMAND_H

#include "PsodaCommand.h"
#include "LocatableObject.h"
#include "Codeable.h"
#include "PsodaProgram.h"

using namespace std;

class UserDefinedCommand : public PsodaCommand, public LocatableObject, public Codeable {
  
 private:

  //Prohibit copying of this object
  UserDefinedCommand(UserDefinedCommand& other);
  void operator =(UserDefinedCommand& rhs);

  /**
   * The name of this command
   */
  string name;

  /**
   * The body of the user defined command
   */
  PsodaProgram body;

 protected:

  /**
   * Prints out the begin keyword
   */
  virtual string getBegin() const;

 public:

  /**
   * Constructor
   */
  UserDefinedCommand();

  /**
   * Destructor
   */
  virtual ~UserDefinedCommand();

  /**
   * Sets the name of the instruction
   *
   * @param newInstructionName The name for the instruction
   */
  virtual void setName(string newInstructionName);

  /**
   * Gets the name of the instruction
   *
   * @return The name of the instruction
   */
  virtual string getName() const;

  /**
   * Returns a reference to the program graph stored in the user defined command
   */
  virtual PsodaProgram& getBody();

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
   * Prints the function to a string
   */
  virtual string toString() const;

  /**
   * Prints the source code of the UserDefinedInstruction to a string at the given indentation depth
   */
  virtual string toString(int depth, string preText = "") const;

};

#endif
