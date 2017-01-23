%module Soda

%include kpr_swig.i
%{

#include "ArrayBase.h"
#include "Node.h"
#include "Tree.h"
#include "MatrixBase.h"
#include "Matrix.h"
#include "Dataset.h"
#include "Interpreter.h"
#include "Sequences.h"
#include "Weights.h"
#include "SearchBase.h"
#include "RetainedResultSearch.h"
#include "StepwiseSpeedAdditionSearch.h"
#include "TreePrinter.h"
#include "NewickTreeParser.h"
%}

%include ArrayBase.h
%template(SequencePtrArray) Array<Sequence*>;
%include MatrixBase.h
%template(CharMatrix) MatrixBase<char>;
%template(IntMatrix) MatrixBase<int>;

%include Node.h
%include Tree.h
%include Matrix.h
%include Dataset.h
%include Interpreter.h
%include Sequences.h
%include Weights.h
%include RetainedResultSearch.h
%include StepwiseSpeedAdditionSearch.h
%include TreePrinter.h
%include NewickTreeParser.h
