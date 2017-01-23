#include <string>
#include "floatfann.h"
#include "fann.h"

// Debug output
#define DEBUG 0

//Designed for use with multiple (i.e. 10) networks
#define NUM_NETS 10
struct fann *anns[NUM_NETS]; //array of pointers to fann objects...

// Must be called before you can convert seqs
struct fann* init_ann_from_file(string filename)
{
  struct fann *ann_temp;
  ann_temp = create_from_file(filename);
  // TODO: Error check, make sure the file was successfully loaded
  if (DEBUG)
  {
    cout << endl << "Loading net from "<< filename << " ..." << endl;
    print_connections(ann_temp);
    print_parameters(ann_temp);
  }
  return ann_temp;
}

//Use the provided .net file - keep it in the same dir as the executable.
//Run destroy_anns() when you're done.
int init_anns()
{
  anns[0] = init_ann_from_file("./nets/casp7_shuffled.fann0.net");
  anns[1] = init_ann_from_file("./nets/casp7_shuffled.fann1.net");
  anns[2] = init_ann_from_file("./nets/casp7_shuffled.fann2.net");
  anns[3] = init_ann_from_file("./nets/casp7_shuffled.fann3.net");
  anns[4] = init_ann_from_file("./nets/casp7_shuffled.fann4.net");
  anns[5] = init_ann_from_file("./nets/casp7_shuffled.fann5.net");
  anns[6] = init_ann_from_file("./nets/casp7_shuffled.fann6.net");
  anns[7] = init_ann_from_file("./nets/casp7_shuffled.fann7.net");
  anns[8] = init_ann_from_file("./nets/casp7_shuffled.fann8.net");
  anns[9] = init_ann_from_file("./nets/casp7_shuffled.fann9.net");

  return 0;
}

//Call this when you're done to free up memory
int destroy_anns()
{
  int i;
  for(i=0; i<NUM_NETS; i++)
  {
    destroy(anns[i]);
  }
}


// Generates a string of predicted secondary structure elements for the input 
// seq and writes it into structure_out
// 
// Returns a string instead of a char* so the calling function doesn't have to
// worry about delete-ing it
// 
// ASSUMPTIONS:
//  - structure_out points to a buffer at least as long as seq, plus 1 for '\0'
//  - init_ann_from_file() (or init_ann()) has already been called
// 
string seq2structure(string seq)
{
  
  //Check at least the first ann... With the current implementation,
  // if the first one isn't initialized, none of them would be.
  if (anns[0] == 0) 
  {
    cout << "Error - Attempting to call seq2structure() without first initializing network with init_ann_from_file()." << endl;
	return -1;
  }

  int seqlen = seq.length();
  char* structure_out = (char*)malloc( sizeof(char) * (seqlen + 1));
  structure_out[seqlen] = '\0';
  int window_size = 13; //13 amino acids wide
  if ( ! window_size % 2)
  {
    cout << "Warning: window size not odd - Output may not be off a tad..." << endl;
  }
  int window_end_buffer_size = window_size/2; //truncate, eg. 13 -> 6
  if (DEBUG)
  {
    cout << "size of fann_type: " << sizeof(fann_type) << "  seqlength: " << seqlen << "  window_end_buffer_size: " << window_end_buffer_size << endl; 
  }
  
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
		if (DEBUG)
		  cout << "1 ";
	  }
	  else
	  {
	    NNinput[20 * (i + window_end_buffer_size) + j] = 0;
		if (DEBUG)
		  cout << "0 ";
	  }
	}
  }
  
    
  //With the input now set up, plug it into the neural network
  fann_type *calc_outs[NUM_NETS];
  fann_type *calc_out_composite;

  // Don't bother allocating 260 new inputs for each residue - 240 of them are 
  // common to the following residue. Instead, pass in a pointer at the head 
  // of the next 260 inputs pertinent to the current residue.
  for (i=0; i<seqlen; i++)
  {
    int j;
    // Use all .net networks for finding the structure
    for (j=0; j<NUM_NETS; j++)
	{
      calc_outs[j] = run(anns[j], &(NNinput[i*20]));
    }
	//Initialize calc_out_composite to a fann_type* of the right length
	//(We assume we have at least one ann, starting at 0)
    calc_out_composite = run(anns[0], &(NNinput[i*20]));
    for (j=1; j<NUM_NETS; j++)
	{
	  if (DEBUG)
        cout << endl << " " << j << ": " << calc_outs[j][0] << " " << calc_outs[j][1] << " " << calc_outs[j][2] << "   " << endl;
      
      //Hard coded for 3 structure types
      calc_out_composite[0] += calc_outs[j][0];
      calc_out_composite[1] += calc_outs[j][1];
      calc_out_composite[2] += calc_outs[j][2];
    }
	if (DEBUG)
      cout << endl << " Composite: " << calc_out_composite[0] << " " << calc_out_composite[1] << " " << calc_out_composite[2] << "   " << endl;

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

	if (DEBUG)
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
      char* tmp = malloc(sizeof(char) * (seqlen+1));

      //copy the window that's being analyzed from the seq
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
	}
  }

  free(NNinput);
  
  return string(structure_out);

}

/* Example usage (uncomment main() and compile as executable)

int main()
{
  init_anns();
  char seq1[] = "GSRAQSSPAAPASLSAPEPASQARVLSSSETPARTLPFTTGLIYDSVMLKHQCSCGDNSRHPEHAGRIQSIWSRLQERGLRSQCECLRGRKASLEELQSVHSERHVLLYGTNPLSRLKLDNGKLAGLLAQRMFVMLPCGGVGVDTDTIWNELHSSNAARWAAGSVTDLAFKVASRELKNGFAVVRPPGHHADHSTAMGFCFFNSVAIACRQLQQQSKASKILIVDWDVHHGNGTQQTFYQDPSVLYISLHRHDDGNFFPGSGAVDEVGAGSGEGFNVNVAWAGGLDPPMGDPEYLAAFRIVVMPIAREFSPDLVLVSAGFDAAEGHPAPLGGYHVSAKCFGYMTQQLMNLAGGAVVLALEGGHDLTAICDASEACVAALLGNRVDPLSEEGWKQKPNLNAIRSLEAVIRVHSKYWGCMQRLAS";
  char seq2[] = "QAKHKQRKRLKSSCKRHPLYVDFSDVGWNDWIVAPPGYHAFYCHGECPFPLADHLNSTNHAIVQTLVNSVNSKIPKACCVPTELSAISMLYLDENEKVVLKNYQDMVVEGCGCR";
  char seq3[] = "KHQCSCGDNSRHPEHAGRIQSIWSRLQERGLRSQCECLRGRKASLEELQSVHSERHVLLYGTNPLSRLKLDNGKLAGLLAQRMFVMLPCGGVGVDTDTIWNELHSSNAARWAAGSVTDLAFKVASRELKNGFAVVRPPGHHADHSTAMGFCFFNSVAIACRQLQQQSKASKILIVDWDVHHGNGTQQTFYQDPSVLYISLHRHDDGNFFPGSGAVDEVGAGSGEGFNVNVAW";
  char str[500];
  seq2structure(seq1, str);
  printf("\nseq:    %s\nstruct: %s\n", seq1, str);
  seq2structure(seq2, str);
  printf("\nseq:    %s\nstruct: %s\n", seq2, str);
  seq2structure(seq3, str);
  printf("\nseq:    %s\nstruct: %s\n", seq3, str);
  fflush(stdout);
  destroy_anns();
  return 0;
}

*/

//For simple command line functionality - pass the seq in on the command line, and this outputs only the secondary structure sequence prediction.
int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "Error, expecting >= 2 argument (i.e. sequence string or something).  Exiting.." << endl;
	fflush(stdout);
  }
   
  init_anns();
  char *seq_in = string(argv[1]);
  char *str_out = malloc (sizeof(char) * (strlen(seq_in) + 1));
  string structure = seq2structure(seq_in, str_out);
  cout << structure << endl;
  destroy_anns();
  return 0;
}