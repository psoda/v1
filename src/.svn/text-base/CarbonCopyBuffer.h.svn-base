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
#ifndef PSODA_CARBON_COPY_BUFFER_H
#define PSODA_CARBON_COPY_BUFFER_H

#include "PrintBuffer.h"
#include <sstream>

using namespace std;

/**
 * A CarbonCopyBuffer hold anything that is written to it in an internal buffer which can be retrieved and cleared
 */
class CarbonCopyBuffer : public PrintBuffer {

 private:
  
  ostringstream writeBuffer;
  ostringstream errorBuffer;

 public:

  /**
   * Constructor
   */
  CarbonCopyBuffer();

  /**
   * Destructor
   */
  virtual ~CarbonCopyBuffer();

  /**
   * @return A string with everything that has been printed to the buffer since the last clear
   */
  virtual string getWriteBuffer() const;

  /**
   * @return A string with everything that has been printed to the buffer since the last clear
   */
  virtual string getErrorBuffer() const;

  /**
   * Clears out the contents of the buffer
   */
  virtual void clearBuffers();

  //VIRTUAL INTERFACE FUNCTIONS
  virtual void writeList(const char* fmt, va_list argptr);
  virtual void writeErrorList(const char* fmt, va_list argptr);
  
};

#endif

