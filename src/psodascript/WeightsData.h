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
#ifndef PSODA_WEIGHTS_DATA_H
#define PSODA_WEIGHTS_DATA_H

#include "ProgramGraphNode.h"
//#include "Weights.h"
#include "Expression.h"
#include <vector>

using namespace std;

struct Entry {
  Expression* start;
  Expression* end;
  Expression* increment;
  bool openEnded;
};

struct WeightPair {
  Expression* value;
  vector<Entry>* numberList;
};

class WeightsData {

 private:

  //Prohibit copying of this object
  void operator =(WeightsData&);
  WeightsData(WeightsData&);

 protected:
  
  vector<WeightPair> weightPairList;

 public:

  /**
   * Constructor
   */
  WeightsData();

  /**
   * Destructor
   */
  virtual ~WeightsData();

  /**
   * Begins a new weightPairVector
   */
  void beginWeightPair(Expression* newValue);

  /**
   * Begins a number list entry
   */
  void beginNumberListEntry();

  /**
   * Sets the start on the current entry
   */
  void setEntryStart(Expression* newStart);
    
  /**
   * Sets the end on the current entry
   */
  void setEntryEnd(Expression* newEnd);
      
  /**
   * Sets the increment value on the current entry
   */
  void setEntryIncr(Expression* newIncr);
	
  /**
   * Sets the open ended property on the current entry (the end will be the very last weight)
   */
  void setEntryOpenEnded();
	  
  /**
   * Get the weights data
   */
  vector<WeightPair>& getWeights();

};

#endif

