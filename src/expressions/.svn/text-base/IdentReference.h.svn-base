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
#ifndef PSODA_IDENT_REFERENCE_H
#define PSODA_IDENT_REFERENCE_H

#include "Literal.h"
#include "Environment.h"
#include <string>
#include <typeinfo>

using namespace std;

class IdentReference : public Literal {

 private:

  /**
   * The name that this reference is aliasing
   */
  string name;

  /**
   * The environment where the actual variable is stored
   */
  Environment* evalEnv;

 public:

  /**
   * Contructor with initial value
   */
  IdentReference(string initialValue, Environment* evalEnv);

  virtual Literal& operator= (const Literal&);

  /**
   * Destructor
   */
  virtual ~IdentReference();
  
  /**
   * Returns a string representation of the code that made the literal
   *
   * The default is to call toString();
   */
  virtual string toCode() const;
  
  /**
   * Returns a real number representation of the literal
   */
  virtual double toRealNumber() const;
  
  /**
   * Returns a boolean representation of the literal
   */
  virtual bool toBool() const;
  
  /**
   * Returns an integer representation of the literal
   */
  virtual int toInt() const;
  
  /**
   * Returns a string representation of the literal
   */
  virtual string toString() const;

  /**
   * Returns the QTree pointer stored in this literal (if it is a TreeLiteral)
   */
  virtual QTree* toTree() const;

  /**
   * Returns the data pointer
   */
  virtual void* getData() const;

  /**
   * Creates a new copy of this Reference on the heap and returns it
   */
  virtual IdentReference* copyToHeap() const;

  /**
   * Returns the environment into which this reference points
   */
  Environment* getEnv() const;

  /**
   * Returns the var name to which this reference points
   */
  string getVar() const;

  /**
   * Returns a reference to the class type for use in compares
   */
  static const type_info& getType();

};

#endif
