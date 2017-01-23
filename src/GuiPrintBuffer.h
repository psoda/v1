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
#ifndef PSODA_GUI_PRINT_BUFFER_H 
#define PSODA_GUI_PRINT_BUFFER_H

#include "PrintBuffer.h"
#include <sstream>

#ifdef GUI
#include <jni.h>
#include "PSODA.h"
#endif

using namespace std;

class GuiPrintBuffer : public PrintBuffer {

 private:

  ostringstream writeBuffer;
  ostringstream errorBuffer;

  public:

    GuiPrintBuffer();
    virtual ~GuiPrintBuffer();

    /**
     * Helper function to write the output to either the write or error buffer
     */
    void writeToBuffer(ostringstream& buffer,const char* fmt, va_list argptr);

    //VIRTUAL INTERFACE FUNCTIONS
    virtual void writeList(const char *, va_list argptr);
    virtual void writeErrorList(const char *, va_list argptr);

};

#endif

