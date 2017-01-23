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
#ifndef SODA_BUFFER 
#define SODA_BUFFER

class Buffer
{
  public:
    Buffer();
    Buffer( int allocLength );
    Buffer( const char* src );
    Buffer( const Buffer& buffer );
    Buffer( void* src, int length );
    Buffer& operator=( const Buffer& buffer );
  public:
    virtual ~Buffer();

    Buffer& operator=( const char* src );

    Buffer& operator+=( const char* src );
    Buffer& operator+=( const char src );
    Buffer& operator+=( const short src );
    Buffer& operator+=( const int src );
    Buffer& operator+=( const long src );

    void assign( const void* src, int length );
    void setDataLength( int length );
    int allocLength() const;
    int dataLength() const;
    int length() const;
    int& allocLength();
    int& dataLength();
    int& length();
    operator const char* () const;
    operator char* ();
    char& operator[] ( int offset );
    char* data();
    
    void growDataLengthBy( int length );
    void growDataLengthTo( int length );

  protected:
    void alloc( int length );
    void dealloc();
    void allocAndInitializeWith( const void* src, int length );
    void append( const void* src, int length );

  protected:
    char* mBuffer;
    int   mAllocLength;
    int   mCurrentLength;
};
#endif

