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
#include "AlternatingSearch.h"
#include "PsodaPrinter.h"
#include "Interpreter.h"
#include "Dataset.h"
#include "PsodaPrinter.h"

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
#include "Sectarian.h"
#include "CommonCladeRefinement.h"
#define MAXSCORESTRING 18

AlternatingSearch::AlternatingSearch() 
{
}

const char* AlternatingSearch::name() const
{
	return "QRetainedResults";
}

/**
 * Perform the Retained Results Search
 */
void AlternatingSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int iterations)
{
    int i;
    double bestScore = DBL_MAX;
    int maxIterations = INT_MAX;
    QTree* currentTree = NULL;
    list<QTree*> trees;
    QTree* newTree = NULL;
    int nsplits = 0;
#ifdef DEBUGTREEREARRANGE
    // DEBUG TREE REARRANGE
    string testString;
    int break1,break2;
    // DEBUG TREE REARRANGE
#endif
	SectarianSearch sectsearch;
	sectsearch.fast_return = false;
	bool tried_sect = false;
    int now=0, lastmenu=0, lastprint=0;
    bool updated = true; // We updated the current tree, so we need to print it out
	Dataset* dataset = Interpreter::getInstance()->dataset();
	

    mStartTime = When();
    PsodaPrinter::getInstance()->write("alternating search iterations %d\n",iterations);
    if (iterations > 0)
    {
        maxIterations = iterations;
    }

    // Clear the visited status on the repository.  We may be scoring using a new metric.  This actually deletes all of the trees and creates new ones.
    qtreeRepository.clearVisited();

    // Get a searchable tree out of the repository (this removes it, so we need to put it back when we finish)
    currentTree = qtreeRepository.getSearchableTree();
    if(currentTree == NULL)
    {
        printf("RetainedResultsSearched called with nothing in the repository\n");
        return;
    }

    // If the tree has not been scored, it hasnt been evaluated yet and we need to score it
    if(!currentTree->isScored()) {
        currentTree->setScore( evaluator->qscoreTree(currentTree) );
    }
    bestScore = currentTree->getScore();

#ifdef DEBUGTREEREARRANGE
    // DEBUG TREE REARRANGE
    //currentTree->canonicalize();
    currentTree->updateTreeString();
    testString = currentTree->treeStr();
    printf("from the repository\n");
    // DEBUG TREE REARRANGE
#endif
	int currentSpace;
	int perturbedIterations = 0;
	bool perturbed = false;
	double bestScoreBak = bestScore;
	int lastperturbed = (int)When();
	int timeafterrefine;
	int perterbed_iterations = 0;
	//int iterations_till_perturb = 3000;
    for ( i = 0 ; i < maxIterations && !gQuit; i ++ )

    {
		now = (int) When();
		//if(qtreeRepository.numTrees() > 200 && qtreeRepository.numTreesToSwap() < qtreeRepository.numTrees() && (tried_sect||perturbed))
		if((now-lastperturbed)>120)
		{
			lastperturbed=now;
			if(perturbed)
			{
				perterbed_iterations++;
				Interpreter::getInstance()->dataset()->resetWeights();
				PsodaPrinter::getInstance()->write("\n-- Resetting Weights -- \n");
				//iterations_till_perturb = 3000;
			}
			else
			{
					if(qtreeRepository.numTreesToSwap() < qtreeRepository.numTrees()&&perterbed_iterations>5)
					{
						PsodaPrinter::getInstance()->write("\nElapsed         Taxa       Rearr.     No. of trees       Best trees     Search Iter\n");
						PsodaPrinter::getInstance()->write("   time         added      tried   saved   to refine  \n");
						PsodaPrinter::getInstance()->write("-------------------------------------------------------------------------------\n");
						lastmenu = now;
						//try sectarian search on all trees
						int numTrees = qtreeRepository.numTrees();
						
						currentSpace = qtreeRepository.maxTrees()-qtreeRepository.numTrees();
						while(1) // continue untill all trees are searched, or something better is found
						{
							currentTree->split();
							//try sectarian search on this tree
							sectsearch.rearrange(currentTree, evaluator, qtreeRepository.maxTrees(), currentSpace, bestScore, true, trees); 
							//i++; //update iterations
							now = (int) When();
							if ((now-lastprint) > 5)
							{
								int seconds = (int) (When()-mStartTime);
								int hours = seconds / (60*60);
								int minutes = (seconds / 60)%60;
								seconds = seconds % 60;
											// We need to print out the score differently if it is likelihood
											// We should probably call the evaluator and let him format score
											// For now, see if the number is integral and if not, print out
											// four digits
											
											double intscore = floor(bestScore);
											char scorestring[MAXSCORESTRING];
											if(intscore != bestScore) { // It is non-integral
												snprintf(scorestring,MAXSCORESTRING,"%14.4f",bestScore);
											} else {
												snprintf(scorestring,MAXSCORESTRING,"%14.0f",bestScore);
											}
											
								PsodaPrinter::getInstance()->write("%02i:%02i:%02i %10i %12.0f %5i %5i     %s           %d\n",
										hours, minutes, seconds, 
										dataset->ntaxa(), /* number of taxa */
										//100,
										qsearchAlgorithm->rearrangements()+sectsearch.rearrangements(), 
										qtreeRepository.numTrees(),
										qtreeRepository.numTreesToSwap(),
										scorestring,
										i);
								lastprint = now;
								updated = false;
							}
							// rearrange wont return trees if there is no currentSpace and no better trees were found.
							// It will also not return trees if no equal trees were found
							if(trees.size() == 0)
							{
								//                printf("rearrange didnt return any trees\n");
								continue;
							}
							newTree = trees.front();  // Get a tree out of the list
							trees.pop_front();

							// If we found a new tree that is better than the best score
							// Flush the tree repository
							if(newTree->getScore() < bestScore)
							{
								//printf("\t Found _BETTER_ tree!\n");
								updated = true;
								bestScore = newTree->getScore();

								// Nuke the tree repository
								qtreeRepository.removeAll();

								
								qtreeRepository.addTree(newTree);
								
							}
							else // If it wasnt better, it must have been equal, so put it in the repository
							{
								//printf("\t found equal trees\n");
								
							//printf("Before add Equal - %d trees\n",qtreeRepository.numTreesToSwap());
								qtreeRepository.addTree(newTree);
							//					printf("After add Equal - %d trees\n",qtreeRepository.numTreesToSwap());
							}	
							// Now put all of the other trees into the repository
							// If rearrange returned any trees, they must be at least as good and need to be put in 
							//  the repository
							while(trees.size())
							{
								newTree = trees.front();  // Get a tree out of the list
								trees.pop_front();
								qtreeRepository.addTree(newTree);
							}
							//printf("Getting next tree\n");
							// If it returns null, then return since there is nothing else to do
							//printf("Before getSearchableTree - %d trees\n",qtreeRepository.numTreesToSwap());
							currentTree = qtreeRepository.getSearchableTree();
							
							//printf("After getSearchableTree - %d trees\n",qtreeRepository.numTreesToSwap());
							if(currentTree == NULL)
							{
								if(!perturbed)
									break;
								else	//don't end with the dataset perturbed
								{
									Interpreter::getInstance()->dataset()->resetWeights();
									PsodaPrinter::getInstance()->write("\nResetting Weights -- Tree Scores are not comparable with perturbed scores\n");
							
									perturbed = false;
									bestScore = qtreeRepository.rescoreTrees(evaluator); //make sure that the trees are reconsidered
									
									updated = true;
								
								}
							}
							if(!currentTree->isScored()) {
								currentTree->setScore( evaluator->qscoreTree(currentTree) );
							}
							if(updated) break; //continue on with normal search
						}
						if(!updated)
						{
							//this didn't help
						
							tried_sect = true;
							qtreeRepository.clearVisited(); //make sure that the trees that we couldn't improve are examined by TBR
							PsodaPrinter::getInstance()->write("\nElapsed         Taxa       Rearr.     No. of trees       Best trees     Search Iter\n");
							PsodaPrinter::getInstance()->write("   time         added      tried   saved left-to-swap  \n");
							PsodaPrinter::getInstance()->write("-------------------------------------------------------------------------------\n");
							lastmenu = now;
							currentTree = qtreeRepository.getSearchableTree();
							if(currentTree == NULL)
							{
								printf("Refinement left nothing in the repository\n");
								return;
							}
						}
					}
			
				Interpreter::getInstance()->dataset()->randomizeWeights(0.15);
				PsodaPrinter::getInstance()->write("\n-- Perturbing Weights -- \n");
				//iterations_till_perturb=500; //search for just a few iterations under the perturbed scores
			}
			perturbed = !perturbed;
			if(qtreeRepository.numTrees() > 0)
				bestScore = qtreeRepository.rescoreTrees(evaluator); //make sure that the trees are reconsidered
			else
				bestScore = evaluator->qscoreTree(currentTree);
			//printf("%d trees with new score %.0f\n",qtreeRepository.numTrees(),bestScore);
			updated = true;
			tried_sect = perturbed;
		}

		



        // Split the current tree at a new split point
        // If we have searched all split points, 
        // put this one at the end and grab a new one off the front 
        nsplits++;
        if(!currentTree->split()) // All splits have been searched
        {
            // Put it back in the repository and get a new tree to search
            if((AlgorithmFlags & KEEPTREEUNTILALLSPLITS) && (currentTree->getScore() > bestScore))
            {
                // If there are better trees in the repository, delete the current one
                printf("++++++++++++++++++++++++++++++++++++++++++++++++\n");
                printf("++Finished n^3 Getting New Tree from Repository+\n");
                printf("++++++++++++++++++++++++++++++++++++++++++++++++\n");
                delete(currentTree);
            }
            else
            {
//For Some reason, minimizing causes the tree to have the wrong score
//              currentTree->minimize(); // Free the nodedata
			  
			  NewickTreeParser* ntp = new NewickTreeParser();
			  QTree * qt = ntp->parseBuffer(currentTree->treeStr(), strlen(currentTree->treeStr()), false);
			  qt->visited() = true;
              qtreeRepository.addTree(qt);
			  delete ntp;
            }
            // If it returns null, then return since there is nothing else to do
            currentTree = qtreeRepository.getSearchableTree();
            if(currentTree == NULL)
            {
                Debug(0,0, "Searched all trees, best score %f\n",bestScore);
                //All of the trees have been exhaustively searched, so put current tree back in and return
                return;
            }
#ifdef DEBUGTREEREARRANGE
            // DEBUG TREE REARRANGE
            //currentTree->canonicalize();
            currentTree->updateTreeString();
            testString = currentTree->treeStr();
            printf("finished splitting old one and got new out of repository\n");
            // DEBUG TREE REARRANGE
#endif
            if(!currentTree->split())
            {
                Debug(0,0,"QRetainedResultSearch: error - tree repository returned tree that wont split, but is not visited\n");
            }
            // Don't update the printout on a simple change of trees of the same score
            //updated = true;
        }


        checkPausePSODA();

#ifdef DEBUGTREEREARRANGE
        break1 = currentTree->subTree1()->nodeInfo()->nodeIndex();
        break2 = currentTree->subTree2()->nodeInfo()->nodeIndex();
#endif
        // Perform the search algorithm.  Since you dont return the same tree that was
        // passed in, you may not return any trees that are better than or equal to the current tree
        // the second to last parameter tells the algorithm to create a new tree 

        trees.clear();
        currentSpace = qtreeRepository.maxTrees()-qtreeRepository.numTrees();
        qsearchAlgorithm->rearrange(currentTree, evaluator, qtreeRepository.maxTrees(), currentSpace, bestScore, true, trees); 
        //        printf("search returned trees %d\n",trees.size());
        // Now rejoin the split tree so we can either split it again, or delete it
        currentTree->join();
        // Now print out progress
        now = (int) When();
        if (((now-lastmenu) > 100)&&!perturbed)
        {
            PsodaPrinter::getInstance()->write("\nElapsed         Taxa       Rearr.     No. of trees       Best trees     Search Iter\n");
            PsodaPrinter::getInstance()->write("   time         added      tried   saved left-to-swap  \n");
            PsodaPrinter::getInstance()->write("-------------------------------------------------------------------------------\n");
            lastmenu = now;
        }

        if (((updated && ((now-lastprint) > 0)) || ((now-lastprint) > 10))&&!perturbed)
		{
            int seconds = (int) (When()-mStartTime);
            int hours = seconds / (60*60);
            int minutes = (seconds / 60)%60;
            seconds = seconds % 60;
						// We need to print out the score differently if it is likelihood
						// We should probably call the evaluator and let him format score
						// For now, see if the number is integral and if not, print out
						// four digits
						
						double intscore = floor(bestScore);
						char scorestring[MAXSCORESTRING];
						if(intscore != bestScore) { // It is non-integral
							snprintf(scorestring,MAXSCORESTRING,"%14.4f",bestScore);
						} else {
							snprintf(scorestring,MAXSCORESTRING,"%14.0f",bestScore);
						}
						
            PsodaPrinter::getInstance()->write("%02i:%02i:%02i %10i %12.0f %5i %5i     %s           %d\n",
                    hours, minutes, seconds, 
                    dataset->ntaxa(), /* number of taxa */
                    //100,
                    qsearchAlgorithm->rearrangements()+sectsearch.rearrangements(), 
                    qtreeRepository.numTrees(),
                    qtreeRepository.numTreesToSwap(),
                    scorestring,
                    i);
            lastprint = now;
            updated = false;
        }
        // rearrange wont return trees if there is no currentSpace and no better trees were found.
        // It will also not return trees if no equal trees were found
        if(trees.size() == 0)
        {
            //                printf("rearrange didnt return any trees\n");
            continue;
        }
        newTree = trees.front();  // Get a tree out of the list
        trees.pop_front();
        //printf("after rearrange %f\n",evaluator->qscoreTree(currentTree, dataset ));
#ifdef DEBUGTREEREARRANGE
        // DEBUG TREE REARRANGE
        //currentTree->canonicalize();
        //cout << "Current Tree1: " << currentTree->treeStr() << endl;
        currentTree-> updateTreeString();
        string afterString = currentTree->treeStr();
        //cout << "Current Tree2: " << currentTree->treeStr() << endl;
        if(testString != afterString)
        {
            printf("ERROR: tree changed break1 %d break2 %d\n",break1,break2);
            cout << "Old\n"<<testString<<"\nNEW\n"<<afterString;
            //cout << "Current Tree3: " << currentTree->treeStr() << endl;
        }
        // DEBUG TREE REARRANGE
#endif

        // If we found a new tree that is better than the best score
        // Flush the tree repository
        if(newTree->getScore() < bestScore)
        {
            updated = true;
			tried_sect = false;
            bestScore = newTree->getScore();
#ifdef DEBUGTREEREARRANGE
            // DEBUG TREE REARRANGE

            //currentTree->canonicalize();
            currentTree->updateTreeString();
            testString = currentTree->treeStr();
            // DEBUG TREE REARRANGE
#endif

            // Nuke the tree repository
            qtreeRepository.removeAll();

            if(AlgorithmFlags & KEEPTREEUNTILALLSPLITS)
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
            qtreeRepository.addTree(newTree);
        }

    }
	
	if(perturbed)  //reset the weights and rescore trees before returning
	{
		Interpreter::getInstance()->dataset()->resetWeights();
		bestScore = qtreeRepository.rescoreTrees(evaluator); //make sure that the trees are reconsidered
		perturbed = false;
		 int seconds = (int) (When()-mStartTime);
            int hours = seconds / (60*60);
            int minutes = (seconds / 60)%60;
            seconds = seconds % 60;
						// We need to print out the score differently if it is likelihood
						// We should probably call the evaluator and let him format score
						// For now, see if the number is integral and if not, print out
						// four digits
						
						double intscore = floor(bestScore);
						char scorestring[MAXSCORESTRING];
						if(intscore != bestScore) { // It is non-integral
							snprintf(scorestring,MAXSCORESTRING,"%14.4f",bestScore);
						} else {
							snprintf(scorestring,MAXSCORESTRING,"%14.0f",bestScore);
						}
						
            PsodaPrinter::getInstance()->write("%02i:%02i:%02i %10i %12.0f %5i %5i     %s           \n",
                    hours, minutes, seconds, 
                    dataset->ntaxa(), /* number of taxa */
                    //100,
                    qsearchAlgorithm->rearrangements()+sectsearch.rearrangements(), 
                    qtreeRepository.numTrees(),
                    qtreeRepository.numTreesToSwap(),
                    scorestring);
	}
	
	//TODO: Add Try block to mimic old behavior
	
    //if(OKToKeepRunning == 0){
    //    PsodaPrinter::getInstance()->write("**Run STOPPED**\n");
    //    qtreeRepository.addTree(currentTree);
    //} else {
      PsodaPrinter::getInstance()->write("Finished Iterations\n");
      qtreeRepository.addTree(currentTree);
    //}

}


