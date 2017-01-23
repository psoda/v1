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
#ifndef PSODA_CODEABLE_H
#define PSODA_CODEABLE_H

#include <string>

using namespace std;

class Codeable {

 private:

  void operator =(Codeable& rhs);

  string sourceCode;

 protected:

  /**
   * Constructors
   */
  Codeable();

  /**
   * Copy Constructor
   */
  Codeable(const Codeable& other);

 public:

  /**
   * Destructor
   */
  virtual ~Codeable();

  /**
   * Sets the location of the object 
   *
   * @param newLocation The location of the object
   */
  void setSourceCode(string newSourceCode);

  /**
   * Gets the location of the object
   *
   * @return The location of the object
   */
  const string& getCode() const;
  
};

#endif
