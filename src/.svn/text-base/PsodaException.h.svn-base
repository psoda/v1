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
#ifndef PSODA_GENERIC_EXCEPTION
#define PSODA_GENERIC_EXCEPTION

#include "LocatableObject.h"
#include "Location.h"
#include <string>

#define ADD_LOCATION_TO_EXCEPTIONS(body, location) try { body } catch (PsodaException& e) { \
    PsodaException::addLocationToException(e, location);		\
    throw;								\
  }									

using namespace std;

class PsodaException : public LocatableObject {

 private:
  /**
   * Exception message
   */
  string msg;

 public:

  /**
   * Constructors
   */
  PsodaException();
  PsodaException(const Location& newLocation);
  PsodaException(string _msg);
  PsodaException(string _msg, const Location& newWhere);
  PsodaException(const PsodaException& other);

  /**
   * Destructor
   */
  virtual ~PsodaException();

  /**
   * For copying
   */
  void operator =(const PsodaException& other);

  /**
   * Accessors and Mutators
   */
  string what() const;

  /**
   * Set the descriptive message
   */
  void describe(string _msg);

  /**
   * Get the Location
   */
  const Location& where() const;

  /**
   * Prints the message and location (if available) to a string
   */
  virtual string toString() const;

  /**
   * Adds the location to exception e if there isn't one already set
   *
   * @param e The exception (passed by reference)
   * @param location The location (in the PSODA source code) at which the exception was thrown
   */
  static void addLocationToException(PsodaException& e, const Location& location);

  /**
   * For testing
   */
  static bool test();

};

#endif
