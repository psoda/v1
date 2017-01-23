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
#include "GradientDescent.h"
#include "PartitionTree.h"
#include <math.h>

GradientDescentSearch::GradientDescentSearch()
{
	setup_tables(false);
}

GradientDescentSearch::~GradientDescentSearch()
{
}

const char* GradientDescentSearch::name() const
{
	return "Gradient Descent Search";
} 

void GradientDescentSearch::search(QTreeRepository &qtreeRepository, QAlgorithmBase *qsearchAlgorithm,  EvaluatorBase *evaluator, int iterations)
{}	

ProjectedTree GradientDescentSearch::map(const PartitionTree* pt) const
{
		ProjectedTree point(3);
		
		//set point to the origin
		point(0) = 0.0;
		point(1) = 0.0;
		point(2) = 0.0;
		
		for(std::list<Partition>::const_iterator part = pt->begin();part != pt->end() ;part++)
		{
			//lookup partition and add to point
			point += lookup(*part);
		}		
		delete(pt);
	
		return point;
}

ProjectedTree GradientDescentSearch::map(QTree* qt) const
{
	
		ProjectedTree point(3);
		PartitionTree* pt = new PartitionTree(qt);
		
		//set point to the origin
		point(0) = 0.0;
		point(1) = 0.0;
		point(2) = 0.0;
		
		for(std::list<Partition>::const_iterator part = pt->begin();part != pt->end() ;part++)
		{
			//lookup partition and add to point
			point += lookup(*part);
		}		
		point.setTree(qt);
		delete(pt);
	
		return point;
}

ProjectedTree GradientDescentSearch::lookup(const Partition part) const
{
	ProjectedTree point(3);
	
	/*Uses Jenkins One at a Time hash adaptied from Wikipedia Hash_tables article*/
	
	unsigned int index;
	int i;
	
	index=0;
	
	for(i=0;i<part.nTaxa();i++)
	{
		index+= (char) part(i);
		index+=(index<<10);
		index^=(index>>6);
	}
	index+=(index<<3);
	index^=(index>>11);
	index+=(index<<15);
	
	/*End One at a Time hash*/
	
	index%=HYPERSPHERE_LOOKUP_TABLE_SIZE;
	
	point(0) = xtable[index];
	point(1) = ytable[index];
	point(2) = ztable[index];
	return point;
}

void GradientDescentSearch::setup_tables(bool orthogonal)
{
	double inner_prod_xy,inner_prod_xz,inner_prod_yz;
	double length_x,length_y,length_z;
	unsigned int i;
#ifdef DEBUG
	FILE* vectors;
	FILE* fstatus;
	
	fdopen(2,"w");
#endif	
	
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		xtable[i] = (double)rand()/RAND_MAX;
		ytable[i] = (double)rand()/RAND_MAX;
		ztable[i] = (double)rand()/RAND_MAX;
		accessed[i]=0;
	}
	
	if(!orthogonal) return;
	
	/*insure linear independence of vectors
	Gram-Schmidt Method
	*/


#ifdef DEBUG
	/*Sanity Check*/

	inner_prod_xy=inner_prod_xz=inner_prod_yz=0.0;
	length_x=length_y=length_z=0.0;
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		inner_prod_xy+=xtable[i]*ytable[i];
		inner_prod_xz+=xtable[i]*ztable[i];
		inner_prod_yz+=ytable[i]*ztable[i];
		
		length_x+=xtable[i]*xtable[i];
		length_y+=ytable[i]*ytable[i];
		length_z+=ztable[i]*ztable[i];
	}
	fprintf(fstatus,"INNER PRODUCTS OF TABLE BEFORE GS\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n",inner_prod_xy,inner_prod_xz,inner_prod_yz);
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n\n",length_x,length_y,length_z);
#endif
	
	
	inner_prod_xy=inner_prod_xz=inner_prod_yz=0.0;
	length_x=length_y=0;
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		inner_prod_xy += xtable[i]*ytable[i];
		inner_prod_xz += xtable[i]*ztable[i];
		length_x += xtable[i]*xtable[i];
	}
	
#ifdef DEBUG
	fprintf(fstatus,"XY_SCALE %f\n",inner_prod_xy / length_x);
	fprintf(fstatus,"XZ_SCALE %f\n",inner_prod_xz / length_x);
#endif
	
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		ytable[i]-=xtable[i] * (inner_prod_xy / length_x);
		ztable[i]-=xtable[i] * (inner_prod_xz / length_x);
	}

#ifdef DEBUG
	/*Sanity Check*/

	inner_prod_xy=inner_prod_xz=inner_prod_yz=0.0;
	length_x=length_y=length_z=0.0;
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		inner_prod_xy+=xtable[i]*ytable[i];
		inner_prod_xz+=xtable[i]*ztable[i];
		inner_prod_yz+=ytable[i]*ztable[i];
		
		length_x+=xtable[i]*xtable[i];
		length_y+=ytable[i]*ytable[i];
		length_z+=ztable[i]*ztable[i];
	}
	fprintf(fstatus,"INNER PRODUCTS OF TABLE AFTER GS WITH X\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n",inner_prod_xy,inner_prod_xz,inner_prod_yz);
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n\n",length_x,length_y,length_z);
#endif
	inner_prod_xy=inner_prod_xz=inner_prod_yz=0.0;
	length_x=length_y=length_z=0.0;	
	
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		inner_prod_yz += ytable[i]*ztable[i];
		length_y += ytable[i]*ytable[i];
	}
	
#ifdef DEBUG
	fprintf(fstatus,"YZ_SCALE %f\n",inner_prod_yz / length_y);
#endif
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		ztable[i]-=ytable[i] * (inner_prod_yz / length_y);
	}

#ifdef DEBUG
	/*Sanity Check*/

	inner_prod_xy=inner_prod_xz=inner_prod_yz=0.0;
	length_x=length_y=length_z=0.0;
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		inner_prod_xy+=xtable[i]*ytable[i];
		inner_prod_xz+=xtable[i]*ztable[i];
		inner_prod_yz+=ytable[i]*ztable[i];
		
		length_x+=xtable[i]*xtable[i];
		length_y+=ytable[i]*ytable[i];
		length_z+=ztable[i]*ztable[i];
	}
	fprintf(fstatus,"INNER PRODUCTS OF TABLE AFTER GS WITH Y\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n",inner_prod_xy,inner_prod_xz,inner_prod_yz);
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n\n",length_x,length_y,length_z);
#endif

	/*We don't want to normalize but we can make the vectors the same length*/
	length_x=length_y=length_z=0.0;
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		length_x+=xtable[i]*xtable[i];
		length_y+=ytable[i]*ytable[i];
		length_z+=ztable[i]*ztable[i];
	}
	length_x = sqrt(length_x);
	length_y = sqrt(length_y);
	length_z = sqrt(length_z);
	
	length_y = length_x / length_y;
	length_z = length_x / length_z;
	
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		ytable[i]*=length_y;
		ztable[i]*=length_z;
	}

#ifdef DEBUG
	/*Sanity Check*/

	inner_prod_xy=inner_prod_xz=inner_prod_yz=0.0;
	length_x=length_y=length_z=0.0;
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		inner_prod_xy+=xtable[i]*ytable[i];
		inner_prod_xz+=xtable[i]*ztable[i];
		inner_prod_yz+=ytable[i]*ztable[i];
		
		length_x+=xtable[i]*xtable[i];
		length_y+=ytable[i]*ytable[i];
		length_z+=ztable[i]*ztable[i];
	}
	fprintf(fstatus,"INNER PRODUCTS OF TABLE AFTER RESCALE\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n",inner_prod_xy,inner_prod_xz,inner_prod_yz);
	fprintf(fstatus,"L2 NORMS AFTER GS\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n\n",length_x,length_y,length_z);
	
	length_x=length_y=length_z=0.0;
	length_x=fabs(xtable[0]);
	length_y=fabs(ytable[0]);
	length_z=fabs(ztable[0]);
	for(i=1;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		if(length_x < fabs(xtable[i])) length_x = fabs(xtable[i]);
		if(length_y < fabs(ytable[i])) length_y = fabs(ytable[i]);
		if(length_z < fabs(ztable[i])) length_z = fabs(ztable[i]);
	}
	fprintf(fstatus,"L-INFTY NORMS AFTER GS\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n\n",length_x,length_y,length_z);
	
	length_x=length_y=length_z=0.0;
	length_x=xtable[0];
	length_y=ytable[0];
	length_z=ztable[0];
	for(i=1;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		if(length_x < xtable[i]) length_x = xtable[i];
		if(length_y < ytable[i]) length_y = ytable[i];
		if(length_z < ztable[i]) length_z = ztable[i];
	}
	fprintf(fstatus,"MAX AFTER GS\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n\n",length_x,length_y,length_z);
	
	length_x=length_y=length_z=0.0;
	length_x=xtable[0];
	length_y=ytable[0];
	length_z=ztable[0];
	for(i=1;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		if(length_x > xtable[i]) length_x = xtable[i];
		if(length_y > ytable[i]) length_y = ytable[i];
		if(length_z > ztable[i]) length_z = ztable[i];
	}
	fprintf(fstatus,"MIN AFTER GS\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n\n",length_x,length_y,length_z);
	
	
	length_x=length_y=length_z=0.0;
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		length_x+=xtable[i];
		length_y+=ytable[i];
		length_z+=ztable[i];
	}
	length_x/=HYPERSPHERE_LOOKUP_TABLE_SIZE;
	length_y/=HYPERSPHERE_LOOKUP_TABLE_SIZE;
	length_z/=HYPERSPHERE_LOOKUP_TABLE_SIZE;
	fprintf(fstatus,"AVERAGE COMPONENT AFTER GS\n");
	fprintf(fstatus,"%2.5f\t%2.5f\t%2.5f\n\n",length_x,length_y,length_z);
	
	vectors = fopen("vectors.dat","w");
	for(i=0;i<HYPERSPHERE_LOOKUP_TABLE_SIZE;i++)
	{
		fprintf(vectors,"%3.2f\t%3.2f\t%3.2f\n",xtable[i],ytable[i],ztable[i]);
	}
#endif

}

list<QTree*> GradientDescentSearch::refine(QTree* source, ProjectedTree direction, int max_unresolution)
{
	const double MAX_INNER_PROD = 0.1;
	if(max_unresolution > 9)
	{
		//the requirement to make a list of possible partitions puts pratical limits on this value
		max_unresolution = 9;
	}
	//Get a list of partitions in source
	PartitionTree pt(source);
	for(std::list<Partition>::const_iterator i = pt.begin();i != pt.end();i++)
	{
		//consider if this partition should be in the resolution or not
		ProjectedTree d = lookup(*i);
		d*=-1;
		if(1)//if(d.dot(direction) > MAX_INNER_PROD)
		{
			//This branch should not be removed, as doing so moves against direction
		}
		else
		{
			//This branch should be considered, as doing so moves with direction
		}
	}
	//Unresolve source tree in direction
	
	//Determine list of possible partitions
	
	//Generate list of all posible resolutions
}

