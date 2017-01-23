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
#ifndef _PSODA_PROJECTED_TREE_
#define _PSODA_PROJECTED_TREE_

#include "QTree.h"

class ProjectedTree
{
	public:
	ProjectedTree();
	ProjectedTree(unsigned int);
	ProjectedTree(const ProjectedTree&);
	~ProjectedTree();
	
	ProjectedTree& operator=(const ProjectedTree&);
	
	ProjectedTree& operator+=(const ProjectedTree&);
	ProjectedTree& operator-=(const ProjectedTree&);
	ProjectedTree& operator*=(const double&);
	ProjectedTree& operator/=(const double&);
	
	ProjectedTree operator+(const ProjectedTree&) const;
	ProjectedTree operator-(const ProjectedTree&) const;
	ProjectedTree operator*(const double&) const;
	ProjectedTree operator/(const double&) const;
	
	double& operator()(unsigned int);
	double operator()(unsigned int) const;
	
	double length();
	int Dimension() const;
	
	QTree* getTree() const;
	const char* getTreeStr() const;
	void setTree(QTree*);
	
	double score;
	
	protected:
	double mLength;
	bool lengthChanged;
	unsigned int dimension;
	double* pos;
	char* mTree;
};

#endif
