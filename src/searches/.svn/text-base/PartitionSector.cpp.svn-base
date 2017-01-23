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
#include "PartitionSector.h"
#include "Interpreter.h"
#include "InteractiveInstr.h"
#include "NewickTreeParser.h"
#include <math.h>

#define MAX_TREE_STRING 65000

/*****************************************************
 *                                                   *
 *   Partition Sector Tree                           *
 *                                                   *
 *****************************************************/

PartitionSectorTree::PartitionSectorTree(QTree* tree,Hypersphere* nMap,PartitionSectorSet* nPSS)
:
 mRoot(NULL),
 map(nMap),
 pss(nPSS),
 mNodeInfoList()
{
	PartitionSector::map = nMap;
	//make PSNode copy of QTree
	copy(mRoot,tree->root());
	copy(mRoot->external(),tree->root()->external());
}

void PartitionSectorTree::copy(PSNode*& dest,QNode* root)
{
	PSNodeInfo* info;
	if(root)
	{
		info = new PSNodeInfo();
		mNodeInfoList.push_back(info);
		dest = new PSNode(info,root,root->nodeInfo()->leaf());
		info->node=dest;
		
		if(root->nodeInfo()->leaf())
		{
			dest->internal1()=dest;
		}
		else
		{
			copy(dest->child1(),root->child1());
			dest->child1()->external()=dest->internal1();
			
			copy(dest->child2(),root->child2());
			dest->child2()->external()=dest->internal2();
			
		}
	}
	return;
}

void PartitionSectorTree::preprocess()
{
	printf("\tPostorder pass\n");
	post_order(mRoot);
	post_order(mRoot->external());
	
	mRoot->info()->pre = mRoot->external()->info()->post;
	mRoot->external()->info()->pre = mRoot->info()->post;
	
	printf("\tPreorder pass\n");
	printf("\t\tPart 1\n");
	pre_order(mRoot->child1(),mRoot->child2());
	pre_order(mRoot->child2(),mRoot->child1());

	printf("\t\tPart 2\n");	
	pre_order(mRoot->external()->child1(),mRoot->external()->child2());
	pre_order(mRoot->external()->child2(),mRoot->external()->child1());
	
}

PartitionSector PartitionSectorTree::post_order(PSNode* root)
{
	if(root->leaf())
	{
		return PartitionSector(Interpreter::getInstance()->dataset()->ntaxa(),root->taxaNum());
	}
	else
	{
		PartitionSector child1 = post_order(root->child1());
		PartitionSector child2 = post_order(root->child2());
		root->info()->post = child1+child2;
		return root->info()->post;
	}
}

void PartitionSectorTree::pre_order(PSNode* root, PSNode* sibling)
{
	
	
	root->info()->pre = root->external()->info()->pre + sibling->info()->post;
	if(!root->leaf())
	{
		pre_order(root->child1(),root->child2());
		pre_order(root->child2(),root->child1());
	}
}

char* buildTreeString(Dataset* cur, Dataset* sector, char* sectorString, char* treeStr)
{
	char newtreeStr[MAX_TREE_STRING];
	char taxonStr[10];
	memset(newtreeStr,0,MAX_TREE_STRING);
	memset(taxonStr,0,10);
	
	int i=0;
	int j=0;
	int k=-1;
	int taxon;
	int new_taxon;
	while(treeStr[i]!='\0')
	{
		if(treeStr[i]=='('||treeStr[i]==')'||treeStr[i]==',')
		{
			if(k!=-1)
			{
					taxon = atoi(taxonStr);
					memset(taxonStr,0,10);
					new_taxon = cur->getTaxonNumber(sector->getTaxonName(taxon));
					if(new_taxon != -1) //replace number in partial set with number from full dataset
					{
						sprintf(taxonStr,"%d",new_taxon);
						k=0;
						while(taxonStr[k]!='\0')
						{
							newtreeStr[j] = taxonStr[k];
							j++;
							k++;
						}
					}
					else //this is the untouched portion of the tree, replace with treestring for that portion
					{
						k=0;
						while(sectorString[k]!='\0')
						{
							newtreeStr[j] = sectorString[k];
							j++;
							k++;
						}
					}
					memset(taxonStr,0,10);
					k = -1;
			}
			newtreeStr[j] = treeStr[i];
			j++;
		}
		else
		{
			k++;
			taxonStr[k] = treeStr[i];
		}
		i++;
	}
	return strdup(newtreeStr); 
}

QTree* PartitionSectorTree::refine()
{
	//choose a partition
	PSNodeInfo* best = NULL;
	double best_score = DBL_MAX;
	for(list<PSNodeInfo*>::const_iterator iter = mNodeInfoList.begin();iter !=mNodeInfoList.end();iter++)
	{
		//Insure that the size is appropriate
		if((*iter)->pre.size() < 50)
		if((*iter)->pre.size() > 10)
		{
			double cur_score = pss->overlap((*iter)->pre);
			if(cur_score < best_score)
			{
				cur_score = best_score;
				best = *iter;
			}
		}
	}
	//we will examine this sector so add it to the set of examined sectors
	pss->add(best->pre);
	
	//create dataset for sector
	Dataset sectorData;
	Dataset* curDataset = Interpreter::getInstance()->dataset();
	sectorData.datatype() = Dataset::DNA_DATATYPE;
	sectorData.dataformat() = Dataset::ALIGNED_DATAFORMAT;
	sectorData.gapchar()  ='-';
	sectorData.missingchar() = '-';
	
	//add taxa names
	for(int i=0;i<curDataset->ntaxa();i++)
	{
		if(best->pre.contains(i))
			sectorData.addName(curDataset->getTaxonName(i));
	}
	sectorData.addName("RemainingTree");
	
	//add taxa data
	for(int i=0;i<curDataset->ntaxa();i++)
	{
		if(best->pre.contains(i))
			sectorData.addCharacters(curDataset->getCharacters(i,false),-1);
	}
	sectorData.addCharacters(best->getCharacters(),-1);
	
	//set Interpreter to use new dataset and repository
	Interpreter::getInstance()->installDataset(&sectorData);
	QTreeRepository repo;
	QTreeRepository* oldRepo = Interpreter::getInstance()->qtreeRepository();
	Interpreter::getInstance()->qtreeRepository() = &repo;
	
	//run TBR search on sector
	
	InteractiveInstr::executeCode("hsearch start=stepwise swap=tbr;");

	//revert to global dataset and repository
	Interpreter::getInstance()->installDataset(curDataset);
	Interpreter::getInstance()->qtreeRepository() = oldRepo;
	
	//build new tree
	QTree* newTree = repo.getTree();
	char* treeStr = buildTreeString(curDataset,&sectorData,best->treeStr(),newTree->treeStr());
	NewickTreeParser ntp;
	QTree* qt = ntp.parseBuffer(treeStr,strlen(treeStr),false);
	qt->setTreeStr(treeStr);
	return qt;
}

/*****************************************************
 *                                                   *
 *   PSNodeInfo                                      *
 *                                                   *
 *****************************************************/

PSNodeInfo::PSNodeInfo():
pre(Interpreter::getInstance()->dataset()->ntaxa(),0),
post(Interpreter::getInstance()->dataset()->ntaxa(),0),
node(NULL)
{
}

PSNodeInfo::~PSNodeInfo()
{
}

char* PSNodeInfo::getCharacters()
{
	return NULL;
}
char* PSNodeInfo::treeStr()
{
	char tempStr[10000];
	partialString(node->node()->nodeInfo()->node()->external(), tempStr);
	return strdup(tempStr);
}

void PSNodeInfo::partialString(QNode* root,char* buf)
{
	if(root->nodeInfo()->leaf())
	{
		sprintf(buf,"%d",root->nodeInfo()->nodeIndex());
	}
	else
	{
		char child1Str[10000];
		char child2Str[10000];
		partialString(root->child1(),child1Str);
		partialString(root->child2(),child2Str);
		sprintf(buf,"(%s,%s)",child1Str,child2Str);
	}
}

/*****************************************************
 *                                                   *
 *   PSNode                                          *
 *                                                   *
 *****************************************************/
PSNode::PSNode(PSNodeInfo* info,QNode* node,bool isLeaf)
:
 mInternal(this),
 mExternal(NULL),
 mNodeInfo(info),
 mQNode(node)
{
	if(!isLeaf)
	{
		mInternal = new PSNode(info,node->child1());
		mInternal->mInternal = new PSNode(info,node->child2());
		mInternal->mInternal->mInternal = this;
		
	}
}

PSNode::PSNode(PSNodeInfo* info,QNode* node)
:
 mInternal(this),
 mExternal(NULL),
 mNodeInfo(info),
 mQNode(node)
{
}


PSNode*& PSNode::external()           { return mExternal; }
PSNode* PSNode::external() const      { return mExternal; }
PSNode*& PSNode::internal1()          { return mInternal; }
PSNode* PSNode::internal1() const     { return mInternal; }
PSNode*& PSNode::internal2()          { return mInternal->mInternal; }
PSNode* PSNode::internal2() const     { return mInternal->mInternal; }
PSNode*& PSNode::child1()         { return mInternal->mExternal; }
PSNode* PSNode::child1() const    { return mInternal->mExternal; }
PSNode*& PSNode::child2()         { return mInternal->mInternal->mExternal; }
PSNode* PSNode::child2() const    { return mInternal->mInternal->mExternal; }
PSNodeInfo* PSNode::info() const {return mNodeInfo;}
QNode* PSNode::node() const {return mQNode;}
bool PSNode::leaf() {return mQNode->nodeInfo()->leaf();}
int PSNode::taxaNum() const {return mQNode->nodeInfo()->nodeIndex();}


/*****************************************************
 *                                                   *
 *   PartitionSector                                 *
 *                                                   *
 *****************************************************/

Hypersphere* PartitionSector::map;

PartitionSector::PartitionSector(int numTaxa,int taxa):position(3),mSize(0),part(numTaxa,taxa%numTaxa),nTaxa(numTaxa)
{}

PartitionSector::PartitionSector(const PartitionSector& a):position(a.position),mSize(a.mSize),part(a.part),nTaxa(a.nTaxa)
{}
	
PartitionSector & PartitionSector::operator=(const PartitionSector& rhs)
{
	if(this != &rhs)
	{
		position = rhs.position;
		mSize = rhs.mSize;
		part = rhs.part;
		nTaxa = rhs.nTaxa;
	}
	return *this;
}

PartitionSector & PartitionSector::operator+=(const PartitionSector& rhs)
{
	mSize += rhs.mSize + 1;
	
	printf("Combining:\n        ");
	rhs.part.print();
	printf("\n        ");
	part.print();
	printf("\n");
	
	part += rhs.part;
	
	printf("Result: ");
	part.print();
	printf("\n");
	
	position += rhs.position;
	//add movement from new partition
	position += map->lookupPartition(part);
	return *this;
}

PartitionSector PartitionSector::operator+(const PartitionSector& other) const
{
	PartitionSector result(*this);
	result += other;
	return result;
}


int PartitionSector::size() const
{
	return mSize;
}

bool PartitionSector::contains(const int taxon) const
{
	return part(taxon);
}

/* returns the volume of overlap*/	
double PartitionSector::overlap(const PartitionSector& other) const
{
	double volume = 1.0;
	for(int i=0;i<3;i++)
	{
		double dim_overlap = ((nTaxa - 3 - mSize) + (other.nTaxa - 3 - other.mSize));
		double dim_dist = fabs(position(i) - other.position(i));
		dim_overlap -= dim_dist;
		if(dim_overlap < 0.0) dim_overlap = 0.0;
		volume *= dim_overlap;
	}
	return volume / (mSize * mSize * mSize * 8); //normalize for volume of partition
}

/*****************************************************
 *                                                   *
 *   PartitionSectorSet                              *
 *                                                   *
 *****************************************************/
 
 PartitionSectorSet::PartitionSectorSet():sectors()
 {}
 
 double PartitionSectorSet::overlap(const PartitionSector& ps) const
 {
	double overlap = 0.0;
	for(list<PartitionSector>::const_iterator iter = sectors.begin();iter!=sectors.end();iter++)
	{
		overlap += ps.overlap(*iter);
	}
	return overlap;
 }
 
 void PartitionSectorSet::add(const PartitionSector& ps)
 {
	sectors.push_front(ps);
 }