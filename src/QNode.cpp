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
#include "QNode.h"

/**
 * Create a new Node Tri without anything fancy
 *  The constructor for an internal node is 
 *  the only one who calls this.
 */

QNode::QNode( QNodeInfo& nodeInfo) 
: 
  mHeight( 0 ),
  mInternal( this ),
  mExternal( NULL ),
  mNodeInfo( &nodeInfo ),
  mMarked( false ),
  mz(0.0),
  mBranchLength( -1 )
{
}

QNode::QNode(const QNode& a)
:
  mHeight( a.mHeight ),
  mInternal( a.mInternal ),
  mExternal( a.mExternal ),
  mNodeInfo( a.mNodeInfo ),
  mMarked( a.mMarked ),
  mz(a.mz),
  mBranchLength( a.mBranchLength )
{
	//TODO implement deep copy
	printf("ALERT: QNode Copy Constructor called, but performed a shallow copy\n");
}


QNode& QNode::operator=(const QNode& a)
{
	mHeight = a.mHeight;
	mInternal = a.mInternal;
	mExternal = a.mExternal;
	mNodeInfo = a.mNodeInfo;
	mMarked = a.mMarked;
	mz = a.mz;
	mBranchLength = a.mBranchLength;
	
	//TODO implement deep copy
	printf("ALERT: QNode operator= called, but performed a shallow copy\n");
	return *this;
}

/**
 * If we arent a leaf, create the tri
 */
QNode::QNode( QNodeInfo& nodeInfo , bool leaf )
: 

  mHeight( 0 ),
  mInternal( this ),
  mExternal( NULL ),
  mNodeInfo( &nodeInfo ),
  mMarked( false ),
  mz(0.0),
  mBranchLength( -1 )
{
  if ( ! leaf )
  {
    internal1() = new QNode( nodeInfo );
    internal2() = new QNode( nodeInfo );
    internal1()->internal1()->internal1() = this;
  } 
}

/**
 * Delete the other two tri nodes, when you delete any one of them
 */
QNode::~QNode()
{
    QNode *intern1;
    QNode *intern2;
	if ( ! nodeInfo()->leaf() )
	{
        // Get pointers to the other two nodes
		intern1 = internal1();
        if(intern1)
            intern2 = internal2();
        else
            intern2 = NULL;
        // Delete their internal links so we wont recurse
        if(intern1)
        {
            intern1->mInternal = NULL;
            delete intern1;
        }
        if(intern2)
        {
            intern2->mInternal = NULL;
            delete intern2;
        }
        mInternal = NULL;
        mExternal = NULL;
    }
}

QNode*& QNode::external()           { return mExternal; }
QNode* QNode::external() const      { return mExternal; }
QNode*& QNode::internal1()          { return mInternal; }
QNode* QNode::internal1() const     { return mInternal; }
QNode*& QNode::internal2()          { return mInternal->mInternal; }
QNode* QNode::internal2() const     { return mInternal->mInternal; }
QNode*& QNode::internal3()          { return mInternal->mInternal->mInternal; }
QNode* QNode::internal3() const     { return mInternal->mInternal->mInternal; }
QNode*& QNode::child1()         { return mInternal->mExternal; }
QNode* QNode::child1() const    { return mInternal->mExternal; }
QNode*& QNode::child2()         { return mInternal->mInternal->mExternal; }
QNode* QNode::child2() const    { return mInternal->mInternal->mExternal; }
QNode*& QNode::child3()         { return mInternal->mInternal->mInternal->mExternal; }
QNode* QNode::child3() const    { return mInternal->mInternal->mInternal->mExternal; }

QNode*& QNode::sibling() {
	QNode*& brother = external();
	QNode* candidate = brother->child1();
	if (candidate->nodeInfo()->leaf() && candidate != this) {
		brother = candidate;
	} else {
		candidate = brother->child2();
		if (candidate->nodeInfo()->leaf() && candidate != this) {
			brother = candidate;
		} else {
			brother = brother->child3();
		}
	}
	if (!brother->nodeInfo()->leaf()) {
		printf("%s, %i - Sibling is not a leaf node!\n",__FILE__,__LINE__);
	}
	return brother;
}
QNode* QNode::sibling() const {
	QNode* brother = external();
	QNode* candidate = brother->child1();
	if (candidate->nodeInfo()->leaf() && candidate != this) {
		brother = candidate;
	} else {
		candidate = brother->child2();
		if (candidate->nodeInfo()->leaf() && candidate != this) {
			brother = candidate;
		} else {
			brother = brother->child3();
		}
	}
	if (!brother->nodeInfo()->leaf()) {
		printf("%s, %i - Sibling is not a leaf node!\n",__FILE__,__LINE__);
	}
	return brother;
}

QNodeInfo*& QNode::nodeInfo()         { return mNodeInfo;}
QNodeInfo* QNode::nodeInfo() const    { return mNodeInfo;}
bool QNode::marked()           { return mMarked; }
void QNode::setMarked() { mMarked = true; }
void QNode::clrMarked() { mMarked = false; }
double QNode::branchLength() const	{return mBranchLength;}
double& QNode::branchLength()		{return mBranchLength;}

// Connect the external links to each other
void QNode::connect( QNode* otherNode ) 
{ 
  connectExternal( this, otherNode ); 
}
/**
 * Connect External pointers to each other
 */
void QNode::connectExternal( QNode* node1, QNode* node2 )
{
	node1->mExternal = node2;
	node2->mExternal = node1;
}

/**
 * Insert new between old and his external
 */
void QNode::insert( QNode* newNode, QNode* oldNode )
{
    // Make new's 1st child the same as old's external
	connectExternal( newNode->internal1(), oldNode->mExternal );
    // link new and old together
	connectExternal( newNode, oldNode );
}

/**
 * Print yourself out
 */
void QNode::print()
{
    if(external())
    {
        printf("me %p, in %p, ex %p [%d], ",/*(unsigned int )*/this, internal1(), external(), external()->nodeInfo()->nodeIndex());
    }
    else
    {
        printf("me %p, in %p, ex %p, ",this, internal1(), external());
    }
}

