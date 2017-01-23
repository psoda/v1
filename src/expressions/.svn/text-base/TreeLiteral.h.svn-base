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
#ifndef PSODA_TREE_LITERAL_H
#define PSODA_TREE_LITERAL_H

#include "Literal.h"

using namespace std;

class TreeLiteral : public Literal {

 private:

  /**
   * The value of this Tree literal
   */
  QTree* value;

  /**
   * Destroys the QTree* inside if there is one
   */
  void clean();

  /**
   * If other is not null, makes a copy of the tree to store in value
   */
  void copyTreeToValue(const QTree* other);

 public:

  /**
   * Default Contructor
   */
  TreeLiteral();

  /**
   * Copy Contructor
   */
  TreeLiteral(const Literal& other);

  /**
   * Contructor with initial value
   */
  TreeLiteral(const QTree& initialValue);

  /**
   * Assignment Operator
   *
   * @return Reference to the resulting Literal
   * @throws InvalidCastException if the rhs cannot be converted to an int
   */
  virtual Literal& operator =(const Literal& rhs);

  /**
   * Destructor
   */
  virtual ~TreeLiteral();
  
  /**
   * Updates the internal value
   *
   * @param value The new value
   */
  void setValue(const QTree& newValue);

  /**
   * Returns a pointer to the tree
   */
  virtual QTree* toTree() const;

  /**
   * Returns the score of the tree
   */
  virtual int toInt() const;

  /**
   * Returns the string representation of the tree
   */
  virtual string toString() const;

  /**
   * Creates a new copy of this Literal on the heap and returns it
   */
  virtual TreeLiteral* copyToHeap() const;

};

#endif
