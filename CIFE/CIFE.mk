##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## GTK-Debug_wx3.1_64bit_static
ProjectName            :=CIFE
ConfigurationName      :=GTK-Debug_wx3.1_64bit_static
WorkspaceConfiguration :=GTK-Debug_wx3.1_64bit_static
WorkspacePath          :=/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer
ProjectPath            :=/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE
IntermediateDirectory  :=../build_$(ProjectName)_$(WorkspaceConfiguration)/lib
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Uwe
Date                   :=08/05/22
CodeLitePath           :=/home/uwe/.codelite
MakeDirCommand         :=mkdir -p
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
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
OutputDirectory        :=/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/build_$(ProjectName)_$(WorkspaceConfiguration)/bin
OutputFile             :=../build_$(ProjectName)_$(WorkspaceConfiguration)/bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  -no-pie $(shell /usr/local/bin/wx-config --libs --unicode=yes --static=yes)
IncludePath            :=  $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(shell /usr/local/bin/wx-config --cxxflags --unicode=yes --static=yes) $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(shell /usr/local/bin/wx-config --cxxflags --unicode=yes --static=yes) $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/CpmTools.cpp$(ObjectSuffix) $(IntermediateDirectory)/RenameFileDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/CpmGuiInterface.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_MainWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/CreateFileDialog.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/FileAttributesDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/FileProtectionsDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@$(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp $(IntermediateDirectory)/MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix) -MM MainWindow.cpp

$(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix) MainWindow.cpp

$(IntermediateDirectory)/CpmTools.cpp$(ObjectSuffix): CpmTools.cpp $(IntermediateDirectory)/CpmTools.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/CpmTools.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CpmTools.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CpmTools.cpp$(DependSuffix): CpmTools.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CpmTools.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CpmTools.cpp$(DependSuffix) -MM CpmTools.cpp

$(IntermediateDirectory)/CpmTools.cpp$(PreprocessSuffix): CpmTools.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CpmTools.cpp$(PreprocessSuffix) CpmTools.cpp

$(IntermediateDirectory)/RenameFileDialog.cpp$(ObjectSuffix): RenameFileDialog.cpp $(IntermediateDirectory)/RenameFileDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/RenameFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenameFileDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenameFileDialog.cpp$(DependSuffix): RenameFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenameFileDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenameFileDialog.cpp$(DependSuffix) -MM RenameFileDialog.cpp

$(IntermediateDirectory)/RenameFileDialog.cpp$(PreprocessSuffix): RenameFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenameFileDialog.cpp$(PreprocessSuffix) RenameFileDialog.cpp

$(IntermediateDirectory)/CpmGuiInterface.cpp$(ObjectSuffix): CpmGuiInterface.cpp $(IntermediateDirectory)/CpmGuiInterface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/CpmGuiInterface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CpmGuiInterface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CpmGuiInterface.cpp$(DependSuffix): CpmGuiInterface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CpmGuiInterface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CpmGuiInterface.cpp$(DependSuffix) -MM CpmGuiInterface.cpp

$(IntermediateDirectory)/CpmGuiInterface.cpp$(PreprocessSuffix): CpmGuiInterface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CpmGuiInterface.cpp$(PreprocessSuffix) CpmGuiInterface.cpp

$(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(ObjectSuffix): Ui_RenameFileDialog.cpp $(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_RenameFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(DependSuffix): Ui_RenameFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(DependSuffix) -MM Ui_RenameFileDialog.cpp

$(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(PreprocessSuffix): Ui_RenameFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(PreprocessSuffix) Ui_RenameFileDialog.cpp

$(IntermediateDirectory)/Ui_MainWindow.cpp$(ObjectSuffix): Ui_MainWindow.cpp $(IntermediateDirectory)/Ui_MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_MainWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_MainWindow.cpp$(DependSuffix): Ui_MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_MainWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_MainWindow.cpp$(DependSuffix) -MM Ui_MainWindow.cpp

$(IntermediateDirectory)/Ui_MainWindow.cpp$(PreprocessSuffix): Ui_MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_MainWindow.cpp$(PreprocessSuffix) Ui_MainWindow.cpp

$(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(ObjectSuffix): Ui_FileAttributesDialog.cpp $(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_FileAttributesDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(DependSuffix): Ui_FileAttributesDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(DependSuffix) -MM Ui_FileAttributesDialog.cpp

$(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(PreprocessSuffix): Ui_FileAttributesDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(PreprocessSuffix) Ui_FileAttributesDialog.cpp

$(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(ObjectSuffix): Ui_RenameFileDialog_bitmaps.cpp $(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_RenameFileDialog_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(DependSuffix): Ui_RenameFileDialog_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(DependSuffix) -MM Ui_RenameFileDialog_bitmaps.cpp

$(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(PreprocessSuffix): Ui_RenameFileDialog_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(PreprocessSuffix) Ui_RenameFileDialog_bitmaps.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/CreateFileDialog.cpp$(ObjectSuffix): CreateFileDialog.cpp $(IntermediateDirectory)/CreateFileDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/CreateFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CreateFileDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CreateFileDialog.cpp$(DependSuffix): CreateFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CreateFileDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CreateFileDialog.cpp$(DependSuffix) -MM CreateFileDialog.cpp

$(IntermediateDirectory)/CreateFileDialog.cpp$(PreprocessSuffix): CreateFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CreateFileDialog.cpp$(PreprocessSuffix) CreateFileDialog.cpp

$(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(ObjectSuffix): Ui_CreateFileDialog_cife_bitmaps.cpp $(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(DependSuffix): Ui_CreateFileDialog_cife_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(DependSuffix) -MM Ui_CreateFileDialog_cife_bitmaps.cpp

$(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(PreprocessSuffix): Ui_CreateFileDialog_cife_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(PreprocessSuffix) Ui_CreateFileDialog_cife_bitmaps.cpp

$(IntermediateDirectory)/FileAttributesDialog.cpp$(ObjectSuffix): FileAttributesDialog.cpp $(IntermediateDirectory)/FileAttributesDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/FileAttributesDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileAttributesDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileAttributesDialog.cpp$(DependSuffix): FileAttributesDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileAttributesDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileAttributesDialog.cpp$(DependSuffix) -MM FileAttributesDialog.cpp

$(IntermediateDirectory)/FileAttributesDialog.cpp$(PreprocessSuffix): FileAttributesDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileAttributesDialog.cpp$(PreprocessSuffix) FileAttributesDialog.cpp

$(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(ObjectSuffix): Ui_CreateFileDialog.cpp $(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_CreateFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(DependSuffix): Ui_CreateFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(DependSuffix) -MM Ui_CreateFileDialog.cpp

$(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(PreprocessSuffix): Ui_CreateFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(PreprocessSuffix) Ui_CreateFileDialog.cpp

$(IntermediateDirectory)/FileProtectionsDialog.cpp$(ObjectSuffix): FileProtectionsDialog.cpp $(IntermediateDirectory)/FileProtectionsDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/FileProtectionsDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileProtectionsDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileProtectionsDialog.cpp$(DependSuffix): FileProtectionsDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileProtectionsDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileProtectionsDialog.cpp$(DependSuffix) -MM FileProtectionsDialog.cpp

$(IntermediateDirectory)/FileProtectionsDialog.cpp$(PreprocessSuffix): FileProtectionsDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileProtectionsDialog.cpp$(PreprocessSuffix) FileProtectionsDialog.cpp

$(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(ObjectSuffix): Ui_FileProtectionsDialog_bitmaps.cpp $(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(DependSuffix): Ui_FileProtectionsDialog_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(DependSuffix) -MM Ui_FileProtectionsDialog_bitmaps.cpp

$(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(PreprocessSuffix): Ui_FileProtectionsDialog_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(PreprocessSuffix) Ui_FileProtectionsDialog_bitmaps.cpp

$(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix): Ui_MainWindow_bitmaps.cpp $(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_MainWindow_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(DependSuffix): Ui_MainWindow_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(DependSuffix) -MM Ui_MainWindow_bitmaps.cpp

$(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(PreprocessSuffix): Ui_MainWindow_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(PreprocessSuffix) Ui_MainWindow_bitmaps.cpp

$(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(ObjectSuffix): Ui_FileAttributesDialog_bitmaps.cpp $(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_FileAttributesDialog_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(DependSuffix): Ui_FileAttributesDialog_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(DependSuffix) -MM Ui_FileAttributesDialog_bitmaps.cpp

$(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(PreprocessSuffix): Ui_FileAttributesDialog_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(PreprocessSuffix) Ui_FileAttributesDialog_bitmaps.cpp

$(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(ObjectSuffix): Ui_FileProtectionsDialog.cpp $(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_FileProtectionsDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(DependSuffix): Ui_FileProtectionsDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(DependSuffix) -MM Ui_FileProtectionsDialog.cpp

$(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(PreprocessSuffix): Ui_FileProtectionsDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(PreprocessSuffix) Ui_FileProtectionsDialog.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


