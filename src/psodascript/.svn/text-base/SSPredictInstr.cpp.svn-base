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
#include "SSPredictInstr.h"
#include "Interpreter.h"
#include "QSSPredict.h"
#include "PsodaPrinter.h"
#include "Timer.h"

using namespace std;

SSPredictInstr::SSPredictInstr() : AlignmentCommand() {
  setDescription("Predict the secondary structure for a sequence using a neural network.");
  return;
}

SSPredictInstr::~SSPredictInstr() {
  return;
}

void SSPredictInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void SSPredictInstr::execute(Environment* baseEnv) 
{
  doSSPredict(baseEnv);
#ifdef GUI
      PsodaPrinter::getInstance()->write("## SSAlign Completed Successfully\n");
#endif
}

string SSPredictInstr::getName() const {
  return "sspredict";
}

/**
 * Predict secondary structure for the sequences in the default dataset.
 */

void SSPredictInstr::doSSPredict(Environment* env) 
{
  assert(env);
  QSSPredict *qSSPredict = NULL;

  // make array of net file names
  char* netfiles[10];
  netfiles[0] = "src/alignment/nets/casp7_shuffled.fann0.net";
  netfiles[1] = "src/alignment/nets/casp7_shuffled.fann1.net";
  netfiles[2] = "src/alignment/nets/casp7_shuffled.fann2.net";
  netfiles[3] = "src/alignment/nets/casp7_shuffled.fann3.net";
  netfiles[4] = "src/alignment/nets/casp7_shuffled.fann4.net";
  netfiles[5] = "src/alignment/nets/casp7_shuffled.fann5.net";
  netfiles[6] = "src/alignment/nets/casp7_shuffled.fann6.net";
  netfiles[7] = "src/alignment/nets/casp7_shuffled.fann7.net";
  netfiles[8] = "src/alignment/nets/casp7_shuffled.fann8.net";
  netfiles[9] = "src/alignment/nets/casp7_shuffled.fann9.net";
  
  if( !Interpreter::getInstance()->dataset()) {
    throw PsodaError("Cannot perform ss prediction without a data set.  Did you forget to give the data?  Skipping sspredict().");
    return;
  }

  qSSPredict = new QSSPredict(netfiles, 10);

  /* run prediction */
  Timer timer;
  if( PSODA_VERBOSE){ timer.start(); }
  if( PSODA_DEBUG){ cerr << "INFO: calling sspredict()" << endl; }
  SSDataset* predictedSSDataset = NULL;
  qSSPredict->sspredict(&predictedSSDataset);
  Interpreter::getInstance()->installSSDataset(predictedSSDataset);
  
  if( PSODA_VERBOSE){
    TimerSecondMicros sm = timer.getCurrentSecondMicros();
    PsodaPrinter::getInstance()->write("Prediction took %li.%li seconds\n" , sm.seconds , sm.micros );
  }
  
  /* print and/or save results */
  Interpreter::getInstance()->dataset()->printSeqs( env->lookupString("output"), env->lookupString("outfile"));
  Interpreter::getInstance()->ssDataset()->printSeqs( env->lookupString("outputSS"), env->lookupString("outfileSS"));
  
  if( qSSPredict)
  {
    delete qSSPredict;
  }

}  /* END doSSPredict(Environment* env) */
