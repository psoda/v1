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
#include "GradDescInstr.h"
#include "GradientDescent.h"
#include "TreeField.h"
#include "ParticleTree.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"

using namespace std;

GradDescInstr::GradDescInstr() : BuiltInCommand() {
  initDefaultValue("start", "stepwise", "Where should the starting trees for the Gradient Descent Search come from");
  initDefaultValue("continue", 10,"How many times should the search continue to look after no improvement");
  setDescription("Perform a gradient descent search for trees.");

/*
  initDefaultValue("criterion", "parsimony");
  initDefaultValue("nreps", 1);
  initDefaultValue("swap", "TBR", "What method should be used for swaping");
  initDefaultValue("wantRecursion", "FALSE");
  
  initDefaultValue("iterations", INT_MAX);
*/
}

GradDescInstr::~GradDescInstr() {
  return;
}

void GradDescInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);	
}

void GradDescInstr::execute(Environment* baseEnv __attribute__((unused))) {

int NUM_POINTS;
GradientDescentSearch gds;
ParticleTree** points;
double* x;
double* y;
double* z;
int i;


double best_score = -2;
int best_tree = -1;

if(baseEnv->lookupString("start") == "stepwise")
{
	NUM_POINTS = 10;
	
	points = (ParticleTree**)malloc(sizeof(ParticleTree*)*NUM_POINTS);

	x = (double*)malloc(sizeof(double)*NUM_POINTS);
	y = (double*)malloc(sizeof(double)*NUM_POINTS);
	z = (double*)malloc(sizeof(double)*NUM_POINTS);
	
	for(i=0;i<NUM_POINTS;i++)
	{
		points[i] = new ParticleTree(&gds);
		points[i]->initStepwise();
		if((best_score < -1)||(best_score > points[i]->score()))
		x[i] = y[i] = z[i] = 0.0;
		{
			best_score = points[i]->score();
			best_tree = i;
		}
	}
}
else if(baseEnv->lookupString("start") == "current")
{

	QTreeRepository* treeRepository = Interpreter::getInstance()->qtreeRepository();
	
	NUM_POINTS = treeRepository->numTrees();
	
	points = (ParticleTree**)malloc(sizeof(ParticleTree*)*NUM_POINTS);

	x = (double*)malloc(sizeof(double)*NUM_POINTS);
	y = (double*)malloc(sizeof(double)*NUM_POINTS);
	z = (double*)malloc(sizeof(double)*NUM_POINTS);
	
	for(i=0;i<NUM_POINTS;i++)
	{
		QTree* qt = treeRepository->popTree();
		points[i] = new ParticleTree(&gds);
		points[i]->initTree(qt);
		if((best_score < -1)||(best_score > points[i]->score()))
		x[i] = y[i] = z[i] = 0.0;
		{
			best_score = points[i]->score();
			best_tree = i;
		}
		treeRepository->addTree(qt);
	}
}
else
{
	PsodaPrinter::getInstance()->write("Invalid start for GradDesc, skipping instruction\n");
	return;
}


double old_best = best_score + 10;
double cur_score;
int cur_best_tree = best_tree;
printf("Score %.1f\n",best_score);
int CONTINUES = atoi(baseEnv->lookupString("continue").c_str());
while(CONTINUES > 0)
{
	if(old_best - best_score < 0.1) CONTINUES--;
	printf("%.1f - %.1f = %.2f (%d)\n",old_best,best_score,old_best-best_score,CONTINUES);
	old_best = best_score;
	cur_best_tree = best_tree;
	for(i=0;i<NUM_POINTS;i++)
	{
		if(cur_best_tree!=i)
		{
			x[i] += points[i]->x() - points[cur_best_tree]->x();
			y[i] += points[i]->y() - points[cur_best_tree]->y();
			z[i] += points[i]->z() - points[cur_best_tree]->z();
			x[i] /=2;
			y[i] /=2;
			z[i] /=2;
			
			printf("Moving %d -> %d (%f,%f,%f)\n",i,cur_best_tree,x[i],y[i],z[i]);
			if(!points[i]->move(x[i],y[i],z[i]))
			{
				printf("\tMove failed, trying to conflate with best tree so far\n");
				points[i]->consenseMove(points[cur_best_tree],Interpreter::getInstance()->dataset()->ntaxa());
			}
		}
		else
		{
			//explore
			
			points[i]->explore();
		}
		
		
		cur_score = points[i]->score();
		//printf("best: %.1f\tcur: %.1f\n",best_score,cur_score); 
		if(cur_score < best_score)
		{
			old_best = best_score;
			best_score = cur_score;
			best_tree = i;
			printf("Score %.1f\n",best_score);
		}
	
	}
}

Interpreter::getInstance()->qtreeRepository()->removeAll();
QTree* qt = points[best_tree]->tree();
Interpreter::getInstance()->qtreeRepository()->addTree(qt);
/*for(i=0;i<NUM_POINTS;i++)
{
	if(i!=best_tree)
	{
		qt = points[i]->tree();
		Interpreter::getInstance()->qtreeRepository()->addTree(qt);	
	}
}*/



}

string GradDescInstr::getName() const {
  return "GradientDescent";
}
