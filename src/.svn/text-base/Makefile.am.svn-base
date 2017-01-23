include Commands.am
if LINUX
path1_to_linux_java = /usr/local/java/jdk*
path2_to_linux_java = /usr/java/jdk*
GUIINCLUDES = -I$(path1_to_linux_java)/include -I/$(path1_to_linux_java)/include/linux -I$(path2_to_linux_java)/include -I/$(path2_to_linux_java)/include/linux
endif
if MAC
GUIINCLUDES = -I/System/Library/Frameworks/JavaVM.framework/Headers 
endif
if WINDOWS
GUIINCLUDES = -I"c:\Program Files\Java\jdk1.5.0_12\include" -I"c:\Program Files\Java\jdk1.5.0_12\include\win32"
endif

INCLUDES = $(GUIINCLUDES) -I${top_srcdir}/src -I${top_srcdir}/src/distances  -I${top_srcdir}/src/visualizations -I${top_srcdir}/src/gui -I${top_srcdir}/src/algorithms  -I${top_srcdir}/src/alignment -I${top_srcdir}/src/bases  -I${top_srcdir}/src/evaluators  -I${top_srcdir}/src/parsers/paupParser  -I${top_srcdir}/src/parsers/newickParser  -I${top_srcdir}/src/searches  -I${top_srcdir}/src/alignment -I${top_srcdir}/src/utility -I${top_srcdir}/src/psodascript  -I${top_srcdir}/src/blocks  -I${top_srcdir}/src/expressions  -I${top_srcdir}/src/readline -I${top_srcdir}/src/fann-2.0.0/src/include

CFLAGS = -DFIXEDFANN
LDADD = ${top_srcdir}/src/fann-2.0.0/src/.libs/libfloatfann.a

basedir_files = \
 main.cpp \
 bayes.cpp \
 Buffer.cpp \
 CarbonCopyBuffer.cpp \
 Codeable.cpp \
 ConsolePrintBuffer.cpp \
 DataUnit.cpp \
 Dataset.cpp \
 DatasetMap.cpp \
 Environment.cpp \
 FilePrintBuffer.cpp \
 FileTools.cpp \
 Globals.cpp \
 GuiPrintBuffer.cpp \
 Interpreter.cpp \
 InvalidCastException.cpp \
 InvalidParameterListException.cpp \
 LocatableObject.cpp \
 Location.cpp \
 ParsingInfo.cpp \
 PsodaBreak.cpp \
 PsodaError.cpp \
 PsodaException.cpp \
 PsodaStopException.cpp \
 PsodaPrinter.cpp \
 PsodaReturn.cpp \
 PsodaRunner.cpp \
 PsodaWarning.cpp \
 QDeque.cpp \
 QNode.cpp \
 QNodeInfo.cpp \
 QTree.cpp \
 QTreeRepository.cpp \
 ReadOnlyEnvironment.cpp \
 CommandRegister.cpp \
 SSDataset.cpp \
 SignalHandlers.cpp \
 TreeMap.cpp \
 TreeReader.cpp \
 UndefinedVariableException.cpp \
 VariableRedeclarationException.cpp \
 raxml.cpp \
 Validatable.cpp

algorithmfiles = \
  QTBR.cpp \
  RTBR.cpp \
  Sectarian.cpp

alignmentfiles = \
  MultChar.cpp \
  QAlign.cpp \
  QSSAlign.cpp \
  QSSPredict.cpp \
  QSumOfPairs.cpp \
  SubMat.cpp

basesfiles = \
  EvaluatorBase.cpp \
  PrintBuffer.cpp \
  QAlgorithmBase.cpp \
  QAlignBase.cpp \
  QSearchBase.cpp \
  SectorSearchBase.cpp

blocksfiles = \
  Block.cpp \
  DataBlock.cpp \
  ProgramBlock.cpp \
  PsodaBlocks.cpp \
  TreeBlock.cpp

evaluatorsfiles = \
  Likelihood.cpp \
  LikelihoodNodeData.cpp \
  Parsimony.cpp \
  ParsimonyNodeData.cpp \
  FastParsimony.cpp \
  FastParsimonyNodeData.cpp

expressionsfiles = \
  AdditionOp.cpp \
  BoolCastOp.cpp \
  BoolLiteral.cpp \
  CallExpression.cpp \
  CatOp.cpp \
  Data.cpp \
  DatasetReference.cpp \
  IdentReference.cpp \
  DivisionOp.cpp \
  EqualityOp.cpp \
  Expression.cpp \
  GreaterThanOp.cpp \
  GreaterThanOrEqualOp.cpp \
  IdentExpression.cpp \
  InequalityOp.cpp \
  IntCastOp.cpp \
  IntLiteral.cpp \
  LessThanOp.cpp \
  LessThanOrEqualOp.cpp \
  Literal.cpp \
  LiteralExpression.cpp \
  LogicalAndOp.cpp \
  LogicalNotOp.cpp \
  LogicalOrOp.cpp \
  ModOp.cpp \
  MultiplicationOp.cpp \
  NameExpression.cpp \
  NegationOp.cpp \
  OperationExpression.cpp \
  Operator.cpp \
  ParenthesesOp.cpp \
  PostDecrOp.cpp \
  PostIncrOp.cpp \
  PreDecrOp.cpp \
  PreIncrOp.cpp \
  RealNumberCastOp.cpp \
  RealNumberLiteral.cpp \
  StringCastOp.cpp \
  StringLiteral.cpp \
  SubtractionOp.cpp \
  TreeLiteral.cpp \
  UndefinedLiteral.cpp

parsersfiles = \
  parsers/newickParser/NewickLexicalAnalyzer.cpp \
  parsers/newickParser/NewickParser.cpp \
  parsers/newickParser/NewickTreeParser.cpp \
  parsers/paupParser/PaupLexicalAnalyzer.cpp \
  parsers/paupParser/PaupParser.cpp \
  parsers/paupParser/PsodaSourceParser.cpp

psodascriptfiles = \
  BuiltInCommand.cpp \
  AlignmentCommand.cpp \
  CommandNode.cpp \
  Conditional.cpp \
  IfElse.cpp \
  IfThenPair.cpp \
  ProgramGraphNode.cpp \
  PsodaCommand.cpp \
  PsodaConstruct.cpp \
  PsodaElement.cpp \
  PsodaProgram.cpp \
  UserDefinedCommand.cpp \
  InlineUserDefinedCommand.cpp \
  WeightsData.cpp \
  WhileLoop.cpp

searchesfiles = \
  QRetainedResultSearch.cpp \
  AlternatingSearch.cpp \
  QStep.cpp \
  Dist.cpp \
  NJ.cpp \
  DPTBR.cpp \
  ResolutionTree.cpp \
  TreeResolve.cpp\
  TreeField.cpp\
  GradientDescent.cpp \
  ParticleTree.cpp \
  CommonCladeRefinement.cpp \
  TreeFusion.cpp \
  SectorStep.cpp \
  PartitionSector.cpp \
  PartitionSectorSearchBase.cpp \
  OrthoSectorSearch.cpp \
  PartitionSpaceSectorSearch.cpp \
  RandomSectorSearch.cpp \
  NoneSectorSearch.cpp \
  PartialTree.cpp \
  PartialTreeSearch.cpp

utilityfiles = \
  Timer.cpp

guifiles = \
  PSODA.cpp \
  GuiCommandNode.cpp

distancesfiles = \
  RF.cpp \
  Partition.cpp \
  PartitionTree.cpp

visualizationfiles = \
  MDS.cpp \
  ProjectedTree.cpp \
  VisualizationBase.cpp \
  VisualizationTerminal.cpp \
  gnuPlotTerminal.cpp \
  AnalysisTerminal.cpp \
  Hypersphere.cpp

bin_PROGRAMS = psoda

VPATH= .:distances::visualizations:gui:algorithms:alignment:bases:blocks:evaluators:expressions:parsers/newickParser:parsers/paupParser:psodascript:RAxML-7.0.4:searches:utility

psoda_SOURCES := \
  $(algorithmfiles) \
  $(alignmentfiles) \
  $(basedir_files) \
  $(basesfiles) \
  $(blocksfiles) \
  $(evaluatorsfiles) \
  $(expressionsfiles) \
  $(guifiles) \
  $(parsersfiles) \
  $(psodascriptfiles) \
  $(psodacommands) \
  $(searchesfiles) \
  $(utilityfiles) \
  $(visualizationfiles) \
  $(distancesfiles)

NewickParser.hpp parsers/newickParser/NewickParser.cpp: NewickParser.yy
	bison -d -o parsers/newickParser/NewickParser.cpp parsers/newickParser/NewickParser.yy

parsers/newickParser/NewickLexicalAnalyzer.cpp: NewickLexicalAnalyzer.ll NewickParser.hpp
	flex -t parsers/newickParser/NewickLexicalAnalyzer.ll >  parsers/newickParser/NewickLexicalAnalyzer.cpp

parsers/paupParser/PaupLexicalAnalyzer.cpp: PaupLexicalAnalyzer.ll PaupParser.hpp
	flex -t parsers/paupParser/PaupLexicalAnalyzer.ll > parsers/paupParser/PaupLexicalAnalyzer.cpp

PaupParser.hpp parsers/paupParser/PaupParser.cpp: PaupParser.yy
	bison -d -o parsers/paupParser/PaupParser.cpp parsers/paupParser/PaupParser.yy

NewickParser.o: NewickParser.hpp parsers/newickParser/NewickParser.cpp
	$(CXXCOMPILE) -Wno-error -MT NewickParser.o -MD -MP -MF .deps/NewickParser.Tpo -c -o NewickParser.o `test -f 'parsers/newickParser/NewickParser.cpp' || echo './'`parsers/newickParser/NewickParser.cpp

NewickLexicalAnalyzer.o: parsers/newickParser/NewickLexicalAnalyzer.cpp
	$(CXXCOMPILE) -Wno-error -MT NewickLexicalAnalyzer.o -MD -MP -MF .deps/NewickLexicalAnalyzer.Tpo -c -o NewickLexicalAnalyzer.o `test -f 'parsers/newickParser/NewickLexicalAnalyzer.cpp' || echo './'`parsers/newickParser/NewickLexicalAnalyzer.cpp

PaupParser.o: parsers/paupParser/PaupParser.cpp
	$(CXXCOMPILE) -Wno-error -MT PaupParser.o -MD -MP -MF .deps/PaupParser.Tpo -c -o PaupParser.o `test -f 'parsers/paupParser/PaupParser.cpp' || echo './'`parsers/paupParser/PaupParser.cpp

PaupLexicalAnalyzer.o: parsers/paupParser/PaupLexicalAnalyzer.cpp
	$(CXXCOMPILE) -Wno-error -MT PaupLexicalAnalyzer.o -MD -MP -MF .deps/PaupLexicalAnalyzer.Tpo -c -o PaupLexicalAnalyzer.o `test -f 'parsers/paupParser/PaupLexicalAnalyzer.cpp' || echo './'`parsers/paupParser/PaupLexicalAnalyzer.cpp


gui: 
	$(MAKE) -C gui
