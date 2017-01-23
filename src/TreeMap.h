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
#ifndef PSODA_TREE_MAP_H
#define PSODA_TREE_MAP_H

#include "QTree.h"
#include <map>
#include <string>

using namespace std;

/**
 * The identifier names are case insensitive and are stored as lower case
 */
class TreeMap {
  
 private:
  map<string, QTree*> trees;
  
 public:
  /**
   * Constructors
   */
  TreeMap();

  /**
   * Destructor  
   */
  virtual ~TreeMap();
  
  /**
   * Adds a new tree by the given name (deleting the former entry if there is one)
   */
  void store(string name, QTree* newTree);

  /**
   * Removes and deletes the tree by the given name (if there is one)
   */
  void remove(string name);

  /**
   * Returns the tree with the given name
   */
  QTree* get(string name);

};

#endif
