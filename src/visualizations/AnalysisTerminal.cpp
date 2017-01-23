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
#include "Interpreter.h"
#include "AnalysisTerminal.h"
#include "RF.h"
#include "Parsimony.h"

#include <stdlib.h>
#include <math.h>

AnalysisTerminal::AnalysisTerminal():VisualizationTerminal()
{
}

void AnalysisTerminal::display(const list<ProjectedTree> points)
{
	for(list<ProjectedTree>::const_iterator i = points.begin();i != points.end();i++)
	{
		mPoints.push_back(*i);
	}
	
	fprintf(fout,"#k\tCon Deg\tRF Dist\tMP Diff\tMP std\n");
	fprintf(fout,"1\t1.000\t0.000\t0.000\t0.000\n");
	for(int k=2;k<25;k++)
	{
		printf("K = %d\n",k);
		fprintf(fout,"%d\t",k);
		FindNearestNeighbors(k);
		fprintf(fout,"%.3f\t",ConsensusDegree());
		fprintf(fout,"%.3f\t",RFDistances());
		fprintf(fout,"%.3f\t",MPDifference());
		if(k<10)
			fprintf(fout,"%.3f\n",MPVariance());
		else
			fprintf(fout,"\n");
		ClearNearestNeighbors();
	}
}

/**
 * FindNearestNEighbors builds the Neigbors list of repositorys, this list contains the k-nearest neigbors for each point
 */
void AnalysisTerminal::FindNearestNeighbors(int k)
{
	double * dist = (double*)malloc(sizeof(double)*k);
	ProjectedTree* neigbors;
	neigbors = new ProjectedTree[k];
	for(int iter=0;iter<k;iter++)
	{
		neigbors[iter] = ProjectedTree(3);
	}
	
	
	for(list<ProjectedTree>::const_iterator i = mPoints.begin();i != mPoints.end(); i++)
	{
		//initialize space to keep track of near neighbors
		for(int iter=0;iter<k;iter++)
		{
			dist[iter] = DBL_MAX;
			neigbors[iter](0) = 0.0;
			neigbors[iter](1) = 0.0;
			neigbors[iter](2) = 0.0;
		}
		
		//check everyone (this is clearly an ineffecient solution)
		for(list<ProjectedTree>::const_iterator j = mPoints.begin();j != mPoints.end(); j++)
		{
			double cur_dist;
			int cur;
			
			cur_dist = (*i - *j).length();
			
			if(cur_dist < dist[k-1])
			{
				dist[k-1] = cur_dist;
				neigbors[k-1] = *j;
				cur = k-1;
				while(cur>0&&dist[cur] < dist[cur-1])
				{
					//swap cur and cur-1 (bubbling distance into correct position
					double temp_dist;
					ProjectedTree temp_tree;
					
					temp_dist = dist[cur-1];
					dist[cur-1] = dist[cur];
					dist[cur] = temp_dist;
					
					temp_tree = neigbors[cur-1];
					neigbors[cur-1] = neigbors[cur];
					neigbors[cur] = temp_tree;
					
					cur--;
				}
			}
		}
		//Add nearest neigbors into near neigbor list
		QTreeRepository* rep = new QTreeRepository(false);
    int ntaxa = Interpreter::getInstance()->dataset()->ntaxa();
		rep->setMaxTrees(k, ntaxa);
		for(int iter=0;iter<k;iter++)
		{
			if(dist[iter] < DBL_MAX)
			{
				QTree* qt = neigbors[iter].getTree();
				rep->addTree(qt);
			}
		}
		mNeighbors.push_back(rep);
		
	}
	free(dist);
}

/**
 * ClearNearestNeighbors cleanly removes all near neighbor lists
 */
void AnalysisTerminal::ClearNearestNeighbors()
{
	/*
	QTreeRepository* rep;
	while(rep = mNeighbors.front())
	{
		delete rep;
		mNeighbors.pop_front();
	}
	*/
	mNeighbors.clear();
}

/**
 * ConsensusDegree() returns a value from 0 to 1 indicating the average degree of resolution between near neigbors, 0 being fully unresolved, 1 being fully resolved.
 */
double AnalysisTerminal::ConsensusDegree()
{
	RF* con_tree = NULL;
	double avg = 0.0;
	int num_trees = 0;
	for(list<QTreeRepository*>::const_iterator i = mNeighbors.begin();i!= mNeighbors.end();i++)
	{
		con_tree = new RF(*i);
		double tmp = con_tree->resolution(0.5);
		
		avg += tmp;
		num_trees++;
		
		delete con_tree;
	}

	return avg/num_trees;
}


/**
 * RFDistances() returns the average RF distance amoung the neigbor groups
 */
double AnalysisTerminal::RFDistances()
{
	RF* distances = NULL;
	double avg = 0.0;
	int num_compares = 0;
	for(list<QTreeRepository*>::const_iterator i = mNeighbors.begin();i!= mNeighbors.end();i++)
	{
		distances = new RF(*i);
		for(int j=0;j<distances->numTrees();j++)
		{
			for(int k=j+1;k<distances->numTrees();k++)
			{
				avg += distances->operator()(j,k);
				num_compares++;
			}
		}
		delete distances;
	}

	return avg/num_compares;
}



/**
 * MPDifference() returns a value from 0 to 1 indicating the average difference in MP scores.
 */
double AnalysisTerminal::MPDifference()
{
	return 0.0;
}


/**
 * MPVariance() returns a value from 0 to 1 indicating the average variance in MP scores.
 */
double AnalysisTerminal::MPVariance()
{
	Parsimony eval;
	double* scores;
	double avg_var = 0.0;
	int num_vars = 0;
	for(list<QTreeRepository*>::const_iterator i = mNeighbors.begin();i!= mNeighbors.end();i++)
	{
		int num_trees = (*i)->numTrees();
		//Calculate MP scores for all trees in the neighborhood
		scores = (double*)malloc(sizeof(double)*num_trees);
		
		deque<QTree *> tree_list = *((*i)->getTrees());
		deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list
		int j=0;
		for(treeIt = tree_list.begin(); treeIt != tree_list.end(); treeIt++)
		{
			//For each tree
			scores[j] = eval.qscoreTree(*treeIt);
			j++;
		}	
		//Calculate Avg MP score
		double avg = 0.0;
		for(int j=0;j<num_trees;j++)
		{
			avg+=scores[j];
		}
		avg /= num_trees;
		
		//Calculate varience
		double var = 0.0;
		for(int j=0;j<num_trees;j++)
		{
			var+=(avg-scores[j])*(avg-scores[j]);
		}
		var /= num_trees;
	
		avg_var+= var;
		num_vars++;
	
		free(scores);
	}
	
	return sqrt(avg_var/num_vars);
}
