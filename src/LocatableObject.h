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
#ifndef PSODA_LOCATABLE_OBJECT_H
#define PSODA_LOCATABLE_OBJECT_H

#include "Location.h"

using namespace std;

class LocatableObject {

 private:

  void operator =(LocatableObject& rhs);

  //the location in the input file of this element
  Location location;

 protected:

  /**
   * Constructors
   */
  LocatableObject();
  LocatableObject(const Location& newLocation);

  /**
   * Copy Constructor
   */
  LocatableObject(const LocatableObject& other);

 public:

  /**
   * Destructor
   */
  virtual ~LocatableObject();

  /**
   * Sets the location of the object 
   *
   * @param newLocation The location of the object
   */
  void setLocation(const Location& newLocation);

  /**
   * Gets the location of the object
   *
   * @return The location of the object
   */
  const Location& getLocation() const;
  
};

#endif
