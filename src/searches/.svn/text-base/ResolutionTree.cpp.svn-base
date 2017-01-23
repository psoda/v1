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
 *  ResolutionTree.cpp
 *  
 *
 *  Created by Kenneth Sundberg on 1/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ResolutionTree.h"
#include <string.h>
#include "Interpreter.h"
#include "HSearchInstr.h"
#include "Parsimony.h"

#include "NewickTreeParser.h"

ResolutionTree::ResolutionTree(QTree* tree, bool TBR):mResolved(true),mDataset(NULL),mResolution(NULL),mNchars(0),mSequenceData(NULL),children(),taxon(-1)
{
	ResolutionTree* left;
	ResolutionTree* right;
	
	
	memset(TreeStr,0,10000);
	
	printf("Begining Postorder Pass\n");
	//postorder pass
	left = BuildNode(tree->root()->external(),0);
	printf("Between left and right\n");
	right = BuildNode(tree->root()->external()->external(),0);
	
	
	printf("Begining Preorder Pass\n");
	//preorder pass
	left->Resolve(right->mSequenceData,TBR);
	right->Resolve(left->mSequenceData,TBR);
	
	//compute TreeStr
	sprintf(TreeStr,"(%s,%s)",left->partialTreeStr(),right->partialTreeStr());
	
}

ResolutionTree::~ResolutionTree()
{
	memset(TreeStr,0,10000);
}

ResolutionTree::ResolutionTree():mResolved(true),mDataset(NULL),mResolution(NULL),mNchars(0),mSequenceData(NULL),children(),taxon(-1)
{}

QTree* ResolutionTree::getQTree()
{
	//printf("Tree String %s\n",TreeStr);
	
    NewickTreeParser* ntp = new NewickTreeParser();
    QTree * qt = ntp->parseBuffer(TreeStr, strlen(TreeStr), false);
	return qt;
}

char* ResolutionTree::partialTreeStr()
{
	int pos = 0;
	if(children.size()>0)
	{	
		if(children.size()>2)
		{
			//Find node in resolution tree that represents the external branch
			QNode* externalNode=findNode(children.size(),mResolution->root());
			sprintf(TreeStr,"%s",partialTreeStringFromNode(externalNode->external()));
		}
		else
		{
			pos += sprintf(&(TreeStr[pos]),"(");
			for(list<ResolutionTree*>::iterator iter=children.begin();iter != children.end();iter++)
				pos += sprintf(&(TreeStr[pos]),"%s,",(*iter)->partialTreeStr());
			TreeStr[pos-1] = ')';
			TreeStr[pos] = '\0';
		}
	}
	else
	{
		pos += sprintf(&(TreeStr[pos]),"%d",taxon);
		TreeStr[pos] = '\0';
	}
	
	return TreeStr;
}

QNode* ResolutionTree::findNode(int id, QNode* root)
{
	if(root->nodeInfo()->nodeIndex()==id)
		return root;
	QNode* result;
	result = findNode(id,root->child1());
	if(result != NULL) return result;
	
	
	result = findNode(id,root->child2());
	if(result != NULL) return result;
	
	//Do this one last as it represents backtracking
	result = findNode(id,root->external());
	if(result != NULL) return result;
	
	printf("ERROR: ResolutionTree::findNode() did not return\n");
	return NULL;
}

char* ResolutionTree::partialTreeStringFromNode(QNode* root)
{
	char buf[1000];
	if(root->nodeInfo()->leaf())
	{
		//Get treestring from appropriate child
		list<ResolutionTree*>::iterator iter=children.begin();
		for(int i=0;i<root->nodeInfo()->nodeIndex();i++)
			iter++;
		return strdup((*iter)->partialTreeStr());
	}
	else
	{
		char* left = partialTreeStringFromNode(root->child1());
		char* right = partialTreeStringFromNode(root->child2()); 
		sprintf(buf,"(%s,%s)",left,right);
		free(left);
		free(right);
		return strdup(buf);
	}
}

ResolutionTree* ResolutionTree::BuildNode(QNode* root,int level)
{
	int i;
	ResolutionTree* Node;
	Node = new ResolutionTree();
	//If root is a leaf
	if(root->nodeInfo()->leaf())
	{
		Node->mResolved=true;
		Node->mLeaf=true;
		Node->mNchars = root->nodeInfo()->nchars();
		Node->mSequenceData = (char*)malloc(sizeof(char)*Node->mNchars);
		Node->taxon = root->nodeInfo()->nodeIndex()+1;
		memcpy(Node->mSequenceData,root->nodeInfo()->getSeqData(),Node->mNchars);
		
		return Node;
	}

	Node->mLeaf = false;


	//Create child for every child of root
	for(QNode* cur=root->internal1();cur!=root;cur=cur->internal1())
		Node->children.push_back(BuildNode(cur->external(),level+1));
	
	Node->mNchars = Node->children.front()->mNchars;	
	Node->mSequenceData = (char*)malloc(sizeof(char)*Node->mNchars);

	/*
	for(i=0;i<level;i++)
			printf(" ");
	printf("Internal node with %d children\n",(int)Node->children.size());
	*/
					
	//Is this node resolved?
	if(Node->children.size() > 2)
	{
		//unresolved node
		Node->mResolved = false;
		//create dataset
		
		/*for(i=0;i<level;i++)
			printf(" ");
		printf("Adding Taxa\n");*/
		
		Node->mDataset = new Dataset();
		Node->mDataset->datatype() = Dataset::DNA_DATATYPE;
		Node->mDataset->dataformat() = Dataset::ALIGNED_DATAFORMAT;
		Node->mDataset->gapchar() = '-';
		Node->mDataset->missingchar() = '-';
		
		
		i=0;
		for(list<ResolutionTree*>::iterator iter=Node->children.begin();iter != Node->children.end();iter++)
		{
			char taxaname[20];
			sprintf(taxaname,"%d",i);
			i++;
			//printf("Adding Taxa %s\n",taxaname);
			Node->mDataset->addName(taxaname);
		}
		
		/*
		for(i=0;i<level;i++)
			printf(" ");
		printf("Adding Characters\n");
		*/
		
		i=0;
		for(list<ResolutionTree*>::iterator iter=Node->children.begin();iter != Node->children.end();iter++)
		{
			
			char* non_binary = (char*)malloc(sizeof(char)*Node->mNchars+1);
			memcpy(non_binary,(*iter)->mSequenceData,Node->mNchars);
			
			
			for(int j=0;j<Node->mNchars;j++)
			{
				if((non_binary[j] < 0)||(non_binary[j] >= 16))
				{
						if(non_binary[j] == 0x1f)
							non_binary[j] = '-';
						else
							printf("Bad non-binary character %d\n",non_binary[j]);
				}
				else
				{
					non_binary[j] = Node->mDataset->convertToASCII((char)non_binary[j]);
				}
			}
			non_binary[Node->mNchars]='\0';
			//printf("Adding Characters for Taxa %d\t%s\n",i,non_binary);
			Node->mDataset->addCharacters(non_binary,i);
			free(non_binary);
			i++;
		}
		
		
			
		if(Node->children.size() == 3)  //Dont run HSearch as only 1 tree is possible
		{
			memcpy(Node->mSequenceData,Node->children.front()->mSequenceData,Node->mNchars);
			
			for(list<ResolutionTree*>::iterator iter=++(Node->children.begin());iter != Node->children.end();iter++)
			{
				char* other_seq = (*iter)->mSequenceData;
				for(i=0;i<Node->mNchars;i++)
				{
					if(Node->mSequenceData[i] & other_seq[i])
					{
						//printf("&");
						Node->mSequenceData[i] &= other_seq[i];
					}
					else
					{
						//printf("|");
						Node->mSequenceData[i] |= other_seq[i];
					}	
				}
			}
		}
		else
		{
			/*for(i=0;i<level;i++)
				printf(" ");
			printf("Running Hsearch\n");*/
			
			//run Hsearch
			Dataset* curDataset = Interpreter::getInstance()->dataset();
			
			Interpreter::getInstance()->installDataset(Node->mDataset);
			HSearchInstr hs;
			QTreeRepository repo;
			
			//setup environment for HSearch
		
			Environment baseEnv;
			//baseEnv.setString("swap","tbr");
			baseEnv.setString("swap","none");
			baseEnv.setString("start","stepwise");
			
			hs.doHeuristicSearch(&baseEnv,&repo);
			Interpreter::getInstance()->installDataset(curDataset);
			
			//get resolution tree for this node
			
			Node->mResolution = repo.popTree();
			
			//Preprocess this tree with parsimony
			Parsimony p;
			p.qpreProcess(Node->mResolution);
			
			//The sequence data for this node is the final sequence data on this tree
			memcpy(Node->mSequenceData,Node->mResolution->root()->nodeInfo()->nodeData()->siteData(PSDI_FINAL) ,Node->mNchars);
		}
		/*for(i=0;i<level;i++)
			printf(" ");
		printf("Unresolved Sequence=\t");
		for(i=0;i<Node->mNchars;i++)
			printf("%x",Node->mSequenceData[i]);
		printf("\n");*/
	}
	else
	{
		//resolved node
		Node->mResolved = true;
		
		memcpy(Node->mSequenceData,Node->children.front()->mSequenceData,Node->mNchars);
	
		char* other_seq = (Node->children.back())->mSequenceData;

		/*
		printf("\tLeft Sequence=\t");
		for(i=0;i<Node->mNchars;i++)
			printf("%x",Node->mSequenceData[i]);
		printf("\n");
		printf("\tRight Sequence=\t");
		for(i=0;i<Node->mNchars;i++)
			printf("%x",other_seq[i]);
		printf("\n\t\t\t");
		*/
		for(i=0;i<Node->mNchars;i++)
		{
			if(Node->mSequenceData[i] & other_seq[i])
			{
				//printf("&");
				Node->mSequenceData[i] &= other_seq[i];
			}
			else
			{
				//printf("|");
				Node->mSequenceData[i] |= other_seq[i];
			}	
		}
		//printf("\n");
	
		/*for(i=0;i<level;i++)
			printf(" ");
		printf("Resolved Sequence=\t");
		for(i=0;i<Node->mNchars;i++)
			printf("%x",Node->mSequenceData[i]);
		printf("\n");*/
	
	}
	return Node;
}

void ResolutionTree::Resolve(const char* pre_seq,bool TBR)
{
	int i;
	if(mLeaf)
		return; //leaf node
	if(mResolved)
	{
		//printf("Prepass for resolved node with %d children\n",children.size());
		char* other_seq = (char*)malloc(sizeof(char)*mNchars);
		
		memcpy(other_seq,pre_seq,mNchars);
		for(i=0;i<mNchars;i++)
		{
			if(other_seq[i] & children.back()->mSequenceData[i])
				other_seq[i] &= children.back()->mSequenceData[i];
			else
				other_seq[i] |= children.back()->mSequenceData[i];
		}
		children.front()->Resolve(other_seq,TBR);
		
		memcpy(other_seq,pre_seq,mNchars);
		for(i=0;i<mNchars;i++)
		{
			if(other_seq[i] & children.front()->mSequenceData[i])
				other_seq[i] &= children.front()->mSequenceData[i];
			else
				other_seq[i] |= children.front()->mSequenceData[i];
		}
		children.back()->Resolve(other_seq,TBR);
		
		
		
		free(other_seq);
	}
	else
	{
		//printf("Prepass for unresolved node with %d children\n",children.size());
		//Add external taxa
		
		i = children.size();
		
		char taxaname[20];
		sprintf(taxaname,"%d",i);
		mDataset->addName(taxaname);
		
		char* non_binary = (char*)malloc(sizeof(char)*mNchars+1);
		memcpy(non_binary,pre_seq,mNchars);
			
			
			for(int j=0;j<mNchars;j++)
			{
				non_binary[j] = mDataset->convertToASCII((char)non_binary[j]);
			}
			non_binary[mNchars]='\0';
		
		
		//printf("Adding Characters for external Taxa %d\t%s\n",i,non_binary);
		
		mDataset->addCharacters(non_binary,i);
		
		//run Hsearch
		Dataset* curDataset = Interpreter::getInstance()->dataset();
		Interpreter::getInstance()->installDataset(mDataset);
		HSearchInstr hs;
		QTreeRepository repo;
		Environment baseEnv;
		
		//printf("Running Hsearch\n");
		
		
		if(TBR)
		{
			baseEnv.setString("swap","tbr");
		}
		else
		{
			baseEnv.setString("swap","none");
		}
		baseEnv.setString("start","stepwise");
		//baseEnv.setString("nreps","10");
		
		hs.doHeuristicSearch(&baseEnv,&repo);
		Interpreter::getInstance()->installDataset(curDataset);
		
		//get resolution tree for this node
		
		//printf("Getting final resolution tree\n");
		
		mResolution = repo.popTree();
		
		//Preprocess this tree with parsimony
		Parsimony p;
		p.qpreProcess(mResolution);
		
		//Pass along the predata for each node
		char* other_seq = (char*)malloc(sizeof(char)*mNchars);
		for(list<ResolutionTree*>::iterator iter=children.begin();iter != children.end();iter++)
		{
			if((*iter)!= NULL)
			{
				memcpy(other_seq,mResolution->root()->nodeInfo()->nodeData()->siteData(PSDI_FINAL),mNchars);
				for(i=0;i<mNchars;i++)
				{
					if(other_seq[i] & (*iter)->mSequenceData[i])
						other_seq[i] &= (*iter)->mSequenceData[i];
					else
						other_seq[i] |= (*iter)->mSequenceData[i];
				}
				(*iter)->Resolve(other_seq,TBR);
			}
			else
			{
				printf("NULL child in non-leaf node\n");
			}
		}
	}
}