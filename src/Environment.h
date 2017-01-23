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
#ifndef PSODA_ENVIRONMENT
#define PSODA_ENVIRONMENT

#include "Literal.h"
#include <map>
#include <set>
#include <string>

using namespace std;

/**
 * The identifier names are case insensitive and are stored as lower case
 */
class Environment {
  
 private:
  map<string, Literal*> currEnv;
  Environment* baseEnv;
  
 public:
  /**
   * Constructors
   */
  Environment();

  /**
   * Constructs the environment and extends the given base environment
   */
  Environment(Environment* baseEnv);
  
  Environment(const Environment& other);
  Environment& operator =(const Environment& rhs);
  
  /**
   * Destructor  
   */
  virtual ~Environment();
  
  /**
   * Sets this env to extend the given base env
   *
   * @param _baseEnv The environment which will be extended by the current
   */
  void extendEnv(Environment* _baseEnv);
  
  /**
   * Returns a pointer to the base Environment
   *
   * @returns A pointer to the environment that this environment extends
   */
  Environment* getBaseEnv() const;
  
  /**
   * Lookup the the given identifier and return the associated literal value
   * Storage and look up are done in a case insensitive manner 
   *
   * @param ident The identifier to lookup
   * @return The literal value which the given identifier represents
   * @throws UndefinedVariableException if the identifier is not found in the environment
   */
  Literal& lookup(string ident) const;
  
  /**
   * This version of lookup doesn't throw UndefinedVariableException.  It will return empty string if the ident is not set.
   * If the ident is set, calls toString() on the Literal value and returns the string
   *
   * @param ident The identifier to lookup
   * @return The string value which the given identifier represents or empty string ("") if the ident is undefined
   */
  string lookupString(string ident) const;

  /**
   * Sets all of the names and values that are currently visible from the toCopyFrom
   */
  void setAll(const Environment* toCopyFrom);

  /**
   * Sets all of the names and values that are currently visible from the toCopyFrom (not dereferencing references)
   */
  void setAllReferences(const Environment* toCopyFrom);

  /**
   * Rather than seting the dereferenced value of the reference (as the regular set method would do),
   * this method calls set with the dereferenceLitIfReference flag set to false
   *
   */
  void setReference(string ident, const Literal* reference);

  /**
   * Set the given env element in the current env
   *
   * @param ident Name of the new env element
   * @param lit Value of the new env element
   * @param dereferenceLitIfReference True by default, this makes it so that if the lit is a reference, the value assigned in the set will be the dereferenced value of the reference rather than an identical reference
   */
  virtual void set(string ident, const Literal* lit, bool dereferenceLitIfReference = true);

  /**
   * A StringLiteral will be created and set as the value of ident
   */
  virtual void setString(string ident, string stringLitValue);
  
  /**
   * Looks for the upper most declaration of the ident and updates its value to lit
   *
   * @param ident Name of the new env element
   * @param lit Value of the new env element
   * @throws UndefinedVariableException if the identifier is not found in the environment
   */
  virtual void update(string ident, const Literal* lit);
  
  /**
   * Returns true if the given ident already exists and can be read
   * false otherwise
   */
  virtual bool canRead(string ident) const;
  
  /**
   * Returns true if the given ident already exists and can be updated
   */
  virtual bool canUpdate(string ident) const;
  
  /**
   * Returns true if the given ident exists in the innermost scope
   */
  virtual bool isShallow(string ident) const;

  /**
   * Erases the given env element in the first found env
   *
   * @param ident Name of the new env element
   */
  virtual void unset(string ident);
  
  /**
   * Prints the top level variables and values
   */
  string toString() const;
  
  /**
   * Erases all env elements in this top env
   */
  virtual void clear();
  
  /**
   * Populates the set of defined variable names
   */
  virtual void getAllReadableNames(std::set<string>& names) const;
  
  /**
   * @return true if the given literal pointer points to a IdentReference
   */
  static bool isReference(const Literal* lit);

};

#endif
