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
 *  QDeque.h
 *  QDeque
 *
 *  Created by Quinn Snell on 5/29/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __QDEQUE_H__
#define __QDEQUE_H__

#include <iostream>
#include <time.h>
#include <deque>
#include "Globals.h"
using namespace std;

#define RANDOM_MAX 2147483647			// 2**31 - 1
#ifdef WIN32
#define srandom srand
#define random rand
#endif

template <class ItemType>
class QDeque
{
public:
	QDeque();
	virtual ~QDeque();
	
	typedef typename deque<ItemType>::iterator iterator;
	
	void push_front(const ItemType& item);
	void push_back(const ItemType& item);
	
	iterator begin() {return data.begin();};
	iterator end() {return data.end();};
	
	ItemType& operator[](int index);
	
	void randomSeed(int seed = 0);
	void randomize();
	ItemType getRandom();
	void remove(ItemType item);
	void clear(void);
	int size(void);
	
private:
	deque<ItemType> data;
	int qrand(int max);
};

template <class ItemType>
QDeque<ItemType>::QDeque()
: data()
{
	
}

template <class ItemType>
QDeque<ItemType>::~QDeque()
{
  return;
}

template <class ItemType>
ItemType& QDeque<ItemType>::operator[](int index)
{
	return data[index];
}

template <class ItemType>
void QDeque<ItemType>::push_front(const ItemType& item)
{
	data.push_front(item);
}

template <class ItemType>
void QDeque<ItemType>::push_back(const ItemType& item)
{
	data.push_back(item);
}

template <class ItemType>
int QDeque<ItemType>::qrand(int max)
{
	//random() returns a number between 0 and (2**31)-1 which is defined as RANDOM_MAX
	double percent = random()/(double)RANDOM_MAX;
	return (int)(percent * max);
}

template <class ItemType>
void QDeque<ItemType>::randomSeed(int seed)
{
	if (seed)
		srandom(seed);
	else
		srandom(time(NULL));
}

template <class ItemType>
void QDeque<ItemType>::randomize()
{
	// Randomize by simply picking random pairs of locations and swapping them
    if(AlgorithmFlags & NORANDOMSTEPWISE)
        return;
	int size = data.size();
	for (int i = 0; i < size/2; i++)
	{
		int loc1 = qrand(size);
		int loc2 = qrand(size);
		ItemType tmp = data[loc1];
		data[loc1] = data[loc2];
		data[loc2] = tmp;
		//cout << "Swapped " << loc1 << " with " << loc2 << endl;
	}
}

template <class ItemType>
ItemType QDeque<ItemType>::getRandom()
{
	int size = data.size();
	return data[qrand(size)];
}

template <class ItemType>
void QDeque<ItemType>::remove(ItemType item)
{
	int i;
	int size = data.size();
	for (i = 0; i < size; i++)
	{
		if (data[i] == item)
			break;
	}
	if (i < size)
	{
		//swap the item with the last in the list
		ItemType tmp = data[i];
		data[i] = data[size-1];
		data[size-1] = tmp;
		
		//now pop the last on off the list
		data.pop_back();
	}
}

template <class ItemType>
void QDeque<ItemType>::clear(void)
{
	data.clear();
}

template <class ItemType>
int QDeque<ItemType>::size(void)
{
	return data.size();
}

#endif
