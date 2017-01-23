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
/*
 *  PartitionTree.cpp
 *  psoda
 *
 *  Created by Kenneth Sundberg on 9/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "PartitionTree.h"
#include "PsodaError.h"
#include "Interpreter.h"

/**
 * Default Constructor for PartitionTree
 */

PartitionTree::PartitionTree(int nTaxa):mNPartitions(0),mNtaxa(nTaxa),mPartitionSet(),ufinfo(NULL),dont_care(NULL),partialTreeString(NULL)
{
}	

/**
 * Constructor of PartitionTree from a QTree
 */

PartitionTree::PartitionTree(QTree* qt):mNPartitions(0),mNtaxa(Interpreter::getInstance()->dataset()->ntaxa()),mPartitionSet(),ufinfo(NULL),dont_care(NULL),partialTreeString(NULL)
{
	dont_care = (bool*)malloc(sizeof(bool)*mNtaxa);
	for(int i=0;i<mNtaxa;i++)
		dont_care[i] = true;
	Partition root = AddPartition(qt->root());
	add(root,qt->root()->branchLength());
	AddPartition(qt->root()->external());
	
	/*Modify mPartitions to double count partitions with the dont cares on either side of the partition*/
	std::list<Partition> doublePartitions;
	
	//copy mPartitions
	for(std::list<Partition>::const_iterator c_iter = begin();c_iter!=end();c_iter++)
	{
		doublePartitions.push_back(*c_iter);
	}
	mPartitions.clear();
	//Move copy back into mPartitions making duplicates as we go
	for(std::list<Partition>::iterator iter = doublePartitions.begin();iter!=doublePartitions.end();iter++)
	{
		//set all don't cares to true
		Partition true_dont_cares = *iter;
		true_dont_cares.setDontCares(dont_care,true);
		mPartitions.push_back(true_dont_cares);
		
		Partition false_dont_cares = *iter;
		false_dont_cares.setDontCares(dont_care,false);
		mPartitions.push_back(false_dont_cares);
		
	}
}	


PartitionTree::~PartitionTree()
{
	if(ufinfo)
	{
		free(ufinfo);
		ufinfo = NULL;
	}
	if(partialTreeString)
	{
	
		for(int i=0;i<mNtaxa;i++)
			if(partialTreeString[i])
				free(partialTreeString[i]);
		free(partialTreeString);
	
	}
	if(dont_care) free(dont_care);
	
}

/**
 * AddPartition Adds the partitions associated with a QNode in postorder fashion
 */
Partition PartitionTree::AddPartition(QNode* root)
{
	if(root->nodeInfo()->leaf())
	{
		int leaf_index = root->nodeInfo()->nodeIndex();
		dont_care[leaf_index] = false;
		return Partition(mNtaxa,leaf_index);
	}
	Partition left = AddPartition(root->child1());
	Partition right = AddPartition(root->child2());
	Partition center = left + right;
	add(center,root->branchLength());
	return center;
}

/**
 * add() adds a partition to the tree.
 *
 * The add method does not insure that the partition is compatible with current partitions in the tree.
 */
void PartitionTree::add(const Partition& p,double branch_length)
{
	Partition part(p);
	part.length() = branch_length;
	//mPartitionSet.push(part);
	mPartitions.push_back(part);
	mNPartitions++;
}

/**
 * begin() provides a constant iterator to the partition set for this tree
 */

std::list<Partition>::const_iterator PartitionTree::begin() const
{
	return mPartitions.begin();
}


/**
 * end() provides a constant iterator to the end of the partition set for this tree
 */

std::list<Partition>::const_iterator PartitionTree::end() const
{
	return mPartitions.end();
}


/**
 * treeString() builds and returns a newick string representation of the tree
 *
 * Note that the treeString method creates a new string which must be freed by the caller
 */
char* PartitionTree::treeString()
{
	int i;
	Partition p(mNtaxa,1);  //This will be overwritten, the node id in the constructor is arbitrary
	//Initialize union-find info
	if(ufinfo)
	{	
		free(ufinfo);
		ufinfo = NULL;
	}
	ufinfo = (int*)malloc(sizeof(int)*mNtaxa);
	for(i=0;i<mNtaxa;i++)
		ufinfo[i] = i;
	//Initialize treestring info
	
	//Remove old TreeString data
	//TODO this is a memory leak
	/*
	if(partialTreeString)
	{
		for(i=0;i<mNtaxa;i++)
			if(partialTreeString[i])
			{
				free(partialTreeString[i]);
				partialTreeString[i] = NULL;
			}
		free(partialTreeString);
		partialTreeString = NULL;
	}*/
	
	partialTreeString = (char**)malloc(sizeof(char*)*mNtaxa);
	for(i=0;i<mNtaxa;i++)
	{
		partialTreeString[i] = (char*)malloc(sizeof(char)*10);
		sprintf(partialTreeString[i],"%d",i+1);  //one based taxa numbers in tree strings
	}	
	
	for(std::list<Partition>::const_iterator part_iter = begin();part_iter!=end();part_iter++)
	{
		mPartitionSet.push(*part_iter);
	}
	
	// Consider all partitions in ascending order, joining together treestrings as needed
	while(!mPartitionSet.empty())
	{
		//Get the next partition
		p=mPartitionSet.top();
		//join trees in partition
		int prev = -1;
		int cur = -1;
		for(i=0;i<mNtaxa;i++)
		{
			if(p(i))
			{
				if(prev == -1)
					prev = i;
				else if(cur == -1)
				{
					cur = i;
					join(prev,cur);  //concatenates tree strings with a comma if needed
				}
				else
				{
					prev = cur;
					cur = i;
					join(prev,cur);  //concatenates tree strings with a comma if needed
				}
			}
		}
		if(cur!=-1)
			partialTreeString[find(cur)] = treestringparens(partialTreeString[find(cur)],p.length());  //add parenthesis around taxa joined by this partition
		mPartitionSet.pop();
	}
	//Join all remaining partials
	for(i=1;i<mNtaxa;i++)
		join(i-1,i);
	
	partialTreeString[find(0)] = treestringparens(partialTreeString[find(0)]);  //add parenthesis around entire tree
				
		
	
	return partialTreeString[find(0)];
}

/**
 * join() joins two partial trees containing the two given taxa
 */
 
void PartitionTree::join(int a,int b,bool treestring)
{
	if(find(a) == find(b))
		return;
	if(treestring)
	{
		partialTreeString[find(a)] = treestringjoin(partialTreeString[find(a)],partialTreeString[find(b)]); //join tree strings
		partialTreeString[find(b)] = NULL; //this string is no longer vaild
	}
	ufinfo[b] = find(a); //join two trees;
}


/**
 * find() returns the partial tree number containing the given taxa
 */
 
int PartitionTree::find(int target)
{
	if(ufinfo[target] == target) 
	{
		return target;
	}
	else
	{
		//do path compression
		ufinfo[target] = find(ufinfo[target]);
		return ufinfo[target];
	}
}

/**
 * treestringjoin takes two strings and returns their concatenation with an interior comma
 */
char* PartitionTree::treestringjoin(char* a,char* b)
{
	char* tree_string;
	int length;
	
	length = strlen(a) + strlen(b) + 2; //one for NULL character and the other for the comma
	tree_string = (char*) malloc(sizeof(char) * length);
	sprintf(tree_string,"%s,%s",a,b);
	free(a);
	free(b);
	return tree_string;
	
}

/**
 * treestringparens adds parenthesis around a given clade in a treestring
 */
char* PartitionTree::treestringparens(char* s,double branch_length)
{
	char* tree_string;
	int length;
	if(branch_length < 0.0)
	{
	length = strlen(s) + 3; //one for NULL character and the other rwo for the parens
	tree_string = (char*) malloc(sizeof(char) * length);
	sprintf(tree_string,"(%s)",s);
	free(s);
	}
	else
	{
	char buf[10000];
	memset(buf,0,10000);
	sprintf(buf,"(%s):%f",s,branch_length);
	buf[9999] = '\0';
	length = strlen(buf)+2; //one for NULL character and the other rwo for the parens
	tree_string = (char*) malloc(sizeof(char) * length);
	sprintf(tree_string,"(%s):%f",s,branch_length);
	free(s);
	}
	return tree_string;
}

/**
 * isCompatible returns true if the given partition is not in conflict with any partition currently in the tree
 */
bool PartitionTree::isCompatible(const Partition& test) const
{
	for(std::list<Partition>::const_iterator i = begin();i!= end();i++)
		if(!i->isCompatible(test))
			return false; //This branch conflicts and should not be added
		else if(*i==test)
			return false; //This branch is already in the tree and should not be duplicated
	return true;
}

/**
 * maybeCompatible returns true if the given partial partition is not in conflict with any partition currently in the tree
 */
bool PartitionTree::maybeCompatible(int ntax,bool* included) const
{
	/*printf("maybeCompatible\n");
	for(int i = 0;i<ntax;i++)
		if(included[i])
			printf("*");
		else
			printf(".");
	printf("\n");*/
	for(std::list<Partition>::const_iterator i = begin();i!= end();i++)
		if(!i->maybeCompatible(ntax,included))
		{
			/*i->print();
			printf(" conflicts\n");*/
			return false; //This branch conflicts and should not be added
		}
	return true;
}

/**
 * AddMatrixRepresentation appendss the matrix representation of a tree to the given dataset
 */
void PartitionTree::AddMatrixRepresentation(Dataset* data) const
{
	for(std::list<Partition>::const_iterator i=begin();i!=end();i++)
	{
		for(int j=0;j<mNtaxa;j++)
		{
			if(dont_care[j])
			{
				data->addCharacters("-",j); //TODO change this to a gap
			}
			else
			{
				if(i->operator()(j))
				{
					data->addCharacters("A",j);  //The use of A and G are arbitrary to indicate the two states in a matrix representation
				}
				else
				{
					data->addCharacters("G",j);
				}
			}
		}
		i++; //partitions are doubled in this list, so skip twice
	}

}

/**
 * compatiblePartitions returns an array, and its size of all partitions which are compatible with this tree
 */
void PartitionTree::compatiblePartitions(int& num_partitions,Partition*& partitions)
{
	bool* included_taxa;
	list<Partition> compat_part;
	included_taxa = (bool*)malloc(sizeof(bool*)*mNtaxa);
	
	considerPartitions(&compat_part);
	num_partitions = compat_part.size();
	//printf("%d partitions from considerPartitions\n",num_partitions);
	/*considerAllPartitions(0,included_taxa,&compat_part);
	
	num_partitions = compat_part.size();
	*/
	//printf("%d partitions from considerAllPartitions\n",num_partitions);
	partitions = (Partition*)malloc(sizeof(Partition)*num_partitions);
	memset(partitions,0,sizeof(Partition)*num_partitions);
	for(int i=0;i<num_partitions;i++)
	{
		partitions[i] = compat_part.front();
		compat_part.pop_front();
	}
	
}

/**
 * partitionsNeededToResolve returns the number of branches that must be added to give a fully resolved tree
 */

int PartitionTree::partitionsNeededToResolve()
{
	//printf("%d = mNPartitions\n%d = mNtaxa\n",mNPartitions,mNtaxa);
	return (mNtaxa - 3) - mNPartitions;  //mNTaxa - 3 is a fully resolved tree
}

void PartitionTree::considerPartitions(list<Partition>* partitions)
{
	int i;
	Partition p(mNtaxa,1);  //This will be overwritten, the node id in the constructor is arbitrary
	bool* possible_partition;
	list<int> tojoin;
	
	possible_partition = (bool*)malloc(sizeof(bool)*mNtaxa);
	
	//Initialize union-find info
	if(ufinfo)
	{	
		free(ufinfo);
		ufinfo = NULL;
	}
	ufinfo = (int*)malloc(sizeof(int)*mNtaxa);
	for(i=0;i<mNtaxa;i++)
		ufinfo[i] = i;
		
	for(std::list<Partition>::const_iterator part_iter = begin();part_iter!=end();part_iter++)
	{
		mPartitionSet.push(*part_iter);
	}
	
	// Consider all partitions in ascending order, looking for unresolved portions of the tree
	while(!mPartitionSet.empty())
	{
		//Get the next partition
		p=mPartitionSet.top();
		/*p.print();
		printf("\n");*/
		//join trees in partition
				
		tojoin.clear();
		for(i=0;i<mNtaxa;i++)
		{
			if(p(i))
			{
				//is i in tojoin?
				if(std::find(tojoin.begin(),tojoin.end(),find(i)) == tojoin.end())
				{
					tojoin.push_back(find(i));
				}
			}
		}
		if(tojoin.size() > 2)
		{	
			//resolve this node
			//printf("Found unresolved node of needing %d branches\n",(int)tojoin.size()-2);
			bool* included_branches = (bool*)malloc(sizeof(bool)*tojoin.size());
			/*printf("Generating branches for ");
			for(list<int>::const_iterator iter = tojoin.begin(); iter != tojoin.end();iter++)
			{
				printf("%d ",*iter);
			}
			printf("\n");*/
			generatePossiblePartitions(tojoin,partitions,0,tojoin.size(),0,included_branches);
			free(included_branches);
			
		}
		for(list<int>::const_iterator iter = ++tojoin.begin(); iter != tojoin.end();iter++)
		{
			//printf("joining %d and %d\n",*(tojoin.begin()),*iter);
			join(*(tojoin.begin()),*iter,false);
		}
		mPartitionSet.pop();
	}
	//Join all remaining partials
	tojoin.clear();
	for(i=0;i<mNtaxa;i++)
	{
		//is i in tojoin?
		if(std::find(tojoin.begin(),tojoin.end(),find(i)) == tojoin.end())
		{
			tojoin.push_back(find(i));
		}
	}
	if(tojoin.size() > 3)
	{
		//resolve the center node
		//printf("Unresolved center node of needing %d branches\n",(int)tojoin.size()-3);
		tojoin.pop_front();  //this is a center node and one of the branches can be removed, do to unrooted nature of tree
		bool* included_branches = (bool*)malloc(sizeof(bool)*tojoin.size());
		generatePossiblePartitions(tojoin,partitions,0,tojoin.size(),0,included_branches);
		free(included_branches);
	}
}

void PartitionTree::generatePossiblePartitions(list<int>& to_consider,list<Partition>* partitions,int cur, int size,int included,bool* included_taxa)
{
	
	//if(cur > size) return;
	
	
		
	
	
	if(cur == size)	//consider this partition
	{
		if(included <= 1) return;  // a trivial partition
		
		/*for(int j=0;j<cur;j++)
		{
			if(included_taxa[j])
				printf("X");
			else
				printf("-");
		}
		printf("\n");
		
		printf("partition (");*/
		bool* possible_partition = (bool*)malloc(sizeof(bool)*mNtaxa);
		for(int j=0;j<mNtaxa;j++)
		{
			possible_partition[j] = false;
		}
		int i=0;
		for(list<int>::const_iterator iter = to_consider.begin();iter != to_consider.end();iter++)
		{
			if(included_taxa[i]&&i<cur)
			{
				//printf("%d(%d) ",*iter,i);
				for(int j=0;j<mNtaxa;j++)
				{
					if((find(j)) == *iter)
						possible_partition[j] = true;
				}
			}
			i++;
		}
		/*printf(")\n");
		for(int j=0;j<mNtaxa;j++)
		{
			if(possible_partition[j])
				printf("*");
			else
				printf(".");
		}
		printf("\n");*/
		Partition* possible = new Partition(mNtaxa,possible_partition);
		partitions->push_back(*possible);
		delete possible;
		delete possible_partition;
		return;
	}
	
	included_taxa[cur] = false;
	generatePossiblePartitions(to_consider,partitions,cur+1,size,included,included_taxa);

	if(included < size/2 + 1 )
	{
		//consider partitions larger than this
		
		included_taxa[cur] = true;
		generatePossiblePartitions(to_consider,partitions,cur+1,size,included+1,included_taxa);
	}
}

void PartitionTree::considerAllPartitions(int taxa,bool* included_taxa,list<Partition>* partitions)
{
	if(taxa == mNtaxa)
	{
		Partition part(mNtaxa,included_taxa);
		//is this partition non-trivial?
		int count = 0;
		for(int i=0;i<mNtaxa;i++)
			if(included_taxa[i])
				count++;
		if(count <= 1) return; // trivial partition
		if(count >= mNtaxa-1) return;  //trivial partition
		//is this partition compatible?
		if(isCompatible(part))
		{
			//add to list
			/*part.print();
			printf("\n");*/
			partitions->push_back(part);
		}
	}
	else
	{
		/*int j;
		for(j=0;j<taxa;j++)
			if(included_taxa[j])
				printf("*");
			else
				printf(".");
		for(j=taxa;j<mNtaxa;j++)
			printf("-");
		printf("\n");
		*/
		//are there partitions on this branch which are compatible?
		if((taxa==0)||(maybeCompatible(taxa,included_taxa)))
		{
			//if so consider each branching
			included_taxa[taxa] = false;
			considerAllPartitions(taxa+1,included_taxa,partitions);
			included_taxa[taxa] = true;
			considerAllPartitions(taxa+1,included_taxa,partitions);
		}
	}
}