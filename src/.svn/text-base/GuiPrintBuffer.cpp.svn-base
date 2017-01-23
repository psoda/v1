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
#include "GuiPrintBuffer.h"
#include "PsodaError.h"
#include <string.h>

#ifdef GUI
#include "PSODA.h"
#endif

#define BUFSIZE 20000

GuiPrintBuffer::GuiPrintBuffer()
:
writeBuffer(),
errorBuffer()
{
}

GuiPrintBuffer::~GuiPrintBuffer() 
{
}

void GuiPrintBuffer::writeToBuffer(ostringstream& buffer __attribute__ ((unused)), const char* fmt __attribute__ ((unused)), va_list argptr __attribute__ ((unused))) {
#ifdef GUI
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
  //if (strrchr(buf, '\n')) {
    //write to the gui
    Java_gui_PSODA_writeToGui(buffer.str().c_str());

    //clear the buffer
    buffer.str("");
  //}
#endif
}

void GuiPrintBuffer::writeList(const char* fmt, va_list argptr) {
  writeToBuffer(writeBuffer, fmt, argptr);
}

void GuiPrintBuffer::writeErrorList(const char* fmt, va_list argptr) {
  writeToBuffer(errorBuffer, fmt, argptr);
}

