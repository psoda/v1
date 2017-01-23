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
#ifndef SODA_VECTOR 
#define SODA_VECTOR

#include <vector>

template<typename T>
class  Vector : public std::vector<T>
{
  public:
    Vector() : std::vector<T>()
    {
    }


  private:
    Vector( const Vector& orig )
    {
    }

    Vector& operator=( const Vector& orig )
    {
      if ( this != &orig )
      {
      }
      return *this;
    }

  public:
    virtual ~Vector()
    {
    }

    void append(T& value)
    {
      mVector.insert(mVector.end(),1,value);
    }

    void add(T& value)
    {
      mVector.insert(mVector.end(),1,value);
    }

    void insertAt( long index, T& value )
    {
      mVector.insert(mVector.begin() + index,1,value);
    }

    void removeAll()
    {
      mVector.clear();
    }

    void removeAt( long index )
    {
      mVector.erase( mVector.begin() + index );
    }

    T elementAt( long index)
    {
	    if (index >= mVector.size())
	    {
		    return 0;
	    }
	    return mVector.at(index);
    }

    int size()
    {
	    return mVector.size();
    }

    //VIRTUAL INTERFACE FUNCTIONS
    //GETTERS & SETTERS
    //INSTANCE SPECIFIC

  protected:
    std::vector<T> mVector;
};
#endif

