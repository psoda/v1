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
#include "Timer.h"
#include <stdlib.h>

Timer::Timer()
{
}

Timer::Timer( const Timer& orig __attribute__((unused)) )
{
}

Timer& Timer::operator=( const Timer& orig )
{
  if ( this != &orig )
  {
  }
  return *this;
}

Timer::~Timer()
{
}

void Timer::start() { gettimeofday ( &mStart , NULL ); }
void Timer::stop() { gettimeofday ( &mStop , NULL ); }
void Timer::diff()
{
  mDiff.tv_usec = mStop.tv_usec - mStart.tv_usec;
  mDiff.tv_sec = mStop.tv_sec - mStart.tv_sec;

  if ( mDiff.tv_usec < 0 )
  {
    mDiff.tv_sec -= 1 ;
    mDiff.tv_usec += 1000000 ;
  }
}

void Timer::diff( const Timer& other )
{

  mDiff.tv_usec = other.mStop.tv_usec - mStart.tv_usec;
  mDiff.tv_sec = other.mStop.tv_sec - mStart.tv_sec;

  if ( mDiff.tv_usec < 0 )
  {
    mDiff.tv_sec -= 1;
    mDiff.tv_usec += 1000000;
  }
}

long Timer::getSeconds()
{
  return mDiff.tv_sec + ( mDiff.tv_usec > 5000000 ? 1 : 0 );
}

long long Timer::getMillis()
{
  return mDiff.tv_sec * 1000 + ( mDiff.tv_usec /  1000 );
}

TimerSecondMillis Timer::getSecondMillis()
{
  TimerSecondMillis sm;
  sm.seconds = mDiff.tv_sec;
  sm.millis = getMillis();
  return sm ;
}

TimerSecondMicros Timer::getSecondMicros()
{
  TimerSecondMicros sm;
  sm.seconds = mDiff.tv_sec;
  sm.micros = mDiff.tv_usec;
  return sm ;
}

TimerSecondMicros Timer::getCurrentSecondMicros()
{
  stop();
  diff();
  return getSecondMicros();
}

