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
#include "QSSPredict.h"
#include "Timer.h"
#include "Interpreter.h"
#include <assert.h>
#include <string.h>

/**
 * Constructor.
 * \param netFiles filenames for FANN nets.
 * \param numNets number of FANN nets.
 */
QSSPredict::QSSPredict(char **netFiles, int numNets)
{
  anns = (struct fann **) malloc(numNets*sizeof(struct fann *));
  numAnns = numNets;
  
  for (int i = 0; i < numAnns; i++) {
    anns[i] = fann_create_from_file(netFiles[i]);
	if (anns[i] == 0) {
	  throw PsodaError("\nERROR: Could not find FANN file!\n\n");
	}
  }
}

/** Destructor
 */
QSSPredict::~QSSPredict()
{
  for (int i = 0; i < numAnns; i++) {
    fann_destroy(anns[i]);
  }
}


int QSSPredict::seq2structure(const char* seq, char* structure_out) {
  int seqlen = strlen(seq);
  
  structure_out[seqlen] = '\0';
  int window_size = 13; //13 amino acids wide
  if ( ! window_size % 2) {
    cout << "Warning: window size not odd - Output may not be off a tad..." << endl;
  }
  int window_end_buffer_size = window_size/2; //truncate, eg. 13 -> 6
  //if (DEBUG)
  //{
  //  cout << "size of fann_type: " << sizeof(fann_type) << "  seqlength: " << seqlen << "  window_end_buffer_size: " << window_end_buffer_size << endl; 
  //}
  
  // each amino acid maps to 20 '0' or '1' characters for the neural network input.
  fann_type* NNinput;
  NNinput = (fann_type*) malloc( sizeof(fann_type) * (20*(seqlen + 2*window_end_buffer_size)));

  // Fill head and tail of input with 0's to pad first and last 6 amino acids 
  // of each sequence
  int i, j;
  for ( i=0; i<window_end_buffer_size; i++)
  {
    for ( j=0; j<20; j++)
	{
	  NNinput[20*i+j] = 0;
	  NNinput[20*(seqlen+window_end_buffer_size+i)+j] = 0;
	}
  }

  // Used to map the binary 0/1 inputs for each amino acid
  char AAs[] = {'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y'};
  char structures[] = {'H', 'E', 'l'};

  //  Convert the AA sequence to neural network input
  for ( i=0; i<seqlen; i++)
  {
    for ( j=0; j<20; j++)
	{
	  if (AAs[j] == toupper(seq[i]))
	  {
	    NNinput[20 * (i + window_end_buffer_size) + j] = 1;
		//if (DEBUG)
		  //cout << "1 ";
	  }
	  else
	  {
	    NNinput[20 * (i + window_end_buffer_size) + j] = 0;
		//if (DEBUG)
		  //cout << "0 ";
	  }
	}
  }
    
  //With the input now set up, plug it into the neural network
  fann_type *calc_outs[numAnns];
  fann_type *calc_out_composite;

  // Don't bother allocating 260 new inputs for each residue - 240 of them are 
  // common to the following residue. Instead, pass in a pointer at the head 
  // of the next 260 inputs pertinent to the current residue.
  for (i=0; i<seqlen; i++)
  {
    int j;
    // Use all .net networks for finding the structure
    for (j=0; j<numAnns; j++)
	{
      calc_outs[j] = fann_run(anns[j], &(NNinput[i*20]));
    }
	//Initialize calc_out_composite to a fann_type* of the right length
	//(We assume we have at least one ann, starting at 0)
    calc_out_composite = fann_run(anns[0], &(NNinput[i*20]));
    for (j=1; j<numAnns; j++)
	{
	  //if (DEBUG)
        //cout << endl << " " << j << ": " << calc_outs[j][0] << " " << calc_outs[j][1] << " " << calc_outs[j][2] << "   " << endl;
      
      //Hard coded for 3 structure types
      calc_out_composite[0] += calc_outs[j][0];
      calc_out_composite[1] += calc_outs[j][1];
      calc_out_composite[2] += calc_outs[j][2];
    }
	//if (DEBUG)
      //cout << endl << " Composite: " << calc_out_composite[0] << " " << calc_out_composite[1] << " " << calc_out_composite[2] << "   " << endl;

	//calc_out_composite points to an array of three output values, floats between 
	//0 and 1 representing the weight for each structure type. Whichever is 
    //greatest is the structure we output.

    // get the structure by finding the maximum output:
	if (calc_out_composite[0] < calc_out_composite[1])
	{
	  if (calc_out_composite[1] < calc_out_composite[2])
	  { //calc_out[2] is greatest
	    structure_out[i] = structures[2];
	  }
	  else //calc_out[1] >= both others
	    structure_out[i] = structures[1];
	}
	else if (calc_out_composite[0] > calc_out_composite[2])
	  structure_out[i] = structures[0]; //calc_out[0] is greatest
	else //calc_out[2] >= both others
	  structure_out[i] = structures[2];

	/*if (DEBUG)
	{
      int k;
	  for (k=0; k<20*window_size; k++)
	  {
        if ( ! (k % 20)) //New line for each amino acid
		  cout << endl;
	    cout << (int) (NNinput[i*20+k]) << " "; 
      }

	  //TODO: malloc this instead. It's currently hard-coded for a window_size of 13
	  //char tmp[] = "-------------"; 
      char* tmp = (char *)malloc(sizeof(char) * (seqlen+1));

      //copy the window that's being analyzed from the seq*/
      /*int tmppos=0, j;
	  for (j=0; j < window_size; j++)
	  {
	    int pos = i - window_end_buffer_size + j;
		if (pos >= 0 && pos < seqlen)
		{
		  tmp[tmppos] = seq[pos];
	      tmppos++;
          tmp[tmppos] = '\0';
		}
	  }

      printf("\n test (%s) on %c -> %f %f %f -> %c ", tmp, seq[i], calc_out_composite[0], calc_out_composite[1], calc_out_composite[2], structure_out[i]); fflush(stdout);*/
      
	  //free(tmp);
	//}
  }
  
  return 0;
}

/**
 * The actual prediction method.
 * Write predicted datasets to \a predictedSSDataset.
 * \param predictedSSDataset pointer to resulting (predicted) SS dataset.
 */
void QSSPredict::sspredict(SSDataset** predictedSSDataset)
{

  mDataset = Interpreter::getInstance()->dataset();
  mPrintBuffer = PsodaPrinter::getInstance();

  if( mDataset->ntaxa() <= 1){
    char str[1024];
    sprintf( str, "ERROR: Can not perform prediction with %d taxa!", mDataset->ntaxa());
    throw PsodaError(str);
  }
  
  Timer timer;

  timer.start();
  fprintf(stderr, "%s", "Prediction: ");
  
  if( (*predictedSSDataset) != NULL){
    delete (*predictedSSDataset);
  }
  (*predictedSSDataset) = new SSDataset();
  (*predictedSSDataset)->datatype() = Dataset::SECONDARY_STRUCTURE_3_STATE_DATATYPE;
  (*predictedSSDataset)->dataformat() = mDataset->dataformat();
  //(*predictedSSDataset)->setGapChar( mDataset->gapchar());
  //(*predictedSSDataset)->gapvalue() = mSSDataset->gapvalue();
  //(*predictedSSDataset)->missingchar() = mDataset->missingchar();
  //(*predictedSSDataset)->matchchar() = mDataset->matchchar();
  (*predictedSSDataset)->setntaxa( mDataset->ntaxa());
  (*predictedSSDataset)->setnchars( mDataset->nchars());
  char* ss_out = (char*)malloc(sizeof(char) * (mDataset->nchars()));
  
  for( int i = 0; i < mDataset->ntaxa(); i++ ){
    (*predictedSSDataset)->addName( mDataset->getTaxonName(i));
	seq2structure(mDataset->getCharacters(i,false), ss_out);
	//mPrintBuffer->write("Sequence = %s\n", mDataset->getPrintableCharacters(i));
	//mPrintBuffer->write("Structure = %s\n", ss_out);
	(*predictedSSDataset)->addCharacters(ss_out);
  }
  //(*predictedSSDataset)->printSeqs("fasta", "stdout");

  if( PSODA_DEBUG > 1) {
    fprintf( stderr, "mDataset:\n");
    mDataset->printSeqs("PHYLIP", "stderr");
    fprintf( stderr, "predictedSSDataset:\n");
    (*predictedSSDataset)->printSeqs("PHYLIP", "stderr");
  }
    //(*alignedDataset)->prologue();
	
  fprintf(stderr, "%s", "\n");
  TimerSecondMicros sm3 = timer.getCurrentSecondMicros();
  mPrintBuffer->write("Prediction took %li.%li seconds\n" , sm3.seconds , sm3.micros );
  
  if( (*predictedSSDataset)->matches( mDataset) == false){
    throw PsodaError( "\nERROR: Predicted secondary structure data set does not match sequences data set (detected in QSSPredict())!\n\n");
  }
  //mPrintBuffer->write("problem not here\n");
  /* mandate that secondary structures are in 3-state form */
  (*predictedSSDataset)->convertTo3State();
}
