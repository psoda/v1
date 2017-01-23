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
#include "AlignInstr.h"
#include "Interpreter.h"
#include "DatasetReference.h"
#include "QAlign.h"
#include "Timer.h"
#include "PsodaPrinter.h"
#include <assert.h>

using namespace std;

AlignInstr::AlignInstr() : AlignmentCommand() {
  setDescription("Take an unaligned dataset with taxa of potentially different lengths and create an aligned dataset with gaps inserted to create a rectangular matrix.");
  initDefaultValue("name", "", "If name is non-empty, the resulting aligned dataset will be added to the interpreter\'s dataset map with this name.  If this command is used as a function a reference to the stored dataset will be returned and if no name was given, a unique identifier will be assigned");
  initDefaultValue("prog", "pmsa","Program used for alignment.  Progressive Multiple Sequence Alignment is currently the only option");
  initDefaultValue("gapopen", "","Gap Open Penalty defaults to -10 for protein sequences and -15 for everything else");      /* default resolved in Interpreter::getGapSettings() */
  initDefaultValue("gapext", "", "Gap Extension defaults to -0.2 for Protein sequences and -6.6 for everything else");       /* default resolved in Interpreter::getGapSettings() */
  initDefaultValue("endgap", "yes", "Penalize Leading Gaps");    /* penalizeLeadingGaps*/
  initDefaultValue("submat", "", "substition matrix to use for alignment.  Possible values include: IUB, BLOSUM62, SCHNEIDER, LuthyA, LuthyB, LuthyL, identity, or a filename with the matrix.");       /* default resolved in SubMat() */
  //initDefaultValue("guidetree", "best");
  initDefaultValue("guidetree", "", "Create a UPGMA guide tree if NULL");
  addParamOption("guidetree", "best");
  addParamOption("guidetree", "current");
  initDefaultValue("output", "fasta", "The output format");
  initDefaultValue("outfile", "","File name to save the output to");
  initDefaultValue("classic", "No", "if Yes, use the stock (unoptimized) progressivive sequence alignment methods of iterating through every character in one sub-alignment with every other character in the other alignment.  If No, use the optimizied MultChars.");      /* do NOT use the classic progressive multiple sequence alignment--use the MultChar PMSA */
  addParamOption("classic", "Yes");
  initDefaultValue("gapdist", 4, "penalize opening a new gap within this distance to another gap");
  //initDefaultValue("localWeight", 0.0, "amount of contribution of local alignment wieghts.  Range form 0.0 to 1.0.  Set to 0.0 to ignore local alignment weights");
}

AlignInstr::~AlignInstr() {
  return;
}

void AlignInstr::execute(Environment* baseEnv) {
  Dataset* alignedDataset = doAlign(baseEnv);
  // if there wasn't an error, set the current default dataset
  if (alignedDataset) {
    Interpreter::getInstance()->getDatasetMap().setCurrent(alignedDataset);
  }
#ifdef GUI
      PsodaPrinter::getInstance()->write("## Align Completed Successfully\n");
#endif
}

void AlignInstr::execute(Environment* baseEnv, Literal*& returnVal) {
  Dataset* alignedDataset = doAlign(baseEnv);
  string id = baseEnv->lookup("name").toString();
  if (id == "") {
    DatasetReference* returnReference = NULL;
    Interpreter::getInstance()->getDatasetMap().add(alignedDataset, returnReference);
    setResult(returnReference);
  } else {
    Interpreter::getInstance()->getDatasetMap().setD(alignedDataset, id);
    setResult(new DatasetReference(id));
  }
  returnVal = getResult();
#ifdef GUI
      PsodaPrinter::getInstance()->write("## Align Completed Successfully\n");
#endif
}

string AlignInstr::getName() const {
  return "align";
}

/**
 * Align the sequences in the default dataset according to the parameters set in the input file.
 * Does not use secondary structures.
 */
Dataset* AlignInstr::doAlign(Environment* env) 
{
  assert(env);
  QAlign *qAlign = NULL;
  Dataset* mDataset = Interpreter::getInstance()->dataset();
  
  if(mDataset == NULL) {
	  PsodaPrinter::getInstance()->write("Current dataset is not loaded.  Select the Run button to input the data\n");
		return(NULL);
  }
    // If the dataset is protein coding dna, convert to codons
  if(mDataset->datatype() == Dataset::PROTEIN_CODING_DNA_DATATYPE)
	mDataset->convertDNAToCodons();
  
  /* set parameters */
  bool useExistingTree = false;
  float gapOpen;
  float gapExt;
  bool penalizeLeadingGaps;
  SubMat* subMat;
  bool useClassicPMSA = false;
  int gapDistanceSetting = -1;
  float localWeight = 0.0;

  if( !Interpreter::getInstance()->dataset()) {
    throw PsodaError("Cannot perform alignment without a data set.  Did you forget to give the data?  Skipping align().");
    //    return;
  }

  /* figure out substitution matrix */
  //cout << "HY: creating SubMat" << endl;
  subMat = new SubMat( env->lookupString("submat"), Interpreter::getInstance()->dataset());
  if( subMat == NULL){
    throw PsodaError("\nERROR: substitution matrix not created for align()!\n\n");
  }
  
  /* get guidetree */
  if( PSODA_DEBUG){ cout << "HY: getting guide tree" << endl; }
  if( env->lookupString("guidetree") == "best" ||
      env->lookupString("guidetree") == "current" ){
    useExistingTree = true;
//     QTree* bestTree = Interpreter::getInstance()->qtreeRepository()->getTree();
//     if( bestTree == NULL){
//       cerr << "No trees in the repository() for Align" << endl;
//       return NULL;
//     }
//     guideTree = bestTree->treeStr();
//     cout << "PSODA_DEBUG: guide tree: " << guideTree << endl;
  }else if( env->lookupString("guidetree") == ""){
//     guideTree = NULL;
    useExistingTree = false;
  }else{
    cerr << "ERROR: guidetree parameter \"" << env->lookupString("guidtree") << "\" of Align not recognized (or NYI)!" << endl;
    exit(1);
  }

  getGapSettings(env, &gapOpen, &gapExt, &penalizeLeadingGaps, &gapDistanceSetting);

  if( env->lookupString("classic") == "y" ||
      env->lookupString("classic") == "Y" ||
      env->lookupString("classic") == "yes" ||
      env->lookupString("classic") == "YES"){
    useClassicPMSA = true;
  }

  if( env->lookupString("localweight") != ""){
    localWeight = strtof( env->lookupString("localweight").c_str(), (char**)NULL);
  }
  
  //if( env->lookupString("addlocal") == "y" ||
  //    env->lookupString("addlocal") == "Y" ||
  //    env->lookupString("addlocal") == "yes" ||
  //    env->lookupString("addlocal") == "YES"){
  //  addLocal = true;
  //}

  /* determine the algorithm for alignment: clustalw, muscle or standard pmsa */
  if( env->lookupString("prog") == "clustal"){
    cerr << "Clustalw alignment NOT YET IMPLEMENTED" << endl;
    return NULL;
  }else if( env->lookupString("prog") == "muscle"){
    cerr << "MUSCLE alignment NOT YET IMPLEMENTED" << endl;
    return NULL;
  }else if( env->lookupString("prog") == "pmsa" ||
	    env->lookupString("prog") == "default" ){
    qAlign = new QAlign(gapOpen, gapExt, subMat, penalizeLeadingGaps, useClassicPMSA, gapDistanceSetting, localWeight);
  }else{
    cerr << "ERROR: prog parameter ( " << env->lookupString("prog") << " of Align not recognized!" << endl;
    return NULL;
  }


  /* run alignment */
  Timer timer;
  if( PSODA_VERBOSE){ 
    timer.start();
  }
  Dataset* alignedDataset = NULL;
  qAlign->align(*Interpreter::getInstance()->qtreeRepository(), useExistingTree, &alignedDataset);
  // Instead of setting the dataset here, we will return a pointer of it to the caller (execute method in the AlignInstr) who will take care of it
  // setDataset(alignedDataset);

  if( PSODA_VERBOSE){
    TimerSecondMicros sm = timer.getCurrentSecondMicros();
    PsodaPrinter::getInstance()->write("Alignment took %li.%li seconds\n" , sm.seconds , sm.micros );
  }

  
  /* print and/or save results */
  
  if(env->lookupString("outfile") != "") {
    alignedDataset->printSeqs( env->lookupString("output"), env->lookupString("outfile"));
  }
  if( PSODA_DEBUG){
    alignedDataset->printSeqs( env->lookupString("output"), "STDOUT");
  }

  
  if( qAlign)
  {
    delete qAlign;
  }

  /* Make the newly aligned dataset the default */
  Interpreter::getInstance()->installDataset(mDataset);

  return alignedDataset;
}  /* END doAlign(Environment* env) */
