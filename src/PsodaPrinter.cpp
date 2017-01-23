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
#include "PsodaPrinter.h"
#include <assert.h>

using namespace std;

int debugCategory;
int debugLevel;

int PsodaPrinter::maxId = 0;

PsodaPrinter::PsodaPrinter() {
  return;
}

PsodaPrinter::~PsodaPrinter() {
  clean();
}

void PsodaPrinter::clean() {
  map<int, PrintBuffer*>::iterator bufferItr = mPrintBuffers.begin();
  map<int, PrintBuffer*>::iterator bufferEnd = mPrintBuffers.end();
  for (; bufferItr != bufferEnd; bufferItr++) {
    //    delete bufferItr->second;
  }
  mPrintBuffers.clear();
}

PsodaPrinter* PsodaPrinter::getInstance() {
  static PsodaPrinter mInstance;
  return &mInstance;
}

int PsodaPrinter::addPrintBuffer(PrintBuffer* newPrintBuffer) {
  addPrintBuffer(++PsodaPrinter::maxId, newPrintBuffer);
  return PsodaPrinter::maxId;
}

void PsodaPrinter::addPrintBuffer(int newId, PrintBuffer* newPrintBuffer) {
  assert(mPrintBuffers.find(newId) == mPrintBuffers.end());
  if (newId > PsodaPrinter::maxId) {
    PsodaPrinter::maxId = newId;
  }
  mPrintBuffers[newId] = newPrintBuffer;
}

void PsodaPrinter::removePrintBuffer(int idToRemove) {
  mPrintBuffers.erase(idToRemove);
}

void PsodaPrinter::clearBufferList() {
  clean();
}

void PsodaPrinter::writeList(const char* fmt, va_list argptr) {
  map<int, PrintBuffer*>::iterator bufferItr = mPrintBuffers.begin();
  map<int, PrintBuffer*>::iterator bufferEnd = mPrintBuffers.end();
  for (; bufferItr != bufferEnd; bufferItr++) {
    PrintBuffer* thisPrintBuffer = bufferItr->second;
    thisPrintBuffer->writeList(fmt, argptr);
  }
}

void PsodaPrinter::writeErrorList(const char* fmt, va_list argptr) {
  map<int, PrintBuffer*>::iterator bufferItr = mPrintBuffers.begin();
  map<int, PrintBuffer*>::iterator bufferEnd = mPrintBuffers.end();
  for (; bufferItr != bufferEnd; bufferItr++) {
    PrintBuffer* thisPrintBuffer = bufferItr->second;
    thisPrintBuffer->writeErrorList(fmt, argptr);
  }
}

void setDebug(int category, int level)
{
	debugCategory = category;
	debugLevel = level;
}

void Debug(int category, int level, const string fmt,...)
{
	int print = 0;
	if ((debugCategory==category) && (level<=debugLevel))
	{
		print = 1; 
	}
	if (category==0 && level==0)
	{
		print = 1; 
	}
	if (print)
	{
		va_list argptr;
		va_start(argptr,fmt);
		PsodaPrinter::getInstance()->writeList(fmt.c_str(),argptr);
		va_end(argptr);
	}
}
