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
#ifndef PSODA_PARTITION_H
#define PSODA_PARTITION_H

/**
 * Class partition represents one of the taxa partitions in a tree, or equivalently one of the branches in a tree
 */
class Partition
{
	public:
	Partition(int,int);
	Partition(int,bool*);
	Partition(const Partition&);
	~Partition();

    double& length();
    double  length() const;

	
	Partition & operator=(const Partition&);
	
	bool operator==(const Partition&) const;
	bool operator!=(const Partition&) const;
	
	bool operator<(const Partition&) const;
	bool operator>(const Partition&) const;
	bool operator<=(const Partition&) const;
	bool operator>=(const Partition&) const;
	
	Partition & operator+=(const Partition&);
	Partition operator+(const Partition&) const;
	bool operator()(const int) const;
	
	Partition & operator*=(const double&);
	Partition operator*(const double&) const;
	double dot(const Partition&) const;
	
	bool isCompatible(const Partition&) const;
	bool maybeCompatible(int,bool*) const;
	
	void setDontCares(bool*,bool);
	
	void print() const;
	int size() const;
	int nTaxa() const;
	
	protected:
		bool* mTaxa;
		int mNtaxa;		
        double mLength;
};

#endif
