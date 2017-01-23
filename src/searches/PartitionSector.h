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
#ifndef PSODA_PARTITION_SECTOR_
#define PSODA_PARTITION_SECTOR_

#include "QNode.h"
#include "QTree.h"
#include "Hypersphere.h"

class PartitionSector;
class PSNode;
class PartitionSectorSet;
class PSNodeInfo;



class PartitionSectorTree
{
	public:
		PartitionSectorTree(QTree*,Hypersphere*,PartitionSectorSet*);
		void preprocess();
		QTree* refine();
	protected:
		PSNode* mRoot;
		Hypersphere* map;
		PartitionSectorSet* pss;
		
		
		void copy(PSNode*& dest,QNode* root);
		
		
	private:
		PartitionSector post_order(PSNode* root);
		void pre_order(PSNode* root , PSNode* sibling);
		list<PSNodeInfo*> mNodeInfoList;
};

class PSNode
{
	public:
		PSNode(PSNodeInfo* info,QNode* node,bool isLeaf);
		PSNode(PSNodeInfo* info,QNode* node);
		bool leaf();
		
		PSNode*& external();
		PSNode* external() const;
		PSNode*& internal1();     
		PSNode* internal1() const;
		PSNode*& internal2();     
		PSNode* internal2() const;
		PSNode*& child1();        
		PSNode* child1() const;   
		PSNode*& child2();        
		PSNode* child2() const;    	
		PSNodeInfo* info() const;	
		int taxaNum() const;
		QNode* node()const;
	protected:
		
		PSNode(PSNodeInfo& info);
		PSNode* mInternal;
		PSNode* mExternal;
		PSNodeInfo* mNodeInfo;
		QNode* mQNode;
};

class PartitionSector
{
  public:	
	PartitionSector(int,int);
	PartitionSector(const PartitionSector&);
	
	PartitionSector & operator=(const PartitionSector&);
	PartitionSector & operator+=(const PartitionSector&);
	PartitionSector operator+(const PartitionSector&) const;
	
	double overlap(const PartitionSector&) const;
	static Hypersphere* map;
	int size() const;
	bool contains(const int)const;
  private:
	ProjectedTree position;
	int mSize;
	Partition part;
	int nTaxa;
	
};

class PSNodeInfo
{
	public:
		PSNodeInfo();
		~PSNodeInfo();
		PartitionSector pre;
		PartitionSector post;
		char* getCharacters();
		char* treeStr();
		PSNode* node;
	protected:
		char* mChars;
	private:
		void partialString(QNode*,char*);
};

class PartitionSectorSet
{
	public:
		PartitionSectorSet();
		double overlap(const PartitionSector&) const;
		void add(const PartitionSector&);
	private:
		list<PartitionSector> sectors;
};
#endif

