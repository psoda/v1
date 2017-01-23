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
#include "TreeLiteral.h"
#include <float.h>

using namespace std;

#define DEFAULT_TREE_VALUE NULL

TreeLiteral::TreeLiteral() : Literal(), value(DEFAULT_TREE_VALUE) {
  return;
}

TreeLiteral::TreeLiteral(const QTree& initialValue) : Literal() {
  copyTreeToValue(&initialValue);
}

TreeLiteral::TreeLiteral(const Literal& other) : Literal() {
  copyTreeToValue(other.toTree());
}

Literal& TreeLiteral::operator =(const Literal& rhs) {
  if (&rhs != this) {
    if (value) {
      clean();
    }
    copyTreeToValue(rhs.toTree());
  }
  return *this;
}

TreeLiteral::~TreeLiteral() {
  clean();
}

void TreeLiteral::clean() {
  if (value) {
    delete value;
    value = NULL;
  }
}

void TreeLiteral::setValue(const QTree& newValue) {
  clean();
  copyTreeToValue(&newValue);
}

void TreeLiteral::copyTreeToValue(const QTree* other) {
  if (other) {
    value = (new QTree(other));
  }
}

QTree* TreeLiteral::toTree() const {
  return value;
}

int TreeLiteral::toInt() const {
  double doubleScore = (value) ? value->getScore() : DBL_MAX;
  int intScore = (doubleScore > (double) INT_MAX) ? INT_MAX : (int) doubleScore;
  return intScore;
}

string TreeLiteral::toString() const {
  return (value) ? value->treeStr() : "NULL";
}

TreeLiteral* TreeLiteral::copyToHeap() const {
  return new TreeLiteral(value);
}
