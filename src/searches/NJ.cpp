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
#include "NJ.h"
#include "QNode.h"
#include "QNodeInfo.h"
#include "NewickTreeParser.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <float.h>
#include "Interpreter.h"
#include "Dataset.h"
#include "Globals.h"

NJSearch::NJSearch()
  : distMatrix(NULL),
    tempMatrix(NULL),
    treeString(NULL),
    size(0),
    distMatrixSet(false)
{}

const char* NJSearch::name() const
{
	return "Neighbor Joining Search";
}

NJSearch::NJSearch(int ntaxa, double * dm[])
  : distMatrix(NULL),
    tempMatrix(NULL),
    treeString(NULL),
    size(0),
    distMatrixSet(false)
{
    init(ntaxa);
    for (int i = 0; i < ntaxa; i++)
    {
        for (int j = 0; j < ntaxa; j++)
        {
            distMatrix[i][j] = dm[i][j];
        }
    }
    distMatrixSet = true;

}

NJSearch::~NJSearch()
{

  for (int i = 0; i < size; ++i)
  {
    delete [] distMatrix[i];
  }
  
  delete [] distMatrix;

}

#define MAX_TAXON_NAME_LEN 256
void NJSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm __attribute__ ((unused)),  EvaluatorBase * /*evaluator*/, int nreps __attribute__ ((unused)))
{
    Dataset* dataset = Interpreter::getInstance()->dataset();
    //bool done = false;
    int ntaxa = dataset->ntaxa();
    //int nchars = dataset->nchars();
    int loci = 1;
	int locj = 0;
    double minval;

    //fprintf(stderr, "In NJSearch\n");

    if (distMatrixSet == false){
        calculateDistMatrix();
    }

    if( PSODA_DEBUG >= 9){
      printMatrix();
    }

    // Copy the distance matrix into a working copy
    // allocate distMatrix memory for two dimensions
    // Need to convert from basic distances to neighbor joining distances
    tempMatrix = new double*[ntaxa];
    treeString = new char*[ntaxa];
    for (int i = 0; i < ntaxa; ++i)
    {
      tempMatrix[i] = new double[ntaxa];
      treeString[i] = new char[MAX_TAXON_NAME_LEN];
      sprintf(treeString[i], "%d", i+1);
      for (int j = 0; j < ntaxa; ++j)
      {
        tempMatrix[i][j] = distMatrix[i][j];
      }
    }

    bool useTaxa[ntaxa];
    for (int q = 0; q < ntaxa; q++) useTaxa[q] = true;

    double uValue[ntaxa];
    for (int q = 0; q < ntaxa-2; q++) // Doesn't include special case when there are only two nodes left
    {
      // Compute u-value for each tip
      for (int r = 0; r < ntaxa; r++)
	{
	  // Check if row has been wiped out
	  if (!useTaxa[r]) continue;
	  uValue[r] = 0;
	  for (int s = 0; s < ntaxa; s++)
	    {
	      if (!useTaxa[s])
		continue;
	      uValue[r] += tempMatrix[r][s];
	    }
	  uValue[r] /= (ntaxa - q - 2); // n = ntaxa - q
	}

      // Search the new distance matrix for the smallest Q-value (distance_ij - u_i - u_j)
      loci = 0;
      locj = 0;
      minval = (double)INT_MAX;
      for (int i = 1; i < ntaxa; i++) // Need to check all values
        {
	  // Check if row has been wiped out
	  if (!useTaxa[i])
	    continue;

            for (int j = 0; j < i; j++)
            {
	      // Check if column has been wiped out
	      if (!useTaxa[j])
		continue;

                if (tempMatrix[i][j] - uValue[i] - uValue[j] < minval)
                {
                    loci = i;
                    locj = j;
                    minval = tempMatrix[i][j] - uValue[i] - uValue[j];
                }
            }
        }
        //fprintf(stderr,"\n\n---Min is ([%d]%s, [%d]%s)\n", loci+1, treeString[loci], locj+1, treeString[locj]);
        // Join the two nodes and place the combination back in the list
        treeString[loci] = strallocat(&(treeString[loci]), &(treeString[locj]));

        // Now change the matrix appropriately
	double ijDist = tempMatrix[loci][locj];
        for (int i = 0; i < ntaxa; i++)
        {
              // Change the i distances to a function of the i and j distances
	      tempMatrix[i][loci] = (tempMatrix[i][loci] + tempMatrix[i][locj] - ijDist)/2;
	      tempMatrix[loci][i] = (tempMatrix[loci][i] + tempMatrix[locj][i] - ijDist)/2;

              // wipe out the j distances in the matrix
	      useTaxa[locj] = false;
        }
	tempMatrix[loci][loci] = 0; // So we don't have to worry about this in the u-value computations
        //printMatrixAndTree();
    }
    locj = -1;
    loci = -1;
    for (int r = 0; r < ntaxa; r++)
      {
	if (useTaxa[r])
	  {
	    if (locj == -1)
	      locj = r;
	    else
	      {
		loci = r;
		break;
	      }
	  }
      }
    // throw error if (locj == -1 || loci == -1)
    treeString[loci] = strallocat(&(treeString[loci]), &(treeString[locj]));
    //fprintf(stderr,"Neighbor-Join tree is --> %s\n", treeString[loci]);
    NewickTreeParser ntp(&qtreeRepository);
    ntp.parseBuffer(treeString[loci], strlen(treeString[loci]));

    //char *stree = "((5,4),(3,2), 1)";
    //char *stree = "((5,4),((3,2), 1))";
    //ntp->parseBuffer(stree, strlen(stree));
    /**/
}

char *NJSearch::strallocat(char **s, char **t)
{
    int len = strlen(*s) + strlen(*t) + 5;  // Add 5 characters for the (, ) and the null
    char *result = new char[len];
    sprintf(result, "(%s, %s)", *s, *t);
    delete[] *s;
    delete[] *t;
    *s = *t = NULL;
    return result;
}

void NJSearch::calculateDistMatrix()
{
  Dataset* dataset = Interpreter::getInstance()->dataset();
  int ntaxa = dataset->ntaxa();
  //int nchars = dataset->nchars();
  int *lengths = new int[ntaxa];
  const char **data = new const char*[ntaxa];

  //cerr << "calculateDistMatrix()\n";
  init(ntaxa);

  for (int i = 0; i < ntaxa; i++)
  {
      lengths[i] = strlen(dataset->getCharacters(i,false));
      data[i] = dataset->getCharacters(i,false);
  }

  for (int i = 0; i < ntaxa; ++i)
  {
    for (int j = i; j < ntaxa; ++j)
    {
      int kmax = min(lengths[i], lengths[j]);
      for (int k = 0; k < kmax; ++k)
      {
        if (data[i][k] != data[j][k])
        {
          distMatrix[i][j]++;          
        }
      }
      distMatrix[i][j] += abs(lengths[i] - lengths[j]);
      distMatrix[j][i] = distMatrix[i][j];
      checkPausePSODA();
    }
  }
  delete[] lengths;
  delete[] data;
}

void NJSearch::init (int ntaxa)
{
  size = ntaxa;

  // allocate distMatrix memory for two dimensions
  distMatrix = new double*[size];
  for (int i = 0; i < size; ++i)
  {
    distMatrix[i] = new double[size];
    for (int j = 0; j < size; ++j)
    {
      distMatrix[i][j] = 0;
    }
  }
}

void NJSearch::printMatrix()
{
  printf("DistSearch distance Matrix:\n");
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
        printf("%15.2f", distMatrix[i][j]);
    }
    printf("\n");
  }

}

void NJSearch::printMatrixAndTree()
{
    printf("\nDistSearch distance Matrix with Tree\n");
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
        printf("%15.2f", tempMatrix[i][j]);
    }
    if (treeString[i] != NULL)
        printf("\t%s\n", treeString[i]);
    else
        printf("\tNULL\n");
  }

}

