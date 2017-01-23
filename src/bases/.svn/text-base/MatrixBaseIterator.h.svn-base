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
#ifndef SODA_MATRIXBASEITERATOR 
#define SODA_MATRIXBASEITERATOR

template< typename M> class MatrixBaseIterator
{
  public:
    typedef typename M::primitiveType MP;
    MatrixBaseIterator(M& m): mMatrix(m), iterator(mMatrix.data())
    {
    }
    
  private:
    MatrixBaseIterator( const MatrixBaseIterator& matrixBaseIterator )
    {
    }
    
    MatrixBaseIterator& operator=( const MatrixBaseIterator& matrixBaseIterator )
    {
    }

  public:
    virtual ~MatrixBaseIterator()
    {
    }

    void incrementRow()
    {
      iterator += mMatrix.columns();
    }
    
    MatrixBaseIterator<M>& operator += ( long i )
    {
      iterator+=i;
      return *this;
    }
    
    MatrixBaseIterator<M>& operator -= ( long i )
    {
      iterator-=i;
      return *this;
    }
    
    MatrixBaseIterator<M>& operator ++ ( int i )
    {
      iterator++;
      return *this;
    }
    
    MatrixBaseIterator<M>& operator -- ( int i )
    {
      iterator--;
      return *this;
    }
    
    MP& operator + (long i)
    {
      return *(iterator + i );
    }

    MP& operator - (long i)
    {
      return *(iterator - i );
    }

    MP& operator * ()
    {
      return *iterator;
    }
    //VIRTUAL INTERFACE FUNCTIONS
    //GETTERS & SETTERS
    //INSTANCE SPECIFIC
  protected:
    M& mMatrix;
    MP* iterator;
};
#endif

