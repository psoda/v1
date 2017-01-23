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
#include "RF.h"
#include "Interpreter.h"
#include "NewickTreeParser.h"
#include <math.h>
#include "PsodaPrinter.h"

/**
 * Constructor for RF
 **/
RF::RF(QTreeRepository* trees):mNtrees(0),partition_map(),distances(NULL),mNtaxa(Interpreter::getInstance()->dataset()->ntaxa())
{
	BuildMap(trees);
	//print();
}

/**
 * Destructor for RF
 */
RF::~RF()
{
	if(distances)
	{
		for(int i=0;i<mNtrees;i++)
			if(distances[i]) free(distances[i]);
		free(distances);
	}
}

/**
 * For debugging purposes print will display the internal map
 */
void RF::print(float cutoff) const
{
	int i;
	bool alldisplayed = true;
	//output header
	for(i=0;i<mNtaxa;i++)
		PsodaPrinter::getInstance()->write("%d",(i+1)%10);
	
	PsodaPrinter::getInstance()->write("\tfreq\t      %%\n");

	for(i=0;i<mNtaxa+18;i++)
		PsodaPrinter::getInstance()->write("_");
	PsodaPrinter::getInstance()->write("\n");

	std::priority_queue < std::pair< float, Partition>, std::vector< std::pair< int, Partition> > > mSortedPartitions;

	std::map<Partition, list<int> >::const_iterator it;
	for(it = partition_map.begin();it != partition_map.end(); it++)
	{
		float percent = (float)it->second.size()/mNtrees;
		if(percent > cutoff)
		{
			mSortedPartitions.push(make_pair(it->second.size(),it->first));
		}
		else
		{
			alldisplayed = false;
		}
	}
	
	while(!mSortedPartitions.empty())
	{
		std::pair< int, Partition> temp_pair = mSortedPartitions.top();
		mSortedPartitions.pop();

		//output partition
		temp_pair.second.print();
		PsodaPrinter::getInstance()->write("\t");

		PsodaPrinter::getInstance()->write("%d\t%3.2f%%",temp_pair.first,(float)temp_pair.first/mNtrees*100);
		PsodaPrinter::getInstance()->write("\n");
	
	}
	if(!alldisplayed)
		PsodaPrinter::getInstance()->write("Partitions apearing in less than %3.2f%% of trees not displayed\n",cutoff*100);
}

/**
 * BuildMap constructs a table mapping partitions to trees for a QTreeRepository
 *
 * Each partition in every tree in the QTreeRepository is saved in a map along with
 * the tree number for every tree that the partition occurs in.  This data is used
 * both for calculating RF distances as well as for computing consensus trees.
 */
 
void RF::BuildMap(QTreeRepository* trees)
{
	//Reset map
	partition_map.clear();
	mNtrees = 0;
	//Go through the QTreeRepository
	deque<QTree *> tree_list = *(trees->getTrees());
	deque <QTree *>::iterator treeIt; // Iterator to use in acccessing list
	for(treeIt = tree_list.begin(); treeIt != tree_list.end(); treeIt++)
	{
		//For each tree
		AddTree(*treeIt,mNtrees);
		mNtrees++;
		
	}
}

/**
 * AddTree deals with the unrootedness of QTrees and begins the process of adding their partitions to the map
 */
 
void RF::AddTree(QTree tree, int tree_num)
{
	//printf("TREE: %d\n",tree_num);
	
	PartitionTree t(&tree);
	for(std::list<Partition>::const_iterator i = t.begin();i != t.end();i++)
	{
		AddPartitionToMap(*i,tree_num);
	}
	

}

/**
 * AddPartitionToMap adds a given tree to the given partition in the map
 */
 
void RF::AddPartitionToMap(Partition p, int tree_num)
{
	if(p.size() > 1)
	{
		//printf("Adding tree %d to partition ", tree_num);
		//p.print();
		//printf("\n");
		
		partition_map[p].push_front(tree_num);
		//check that the tree has been added to this partition only once
		if(*(++partition_map[p].begin()) == tree_num)
		{
			if(partition_map[p].size() > 1)
				partition_map[p].pop_front();
		}
		//printf("Current map:\n");
		//print();
	}
}

/**
 * CalculateDistanceMatrix converts partition map to an all to all RF distance matrix
 */
 
void RF::CalculateDistanceMatrix()
{
	//allocate space for matrix
	distances = (int**)malloc(sizeof(int*)*mNtrees);
	for(int i=0;i<mNtrees;i++)
		distances[i] = (int*)malloc(sizeof(int)*mNtrees);
	// A tree has n-3 non-trivial partitions, so the max RF distance is 2(n-3)
	int max_dist = 2*(mNtaxa - 3);
	
	//initialize all elements to the maximum distance
	for(int i=0;i<mNtrees;i++)
	for(int j=0;j<mNtrees;j++)
		distances[i][j] = max_dist;
		
	//consider each partition in the map
	
	std::map<Partition, list<int> >::const_iterator part_it;
	for(part_it = partition_map.begin();part_it != partition_map.end(); part_it++)
	{
		//for each pair of trees associated with the tree decrement their distances from each other by 2
		for(list<int>::const_iterator tree_1 = part_it->second.begin(); tree_1 != part_it->second.end(); tree_1++)
		for(list<int>::const_iterator tree_2 = part_it->second.begin(); tree_2 != part_it->second.end(); tree_2++)
		{
			distances[*tree_1][*tree_2]--;
			distances[*tree_2][*tree_1]--;
		}
	}
	
	//We double decremented the diagonal elements, so fix them
	for(int i=0;i<mNtrees;i++)
		distances[i][i] = 0;
}

/**
 * numTrees() returns the number of trees in the map
 */
 
int RF::numTrees() const
{
	return mNtrees;
}

/**
 * uniqueSize counts the number of unique elements, it assumes that duplicates are consecutive
 */
unsigned int uniqueSize(std::list<int> trees)
{
	int last = -1;
	unsigned int count = 0;
	for(std::list<int>::iterator i=trees.begin();i!=trees.end();i++)
	{
		if(last != *i) count++;
		last = *i;
	}
	
	return count;
}


/**
 * resolution returns the percentance (from 0 to 1) of resolved branches in the consensus tree from the partition map.
 * The degree of consensus is arbitrarily specified but is in the range 0.5-1.0
 */
double RF::resolution (float percentage)  

{
	int resolved = 0;
	
	if(percentage < 0.5) percentage = 0.5;
	if(percentage > 1.0) percentage = 1.0;

	PartitionTree conTree(mNtaxa);
	unsigned int threshold = (unsigned int) ceil(mNtrees*percentage);
    if (threshold <= (unsigned int)(mNtrees/2))
        threshold++;
	
	//walk through map, adding appropriate partitions to consensus tree
	
	std::map<Partition, list<int> >::const_iterator part_it;
	for(part_it = partition_map.begin();part_it != partition_map.end(); part_it++)
	{
		if(uniqueSize(part_it->second) >= threshold)
		{
			resolved++;
		}
	}
	
	double rval = (double)resolved/(mNtaxa-3);
	if(rval < 0.0)
	{
		printf("%d/%d = %f\n",resolved,mNtaxa-2,rval);
	}
	if(rval > 1.0)
	{
		printf("%d/%d = %f\n",resolved,mNtaxa-2,rval);
	}
	
	return rval;
} 



 
/**
 * consensus builds and returns a consensus tree from the partition map.
 * The degree of consensus is arbitrarily specified but is in the range 0.5-1.0
 */ 
QTree* RF::consensus (float percentage)
{
	char* treestring;

	bool checkForConflict;

	//if(percentage < 0.5) percentage = 0.5;
	if(percentage > 1.0) percentage = 1.0;

	PartitionTree conTree(mNtaxa);
	unsigned int threshold = (unsigned int) ceil(mNtrees*percentage);
    if (threshold <= (unsigned int)(mNtrees/2))
        checkForConflict = true;
	else
		checkForConflict = false;
	
	//walk through map, adding appropriate partitions to consensus tree
	
	std::map<Partition, list<int> >::const_iterator part_it;
	
	std::priority_queue < std::pair< float, Partition>, std::vector< std::pair< float, Partition> > > mSortedPartitions;
	
	for(part_it = partition_map.begin();part_it != partition_map.end(); part_it++)
	{
		if(uniqueSize(part_it->second) >= threshold)
		{
			//add partition to tree (currently a priority_queue)
            //part_it->first->length() = part_it->second.size()/(double)mNtrees;
			if(checkForConflict)
			{
				mSortedPartitions.push(make_pair((double)uniqueSize(part_it->second)/mNtrees,part_it->first));
			}
			else
			{
				conTree.add(part_it->first,(double)uniqueSize(part_it->second)/mNtrees);
			}
		}
	}
	
	if(checkForConflict)
	{
		while(!mSortedPartitions.empty())
		{
			std::pair< float, Partition> temp_pair = mSortedPartitions.top();
			mSortedPartitions.pop();
			if(conTree.isCompatible(temp_pair.second))
			{
				conTree.add(temp_pair.second,temp_pair.first);
				//printf("Adding ");
				//temp_pair.second.print();
				//printf("\t%.3f\n",temp_pair.first);
			}
		}
	
	}
	
	//build tree from selected partitions
	treestring = conTree.treeString();
	
    NewickTreeParser* ntp = new NewickTreeParser();
    QTree * qt = ntp->parseBuffer(treestring, strlen(treestring), false);
	qt->setTreeStr(treestring);
	return qt;
} 

/**
 * conflate builds and returns a tree with at most the given degree of unresolution.
 * All branches in a strict consensus are included an enough from the first tree to yeild the required tree.
 */ 
void RF::conflate (int unresolution_degree,PartitionTree* conTree)
{
	char* treestring;
	float percentage = 1.0;

	unsigned int threshold = (unsigned int) ceil(mNtrees*percentage);
   
	
	//walk through map, adding appropriate partitions to consensus tree
	
	std::map<Partition, list<int> >::const_iterator part_it;
	
	std::list < Partition > mOtherPartitions;
	
	int ct_total = 0;
	int ct_added = 0;
	int ct_other = 0;
	
	for(part_it = partition_map.begin();part_it != partition_map.end(); part_it++)
	{
		ct_total++;
		if(uniqueSize(part_it->second) >= threshold)
		{
			//add partition to tree (currently a priority_queue)
            ct_added++;
			conTree->add(part_it->first);

		}
		else
		{
			//check that first tree has this partition
			list<int>::const_iterator result = find(part_it->second.begin(),part_it->second.end(),0);
			if(result != part_it->second.end())
			{
				ct_other++;
				mOtherPartitions.push_back(part_it->first);
			}
		}
	}
	
	int still_needed = conTree->partitionsNeededToResolve();
	still_needed -= unresolution_degree;
	
	/*printf("still_needed = %d\n",still_needed);
	printf("%d = %d + %d\n",ct_total,ct_added,ct_other);
	printf("%d - %d = %d\n",mNtaxa,still_needed,mNtaxa - still_needed);
	*/
	//printf("%d = ct_other, %d = Other.size()\n",ct_other,mOtherPartitions.size());
	while(still_needed > 0)
	{
		if(mOtherPartitions.empty())
		{
			printf("ERROR: Not enough partitions (%d needed)\n",still_needed);
		}
		Partition temp_part = mOtherPartitions.front();
		if(conTree->isCompatible(temp_part))
		{
			conTree->add(temp_part);
			still_needed--;
			//printf("Adding ");
			//temp_pair.second.print();
			//printf("\t%.3f\n",temp_pair.first);
		}
		mOtherPartitions.pop_front();
	}



}

/**
 * Paren operator, RF(i,j) returns the rf distance between the ith and jth trees
 */
 
int RF::operator()(int i,int j)
{
	//If the request is out of bounds return a -1
	if(i>=mNtrees) return -1;
	if(j>=mNtrees) return -1;
	if(i<0) return -1;
	if(j<0) return -1;
	
	//A valid distance has been requested, calculate distance if needed
	
	if(!distances)
		CalculateDistanceMatrix();  

	//Return appropriate distance from distances matrix
	return distances[i][j];
}
