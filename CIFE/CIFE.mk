##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release_-_MSW_static
ProjectName            :=CIFE
ConfigurationName      :=Release-MSW_static
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=Z:/wxWidgets/Projekte/CPM_Image-File_Explorer
ProjectPath            :=Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE
IntermediateDirectory  :=../build-$(ConfigurationName)/CIFE
OutDir                 :=../build-$(ConfigurationName)/CIFE
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Uwe
Date                   :=30.12.2021
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := $(shell wx-config --rcflags)
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  -s -mwindows $(shell wx-config --libs --unicode=yes --static=yes)
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
CXXFLAGS :=  -O3 -Wall $(shell wx-config --cxxflags --unicode=yes --static=yes) $(Preprocessors)
CFLAGS   :=  -O3 -Wall $(shell wx-config --cxxflags --unicode=yes --static=yes) $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\CodeLite
MINGW:=C:\TDM-GCC-64
WXWIN:=C:\wxWidgets_315
wxcfg:=\gcc_lib\mswu
Objects0=../build-$(ConfigurationName)/CIFE/main.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/CreateFileDialog.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/cife.rc$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/RenameFileDialog.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp$(ObjectSuffix) \
	../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog_bitmaps.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/FileAttributesDialog.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog_bitmaps.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/FileProtectionsDialog.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(ObjectSuffix) 



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

../build-$(ConfigurationName)/CIFE/CreateFileDialog.cpp$(ObjectSuffix): CreateFileDialog.cpp ../build-$(ConfigurationName)/CIFE/CreateFileDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/CreateFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CreateFileDialog.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/CreateFileDialog.cpp$(DependSuffix): CreateFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/CreateFileDialog.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/CreateFileDialog.cpp$(DependSuffix) -MM CreateFileDialog.cpp

../build-$(ConfigurationName)/CIFE/CreateFileDialog.cpp$(PreprocessSuffix): CreateFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/CreateFileDialog.cpp$(PreprocessSuffix) CreateFileDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix): Ui_MainWindow_bitmaps.cpp ../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_MainWindow_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(DependSuffix): Ui_MainWindow_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(DependSuffix) -MM Ui_MainWindow_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(PreprocessSuffix): Ui_MainWindow_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_MainWindow_bitmaps.cpp$(PreprocessSuffix) Ui_MainWindow_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(ObjectSuffix): CpmGuiInterface.cpp ../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/CpmGuiInterface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CpmGuiInterface.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(DependSuffix): CpmGuiInterface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(DependSuffix) -MM CpmGuiInterface.cpp

../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(PreprocessSuffix): CpmGuiInterface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/CpmGuiInterface.cpp$(PreprocessSuffix) CpmGuiInterface.cpp

../build-$(ConfigurationName)/CIFE/cife.rc$(ObjectSuffix): cife.rc
	$(RcCompilerName) -i "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/cife.rc" $(RcCmpOptions)   $(ObjectSwitch)$(IntermediateDirectory)/cife.rc$(ObjectSuffix) $(RcIncludePath)
../build-$(ConfigurationName)/CIFE/RenameFileDialog.cpp$(ObjectSuffix): RenameFileDialog.cpp ../build-$(ConfigurationName)/CIFE/RenameFileDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/RenameFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenameFileDialog.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/RenameFileDialog.cpp$(DependSuffix): RenameFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/RenameFileDialog.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/RenameFileDialog.cpp$(DependSuffix) -MM RenameFileDialog.cpp

../build-$(ConfigurationName)/CIFE/RenameFileDialog.cpp$(PreprocessSuffix): RenameFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/RenameFileDialog.cpp$(PreprocessSuffix) RenameFileDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog.cpp$(ObjectSuffix): Ui_RenameFileDialog.cpp ../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_RenameFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_RenameFileDialog.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog.cpp$(DependSuffix): Ui_RenameFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog.cpp$(DependSuffix) -MM Ui_RenameFileDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog.cpp$(PreprocessSuffix): Ui_RenameFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog.cpp$(PreprocessSuffix) Ui_RenameFileDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog.cpp$(ObjectSuffix): Ui_CreateFileDialog.cpp ../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_CreateFileDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_CreateFileDialog.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog.cpp$(DependSuffix): Ui_CreateFileDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog.cpp$(DependSuffix) -MM Ui_CreateFileDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog.cpp$(PreprocessSuffix): Ui_CreateFileDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog.cpp$(PreprocessSuffix) Ui_CreateFileDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp$(ObjectSuffix): Ui_CreateFileDialog_cife_bitmaps.cpp ../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_CreateFileDialog_cife_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp$(DependSuffix): Ui_CreateFileDialog_cife_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp$(DependSuffix) -MM Ui_CreateFileDialog_cife_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp$(PreprocessSuffix): Ui_CreateFileDialog_cife_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_CreateFileDialog_cife_bitmaps.cpp$(PreprocessSuffix) Ui_CreateFileDialog_cife_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp$(ObjectSuffix): Ui_FileProtectionsDialog_bitmaps.cpp ../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_FileProtectionsDialog_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp$(DependSuffix): Ui_FileProtectionsDialog_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp$(DependSuffix) -MM Ui_FileProtectionsDialog_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp$(PreprocessSuffix): Ui_FileProtectionsDialog_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog_bitmaps.cpp$(PreprocessSuffix) Ui_FileProtectionsDialog_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(ObjectSuffix): Ui_MainWindow.cpp ../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_MainWindow.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(DependSuffix): Ui_MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(DependSuffix) -MM Ui_MainWindow.cpp

../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(PreprocessSuffix): Ui_MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_MainWindow.cpp$(PreprocessSuffix) Ui_MainWindow.cpp

../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog_bitmaps.cpp$(ObjectSuffix): Ui_FileAttributesDialog_bitmaps.cpp ../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_FileAttributesDialog_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_FileAttributesDialog_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog_bitmaps.cpp$(DependSuffix): Ui_FileAttributesDialog_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog_bitmaps.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog_bitmaps.cpp$(DependSuffix) -MM Ui_FileAttributesDialog_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog_bitmaps.cpp$(PreprocessSuffix): Ui_FileAttributesDialog_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog_bitmaps.cpp$(PreprocessSuffix) Ui_FileAttributesDialog_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/FileAttributesDialog.cpp$(ObjectSuffix): FileAttributesDialog.cpp ../build-$(ConfigurationName)/CIFE/FileAttributesDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/FileAttributesDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileAttributesDialog.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/FileAttributesDialog.cpp$(DependSuffix): FileAttributesDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/FileAttributesDialog.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/FileAttributesDialog.cpp$(DependSuffix) -MM FileAttributesDialog.cpp

../build-$(ConfigurationName)/CIFE/FileAttributesDialog.cpp$(PreprocessSuffix): FileAttributesDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/FileAttributesDialog.cpp$(PreprocessSuffix) FileAttributesDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog.cpp$(ObjectSuffix): Ui_FileProtectionsDialog.cpp ../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_FileProtectionsDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_FileProtectionsDialog.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog.cpp$(DependSuffix): Ui_FileProtectionsDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog.cpp$(DependSuffix) -MM Ui_FileProtectionsDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog.cpp$(PreprocessSuffix): Ui_FileProtectionsDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_FileProtectionsDialog.cpp$(PreprocessSuffix) Ui_FileProtectionsDialog.cpp

../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp ../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(DependSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(DependSuffix) -MM MainWindow.cpp

../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/MainWindow.cpp$(PreprocessSuffix) MainWindow.cpp

../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog.cpp$(ObjectSuffix): Ui_FileAttributesDialog.cpp ../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_FileAttributesDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_FileAttributesDialog.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog.cpp$(DependSuffix): Ui_FileAttributesDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog.cpp$(DependSuffix) -MM Ui_FileAttributesDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog.cpp$(PreprocessSuffix): Ui_FileAttributesDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_FileAttributesDialog.cpp$(PreprocessSuffix) Ui_FileAttributesDialog.cpp

../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog_bitmaps.cpp$(ObjectSuffix): Ui_RenameFileDialog_bitmaps.cpp ../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog_bitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/Ui_RenameFileDialog_bitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Ui_RenameFileDialog_bitmaps.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog_bitmaps.cpp$(DependSuffix): Ui_RenameFileDialog_bitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog_bitmaps.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog_bitmaps.cpp$(DependSuffix) -MM Ui_RenameFileDialog_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog_bitmaps.cpp$(PreprocessSuffix): Ui_RenameFileDialog_bitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/Ui_RenameFileDialog_bitmaps.cpp$(PreprocessSuffix) Ui_RenameFileDialog_bitmaps.cpp

../build-$(ConfigurationName)/CIFE/FileProtectionsDialog.cpp$(ObjectSuffix): FileProtectionsDialog.cpp ../build-$(ConfigurationName)/CIFE/FileProtectionsDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/FileProtectionsDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileProtectionsDialog.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/FileProtectionsDialog.cpp$(DependSuffix): FileProtectionsDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/FileProtectionsDialog.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/FileProtectionsDialog.cpp$(DependSuffix) -MM FileProtectionsDialog.cpp

../build-$(ConfigurationName)/CIFE/FileProtectionsDialog.cpp$(PreprocessSuffix): FileProtectionsDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/FileProtectionsDialog.cpp$(PreprocessSuffix) FileProtectionsDialog.cpp

../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(ObjectSuffix): CpmTools.cpp ../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "Z:/wxWidgets/Projekte/CPM_Image-File_Explorer/CIFE/CpmTools.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CpmTools.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(DependSuffix): CpmTools.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(DependSuffix) -MM CpmTools.cpp

../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(PreprocessSuffix): CpmTools.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CIFE/CpmTools.cpp$(PreprocessSuffix) CpmTools.cpp


-include ../build-$(ConfigurationName)/CIFE//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


