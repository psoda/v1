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
#include "WeightsInstr.h"
#include "WeightsData.h"
#include "Data.h"
#include "Interpreter.h"
#include "BuiltInCommand.h"
#include "Dataset.h"
#include "IntLiteral.h"
#include "LiteralExpression.h"
#include "PsodaWarning.h"

using namespace std;

struct WeightsPair;

WeightsInstr::WeightsInstr() : BuiltInCommand() {
  setDescription("Adjust the weights across the columns of the alignment.");
  return;
}

WeightsInstr::~WeightsInstr() {
  return;
}

int* WeightsInstr::init() const {
  //getting the weights object from the interpreter's dataset
  //we will modify the weights directly rather than through the interpreter and dataset objects
  Dataset *dset = Interpreter::getInstance()->dataset();
  return dset->weights();
}

void WeightsInstr::execute(Environment* baseEnv,Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void WeightsInstr::execute(Environment* baseEnv) 
{
  Environment* varEnv = getVariableEnv(baseEnv);
  Dataset *dset = Interpreter::getInstance()->dataset();
  //sets up default dataset and gets mWeights
  //mWeights points to the actual weights object in the interpreter's dataset
  //changing mWeights will modify the interpreter
  int* mWeights = init();

  //The number of characters in each taxon (DNA sequence)
  //  int numColumns = Interpreter::getInstance()->dataset()->nchars();
  int numColumns = dset->nchars();

  WeightsData* weightsData = (WeightsData*) baseEnv->lookup("*weights").getData();
  if (weightsData == NULL) 
  {
    return;
  }

  vector<WeightPair>& weightPairList = weightsData->getWeights();
  //iterate over weight pairs
  vector<WeightPair>::const_iterator weightPairItr = weightPairList.begin();
  vector<WeightPair>::const_iterator weightPairEnd = weightPairList.end();
  for (; weightPairItr != weightPairEnd; weightPairItr++) 
  {
    WeightPair thisWeightPair = *weightPairItr;

    //iterate over the numberList entries for thisWeightPair
    vector<Entry>::const_iterator numberListItr = thisWeightPair.numberList->begin();
    vector<Entry>::const_iterator numberListEnd = thisWeightPair.numberList->end();
    for (; numberListItr != numberListEnd; numberListItr++) 
    {
      Entry thisEntry = *numberListItr;

      int end;
      int start = thisEntry.start->evaluate(varEnv)->toInt();
      int increment = thisEntry.increment->evaluate(varEnv)->toInt();

      //check for open-ended
      if (thisEntry.openEnded) 
      {
        end = numColumns;
      } 
      else 
      {
        if (thisEntry.end) 
        {
          end = thisEntry.end->evaluate(varEnv)->toInt();
        } 
        else 
        {
          // .end was NULL so just use the start value
          end = start;
        }
      }
      
      //Bounds checking
      //PaupBlock weights are 1 based indexing
      if (start < 1 || end > numColumns) 
      {
        cout << "ERROR:  Weight column index out of range" << endl;
        cout << "valid range: " << 1 << " - " << numColumns << endl;
        cout << "attempted range: " << start << " - " << end << endl;
        exit(1);
      }

      for (int i = start; i <= end; i += increment) 
      {
        //The PSODA grammar indexing on weights is 1 based but the mWeights object is 0 based so subtract 1 from the index
        int thisValue = thisWeightPair.value->evaluate(varEnv)->toInt();
        mWeights[i - 1] = thisValue;
      }
    }
  }
  if (Interpreter::getInstance()->qtreeRepository()) 
  {
    Interpreter::getInstance()->qtreeRepository()->clearVisited();
  }

}

string WeightsInstr::getName() const {
  return "weights";
}

  /*
string WeightsInstr::toString() const {
  stringstream returnString;
  returnString << getName();
  //go through each pair and print out the key followed by its vector
  vector<WeightPair>::const_iterator weightPairItr = weightPairList.begin();
  vector<WeightPair>::const_iterator weightPairEnd = weightPairList.end();
  bool isFirstWeightPair = true;
  for (; weightPairItr != weightPairEnd; weightPairItr++) {
    WeightPair thisWeightPair = *weightPairItr;
    if (isFirstWeightPair) {
      returnString << " ";
      isFirstWeightPair = false;
    } else {
      returnString << ",";
    }
    returnString << thisWeightPair.value->toString() << " :";
    bool isFirstEntry = true;

    vector<Entry>::const_iterator numberListItr = thisWeightPair.numberList->begin();
    vector<Entry>::const_iterator numberListEnd = thisWeightPair.numberList->end();
    for (; numberListItr != numberListEnd; numberListItr++) {
      Entry thisEntry = *numberListItr;
      //check for all

      string start = thisEntry.start->toString();
      string increment = thisEntry.increment->toString();

      if (isFirstEntry && start == "1" && thisEntry.openEnded && increment == "1") {
	returnString << " all";
      } else {
	returnString << " " << start;
	if (thisEntry.openEnded) {
	  returnString << "-.";
	} else if (thisEntry.end) {
	  returnString << "-" << thisEntry.end->toString();
	}
	if (increment != "1") {
	  returnString << "\\" << increment;
	}
      }
      isFirstEntry = false;
    }
  }
  return returnString.str();
}
  */
