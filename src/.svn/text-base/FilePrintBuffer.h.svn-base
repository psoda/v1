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
#ifndef PSODA_FILE_PRINT_BUFFER_H
#define PSODA_FILE_PRINT_BUFFER_H

#include "PrintBuffer.h"
#include <sstream>

using namespace std;

/**
 * A FilePrintBuffer prints output to a file (all output is appended to the file, the files are created if they do not exist)
 */
class FilePrintBuffer : public PrintBuffer {

 private:
  
  /**
   * The path to the file to write standard output to
   */
  string writeFilename;

  /**
   * The name to the file to write error output to
   */
  string errorFilename;

  /**
   * String buffers to hold the outpute between lines
   */
  ostringstream writeBuffer;
  ostringstream errorBuffer;

 public:

  /**
   * Constructors
   */
  FilePrintBuffer();
  FilePrintBuffer(string newWriteFilename);
  FilePrintBuffer(string newWriteFilename, string newErrorFilename);

  /**
   * Helper function to write the output to either the write or error buffer
   */
  void writeToFile(string filename, ostringstream& buffer,const char* fmt, va_list argptr);

  
  /**
   * Clears the contents of the output files
   */
  void clearFiles();

  /**
   * Destructor
   */
  virtual ~FilePrintBuffer();

  //VIRTUAL INTERFACE FUNCTIONS
  virtual void writeList(const char* fmt, va_list argptr);
  virtual void writeErrorList(const char* fmt, va_list argptr);
  
};

#endif

