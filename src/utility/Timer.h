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
#ifndef SODA_TIMER 
#define SODA_TIMER

#include <sys/time.h>

typedef struct
{
  unsigned long seconds ;
  unsigned long millis ;
} TimerSecondMillis ;

typedef struct
{
  unsigned long seconds ;
  unsigned long micros ;
} TimerSecondMicros ;


class Timer
{
  public:
    Timer();
  private:
    Timer( const Timer& timer );
    Timer& operator=( const Timer& timer );
  public:
    virtual ~Timer();

    void start();
    void stop();
    void diff();
    void diff( const Timer& other );

    long getSeconds();
    long long getMillis();
    TimerSecondMillis getSecondMillis();
    TimerSecondMicros getSecondMicros();
    TimerSecondMicros getCurrentSecondMicros();
    TimerSecondMicros getCurrentElapsedSecondMicros();
    void getElapsedTimestamp( char* buffer );
    void printTimestamp( char* buffer );

  protected:
    struct timeval mStart ;
    struct timeval mStop ;
    struct timeval mDiff ;
};
#endif

