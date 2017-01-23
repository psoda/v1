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
#include "Buffer.h"
#include "PrintBuffer.h"

#include <string.h>
#include <stdio.h>

  Buffer::Buffer()
: mBuffer(NULL),
  mAllocLength(0),
  mCurrentLength(0)
{
}

  Buffer::Buffer( int allocLength )
: mBuffer(NULL),
  mAllocLength(0),
  mCurrentLength(0)
{
  alloc( allocLength );
}

  Buffer::Buffer( const char* src )
: mBuffer(NULL),
  mAllocLength(0),
  mCurrentLength(0)
{
  allocAndInitializeWith( src, strlen( src ) );
}

  Buffer::Buffer( void* src, int length )
: mBuffer(NULL),
  mAllocLength(0),
  mCurrentLength(0)
{
  allocAndInitializeWith( src, length );
}

  Buffer::Buffer( const Buffer& orig )
: mBuffer(NULL),
  mAllocLength(0),
  mCurrentLength(0)
{
  allocAndInitializeWith( static_cast<const void*>(orig.mBuffer), orig.dataLength() );
}

Buffer& Buffer::operator=( const Buffer& orig )
{
  if ( this != &orig )
  {
    mAllocLength = 0;
    mCurrentLength = 0;
    allocAndInitializeWith( static_cast<const void*>(orig.mBuffer), orig.dataLength() );
  }
  return *this;
}

Buffer::~Buffer()
{
  dealloc();
}

Buffer& Buffer::operator=( const char* src )
{
  allocAndInitializeWith( src, strlen( src ) );
  return *this;
}

void Buffer::assign( const void* src, int length )
{
  allocAndInitializeWith( src, length );
}

Buffer& Buffer::operator+=( const char* src )
{
  append( src, strlen( src ) );
  return *this;
}

Buffer& Buffer::operator+=( const char src )
{
  append( &src, 1 );
  return *this;
}

Buffer& Buffer::operator+=( const short src )
{
  char temp[20];
  int length = snprintf( temp, 20, "%hi", src );
  append( temp, length ); 
  return *this;
}

Buffer& Buffer::operator+=( const int src )
{
  char temp[20];
  int length = snprintf( temp, 20, "%i", src );
  append( temp, length ); 
  return *this;
}

Buffer& Buffer::operator+=( const long src )
{
  char temp[20];
  int length = snprintf( temp, 20, "%li", src );
  append( temp, length ); 
  return *this;
}

void Buffer::setDataLength( int length ) {
  mCurrentLength = length;
}

int Buffer::allocLength() const {
  return mAllocLength;
}

int Buffer::dataLength() const {
  return mCurrentLength;
}

int Buffer::length() const {
  return mCurrentLength;
}

int& Buffer::allocLength() {
  return mAllocLength;
}

int& Buffer::dataLength() {
  return mCurrentLength;
}

int& Buffer::length() {
  return mCurrentLength;
}

Buffer::operator const char* () const {
  return mBuffer;
}

Buffer::operator char* () {
  return mBuffer;
}

char& Buffer::operator[] ( int offset ) {
  return mBuffer[offset];
}

char* Buffer::data() {
  return mBuffer;
}

void Buffer::alloc( int length )
{
  if ( mBuffer )
  {
    dealloc();
  }
  mBuffer = new char[ length + 1 ];
  allocLength() = length;
  setDataLength( 0 );
}

void Buffer::dealloc()
{
  delete[] mBuffer;
  mBuffer = NULL;
}

void Buffer::allocAndInitializeWith( const void* src, int length )
{
  if ( length )
  {
    alloc( length );
    memcpy( mBuffer, src, length );
    setDataLength( length );
  }
}

void Buffer::growDataLengthBy( int length )
{
  int newLength = dataLength() + length;
  if ( allocLength() < newLength )
  {
    char* temp = new char [ newLength + 1 ];
    memcpy( temp, mBuffer, dataLength() );

    dealloc();
    mBuffer = temp;
    allocLength() = newLength;
    //dataLength(); //doesn't need to change
  }
}

void Buffer::growDataLengthTo( int length )
{
  length++;
  if ( allocLength() < length )
  {
    char* temp = new char [ length + 1 ];
    memcpy( temp, mBuffer, dataLength() );

    dealloc();
    mBuffer = temp;
    allocLength() = length;
    //dataLength(); //doesn't need to change
  }
}

void Buffer::append( const void* src, int length )
{
  growDataLengthBy( length );
  memcpy( mBuffer + dataLength(), src, length );
  dataLength() += length;
}

