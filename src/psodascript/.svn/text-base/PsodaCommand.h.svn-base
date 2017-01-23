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
#ifndef PSODA_COMMAND_H
#define PSODA_COMMAND_H

#include "PsodaElement.h"
#include "Expression.h"
#include "ReadOnlyEnvironment.h"
#include "Literal.h"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <assert.h>

using namespace std;

class PsodaCommand : public PsodaElement {
  

protected:
  //Prohibit copying of this object
  PsodaCommand(PsodaCommand& other);
  void operator =(PsodaCommand& rhs);

  /**
   * Retrieves the variable scope environment from the baseEnvironment (the variable environment is stored
   * as the parameter *env in the baseEnv before it is passed from CommandNode::execute to BuiltInCommand::execute
   */
  static Environment* getVariableEnv(Environment* baseEnv);

  /**
   * Sets the description of the command (for use by commands such as Help)
   */
  void setDescription(const string& commandDescription);

 private:
  /**
   * Default values should be defined for every parameter that may be used by the execution of the command
   * When parameters are passed, their names are also check against the names defined in defaults; if a
   * parameter that has no default value is set, a warning will be issued
   */
  Environment currentDefaults;
  ReadOnlyEnvironment readOnlyEnv;
  Environment initialDefaults;

  /**
   * The general description of what the command does
   */
  string commandDescription;

  /**
   * The description of what the parameters are for
   */
  map<string, string> paramDescriptions;

  /**
   * The possible values (unparsed, these are just strings) of the parameter [if the command creator chooses to enumerate]
   */
  map<string, set<string> > paramOptions;

  /**
   * A set of the parameter names that should be passed by reference
   * 
   * If the defaults environment contains a value for these params, they will be optional and will get those values rather than be treated as references should one of them not be passed in to the command
   */
  set<string> referencedParams;

  /**
   * The names of the params that should be treated as filenames (this is especially for the gui which will the display a file chooser for these params)
   */
  set<string> fileParamNames;

 public:

  /**
   * Set a default value for this parameter and specify that the parmeter should be displayed with a file chooser from the gui
   */
  void initFileDefaultValue(string name, string filename = "", string description = "");

  /**
   * Set a default value for this parameter and specify that the parmeter should accept values of "yes" or "no" (this should only be called in the constructor)
   */
  void initBooleanDefaultValue(string name, bool value, string description = "");

  /**
   * Set a default value for this variable (this should only be called in the constructor)
   */
  void initDefaultValue(string name, double value, string description = "");

  /**
   * Set a default value for this variable (this should only be called in the constructor)
   */
  void initDefaultValue(string name, string value, string description = "");

  /**
   * Set a default value for this variable (this should only be called in the constructor)
   */
  void initDefaultValue(string name, int value, string description = "");

  /**
   * Add a default value for this variable (this should only be called in the constructor)
   */
  void initDefaultValue(string name, const Literal* value, string description = "");

  /**
   * Identify an additional possible value for the parameter (this should only be called in the constructor)
   * The value here should be as a string as it would be entered in a PsodaScript program
   */
  void addParamOption(const string& name, const string& value);
  void addParamOption(const string& name, int value);
  void addParamOption(const string& name, double value);
  void addParamOption(const string& name, const Literal* value);

  /**
   * Declare that the param with the given name should be passed by reference
   */
  void declareReference(string name);

  /**
   * Returns a reference to the set of the declared references for this command
   */
  const set<string>& getReferences() const;

  /**
   * Returns true is the ident with the given name has been specified as a reference, false otherwise
   */
  bool isReference(string name) const;

  /**
   * Constructor
   */
  PsodaCommand();

  /**
   * Destructor
   */
  virtual ~PsodaCommand();

  /**
   * Add a default value for this variable (this should only be called in the constructor)
   */
  void setDefaultValue(string name, Literal* value);

  /**
   * Get the default value for the given parameter
   */
  Literal* getDefaultValue(string name) const;

  /**
   * Get the default valuse for all parameters
   */
  vector<string> getDefaultValues() const;

  /**
   * Get the description of the given parameter
   */
  string getParamDescription(string paramName) const;

  /**
   * Get the descriptions of all parameters
   */
  vector<string> getParamDescriptions() const;

  /**
   * Reset the default value for the given parameter to the inital default value
   */
  void resetDefaultValue(string name);

  /**
   * Return the environment that represents the default values for this command
   */
  Environment* getDefaultEnvironment();

  /**
   * Returns true if the command has an initial default value specified for the given param name
   */
  bool hasParam(string paramName) const;

  /**
   * Returns a vector holding the command names recognized by this command
   */
  vector<string> getParamNames() const;

  /**
   * Returns a set containing any parameter options that have be specified
   */
  set<string> getParamOptions(const string& paramName) const;

  /**
   * Returns a set containing the names of any parameters that have been marked as file paramters
   */
  set<string> getFileParamNames() const;

  /**
   * Returns the name of this command (as it is called from the PSODA code) ie. log, random, hsearch, etc.
   */
  virtual string getName() const = 0;

  /**
   * Returns a string containing the user documentation for this command
   */
  virtual string getHelp() const;

  /**
   * Returns a string containing the syntax for calling this command
   */
  virtual string getUsage() const;

  /*
   * Returns a string containing a description of the command
   */
  string getDescription() const;

  /**
   * Converts the object to its equivalent in psoda program code
   */
  virtual string toString() const;

  /**
   * Converts the object to its equivalent in psoda program code with the given paramNames and paramValues
   */
  virtual string toCode(const map<string, string>& params) const;

  /**
   * Prints the element out at the given indentation depth
   */
  virtual string toString(int depth, string preText = "") const;

};

#endif

