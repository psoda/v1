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
 #include "SectorSearchBase.h"
 
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include "QStep.h"
#include "SectorStep.h"
#include "QTBR.h"

#include <sys/time.h>
#include <sys/types.h>
#include <float.h>
#include <math.h>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#define sleep(s) Sleep(s*1000)
#endif
#include "Globals.h"
#include "NewickTreeParser.h"
#include "CommonCladeRefinement.h"
#include "PartialTreeSearch.h"

void LabelSector(int curSector,int totalSectors)
{
	PsodaPrinter::getInstance()->write("--------------------------------------------------------------------------\n");
	PsodaPrinter::getInstance()->write("|                       Sector     %d     of      %d                       |\n",curSector,totalSectors);
	PsodaPrinter::getInstance()->write("--------------------------------------------------------------------------\n");
}

void LabelGlobal()
{
	PsodaPrinter::getInstance()->write("--------------------------------------------------------------------------\n");
	PsodaPrinter::getInstance()->write("|                           Global Swapping                              |\n");
	PsodaPrinter::getInstance()->write("--------------------------------------------------------------------------\n");
}

void LabelPerturbed()
{
	PsodaPrinter::getInstance()->write("--------------------------------------------------------------------------\n");
	PsodaPrinter::getInstance()->write("|            Global Swapping with Character Reweighting                  |\n");
	PsodaPrinter::getInstance()->write("--------------------------------------------------------------------------\n");
}


SectorSearchBase::SectorSearchBase():mStartTime(0.0),mUpdated(false),mStepwiseReplicates(5),mReplicates(20),mGlobalTBRLength(45),mRepoSize(1),mPTMStart(false),mBestScore(DBL_MAX),mBestTree(NULL)
{
}

SectorSearchBase::~SectorSearchBase()
{
}


unsigned int& SectorSearchBase::replicates()
{
	return mReplicates;
}

unsigned int SectorSearchBase::replicates() const
{
	return mReplicates;
}

int& SectorSearchBase::stepwise_replicates()
{
	return mStepwiseReplicates;
}
int SectorSearchBase::stepwise_replicates() const
{
	return mStepwiseReplicates;
}

int& SectorSearchBase::global_tbr_length()
{
	return mGlobalTBRLength;
}
int SectorSearchBase::global_tbr_length() const
{
	return mGlobalTBRLength;
}

int& SectorSearchBase::global_perturbed_tbr_length()
{
	return mGlobalPerturbedTBRLength;
}
int SectorSearchBase::global_perturbed_tbr_length() const
{
	return mGlobalPerturbedTBRLength;
}

int& SectorSearchBase::repo_size()
{
	return mRepoSize;
}
int SectorSearchBase::repo_size() const
{
	return mRepoSize;
}

bool SectorSearchBase::sector_keep_tree() const
{
	return mSectorKeepCur;
}

bool& SectorSearchBase::sector_keep_tree()
{
	return mSectorKeepCur;
}

bool SectorSearchBase::global_keep_tree() const
{
	return mGlobalKeepCur;
}
bool& SectorSearchBase::global_keep_tree()
{
	return mGlobalKeepCur;
}
bool SectorSearchBase::perturbed_keep_tree() const
{
	return mPerturbedKeepCur;
}
bool& SectorSearchBase::perturbed_keep_tree()
{
	return mPerturbedKeepCur;
}

bool SectorSearchBase::fast_exit_sector_search() const
{
	return mFastSectorExit;
}

bool& SectorSearchBase::fast_exit_sector_search()
{
	return mFastSectorExit;
}

bool SectorSearchBase::ptm_start() const
{
	return mPTMStart;
}

bool& SectorSearchBase::ptm_start()
{
	return mPTMStart;
}

int SectorSearchBase::ptm_min() const
{
	return mPTMMinSize;
}

int& SectorSearchBase::ptm_min()
{
	return mPTMMinSize;
}

int SectorSearchBase::ptm_max() const
{
	return mPTMMaxSize;
}
int& SectorSearchBase::ptm_max()
{
	return mPTMMaxSize;
}
 
void SectorSearchBase::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int iterations)
{
	QSectorStepwiseAdditionSearch stepwise;
	PartialTreeSearch ptm;
	int j;
	QTreeRepository* repo;
	QTreeRepository stepwise_ccr_repo;
	bool* repo_finished;
	
	repo = new QTreeRepository[mReplicates];
	repo_finished = new bool[mReplicates];
	
	ptm.setMinSize(mPTMMinSize);
	ptm.setMaxSize(mPTMMaxSize);
	
    int maxIterations;
	 if (iterations > 0)
    {
        maxIterations = iterations;
    }


    mStartTime = When();
	mUpdated = true;
	
	mPTMStart = false;
	if(mPTMStart)
	{
		//Start with a stepwise tree
		
		if(mRepoSize == 0) mRepoSize = 1;

		for(j=0;j<mReplicates;j++)
		{
			repo[j] = new QTreeRepository();
			repo[j].setMaxTrees(mRepoSize,Interpreter::getInstance()->dataset()->ntaxa());
			
			ptm.search(repo[j],qsearchAlgorithm,evaluator,mStepwiseReplicates);
				
			repo_finished[j] = false;
		}
	}
	else
	{
		//Start with a stepwise tree
		
		if(mRepoSize == 0) mRepoSize = 1;

		for(j=0;j<mReplicates;j++)
		{
			repo[j] = new QTreeRepository();
			repo[j].setMaxTrees(mRepoSize,Interpreter::getInstance()->dataset()->ntaxa());
			
			stepwise.search(repo[j],qsearchAlgorithm,evaluator,mStepwiseReplicates);
				
			repo_finished[j] = false;
		}
	}
	
	if(iterations!=0)
	{
	
	
	printf("Starting search =>  global %d, perturbed %d\n",mGlobalTBRLength,mGlobalPerturbedTBRLength);
	printf("Fast Sector Exit: ");
	if(mFastSectorExit) 
		printf("ON\n");
	else
		printf("OFF\n");
	
	printf("Keep Current in Sector: ");
	if(mSectorKeepCur) 
		printf("ON\n");
	else
		printf("OFF\n");
		
	printf("Keep Current in Global: ");
	if(mGlobalKeepCur) 
		printf("ON\n");
	else
		printf("OFF\n");
		
	printf("Keep Current in Perturbed: ");
	if(mPerturbedKeepCur) 
		printf("ON\n");
	else
		printf("OFF\n");
	

	
	for ( int i = 0 ; i < maxIterations && !gQuit; i ++ )
    {
		for(j=0;j<mReplicates;j++)
		{
			if(!repo_finished[j])
			{
				bool not_perturbed;
				bool perturbed;
				
				//Divide current tree into sectors and refine
				PartitionSearch(repo[j],qsearchAlgorithm,evaluator,mSectorKeepCur);
				
				//Perform Limited Global Swapping
				
				if(mGlobalTBRLength > 0)
				{
					LabelGlobal();
					not_perturbed = GlobalSearch(repo[j],qsearchAlgorithm,evaluator,mGlobalTBRLength,mGlobalKeepCur);
				}
				else
				{
					not_perturbed=true;
				}
				
				if(mGlobalPerturbedTBRLength > 0)
				{
					LabelPerturbed();
					//Reweight Characters
					Interpreter::getInstance()->dataset()->randomizeWeights(0.15);
					//Perform Limited Global Swapping
					
					perturbed = GlobalSearch(repo[j],qsearchAlgorithm,evaluator,mGlobalPerturbedTBRLength,mPerturbedKeepCur);
					//Reset Characters
					Interpreter::getInstance()->dataset()->resetWeights();
				}
				else
				{
					perturbed=true;
				}
				repo_finished[j] = not_perturbed&&perturbed; //if a full global search made no progress stop looking
				
				if(mGlobalPerturbedTBRLength > 0) //else there is no need to correct for perterbations
				{
					if(repo_finished[j])
					{
						GlobalSearch(repo[j],qsearchAlgorithm,evaluator,mGlobalPerturbedTBRLength); // this tree will not be looked at again, make sure that perturbation has not lead to a worse tree
					}
					else
					{
						GlobalSearch(repo[j],qsearchAlgorithm,evaluator,mGlobalPerturbedTBRLength/3);
					}
				}
				
			}
		}
	}

	gQuit = false;
	
	}
	
	//Fuse Replicates
	
	if (qtreeRepository.maxTrees() < mReplicates)
		qtreeRepository.setMaxTrees(mReplicates,Interpreter::getInstance()->dataset()->ntaxa());
	if (repo[0].maxTrees() < mReplicates)
		repo[0].setMaxTrees(mReplicates,Interpreter::getInstance()->dataset()->ntaxa());
	
	
	
	NewickTreeParser ntp;
	
	for(j=1;j<mReplicates;j++)
	{
		QTree* temp = repo[j].getTree();
		QTree* qt = ntp.parseBuffer(temp->treeStr(), strlen(temp->treeStr()), false);
		repo[0].addTree(qt);
	}
	
	PsodaPrinter::getInstance()->write("Fusing Results\n");
	double bestScore;
	if(iterations!=0)
	{
		bestScore = FuseTrees(qtreeRepository,repo[0],qsearchAlgorithm,evaluator,true);
	}
	else
	{
		bestScore = FuseTrees(qtreeRepository,repo[0],qsearchAlgorithm,evaluator,false);
	}
		
	PsodaPrinter::getInstance()->write("\nPBS Search finished best tree found %.1f\n",bestScore);

	for(j=1;j<mReplicates;j++)
	{
	    repo[j].removeAll();
	}
	delete[] repo_finished;
	delete[] repo;
	
	
}

double SectorSearchBase::FuseTrees(QTreeRepository& dest,QTreeRepository& src, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, bool use_best)
{

	
	//CommonCladeRefinement ccr;
	//ccr.search(src,qsearchAlgorithm,evaluator,1);
	
	
	//put only the best, or equally best trees into the main repository
	
	src.clearVisited();
	QTree* cur = src.getSearchableTree(false);
	
	//start with the last global best tree
	double bestScore = mBestScore;
	if(use_best)
	{
		dest.addTree(mBestTree);
	}
	else
	{
		bestScore = DBL_MAX;
	}
	
	while(cur!=NULL)
	{
		//printf("Considering Tree\n");
		if(!cur->isScored())
		{
			double temp_score = evaluator->qscoreTree(cur);
			cur->setScore(temp_score);
		}
		if(cur->getScore() < bestScore)
		{
			bestScore = cur->getScore();
			
			//printf("\tTree is Better %.1f\n",bestScore);
            dest.removeAll();
			dest.addTree(cur);
		}
		else if(cur->getScore() == bestScore)
		{
			
			//printf("\tTree is Same\n");
			dest.addTree(cur);
		}
		else
		{
			
			//printf("\tTree is Worse\n");
			delete cur;
		}
		cur = src.getSearchableTree(false);
	}
	
	return bestScore;
}



void SectorSearchBase::PartitionSearch(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,bool keepCurTree)
{
    double bestScore = DBL_MAX;
    QTree* currentTree = NULL;
    
    int nsplits = 0;

	
    // Clear the visited status on the repository.  We may be scoring using a new metric.  This actually deletes all of the trees and creates new ones.
    qtreeRepository.clearVisited();

    // Get a searchable tree out of the repository (this removes it, so we need to put it back when we finish)
    currentTree = qtreeRepository.getSearchableTree();
    if(currentTree == NULL)
    {
        printf("PartitionSectorSearch::PartitionSearch called with nothing in the repository\n");
        return;
    }

    // If the tree has not been scored, it hasnt been evaluated yet and we need to score it
    if(!currentTree->isScored()) {
		double temp_dbl = evaluator->qscoreTree(currentTree);
		//printf("%f\n",temp_dbl);
        currentTree->setScore( temp_dbl);
    }
    bestScore = currentTree->getScore();

	NewickTreeParser ntp;

	int sectorsRemaining = BuildSectors(currentTree);

	int curSector = 1;
	int totalSectors = sectorsRemaining;
	
	addConstraints(currentTree,sectorsRemaining);

	LabelSector(curSector,totalSectors);

    while (sectorsRemaining > 0 && !gQuit)
    {
        // Split the current tree at a new split point
        // If we have searched all split points, 
        // put this one at the end and grab a new one off the front 
        nsplits++;
        if(!currentTree->split()) // All splits have been searched, this tree is finished
        {
			nsplits=0;
			if(currentTree->getScore() > bestScore)
			{
				delete(currentTree);
			}
			else
			{
				QTree * qt = ntp.parseBuffer(currentTree->treeStr(), strlen(currentTree->treeStr()), false);
				delete(currentTree);
				qt->visited() = true;
				qtreeRepository.addTree(qt);
			}
			// If it returns null, then return since there is nothing else to do
			currentTree = qtreeRepository.getSearchableTree(false);
			
			if(currentTree == NULL||mFastSectorExit)
			{
				curSector++;
				sectorsRemaining--;
				if(sectorsRemaining > 0)
				{
					qtreeRepository.clearVisited();
					if(currentTree==NULL)
						currentTree = qtreeRepository.getSearchableTree(false);
					addConstraints(currentTree,sectorsRemaining);
					
					LabelSector(curSector,totalSectors);
				}
				else
				{
					return;
				}
			}
			else
			{
				addConstraints(currentTree,sectorsRemaining);
			}

			if(!currentTree->split())
			{
				Debug(0,0,"QRetainedResultSearch: error - tree repository returned tree that wont split, but is not visited\n");
			}
			// Don't update the printout on a simple change of trees of the same score
			//updated = true;
        }

        checkPausePSODA();

		currentTree = doTBR(qtreeRepository,qsearchAlgorithm,evaluator,currentTree,bestScore,keepCurTree);
    }
    gQuit = false;
	
	qtreeRepository.addTree(currentTree);
  
} 


bool SectorSearchBase::GlobalSearch(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,int timelimit, bool keepCurTree)
{
    double bestScore = DBL_MAX;
    QTree* currentTree = NULL;
    
    int nsplits = 0;

	
    // Clear the visited status on the repository.  We may be scoring using a new metric.  This actually deletes all of the trees and creates new ones.
    qtreeRepository.clearVisited();

    // Get a searchable tree out of the repository (this removes it, so we need to put it back when we finish)
    currentTree = qtreeRepository.getSearchableTree();
    if(currentTree == NULL)
    {
        printf("PartitionSectorSearch::GlobalSearch called with nothing in the repository\n");
        return true;
    }

    // If the tree has not been scored, it hasnt been evaluated yet and we need to score it
    if(!currentTree->isScored()) {
		double temp_dbl = evaluator->qscoreTree(currentTree);
		//printf("%f\n",temp_dbl);
        currentTree->setScore( temp_dbl);
    }
    bestScore = currentTree->getScore();

	NewickTreeParser ntp;

	int now;
	int global_search_start;
	
	now = (int) When();
	global_search_start = now;
	
    while ((now - global_search_start) < timelimit && !gQuit)
    {
        // Split the current tree at a new split point
        // If we have searched all split points, 
        // put this one at the end and grab a new one off the front 
        nsplits++;
        if(!currentTree->split()) // All splits have been searched, this tree is finished
        {
			if(currentTree->getScore() > bestScore)
			{
				delete(currentTree);
			}
			else
			{
				QTree * qt = ntp.parseBuffer(currentTree->treeStr(), strlen(currentTree->treeStr()), false);
				delete currentTree;
				qt->visited() = true;
				qtreeRepository.addTree(qt);
			}
			
			// If it returns null, then return since there is nothing else to do
			currentTree = qtreeRepository.getSearchableTree(false);
			if(currentTree == NULL)
			{
				//All of the trees have been exhaustively searched, so put current tree back in and return
				return true;
			}

			if(!currentTree->split())
			{
				Debug(0,0,"QRetainedResultSearch: error - tree repository returned tree that wont split, but is not visited\n");
			}
			// Don't update the printout on a simple change of trees of the same score
        }

        checkPausePSODA();
		currentTree = doTBR(qtreeRepository,qsearchAlgorithm,evaluator,currentTree,bestScore,keepCurTree);
		now = (int) When();
    }
    gQuit = false;
	qtreeRepository.addTree(currentTree);
	return false;
  
} 

QTree* SectorSearchBase::doTBR(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator,QTree* currentTree, double& bestScore,bool keepCurTree)
{
	list<QTree*> trees;
	
    QTree* newTree = NULL;
	
		// Perform the search algorithm.  Since you dont return the same tree that was
        // passed in, you may not return any trees that are better than or equal to the current tree
        // the second to last parameter tells the algorithm to create a new tree 

        trees.clear();
        int currentSpace = qtreeRepository.maxTrees()-qtreeRepository.numTrees();
        qsearchAlgorithm->rearrange(currentTree, evaluator, qtreeRepository.maxTrees(), currentSpace, bestScore, true, trees); 
        //        printf("search returned trees %d\n",trees.size());
        // Now rejoin the split tree so we can either split it again, or delete it
        currentTree->join();
        // Now print out progress
        DisplayScore(qtreeRepository,qsearchAlgorithm,bestScore);
		
        // rearrange wont return trees if there is no currentSpace and no better trees were found.
        // It will also not return trees if no equal trees were found
        if(trees.size() == 0)
        {
            //                printf("rearrange didnt return any trees\n");
            return currentTree;
        }
        newTree = trees.front();  // Get a tree out of the list
        trees.pop_front();
        //printf("after rearrange %f\n",evaluator->qscoreTree(currentTree, dataset ));


        // If we found a new tree that is better than the best score
        // Flush the tree repository
        if(newTree->getScore() < bestScore)
        {
            mUpdated = true;
            bestScore = newTree->getScore();

			if(bestScore < mBestScore) //copy this tree into the global best tree
			{
				mBestScore = bestScore;
				if(mBestTree) delete mBestTree;
				NewickTreeParser ntp;
				mBestTree = ntp.parseBuffer(newTree->treeStr(), strlen(newTree->treeStr()), false);
			}


            // Nuke the tree repository
            qtreeRepository.removeAll();

            if(keepCurTree)
            {
              qtreeRepository.addTree(newTree);
            }
            else
            {
                delete(currentTree);
                currentTree = newTree;
            }
        }
        else // If it wasnt better, it must have been equal, so put it in the repository
        {
            //        printf("equal adding to repository split score %f nnodes %d nchars %d earlyprune %d\n",newTree->score(),newTree->nnodes(), newTree->nchars(), newTree->earlyPrune());
	  qtreeRepository.addTree(newTree);
        }	
        // Now put all of the other trees into the repository
        // If rearrange returned any trees, they must be at least as good and need to be put in 
        //  the repository
        while(trees.size())
        {
            newTree = trees.front();  // Get a tree out of the list
            trees.pop_front();
			//temp = ntp.parseBuffer(newTree->treeStr(),strlen(newTree->treeStr()),false);
			//consense.addTree(temp);
            qtreeRepository.addTree(newTree);
        }

	return currentTree;
}

void SectorSearchBase::DisplayScore(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,double curScore)
{
	static int now = 0;
	static int lastprint = 0;
	static int lastmenu = 0;
	static double bestScore = DBL_MAX;
	
	if(curScore < bestScore) bestScore = curScore;

	Dataset* dataset = Interpreter::getInstance()->dataset();

	now = (int) When();
		
		//display
		if ((now-lastmenu) > 100)
        {
            PsodaPrinter::getInstance()->write("\nElapsed\t\t      Rearr.   No. of trees\t\tCurrent trees\tBest Tree\n");
            PsodaPrinter::getInstance()->write("   time\t\t      tried   saved left-to-swap\n");
            PsodaPrinter::getInstance()->write("--------------------------------------------------------------------------\n");
            lastmenu = now;
        }

        if ((mUpdated && ((now-lastprint) > 0)) || ((now-lastprint) > 10))
		{
            int seconds = (int) (When()-mStartTime);
            int hours = seconds / (60*60);
            int minutes = (seconds / 60)%60;
            seconds = seconds % 60;
						// We need to print out the score differently if it is likelihood
						// We should probably call the evaluator and let him format score
						// For now, see if the number is integral and if not, print out
						// four digits
						
						double intscore = floor(curScore);
#define MAXSCORESTRING 18
						char curscorestring[MAXSCORESTRING];
						if(intscore != curScore) { // It is non-integral
							snprintf(curscorestring,MAXSCORESTRING,"%14.4f",curScore);
						} else {
							snprintf(curscorestring,MAXSCORESTRING,"%14.0f",curScore);
						}
						
						intscore = floor(bestScore);
						char scorestring[MAXSCORESTRING];
						if(intscore != bestScore) { // It is non-integral
							snprintf(scorestring,MAXSCORESTRING,"%14.4f",bestScore);
						} else {
							snprintf(scorestring,MAXSCORESTRING,"%14.0f",bestScore);
						}
						
            PsodaPrinter::getInstance()->write("%02i:%02i:%02i",hours, minutes, seconds);
			  PsodaPrinter::getInstance()->write("\t%12.0f %5i %5i\t%s\t%s\n",
                    
                    qsearchAlgorithm->rearrangements(),
                    qtreeRepository.numTrees(),
                    qtreeRepository.numTreesToSwap(),
					curscorestring, 
                    scorestring);
            lastprint = now;
            mUpdated = false;
        }
}


