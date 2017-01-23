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
#include "SSAlignInstr.h"
#include "Interpreter.h"
#include "QSSAlign.h"
#include "PsodaPrinter.h"
#include "Timer.h"

using namespace std;

SSAlignInstr::SSAlignInstr() : AlignmentCommand() {
  setDescription("Take an unaligned dataset with taxa of potentially different lengths and create an aligned dataset with gaps inserted to create a rectangular matrix.  Use the secondary structure to align the sequences using chemical properties with weights optimized for the particular secondary structure");
  initDefaultValue("prog", "default","Program used for alignment.  Progressive Multiple Sequence Alignment is currently the only option");
  initDefaultValue("gapopen", "","Gap Open Penalty defaults to -10 for protein sequences and -15 for everything else");      /* default resolved in Interpreter::getGapSettings() */
  initDefaultValue("submata", "", "substition matrix to use for alignment in alpha helix regions.  Possible values include: IUB, BLOSUM62, SCHNEIDER, LuthyA, LuthyB, LuthyL, identity, or a filename with the matrix.");       /* default resolved in SubMat() */
  initDefaultValue("submatb", "", "substition matrix to use for alignment in beta sheet regions.  Possible values include: IUB, BLOSUM62, SCHNEIDER, LuthyA, LuthyB, LuthyL, identity, or a filename with the matrix.");       /* default resolved in SubMat() */
  initDefaultValue("submatl", "", "substition matrix to use for alignment in loop regions.  Possible values include: IUB, BLOSUM62, SCHNEIDER, LuthyA, LuthyB, LuthyL, identity, or a filename with the matrix.");       /* default resolved in SubMat() */
  initDefaultValue("outfile", "ssalignment.afa","File name to save the output to");
  initDefaultValue("outputSS", "fasta", "The output format");
  initDefaultValue("outfileSS", "alignment.ss.afa","File name to save the SS to");
  initDefaultValue("outputCombined", "","The output format");
  initDefaultValue("outfileCombined", "","File name to save the combined output to");
  initDefaultValue("gapdist", 4, "penalize opening a new gap within this distance to another gap");
  initDefaultValue("gapext", "", "Gap Extension defaults to -0.2 for Protein sequences and -6.6 for everything else");       /* default resolved in Interpreter::getGapSettings() */
  initDefaultValue("endgap", "yes", "Penalize Leading Gaps");    /* penalizeLeadingGaps*/
  //initDefaultValue("guidetree", "best");
  initDefaultValue("guidetree", "", "Create a UPGMA guide tree if NULL");
  addParamOption("guidetree", "best");
  addParamOption("guidetree", "current");
  initDefaultValue("classic", "No", "if Yes, use the stock (unoptimized) progressivive sequence alignment methods of iterating through every character in one sub-alignment with every other character in the other alignment.  If No, use the optimizied MultChars.");      /* do NOT use the classic progressive multiple sequence alignment--use the MultChar PMSA */
  addParamOption("classic", "Yes");
}

SSAlignInstr::~SSAlignInstr() {
  return;
}

void SSAlignInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void SSAlignInstr::execute(Environment* baseEnv) {
  doSSAlign(baseEnv);
#ifdef GUI
      PsodaPrinter::getInstance()->write("## SSAlign Completed Successfully\n");
#endif
}

string SSAlignInstr::getName() const {
  return "ssalign";
}


/**
 * Align the sequences in the default dataset using secondary structures according to the parameters set in the input file.
 */

void SSAlignInstr::doSSAlign(Environment* env) 
{
  assert(env);
  QSSAlign *qSSAlign = NULL;
  Dataset* mDataset = Interpreter::getInstance()->dataset();
  
  /* set parameters */
  bool useExistingTree = false;
  float gapOpen;
  float gapOpenHelix;
  float gapOpenBetaStrand;
  float gapOpenLoop;
  float gapExt;
  bool penalizeLeadingGaps;
  SubMat* subMatA = NULL;
  SubMat* subMatB = NULL;
  SubMat* subMatL = NULL;
  SubMat* subMat = NULL;
  SubMat* subMatSS = NULL;

  bool useClassicPMSA = false;

  int gapDistanceSetting = -1;
  float localWeight = 0.0;
  
  if( !Interpreter::getInstance()->dataset()) {
    throw PsodaError("Cannot perform alignment without a data set.  Did you forget to give the data?  Skipping align().");
    return;
  }

  if( !Interpreter::getInstance()->ssDataset()) {
    throw PsodaError("Cannot perform ssalign without a secondary structure data set.  Include a SSData block.  Skipping ssalign().");
    return;
  }

  /* figure out substitution matrix */
  //cout << "HY: creating SubMat" << endl;
  subMatA = new SubMat( env->lookupString("submata"), Interpreter::getInstance()->dataset());
  if( subMatA == NULL){
    throw PsodaError("ERROR: substitution matrix for alpha-helices not created for ssalign()!");
  }
  subMatB = new SubMat( env->lookupString("submatb"), Interpreter::getInstance()->dataset());
  if( subMatB == NULL){
    throw PsodaError("ERROR: substitution matrix for beta-sheets not created for ssalign()!");
  }
  subMatL = new SubMat( env->lookupString("submatl"), Interpreter::getInstance()->dataset());
  if( subMatL == NULL){
    throw PsodaError("ERROR: substitution matrix for loops not created for ssalign()!");
  }
  
  subMat = new SubMat( env->lookupString("submat"), Interpreter::getInstance()->dataset());
  if( subMat == NULL){
    throw PsodaError("ERROR: substitution matrix for other not created for ssalign()!");
  }
  
  subMatSS = new SubMat( env->lookupString("submatss"), Interpreter::getInstance()->ssDataset());
  if( subMat == NULL){
    throw PsodaError("ERROR: substitution matrix for secondary structure not created for ssalign()!");
  }
  
  /* get guidetree */
  if( env->lookupString("guidetree") == "best" ||
      env->lookupString("guidetree") == "current" ){
    useExistingTree = true;
  }else if( env->lookupString("guidetree") == ""){
    useExistingTree = false;
  }else{
    cerr << "ERROR: guidetree parameter \"" << env->lookupString("guidtree") << "\" of Align not recognized (or NYI)!" << endl;
    exit(1);
  }

  /* classic setting does not use the multchar optimatizations */
  if( env->lookupString("classic") == "y" ||
      env->lookupString("classic") == "Y" ||
      env->lookupString("classic") == "yes" ||
      env->lookupString("classic") == "YES"){
    useClassicPMSA = true;
  }

  /* Include extra weight for local alignments */
  if( env->lookupString("localweight") != ""){
    localWeight = strtof( env->lookupString("localweight").c_str(), (char**)NULL);
  }
  
  //if( env->lookupString("addlocal") == "y" ||
  //    env->lookupString("addlocal") == "Y" ||
  //    env->lookupString("addlocal") == "yes" ||
  //    env->lookupString("addlocal") == "YES"){
  //  addLocal = true;
  //}
  
  getGapSettings(env, &gapOpen, &gapOpenHelix, &gapOpenBetaStrand, &gapOpenLoop, &gapExt, &penalizeLeadingGaps, &gapDistanceSetting);

  qSSAlign = new QSSAlign(gapOpen, gapOpenHelix, gapOpenBetaStrand, gapOpenLoop, gapExt, subMatA, subMatB, subMatL, subMat, subMatSS, penalizeLeadingGaps, useClassicPMSA, gapDistanceSetting, localWeight);

  /* run alignment */
  Timer timer;
  if( PSODA_VERBOSE){ timer.start(); }
  if( PSODA_DEBUG){ cerr << "INFO: calling ssalign()" << endl; }
  Dataset* alignedDataset = NULL;
  SSDataset* alignedSSDataset = NULL;
  qSSAlign->ssalign(*Interpreter::getInstance()->qtreeRepository(), useExistingTree, &alignedDataset, &alignedSSDataset);
  Interpreter::getInstance()->setDataset(alignedDataset);
  Interpreter::getInstance()->installSSDataset(alignedSSDataset);
  
  if( PSODA_VERBOSE){
    TimerSecondMicros sm = timer.getCurrentSecondMicros();
    PsodaPrinter::getInstance()->write("Alignment took %li.%li seconds\n" , sm.seconds , sm.micros );
  }
  
  /* print and/or save results */
  Interpreter::getInstance()->dataset()->printSeqs( env->lookupString("output"), env->lookupString("outfile"));
  Interpreter::getInstance()->ssDataset()->printSeqs( env->lookupString("outputSS"), env->lookupString("outfileSS"));

  if( env->lookupString("outputCombined").length() > 0){
   Interpreter::getInstance()->ssDataset()->printCombinedSeqs( env->lookupString("outputCombined"), env->lookupString("outfileCombined"), Interpreter::getInstance()->dataset());
  }

  
  if( qSSAlign)
  {
    delete qSSAlign;
  }

  /* Make the newly aligned dataset the default */
  Interpreter::getInstance()->installDataset(mDataset);

}  /* END doSSAlign(Environment* env) */

