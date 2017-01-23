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
#include "Dist.h"
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

DistSearch::DistSearch()
  : distMatrix(NULL),
    tempMatrix(NULL),
    treeString(NULL),
    size(0),
    distMatrixSet(false)
{}

const char* DistSearch::name() const
{
	return "Distance Search";
}

DistSearch::DistSearch(int ntaxa, double * dm[])
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

DistSearch::~DistSearch()
{

  for (int i = 0; i < size; ++i)
  {
    delete [] distMatrix[i];
  }
  
  delete [] distMatrix;

}

#define MAX_TAXON_NAME_LEN 256
void DistSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm __attribute__ ((unused)),  EvaluatorBase * /*evaluator*/, int nreps __attribute__ ((unused)))
{
    Dataset* dataset = Interpreter::getInstance()->dataset();
    //bool done = false;
    int ntaxa = dataset->ntaxa();
    //int nchars = dataset->nchars();
    int loci = 1;
	int locj = 0;
    double maxval;

    if (distMatrixSet == false){
        calculateDistMatrix();
    }

    if( PSODA_DEBUG >= 9){
      printMatrix();
    }

    // Copy the distance matrix into a working copy
    // allocate distMatrix memory for two dimensions
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


    for (int q = 0; q < ntaxa-1; q++)
    {
      checkPausePSODA();
      // Search the distance matrix for the largest distance
      loci = 1;
      locj = 0;
      maxval = tempMatrix[loci][locj];
        for (int i = 2; i < ntaxa; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (tempMatrix[i][j] > maxval)
                {
                    loci = i;
                    locj = j;
                    maxval = tempMatrix[i][j];
                }
            }
        }
        //fprintf(stderr,"\n\n---Min is ([%d]%s, [%d]%s)\n", loci+1, treeString[loci], locj+1, treeString[locj]);
        // Join the two nodes and place the combination back in the list
        treeString[loci] = strallocat(&(treeString[loci]), &(treeString[locj]));

        for (int i = 0; i < ntaxa; i++)
        {
              // Change the i distances to the average of the i and j distances
              tempMatrix[i][loci] = (tempMatrix[i][loci] + tempMatrix[i][locj])/2;
              tempMatrix[loci][i] = (tempMatrix[loci][i] + tempMatrix[locj][i])/2;

              // wipe out the j distances in the matrix
              tempMatrix[i][locj] = (double)INT_MIN;
              tempMatrix[locj][i] = (double)INT_MIN;
        }
        //printMatrixAndTree();
    }
    //fprintf(stderr,"UPGMA tree is --> %s\n", treeString[loci]);
    NewickTreeParser ntp(&qtreeRepository);
    QTree* result_tree = ntp.parseBuffer(treeString[loci], strlen(treeString[loci]),false);
	//printf("Before Add\n");
	//result_tree->print();
	qtreeRepository.addTree(result_tree);
	//printf("After Add\n");
	//result_tree->print();
    //char *stree = "((5,4),(3,2), 1)";
    //char *stree = "((5,4),((3,2), 1))";
    //ntp->parseBuffer(stree, strlen(stree));
    /**/
}

char *DistSearch::strallocat(char **s, char **t)
{
    int len = strlen(*s) + strlen(*t) + 5;  // Add 5 characters for the (, ) and the null
    char *result = new char[len];
    sprintf(result, "(%s, %s)", *s, *t);
    delete[] *s;
    delete[] *t;
    *s = *t = NULL;
    return result;
}

void DistSearch::calculateDistMatrix()
{
  Dataset* dataset = Interpreter::getInstance()->dataset();
  int ntaxa = dataset->ntaxa();
  //int nchars = dataset->nchars();
  int *weights = dataset->weights();
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
        if (weights[k] > 0 && data[i][k] != data[j][k])
        {
          distMatrix[i][j] += weights[k];          
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

void DistSearch::init (int ntaxa)
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

void DistSearch::printMatrix()
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

void DistSearch::printMatrixAndTree()
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

