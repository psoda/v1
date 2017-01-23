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
#include "QTreeRepository.h"
#include "QTree.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include <float.h>

extern double PSODA_maxmemory; // The max java vm memory or OS memory we can use for trees

/**
 * The tree repository is a doubly linked list
 */
QTreeRepository::QTreeRepository()
{
  mMaxTrees = 1;
	mFilterIsomorphic = true;
}

/**
 * Turn off filtering
 */
QTreeRepository::QTreeRepository(bool filterIsomorphic)
{
  mMaxTrees = 1;
	mFilterIsomorphic = filterIsomorphic;
}

/**
 * Go through the linked list and delete each entry
 */
QTreeRepository::~QTreeRepository()
{
  removeAll();
}

// This is kind of complicated
// We want to keep all of the best trees in the repository, but 
//  dont want duplicates.
// When a new tree is added, see if there is already an isomorphic
//  tree there.
// Generate the canonical tree string for the trees and compare 
//  strings.
// If the tree has already been completely searched, dont replace
//  it with a copy.  If a completely searched tree is being put in, 
//  replace the unsearched copy.
//
// Once a tree is added it belongs to the repository, the reference in the caller is nuked
void QTreeRepository::addTree(QTree*& callersTree)
{
	//Enforce the rule that the tree belongs to the repository after addTree
	QTree* newTree = callersTree;
	callersTree = NULL;
    // Did we replace a duplicate tree in the queue with this new tree
    bool replaced = false;

    // If we are over the limit, throw the new one away without doing anything else
    if (mList.size() >= maxTrees())
    {
        // If the new tree is not visited, delete the new tree and return
        if(!newTree->visited())
        {
            delete newTree;
            return;
        }
    }
		// If we arent filtering isomorphisms, just put it in and return
		if(mFilterIsomorphic == false) {
			mList.push_back(newTree);
			return;
		}

    // Look at trees in the repository, only add this one if it is new
    newTree->canonicalize();
    newTree->updateTreeString();

#ifdef debugTreeRepository
    printf("addTree: after update \n%s\n", newTree->treeStr());
#endif

    deque <QTree *>::iterator treeIt = mList.begin(); // Iterator to use in acccessing list
    QTree *thisTree = 0;
    for(unsigned int ind = 0; ind < mList.size(); ind++)
    {
        thisTree = mList[ind];

#ifdef debugTreeRepository
        printf("addTree: list \n%s\n",  newTree->treeStr());
#endif

        // If this tree is the same as one already in the repository, delete it
        if(strcmp(thisTree->treeStr(),newTree->treeStr()) == 0) 
        {
            // If this is visited, keep it and throw the duplicate in the repository away
            if(newTree->visited())
            {
                replaced = true;
                delete(thisTree);
                mList[ind] = newTree;
                return;
            }
            else
            {
                delete newTree;
                return;
            }
        } 
    }
    // If there still isnt room and the new tree is visited and it didnt replace a duplicate, find a non-visited one to delete
    if ((mList.size() >= maxTrees()) && newTree->visited() && !replaced)
    {
        // Delete an unvisited tree to make room for this searched one
				deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list
				for(treeIt = mList.begin(); treeIt != mList.end(); treeIt++)
				{
						if(!(*treeIt)->visited()) 
						{
								delete(*treeIt);
								mList.erase(treeIt);
								break;
						}
				}
    }
        //printf("addTree: over limit size %d, max %d\n",mList.size(),maxTrees());
#ifdef notdef
    printf("QTreeRepository::addtree: adding %s\n", newTree->treeStr());
#endif
    // Apend the new tree at the end
    mList.push_back(newTree);
}

/**
 * Get a unvisited tree from the repository.  Remove it and return it.
 * Return NULL if you cant find one.
 */
QTree* QTreeRepository::getSearchableTree(bool print)
{
    deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list
    QTree *treeptr;
    int curtree = 0;

    for(treeIt = mList.begin(); treeIt != mList.end(); treeIt++)
    {
        //        printf("getBestTree: tree was %f\n",(*treeIt)->score());
        if(!(*treeIt)->visited()) 
        {
            treeptr = *treeIt;
            mList.erase(treeIt);
            return (treeptr);
        }
    }
    if(print)
		PsodaPrinter::getInstance()->write("No Searchable Trees Found in repository(%d)\n",(int)mList.size());
      return NULL;
}
/**
 * Get a tree from the repository.  Leave it in place.
 * Return NULL if you cant find one.
 */
QTree* QTreeRepository::getTree()
{
  if (mList.size() < 1) {
    return NULL;
  } else {
    return(mList.front());
  }
}

/**
 * Remove a tree from the repository.  
 * The caller needs to delete it if it is not added back
 */
QTree* QTreeRepository::popTree()
{
  QTree *retTree;
  if(!mList.empty()) {
    // Remove from the front
    retTree = mList.front();
    mList.pop_front();
    return(retTree);
  } else {
    //printf("Pop from repository when no trees are present\n");
    return(NULL);
  }
}

void  QTreeRepository::setMaxTrees(int maxTreeVal, int ntaxa) {
// This is kind of sloppy, but we have found that 100MB can fit about 5513 trees with 500 taxa each
// So, each tree is about 100MB/5513 = 18,000B
// if you write the TreeSize=K*nnodes, then K= 18,000/512 = 36B/taxa
// To calculate the max number of trees ntrees = maxmem/treesize = maxmem/(36*ntaxa)
// For 100MB, the max trees is 100M/(36*500) = 5555 which is about right.  
// Now we want to give ourselves some slack, so we will only allow you to use 75% of these
#define MAXSLACK (0.75)
#define SIZEPERTAXA (36)

	mMaxTrees = MAXSLACK * (PSODA_maxmemory /(SIZEPERTAXA*ntaxa)) ; // The max java vm memory or OS memory we can use for trees
  if(maxTreeVal < mMaxTrees) {
		mMaxTrees = maxTreeVal;
  }
}

unsigned int  QTreeRepository::maxTrees() const {
  return mMaxTrees;
}

const deque<QTree *>* QTreeRepository::getTrees() {
  return &mList;
}

// The number of trees in the tree repository
int QTreeRepository::numTrees() const
{
  return(mList.size());
}
// The number of trees in the tree repository that still need to be swapped
int QTreeRepository::numTreesToSwap()
{
  int leftToSwap = 0;
  deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list

  for(treeIt = mList.begin(); treeIt != mList.end(); treeIt++)
  {
      if(!(*treeIt)->visited())
          leftToSwap++;
  }
  return(leftToSwap);
}

void QTreeRepository::removeAll()
{
  deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list

  for(treeIt = mList.begin(); treeIt != mList.end(); treeIt++)
    {
      delete *treeIt;
    }

  mList.clear();
}


void QTreeRepository::print()
{
  deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list

  for(treeIt = mList.begin(); treeIt != mList.end(); treeIt++)
  {
        printf("Tree score %f visited %d\n",(*treeIt)->getScore(),(*treeIt)->visited());
	//        (*treeIt)->print();
  }
}

void QTreeRepository::print(std::ostream &output)
{
//if(format == NEXUS_TREE_FORMAT){
  output << "#NEXUS\nbegin trees;" << endl;
  output << "Translate" << endl;
 
  int taxonNamesSize = Interpreter::getInstance()->dataset()->ntaxa();
  for ( int i = 0; i < taxonNamesSize; i++ )
    {
      output << i + 1 << " " << Interpreter::getInstance()->dataset()->getTaxonName(i); 
      if ( i != taxonNamesSize - 1 )
	{
	  output << "," << endl; 
	}
      output<<endl;
    }
  
  output << ";" << endl;
//}

  deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list
  int i=1;

  for(treeIt = mList.begin(); treeIt != mList.end(); treeIt++)
    {
	//if(format == NEXUS_TREE_FORMAT){
	        output << "tree treeName"<< i++ << " = [&U] ";
	//}
        output << (*treeIt)->treeStr();
        output << ";" << endl; 
    }
   //if(format == NEXUS_TREE_FORMAT){
	output << "end;" << endl;
   //}
}

void QTreeRepository::printNewick(std::ostream &output){
	//int taxonNamesSize = taxonNames.size();
	
	deque <QTree *>::iterator treeIt; // Iterator to use in accessing tree list
	for(treeIt = mList.begin(); treeIt != mList.end(); treeIt++){
		const char* treeString = (*treeIt)->treeStr();

		//cout << treeString << endl;
		//cout << "len: " << strlen(treeString) << endl;
		
		for(unsigned int i = 0; i < strlen(treeString); i++){
			if(isdigit((char)treeString[i])){
				char* taxaNumString;
				char tmpString[5];
				int j = 0; 
				while(isdigit((char)treeString[i])){
					tmpString[j++] = treeString[i++];
				}
				i--;  // setting i back in order not to miss a character
				tmpString[j] = '\0';
				taxaNumString = tmpString;
				int taxaNum = atoi(taxaNumString);
				output << Interpreter::getInstance()->dataset()->getTaxonName(taxaNum - 1);
			}
			else{
				output << treeString[i];
			}
		}
		output << ";" << endl;
		//cout << ";" << endl;
	}
}

/**
 *rescoreTrees assumes that somethings (such as character weights) has changed how trees should be scored
 *all trees are rescored using evaluator with now suboptimal trees being removed, it returns the new best score
 */
double QTreeRepository::rescoreTrees(EvaluatorBase *evaluator)
{
  double bestScore = DBL_MAX;
  double curScore;
  deque<QTree*> tempDeque;
  deque<QTree*>::iterator treeItr = mList.begin();
  deque<QTree*>::iterator treeEnd = mList.end();
  for (; treeItr != treeEnd; treeItr++) 
  {
	curScore = evaluator->qscoreTree(*treeItr);
	//printf("New Score %.0f\n",curScore);
	if(curScore < bestScore)
	{
		//printf("\tNew Best Score\n");
		//clear out tempDeque
		for(deque<QTree*>::iterator clearItr = tempDeque.begin();clearItr!=tempDeque.end();clearItr++)
			delete *clearItr;
		tempDeque.clear();
		bestScore = curScore;
	}
	if(curScore <= bestScore)
	{
		(*treeItr)->setScore(curScore);
		tempDeque.push_back(new QTree(*treeItr));
	}
	delete *treeItr;
  }
  mList.clear();
  deque<QTree*>::iterator tempItr = tempDeque.begin();
  deque<QTree*>::iterator tempEnd = tempDeque.end();
  for (; tempItr != tempEnd; tempItr++) {
    (*tempItr)->updateTreeString();
    mList.push_back(*tempItr);
  }
  
  return bestScore;
}

void QTreeRepository::clearVisited() {
  deque<QTree*> tempDeque;
  deque<QTree*>::iterator treeItr = mList.begin();
  deque<QTree*>::iterator treeEnd = mList.end();
  for (; treeItr != treeEnd; treeItr++) {
    tempDeque.push_back(new QTree(*treeItr));
    //    tempDeque.push_back(*treeItr);
    delete *treeItr;
  }
  mList.clear();
  //  tempDeque.clear();
  deque<QTree*>::iterator tempItr = tempDeque.begin();
  deque<QTree*>::iterator tempEnd = tempDeque.end();
  for (; tempItr != tempEnd; tempItr++) {
    (*tempItr)->updateTreeString();
    mList.push_back(*tempItr);
  }
}

void QTreeRepository::setEarlyPrune(int newEarlyPruneScore) {
  deque<QTree*>::iterator treeItr = mList.begin();
  deque<QTree*>::iterator treeEnd = mList.end();
  for (; treeItr != treeEnd; treeItr++) {
    (*treeItr)->earlyPrune() = newEarlyPruneScore;
  }
}

void QTreeRepository::randomMorph() {
  deque<QTree*>::iterator treeItr = mList.begin();
  deque<QTree*>::iterator treeEnd = mList.end();
  for (; treeItr != treeEnd; treeItr++) {
    (*treeItr)->randomMorph();
  }
  clearVisited();
}
