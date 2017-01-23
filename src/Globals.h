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
#ifndef QSODA_GLOBALS_H
#define QSODA_GLOBALS_H

#include <sys/time.h>
#include "PsodaError.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef WIN32
#include <limits.h>
#endif


extern int break_readline_input;
extern int g_strlcat(char *dst, const char *src, size_t siz);
extern void checkPausePSODA();


#define ACCESS_ARRAY(name,row,col,numCols) ((name)[(row) * (numCols) + (col)])


#define ROOTLAST 0x1
#define ROOTFIRST 0x2
#define PRINTALLTBR 0x4
#define EARLYPRUNE 0x8
#define REALYEARLYPRUNE 0x10
#define SWITCHTREEWHENBETTER 0x20
#define KEEPTREEUNTILALLSPLITS 0x40
// Run with -f 0x44 to get all trees one TBR away
#define NOPREPROCESS 0x80
#define STEPWITHPREPROCESSREFINE 0x100
#define NORANDOMSTEPWISE 0x200
#define NOCATCHSIGBUS 0x400
#define NOCATCHSIGSEGV 0x800
#define BATCHMODE 0x1000                /* currently just doesn't register signal handlers */
/* place holders
#define PLACEHOLDER_0x2000 0x2000
#define PLACEHOLDER_0x4000 0x4000
#define PLACEHOLDER_0x8000 0x8000
 */

extern int PSODA_VERBOSE; /* specified on the command-line */
extern int PSODA_DEBUG; /* specified on the command-line */

extern int AlgorithmFlags;

extern bool gQuit; /* True if PSODA should quit on next call of checkPausePSODA */
#ifdef GUI
	extern int pausePSODA;
#endif

extern void gStopPSODA() __attribute__((noreturn));

extern char *psodaHome; //Where the base directory of the code is


static inline double When()  
{
	struct timeval tp;
	gettimeofday(&tp,0);
	return ((double)tp.tv_sec + (double)tp.tv_usec * 1e-6);
}  


#endif

