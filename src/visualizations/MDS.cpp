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
#include "MDS.h"
#include "RF.h"
#include "Interpreter.h"
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#define MDS_DIM 2

/**
 * Default constructor for MDS class
 */

MDS::MDS():VisualizationBase(),ntrees(0),target(NULL),points(NULL),rate(0.001)
{}


/**
 * Destructor for MDS
 */
MDS::~MDS()
{
	if(target)
	{
		for(int i=0;i<ntrees;i++)
			if(target[i])
				free(target[i]);
		free(target);
	}
	if(points)
		delete[] points;
	
}

/**
 * visualization produces a list of projected points which will be displayed by some other function
 */

list<ProjectedTree> MDS::visualization()
{
	produceMap(&mTrees);
	list<ProjectedTree> treeList;
	
	for(int i=0;i<ntrees;i++)
	{
		treeList.push_back(points[i]);
	}	
	return treeList;
	
}

/**
 * Build a MDS mapping for the given tree repository
 */
void MDS::produceMap(QTreeRepository* trees)
{
	int i;
	
	mNTaxa = Interpreter::getInstance()->dataset()->ntaxa();
	
	calculateTargets(trees);
	//start with random locations
	points = new ProjectedTree[ntrees];
	for(i=0;i<ntrees;i++)
	{
		QTree* qt = trees->popTree();
		points[i]= ProjectedTree(MDS_DIM);
			for(int j=0;j<MDS_DIM;j++)
				points[i](j) = (2*(double)rand()/INT_MAX -1) * mNTaxa;
		points[i].setTree(qt);
		trees->addTree(qt);
	}

	//ClassicMDS();
	GoldenMDS();
	//GeneticMDS();
	
	/* Display list of points
	* TODO this should be done in a better way
	*
	FILE* display = fopen("MDS.data","w");
	for(int i=0;i<ntrees;i++)
		fprintf(display,"%.2f\t%.2f\n",points[i](0),points[i](1));
	fclose(display);	
	*/
	/* Display Sheppard Diagram
	* TODO this should be done in a better way
	*/
	FILE* sheppard = fopen("sheppards.data","w");
	printf("\nSheppards Diagram\n\n");
	for(int i=0;i<ntrees;i++)
	for(int j=i+1;j<ntrees;j++)
	{
		ProjectedTree error(MDS_DIM);
		error = points[i] - points[j];
		fprintf(sheppard,"%.2f\t%.2f\n",target[i][j],error.length());
	}
	fclose(sheppard);
}


/**
 * Build a MDS mapping using a Golden Section search for reducing strain
 */
 
void MDS::GoldenMDS()
{
	int i,j;
	//int iter = 0;

	
	
	
	ProjectedTree* gradient = new ProjectedTree[ntrees];
	for(i=0;i<ntrees;i++)
	{
		gradient[i]= ProjectedTree(MDS_DIM);
		for(j=0;j<MDS_DIM;j++)
			gradient[i](j) = 0;
	}
	
	
	
	ProjectedTree* best = new ProjectedTree[ntrees];
	for(i=0;i<ntrees;i++)
	{
		best[i]= points[i];
	}
	
	double best_strain = DBL_MAX;
	
	for(int iter=0;iter< 5;iter++)
	{
		
	for(i=0;i<ntrees;i++)
	{
		QTree* qt = mTrees.popTree();
		points[i]= ProjectedTree(MDS_DIM);
		for(j=0;j<MDS_DIM;j++)
			points[i](j) = (2*(double)rand()/INT_MAX -1) * mNTaxa;
		points[i].setTree(qt);
		mTrees.addTree(qt);
	}
	
	double jitter = 2*mNTaxa;
	
	while(jitter > 0.0001)
	{
	
	double old_strain = DBL_MAX;
	printf("Calculating strain . . . ");
	double cur_strain = calculateStrain();
	printf("Strain %.2f\n",cur_strain);
	
	while(old_strain - cur_strain > 0.000001)
	{
	
#undef ANALITICAL_GRAD
#ifdef ANALITICAL_GRAD
		//Calculate gradient from current position
		//calculations based on analytical gradient of strain function
		for(i=0;i<ntrees;i++)
		{
			gradient[i](0) = 0;
			gradient[i](1) = 0;
			for(j=0;j<ntrees;j++)
			{
				if(i!=j)
				{
					double x = points[i](0)-points[j](0);
					double y =  points[i](1)-points[j](1);
					gradient[i](0) += x/(sqrt(x*x + y*y));
					gradient[i](1) += y/(sqrt(x*x + y*y));
				}
			}
			gradient[i](0) *= -2;
			gradient[i](1) *= -2;
		}
#else
	for(i=0;i<ntrees;i++)
	{
		for(j=0;j<MDS_DIM;j++)
			gradient[i](j) = 0;

		for(int j=0;j<ntrees;j++)
		{
			ProjectedTree error(MDS_DIM);
			if(i!=j)
			{
				//Find direction from ith point to jth point
				error = points[j] - points[i];
			
				//scale error so that adding error to ith point would make the distance between i and j correct
				error *= target[i][j] / error.length();
				//add this error to the residual of i
				gradient[i] += error;
			}
		}
	}
#endif
		//minimization requires three points a < b < c and strain(b) < strain(a) and strain(b) < strain(c)
		double a,b,c,x;
		double strain_a,strain_b,strain_c;
		a = 0;
		strain_a = cur_strain;
		
		b = 0.63;
		ProjectedTree* test = new ProjectedTree[ntrees];
		for(i=0;i<ntrees;i++)
		{
			test[i]= ProjectedTree(MDS_DIM);
			test[i] = points[i] + gradient[i] * b;
		}
		strain_b = calculateStrain(test);
		
		c = 1.0;
		for(i=0;i<ntrees;i++)
		{
			test[i]= ProjectedTree(MDS_DIM);
			test[i] = points[i] + gradient[i] * c;
		}
		strain_c = calculateStrain(test);
		
		while(!((strain_b < strain_a)&&(strain_b < strain_c)))
		{
			if(strain_c < strain_b)
			{
				x = c + (c-b)*1.6;
				a = b;
				strain_a = strain_b;
				b = c;
				strain_b = strain_c;
				
				c = x;
				for(i=0;i<ntrees;i++)
				{
					test[i]= ProjectedTree(MDS_DIM);
					test[i] = points[i] + gradient[i] * c;
				}
				strain_c = calculateStrain(test);
			}
			else
			{
				x = a - (b-a)*1.6;
				c = b;
				strain_c = strain_b;
				b = a;
				strain_b = strain_a;
				a = x;
				
				for(i=0;i<ntrees;i++)
				{
					test[i]= ProjectedTree(MDS_DIM);
					test[i] = points[i] + gradient[i] * a;
				}
				strain_a = calculateStrain(test);
			}
		}
		
		delete[] test;
		
		//find a minimum
		double min_displacement = minimize(gradient,a,strain_a,b,strain_b,c,strain_c);
		
		//adjust points
		for(i=0;i<ntrees;i++)
		{
			points[i] += gradient[i] * min_displacement ;
		}
		
		//readjust strain
		old_strain = cur_strain;
		printf("Calculating strain . . . ");
		cur_strain = calculateStrain();
		printf("Strain %.2f\n",cur_strain); 
	}
	if(cur_strain < best_strain)
	{
		for(i=0;i<ntrees;i++)
		{
			best[i] = points[i];
		}
		best_strain = cur_strain;
		printf("New best mapping found Strain = %.3f\n",best_strain);
	}
	
	printf("Jittering %.5f\n",jitter);
	for(i=0;i<ntrees;i++)
	{
		for(j=0;j<MDS_DIM;j++)
			points[i](j) += (2*(double)rand()/INT_MAX - 1.0) * jitter;
	}
	jitter *= 0.6;
	}
	for(i=0;i<ntrees;i++)
	{
		points[i] = best[i];
	}
	}
	printf("Best mapping found Strain = %.5f\n",best_strain);
	delete[] best;
	delete[]gradient;
}

/**
 * Build a MDS mapping for the given tree repository using classic MDS methods
 */
void MDS::ClassicMDS()
{
	int iter = 0;

	double old_strain = DBL_MAX;
	printf("Calculating strain . . . ");
	double cur_strain = calculateStrain();
	printf("Strain %.2f\n",cur_strain);
	while(old_strain - cur_strain > 0.00001)
	{
		iter++;
		adjustPoints();
		old_strain = cur_strain;
		printf("Calculating strain . . . ");
		fflush(NULL);
		cur_strain = calculateStrain();
		printf("Strain %.2f\n",cur_strain);
		printf("Diff = %.6f\n",old_strain-cur_strain);
		if(iter%10==0)
		{
		//	printf("Strain %.2f\n",cur_strain);
			if(iter%100==0)
			{
				FILE* display = fopen("MDS.data","w");
				for(int i=0;i<ntrees;i++)
					fprintf(display,"%.2f\t%.2f\n",points[i](0),points[i](1));
				fclose(display);	
				/* Display Sheppard Diagram
				* TODO this should be done in a better way
				*/
				FILE* sheppard = fopen("sheppards.data","w");
				printf("\nSheppards Diagram\n\n");
				for(int i=0;i<ntrees;i++)
				for(int j=i+1;j<ntrees;j++)
				{
					ProjectedTree error(2);
					error = points[i] - points[j];
					fprintf(sheppard,"%.2f\t%.2f\n",target[i][j],error.length());
				}
				fclose(sheppard);
			}
		}
	}

}
 
/**
 * Calculate the all-to-all distances between trees in the given repository
 */
void MDS::calculateTargets(QTreeRepository* trees)
{
	int i,j;
	RF dist_mat(trees);
	ntrees = dist_mat.numTrees();
	
	target = (double**) malloc(sizeof(double*)*ntrees);
	for(i=0;i<ntrees;i++)
	target[i] = (double*) malloc(sizeof(double)*ntrees);
	
	for(i=0;i<ntrees;i++)
	for(j=0;j<ntrees;j++)
		target[i][j] = (double)dist_mat(i,j);
	return;
	
		
}

/**
 *  Adjust all points along the error gradient
 */
#define BATCH_MDS
void MDS::adjustPoints()
{
	ProjectedTree* residual;
	bool adjust_rate = false;
	residual = new ProjectedTree[ntrees]; 
	for(int i=0;i<ntrees;i++)
	{
		residual[i] = ProjectedTree(MDS_DIM);
		
		
		for(int j=0;j<ntrees;j++)
		{
			ProjectedTree error(MDS_DIM);
			if(i!=j)
			{
				//Find direction from ith point to jth point
				error = points[j] - points[i];
			
				//scale error so that adding error to ith point would make the distance between i and j correct
				error *= target[i][j] / error.length();
				//add this error to the residual of i
				residual[i] += error;
			}
		
		}
		//scale residual by the rate parameter
		
		residual[i] *= rate;
#ifndef BATCH_MDS
		double strain = calculateStrain();
		printf("Adjusting point %d/%d - current strain %.5f\n",i,ntrees,strain);
		points[i] -= residual[i];
		double nstrain = calculateStrain();
		if(nstrain > strain)
		{
			//printf("Reversing bad adjustment\n");
			points[i] += residual[i];
			adjust_rate = true;
		}
#endif		
	}
	
#ifdef BATCH_MDS
	//adjust points by scaled residuals
	double strain = calculateStrain();
	for(int i=0;i<ntrees;i++)
	{
		printf("Adjusting point %d/%d - current strain %.5f\n",i,ntrees,strain);
		points[i] -= residual[i];
		double nstrain = calculateStrain();
		if(nstrain > strain)
		{
			//printf("Reversing bad adjustment\n");
			points[i] += residual[i];
			adjust_rate = true;
		}
		else
		{
			strain = nstrain;
		}
	}
#endif
	if(adjust_rate) rate*= 0.7;

	delete[] residual;
}

/**
 *  Find the a local minimum in strain offset from points along given direction
 *
 *  Precondition: a < b < c and strain(a) > strain(b), strain(c) > strain(b)
 */
double MDS::minimize(ProjectedTree* direction,double a,double strain_a, double b,double strain_b, double c,double strain_c)
{
	const double golden_mean = (3-sqrt(5))/2;
	double x;
	double strain_x;
	
	//base case for recursion
	if(c-a < 0.0001) return b;
	
	/*
	printf("%.5f(%.2f)\t",a,strain_a);
	printf("%.5f(%.2f)\t",b,strain_b);
	printf("%.5f(%.2f)\n",c,strain_c);
	*/
	
	
	//determine new test point;
	if((b-a) > (c-b))
		x = b - golden_mean*(b-a);
	else
		x = b + golden_mean*(c-b);
		
	//evaluate new test point
	ProjectedTree* test = new ProjectedTree[ntrees];

	for(int i=0;i<ntrees;i++)
	{
		test[i]= ProjectedTree(MDS_DIM);
		
		test[i] = points[i] + direction[i] * x;
	}
	
	strain_x = calculateStrain(test);

	
	delete[] test;
	
	//recurse
	if((b-a) > (c-b))
	{
		if(strain_x > strain_b)
			return minimize(direction,x,strain_x,b,strain_b,c,strain_c);
		else
			return minimize(direction,a,strain_a,x,strain_x,b,strain_b);
	}
	else
	{
		if(strain_x > strain_b)
			return minimize(direction,a,strain_a,b,strain_b,x,strain_x);
		else
			return minimize(direction,b,strain_b,x,strain_x,c,strain_c);
	}
}

/**
 *  Calculate the normalized strain (the RMS error from current distances and target distances) on current points
 */
double MDS::calculateStrain()
{
	return calculateStrain(points);
}

/**
 *  Calculate the normalized strain (the RMS error from current distances and target distances)
 */
double MDS::calculateStrain(ProjectedTree*& p)
{
	ProjectedTree error(MDS_DIM);
	double strain = 0;
	double target_dist = 0;
	for(int i=0;i<ntrees;i++)
	for(int j=i+1;j<ntrees;j++)
	{
		error = p[j] - p[i];
		double e;
		e = target[i][j] - error.length();
		target_dist += target[i][j];
		strain += e*e;
	}
	strain/=target_dist;
//	strain = sqrt(strain);
//	strain /= ntrees;
	return strain;
}
