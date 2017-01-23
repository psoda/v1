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
#include "PsodaStopException.h"

using namespace std;

/**
 * Default Constructor for PsodaStopException
 *
 * The default constructor for PsodaStopException calls the default constructor for PsodaException
 */
PsodaStopException::PsodaStopException() : PsodaException() {
  return;
}


/**
 * Copy Constructor for PsodaStopException
 *
 * The copy constructor for PsodaStopException calls the copy constructor for PsodaException
 */
PsodaStopException::PsodaStopException(const Location& newLocation) : PsodaException(newLocation) {
  return;
}


/**
 * Constructor for PsodaStopException taking a message string
 *
 * This constructor takes a message string an passes that to the PsodaException constructor
 */
PsodaStopException::PsodaStopException(string _msg) : PsodaException(_msg) {
  return;
}

PsodaStopException::PsodaStopException(string _msg, const Location& newLocation) : PsodaException(_msg, newLocation) {
  return;
}

PsodaStopException::PsodaStopException(const PsodaStopException& other) : PsodaException(other) {
  return;
}

PsodaStopException::~PsodaStopException() {
  return;
}

string PsodaStopException::toString() const {
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
