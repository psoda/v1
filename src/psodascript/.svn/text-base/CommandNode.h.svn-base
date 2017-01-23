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
#ifndef PSODA_COMMAND_NODE_H
#define PSODA_COMMAND_NODE_H

#include "ProgramGraphNode.h"
#include "Expression.h"
#include "Environment.h"
#include "ReadOnlyEnvironment.h"
#include "PsodaCommand.h"
#include <string>
#include <map>

using namespace std;

class CommandNode : public ProgramGraphNode {

 private:
  
  //Prohibit copying of this object
  CommandNode(CommandNode& other);
  void operator =(CommandNode& rhs);
  
  //  Environment paramsEnv;
  //  ReadOnlyEnvironment readOnlyEnv;

  /**
   * Store instr params
   */
  map<string, Expression*> params;

  /**
   * The last param name that was set
   */
  string lastAddedParamName;
  
  /**
   * The name of the command to be executed
   */
  string commandName;
  
  /**
   * Get the PsodaCommand to which this node points
   */
  PsodaCommand* getCommand() const;

  /**
   * Actually does the execute, and sets the returnVal if shouldReturn is true
   */
  void doExecute(Environment* baseEnv, Literal*& returnVal, bool shouldReturn);

 protected:
  
  /**
   * Iterates through the params and returns a string of the following format
   * (newline)(tab)name = value
   */
  string paramsString() const;
  
  /**
   * Returns true if the parameter with the given name has been set
   */
  bool paramIsSet(string name) const;
  
 public:
  
  /**
   * Constructor
   */
  CommandNode();

  /**
   * Destructor
   */
  virtual ~CommandNode();

  /**
   * Sets the name of this command
   */
  void setCommandName(string commandName);

  /**
   * Gets the name of this command
   */
  string getCommandName() const;

  /**
   * Set the name/value pair to the elements params
   */
  void setParamName(string name);

  /**
   * Set the value of the last set param name (A new LiteralExpression is create with a new StringLiteral)
   */
  void setParamValue(string value);

  /**
   * Set the value of the last set param name
   */
  void setParamValue(Expression* value);

  /**
   * Add a param name and value (A new LiteralExpression is create with a new StringLiteral)
   */
  void setParameter(string name, string value);

  /**
   * Add a param name and value (A new LiteralExpression is create with a new IntLiteral)
   */
  void setParameter(string name, int value);

  /**
   * Add a param name and value (A new LiteralExpression is create with the Literal*)
   */
  void setParameter(string name, Literal* value);

  /**
   * Add a param name and value
   */
  void setParameter(string name, Expression* value);

  /**
   * Removes the parameter from params
   */
  void unsetParam(string name);

  /**
   * Execute the function and get the result through returnVal
   *
   */
  void execute(Environment* baseEnv);

  /**
   * Execute the function and get the result through returnVal
   *
   * @param returnVal after calling this mehod, returnVal will point to the return value of the function
   * If nothing was returned, it will point to a singleton instance of UndefinedLiteral
   */
  void execute(Environment* baseEnv, Literal*& returnVal);

  /**
   * Searches through the environment stack to see if the current file is already being executed
   */
  static bool isExecutingFile(const string& pathToFile, Environment* baseEnv);

  /**
   * Recursively builds a string stream with the stack trace
   */
  static void buildStackTrace(ostringstream& stackTrace, Environment* baseEnv);
  
  /**
   * Performs possible validation in self and children
   */
  void validate();

  /**
   * Converts the object to its equivalent in psoda program code
   */
  string toString() const;
  
  string toString(int depth, string preText = "") const;

};

#endif
