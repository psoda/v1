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
#include "PsodaError.h"

using namespace std;

/**
 * Default Constructor for PsodaError
 *
 * The default constructor for PsodaError calls the default constructor for PsodaException
 */
PsodaError::PsodaError() : PsodaException() {
  return;
}


/**
 * Copy Constructor for PsodaError
 *
 * The copy constructor for PsodaError calls the copy constructor for PsodaException
 */
PsodaError::PsodaError(const Location& newLocation) : PsodaException(newLocation) {
  return;
}


/**
 * Constructor for PsodaError taking a message string
 *
 * This constructor takes a message string an passes that to the PsodaException constructor
 */
PsodaError::PsodaError(string _msg) : PsodaException(_msg) {
  return;
}

PsodaError::PsodaError(string _msg, const Location& newLocation) : PsodaException(_msg, newLocation) {
  return;
}

PsodaError::PsodaError(const PsodaError& other) : PsodaException(other) {
  return;
}

PsodaError::~PsodaError() {
  return;
}

string PsodaError::toString() const {
  string returnString = "Error: ";
  if (where().getFirstLine() >= 0 ) { //if valid line number, we want to display it
    returnString += where().toString();
    returnString += " (";
    returnString += what();
    returnString += ")";
  } else {
    returnString += what();
  }
  return returnString;
}
