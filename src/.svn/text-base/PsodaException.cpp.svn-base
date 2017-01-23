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
#include "PsodaException.h"
#include <iostream>

using namespace std;

PsodaException::PsodaException() : LocatableObject(), msg("") {
	return;
}

PsodaException::PsodaException(const Location& newLocation) : LocatableObject(newLocation), msg("") {
	return;
}

PsodaException::PsodaException(string _msg) : LocatableObject(), msg(_msg) {
	return;
}

PsodaException::PsodaException(string _msg, const Location& newLocation) : LocatableObject(newLocation), msg(_msg) {
	return;
}

PsodaException::PsodaException(const PsodaException& other) : LocatableObject(other), msg(other.msg) {
	return;
}

void PsodaException::operator =(const PsodaException& other) {
	if( this != &other ) {
		msg = other.msg;
		setLocation(other.getLocation());
	}
}

PsodaException::~PsodaException() {
	return;
}

void PsodaException::addLocationToException(PsodaException& e, const Location& location) {
  if (e.where().getFirstLine() < 0) {
    //no valid location, add one to the exception and throw it again
    e.setLocation(location);
  }
}

string PsodaException::what() const {
	return msg;
}

const Location& PsodaException::where() const {
  return getLocation();
}

void PsodaException::describe(string _msg) {
	msg = _msg;
}

bool PsodaException::test() {
	PsodaException e;
	if( e.what() != "" ) {
		cout << __LINE__ << " ";
		return false;
	}

	PsodaException e1("test exception");
	if( e1.what() != "test exception") {
		cout << __LINE__ << " ";
		return false;
	}

	PsodaException e2(e1);
	if( e2.what() != "test exception" ) {
		cout << __LINE__ << " ";
		return false;
	}

	PsodaException e3;
	e3 = e1;
	if( e3.what() != "test exception" ) {
		cout << __LINE__ << " ";
		return false;
	}

	e3.describe("new test msg");
	if( e3.what() != "new test msg" ) {
		cout << __LINE__ << " ";
		return false;
	}

	try {
		throw PsodaException();
	}
	catch(PsodaException myException) {
		return true;
	}
}

string PsodaException::toString() const {
  string returnString = "";
  if (getLocation().getFirstLine() >= 0 ) { //if valid line number, we want to display it
    returnString += what();
    returnString += " (at ";
    returnString += where().toString();
    returnString += ")";
  } else {
    returnString += what();
  }
  return returnString;
}
