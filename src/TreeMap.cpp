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
#include "TreeMap.h"

using namespace std;

TreeMap::TreeMap() : trees() {
  return;
}

TreeMap::~TreeMap() {
  map<string, QTree*>::const_iterator treeItr = trees.begin();
  map<string, QTree*>::const_iterator treeEnd = trees.end();
  while (treeItr != treeEnd) {
    delete treeItr->second; 
    treeItr++;
  }
  trees.clear();
}

void TreeMap::store(string name, QTree* treeToStore) {
  // delete the old one if there is one
  if (trees.find(name) != trees.end()) {
    delete trees[name];
  }
  trees[name] = treeToStore;
}

void TreeMap::remove(string name) {
  // delete the old one if there is one
  if (trees.find(name) != trees.end()) {
    delete trees[name];
    trees.erase(name);
  }
}

QTree* TreeMap::get(string name) {
  if (trees.find(name) != trees.end()) {
    return trees[name];
  } else {
    return NULL;
  }
}
