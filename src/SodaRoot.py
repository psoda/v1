#!/usr/bin/python

import os
import sys
import string
import types
import code

topsrcdir = os.path.dirname ( os.path.abspath ( __file__ ) )

def Split ( arg ):
  if  type(arg) is types.StringType or isinstance(arg, UserString):
    return string.split ( arg )
  else:
    return [ arg ]

def spacejoin ( list ):
  return string.join ( list , ' ' )

def topDirPrefix ( path ):
  if not len ( path ):
    return topsrcdir + '/'
  if path[-1] == '/' or path[-1] == '\\': 
    return topsrcdir + path
  else:
    return topsrcdir + '/' + path + '/'

def makeInclude( path ):
  return topDirPrefix( path )

def makeAbsolute( path ):
  return topDirPrefix( path )

###############################Include Paths

#ExternalSources
#LINUX

#Libs
KPR_ABSOLUTE = makeAbsolute( "kpr" )

soda = {}

CORE_ABSOLUTE = makeAbsolute( "." )
soda['CORE_LIB_NAME'] = "libsoda"
soda['CORE_LIB_DEPENDENCIES'] = ""

UTILITY_ABSOLUTE = makeAbsolute( "utility" )
soda['UTILITY_LIB_NAME'] = "libsodautility"
soda['UTILITY_LIB_DEPENDENCIES'] = ""

BASE_ABSOLUTE = makeAbsolute( "bases" )
soda['BASE_LIB_NAME'] = "libsodabase"
soda['BASE_LIB_DEPENDENCIES'] = ""

ALGORITHM_ABSOLUTE = makeAbsolute( "algorithms" )
soda['ALGORITHM_LIB_NAME'] = "libsodaalgorithm"
soda['ALGORITHM_LIB_DEPENDENCIES'] = ""

EVALUATOR_ABSOLUTE = makeAbsolute( "evaluators" )
soda['EVALUATOR_LIB_NAME'] = "libsodaevaluator"
soda['EVALUATOR_LIB_DEPENDENCIES'] = ""

DIRECTALIGNMENT_ABSOLUTE = makeAbsolute( "evaluators/directAlignment" )
soda['DIRECTALIGNMENT_LIB_NAME'] = "libdirectAlignment"
soda['DIRECTALIGNMENT_LIB_DEPENDENCIES'] = ""

SEARCH_ABSOLUTE = makeAbsolute( "searches" )
soda['SEARCH_LIB_NAME'] = "libsodasearch"
soda['SEARCH_LIB_DEPENDENCIES'] = ""

PAUPPARSER_ABSOLUTE = makeAbsolute( "parsers/paupParser" )
soda['PAUPPARSER_LIB_NAME'] = "libsodapaupparser"
soda['PAUPPARSER_LIB_DEPENDENCIES'] = ""

NEWICKPARSER_ABSOLUTE = makeAbsolute( "parsers/newickParser" )
soda['NEWICKPARSER_LIB_NAME'] = "libsodanewickparser"
soda['NEWICKPARSER_LIB_DEPENDENCIES'] = ""

FRONTENDS_ABSOLUTE = makeAbsolute( "frontends" )
soda['FRONTENDS_LIB_NAME'] = "libsodafrontends"
soda['FRONTENDS_LIB_DEPENDENCIES'] = ""

soda['EXECUTABLE_NAME'] = "soda"
soda['EXECUTABLE_LIB_DEPENDENCIES'] = [ soda['CORE_LIB_NAME'], soda['UTILITY_LIB_NAME'], soda['BASE_LIB_NAME'], soda['PAUPPARSER_LIB_NAME'], soda['NEWICKPARSER_LIB_NAME'], soda['ALGORITHM_LIB_NAME'], soda['EVALUATOR_LIB_NAME'], 'pthread', soda['SEARCH_LIB_NAME'], soda['DIRECTALIGNMENT_LIB_NAME'], soda['FRONTENDS_LIB_NAME'], ]

ALL = [ CORE_ABSOLUTE, UTILITY_ABSOLUTE, KPR_ABSOLUTE, BASE_ABSOLUTE, PAUPPARSER_ABSOLUTE, NEWICKPARSER_ABSOLUTE, ALGORITHM_ABSOLUTE, EVALUATOR_ABSOLUTE, SEARCH_ABSOLUTE, DIRECTALIGNMENT_ABSOLUTE, FRONTENDS_ABSOLUTE ]
soda['GLOBAL_INCLUDES'] = ALL
soda['LIBRARY_SEARCH_PATH'] = ALL 
soda['WRAPPER'] = "Wrapper"
