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
#include "IdentReference.h"
#include "InvalidCastException.h"
#include "Interpreter.h"
#include <sstream>

using namespace std;

IdentReference::IdentReference(string _name, Environment* _evalEnv) : Literal(), name(_name), evalEnv(_evalEnv) {
  return;
}

Literal& IdentReference::operator= (const Literal&)
{
return *this;
}

IdentReference::~IdentReference() {
  return;
}
  
string IdentReference::toCode() const {
  return name;
}

bool IdentReference::toBool() const {
  return evalEnv->lookup(name).toBool();
}

double IdentReference::toRealNumber() const {
  return evalEnv->lookup(name).toRealNumber();
}

int IdentReference::toInt() const {
  return evalEnv->lookup(name).toInt();
}

string IdentReference::toString() const {
  return evalEnv->lookup(name).toString();
}

QTree* IdentReference::toTree() const {
  return evalEnv->lookup(name).toTree();
}

void* IdentReference::getData() const {
  return evalEnv->lookup(name).getData();
}

IdentReference* IdentReference::copyToHeap() const {
  return new IdentReference(name, evalEnv);
}

string IdentReference::getVar() const {
  return name;
}

Environment* IdentReference::getEnv() const {
  return evalEnv;
}

const type_info& IdentReference::getType() {
  static IdentReference identReference("", NULL);
  static const type_info& info = typeid(identReference);
  return info;
}
