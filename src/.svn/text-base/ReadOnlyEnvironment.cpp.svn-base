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
#include "ReadOnlyEnvironment.h"
#include "UndefinedVariableException.h"
#include "StringLiteral.h"
#include <string>

using namespace std;

ReadOnlyEnvironment::ReadOnlyEnvironment() : Environment() {
  return;
}

ReadOnlyEnvironment::~ReadOnlyEnvironment() {
  return;
}

void ReadOnlyEnvironment::set(string ident __attribute__ ((unused)) , const Literal* lit __attribute__ ((unused)), bool dereferenceLitIfReference __attribute__((unused))) {
  throw PsodaException("This is a read only environment (no one should see this error)\n");
}

void ReadOnlyEnvironment::update(string ident, const Literal* lit __attribute__ ((unused))) {
  string message = "Cannot update the variable \"" + ident + "\"";
  throw UndefinedVariableException(message);
}

bool ReadOnlyEnvironment::canUpdate(string ident __attribute__ ((unused))) const {
  return false;
}
