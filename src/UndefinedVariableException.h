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
#ifndef PSODA_UNDEFINED_VARIABLE_EXCEPTION
#define PSODA_UNDEFINED_VARIABLE_EXCEPTION

#include "PsodaException.h"
#include <string>

using namespace std;

/**
 * This exception should be thrown when there is an attempt to look
 * the value of a variable that has not been defined in the current environment
 *
 * If such an exception is caught, possible ways to handle it would be to
 * print an error and exit or print a warning and assume a default value
 */
class UndefinedVariableException : public PsodaException {

 public:
  /**
   * Constructors
   */
  UndefinedVariableException();
  UndefinedVariableException(string _msg);
  UndefinedVariableException(const PsodaException& other);

  /**
   * Destructor
   */
  virtual ~UndefinedVariableException();

};

#endif
