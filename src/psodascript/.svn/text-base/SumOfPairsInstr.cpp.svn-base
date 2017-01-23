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
#include "SumOfPairsInstr.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include "Timer.h"
#include "QSumOfPairs.h"

using namespace std;

SumOfPairsInstr::SumOfPairsInstr() : AlignmentCommand() {
  setDescription("Calculate the sum of pairs metric for the alignment.");
  initDefaultValue("prog", "default");
  initDefaultValue("gapopen", "");      /* default resolved in Interpreter::getGapSettings() */
  initDefaultValue("gapext", "");       /* default resolved in Interpreter::getGapSettings() */
  initDefaultValue("endgap", "yes");    /* penalizeLeadingGaps*/
  initDefaultValue("submata", "");  /* default resolved in SubMat() */
  initDefaultValue("submatb", "");  /* default resolved in SubMat() */
  initDefaultValue("submatl", "");  /* default resolved in SubMat() */
  //initDefaultValue("guidetree", "best");
  initDefaultValue("guidetree", "");
  initDefaultValue("outfile", "ssalignment.afa");
  initDefaultValue("output", "fasta");
}

SumOfPairsInstr::~SumOfPairsInstr() {
  return;
}

void SumOfPairsInstr::execute(Environment* baseEnv) {
  doSumOfPairs(baseEnv);
}

void SumOfPairsInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

string SumOfPairsInstr::getName() const {
  return "sumOfPairs";
}


/**
 * Calculate sum of pairs for default dataset.
 */
void SumOfPairsInstr::doSumOfPairs(Environment* env) 
{
  assert(env);
  QSumOfPairs *metric = NULL;
  float score = false;
  
  /* set parameters */
  float gapOpen;
  float gapExt;
  bool penalizeLeadingGaps = false;
  bool useReferenceDataset = false;
  bool useSSDataset = false;
  bool justSSSofP = false;
  SubMat* subMatA = NULL;
  SubMat* subMatB = NULL;
  SubMat* subMatL = NULL;
  SubMat* subMat = NULL;
  SubMat* subMatSS = NULL;


  if( !Interpreter::getInstance()->dataset()) {
    throw PsodaError("Cannot perform alignment without a data set.  Did you forget to give the data?  Skipping align().");
    return;
  }

  /* figure out substitution matrix */
  //cout << "HY: creating SubMat" << endl;
  if( env->lookupString("submata").length() > 0){
    subMatA = new SubMat( env->lookupString("submata"), Interpreter::getInstance()->dataset(), false);
    if( subMatA == NULL){
      throw PsodaError("ERROR: substitution matrix for alpha-helices not created for ssalign()!");
    }
  }
  if( env->lookupString("submatb").length() > 0){
    subMatB = new SubMat( env->lookupString("submatb"), Interpreter::getInstance()->dataset(), false);
    if( subMatB == NULL){
      throw PsodaError("ERROR: substitution matrix for beta-sheets not created for ssalign()!");
    }
  }
  if( env->lookupString("submatl").length() > 0){
    subMatL = new SubMat( env->lookupString("submatl"), Interpreter::getInstance()->dataset(), false);
    if( subMatL == NULL){
      throw PsodaError("ERROR: substitution matrix for loops not created for ssalign()!");
    }
  }
  if( env->lookupString("submat").length() > 0){
    subMat = new SubMat( env->lookupString("submat"), Interpreter::getInstance()->dataset(), false);
    if( subMat == NULL){
      throw PsodaError("ERROR: substitution matrix for other not created for ssalign()!");
    }
  }
  
  if( env->lookupString("submatss").length() > 0 &&
      Interpreter::getInstance()->ssDataset() != NULL){
    subMatSS = new SubMat( env->lookupString("submatss"), Interpreter::getInstance()->ssDataset(), false);
    if( subMatSS == NULL){
      throw PsodaError("ERROR: substitution matrix for secondary structure not created for ssalign()!");
    }
  }
  
  getGapSettings(env, &gapOpen, &gapExt, &penalizeLeadingGaps, NULL);

  /* Use reference dataset */
  if( env->lookupString("ref") == "y" ||
      env->lookupString("ref") == "Y" ||
      env->lookupString("ref") == "yes" ||
      env->lookupString("ref") == "YES"){
    useReferenceDataset = true;
  }

  /* use secondary structure dataset */
  if( env->lookupString("ss") == "y" ||
      env->lookupString("ss") == "Y" ||
      env->lookupString("ss") == "yes" ||
      env->lookupString("ss") == "YES"){
    useSSDataset = true;
  }

  /* just look at the secondary structures */
  if( env->lookupString("justss") == "y" ||
      env->lookupString("justss") == "Y" ||
      env->lookupString("justss") == "yes" ||
      env->lookupString("justss") == "YES"){
    justSSSofP = true;
  }
  
  metric = new QSumOfPairs(gapOpen, gapExt, subMatA, subMatB, subMatL, subMat, subMatSS, penalizeLeadingGaps);

  Timer timer;
  if( PSODA_VERBOSE){ timer.start(); }

  /* actually calculate the sum of pairs score after checking if parameters are corrently set */
  if( justSSSofP){
    if( subMatSS == NULL ){
      throw PsodaError("ERROR: Missing the secondary structure substition matrix for ss sumOfPairs!");
    }

    if( !Interpreter::getInstance()->ssDataset()) {
      throw PsodaError("ERROR: Cannot perform just sum-of-pairs using secondary structure, without a SSData block!");
    }
    if( !Interpreter::getInstance()->refSSDataset()) {
      throw PsodaError("ERROR: Cannot perform just sum-of-pairs using secondary structure, without a RefSSData block!");
    }
    score = metric->justReferenceSSSumOfPairs();
  }else if( useSSDataset){
    if( subMat == NULL){
      throw PsodaError("ERROR: Missing the default substition matrix for ss sumOfPairs!");
    }
    if( subMatA == NULL){
      throw PsodaError("ERROR: Missing the alpha-helices substition matrix for ss sumOfPairs!");
    }
    if( subMatB == NULL){
      throw PsodaError("ERROR: Missing the beta-sheet substition matrix for ss sumOfPairs!");
    }
    if( subMatL == NULL){
      throw PsodaError("ERROR: Missing the loops substition matrix for ss sumOfPairs!");
    }
    if( subMatSS == NULL ){
      throw PsodaError("ERROR: Missing the secondary structure substition matrix for ss sumOfPairs!");
    }


    if( !Interpreter::getInstance()->ssDataset()){
      throw PsodaError("ERROR: Cannot perform sum-of-pairs using secondary structure, without a SSData block!");
    }
    
    if( useReferenceDataset ){
      score = metric->referenceSSSumOfPairs();
    }else{
      score = metric->selfSSSumOfPairs();
    }
  }else{
    if( useReferenceDataset ){
      score = metric->referenceSumOfPairs();
    }else{
      score = metric->selfSumOfPairs();
    }
  }
  
  if( PSODA_VERBOSE){
    TimerSecondMicros sm = timer.getCurrentSecondMicros();
    PsodaPrinter::getInstance()->write("sumOfPairs took %li.%li seconds\n" , sm.seconds , sm.micros );
  }
  
  if( metric){
    delete metric;
  }

}  /* END doSumOfPairs(Environment* env) */


