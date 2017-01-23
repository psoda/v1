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
#ifndef PSODA_DATASET_MAP_H
#define PSODA_DATASET_MAP_H

#include "DataBlock.h"
#include "DatasetReference.h"
#include <map>
#include <set>
#include <string>

using namespace std;

/**
 * The identifier names are case insensitive and are stored as lower case
 */
class DatasetMap {
  
 private:

  /**
   * The dataset pointer that is associated with a given string
   */
  map<string, Dataset*> datasets;

  /**
   * How many references are in existance to the given id
   */
  map<string, int> idCounts;

  /**
   * How many references are in existance to the given pointer
   */
  map<Dataset*, int> pointerCounts;

  set<string> names;

  DatasetReference* originalDatasetReference;
  DatasetReference* currentDatasetReference;
  Dataset* originalDataset;
  Dataset* currentDataset;
  
  /**
   * Assign a unique (in terms of the datasets map) for the given dataset pointer
   */
  string assignId(Dataset* dataset);

  /**
   * This updates the counts and frees any possible unreferenced memory when a id in the datasetmap is assigned to a different dataset than before
   */
  void transferId(string id, Dataset* oldDataset, Dataset* newDataset);

  /**
   * Adds the dataset into the datasets map with the given id, managing memory and returning (by reference) a pointer to a DatasetReference associated with the new id
   */
  void addReference(Dataset* dataset, string id, DatasetReference*& returnReference);

 public:

  /**
   * Constructors
   */
  DatasetMap();

  /**
   * Destructor  
   */
  virtual ~DatasetMap();
  
  /**
   * Get the original dataset
   */
  Dataset* getOriginal() const;

  /**
   * Get the current dataset
   */
  Dataset* getCurrent() const;

  /**
   * Get a pointer to the dataset in the datasets map with the given id
   */
  Dataset* getDataset(string id) const;
  
  /**
   * Decrement the reference count on the dataset with the given id
   * Frees any dataset memory that is no longer referenced
   */
  void decrementRefCount(string id);
  
  /**
   * Increment the reference count on the dataset with the given id
   */
  void incrementRefCount(string id);
  
  /**
   * Set the original dataset (performs the memory management for this)
   */
  void setOriginal(Dataset* dataset);
  
  /**
   * Set the current dataset (performs the memory management for this)
   */
  void setCurrent(Dataset* dataset);
  
  /**
   * Set the current dataset to be the one referenced by id
   */
  void setCurrent(string id);

  /**
   * Sets the given dataset in the map with the given id (the id is also added to a list of names so that we don't delete the dataset even if all references to it are deleted).  We will, however delete the dataset if all references and all ids pointing to it are gone.
   */
  void setD(Dataset* dataset, string id);
  
  /**
   * Adds the given dataset to the map, creating a unique id and returning (by reference) a pointer to a DatasetReference on the heap.  When this reference is deleted, memory management rules will be automatically applied
   */
  void add(Dataset* dataset, DatasetReference*& returnReference);
  
  /**
   * Explicitly removes the entry in the dataset map, erases the entry in the idCounts map and decrements the pointerCounts map by the number of references to the id, deleting the dataset if this count drops below 1
   */
  void remove(string id);

};

#endif
