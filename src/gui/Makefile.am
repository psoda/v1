path1_to_linux_java = /usr/local/java/jdk*
path2_to_linux_java = /usr/java/jdk*

AM_CPPFLAGS= -D GUI

bin_PROGRAMS = buildgui

guiarchivedir = .psodaguiar
guijar = ../psoda.jar

JAVACLASSPATH = "../lib/EDU.jar$(AM_CLASSPATHSEP)../lib/byu.jar$(AM_CLASSPATHSEP)../lib/jxl.jar$(AM_CLASSPATHSEP)../lib/swing-layout-1.0.3.jar$(AM_CLASSPATHSEP)Jalview/dist/jalview.jar$(AM_CLASSPATHSEP)../$(AM_CLASSPATHSEP)./"

LINUXGUIINCLUDES = -I$(path1_to_linux_java)/include -I/$(path1_to_linux_java)/include/linux -I$(path2_to_linux_java)/include -I/$(path2_to_linux_java)/include/linux
MACGUIINCLUDES = -I/System/Library/Frameworks/JavaVM.framework/Headers
#WINDOWSGUIINCLUDES = -I/cygdrive/c/Program\ Files/Java/jdk1.5.0_12/include -I/cygdrive/c/Program\ Files/Java/jdk1.5.0_12/include/win32
WINDOWSGUIINCLUDES = -I/cygdrive/c/Program\ Files/Java/jdk1.6.0_07/include -I/cygdrive/c/Program\ Files/Java/jdk1.6.0_07/include/win32

if LINUX
  AM_CLASSPATHSEP= :
  AM_EXE= psoda
  LINUXSHAREDLIBFILE = libPSODA.so
  LINUXSHAREDLIBBUILD = g++ -fPIC -shared -o $(SHAREDLIBFILE) $(guiarchivedir)/*.o $(guiarchivedir)/*.a $(libraries) $(LINUXGUIINCLUDES) -lncurses 
else
  LINUXSHAREDLIBFILE =
  LINUXSHAREDLIBBUILD =
endif

if MAC
  AM_CLASSPATHSEP= :
  AM_EXE= psoda
  MACSHAREDLIBFILE = libPSODA.jnilib
  MACSHAREDLIBBUILD = g++ -dynamiclib -o $(MACSHAREDLIBFILE) $(MACGUIINCLUDES) $(guiarchivedir)/*.o $(guiarchivedir)/*.a $(libraries) -lncurses -framework JavaVM
else
  MACSHAREDLIBFILE =
  MACSHAREDLIBBUILD =
endif

if WINDOWS
  AM_CLASSPATHSEP= ;
  AM_EXE= psoda.exe
  WINSHAREDLIBFILE = PSODA.dll
  WINSHAREDLIBBUILD = g++ -mno-cygwin -shared -Wl,--add-stdcall-alias -o $(WINSHAREDLIBFILE) $(guiarchivedir)/*.o $(guiarchivedir)/*.a $(libraries) $(WINDOWSGUIINCLUDES)
else
  WINSHAREDLIBFILE =
  WINSHAREDLIBBUILD =
endif

SHAREDLIBFILE = $(WINSHAREDLIBFILE)$(MACSHAREDLIBFILE)$(LINUXSHAREDLIBFILE)
SHAREDLIBBUILD = $(WINSHAREDLIBBUILD)$(MACSHAREDLIBBUILD)$(LINUXSHAREDLIBBUILD)

psodaGuiFiles = \
  ExecuteCommandFrame.java \
  ExecutePsodaCodeThread.java \
  GuiCommandNode.java \
  MainMenu.java \
  MainTabs.java \
  MyDocumentListener.java \
  MyUndoableEditListener.java \
  PSODA.java \
  PsodaButtons.java \
  PsodaPanel.java \
  Registration.java \
  UndoRedo.java \
  UndoableContainer.java

treesaapJavaFiles = \
 ../treesaap/BASEML/BASEML.java \
 ../treesaap/BASEML/BASEMLStub.java \
 ../treesaap/BASEML/BASEMLTreeBean.java \
 ../treesaap/BASEML/BASEMLUsageBean.java \
 ../treesaap/BASEML/ExecBaseml.java \
 ../treesaap/Block/Block.java \
 ../treesaap/Block/BlockStub.java \
 ../treesaap/Block/BlockUsageBean.java \
 ../treesaap/CDM/CDM.java \
 ../treesaap/CDM/CDMPrint.java \
 ../treesaap/CDM/CDMResultBean.java \
 ../treesaap/CDM/CDMStub.java \
 ../treesaap/CDM/CDMUsageBean.java \
 ../treesaap/CommandLine/CommandLine.java \
 ../treesaap/CommandLine/CommandLineStub.java \
 ../treesaap/CommandLine/CommandLineUsageBean.java \
 ../treesaap/Data/CriticalValueBean.java \
 ../treesaap/Data/CriticalValueControl.java \
 ../treesaap/Data/Data.java \
 ../treesaap/Data/DataStub.java \
 ../treesaap/Data/DataUsageBean.java \
 ../treesaap/Data/GeneticCodeBean.java \
 ../treesaap/Data/GeneticCodeControl.java \
 ../treesaap/Data/ProteinBean.java \
 ../treesaap/Data/ProteinControl.java \
 ../treesaap/Data/TableBean.java \
 ../treesaap/Driver/Driver.java \
 ../treesaap/Driver/DriverForPsoda.java \
 ../treesaap/Driver/DriverPrint.java \
 ../treesaap/Driver/DriverPrintStub.java \
 ../treesaap/Driver/DriverPrintUsageBean.java \
 ../treesaap/Driver/DriverStub.java \
 ../treesaap/Driver/DriverUsageBean.java \
 ../treesaap/Driver/TestPsodaPanelJFrame.java \
 ../treesaap/Evpthwy/Evpthwy.java \
 ../treesaap/Evpthwy/EvpthwyBean.java \
 ../treesaap/Evpthwy/EvpthwyControl.java \
 ../treesaap/Evpthwy/EvpthwyPrint.java \
 ../treesaap/Evpthwy/EvpthwyResultBean.java \
 ../treesaap/Evpthwy/EvpthwyStub.java \
 ../treesaap/Evpthwy/EvpthwyUsageBean.java \
 ../treesaap/GeneralDNAFileParser/DNAFileParserPrint.java \
 ../treesaap/GeneralDNAFileParser/DNAFileParserProgressWindow.java \
 ../treesaap/GeneralDNAFileParser/DNAFileParserStub.java \
 ../treesaap/GeneralDNAFileParser/DNAFileParserUsageBean.java \
 ../treesaap/GeneralDNAFileParser/GeneralDNAFileParser.java \
 ../treesaap/GeneralDNAFileParser/TaxaBean.java \
 ../treesaap/GeneralDNAFileParser/TaxaControl.java \
 ../treesaap/GeneralDNAFileParser/TreeBean.java \
 ../treesaap/GeneralDNAFileParser/TreeControl.java \
 ../treesaap/GraphicInterface/ActionControl.java \
 ../treesaap/GraphicInterface/GraphicInterface.java \
 ../treesaap/GraphicInterface/GraphicInterfaceStub.java \
 ../treesaap/GraphicInterface/GraphicInterfaceUsageBean.java \
 ../treesaap/GraphicInterface/ObjectControl.java \
 ../treesaap/GraphicInterface/Objects/ActionComponent.java \
 ../treesaap/GraphicInterface/Objects/ActionMenuItem.java \
 ../treesaap/GraphicInterface/Objects/ActionPanel.java \
 ../treesaap/GraphicInterface/Objects/ToolTipButton.java \
 ../treesaap/GraphicInterface/Panels/AnalysisPanel.java \
 ../treesaap/GraphicInterface/Panels/BasemlPanel.java \
 ../treesaap/GraphicInterface/Panels/bean.java \
 ../treesaap/GraphicInterface/Panels/BranchesPanel.java \
 ../treesaap/GraphicInterface/Panels/CDMPanel.java \
 ../treesaap/GraphicInterface/Panels/CorrelatePanel.java \
 ../treesaap/GraphicInterface/Panels/EvpthwyPanel.java \
 ../treesaap/GraphicInterface/Panels/FilePanel.java \
 ../treesaap/GraphicInterface/Panels/MainPanel5.java \
 ../treesaap/GraphicInterface/Panels/OperationPanel.java \
 ../treesaap/GraphicInterface/Panels/ParserPanel.java \
 ../treesaap/GraphicInterface/Panels/PropertyPanel.java \
 ../treesaap/GraphicInterface/Panels/PropListCreatePanel.java \
 ../treesaap/GraphicInterface/Panels/PropListDeletePanel.java \
 ../treesaap/GraphicInterface/Panels/PropListDisplayPanel.java \
 ../treesaap/GraphicInterface/Panels/PropListSelectPanel.java \
 ../treesaap/GraphicInterface/Panels/RunPanel.java \
 ../treesaap/GraphicInterface/Panels/RunSettingsPanel.java \
 ../treesaap/GraphicInterface/Panels/RunTreePanel.java \
 ../treesaap/GraphicInterface/Panels/TaxaChoosePanel.java \
 ../treesaap/GraphicInterface/Panels/TaxaDisplayPanel.java \
 ../treesaap/GraphicInterface/Panels/TaxaPanel.java \
 ../treesaap/GraphicInterface/Panels/TreeChoosePanel.java \
 ../treesaap/GraphicInterface/Panels/TreeDisplayPanel.java \
 ../treesaap/GraphicInterface/Panels/TreePanel.java \
 ../treesaap/Objects/Stub.java \
 ../treesaap/Objects/UsageBean.java \
 ../treesaap/Run/Run.java \
 ../treesaap/Run/RunStub.java \
 ../treesaap/Run/RunUsageBean.java \
 ../treesaap/Run/SubstsCDMProgressWindow.java \
 ../treesaap/Run/SubstsEvpthwyProgressWindow.java \
 ../treesaap/Substs/MultiPath.java \
 ../treesaap/Substs/Substs.java \
 ../treesaap/Substs/SubstsBean.java \
 ../treesaap/Substs/SubstsControl.java \
 ../treesaap/Substs/SubstsGeneticCodeBean.java \
 ../treesaap/Substs/SubstsPrint.java \
 ../treesaap/Substs/SubstsResultBean.java \
 ../treesaap/Substs/SubstsStub.java \
 ../treesaap/Substs/SubstsUsageBean.java \
 ../treesaap/Substs/TableBean.java \
 ../treesaap/Tree/Tree.java \
 ../treesaap/Tree/TreePrint.java \
 ../treesaap/Tree/TreeStub.java \
 ../treesaap/Tree/TreeUsageBean.java \
 ../treesaap/Weka/ChemicalPropertiesException.java \
 ../treesaap/Weka/DiGraph.java \
 ../treesaap/Weka/Edge.java \
 ../treesaap/Weka/InvalidWekaFileException.java \
 ../treesaap/Weka/Node.java \
 ../treesaap/Weka/TestDriver.java \
 ../treesaap/Weka/WekaFileReader.java \
 ../treesaap/Weka/wekasim.java

dataconvertJavaFiles = \
       ../DataConvert/CLUSTAL/CLUSTAL.java \
       ../DataConvert/CLUSTAL/CLUSTALPanel.java \
       ../DataConvert/CLUSTAL/CLUSTALReader.java \
       ../DataConvert/CLUSTAL/CLUSTALStub.java \
       ../DataConvert/CLUSTAL/CLUSTALUsageBean.java \
       ../DataConvert/CLUSTAL/CLUSTALWriter.java \
       ../DataConvert/Components/Controller.java \
       ../DataConvert/Components/Reader.java \
       ../DataConvert/Components/Stub.java \
       ../DataConvert/Components/UsageBean.java \
       ../DataConvert/Components/Writer.java \
       ../DataConvert/DataConvert/Driver.java \
       ../DataConvert/DataConvert/DriverUsageBean.java \
       ../DataConvert/DataConvert/Read.java \
       ../DataConvert/DataConvert/Write.java \
       ../DataConvert/FASTA/FASTA.java \
       ../DataConvert/FASTA/FASTAPanel.java \
       ../DataConvert/FASTA/FASTAReader.java \
       ../DataConvert/FASTA/FASTAStub.java \
       ../DataConvert/FASTA/FASTAUsageBean.java \
       ../DataConvert/FASTA/FASTAWriter.java \
       ../DataConvert/GENBANKFLAT/GENBANKFLAT.java \
       ../DataConvert/GENBANKFLAT/GENBANKFLATReader.java \
       ../DataConvert/GENBANKFLAT/GENBANKFLATStub.java \
       ../DataConvert/GENBANKFLAT/GENBANKFLATUsageBean.java \
       ../DataConvert/GENBANKXML/GENBANKXML.java \
       ../DataConvert/GENBANKXML/GENBANKXMLReader.java \
       ../DataConvert/GENBANKXML/GENBANKXMLStub.java \
       ../DataConvert/GENBANKXML/GENBANKXMLUsageBean.java \
       ../DataConvert/GraphicInterface/ActionControl.java \
       ../DataConvert/GraphicInterface/GraphicInterface.java \
       ../DataConvert/GraphicInterface/GraphicInterfaceStub.java \
       ../DataConvert/GraphicInterface/GraphicInterfaceUsageBean.java \
       ../DataConvert/GraphicInterface/ObjectControl.java \
       ../DataConvert/GraphicInterface/Viewer.java \
       ../DataConvert/MEGA/MEGA.java \
       ../DataConvert/MEGA/MEGAPanel.java \
       ../DataConvert/MEGA/MEGAReader.java \
       ../DataConvert/MEGA/MEGAStub.java \
       ../DataConvert/MEGA/MEGAUsageBean.java \
       ../DataConvert/MEGA/MEGAWriter.java \
       ../DataConvert/NEXUS/NEXUS.java \
       ../DataConvert/NEXUS/NEXUSBlockReader.java \
       ../DataConvert/NEXUS/NEXUSPanel.java \
       ../DataConvert/NEXUS/NEXUSReader.java \
       ../DataConvert/NEXUS/NEXUSStub.java \
       ../DataConvert/NEXUS/NEXUSUsageBean.java \
       ../DataConvert/NEXUS/NEXUSWriter.java \
       ../DataConvert/Objects/GenbankBean.java \
       ../DataConvert/Objects/TaxaBean.java \
       ../DataConvert/Objects/TaxaBeanManager.java \
       ../DataConvert/Objects/TreeBean.java \
       ../DataConvert/Objects/TreeBeanManager.java \
       ../DataConvert/PHYLIP/PHYLIP.java \
       ../DataConvert/PHYLIP/PHYLIPPanel.java \
       ../DataConvert/PHYLIP/PHYLIPReader.java \
       ../DataConvert/PHYLIP/PHYLIPStub.java \
       ../DataConvert/PHYLIP/PHYLIPUsageBean.java \
       ../DataConvert/PHYLIP/PHYLIPWriter.java \
       ../DataConvert/RST/RST.java \
       ../DataConvert/RST/RSTReader.java \
       ../DataConvert/RST/RSTStub.java \
       ../DataConvert/RST/RSTUsageBean.java \
       ../DataConvert/TCS/TCS.java \
       ../DataConvert/TCS/TCSReader.java \
       ../DataConvert/TCS/TCSStub.java \
       ../DataConvert/TCS/TCSUsageBean.java \
       ../DataConvert/XML/XML.java \
       ../DataConvert/XML/XMLReader.java \
       ../DataConvert/XML/XMLStub.java \
       ../DataConvert/XML/XMLUsageBean.java 


buildgui: buildJalview buildpsoda $(guijar)
	echo "Gui built."

clean:
	-rm *.class
	-rm ../DataConvert/GraphicInterface/*.class
	-rm ../treesaap/*/*.class
	-rm ../treesaap/*/*/*.class
	-rm psodagui.jar
	-rm $(guijar)
	-rm $(SHAREDLIBFILE)

$(guijar): psodagui.jar $(SHAREDLIBFILE)
	-rm -rf ../one-jar-dir
	mkdir ../one-jar-dir
	cd ../one-jar-dir; jar -xf ../../one-jar/one-jar-boot*.jar; mkdir main; mkdir binlib; mkdir lib;
	cp psodagui.jar ../one-jar-dir/main/main.jar;
	cp ../lib/* ../one-jar-dir/lib/
	cp Jalview/lib/* ../one-jar-dir/lib/
	cp Jalview/dist/jalview.jar ../one-jar-dir/lib/
	cp $(SHAREDLIBFILE) ../one-jar-dir/binlib/;
	cd ../one-jar-dir; jar -cfM $(guijar) *
	-rm -rf ../one-jar-dir

psodagui.jar: ../$(AM_EXE) $(treesaapJavaFiles) $(psodaGuiFiles)
	javac -cp $(JAVACLASSPATH)  $(treesaapJavaFiles) 
	javac -cp $(JAVACLASSPATH)  $(dataconvertJavaFiles) 
	javac -cp $(JAVACLASSPATH) $(psodaGuiFiles)
	cd ../; find . -name "*.class" -print > psodaguijarfiles.txt
	cd ../; find . -name "*.png" -print >> psodaguijarfiles.txt
	cd ../; find . -name "*.jpg" -print >> psodaguijarfiles.txt
	cd ../; jar cmf gui/mainClass.txt gui/psodagui.jar @psodaguijarfiles.txt

buildJalview:
	cd Jalview; ant makedist

cleangui: clean
	-rm `bash listguiofiles.sh`

GUIPSODA.h: PSODA.java
	javac -cp $(JAVACLASSPATH) PSODA.java
	rm -f GUIPSODA.h
	javah -o GUIPSODA.h -classpath $(JAVACLASSPATH) gui.PSODA

GuiCommandNode.h: GuiCommandNode.java
	javac -cp $(JAVACLASSPATH) GuiCommandNode.java
	rm GuiCommandNode.h
	javah -o GuiCommandNode.h -classpath $(JAVACLASSPATH) gui.GuiCommandNode

$(SHAREDLIBFILE): ../$(AM_EXE)
	-mkdir $(guiarchivedir)
	-rm .psodaguiar/*.o $(guiarchivedir)/*.a
	cp ../*.o $(guiarchivedir)/
	cp ../fann-2.0.0/src/.libs/libfloatfann.a $(guiarchivedir)/
	$(SHAREDLIBBUILD)
	-rm $(guiarchivedir)/*.o $(guiarchivedir)/*.a
	-rmdir $(guiarchivedir)

buildpsoda: GUIPSODA.h GuiCommandNode.h
	$(MAKE) -C ${top_srcdir}
