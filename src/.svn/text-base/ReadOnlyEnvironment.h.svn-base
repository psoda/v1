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
#ifndef PSODA_READ_ONLY_ENVIRONMENT
#define PSODA_READ_ONLY_ENVIRONMENT

#include "Literal.h"
#include "Environment.h"
#include <string>

using namespace std;

class ReadOnlyEnvironment : public Environment {

  public:
	/**
	* Constructor
	*/
	ReadOnlyEnvironment();

	/**
	* Destructor  
	*/
	virtual ~ReadOnlyEnvironment();

	/**
	* Throws PsodaException
	*/
	virtual void set(string ident, const Literal* lit, bool dereferenceLitIfReference) __attribute__((noreturn));

	/**
	* @throws UndefinedVariableException
	*/
	virtual void update(string ident, const Literal* lit) __attribute__((noreturn));

	/**
	* Returns false
	*/
	virtual bool canUpdate(string ident) const;

};

#endif
