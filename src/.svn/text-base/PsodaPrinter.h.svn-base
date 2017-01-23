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
#ifndef PSODA_PRINTER_H
#define PSODA_PRINTER_H

#include "PrintBuffer.h"
#include <map>

using namespace std;

class PsodaPrinter : public PrintBuffer {

 private:

  /**
   * The max id stored so far
   */
  static int maxId;

  /**
   * The current PrintBuffers being used
   */
  map<int, PrintBuffer*> mPrintBuffers;

  /**
   * Constructor
   */
  PsodaPrinter();
  
  /**
   * Destroys the internal print buffer
   */
  void clean();

 public:

  /**
   * Destructor
   */
  ~PsodaPrinter();
  
  /**
   * Get a pointer to the Singleton Printer
   */
  static PsodaPrinter* getInstance();

  /**
   * Add a print buffer with the next Id and return the id
   */
  int addPrintBuffer(PrintBuffer* newPrintBuffer);
  
  /**
   * Add a print buffer to the list of buffers to be written to
   */
  void addPrintBuffer(int newId, PrintBuffer* newPrintBuffer);

  /**
   * Remove a brint buffer
   */
  void removePrintBuffer(int idToRemove);

  /**
   * Clear buffer list
   */
  void clearBufferList();

  /**
   * Perform the PrintBuffer methods on the current PrintBuffer
   */
  virtual void writeList(const char *, va_list argptr);
  virtual void writeErrorList(const char *, va_list argptr);

};

void  Debug(int category, int level, const string fmt,...);
void setDebug(int category, int level);

#endif

