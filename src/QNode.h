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
#ifndef QSODA_NODE 
#define QSODA_NODE
#include "QNodeInfo.h"

#define ISLEAF true
#define ISNTLEAF false

class QNode;

class LikelihoodNodeData;
class QNodeInfo;
class QNode;
class QNode
{
  public:
    QNode(  QNodeInfo& nptr, bool isLeaf ); // Create a leaf or tri
    QNode(  const QNode& node );  // Deep copy

	QNode& operator=(const QNode& node);

  private:
    QNode( QNodeInfo& nptr);  // Only for internal use, creates on piece of a tri
  public:
    virtual ~QNode();

    QNode*& external();
    QNode* external() const;
    QNode*& internal1();
    QNode* internal1() const;
    QNode*& internal2();
    QNode* internal2() const;
    QNode*& internal3();
    QNode* internal3() const;
    QNode*& child1();
    QNode* child1() const;
    QNode*& child2();
    QNode* child2() const;
    QNode*& child3();
    QNode* child3() const;
	QNode*& sibling();
	QNode* sibling() const;
    QNodeInfo*& nodeInfo();
    QNodeInfo* nodeInfo() const;
    bool marked();
    void setMarked();
    void clrMarked();
    double branchLength() const;
    double& branchLength();

    // Connect the external links to each other
    void connect( QNode* otherNode );
    // Connect the external links to each other
    void connectExternal( QNode* node1, QNode* node2 );
    // Insert new between old and his external
    void insert( QNode* newNode, QNode* oldNode );
    void print();
    int mHeight; //Height from the leaves


  protected:
    QNode*   mInternal; // points to next Node in ring if interior node, if leaf it is not used
    QNode*   mExternal;  // points to external (parent/child) node, if singleton points to NULL 
    QNodeInfo *mNodeInfo;    // Pointer to information associated with this Node
    bool    mMarked;    // Has this node been marked in building node list
    double mz;			// z for likelihood calculations
    double mBranchLength; //Hold the length of this branch if specified (-1 otherwise)
};
#endif

