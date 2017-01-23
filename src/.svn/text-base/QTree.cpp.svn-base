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
#include "QTree.h"
#include "Interpreter.h"
#include "Dataset.h"
#include "PsodaPrinter.h"
#include <assert.h>

#include <limits>
#include <string.h>

const char* branch_length_fmt = "- %.3f -";

using namespace std;

QTree::QTree()
:
mEarlyPrune(INT_MAX),
mNNodes( 0 ),
mNodeDataInitialized(false),
mNodeDataDAInitialized(false),
mNodeDataMLInitialized(false),
mRoot( NULL ),
mStart( NULL),
mSplit(false),
mAllVisited(false),
mScore(std::numeric_limits<double>::max()),
scored(false),
mTreeString(),
mTreeStr(NULL),
mNodeInfo(),
mNodeInfoIter(),
mNodeInfo1(),
mNodeInfoIter1(),
mNodeInfo2(),
mNodeInfoIter2(),
mSubTree1( NULL ),
mSubTreeScore1(std::numeric_limits<double>::max()),
mSubTree2( NULL ),
mSubTreeScore2(std::numeric_limits<double>::max()),
mSplitNode1(NULL),
mSplitNode2(NULL),
hasConstraints(false),
mNodeInfoConstraints(),
mIterQueueIndex(0),								  // Only do this for a brand new tree
mIterQueueIndexCount(0),
mIterQueue(),
mIterQueueInitialized(false)
{
	resetScore();
	mNodeInfo1.clear();
	mNodeInfo2.clear();
}


QTree::QTree(const QTree& orig):
mEarlyPrune(INT_MAX),
mNNodes( orig.mNNodes ),
mNodeDataInitialized(false),
mNodeDataDAInitialized(false),
mNodeDataMLInitialized(false),
mRoot( NULL ),
mStart( NULL),
mSplit(orig.mSplit),
mAllVisited(false),								  //Start over if you clone
mScore(orig.mScore),
scored(orig.scored),
mTreeString(),
mTreeStr(strdup(orig.mTreeStr)),
mNodeInfo(),
mNodeInfoIter(),
mNodeInfo1(),
mNodeInfoIter1(),
mNodeInfo2(),
mNodeInfoIter2(),
mSubTree1( NULL ),
mSubTreeScore1(std::numeric_limits<double>::max()),
mSubTree2( NULL ),
mSubTreeScore2(std::numeric_limits<double>::max()),
mSplitNode1(NULL),
mSplitNode2(NULL),
hasConstraints(false),
mNodeInfoConstraints(),
mIterQueueIndex(0),
mIterQueueIndexCount(0),
mIterQueue(),
mIterQueueInitialized(false)

{
	resetScore();
//We need to traverse the tree to see which nodinfos we have to copy.  There is no way to copy the nodeinfo
//  objects and then tie them to nodes.
// If the tree is split, copy the two subtrees
	if(orig.isSplit())
	{
		printf("QTree constructor ERROR!! Should not create with split orig tree\n");
	}
	else										  // Tree is not split
	{
// We dont need the subtree lists, since we know this is not split
		recurseCopy( root(), orig.root());
		recurseCopy( root()->external(), orig.root()->external());
		if ( root()->external() )
		{
			root()->external()->external() = root();
		}
	}
	mNodeInfo1.clear();
	mNodeInfo2.clear();
	isSplit() = false;
	mIterQueueIndex = orig.mIterQueueIndex;
	mIterQueueIndexCount= 0;
}


QTree::QTree( const QTree* orig):
mEarlyPrune(INT_MAX),
mNNodes( orig->mNNodes ),
mNodeDataInitialized(false),
mNodeDataDAInitialized(false),
mNodeDataMLInitialized(false),
mRoot( NULL ),
mStart( NULL),
mSplit(orig->mSplit),
mAllVisited(false),								  //Start over if you clone
mScore(orig->mScore),
scored(orig->scored),
mTreeString(),
mTreeStr(NULL),
mNodeInfo(),
mNodeInfoIter(),
mNodeInfo1(),
mNodeInfoIter1(),
mNodeInfo2(),
mNodeInfoIter2(),
mSubTree1( NULL ),
mSubTreeScore1(std::numeric_limits<double>::max()),
mSubTree2( NULL ),
mSubTreeScore2(std::numeric_limits<double>::max()),
mSplitNode1(NULL),
mSplitNode2(NULL),
hasConstraints(false),
mNodeInfoConstraints(),
mIterQueueIndex(0),
mIterQueueIndexCount(0),
mIterQueue(),
mIterQueueInitialized(false)

{
	if(orig->mTreeStr)
		mTreeStr = strdup(orig->mTreeStr);
	resetScore();
//We need to traverse the tree to see which nodinfos we have to copy.  There is no way to copy the nodeinfo
//  objects and then tie them to nodes.
// If the tree is split, copy the two subtrees
	if(orig->isSplit())
	{
		printf("QTree constructor ERROR!! Should not create with split orig tree\n");
	}
	else										  // Tree is not split
	{
// We dont need the subtree lists, since we know this is not split
		recurseCopy( root(), orig->root());
		recurseCopy( root()->external(), orig->root()->external());
		if ( root()->external() )
		{
			root()->external()->external() = root();
		}
	}
	mNodeInfo1.clear();
	mNodeInfo2.clear();
	isSplit() = false;
	mIterQueueIndex = orig->mIterQueueIndex;
	mIterQueueIndexCount= 0;
}


QTree::~QTree()
{
	clean();
}


/**
 * Dealocate Nodes, Nodinfo's and lazy evaluation scoring data
 */
void QTree::clean()
{
	QDeque<QNodeInfo *>::iterator nodePointer;

// Clear the main list, it will delete everything
	for(nodePointer = mNodeInfo.begin(); nodePointer != mNodeInfo.end(); nodePointer++)
	{
// first delete the node
		delete (*nodePointer)->node();
// Now delete the nodeInfo structure
		delete *nodePointer;
	}
	mNodeInfo.clear();
	mNodeInfo1.clear();
	mNodeInfo2.clear();
	mNodeInfoConstraints.clear();
	if(mTreeStr)
	{
		free(mTreeStr);
		mTreeStr = NULL;
	}
}


const QTree& QTree::operator=( const QTree& orig )
{
	if ( this != &orig )
	{
//We need to traverse the tree to see which nodinfos we have to copy.  There is no way to copy the nodeinfo
//  objects and then tie them to nodes.
// We dont need the subtree lists, since this is used to create a tree once we have the optimal join points
		recurseCopy( root(), orig.root());
		recurseCopy( root()->external(), orig.root()->external());
		if ( root()->external() )
		{
			root()->external()->external() = root();
		}
		updateTreeString();
	}
	return *this;
}


double QTree::getScore() const
{
	return mScore;
}


void QTree::setScore(double newScore)
{
	mScore = newScore;
	scored = true;
}


void QTree::resetScore()
{
	setScore((double)DBL_MAX);
	scored = false;
}


bool QTree::isScored()
{
	return scored;
}


int* QTree::weights() const
{
	return Interpreter::getInstance()->dataset()->weights();
}


void QTree::recurseCopy( QNode*& dest, QNode* src)
{
	QNodeInfo *newNodeInfo;
	if ( src )
	{
		if ( src->nodeInfo()->leaf() )
		{
			newNodeInfo = new QNodeInfo(src->nodeInfo()->nodeIndex(), ISLEAF);
			mNodeInfo.push_back(newNodeInfo);	  // Insert NodeInfo into the list on the new tree
												  // Create a leaf
			dest = new QNode(*newNodeInfo, ISLEAF);
			newNodeInfo->node() = dest;
			dest->internal1() = dest;
		}
		else
		{
			newNodeInfo = new QNodeInfo(src->nodeInfo()->nodeIndex(), ISNTLEAF);
			mNodeInfo.push_back(newNodeInfo);	  // Insert NodeInfo into the list on the new tree
												  // Create an internal
			dest = new QNode(*newNodeInfo, ISNTLEAF);
			newNodeInfo->node() = dest;

// The copy will point my external at the child
			recurseCopy( dest->child1(), src->child1());
// Now point the childs external at me
			dest->child1()->external() = dest->internal1();

// The copy will point my external at the child
			recurseCopy( dest->child2(), src->child2());
// Now point the childs external at me
			dest->child2()->external() = dest->internal2();
		}
	}
}


QNode*& QTree::subTree1() { return mSubTree1; }
QNode* QTree::subTree1() const { return mSubTree1; }
QNode*& QTree::subTree2() { return mSubTree2; }
QNode* QTree::subTree2() const { return mSubTree2; }
QNode*& QTree::splitNode1() { return mSplitNode1; }
QNode* QTree::splitNode1() const { return mSplitNode1; }
QNode*& QTree::splitNode2() { return mSplitNode2; }
QNode* QTree::splitNode2() const { return mSplitNode2; }

bool& QTree::nodeDataInitialized() { return mNodeDataInitialized; }
bool QTree::nodeDataInitialized() const { return mNodeDataInitialized; }
bool& QTree::nodeDataDAInitialized() { return mNodeDataDAInitialized; }
bool QTree::nodeDataDAInitialized() const { return mNodeDataDAInitialized; }
bool& QTree::nodeDataMLInitialized() { return mNodeDataMLInitialized; }
bool QTree::nodeDataMLInitialized() const { return mNodeDataMLInitialized; }
// This is pretty ugly, but parsimony needs to get to the nodinfo list
QDeque<QNodeInfo *>* QTree::nodeInfoList() {return(&mNodeInfo);}

/**
 * First update the newick string, then print it
 * @return Side Effects: mTreeString class variable is updated
 */
void QTree::print()
{

	QDeque<QNodeInfo *>::iterator nodePointer;
	PsodaPrinter* printBuffer = PsodaPrinter::getInstance();

	printBuffer->write("treestring= %s\n",treeStr());
// Comment out the return to get more details
	return;
// Print out the main nodeInfo list
	printf("root %p[%d]\n",root(),root()->nodeInfo()->nodeIndex());
	printf("Main Node List\n");
	for(nodePointer = mNodeInfo.begin(); nodePointer != mNodeInfo.end(); nodePointer++)
	{
		(*nodePointer)->print();
	}
	return;
// Print out the subtree 1 nodeInfo list
	printf("\n Nodeinfo 1\n");
	for(nodePointer = mNodeInfo1.begin(); nodePointer != mNodeInfo1.end(); nodePointer++)
	{
		(*nodePointer)->print();
	}
	printf("\n Nodeinfo 2\n");
	for(nodePointer = mNodeInfo2.begin(); nodePointer != mNodeInfo2.end(); nodePointer++)
	{
		(*nodePointer)->print();
	}
}


char * QTree::treeStr()
{
	if(mTreeStr==NULL)
		updateTreeString();
	return (mTreeStr);
		
}


void QTree::setTreeStr(char *newStr)
{
	if(mTreeStr)
	{
		free(mTreeStr);
	}
  if(newStr != NULL) {
		mTreeStr = strdup(newStr);
	} 
}


void QTree::updateTreeString()
{
	updateTreeString(0, false);
}


void QTree::updateTreeString(int whichTree, bool names)
{
	updateTreeString(whichTree, names, true);
}


#define MAXNAMESTRING 100
#define MAXTREESTRING 100000
void appendNodeName(char *buf, QNode *treeNode, bool names)
{
	char intBuf[MAXNAMESTRING];
	if(names)
	{
		g_strlcat(buf, treeNode->nodeInfo()->getTaxonName(), MAXTREESTRING);
	}
	else
	{
		sprintf(intBuf, "%d", treeNode->nodeInfo()->nodeIndex()+1);
		g_strlcat(buf, intBuf, MAXTREESTRING);
	}
}


void debugTreeString(QNode *current)
{
	if(current->child1()->external() != current->internal1())
	{
		printf("ERROR!!!!!! current myid %d(internal1 %p), child1 %d(child external %p)\n",
			current->nodeInfo()->nodeIndex(),
			(current->internal1()),
			current->child1()->nodeInfo()->nodeIndex(),
			(current->child1()->external()));
		printf("I am ");
		current->print();
		printf("\nChild1 is ");
		current->child1()->print();
		printf("\n");
	}
	if(current->child2()->external() != current->internal2())
	{
		printf("ERROR!!!!!! current %d(%p), child2 %d(%p)\n",
			current->nodeInfo()->nodeIndex(),
			(current->internal2()),
			current->child2()->nodeInfo()->nodeIndex(),
			(current->child2()->external()));
		printf("I am ");
		current->print();
		printf("\nChild2 is ");
		current->child1()->print();
		printf("\n");
	}
	if((current->child1() == current)||(current->child2() == current))
	{
		printf("ERROR!!!!!! Tree contains a lop\n");
	}
}


/**
 * Update the Newick string representing this tree
 * Since we have these tri node structures, the node that root points to can
 *   really be thought of as the 1st child as far as a normal rooted tree
 *   After you get the substructure of this child printed out, you need to account
 *   for the logical 2nd child which comes from the root's external
 * If you dont want a rooted tree, you do the two children, then put them in a tri with the external
 * @param Inputs: Nodes in this tree
 * @return Side Effects: mTreeString class variable is updated
 */
void QTree::updateTreeString(int whichTree, bool names, bool rooted __attribute__((unused)) )
{
	QNode *treeNode;

	char TreeStringBuffer[MAXTREESTRING] = "";;

// Pick the tree we want to look at
	if(whichTree == 0)
		treeNode = root();
	else if(whichTree == 1)
		treeNode = subTree1();
	else
		treeNode = subTree2();

// If there was a root
	if(!treeNode)
	{
		printf("updateTreeString: no root\n");
		setTreeStr(TreeStringBuffer);
		return;
	}
// If this tree has a root that is a leaf
	if(treeNode->nodeInfo()->leaf())
	{
// If there is a parent, then use him as the root, otherwise just use yourself
		if(treeNode->external() != NULL)
		{
			treeNode = treeNode->external();
// This is a tree with a single node
		}
		else
		{
			appendNodeName(TreeStringBuffer, treeNode, names);
			setTreeStr(TreeStringBuffer);
			return;
		}
	}
// If we make it here, we have a treeNode that is an internal node
// If rooted, then put parens around the two children,
// then a comma, then the external, then a close paren
	g_strlcat(TreeStringBuffer, "(", MAXTREESTRING);
	if(rooted)
	{
		inorderRecurse(treeNode,TreeStringBuffer,names);
	}
	else
	{
		inorderRecurse(treeNode->child1(),TreeStringBuffer,names);
		g_strlcat(TreeStringBuffer, ",", MAXTREESTRING);
		inorderRecurse(treeNode->child2(),TreeStringBuffer,names);
	}
	g_strlcat(TreeStringBuffer, ",", MAXTREESTRING);
	if(treeNode->external())
	{
		inorderRecurse(treeNode->external(),TreeStringBuffer,names);
	}
												  // This first set of parens goes around the external and the root's descendents
	g_strlcat(TreeStringBuffer, ")", MAXTREESTRING);
// Allocate space to hold a permanent copy
	setTreeStr(TreeStringBuffer);
}


/**
 * Recurse through the tree to create the mTreeString newick representation
 * @param current The current node we are examining.
 * @return Side Effects: mTreeString class variable is updated
 */
void QTree::inorderRecurse(QNode *& current, char *buf, bool names)
{
	if(current == NULL)
	{
		printf("inorderRecurse: NULL\n");
		return;
	}
	if(current->nodeInfo()->leaf())
	{
		appendNodeName(buf, current, names);
		return;
	}

// If this is a single node tree, dont recurse

	if(current->child1() == NULL)
	{
		return;
	}
#ifdef debugTree
	debugTreeString(current);
	printf("\n%s\n",buf);
#endif
	g_strlcat(buf, "(", MAXTREESTRING);
	inorderRecurse(current->child1(),buf, names);
	g_strlcat(buf, ",", MAXTREESTRING);
	inorderRecurse(current->child2(),buf, names);
	g_strlcat(buf, ")", MAXTREESTRING);
}


/**
 * Recurse through the tree in posfix order to cannonicalize the tree.
 *  If the 2nd child node number is less than the 1st, switch them
 * @param current The current node we are examining
 * @return Side Effects: mTreeString class variable is updated
 */
int QTree::postorderRecurse(QNode *& current)
{
	int firstMinNode;
	int secondMinNode;
	QNode *minChild;
	QNode *maxChild;
	double tmpBranchLength;

	if(current->nodeInfo()->leaf())
	{
		return(current->nodeInfo()->nodeIndex());
	}
	else
	{
		firstMinNode = postorderRecurse(current->child1());
		secondMinNode = postorderRecurse(current->child2());
		if(secondMinNode < firstMinNode)
		{
// Swap the two children
			minChild = current->child2();
			maxChild = current->child1();
#ifdef debugTree
			printf("swapping kids, I am %d, 1st was %d(%d), 2nd was %d(%d)\n",
				current->nodeInfo()->nodeIndex(), current->child1()->nodeInfo()->nodeIndex(), firstMinNode,
				current->child2()->nodeInfo()->nodeIndex(), secondMinNode);
#endif

			tmpBranchLength = current->internal1()->branchLength();
			current->internal1()->connect(minChild); current->internal1()->branchLength() = current->internal2()->branchLength();
			current->internal2()->connect(maxChild); current->internal2()->branchLength() = tmpBranchLength;
// Update this value since we are going to return it
			firstMinNode = secondMinNode;
#ifdef debugTree
			printf("after swapping kids, I am %d, 1st was %d(%d), 2nd was %d(%d)\n",
				current->nodeInfo()->nodeIndex(), current->child1()->nodeInfo()->nodeIndex(), firstMinNode,
				current->child2()->nodeInfo()->nodeIndex(), secondMinNode);
#endif
		}
		return(firstMinNode);
	}
}


QNode*& QTree::root() { return mRoot; }
QNode*  QTree::root() const{ return mRoot;}
QNode*& QTree::start() { return mStart; }
QNode*  QTree::start() const { return mStart; }

/**
 * Cannonicalize the tree.  The subtree with the lowest node number descendent should be
 * in child1.  First do a depth first search and mark the minChildnodeIndex for each node.
 * Then switch children if they are out of order.
 * @param Inputs: Nodes in this tree
 * @return Side Effects: The tree will be reordered.
 */
void QTree::canonicalize()
{
	QNode *minChild;
	QNode *midChild;
	QNode *maxChild;
	int minChildNodeNumber;
	int midChildNodeNumber;
	int maxChildNodeNumber;
	QNode *tmpChild;
	int tmpNode;
	double tmpBranchLength;
	QNode *firstNode;
	QDeque<QNodeInfo *>::iterator nodePointer;

	int minNumber = INT_MAX;

	firstNode = NULL;

// First we need to root the tree by pointing the root at the first taxon's parent
// Find the node with the min Number
	for(nodePointer = mNodeInfo.begin(); nodePointer != mNodeInfo.end(); nodePointer++)
	{
		// If this is not a bifurcating tree, dont process
		if(*nodePointer && ((*nodePointer)->node()->internal3() != (*nodePointer)->node())) {
		return;
		}

		// If this node has branch lengths dont process
		if((*nodePointer)->node()->branchLength() < 0.0) return;
		if((*nodePointer)->node()->internal1()->branchLength() < 0.0) return;
		if((*nodePointer)->node()->internal2()->branchLength() < 0.0) return;

		if(*nodePointer && ((*nodePointer)->nodeIndex() >= 0) && ((*nodePointer)->nodeIndex() < minNumber))
		{
			firstNode = (*nodePointer)->node();
			minNumber = firstNode->nodeInfo()->nodeIndex();
		}
	}

	if(firstNode==NULL) return;

// Everyone should have a external unless this is a single node tree
	if(firstNode->external())
	{
		root() = firstNode->external();
	}
	else
	{
// This is a single node tree, so return
		return;
	}

// Since there is really no root with the tri, we will get the values from 1st child, 2nd child
//  and external and then put them in order.  Everone else can just switch their children.

// We want the min on 1st child, mid on 2nd child and max on external
// OK, we are going to bubble sort this, you only need 3 iterations
//  The 3 iterations are embodied in the if statements below
	minChild = root()->child1();
	minChildNodeNumber = postorderRecurse(minChild);
	midChild = root()->child2();
	midChildNodeNumber = postorderRecurse(midChild);
	maxChild = root()->external();
	maxChildNodeNumber = postorderRecurse(maxChild);

	if(minChildNodeNumber > midChildNodeNumber)
	{
// Swap min and mid
		tmpChild = minChild; tmpNode = minChildNodeNumber; tmpBranchLength = minChild->branchLength();
		minChild = midChild; minChildNodeNumber = midChildNodeNumber; minChild->branchLength() = midChild->branchLength();
		midChild = tmpChild; midChildNodeNumber = tmpNode; midChild->branchLength() = tmpBranchLength;
	}
	if(midChildNodeNumber > maxChildNodeNumber)
	{
// Swap mid and max
		tmpChild = midChild; tmpNode = midChildNodeNumber; tmpBranchLength = midChild->branchLength();
		midChild = maxChild; midChildNodeNumber = maxChildNodeNumber; midChild->branchLength() = maxChild->branchLength();
		maxChild = tmpChild; maxChildNodeNumber = tmpNode; maxChild->branchLength() = tmpBranchLength;
	}
	if(minChildNodeNumber > midChildNodeNumber)
	{
// Swap min and mid
		tmpChild = minChild; tmpNode = minChildNodeNumber; tmpBranchLength = minChild->branchLength();
		minChild = midChild; minChildNodeNumber = midChildNodeNumber; minChild->branchLength() = midChild->branchLength();
		midChild = tmpChild; midChildNodeNumber = tmpNode; midChild->branchLength() = tmpBranchLength;
	}

	root()->internal1()->connect(minChild);
	root()->internal2()->connect(midChild);
	root()->internal3()->connect(maxChild);
}


int& QTree::nnodes() { return mNNodes; }
int QTree::nnodes() const { return mNNodes; }

bool& QTree::visited()							  /* Have all split points been visited */
{
	return mAllVisited;
}


bool QTree::visited() const						  /* Have all split points been visited */
{
	return mAllVisited;
}


bool& QTree::isSplit()							  // This tree is already split
{
	return mSplit;
}


bool QTree::isSplit()  const					  // This tree is already split
{
	return mSplit;
}


/**
 * Find the next split point and split yourself
 *  Mark this splitpoint as visited and keep track of the nodes you removed so they
 *  can be joined back again.
 * Returns true if it worked, false if everything was visited.
 */
bool QTree::split()
{
// Look through NodeInfo's to find one that hasnt been visited.
	QNodeInfo *splitNodeInfo = findSplitPoint();
// If all nodes are visited
	if(splitNodeInfo == NULL)
	{
		visited() = true;
		return(false);
	}
	QNode *splitPoint = splitNodeInfo->node();
	//printf("Split node %d\n",splitNodeInfo->nodeIndex());
// Mark this splitpoint as visited
	splitNodeInfo->visited() = true;

#ifdef debugTree
// Debug
	printf("split() original tree\n");
	print();
	printf("split: splitnode %d external %d\n",splitNodeInfo->nodeIndex(),splitPoint->external()->nodeInfo()->nodeIndex());
// Debug
#endif
	splitUtil( splitPoint->external(), SUBTREE1);
	splitUtil( splitPoint, SUBTREE2);
#ifdef debugTree
	printf("split() after splitutil subtree1 %d, subtree2 %d\n",mSubTree1->nodeInfo()->nodeIndex(),mSubTree2->nodeInfo()->nodeIndex());
#endif

// Now copy the nodes for each tree
	makeNodeLists();
	isSplit() = true;

#ifdef debugTree
	printf("split leaving\n");
	print();
#endif
	return(true);
}


// Split the tree and point the subtree identified by whichSubTree at the node
void QTree::splitUtil(QNode *splitNode, int whichSubTree)
{
#ifdef debugConstraintList
	bool found = false;
	for(deque<QNodeInfo *>::iterator iter = mNodeInfoConstraints.begin();iter != mNodeInfoConstraints.end();iter++)
		if(*iter == splitNode->nodeInfo())
			found = true;
	if(!found)
		printf("!!!ERROR split node %d not in constraint list!!!\n",splitNode->nodeInfo()->nodeIndex());
#endif
// node is a leaf
#ifdef debugTree
	printf("splitUtil: splitnode leaf? %d, id %d\n",splitNode->nodeInfo()->leaf(),splitNode->nodeInfo()->nodeIndex());
#endif
	if (splitNode->nodeInfo()->leaf())
	{
// We dont end up with an extra NodeInfo here
		if(whichSubTree == SUBTREE1)
			mSubTree1 = splitNode;
		else
			mSubTree2 = splitNode;
		splitNode->external() = NULL;			  // Sever the link to the rest of the tree
	}
// node is an interior
	else
	{
		if(whichSubTree == SUBTREE1)
			mSubTree1 = splitNode->child1();	  // point the tree at an arbitrary child
		else
			mSubTree2 = splitNode->child1();	  // point the tree at an arbitrary child

		splitNode->child1()->connect(splitNode->child2());
#ifdef debugTree
		printf("connected children %d = %X, %d = %X\n",
			splitNode->child1()->nodeInfo()->nodeIndex(),
			splitNode->child1()->external(),
			splitNode->child2()->nodeInfo()->nodeIndex(),
			splitNode->child2()->external());
// node is now orphaned, so save it for next time
#endif
		saveJoinNode(splitNode,whichSubTree);
	}
}


/**
 * Find a place to split the tree that hasnt been used before
 *  We may want to start at the end, so you break the tree near the root
 *  first.  Maybe random would be better, but this is hard with a linked
 *  list, so try starting from the end
 * Returns: A pointer to the NodeInfo structure or NULL if none were found
 */
QNodeInfo *QTree::findSplitPoint()
{
	QNodeInfo *nodeInfo;
// For now, initialize the iterator every time, later, have the search initialize
//  it so we dont look so far to find an unused nodeinfo
	if(!mIterQueueInitialized)
	{
		initNodeInfoIter();						  // Initialize the iterator
		mIterQueueInitialized = true;
	}
	nodeInfo = nextNodeInfo();
	if(hasConstraints)
	{
		for(deque<QNodeInfo *>::iterator iter = mNodeInfoConstraints.begin();iter != mNodeInfoConstraints.end();iter++)
			if( !(*iter)->visited())
				return *iter;
		return NULL;
	}
	else
	{
		while(nodeInfo &&(nodeInfo->visited()))
		{
			nodeInfo = nextNodeInfo();
		// If we are at the end
			if(nodeInfo == NULL)
				return(NULL);
		}
	}
//    printf("Split Point %d[%X]\n",nodeInfo->nodeIndex(),nodeInfo);
	return(nodeInfo);
}


// Just join the tree together in the same way it was split
void QTree::join()
{
#ifdef debugTree
	printf("join before %d and %d\n",mSubTree1->nodeInfo()->nodeIndex(),mSubTree2->nodeInfo()->nodeIndex());
	print();
#endif
	QNode *joinPoint1 = joinUtil(mSubTree1, SUBTREE1);
	QNode *joinPoint2 = joinUtil(mSubTree2, SUBTREE2);
	joinPoint1->connect(joinPoint2);
// The nodeinfo list is still good, so clear the sublists
	mNodeInfo1.clear();
	mNodeInfo2.clear();
	isSplit() = false;
#ifdef debugTree
	printf("join after %d and %d\n",mSubTree1->nodeInfo()->nodeIndex(),mSubTree2->nodeInfo()->nodeIndex());
	print();
#endif
}


// hook up A new node to a node and its external
QNode *QTree::joinUtil(QNode *subTree, int whichSubTree)
{
// If this node had an external before, hook up a new node between him and his external
// The only time this is not true is if you are adding a new node as a single node tree
	if (subTree->external() != NULL)
	{
// Get node to join with
		QNode *joinNode = getJoinNode(whichSubTree);

// Connect our second child to the node pointed to by the subtrees external
		joinNode->internal2()->connect(subTree->external());
// Connect our first child to the subtree node
		joinNode->internal1()->connect(subTree);
		return(joinNode);
	}
	else
	{
		return(subTree);
	}
}


// Get a recycled join node, or create a new one
QNode *QTree::getJoinNode(int whichSubTree)
{
	QNode *joinNode;
	if(whichSubTree == SUBTREE1)				  // Recycle this old split node
	{
		joinNode = splitNode1();
		splitNode1() = NULL;
		if(joinNode)
			return(joinNode);
	}
	else if(whichSubTree == SUBTREE2)
	{
		joinNode = splitNode2();
		splitNode2() = NULL;
		if(joinNode)
			return(joinNode);
	}
// This is in the case of a single node tree being joined, we need a new ancestor
	QNodeInfo *joinNodeInfo = new QNodeInfo(nnodes()++, ISNTLEAF);
	joinNode = new QNode(*joinNodeInfo,ISNTLEAF);
	joinNodeInfo->node() = joinNode;
	insertNodeInfo(joinNodeInfo);				  // Insert the internal node on the tree list
	return(joinNode);
}


// Save the join node for later use in a join
// This assumes that we will only have two to save at any time.
void QTree::saveJoinNode(QNode *saveNode,int whichSubTree)
{
	if(whichSubTree == SUBTREE1)
	{
#ifdef debugTree
		printf("Saving Join node in 1 %d\n",saveNode->nodeInfo()->nodeIndex());
#endif
		splitNode1() = saveNode;
	}
	else
	{
#ifdef debugTree
		printf("Saving Join node in 2 %d\n",saveNode->nodeInfo()->nodeIndex());
#endif
		splitNode2() = saveNode;
	}
// Get rid of all the externals
	saveNode->external() = NULL;
	saveNode->internal1()->external() = NULL;
	saveNode->internal2()->external() = NULL;
}


// Create the two subtree lists from the subtrees
void QTree::makeNodeLists()
{
// First clear the lists.  The nodes are still in the main list so we dont have to delete them
	mNodeInfo1.clear();
	mNodeInfo2.clear();

	makeNodeList(subTree1(), 1);
	makeNodeList(subTree2(), 2);
}


// Recurse through the tree and put all of the nodes you find on the Node list associated with the subtree
// You need a seperate method to kick things off because the first one can have 3 connections
void QTree::makeNodeList( QNode* root, int whichList )
{
// Put self on the list
	if(whichList == 1)
	{
		insertNodeInfo1(root->nodeInfo());
	}
	else
	{
		insertNodeInfo2(root->nodeInfo());
	}
// If we are a leaf, recurse through external
	if(root->nodeInfo()->leaf())
	{
// If there is an external link, follow it
		if(root->external() != NULL)
			recurseMakeNodeList(root->external(), whichList);
	}
	else
	{
// Recurse through all 3 children
		if(root->child1() != NULL)
			recurseMakeNodeList(root->child1(), whichList);
		if(root->child2() != NULL)
			recurseMakeNodeList(root->child2(), whichList);
		if(root->child3() != NULL)
			recurseMakeNodeList(root->child3(), whichList);
	}
}


void QTree::recurseMakeNodeList(QNode* node, int whichList)
{
// Put yourself on the list, then traverse the children
	if(whichList == 1)
	{
		insertNodeInfo1(node->nodeInfo());
	}
	else
	{
		insertNodeInfo2(node->nodeInfo());
	}

// Traverse the children
	if(!node->nodeInfo()->leaf())
	{
		if(node->child1() != NULL)
			recurseMakeNodeList(node->child1(), whichList);
		if(node->child2() != NULL)
			recurseMakeNodeList(node->child2(), whichList);
	}
}


// Create a new tree by joining at the two join points
// This is called on an existing tree and returns a new tree
// Hook the two nodes together, create a new tree, and unhook them
QTree *QTree::createSpecificTree(QNodeInfo *node1, QNodeInfo *node2, bool saveOrigTree)
{
	QTree *newTree;
	QNode *joinPoint1;
	QNode *joinPoint2;
	QNode *tmpSubTree1;
	QNode *tmpSubTree2;

	tmpSubTree1 = subTree1();
	tmpSubTree2 = subTree2();
// Debug
#ifdef debugTree
	print();
// Debug
#endif
	joinPoint1 = joinUtil( node1->node(), SUBTREE1);
	joinPoint2 = joinUtil( node2->node(), SUBTREE2);
	joinPoint1->connect(joinPoint2);

#ifdef debugTree
// Debug
	printf("createspecificTree after join Joinpoints %d and %d\n",joinPoint1->nodeInfo()->nodeIndex(),joinPoint2->nodeInfo()->nodeIndex());
	print();
// Debug
#endif
	isSplit() = false;
	if(saveOrigTree)
	{
// If we need to reconstruct the original, create a copy of the joined tree
//  And then split the original
// We have to do this because the joinpoints only work on the original
		newTree = new QTree(this);
		splitUtil(joinPoint1, SUBTREE1);
		splitUtil(joinPoint2, SUBTREE2);
		isSplit() = true;
// Restore the saved subtrees
		subTree1() = tmpSubTree1;
		subTree2() = tmpSubTree2;
		newTree->setTreeStr(treeStr());
#ifdef debugTree
// Debug
		printf("createspecificTree after resplit Joinpoints %d and %d\n",joinPoint1->nodeInfo()->nodeIndex(),joinPoint2->nodeInfo()->nodeIndex());
		print();
		printf("createspecific, new tree created\n");
		newTree->print();
// Debug
#endif

	}
	else
	{
		newTree = this;
	}
	newTree->updateTreeString();				  // Update the string so it can be matched with repository
	
	//if(0)
	if(hasConstraints)
	{
		newTree->hasConstraints = true;
		for(deque<QNodeInfo*>::iterator constraint_iter = mNodeInfoConstraints.begin();constraint_iter!=mNodeInfoConstraints.end();constraint_iter++)
		{
			for(QDeque<QNodeInfo *>::iterator iter = newTree->mNodeInfo.begin();iter != newTree->mNodeInfo.end();iter++)
			{
				if((*constraint_iter)->nodeIndex() == (*iter)->nodeIndex())
					newTree->addConstraint(*iter);
			}
		}
	}
	
	
	return(newTree);
}


// Insert Node into master list
void  QTree::insertNodeInfo(QNodeInfo *nodeInfo)
{
	mNodeInfo.push_back(nodeInfo);
}


// Insert Node into list 1
void  QTree::insertNodeInfo1(QNodeInfo *nodeInfo)
{
	if(hasConstraints)
	{
		for(deque<QNodeInfo*>::iterator iter = mNodeInfoConstraints.begin();iter!=mNodeInfoConstraints.end();iter++)
		{
			if((*iter) == nodeInfo)
				mNodeInfo1.push_back(nodeInfo);
		}
	}
	else
	{
		mNodeInfo1.push_back(nodeInfo);
	}
}


// Insert Node into list 2
void  QTree::insertNodeInfo2(QNodeInfo *nodeInfo)
{
	if(hasConstraints)
	{
		//this node info must be in the constraint list
		for(deque<QNodeInfo*>::iterator iter = mNodeInfoConstraints.begin();iter!=mNodeInfoConstraints.end();iter++)
		{
			if((*iter) == nodeInfo)
				mNodeInfo2.push_back(nodeInfo);
		}
	}
	else
	{
		mNodeInfo2.push_back(nodeInfo);
	}
}


// Point the iterator at the beginning of the list
void  QTree::initNodeInfoIter()
{
//   mNodeInfoIter = mNodeInfo.begin();
// Create a new list of nodes that starts with the root nodes and
//  ends with the leaf nodes.  Then you can start with swapping
//  big branches and end by moving leaves.  This should make it so
//  your score decreases rapidly at the first of the run and
//  should alow you to do more early pruning.

	QDeque <QNodeInfo *>::iterator nodeIter;	  // Iterator to use in acccessing list
	mIterQueue.clear();
	int numNotMarked = 0;
	bool done = false;
// Initialize to the tree root in case we dont find anything better
	QNode *rootp = root();
// Find a root near the center of the tree
	while(!done)
	{
		numNotMarked = 0;
		for(nodeIter = mNodeInfo.begin(); nodeIter != mNodeInfo.end(); nodeIter++)
		{
			QNodeInfo *nodeInfo = *nodeIter;
// If this is a leaf, mark the parent
			if(nodeInfo->leaf())
			{
				nodeInfo->node()->external()->setMarked();
				continue;
			}
			int markCount = 0;
			QNode *node = nodeInfo->node();
// Count the number of tries that are marked
			if(node->marked()) markCount++;
			if(node->internal1()->marked()) markCount++;
			if(node->internal2()->marked()) markCount++;
#define TWOMARKED 2
#define ALLMARKED 3
// If two of the tries are marked, mark the third
			if(markCount == TWOMARKED)
			{
				int maxHeight = 0;
				if(node->child1()->mHeight > maxHeight)
					maxHeight = node->child1()->mHeight;
				if(node->child2()->mHeight > maxHeight)
					maxHeight = node->child2()->mHeight;
				if(node->external()->mHeight > maxHeight)
					maxHeight = node->external()->mHeight;
				node->mHeight = maxHeight+1;

				node->setMarked();
				node->internal1()->setMarked();
				node->internal2()->setMarked();
			}
// If all of the tries are marked, mark the parents
			else if(markCount == ALLMARKED)
			{
				node->external()->setMarked();
				node->child1()->setMarked();
				node->child2()->setMarked();
			}
// Else, count this node as unmarked
			else
			{
				numNotMarked++;
// Keep track of one of the unmarked nodes
				rootp = node;
			}
		}
// If the number of unmarked nodes is less than 2, break out and use one of them as the root
		if(numNotMarked <= TWOMARKED)
		{
			break;
		}
	}
// At this point, rootp countains a node near the center of the tree

// Now do a level order traversal of the tree
	deque<QNode *> levelQueue;					  // Temporary queue
	mIterQueue.push_back(rootp->nodeInfo());
	levelQueue.push_back(rootp->external());
	levelQueue.push_back(rootp->child1());
	levelQueue.push_back(rootp->child2());
	while(!levelQueue.empty())
	{
// Get the node from the front
		QNode *node = levelQueue.front();
		levelQueue.pop_front();
// Dont put leaves on the list until the end
// Put his kids on the level queue
		if(!(node->nodeInfo()->leaf()))
		{
			mIterQueue.push_back(node->nodeInfo());
			levelQueue.push_back(node->child1());
			levelQueue.push_back(node->child2());
		}
	}
// Now put the leaves on the list
	for(nodeIter = mNodeInfo.begin(); nodeIter != mNodeInfo.end(); nodeIter++)
	{
		if((*nodeIter)->leaf())
			mIterQueue.push_back(*nodeIter);
	}
}


// Get the next value from the iterator
QNodeInfo *QTree::nextNodeInfo()
{
//    if(mNodeInfoIter == mNodeInfo.end())
//        return(NULL);
//    return(*mNodeInfoIter++);
	if(hasConstraints) //nodeinfo must come from constraint list
	{
		if(mNodeInfoConstraints.empty())
		{
			return(NULL);
		}
		QNodeInfo* rval=mNodeInfoConstraints.front();
		mNodeInfoConstraints.pop_front();
		mNodeInfoConstraints.push_back(rval);
		if(rval->visited())
			return(NULL); //we've gone through the whole list, but we want to keep track of it, so that joining can also be constrained
		return(rval);
	}
	else
	{
		if(AlgorithmFlags & ROOTFIRST)
		{
			if(mIterQueue.empty())
			{
				return(NULL);
			}
			QNodeInfo *rval = mIterQueue.front();
			mIterQueue.pop_front();
			return(rval);
		}
		else if(AlgorithmFlags & ROOTLAST)
		{
			QNodeInfo *rval = mIterQueue.back();
			mIterQueue.pop_back();
			return(rval);
		}
		else
		{
			mIterQueueIndex--;
			if(mIterQueueIndex < 0)
				mIterQueueIndex = mIterQueue.size()-1;
			if(mIterQueueIndexCount == mIterQueue.size())
				return(NULL);
			QNodeInfo *rptr = mIterQueue[mIterQueueIndex];
			mIterQueueIndexCount++;
			return(rptr);
		}
	}
}


// Structures for tree #1
void  QTree::initNodeInfoIter1()
{
	/*
	printf("NodeInfo1 size = %d\t",mNodeInfo1.size());
	printf("NodeInfo2 size = %d\t",mNodeInfo2.size());
	printf("mNodeInfoConstraints size = %d\n",mNodeInfoConstraints.size());
	*/
	
	mNodeInfoIter1 = mNodeInfo1.begin();
}												  // Point the iterator at the beginning of the list


// Get the next value from the iterator
QNodeInfo *QTree::nextNodeInfo1()
{
	if(mNodeInfoIter1 == mNodeInfo1.end())
		return(NULL);
	return(*mNodeInfoIter1++);
}


// Structures for tree #2
void  QTree::initNodeInfoIter2()
{
	
	//printf("NodeInfo2 size = %d\n",mNodeInfo2.size());
	mNodeInfoIter2 = mNodeInfo2.begin();
}												  // Point the iterator at the beginning of the list


// Get the next value from the iterator
QNodeInfo *QTree::nextNodeInfo2()
{
	if(mNodeInfoIter2 == mNodeInfo2.end())
		return(NULL);
	return(*mNodeInfoIter2++);
}


int  QTree::nchars() const
{
	return Interpreter::getInstance()->dataset()->nchars();
}


double& QTree::subTreeScore1() { return mSubTreeScore1; }
double QTree::subTreeScore1() const { return mSubTreeScore1; }
double& QTree::subTreeScore2() { return mSubTreeScore2; }
double QTree::subTreeScore2() const { return mSubTreeScore2; }
int& QTree::earlyPrune() { return mEarlyPrune; }
int QTree::earlyPrune() const { return mEarlyPrune; }

void QTree::removeNodeInfo(QNodeInfo *nodeInfoToRemove)
{
// Delete it out of the lists
	mNodeInfo.remove(nodeInfoToRemove);
	mNodeInfo1.remove(nodeInfoToRemove);
	mNodeInfo2.remove(nodeInfoToRemove);
// now delete the node
	delete nodeInfoToRemove->node();
// now delete the nodeinfo
	delete nodeInfoToRemove;
}


void QTree::randomMorph()
{
	QNodeInfo *splitNodeInfo = mNodeInfo.getRandom();

// We should really look to see if this has already been used

	QNode *splitPoint = splitNodeInfo->node();

// Mark this splitpoint as visited
	splitNodeInfo->visited() = true;

	splitUtil( splitPoint->external(), SUBTREE1);
	splitUtil( splitPoint, SUBTREE2);

// Now copy the nodes for each tree
	makeNodeLists();
	isSplit() = true;

// Now join them at random positions
	QNodeInfo *nodeInfo1 = mNodeInfo1.getRandom();
	mSubTree1 = nodeInfo1->node();
	QNodeInfo *nodeInfo2 = mNodeInfo2.getRandom();
	mSubTree2 = nodeInfo2->node();
	join();

	return;
}


// delete everything that isnt essential after you have searched a tree
void QTree::minimize()
{
// First we need to delete all of the nodedatas
	nodeDataInitialized() = false;

	QDeque <QNodeInfo *> *nodes = nodeInfoList();
	QDeque<QNodeInfo *>::iterator nodePointer;

	for(nodePointer = nodes->begin(); nodePointer != nodes->end(); nodePointer++)
	{
// If data for this node has already bee allocated
		if((*nodePointer)->nodeData())
		{
			delete((*nodePointer)->nodeData());
			(*nodePointer)->nodeData() = NULL;
		}
		if((*nodePointer)->nodeDataML())
		{
			delete((*nodePointer)->nodeDataML());
			(*nodePointer)->nodeDataML() = NULL;
		}
	}
}


/*Methods for calculateing Ancestral States using ACCTRANS method*/

void QTree::calculateAncestralStates()
{
	QNode *Child1;
	QNode *Child2;
	QNode *Child3;
	QNode *firstNode;
	QDeque<QNodeInfo *>::iterator nodePointer;

	int minNumber = INT_MAX;

	char temp1,temp2,temp3;

	firstNode = NULL;

// First we need to root the tree by pointing the root at the first taxon's parent
// Find the node with the min Number
	for(nodePointer = mNodeInfo.begin(); nodePointer != mNodeInfo.end(); nodePointer++)
	{
		if(*nodePointer && ((*nodePointer)->nodeIndex() >= 0) && ((*nodePointer)->nodeIndex() < minNumber))
		{
			firstNode = (*nodePointer)->node();
		}
	}

	if(firstNode==NULL) return;

// Everyone should have a external unless this is a single node tree
	if(firstNode->external())
	{
		root() = firstNode->external();
	}
	else
	{
// This is a single node tree, so return
		return;
	}

//Get the three subtrees from the root
	Child1 = root()->child1();
	Child2 = root()->child2();
	Child3 = root()->external();

// Perform the Postorder pass of the ACCTRANS method
	postorderAccTransPass(Child1);
	postorderAccTransPass(Child2);
	postorderAccTransPass(Child3);
//Set Taxon name
	string tax1name,tax2name,tax3name,taxthisname;
	tax1name = Child1->nodeInfo()->getTaxonName();
	tax2name = Child2->nodeInfo()->getTaxonName();
	tax3name = Child3->nodeInfo()->getTaxonName();
	taxthisname = "(" + tax3name + "," + tax1name + "," + tax2name + ")";
	root()->nodeInfo()->setTaxonName(taxthisname.c_str());
	for(int i=0;i<root()->nodeInfo()->nchars();i++)
	{
		temp1 = Child1->nodeInfo()->AncState(i);
		temp2 = Child2->nodeInfo()->AncState(i);
		temp3 = Child3->nodeInfo()->AncState(i);
		if(temp1&temp2&temp3)
			root()->nodeInfo()->AncState(i) = temp1&temp2&temp3;
		else
			root()->nodeInfo()->AncState(i) = temp1|temp2|temp3;
	}
// Perform first preorder pass of the ACCTRANS method
	assignAncestor();

/*
	 preorderAccTransPass(Child1);
	 preorderAccTransPass(Child2);
	 preorderAccTransPass(Child3);
	 -- This block Commented by Gordon Bean, May 6, 2008
	 It appears that assignAncestor() was meant to be called, instead of
	 preorderAccTransPass(), which replaces all the data with 'N'.
	 assignAncestor() and it's sub-routines were fully written, but not called
	 at any time. With this change, it all appears to work.
 */
}


void QTree::printLongestLeaf()
{
	QDeque<QNodeInfo *>::iterator nodePointer;

	printf("PrintLongestLeaf\n");
	for(nodePointer = mNodeInfo.begin(); nodePointer != mNodeInfo.end(); nodePointer++)
	{
		if((*nodePointer)->leaf())
		{
			QNode *parent = (*nodePointer)->node()->external();
			if(((*nodePointer)->nodeData() == NULL) || (parent->nodeInfo()->nodeData() == NULL))
			{
				return;
			}
			int differences = 0;
			char* mySiteData = (*nodePointer)->nodeData()->siteData(PSDI_POST) ;
			char* parentSiteData = parent->nodeInfo()->nodeData()->siteData(PSDI_POST) ;
			int* myweights = weights();
			for (int i = 0; i < nchars(); i++)
			{
				printf("i %d, me %x parent %x, weights %d\n",i,mySiteData[i], parentSiteData[i], myweights[i]);
				if (!(mySiteData[i] & parentSiteData[i]))
				{
					differences += myweights[i] ;
				}
			}
			printf("node %d, differences %d\n",(*nodePointer)->nodeIndex(),differences);
		}
	}
}


void QTree::assignAncestor()
{
	QNode *Child1;
	QNode *Child2;
	QNode *Child3;
	QNode *firstNode;
	QDeque<QNodeInfo *>::iterator nodePointer;

	int minNumber = INT_MAX;
	char parent;

	firstNode = NULL;

// First we need to root the tree by pointing the root at the first taxon's parent
// Find the node with the min Number
	for(nodePointer = mNodeInfo.begin(); nodePointer != mNodeInfo.end(); nodePointer++)
	{
		if(*nodePointer && ((*nodePointer)->nodeIndex() >= 0) && ((*nodePointer)->nodeIndex() < minNumber))
		{
			firstNode = (*nodePointer)->node();
		}
	}

	if(firstNode == NULL)
	{
//This tree is empty so return
		return;
	}

// Everyone should have a external unless this is a single node tree
	if(firstNode->external())
	{
		root() = firstNode->external();
	}
	else
	{
// This is a single node tree, so return
		return;
	}

//Get the three subtrees from the root
	Child1 = root()->child1();
	Child2 = root()->child2();
	Child3 = root()->external();

// Choose ancestral state
	for(int i=0;i<root()->nodeInfo()->nchars();i++)
	{
		parent = root()->nodeInfo()->AncState(i);
		if(parent&0x1<<0) root()->nodeInfo()->AncState(i) = 0x1<<0;
		else if(parent&0x1<<1) root()->nodeInfo()->AncState(i) = 0x1<<1;
		else if(parent&0x1<<2) root()->nodeInfo()->AncState(i) = 0x1<<2;
		else if(parent&0x1<<3) root()->nodeInfo()->AncState(i) = 0x1<<3;
	}

// Perform final preorder pass to assign ancestral states using ACCTRANS

	assignmentAccTransPass(Child1);
	assignmentAccTransPass(Child2);
	assignmentAccTransPass(Child3);
}


void QTree::postorderAccTransPass(QNode *& current)
{
	char temp1,temp2;
	const char* seq;
	if(current->nodeInfo()->leaf())
	{
		seq = current->nodeInfo()->getSeqData();
		for(int i=0;i<current->nodeInfo()->nchars();i++)
		{
			current->nodeInfo()->AncState(i) = seq[i];
//printf("%s, %i - ancState[i] for %s = %d\n",__FILE__,__LINE__,current->nodeInfo()->getTaxonName(),current->nodeInfo()->AncState(i)); // DEBUG
//At least one of the meaningfull bits is set
			assert(seq[i]&0x1f);
//None of the meaningless bits are set
			assert(!(seq[i]&0xe0));
		}
		return;
	}

	postorderAccTransPass(current->child1());
	postorderAccTransPass(current->child2());

	for(int i=0;i<current->nodeInfo()->nchars();i++)
	{
		temp1 = current->child1()->nodeInfo()->AncState(i);
		temp2 = current->child2()->nodeInfo()->AncState(i);
		if(temp1&temp2)
			current->nodeInfo()->AncState(i) = temp1&temp2;
		else
			current->nodeInfo()->AncState(i) = temp1|temp2;

//printf("%s, %i - temp1=%d, temp2=%d\n",__FILE__,__LINE__,temp1,temp2); // DEBUG
//printf("%s, %i - ancState[i] = %d\n",__FILE__,__LINE__,current->nodeInfo()->AncState(i)); // DEBUG
		assert(current->nodeInfo()->AncState(i)&0x1f);
		assert(!(current->nodeInfo()->AncState(i)&0xe0));
	}

//Set Taxon name
	string tax1name,tax2name,taxthisname;
	tax1name = current->child1()->nodeInfo()->getTaxonName();
	tax2name = current->child2()->nodeInfo()->getTaxonName();
	taxthisname = "(" + tax1name + "," + tax2name + ")";
	current->nodeInfo()->setTaxonName(taxthisname.c_str());

//printf("%s, %i - ancestor name: %s\n",__FILE__,__LINE__,taxthisname.c_str()); // DEBUG
//printf("%s, %i - ancestral state: %s\n",__FILE__,__LINE__,Interpreter::getInstance()->dataset()->convertBinaryToASCII(current->nodeInfo()->AncState()).c_str()); // DEBUG
}


void QTree::preorderAccTransPass(QNode *& current)
{
	char mask[4];
	char child;
	char new_child;
	char parent;

	for(int i=0;i<4;i++)
		mask[i] = 0x01 << i;

	for(int i=0;i<current->nodeInfo()->nchars();i++)
	{
		new_child = 0;
		child = current->nodeInfo()->AncState(i);
		parent = current->external()->nodeInfo()->AncState(i);
		assert(parent&0x1f);
		assert(child&0x1f);
		assert(!(parent&0xe0));
		assert(!(child&0xe0));
		for(int j=0;j<4;j++)
		{
			if(mask[j]&parent)
			{
				new_child|=mask[j];
			}
			else
			{
				new_child|=mask[j]|child;
			}
		}
		assert(new_child&0x1f);
		assert(!(new_child&0xe0));
		current->nodeInfo()->AncState(i) = new_child;
	}

	if(current->nodeInfo()->leaf())
	{
		return;
	}
	else
	{
		preorderAccTransPass(current->child1());
		preorderAccTransPass(current->child2());
	}
}


void QTree::assignmentAccTransPass(QNode *& current)
{
	char child;
	char parent;
	char new_child;
	if(current->nodeInfo()->leaf())
	{
		const char* seqdata = current->nodeInfo()->getSeqData();
		for (int i = 0; i < current->nodeInfo()->nchars(); i++)
		{
			current->nodeInfo()->AncState(i) = seqdata[i];
		}
		return;
	}

	for(int i=0;i<current->nodeInfo()->nchars();i++)
	{
		child = current->nodeInfo()->AncState(i);
		parent = current->external()->nodeInfo()->AncState(i);
		new_child=child&parent;
		if(new_child!=0)
		{
/*propagate parent down to the child*/
			current->nodeInfo()->AncState(i) = new_child;
		}
		else
		{
/*choose a state that can propagate to the nodes children*/
			parent=child;
			child = current->child1()->nodeInfo()->AncState(i);
			new_child=child&parent;
			if(new_child!=0)
			{
				current->nodeInfo()->AncState(i) = new_child;
			}
			else
			{
				child = current->child2()->nodeInfo()->AncState(i);
				new_child=child&parent;
				if(new_child!=0)
				{
					current->nodeInfo()->AncState(i) = new_child;
				}
				else
				{
					if(parent&0x1<<0) current->nodeInfo()->AncState(i) = 0x1<<0;
					else if(parent&0x1<<1) current->nodeInfo()->AncState(i) = 0x1<<1;
					else if(parent&0x1<<2) current->nodeInfo()->AncState(i) = 0x1<<2;
					else if(parent&0x1<<3) current->nodeInfo()->AncState(i) = 0x1<<3;
				}
			}
		}
	}

	assignmentAccTransPass(current->child1());
	assignmentAccTransPass(current->child2());
}


void QTree::displayAncestralStates()
{
	const char* name;
	if(root()==NULL)
	{
		PsodaPrinter::getInstance()->write("No Root\n");
		return;
	}

	showTree();

	displayAncStates(root()->external());
	displayAncStates(root()->child1());
	displayAncStates(root()->child2());

	root()->nodeInfo()->writeSequence();
	PsodaPrinter::getInstance()->write("\t");
	name = root()->nodeInfo()->getTaxonName();
	if(name)
	{
		PsodaPrinter::getInstance()->write("%s\n\n",name);
	}
	else
	{
		PsodaPrinter::getInstance()->write("\n\n");
	}

}


void QTree::displayAncStates(QNode *& current)
{
	PsodaPrinter* p;
	const char* name;
	p=PsodaPrinter::getInstance();

	if(current == NULL) return;
	if(current->nodeInfo()->leaf())
	{
		current->nodeInfo()->writeSequence();
		p->write("\t");
		p->write("%s",current->nodeInfo()->getTaxonName());
		p->write("\n");
		return;
	}
	displayAncStates(current->child1());

	current->nodeInfo()->writeSequence();
	p->write("\t");
	name = current->nodeInfo()->getTaxonName();
	if(name)
	{
		p->write("%s\n",name);
	}
	else
	{
		p->write("\n");
	}

	displayAncStates(current->child2());
}


/*Methods to display constraints*/
void QTree::showConstraints()
{

	for(QDeque<QNodeInfo*>::iterator iter = mNodeInfoConstraints.begin();iter != mNodeInfoConstraints.end();iter++)
	{
		PsodaPrinter::getInstance()->write("%d " ,(*iter)->nodeIndex());
	}
	PsodaPrinter::getInstance()->write("\n");
	mTreeLineLength = SubTreeDepth(mNodeInfoConstraints.back()->node(),false, false);

	int temp = SubTreeDepth(mNodeInfoConstraints.back()->node()->external(),false, false);
	if(temp > mTreeLineLength) mTreeLineLength = temp;

	char* buf = (char*)malloc(sizeof(char)*mTreeLineLength);

	for(int i=0;i<mTreeLineLength;i++)
		buf[i] = ' ';

	/*
	for(int i=0;i<mTreeLineLength;i++)
		printf("*");
	printf("\n");
	*/
	
	showSubTreeConstraints(buf,0,mNodeInfoConstraints.back()->node(),true,false);
	PsodaPrinter::getInstance()->write("| (%d)\n",mNodeInfoConstraints.back()->nodeIndex());
	showSubTreeConstraints(buf,0,mNodeInfoConstraints.back()->node()->external(),false,true);

	free(buf);

}

void QTree::showSubTreeConstraints(char* buf, int length, QNode *& current, bool top_child, bool bot_child)
{
	int i;
	int branch_len;

	
	branch_len = TREE_LEVEL_INDENT;
	

	if(current->nodeInfo()->leaf())
	{
		buf[length] = '\0';
		PsodaPrinter::getInstance()->write(buf);
		buf[length] = ' ';
		if(top_child)
			PsodaPrinter::getInstance()->write("/");
		if(bot_child)
			PsodaPrinter::getInstance()->write("\\");
		if(!(top_child || bot_child))
			PsodaPrinter::getInstance()->write("|");
		
		for(i=length+1;i<mTreeLineLength;i++)
			PsodaPrinter::getInstance()->write("-");
		
		if(!isConstraint(current->nodeInfo()))
		{
			PsodaPrinter::getInstance()->write("*** SECTOR EDGE (%s %d) ***",current->nodeInfo()->getTaxonName(),current->nodeInfo()->nodeIndex());
		}
		else
		{
			PsodaPrinter::getInstance()->write(current->nodeInfo()->getTaxonName());
		}
		PsodaPrinter::getInstance()->write("\n");
	}
	else if(!isConstraint(current->nodeInfo()))
	{
		buf[length] = '\0';
		PsodaPrinter::getInstance()->write(buf);
		buf[length] = ' ';
		if(top_child)
			PsodaPrinter::getInstance()->write("/");
		if(bot_child)
			PsodaPrinter::getInstance()->write("\\");
		if(!(top_child || bot_child))
			PsodaPrinter::getInstance()->write("|");
		
		for(i=length+1;i<mTreeLineLength;i++)
			PsodaPrinter::getInstance()->write("-");
		
		PsodaPrinter::getInstance()->write("*** SECTOR EDGE (%d)***",current->nodeInfo()->nodeIndex());
		PsodaPrinter::getInstance()->write("\n");
	}
	else
	{
		int children = 0;
		QNode* cur;
		for(cur = current->internal1(); cur != current;cur = cur->internal1())
		{
			children++;
		}

//Print half of the children
		if(!top_child)
			buf[length] = '|';

		cur = current->internal1();
		for(i = 0;i<children/2;i++)
		{
			if(i==0)
			{
				showSubTreeConstraints(buf,length+branch_len,cur->external() ,true,false);
			}
			else
				showSubTreeConstraints(buf,length+branch_len,cur->external() ,false,false);

			cur = cur->internal1();
		}
		buf[length] = ' ';

//Print the current node
		buf[length] = '\0';
		PsodaPrinter::getInstance()->write(buf);
		buf[length] = ' ';
		if(top_child)
			PsodaPrinter::getInstance()->write("/");
		if(bot_child)
			PsodaPrinter::getInstance()->write("\\");
		if(!(top_child || bot_child))
			PsodaPrinter::getInstance()->write("|");


		for(i=1;i<TREE_LEVEL_INDENT;i++)
			PsodaPrinter::getInstance()->write("-");
	
		PsodaPrinter::getInstance()->write("|");

		PsodaPrinter::getInstance()->write(" <-(%d)",current->nodeInfo()->nodeIndex());

		PsodaPrinter::getInstance()->write("\n");

//Print the other half of the children

		if(!bot_child)
			buf[length] = '|';

		i=children/2;
		while(cur != current)
		{
			i++;

			if(i==children)
			{
				showSubTreeConstraints(buf,length+branch_len,cur->external() ,false,true);
			}
			else
				showSubTreeConstraints(buf,length+branch_len,cur->external() ,false,false);

			cur = cur->internal1();
		}
		buf[length] = ' ';

	}

}

bool QTree::isConstraint(QNodeInfo* p)
{
	//printf("%d constraints\n",mNodeInfoConstraints.size());
	for(QDeque<QNodeInfo*>::iterator iter = mNodeInfoConstraints.begin();iter != mNodeInfoConstraints.end();iter++)
	{
		if(p->nodeIndex() == (*iter)->nodeIndex()) 
			return true;
		//else
		//	printf("%d != %d\n",p->nodeIndex(), (*iter)->nodeIndex());
	}
	//printf("Not found\n");
	return false;
}

/*Methods to display a tree*/

void QTree::showTree(bool labels,bool branch_lengths)
{

	mTreeLineLength = SubTreeDepth(root(),labels, branch_lengths);

	int temp = SubTreeDepth(root()->external(),labels, branch_lengths);
	if(temp > mTreeLineLength) mTreeLineLength = temp;

	char* buf = (char*)malloc(sizeof(char)*mTreeLineLength);

	for(int i=0;i<mTreeLineLength;i++)
		buf[i] = ' ';

	/*
	for(int i=0;i<mTreeLineLength;i++)
		printf("*");
	printf("\n");
	*/
	
	showSubTree(buf,0,root(),true,false,labels, branch_lengths);
	showSubTree(buf,0,root()->external(),false,true,labels, branch_lengths);

	free(buf);

}

int QTree::SubTreeDepth( QNode *& current,bool labels,bool branch_lengths)
{
	if(!current)
		return 0;
	if(current->nodeInfo()->leaf())
	{
		if(branch_lengths)
		{
			int branch_len;
			char buf[100];
			sprintf(buf,branch_length_fmt,current->branchLength());
			if(current->branchLength()>=0.0)
				branch_len = strlen(buf)+1;
			else
				branch_len =TREE_LEVEL_INDENT;
			return branch_len;
		}
		return TREE_LEVEL_INDENT;
	}
	else
	{
		int max = 0;
		int cur_len = 0;
		QNode* cur;
		for(cur = current->internal1(); cur != current;cur = cur->internal1())
		{
			cur_len = SubTreeDepth(cur->external(),labels, branch_lengths);
			if(cur_len > max) max = cur_len;
		}
		
		if(labels)
		{
		int label_len;
		if(current->nodeInfo()->getLabel())
			label_len = strlen(current->nodeInfo()->getLabel());
		else
			label_len = 0;
		if(label_len + 5 > max) max = label_len + 5;
		}
		
		
		if(branch_lengths)
		{
			int branch_len;
			char buf[100];
			sprintf(buf,branch_length_fmt,current->branchLength());
			if(current->branchLength()>=0.0)
				branch_len = strlen(buf)+1;
			else
				branch_len =TREE_LEVEL_INDENT;
		
			return max+branch_len;
		
		}
		
		return max+TREE_LEVEL_INDENT;

	}

}


void QTree::showSubTree(char* buf, int length, QNode *& current, bool top_child, bool bot_child, bool labels,bool branch_lengths)
{
	int i;
	int branch_len;

	if(branch_lengths&&(current->branchLength()>=0.0))
	{
		char buf[100];
		sprintf(buf,branch_length_fmt,current->branchLength());
		branch_len = strlen(buf) + 1;
	}
	else
	{
		branch_len = TREE_LEVEL_INDENT;
	}

	if(current->nodeInfo()->leaf())
	{
		buf[length] = '\0';
		PsodaPrinter::getInstance()->write(buf);
		buf[length] = ' ';
		if(top_child)
			PsodaPrinter::getInstance()->write("/");
		if(bot_child)
			PsodaPrinter::getInstance()->write("\\");
		if(!(top_child || bot_child))
			PsodaPrinter::getInstance()->write("|");
		if(branch_lengths&&(current->branchLength()>=0.0))
		{
			for(i=length;i<mTreeLineLength-branch_len;i++)
				PsodaPrinter::getInstance()->write("-");
			PsodaPrinter::getInstance()->write(branch_length_fmt,current->branchLength());

		}
		else
		{
		for(i=length+1;i<mTreeLineLength;i++)
			PsodaPrinter::getInstance()->write("-");
		}
		PsodaPrinter::getInstance()->write(current->nodeInfo()->getTaxonName());
		PsodaPrinter::getInstance()->write("\n");
	}
	else
	{
		int children = 0;
		QNode* cur;
		for(cur = current->internal1(); cur != current;cur = cur->internal1())
		{
			children++;
		}

//Print half of the children
		if(!top_child)
			buf[length] = '|';

		cur = current->internal1();
		for(i = 0;i<children/2;i++)
		{
			if(i==0)
			{
				showSubTree(buf,length+branch_len,cur->external() ,true,false,labels, branch_lengths);
			}
			else
				showSubTree(buf,length+branch_len,cur->external() ,false,false,labels, branch_lengths);

			cur = cur->internal1();
		}
		buf[length] = ' ';

//Print the current node
		buf[length] = '\0';
		PsodaPrinter::getInstance()->write(buf);
		buf[length] = ' ';
		if(top_child)
			PsodaPrinter::getInstance()->write("/");
		if(bot_child)
			PsodaPrinter::getInstance()->write("\\");
		if(!(top_child || bot_child))
			PsodaPrinter::getInstance()->write("|");

		if(branch_lengths&&(current->branchLength()>=0.0))
		{
			PsodaPrinter::getInstance()->write(branch_length_fmt,current->branchLength());
		}
		else
		{
			for(i=1;i<TREE_LEVEL_INDENT;i++)
				PsodaPrinter::getInstance()->write("-");
		}
		PsodaPrinter::getInstance()->write("|");
		if(labels)
		{
		if(current->nodeInfo()->getLabel())
			PsodaPrinter::getInstance()->write(" <-(%d) %s",current->nodeInfo()->nodeIndex(),current->nodeInfo()->getLabel());
		}
		PsodaPrinter::getInstance()->write("\n");

//Print the other half of the children

		if(!bot_child)
			buf[length] = '|';

		i=children/2;
		while(cur != current)
		{
			i++;

			if(i==children)
			{
				showSubTree(buf,length+branch_len,cur->external() ,false,true,labels, branch_lengths);
			}
			else
				showSubTree(buf,length+branch_len,cur->external() ,false,false,labels, branch_lengths);

			cur = cur->internal1();
		}
		buf[length] = ' ';

	}

}


QNode* QTree::findLeaf(string& taxonName)
{
	QDeque<QNodeInfo *>::iterator iter;
	for (iter = nodeInfoList()->begin(); iter != nodeInfoList()->end(); iter++)
	{
		if ((*iter) == NULL) { std::cerr << __FILE__ << " " << __LINE__ << " (*iter) is NULL" << endl;}
//printf("%s %i Node name: %s\n", __FILE__ , __LINE__ ,(*iter)->getTaxonName());
//printf("%s %i Node seq: %s\n", __FILE__, __LINE__, Interpreter::getInstance()->dataset()->convertBinaryToASCII((*iter)->getSeqData()).c_str());
		if ((*iter)->leaf() && taxonName == (*iter)->getTaxonName())
		{
			return (*iter)->node();
		}
	}
	return NULL;
}

void QTree::clearConstraints()
{
	hasConstraints = false;
	mNodeInfoConstraints.clear();
}

void QTree::addConstraint(QNodeInfo* p)
{
	if(p==NULL) return;
	
	hasConstraints = true;
	mNodeInfoConstraints.push_back(p);
	//printf("mNodeInfoConstraints size = %d\n",mNodeInfoConstraints.size());
}

void QTree::addConstraint(int index)
{
	QNodeInfo* p = findIndex(index);
	if(p!= NULL)
		addConstraint(p);
}

QNodeInfo* QTree::findIndex(int index)
{
	QNodeInfo* p = findIndex_recurse(root(),index);
	if(p!=NULL) return p;
	return findIndex_recurse(root()->external(),index);
}

QNodeInfo* QTree::findIndex_recurse(QNode* root,int index)
{
	if(root->nodeInfo()->nodeIndex() == index)
		return root->nodeInfo();
	if(root->nodeInfo()->leaf())
		return NULL;
	QNodeInfo* p = findIndex_recurse(root->child1(),index);
	if(p!=NULL) return p;
	return findIndex_recurse(root->child2(),index);
		
}