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
#ifndef QSODA_TREE 
#define QSODA_TREE
#include <list>
#include <deque>
#include <sstream>
#include <float.h>
#include "QNode.h"
#include "QNodeInfo.h"
#include "QDeque.h"

#define MAINTREE 0
#define SUBTREE1 1
#define SUBTREE2 2
#define SUBTREENONE 3

#define TREE_LEVEL_INDENT 5

using namespace std;

class QNodeInfo;
class QNode;
class QTree
{
    public:
        // These four are needed for elements of a list
        QTree();
        QTree(const QTree* m);
		QTree(const QTree& m);
        ~QTree();
        const QTree& operator= (const QTree& right);

	/**
	 * Deallocates the memory for this tree
	 */
        void clean();

        // These four are needed for elements of a list

        void recurseCopy( QNode*& dest, QNode* src);
        QNode*& root();
        QNode*  root() const;
        QNode*& start();
        QNode*  start() const;
        void canonicalize();
        //double& score() { return mScore; }
        //double  score() const { return mScore; }

		double getScore() const;
		void setScore(double newScore);	
		void resetScore();	
		bool isScored();	

        int& nnodes();
        int  nnodes() const;
        bool& visited(); /* Have all split points been visited */
        bool visited() const; /* Have all split points been visited */
	
        bool& isSplit(); // This tree is already split
        bool isSplit()  const; // This tree is already split
        bool split(); // Find the next split point and split yourself
        QNodeInfo *findSplitPoint();// Get the next non-visited node
        void splitUtil(QNode *splitNode, int whichSubTree); // Split into 2 subtrees
        void makeNodeLists();  // Create NodeLists for the two subtrees
        void makeNodeList( QNode* root, int whichList ); // Make a node list
        void recurseMakeNodeList(QNode* root, int whichList); // Recursive call
        void join(); // Join the subtrees again
        QNode *joinUtil(QNode *subTree, int whichTree); // join using saved nodeinfo
        QNode *getJoinNode(int whichTree); // Get a saved join node, or make a new one
        void saveJoinNode(QNode *saveNode, int whichTree); // Save a join node for later use

        // Create a new tree from the old one using the two join points
        QTree *createSpecificTree(QNodeInfo *node1, QNodeInfo *node2, bool saveOrigTree);
	
        // Structures for total tree
        void  insertNodeInfo(QNodeInfo *nodeInfo); // Insert NodeInfo 
        void  insertNodeInfo1(QNodeInfo *nodeInfo); // Insert NodeInfo 
        void  insertNodeInfo2(QNodeInfo *nodeInfo); // Insert NodeInfo 
        void  initNodeInfoIter(); // Point the iterator at the beginning of the list
        QNodeInfo *nextNodeInfo(); // Get the next value from the iterator, NULL if end

        // Structures for tree #1
        void  initNodeInfoIter1(); // Point the iterator at the beginning of the list
        QNodeInfo *nextNodeInfo1(); // Get the next value from the iterator, NULL if end

        // Structures for tree #2
        void  initNodeInfoIter2(); // Point the iterator at the beginning of the list
        QNodeInfo *nextNodeInfo2(); // Get the next value from the iterator, NULL if end

        // Copy the nodeinfo entries corresponding to the tree starting at root into separate lists
        void makeNodeList1(QNode *root);
        void makeNodeList2(QNode *root);

        QNode *& subTree1();
        QNode * subTree1() const;
        QNode *& subTree2();
        QNode * subTree2() const;
        QNode  *&splitNode1();
        QNode  *splitNode1() const;
        QNode  *&splitNode2();
        QNode  *splitNode2() const;
	
        bool & nodeDataInitialized();
        bool  nodeDataInitialized() const;
        bool & nodeDataDAInitialized();
        bool  nodeDataDAInitialized() const;
        bool & nodeDataMLInitialized();
        bool  nodeDataMLInitialized() const;
        // This is pretty ugly, but parsimony needs to get to the nodinfo list
        QDeque <QNodeInfo *> *nodeInfoList();
	
        void print(); // Print the main tree
        void updateTreeString(int whichtree, bool names, bool rooted); // Update with one of the subtrees
        void updateTreeString(int whichtree, bool names); // Update with one of the subtrees
        void updateTreeString(); // Update from the main tree.
        char *treeStr();
        void setTreeStr(char *newStr);
        void inorderRecurse(QNode *& current,char *buf, bool names);
        int postorderRecurse(QNode *& current);
        int  nchars() const;
        int* weights() const;
        double  &subTreeScore1();
        double  subTreeScore1() const;
        double  &subTreeScore2();
        double  subTreeScore2() const;
        int  &earlyPrune();
        int  earlyPrune() const;
        void removeNodeInfo(QNodeInfo *nodeInfoToRemove); // Remove this nodeinfo and all links to it
        void  randomMorph(); // Randomly split and randomly join a tree to find another in the TBR space
        void  minimize(); // Delete everything that can be spared when we have finished searching a tree.

		void calculateAncestralStates();  /*Calculate Ancestral States using ACCTRANS method*/
		void displayAncestralStates();    /*Display caluclated ancestral states*/
		void assignAncestor(); /*Assign specific ancestral states*/
		void printLongestLeaf(); /* print leaf with the longest branch */

		void showTree(bool labels=true,bool branch_lengths=true); /*Display tree in ascii art*/
		void showConstraints(); /*Display constraints as ascii art tree*/
		
		QNode* findLeaf(string& taxonName);	/* Returns a pointer to the node with the specified name */

		void clearConstraints();
		void addConstraint(QNodeInfo*);
		void addConstraint(int index);

    protected:
        int mEarlyPrune;  /* The best score we have found so far, so we can prune */
        int mNNodes;  /*number of nodes in the tree*/
        bool mNodeDataInitialized; //set if the nodedata has been set in all of the nodeinfos
        bool mNodeDataDAInitialized; //set if the nodedata has been set in all of the nodeinfos
        bool mNodeDataMLInitialized; //set if the nodedata has been set in all of the nodeinfos
        QNode *mRoot;  /*the root node if rooted, otherwise an arbitrary node in the tree*/
        QNode *mStart;  /*the root node if rooted, otherwise an arbitrary node in the tree*/
        bool mSplit; /*This tree is already split */
        bool mAllVisited; /*indicates that all split points have been visited */
        double mScore;    /*tree score parsimony or likelihood*/
		bool scored;	 /*a boolean indicating if the tree has been scored*/
        ostringstream mTreeString;    /* String to temporarily hold Newick format */
        char *mTreeStr;    /* String to permanently hold Newick format */
        //STL list of NodeInfo Classes
        QDeque<QNodeInfo *> mNodeInfo;  // STL list of NodeInfo Objects for all taxa
        QDeque <QNodeInfo *>::iterator mNodeInfoIter; // Iterator to use in acccessing list
        QDeque <QNodeInfo *> mNodeInfo1;  // STL list of NodeInfo Classes for first Tree 
        QDeque <QNodeInfo *>::iterator mNodeInfoIter1; // Iterator to use in acccessing list
        QDeque <QNodeInfo *> mNodeInfo2;  // STL list of NodeInfo Classes for second Tree 
        QDeque <QNodeInfo *>::iterator mNodeInfoIter2; // Iterator to use in acccessing list
        QNode *mSubTree1; // When you split a tree this is one of the subtrees
        double mSubTreeScore1; // Score for subtree #1
        QNode *mSubTree2; // When you split a tree this is one of the subtrees
        double mSubTreeScore2; // Score for subtree #2
        QNode *mSplitNode1; // Split node pointer for recycled join nodes
        QNode *mSplitNode2; // Split node pointer for recycled join nodes
		//Constraint lists for splitting and joining
		bool hasConstraints;
		deque<QNodeInfo *> mNodeInfoConstraints;  // STL list of NodeInfo Objects for all taxa
		
        // Evaluator needs to get to the tree data
        friend class QEvaluator;
        int mIterQueueIndex; // Keep track of where we are in the deque
        unsigned int mIterQueueIndexCount; // Keep track of how many we have returned
        deque<QNodeInfo *> mIterQueue; // Traverse split points in tree order
        bool mIterQueueInitialized; // Flag if we have initialized
		
		// Functions for reconstructing ACCTRANS Ancestral States
		void postorderAccTransPass(QNode *& current);
		void preorderAccTransPass(QNode *& current);
		void assignmentAccTransPass(QNode *& current);

		// Function for displaying Ancestral States
		void displayAncStates(QNode *& current);
		void showSubTree(char*,int,QNode *& current,bool,bool,bool,bool);
		void showSubTreeConstraints(char* buf, int length, QNode *& current, bool top_child, bool bot_child);
		int SubTreeDepth(QNode *& current,bool,bool);
		bool isConstraint(QNodeInfo*);
		int mTreeLineLength;
		QNodeInfo* findIndex(int);
		QNodeInfo* findIndex_recurse(QNode*,int);
};
#endif

