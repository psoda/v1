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
#include "WeightsData.h"
//#include "Weights.h"
#include "Interpreter.h"
#include "Dataset.h"
#include "IntLiteral.h"
#include "LiteralExpression.h"
#include "PsodaWarning.h"

using namespace std;

WeightsData::WeightsData() : weightPairList() {
  return;
}

WeightsData::~WeightsData() {
  vector<WeightPair>::const_iterator weightPairItr = weightPairList.begin();
  vector<WeightPair>::const_iterator weightPairEnd = weightPairList.end();
  // delete the things that were stored on the heap
  for (; weightPairItr != weightPairEnd; weightPairItr++) {
    WeightPair thisWeightPair = *weightPairItr;
    vector<Entry>* thisWeightEntryList = thisWeightPair.numberList;

    vector<Entry>::iterator weightEntryItr = thisWeightEntryList->begin();
    vector<Entry>::iterator weightEntryEnd = thisWeightEntryList->end();
    // delete the vectors that were stored on the heap
    for (; weightEntryItr != weightEntryEnd; weightEntryItr++) {
      Entry& thisEntry = *weightEntryItr;
      if (thisEntry.start) {
	delete thisEntry.start;
	thisEntry.start = 0;
      }
      if (thisEntry.end) {
	delete thisEntry.end;
	thisEntry.end = 0;
      }
      if (thisEntry.increment) {
	delete thisEntry.increment;
	thisEntry.increment = 0;
      }
    }
    delete thisWeightPair.numberList;
    thisWeightPair.numberList = 0;

    delete thisWeightPair.value;
    thisWeightPair.value = 0;
  }
}

void WeightsData::beginWeightPair(Expression* newValue) {
  WeightPair newWeightPair;
  newWeightPair.value = newValue;
  newWeightPair.numberList = new vector<Entry>();
  weightPairList.push_back(newWeightPair);
}

void WeightsData::beginNumberListEntry() {
  Entry newEntry;
  newEntry.start = new LiteralExpression(new IntLiteral(1));
  newEntry.end = new LiteralExpression(new IntLiteral(1));
  newEntry.increment = new LiteralExpression(new IntLiteral(1));
  newEntry.openEnded = false;
  weightPairList.back().numberList->push_back(newEntry);
}

void WeightsData::setEntryStart(Expression* newStart) {
  Expression*& currentStart = weightPairList.back().numberList->back().start;
  if (currentStart) {
    delete currentStart;
    currentStart = NULL;
  }
  currentStart = newStart;
}

void WeightsData::setEntryEnd(Expression* newEnd) {
  Expression*& currentEnd = weightPairList.back().numberList->back().end;
  if (currentEnd) {
    delete currentEnd;
    currentEnd = NULL;
  }
  currentEnd = newEnd;
}

void WeightsData::setEntryIncr(Expression* newIncr) {
  Expression*& currentIncr = weightPairList.back().numberList->back().increment;
  if (currentIncr) {
    delete currentIncr;
    currentIncr = NULL;
  }
  currentIncr = newIncr;
}

void WeightsData::setEntryOpenEnded() {
  weightPairList.back().numberList->back().openEnded = true;
}

vector<WeightPair>& WeightsData::getWeights() {
  return weightPairList;
}
