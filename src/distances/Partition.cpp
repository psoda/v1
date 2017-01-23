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
 *  Partition.cpp
 *  psoda
 *
 *  Created by Kenneth Sundberg on 9/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Globals.h"
#include "Partition.h"
#include "PsodaError.h"
#include "PsodaPrinter.h"
#include "assert.h"

Partition::Partition(int nTaxa,int taxa):mTaxa(NULL),mNtaxa(nTaxa),mLength(-1.0)
{
	mTaxa = (bool*)malloc(sizeof(bool)*mNtaxa);
	assert(mTaxa);
	for(int i=0;i<mNtaxa;i++)
		mTaxa[i] = false;
	mTaxa[taxa] = true;
}

Partition::Partition(int nTaxa,bool* taxa):mTaxa(NULL),mNtaxa(nTaxa),mLength(-1.0)
{
	mTaxa = (bool*)malloc(sizeof(bool)*mNtaxa);
	assert(mTaxa);
	int true_size = 0;
	for(int i=0;i<mNtaxa;i++)
	{
		mTaxa[i] = taxa[i];
		if(taxa[i]) true_size++;
	}
	if((true_size * 2) > nTaxa)
	{
		//invert so that true partition is smaller than false partition
		for(int i=0;i<mNtaxa;i++) mTaxa[i] = !mTaxa[i];
	}
}

Partition::Partition(const Partition& a):mTaxa(NULL),mNtaxa(a.mNtaxa),mLength(a.mLength)
{
	mTaxa = (bool*)malloc(sizeof(bool)*mNtaxa);
	for(int i=0;i<mNtaxa;i++)
		mTaxa[i] = a.mTaxa[i];
}

Partition::~Partition()
{
	if(mTaxa) free(mTaxa);
	mTaxa = NULL;
}

double& Partition::length()
{
    return mLength;
}
double  Partition::length() const
{
    return mLength;
}


/**
 * Assignment opperator, performs a deep copy
 */
Partition & Partition::operator=(const Partition& rhs)
{
	if(this != &rhs)
	{
		if(mNtaxa != rhs.mNtaxa)
		{
			if(mTaxa) free(mTaxa);
			mTaxa = NULL;
			mNtaxa = rhs.mNtaxa;
			mTaxa = (bool*)malloc(sizeof(bool) * mNtaxa);
			assert(mTaxa);
		}
		for(int i=0;i<mNtaxa;i++)
			mTaxa[i] = rhs.mTaxa[i];
        mLength = rhs.mLength;
	}
	return *this;
}


/**
 * Equality operator returns true if the given bipartitions are equal
 *
 * It is assumed that the taxa in the 'true' partition are in the smaller partition.
 * Thus equality of the partitions is accomplished with an equality of the representation
 */
bool Partition::operator==(const Partition& rhs) const
{
	if(mNtaxa!=rhs.mNtaxa) return false;
	for(int i=0;i<mNtaxa;i++)
		if(mTaxa[i] != rhs.mTaxa[i])
			return false;
	return true;
}

/**
 * Inequality operator returns true if the given bipartitions are unequal
 */
bool Partition::operator!=(const Partition& rhs) const
{
	return ! operator==(rhs);
}

/**
 * Less than operator
 *
 * The ordering of partitions is arbitrary chosen to be as follows:
 * 1. Partitions of a smaller taxa set are less than those of a larger taxa set
 * 2. Partitions with a smaller 'true' set are less than those with a larger 'true' set
 *       (this property is useful for constructing trees from partition sets)
 * 3. Partitions of the same size are treated as big endian binary numbers
 */
bool Partition::operator<(const Partition& rhs) const
{
	int lsize = 0;
	int rsize = 0;
	bool smaller_number = false;
	bool small_set = false;
	
	if(mNtaxa < rhs.mNtaxa) return true;
	if(mNtaxa > rhs.mNtaxa) return false;
	
	for(int i=0;i<mNtaxa;i++)
	{
		if(mTaxa[i] != rhs.mTaxa[i])
			if(!small_set)
			{
				smaller_number = !mTaxa[i];
				small_set = true;
			}
		if(mTaxa[i]) lsize++;
		if(rhs.mTaxa[i]) rsize++;
	}
	if(lsize < rsize) return true;
	if(lsize > rsize) return false;
	if(!small_set) return false; // They are equal
	
	
	
	
	return smaller_number;  //They are equal in size, return the smaller number
	
}

/**
 * Greater than operator
 *
 * All comparison operators are defined in terms of < and ==
 */
bool Partition::operator>(const Partition& rhs) const
{
	return ! operator<=(rhs);
}

/**
 * Less than or equals operator
 *
 * All comparison operators are defined in terms of < and ==
 */
bool Partition::operator<=(const Partition& rhs) const
{
	if(operator<(rhs)) return true;
	if(operator==(rhs)) return true;
	return false;
}

/**
 * Greater than or equals operator
 *
 * All comparison operators are defined in terms of < and ==
 */
bool Partition::operator>=(const Partition& rhs) const
{
	return ! operator<(rhs);
}

/**
 * Assignement Addition operator
 *
 * Throws a PsodaError if the size of the taxa sets are unequal
 */
Partition & Partition::operator+=(const Partition& rhs)
{
	int i;
	if(mNtaxa != rhs.mNtaxa)
		throw PsodaError("Partitions on unequal taxa sets are undefined");
	bool* newTaxa = (bool*)malloc(sizeof(bool)*mNtaxa);
	//partition sets have an implied direction, to be succesfully added both branches must point away from the node
	bool needsInverted = false;
	//Try to add nodes as though they did not need to be inverted
	for(i=0;i<mNtaxa;i++)
	{
		newTaxa[i] = mTaxa[i] || rhs.mTaxa[i];
		if(mTaxa[i] && rhs.mTaxa[i])
			needsInverted = true;
	}
	if(needsInverted)
	{
		needsInverted = false;
		
		//Attept addition by inverting *this
		for(int i=0;i<mNtaxa;i++)
		{
			newTaxa[i] = (!mTaxa[i]) || rhs.mTaxa[i];
			if((!mTaxa[i]) && rhs.mTaxa[i])
				needsInverted = true;
		}
			
		if(needsInverted)
		{
			needsInverted = false;
			for(int i=0;i<mNtaxa;i++)
			{
				newTaxa[i] = mTaxa[i] || (!rhs.mTaxa[i]);
				if(mTaxa[i] && (!rhs.mTaxa[i]))
					needsInverted = true;
			}
			if(needsInverted)
				printf("ERROR: Partitions can not be combined!\n");
		}

	}
	
	
	//Values calculated in newTaxa are correct
	free(mTaxa);
	mTaxa = newTaxa;
	
	
	//Check that the true set is the largest set
	int t_count = 0;
	for(int i=0;i<mNtaxa;i++)
		if(mTaxa[i])
			t_count++;
	// if not make it so
	if(t_count > mNtaxa / 2)
		for(int i=0;i<mNtaxa;i++)
			mTaxa[i] = !mTaxa[i];
	
	return *this;
}

/**
 * Binary Addition operator
 */
 
Partition Partition::operator+(const Partition& other) const
{
	Partition result(*this);
	result += other;
	return result;
}

/**
 * Paren operator returns whether a taxa is in the smaller partition
 */
bool Partition::operator()(const int index) const
{
	if(index < 0) return false;
	if(index > mNtaxa) return false;
	return mTaxa[index];
}

/**
 * isCompatible returns true is both partitions can be in the same tree otherwise it returns false
 *
 * A partition is compatible if one of the partitions is a subset of another
 */
bool Partition::isCompatible(const Partition& other) const
{
	int i;
	//Is the true partition of this a subset of the true partition of other?
	bool subset = true;
	for(i=0;i<mNtaxa;i++)
		if(mTaxa[i])
			if(!other.mTaxa[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the false partition of this a subset of the false partition of other?
	subset = true;
	for(i=0;i<mNtaxa;i++)
		if(!mTaxa[i])
			if(other.mTaxa[i])
				subset = false;
	if(subset) return true;
	
	//Is the true partition of this a subset of the false partition of other?
	subset = true;
	for(i=0;i<mNtaxa;i++)
		if(mTaxa[i])
			if(other.mTaxa[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the false partition of this a subset of the true partition of other?
	subset = true;
	for(i=0;i<mNtaxa;i++)
		if(!mTaxa[i])
			if(!other.mTaxa[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the true partition of other a subset of the true partition of this?
	subset = true;
	for(i=0;i<mNtaxa;i++)
		if(other.mTaxa[i])
			if(!mTaxa[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the false partition of other a subset of the false partition of this?
	subset = true;
	for(i=0;i<mNtaxa;i++)
		if(!other.mTaxa[i])
			if(mTaxa[i])
				subset = false;
	if(subset) return true;
	
	//Is the true partition of other a subset of the false partition of this?
	subset = true;
	for(i=0;i<mNtaxa;i++)
		if(other.mTaxa[i])
			if(mTaxa[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the false partition of other a subset of the true partition of this?
	subset = true;
	for(i=0;i<mNtaxa;i++)
		if(!other.mTaxa[i])
			if(!mTaxa[i])
				subset = false;
	if(subset) return true;

	return false;
	
}

/**
 * maybeCompatible is a less strict version of isCompatible
 * it takes a number of taxa and which partition each is in, this list can be subset
 */
bool Partition::maybeCompatible(int ntax,bool* included) const
{
	int i;
	//Is the true partition of this a subset of the true partition of other?
	bool subset = true;
	for(i=0;i<ntax;i++)
		if(mTaxa[i])
			if(!included[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the false partition of this a subset of the false partition of other?
	subset = true;
	for(i=0;i<ntax;i++)
		if(!mTaxa[i])
			if(included[i])
				subset = false;
	if(subset) return true;
	
	//Is the true partition of this a subset of the false partition of other?
	subset = true;
	for(i=0;i<ntax;i++)
		if(mTaxa[i])
			if(included[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the false partition of this a subset of the true partition of other?
	subset = true;
	for(i=0;i<ntax;i++)
		if(!mTaxa[i])
			if(!included[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the true partition of other a subset of the true partition of this?
	subset = true;
	for(i=0;i<ntax;i++)
		if(included[i])
			if(!mTaxa[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the false partition of other a subset of the false partition of this?
	subset = true;
	for(i=0;i<ntax;i++)
		if(!included[i])
			if(mTaxa[i])
				subset = false;
	if(subset) return true;
	
	//Is the true partition of other a subset of the false partition of this?
	subset = true;
	for(i=0;i<ntax;i++)
		if(included[i])
			if(mTaxa[i])
				subset = false;
	if(subset) return true;
	
	
	//Is the false partition of other a subset of the true partition of this?
	subset = true;
	for(i=0;i<ntax;i++)
		if(!included[i])
			if(!mTaxa[i])
				subset = false;
	if(subset) return true;

	return false;
}

/**
 * Size returns the size of the smallest of the two partitions
 */
int Partition::size() const
{
	int t_count = 0;
	for(int i=0;i<mNtaxa;i++)
		if(mTaxa[i])
			t_count++;
	return t_count;
}

/**
 * For debugging purposes print will display the partition
 */
void Partition::print() const
{
	for(int i=0;i<mNtaxa;i++)
		if(mTaxa[i])
			PsodaPrinter::getInstance()->write("*");
		else
			PsodaPrinter::getInstance()->write(".");
		
	/*
	for(int i=0;i<mNtaxa;i++)
		if(mTaxa[i])
			printf("%d ",i);
	*/
}

/**
 * nTaxa() returns the number of taxa in both partitions
 */
 
int Partition::nTaxa() const
{
	return mNtaxa;
}

void Partition::setDontCares(bool* dont_care,bool value)
{
	/*set all the don't cares to the given partition*/
	for(int i=0;i<mNtaxa;i++)
		if(dont_care[i])
			mTaxa[i] = value;
			
	if(size() > mNtaxa/2)
	{
		/*invert so the the 'true' partition is smaller than the 'false' partition*/
		for(int i=0;i<mNtaxa;i++)
			mTaxa[i] = !mTaxa[i];
	}
}
