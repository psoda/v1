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
#include "Location.h"
#include <sstream>

using namespace std;

void Location::copy(const Location& other) {
  init(other.pathToFile, other.firstLine, other.firstColumn, other.lastLine, other.lastColumn);
}

Location::Location() {
  init();
}

Location::Location(string newPathToFile, int newFirstLine, int newFirstColumn, int newLastLine, int newLastColumn) {
  init(newPathToFile, newFirstLine, newFirstColumn, newLastLine, newLastColumn);
}

Location::Location(const Location& other) {
  copy(other);
}

void Location::operator =(const Location& rhs) {
  if (this != &rhs) {
    copy(rhs);
  }
}

void Location::init(string newPathToFile, int newFirstLine, int newFirstColumn, int newLastLine, int newLastColumn) {
  pathToFile = newPathToFile;
  firstLine = newFirstLine;
  firstColumn = newFirstColumn;
  lastLine = newLastLine;
  lastColumn = newLastColumn;
}

void Location::setPathToFile(string newPathToFile) {
  pathToFile = newPathToFile;
}

void Location::setFirst(int newFirstLine, int newFirstColumn) {
  firstLine = newFirstLine;
  firstColumn = newFirstColumn;
}

void Location::setLast(int newLastLine, int newLastColumn) {
  lastLine = newLastLine;
  lastColumn = newLastColumn;
}

string Location::getPathToFile() const {
  return pathToFile;
}

int Location::getFirstLine() const {
  return firstLine;
}

int Location::getFirstColumn() const {
  return firstColumn;
}

int Location::getLastLine() const {
  return lastLine;
}

int Location::getLastColumn() const {
  return lastColumn;
}

string Location::toString() const {
  ostringstream returnStream;
  returnStream.str("");
  if (firstLine >= 0) {
    returnStream << "\"" << getPathToFile() << "\" ";
    returnStream << "Line " << firstLine;
    if (firstColumn >= 0) {
      returnStream << ":" << firstColumn;
      if (lastLine >= 0) {
	returnStream << "--" << lastLine;
	if (lastLine >= 0) {
	  returnStream << ":" << lastColumn;
	}
      }
    }
  }
  return returnStream.str();
}
