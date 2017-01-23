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
#ifndef PSODA_LOCATION_H
#define PSODA_LOCATION_H

using namespace std;

#include <string>

class Location {

 private:

  string pathToFile;
  int firstLine;
  int firstColumn;
  int lastLine;
  int lastColumn;

  /**
   * Makes a deep copy of other
   */
  void copy(const Location& other);

  /**
   * Makes a deep copy of other
   */
  void init(string pathToFile = "", int newFirstLine = -1, int newFirstColumn = -1, int newLastLine = -1, int newLastColumn = -1);

 public:

  /**
   * Default Constructor
   */
  Location();

  /**
   * Constructor initializes line and column
   */
  Location(string pathToFile, int newFirstLine = -1, int newFirstColumn = -1, int newLastLine = -1, int newLastColumn = -1);

  /**
   * Copy Constructor makes a deep copy
   */
  Location(const Location& other);

  /**
   * Assignment Operator
   */
  void operator =(const Location& rhs);
  
  /**
   * Sets the path to the file
   */
  void setPathToFile(string = "");

  /**
   * Sets the first line and column
   */
  void setFirst(int newFirstLine = -1, int newFirstColumn = -1);

  /**
   * Sets the last line and column
   */
  void setLast(int newLastLine = -1, int newLastColumn = -1);

  /**
   * Gets the path to the file
   */
  string getPathToFile() const;

  /**
   * Gets the first line
   */
  int getFirstLine() const;

  /**
   * Gets the first column
   */
  int getFirstColumn() const;

  /**
   * Gets the last line
   */
  int getLastLine() const;

  /**
   * Gets the last column
   */
  int getLastColumn() const;

  /**
   * Prints the location to a string
   */
  string toString() const;

};

#endif
