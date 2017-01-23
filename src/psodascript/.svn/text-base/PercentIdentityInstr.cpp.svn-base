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
#include "PercentIdentityInstr.h"
#include "Interpreter.h"
#include "QAlign.h"
#include "Timer.h"
#include "PsodaPrinter.h"

using namespace std;

PercentIdentityInstr::PercentIdentityInstr() : AlignmentCommand() {
  setDescription("Calculate the percent identity for the alignment");
  initDefaultValue("gapopen", "", "Gap Open penalty");      /* default resolved in Interpreter::getGapSettings() */
  initDefaultValue("gapext", "", "Gap Extension penalty");       /* default resolved in Interpreter::getGapSettings() */
  initDefaultValue("endgap", "yes", "penalize Leading Gaps?");    /* penalizeLeadingGaps*/
  initDefaultValue("submat", "");       /* default resolved in SubMat() */
  initDefaultValue("counts", "", "print matrix of accumulative pairwise (mis)matches");       /* print matrix of accumaltive pairwise (mis)matches */
}

PercentIdentityInstr::~PercentIdentityInstr() {
  return;
}

void PercentIdentityInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void PercentIdentityInstr::execute(Environment* baseEnv) {
  doPercentIdentity(baseEnv);
}

string PercentIdentityInstr::getName() const {
  return "PercentIdentity";
}

/**
 * Calculate and print out the percent identity for the default dataset.
 */
void PercentIdentityInstr::doPercentIdentity(Environment* env) 
{
  assert(env);
  QAlign *qAlign = NULL;
  
  /* set parameters */
  float gapOpen;
  float gapExt;
  bool penalizeLeadingGaps;
  SubMat* subMat;
  int gapDistanceSetting = -1;
  
  bool printCounts = false;
  
  if( !Interpreter::getInstance()->dataset()) {
    throw PsodaError("Cannot calculate percent identity without a data set.  Did you forget to give the data?  Skipping PercentIdentity().");
    return;
  }

  /* figure out substitution matrix */
  //cout << "HY: creating SubMat" << endl;
  if( env->lookupString("submat").length() > 0){
    subMat = new SubMat( env->lookupString("submat"), Interpreter::getInstance()->dataset(), false);
  }else{
    subMat = new SubMat( "identity", Interpreter::getInstance()->dataset(), false);
  }
  if( subMat == NULL){
    throw PsodaError("\nERROR: substitution matrix not created for align()!\n\n");
  }
  
  getGapSettings(env, &gapOpen, &gapExt, &penalizeLeadingGaps, &gapDistanceSetting);

  if( env->lookupString("counts").length() > 0){
    printCounts = true;
  }

  
  Timer timer;
  if( PSODA_VERBOSE){ timer.start(); }
  qAlign = new QAlign(gapOpen, gapExt, subMat, penalizeLeadingGaps, false, gapDistanceSetting, false);
  qAlign->percentIdentity( printCounts);

  if( PSODA_VERBOSE){
    TimerSecondMicros sm = timer.getCurrentSecondMicros();
    PsodaPrinter::getInstance()->write("Percent Identity took %li.%li seconds\n" , sm.seconds , sm.micros );
  }

  if( qAlign)
  {
    delete qAlign;
  }

}  /* END doPercentIdentity(Environment* env) */
