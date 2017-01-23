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
#include "DatasetMap.h"
#include "PsodaPrinter.h"
#include <assert.h>

using namespace std;

DatasetMap::DatasetMap() : datasets() {
  return;
}

DatasetMap::~DatasetMap() {

  if (originalDatasetReference) {
    delete originalDatasetReference;
    originalDatasetReference = NULL;
  }
  originalDataset = NULL;

  if (currentDatasetReference) {
    delete currentDatasetReference;
    currentDatasetReference = NULL;
  }
  currentDataset = NULL;

  map<Dataset*, int>::iterator pointerCountItr = pointerCounts.begin();
  map<Dataset*, int>::iterator pointerCountEnd = pointerCounts.end();
  while (pointerCountItr != pointerCountEnd) {
    delete pointerCountItr->first;
    pointerCountItr++;
  }

  datasets.clear();
  idCounts.clear();
  pointerCounts.clear();
  names.clear();
}

string DatasetMap::assignId(Dataset* dataset) {
  ostringstream id;
  id << (Dataset*) dataset << "**PSODA_ID**";
  return id.str();
}

Dataset* DatasetMap::getOriginal() const {
  return originalDataset;
}

Dataset* DatasetMap::getCurrent() const {
  return currentDataset;
}

Dataset* DatasetMap::getDataset(string id) const {
  map<string, Dataset*>::const_iterator datasetFound = datasets.find(id);
  if (datasetFound != datasets.end()) {
    return datasetFound->second;
  } else {
    return NULL;
  }  
}

void DatasetMap::decrementRefCount(string id) {
  // Get the pointer by the id
  map<string, Dataset*>::iterator datasetFound = datasets.find(id);
  if (datasetFound != datasets.end()) {
    // This means that the user gave it a real name so he may be keeping track of it on paper or something
    bool isNamed = names.find(id) != names.end();

    Dataset* datasetPtr = datasetFound->second;
    map<string, int>::iterator idCountFound = idCounts.find(id);
    int oldIdCount = 0;
    if (idCountFound != idCounts.end()) {
      oldIdCount = idCountFound->second;
    }
    int newIdCount = oldIdCount - 1;
    if (newIdCount > 0 || isNamed) {
      idCounts[id] = newIdCount;
    } else {
      // erase the entry from the datasets and the idCounts
      datasets.erase(datasetFound);
      idCounts.erase(idCountFound);
    }

    map<Dataset*, int>::iterator pointerCountFound = pointerCounts.find(datasetPtr);
    if (pointerCountFound != pointerCounts.end()) {
      int newPointerCount = pointerCountFound->second - 1;
      if (newPointerCount > 0 || isNamed) 
      {
        pointerCounts[datasetPtr] = newPointerCount;
      } 
      else if (newPointerCount < 1) 
      {
          /* This is a HACK: We need to fix this.
             Currently, we have a bug that ends up 
             doing a double delete of these datasets.
             The bug is in the reference counting
          */
        // delete the dataset and erase the entry
        //delete datasetPtr;
        //datasetPtr = NULL;
        //pointerCounts.erase(pointerCountFound);
      }
    }
  }
}

void DatasetMap::incrementRefCount(string id) {
  // Get the pointer by the id
  map<string, Dataset*>::iterator datasetFound = datasets.find(id);
  if (datasetFound != datasets.end()) {
    Dataset* datasetPtr = datasetFound->second;
    idCounts[id] = idCounts[id] + 1;
    pointerCounts[datasetPtr] = pointerCounts[datasetPtr] + 1;
  }
}

void DatasetMap::setOriginal(Dataset* dataset) {
  DatasetReference* newOriginalDatasetReference = NULL;
  add(dataset, newOriginalDatasetReference);
  if (originalDatasetReference) {
    delete originalDatasetReference;
    originalDatasetReference = newOriginalDatasetReference;
  }
  originalDataset = dataset;
}

void DatasetMap::setCurrent(string id) {
  Dataset* dataset = getDataset(id);
  if (dataset) {
    PsodaPrinter::getInstance()->write("Set current dataset to \"%s\"\n", id.c_str());
    setCurrent(dataset);
  } else {
    PsodaPrinter::getInstance()->write("Warning: didn\'t find your dataset \"%s\"\n", id.c_str());
  }
}

void DatasetMap::setCurrent(Dataset* dataset) {
  DatasetReference* newCurrentDatasetReference = NULL;
  add(dataset, newCurrentDatasetReference);
  if (currentDatasetReference) {
    delete currentDatasetReference;
  }
  currentDatasetReference = newCurrentDatasetReference;
  currentDataset = dataset;
}



void DatasetMap::transferId(string id, Dataset* oldDataset, Dataset* newDataset) {
  assert(oldDataset == datasets.find(id)->second);
  // No need to transfer counts if they are the same
  if (oldDataset == newDataset) return;

  // set the id to point to the newDataset
  datasets[id] = newDataset;

  // Now, we need to decrement the oldDataset pointerCounter and increment the newDataset pointerCounts by the idCounts of this id
  map<string, int>::iterator idCountFound = idCounts.find(id);
  if (idCountFound != idCounts.end()) {
    int idCount = idCountFound->second;

    // Update the counts on the old dataset
    map<Dataset*, int>::iterator oldDatasetPointerCountFound = pointerCounts.find(oldDataset);
    if (oldDatasetPointerCountFound != pointerCounts.end()) {
      int newCountForOldDatasetPointer = oldDatasetPointerCountFound->second - idCount;
      if (newCountForOldDatasetPointer > 0) {
	pointerCounts[oldDataset] = newCountForOldDatasetPointer;
      } else {
	// otherwise, we should delete this dataset
	delete oldDataset;
	pointerCounts.erase(oldDatasetPointerCountFound);
      }
    }

    // Update the counts on the new dataset
    map<Dataset*, int>::iterator newDatasetPointerCountFound = pointerCounts.find(newDataset);
    int oldCoundForNewDatasetPointer = 0;
    if (newDatasetPointerCountFound != pointerCounts.end()) {
      oldCoundForNewDatasetPointer = oldDatasetPointerCountFound->second;
    }
    pointerCounts[newDataset] = oldCoundForNewDatasetPointer + idCount;
  } else {
    // if there were no counts for the given id, no need to transfer
    return;
  }
}

void DatasetMap::setD(Dataset* dataset, string id) {
  DatasetReference* tempReference = NULL;
  addReference(dataset, id, tempReference);

  // keep track of the fact that the reference that may be stored in someone's head, or written down or something
  names.insert(id);

  // now delete the reference that was returned
  delete tempReference;
  tempReference = NULL;
}

void DatasetMap::add(Dataset* dataset, DatasetReference*& returnReference) {
  addReference(dataset, assignId(dataset), returnReference);
}

void DatasetMap::addReference(Dataset* dataset, string id, DatasetReference*& returnReference) {
  map<string, Dataset*>::iterator oldDatasetFound = datasets.find(id);
  if (oldDatasetFound != datasets.end()) {
    // transfer the id if there was already a datset being pointed to by id
    transferId(id, oldDatasetFound->second, dataset);
  } else {
    // otherwise, initialize the id count to 0
    idCounts[id] = 0;
    // and add an entry in the datsets map
    datasets[id] = dataset;
  }

  // initialize the pointerCounts to zero only if there was no entry before
  map<Dataset*, int>::iterator pointerCountFound = pointerCounts.find(dataset);
  if (pointerCountFound == pointerCounts.end()) {
    pointerCounts[dataset] = 0;
  }

  returnReference = new DatasetReference(id);
}

void DatasetMap::remove(string id) {
  // delete the dataset with the given id and update the counts
  map<string, Dataset*>::iterator datasetFound = datasets.find(id);
  if (datasetFound != datasets.end()) {
    Dataset* dataset = datasetFound->second;

    map<string, int>::iterator idCountFound = idCounts.find(id);
    int idCount = idCountFound->second;
    idCounts.erase(idCountFound);
    datasets.erase(datasetFound);
    names.erase(id);

    map<Dataset*, int>::iterator pointerCountFound = pointerCounts.find(dataset);
    int newPointerCount = pointerCountFound->second - idCount;
    if (newPointerCount > 0) {
      pointerCounts[dataset] = newPointerCount;
    } else {
      pointerCounts.erase(pointerCountFound);
      delete dataset;
      dataset = NULL;
    }
  }
}
