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
#include "Globals.h"
#include "FilePrintBuffer.h"
#include "PsodaError.h"

#define BUFSIZE 20000

using namespace std;

FilePrintBuffer::FilePrintBuffer() 
: 
	writeFilename("psoda.out"),
	errorFilename("psoda.err"),
	writeBuffer(),
	errorBuffer()
{
}

FilePrintBuffer::FilePrintBuffer(string newWriteFilename) :
	writeFilename(newWriteFilename), 
	errorFilename(newWriteFilename),
	writeBuffer(),
	errorBuffer()
{
}
  
FilePrintBuffer::FilePrintBuffer(string newWriteFilename, string newErrorFilename):
	writeFilename(newWriteFilename),
	errorFilename(newErrorFilename),
	writeBuffer(),
	errorBuffer()
{}

FilePrintBuffer::~FilePrintBuffer() {
  return;
}

void FilePrintBuffer::writeToFile(string filename, ostringstream& buffer, const char* fmt, va_list argptr) {
  char buf[BUFSIZE];
  int size = vsprintf(buf, fmt, argptr);

  //Check for buffer overflow (I think this happens after the damage could have been done)
  if (size >= BUFSIZE) {
    throw PsodaError("Overflowed buffer Gui Print Buffer");
  }

  buf[size] = 0;

  //write to the buffer
  buffer << buf;

  //check for end of line
  if (strrchr(buf, '\n')) {
    //write to the file
    FILE* output = fopen(filename.c_str(), "a");
    fprintf(output, "%s", (const char*)buffer.str().c_str());
    fclose(output);

    //clear the buffer
    buffer.str("");
  }
}

void FilePrintBuffer::clearFiles() {
  FILE* output = fopen(writeFilename.c_str(), "w");
  fclose(output);
  if (writeFilename != errorFilename) {
    output = fopen(errorFilename.c_str(), "w");
    fclose(output);
  }
}

void FilePrintBuffer::writeList(const char* fmt, va_list argptr) {
  writeToFile(writeFilename, writeBuffer, fmt, argptr);
}

void FilePrintBuffer::writeErrorList(const char* fmt, va_list argptr) {
  writeToFile(errorFilename, errorBuffer, fmt, argptr);
}

