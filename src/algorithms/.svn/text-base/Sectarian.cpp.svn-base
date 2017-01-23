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
#include "Sectarian.h"
#include "QTBR.h"
#include "Interpreter.h"
#include "QNode.h"
#include "QTree.h"
#include "Dataset.h" 
#include "QRetainedResultSearch.h"
#include "NewickTreeParser.h"

using namespace std;

#define MAX_CHILD 4
#define MAX_RECURSION 1
#define MAXTREES 1
#define MAXEQUAL 0

#undef VERBOSE_SECTARIAN

#ifdef VERBOSE_SECTARIAN
#undef VERBOSE_SECTARIAN_NODEDATA
#undef VERBOSE_SECTARIAN_SCORING
#else
#undef VERBOSE_SECTARIAN_NODEDATA
#undef VERBOSE_SECTARIAN_SCORING
#endif

SectarianSearch::SectarianSearch()
{
    mRearrangements = 0;
	fast_return = false;
}

SectarianSearch::~SectarianSearch()
{
}

/**
 * Split the tree, then try all of the possible join points on both subtrees.
 * Returns an array of tree pointers of all the most optimal trees found
 */
void SectarianSearch::rearrange(QTree* startTree, EvaluatorBase  *eval, int numtrees __attribute__((unused)) , int numequal __attribute__((unused)), double bestScore, bool saveOrigTree __attribute__((unused)), list<QTree*>& treeList)
{
	//printf("In Sectarian Search\n");
	QNodeInfo* node;
	NewickTreeParser ntp;
    list<char*> treeStrList;
	if(startTree->isSplit())
	{
		//Join the tree together
		startTree->join();
	}
	//printf("Tree not split or rejoined\n");
	// Build the preprocess data
	eval->qpreProcess(startTree);
	//printf("Tree preprocessed\n");
	startTree->initNodeInfoIter();
	//printf("NodeInfoIter initialized\n");
	bool improved = false;
	double best_score = bestScore;
	treeStrList.clear();
	while((node = startTree->nextNodeInfo()) != NULL)
	{

#ifdef VERBOSE_SECTARIAN_NODEDATA
		printf("Considering node ");
		node->print();
#endif
		if(!node->leaf())
		{
			if(!(node->node()->child1()->nodeInfo()->leaf()||node->node()->child2()->nodeInfo()->leaf()))
			{
				//refine this branch
				improved |= refine(node->nodeData()->siteData(PSDI_PRE),(int)node->nodeData()->score(PSDI_PRE),
			       node->node()->child1()->child1()->nodeInfo()->nodeData()->siteData(PSDI_POST),(int)node->node()->child1()->child1()->nodeInfo()->nodeData()->score(PSDI_POST),
			       node->node()->child1()->child2()->nodeInfo()->nodeData()->siteData(PSDI_POST),(int)node->node()->child1()->child2()->nodeInfo()->nodeData()->score(PSDI_POST),
			       node->node()->child2()->child1()->nodeInfo()->nodeData()->siteData(PSDI_POST),(int)node->node()->child2()->child1()->nodeInfo()->nodeData()->score(PSDI_POST),
			       node->node()->child2()->child2()->nodeInfo()->nodeData()->siteData(PSDI_POST),(int)node->node()->child2()->child2()->nodeInfo()->nodeData()->score(PSDI_POST),
				   startTree->nchars(),startTree->weights(),node,treeStrList,best_score);
				node->visited()=true;
				if(improved&&fast_return)
					break;  //break out of loop
			}
		}
	}
	if(improved)
	{
#ifdef VERBOSE_SECTARIAN
		printf("%d better trees found scored %.1f\n",(int)treeList.size(),best_score);
#endif
		for(list<char*>::iterator iter = treeStrList.begin();iter!=treeStrList.end();iter++)
		{
			QTree* qt = ntp.parseBuffer(*iter,strlen(*iter),false);
			qt->setScore(best_score);
			qt->setTreeStr(*iter);
			treeList.push_front(qt);
			free(*iter);
		}
		treeStrList.clear();
	}
	else
	{
#ifdef VERBOSE_SECTARIAN
		printf("No improvements found\n");
		printf("Returning Original Tree\n");
#endif
	
		startTree->updateTreeString();
		
		
		
		QTree * qt = ntp.parseBuffer(startTree->treeStr(), strlen(startTree->treeStr()), false);
		double startScore = eval->qscoreTree(qt);
		
		qt->setScore(startScore);
		qt->visited() = true;
		qt->setTreeStr(startTree->treeStr());
		treeList.push_front(qt);
		
		for(list<char*>::iterator iter = treeStrList.begin();iter!=treeStrList.end();iter++)
		{
			qt = ntp.parseBuffer(*iter,strlen(*iter),false);
			qt->setScore(startScore);
			qt->setTreeStr(*iter);
			treeList.push_back(qt);
			free(*iter);
		}
	}
	startTree->minimize();
	return;
}

const int MAX_STRING_SIZE = 10000;

void SectarianSearch::buildTreeString(QNode* root,char* root_str)
{
	char child1[MAX_STRING_SIZE];
	char child2[MAX_STRING_SIZE];
	if(root->nodeInfo()->leaf())
	{
		sprintf(root_str,"%d",root->nodeInfo()->nodeIndex()+1);
		//printf("root_str = %s\n",root_str);
		return;
	}
	memset(child1,0,MAX_STRING_SIZE);
	memset(child2,0,MAX_STRING_SIZE);
	buildTreeString(root->child1(),child1);
	buildTreeString(root->child2(),child2);
	sprintf(root_str,"(%s,%s)",child1,child2);
	//printf("root_str = %s\n",root_str);
	return;
}

/**
 * refine takes the five sequences around a given sector and finds the best tree to resolve them
 */
bool SectarianSearch::refine(char* A,int A_score, char* B,int B_score, char* C,int C_score, char* D,int D_score, char* E,int E_score,int nchar, int* weights, QNodeInfo* node, list<char*>& treeList,double& best_score)
{
	int i,j,k,m,n;
	
	//move parameters into an array
	char* seq[5];
	int score[5];
	char partial_str[5][MAX_STRING_SIZE];
	char* TreeString = NULL;
	int TreeStringSize = 0;
	
	seq[0] = A;
	seq[1] = B;
	seq[2] = C;
	seq[3] = D;
	seq[4] = E;
	
	score[0] = A_score;
	score[1] = B_score;
	score[2] = C_score;
	score[3] = D_score;
	score[4] = E_score;
	
	int all_scores = A_score + B_score + C_score + D_score + E_score;
#ifdef VERBOSE_SECTARIAN_SCORING
	printf("Sum of leaf scores - %d\n",all_scores);
#endif
	
	//combine sequences into all pairs
	char* combined[5][5];
	int combined_score[5][5];
	
	for(i=0;i<5;i++)
	for(j=i+1;j<5;j++)
	{
		combined[i][j] = (char*)malloc(sizeof(char)*nchar);
		combined_score[i][j] = combine(combined[i][j],seq[i],seq[j],nchar,weights);
	}
	
	
	
	//consider trees with topology (i,(j,k),(m,n))
	double cur_score;
	char* temp1=(char*)malloc(sizeof(char)*nchar);
	char* temp2=(char*)malloc(sizeof(char)*nchar);
	
	bool TreeStringsComputed = false;
	bool better_found = false;
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			if(j!=i)
			{
				for(k=j+1;k<5;k++)
				{
					if(k!=i)
					{
						for(m=0;m<5;m++)
						{
							if(m!=i&&m!=j&&m!=k)
							{
								for(n=m+1;n<5;n++)
								{
									if(n!=i&&n!=j&&n!=k)
									{
										if((i==0)&&(j==1)&&(k==2)&&(m==3)&&(n==4))
										{
											continue; //original topology
										}
										mRearrangements++;
#ifdef VERBOSE_SECTARIAN_SCORING
										printf("Consider (%c,(%c,%c),(%c,%c))\n",'A'+i,'A'+j,'A'+k,'A'+m,'A'+n);
										printf("\tPartial score = %d + %d + %d\n",all_scores,combined_score[j][k],combined_score[m][n]);
#endif
										//this is a unique topology, consider its score
										cur_score = all_scores;
										cur_score += combined_score[j][k];
										cur_score += combined_score[m][n];
#ifdef VERBOSE_SECTARIAN_SCORING
										printf("\tPartial score = %.1f",cur_score);
										if(cur_score < best_score)
											printf(" <  %.1f continue\n", best_score);
										else
											printf(" >= %.1f\n", best_score);
#endif
										if(cur_score < best_score)  //if not this topology is not better, don't bother scoring
										{
											cur_score += combine(temp1,combined[j][k],combined[m][n],nchar,weights);
											cur_score += combine(temp2,temp1,seq[i],nchar,weights);
											
#ifdef VERBOSE_SECTARIAN_SCORING
											printf("\tFinal score = %.1f",cur_score);
											if(cur_score <= best_score)
												printf(" <  %.1f save\n", best_score);
											else
												printf(" >= %.1f\n", best_score);
#endif
											if(cur_score <= best_score)
											{
												if(cur_score < best_score)
												{
													for(list<char*>::iterator iter = treeList.begin();iter!=treeList.end();iter++)
														free(*iter);
													treeList.clear();
													
													best_score = cur_score;
#ifdef VERBOSE_SECTARIAN
												printf("Better Tree found (%c,(%c,%c),(%c,%c)) - %.1f\n",'A'+i,'A'+j,'A'+k,'A'+m,'A'+n,best_score);
#endif
												better_found = true;
												}
												//save tree

												if(!TreeStringsComputed)
												{
													//build partial tree strings
													for(int k=0;k<5;k++)
													{
														memset(partial_str[k],0,MAX_STRING_SIZE);
													}

													buildTreeString(node->node()->external(),partial_str[0]);
													buildTreeString(node->node()->child1()->child1(),partial_str[1]);
													buildTreeString(node->node()->child1()->child2(),partial_str[2]);
													buildTreeString(node->node()->child2()->child1(),partial_str[3]);
													buildTreeString(node->node()->child2()->child2(),partial_str[4]);
																			
													TreeStringSize = 20; //Padding for parenthesis
													TreeStringSize += strlen(partial_str[0]);
													TreeStringSize += strlen(partial_str[1]);
													TreeStringSize += strlen(partial_str[2]);
													TreeStringSize += strlen(partial_str[3]);
													TreeStringSize += strlen(partial_str[4]);
																										
													TreeString = (char*)malloc(sizeof(char)*TreeStringSize);
												}
												sprintf(TreeString,"(%s,(%s,%s),(%s,%s))",partial_str[i],partial_str[j],partial_str[k],partial_str[m],partial_str[n]);
												//printf("%s\n",TreeString);
												treeList.push_front(strdup(TreeString));
												
												if(fast_return)
												{
													goto cleanup; //goto rather than return so that the clean up code can be in just one place
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
cleanup:	
	//free data
	for(i=0;i<5;i++)
	for(j=i+1;j<5;j++)
	{
		free(combined[i][j]);
	}
	free(temp1);
	free(temp2);
	if(TreeStringsComputed)
	{
		free(TreeString);
	}
	
	return better_found;
}

int SectarianSearch::combine(char* dest, char* src1, char* src2,int nchar, int* weights)
{

	int differences = 0;


  for (int i = 0; i < nchar; i++)
  {
	  if ( ! ( *dest = *src1 & *src2 ) )
      {
          differences += weights[i] ;
          //differences ++;
		  *dest = *src1 | *src2 ;
	  }
	  dest++;
	  src1++;
	  src2++;
  }

  return differences ;
}
