##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug_-_MSW_static
ProjectName            :=CIFE
ConfigurationName      :=Debug-MSW_static
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=Z:/wxWidgets/Projekte/CPM_Image-File_Explorer
ProjectPath            :=Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE
IntermediateDirectory  :=../build-$(ConfigurationName)/CIFE
OutDir                 :=../build-$(ConfigurationName)/CIFE
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Uwe
Date                   :=11.09.2021
CodeLitePath           :=C:/CodeLite
LinkerName             :=C:/TDM-GCC-64/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=..\build-$(ConfigurationName)\bin\$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := $(shell wx-config --rcflags)
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  -mwindows $(shell wx-config --libs --unicode=yes --static=yes)
IncludePath            :=  $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-64/bin/ar.exe rcu
CXX      := C:/TDM-GCC-64/bin/g++.exe
CC       := C:/TDM-GCC-64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(shell wx-config --cxxflags --unicode=yes --static=yes) $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(shell wx-config --cxxflags --unicode=yes --static=yes) $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\CodeLite
MINGW:=C:\TDM-GCC-64
WXWIN:=C:\wxWidgets_315
Objects0=../build-$(ConfigurationName)/CIFE/main.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/cife.rc$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/CIFE/.d $(Objects) 
	@if not exist "..\build-$(ConfigurationName)\CIFE" mkdir "..\build-$(ConfigurationName)\CIFE"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "..\build-$(ConfigurationName)\CIFE" mkdir "..\build-$(ConfigurationName)\CIFE"
	@if not exist ""..\build-$(ConfigurationName)\bin"" mkdir ""..\build-$(ConfigurationName)\bin""

../build-$(ConfigurationName)/CIFE/.d:
	@if not exist "..\build-$(ConfigurationName)\CIFE" mkdir "..\build-$(ConfigurationName)\CIFE"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/CIFE/main.cpp$(ObjectSuffix): main.cpp ../build-$(ConfigurationName)/CIFE/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/main.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/main.cpp$(DependSuffix) -MM main.cpp

../build-$(ConfigurationName)/CIFE/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/main.cpp$(PreprocessSuffix) main.cpp

../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp ../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(DependSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(DependSuffix) -MM MainWindow.cpp

../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(PreprocessSuffix) MainWindow.cpp

../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(ObjectSuffix): Ui_MainWindow.cpp ../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_MainWindow.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(DependSuffix): Ui_MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(DependSuffix) -MM Ui_MainWindow.cpp

../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(PreprocessSuffix): Ui_MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(PreprocessSuffix) Ui_MainWindow.cpp

../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(ObjectSuffix): CpmGuiInterface.cpp ../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/CpmGuiInterface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CpmGuiInterface.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(DependSuffix): CpmGuiInterface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(DependSuffix) -MM CpmGuiInterface.cpp

../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(PreprocessSuffix): CpmGuiInterface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(PreprocessSuffix) CpmGuiInterface.cpp

../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix): Ui_MainWindow_bitmaps.cpp ../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_MainWindow_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(DependSuffix): Ui_MainWindow_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(DependSuffix) -MM Ui_MainWindow_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(PreprocessSuffix): Ui_MainWindow_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(PreprocessSuffix) Ui_MainWindow_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(ObjectSuffix): CpmTools.cpp ../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/CpmTools.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CpmTools.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(DependSuffix): CpmTools.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(DependSuffix) -MM CpmTools.cpp

../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(PreprocessSuffix): CpmTools.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(PreprocessSuffix) CpmTools.cpp

../build-$(ConfigurationName)/CIFE/cife.rc$(ObjectSuffix): cife.rc
	$(RcCompilerName) -i "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/cife.rc" $(RcCmpOptions)   $(ObjectSwitch)$(IntermediateDirectory)/cife.rc$(ObjectSuffix) $(RcIncludePath)

-include ../build-$(ConfigurationName)/CIFE//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


