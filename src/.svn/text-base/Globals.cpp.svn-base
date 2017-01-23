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
#include "Globals.h"
#include "PsodaStopException.h"

int break_readline_input = 0;
int AlgorithmFlags = EARLYPRUNE|REALYEARLYPRUNE;
int PSODA_VERBOSE = 0;
int PSODA_DEBUG = 0;
bool gQuit = false;
#ifdef GUI
int pausePSODA = 0;
int pauseCount = 0;
#endif
char *psodaHome = "../src/";
double PSODA_maxmemory; // The max java vm memory or OS memory we can use for trees

int g_strlcat(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));       /* count does not include NUL */
}

/**
 * gStopPSODA stops the PSODA process
 */
void gStopPSODA()
{
#ifdef GUI
    // You must set gQuit = true in the GUI code not here
    //gQuit = true; /* We will quit on next call of checkPausePSODA */
#else
    //throw PsodaStopException("PSODA Stopped");
    gQuit = true;
#endif

}

void checkPausePSODA()
{
#ifdef GUI
        if (gQuit)
        {
            //fprintf(stderr, "Throwing a PsodaStopException\n");
            //throw PsodaStopException("PSODA Stopped");
        }
        while(pausePSODA){
            if(pauseCount == 0){
                PsodaPrinter::getInstance()->write("**Run PAUSED**\n");
                pauseCount++;
            }
#ifndef WIN32
            sleep(2);
#endif
            if(!pausePSODA){ pauseCount = 0; PsodaPrinter::getInstance()->write("**Continuing...**\n"); }
        }
#endif
}

