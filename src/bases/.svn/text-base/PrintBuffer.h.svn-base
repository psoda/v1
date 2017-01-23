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
#ifndef SODA_PRINTBUFFER 
#define SODA_PRINTBUFFER

#include "Vector.h"
#include <string>
#include <stdarg.h>

using std::string;

class Interpreter;

class PrintBuffer
{
  private:
    PrintBuffer( const PrintBuffer& printBuffer );
    PrintBuffer& operator=( const PrintBuffer& printBuffer );

  public:
    PrintBuffer();
    virtual ~PrintBuffer();

    //VIRTUAL INTERFACE FUNCTIONS
    virtual void write(const string,...);
    virtual void writeList(const char *, va_list)=0;
    virtual void writeError(const char *,...);
    virtual void writeErrorList(const char *, va_list)=0;

};

#endif
