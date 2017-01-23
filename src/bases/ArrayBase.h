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
#ifndef SODA_ARRAYBASE
#define SODA_ARRAYBASE

#include <string.h>
#include <assert.h>
#include <stdlib.h>

template< typename TYPE >
class Array
{
  public:
    Array()
      : mData( NULL ),
      mSize( 0 )
      {
      }

    Array( int size )
      : mData( NULL ),
      mSize( size )
      {
        alloc();
      }

    Array( int size , TYPE initialValue )
      : mData( NULL ),
      mSize( size )
      {
        alloc();
        setAllTo( initialValue );
      }

    Array( const Array& orig )
      : mData( NULL ),
      mSize( orig.mSize )
      {
        alloc();
        memcpy( mData, orig.mData, orig.mSize * sizeof ( TYPE ) );
      }

    Array& operator=( const Array& orig )
    {
      if ( this != &orig )
      {
        if ( orig.size() != size() )
        {
          realloc(orig.mSize);
        }
        memcpy( mData, orig.mData, orig.mSize * sizeof ( TYPE ) );
      }
      return *this;
    }

  public:
    virtual ~Array()
    {
      dealloc();
    }

    int size() const { return mSize; }

    void setAllTo( TYPE initialValue )
    {
      for ( int i = 0 ; i < mSize ; i ++ )
      {
        operator[](i) = initialValue;
      }
    }

    void growTo( int count )
    {
      if ( count > mSize )
      {
        init( count );
      }
    }
    
    void init( int count )
    {
      dealloc();
      mSize = count;
      alloc();
    }

    TYPE* operator[]( int index ) const
    {
      assert( ! ( index < 0 || index >= mSize ) );
      return mData + index;
    }

    TYPE& operator[]( int index )
    {
      assert( ! ( index < 0 || index >= mSize ) );
      return *(mData + index );
    }
    
    TYPE* data( int index = 0 )
    {
      assert( ! ( index < 0 || index >= mSize ) );
      return mData + index;
    }
    

    void swap(int index1, int index2)
    {
      TYPE temp = operator[]( index1 );
      operator[]( index1 ) = operator[]( index2 );
      operator[]( index2 ) = temp;
    }

    /** randomize
     * @param start inclusive
     * @para end includive
     * start and end define an inclusive range
     */
    void randomize ( int start, int end )
    {
      int count = end + 1 - start;
      for ( int i = start ; i < end + 1 ; i ++ )
      {

        int destIndex = end + 1 - i - 1 ;
        int srcIndex = start + rand() % ( count - i ) ;
        swap( destIndex, srcIndex );
      }
    }

  protected:
    void alloc()
    {
        mData = new TYPE[mSize];
    }
    
    void realloc( int newSize )
    {
      dealloc();
      mSize = newSize;
      alloc();
    }
    
    void dealloc()
    {
      if ( mData )
      {
        delete[] mData;
        mData = NULL;
        mSize = 0;
      }
    }

  protected:
    TYPE* mData;
    int mSize;
};
#endif

