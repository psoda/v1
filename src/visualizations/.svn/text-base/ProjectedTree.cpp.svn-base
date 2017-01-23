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
#include "ProjectedTree.h"
#include "NewickTreeParser.h"
#include <math.h>
#include <string.h>

/**
 * Default constructor, sets projection to a 3-D space
 */
ProjectedTree::ProjectedTree():score(-1),mLength(0.0),lengthChanged(false),dimension(3),pos((double*)malloc(sizeof(double)*dimension)),mTree(NULL)
{
	for(unsigned int i=0;i<dimension;i++)
	{
		pos[i] = 0.0;
	}
}


/**
 * Constructor, sets projection to a size-D space
 */
ProjectedTree::ProjectedTree(unsigned int size):score(-1),mLength(0.0),lengthChanged(false),dimension(size),pos((double*)malloc(sizeof(double)*size)),mTree(NULL)
{
	for(unsigned int i=0;i<dimension;i++)
	{
		pos[i] = 0.0;
	}
}

/**
 * Copy constructor, performs deep copy of position, but a shallow copy of the tree
 */
ProjectedTree::ProjectedTree(const ProjectedTree& a):score(a.score),mLength(a.mLength),lengthChanged(a.lengthChanged),dimension(a.dimension),pos((double*)malloc(sizeof(double)*dimension)),mTree(NULL)
{
	for(unsigned int i=0;i<dimension;i++)
	{
		pos[i] = a.pos[i];
	}
	if(a.mTree)
		mTree = strdup(a.mTree);
}

/**
 * Destructor
 */

ProjectedTree::~ProjectedTree()
{
	if(pos)
		free(pos);
	if(mTree)
		free(mTree);
	mTree = NULL;
}

/**
 * Assignment operator, performs a deep copy of position, but a shallow copy of the tree
 */
ProjectedTree& ProjectedTree::operator=(const ProjectedTree& rhs)
{
	if (this != &rhs)
	{
		if(dimension != rhs.dimension)
		{
			free(pos);
			dimension = rhs.dimension;
			pos = (double*)malloc(sizeof(double)*dimension);
		}
		for(unsigned int i=0;i<dimension;i++)
			pos[i] = rhs.pos[i];
		if(mTree)
			free(mTree);
		mTree = NULL;
		if(rhs.mTree)
		{
			mTree = strdup(rhs.mTree);
		}
		mLength = rhs.mLength;
		lengthChanged=rhs.lengthChanged;
		score = rhs.score;
	}
	return *this;
	
}

/**
 * Assignment Addition operator, if dimensions do not match, result has the larger dimension 
 * with all extra dimensions in the smaller dimensions havinig a zero component
 */

ProjectedTree& ProjectedTree::operator+=(const ProjectedTree& rhs)
{
	unsigned int i;
	
	if(dimension < rhs.dimension)
	{
		double* temp;
		temp = (double*)malloc(sizeof(double)*rhs.dimension);
		
		for(i=0;i< dimension;i++)
		{
			temp[i] = pos[i] + rhs.pos[i];
		}
		for(i=dimension;i<rhs.dimension;i++)
		{
			temp[i] = rhs.pos[i];
		}
		free(pos);
		pos = temp;
		dimension = rhs.dimension;
	}
	else
	{
		for(i=0;i< rhs.dimension;i++)
		{
			pos[i] +=rhs.pos[i];
		}
	}
	lengthChanged = true;
	if(!mTree)
	{
		free(mTree);
		mTree = NULL;
		if(rhs.mTree)
			mTree = strdup(rhs.mTree);
	}
	score = rhs.score;
	return *this;
}

/**
 * Assignment Subtraction operator, if dimensions do not match, result has the larger dimension 
 * with all extra dimensions in the smaller dimensions havinig a zero component
 */

ProjectedTree& ProjectedTree::operator-=(const ProjectedTree& rhs)
{
	unsigned int i;
	
	if(dimension < rhs.dimension)
	{
		double* temp;
		temp = (double*)malloc(sizeof(double)*rhs.dimension);
		
		for(i=0;i< dimension;i++)
		{
			temp[i] = pos[i] - rhs.pos[i];
		}
		for(i=dimension;i<rhs.dimension;i++)
		{
			temp[i] = -rhs.pos[i];
		}
		free(pos);
		pos = temp;
		dimension = rhs.dimension;
	}
	else
	{
		for(i=0;i< rhs.dimension;i++)
		{
			pos[i] -=rhs.pos[i];
		}
	}
	lengthChanged = true;
	if(!mTree)
	{
		free(mTree);
		mTree = NULL;
		if(rhs.mTree)
			mTree = strdup(rhs.mTree);
	}
	score = rhs.score;
	return *this;
}

/**
 * Assignment Multiplication operator, scales all components by the given value
 */
 
ProjectedTree& ProjectedTree::operator*=(const double& rhs)
{
	for(unsigned int i=0;i<dimension;i++)
		pos[i] *= rhs;
	mLength*=rhs;
	return *this;
}

/**
 * Assignment Division operator, scales all components by the given value
 */
 
ProjectedTree& ProjectedTree::operator/=(const double& rhs)
{
	for(unsigned int i=0;i<dimension;i++)
		pos[i] /= rhs;
	mLength/=rhs;
	return *this;
}

/**
 * Binary Addition opeartor
 */
 
ProjectedTree ProjectedTree::operator+(const ProjectedTree& other) const
{
	ProjectedTree result(*this);
	result += other;
	return result;
}

/**
 * Binary Subtraction opeartor
 */
 
ProjectedTree ProjectedTree::operator-(const ProjectedTree& other) const
{
	ProjectedTree result(*this);
	result -= other;
	return result;
}

/**
 * Binary Multiplication opeartor
 */
 
ProjectedTree ProjectedTree::operator*(const double& other) const
{
	ProjectedTree result(*this);
	result *= other;
	return result;
}

/**
 * Binary Division opeartor
 */
 
ProjectedTree ProjectedTree::operator/(const double& other) const
{
	ProjectedTree result(*this);
	result /= other;
	return result;
}

/**
 * Dimension() returns the current dimension of the projected point
 */
int ProjectedTree::Dimension() const
{
	return dimension;
}

/**
 * Paren opeartor as a setter returns the address of the ith element of pos,
 * if an element that is out of range is requested the dimensionality is expanded to accomadate the request
 */
double& ProjectedTree::operator()(unsigned int i)
{
	unsigned int j;
	if(i>=dimension)
	{
		//resize pos
		double* temp = (double*)malloc(sizeof(double)*(i+1));  //+1 because of the zero based array
		for(j=0;j<dimension;j++)
			temp[j] = pos[j];
		for(j=dimension;j<i;j++)
			temp[j] = 0.0;
		temp[i] = 0.0;
		dimension = i+1;
		free(pos);
		pos = temp;
	}
	lengthChanged = true;  //An element is being used as an L-value
	return pos[i];
		
}
/**
 * Paren operator as a getter returns the value of the ith element of pos,
 * if an element that is out of range is requested the value of 0.0 is returned
 */
double ProjectedTree::operator()(unsigned int i) const
{ 
	if(i>=dimension)
		return 0.0;
	else
		return pos[i];
}

/**
 * length returns the euclidian length (L-2 Norm) of pos
 */ 
double ProjectedTree::length()
{
	if(lengthChanged)
	{
	double length = 0.0;
	for(unsigned int i=0;i<dimension;i++)
		length += pos[i]*pos[i];
	mLength = sqrt(length);
	}	
	return mLength;
}

/**
 * getTree returns the tree associated with this point
 */
QTree* ProjectedTree::getTree() const
{
	if(mTree)
	{
		NewickTreeParser* ntp = new NewickTreeParser();
		QTree * qt = ntp->parseBuffer(mTree, strlen(mTree), false);
		qt->setTreeStr(mTree);
		return qt;
	}
	return NULL;
}

/**
 * getTreeStr returns the newick tree string associated with this point
 */
const char* ProjectedTree::getTreeStr() const
{
	return mTree;
}


/**
 * setTree sets the tree associated with this point
 */
void ProjectedTree::setTree(QTree* t)
{
	if(mTree)
	{
		free(mTree);
		mTree = NULL;
	}
	mTree = strdup(t->treeStr());

	
}