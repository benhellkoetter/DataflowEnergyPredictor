# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspaces/DataflowEnergyPredictor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspaces/DataflowEnergyPredictor/build

# Include any dependencies generated for this target.
include CMakeFiles/DataflowEnergyPredictor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DataflowEnergyPredictor.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DataflowEnergyPredictor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DataflowEnergyPredictor.dir/flags.make

CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o: CMakeFiles/DataflowEnergyPredictor.dir/flags.make
CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o: /workspaces/DataflowEnergyPredictor/src/main.cpp
CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o: CMakeFiles/DataflowEnergyPredictor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspaces/DataflowEnergyPredictor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o -MF CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o.d -o CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o -c /workspaces/DataflowEnergyPredictor/src/main.cpp

CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspaces/DataflowEnergyPredictor/src/main.cpp > CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.i

CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspaces/DataflowEnergyPredictor/src/main.cpp -o CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.s

CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o: CMakeFiles/DataflowEnergyPredictor.dir/flags.make
CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o: /workspaces/DataflowEnergyPredictor/src/EnergyOfValueAnalysis.cpp
CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o: CMakeFiles/DataflowEnergyPredictor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspaces/DataflowEnergyPredictor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o -MF CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o.d -o CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o -c /workspaces/DataflowEnergyPredictor/src/EnergyOfValueAnalysis.cpp

CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspaces/DataflowEnergyPredictor/src/EnergyOfValueAnalysis.cpp > CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.i

CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspaces/DataflowEnergyPredictor/src/EnergyOfValueAnalysis.cpp -o CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.s

CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o: CMakeFiles/DataflowEnergyPredictor.dir/flags.make
CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o: /workspaces/DataflowEnergyPredictor/src/EnergyModel.cpp
CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o: CMakeFiles/DataflowEnergyPredictor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspaces/DataflowEnergyPredictor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o -MF CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o.d -o CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o -c /workspaces/DataflowEnergyPredictor/src/EnergyModel.cpp

CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspaces/DataflowEnergyPredictor/src/EnergyModel.cpp > CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.i

CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspaces/DataflowEnergyPredictor/src/EnergyModel.cpp -o CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.s

# Object files for target DataflowEnergyPredictor
DataflowEnergyPredictor_OBJECTS = \
"CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o" \
"CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o" \
"CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o"

# External object files for target DataflowEnergyPredictor
DataflowEnergyPredictor_EXTERNAL_OBJECTS =

DataflowEnergyPredictor: CMakeFiles/DataflowEnergyPredictor.dir/src/main.cpp.o
DataflowEnergyPredictor: CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyOfValueAnalysis.cpp.o
DataflowEnergyPredictor: CMakeFiles/DataflowEnergyPredictor.dir/src/EnergyModel.cpp.o
DataflowEnergyPredictor: CMakeFiles/DataflowEnergyPredictor.dir/build.make
DataflowEnergyPredictor: /usr/local/lib/libphasar_llvm_ifdside.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_llvm.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_taintconfig.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_llvm_controlflow.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_llvm_pointer.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_pointer.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMPasses.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMCoroutines.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMipo.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMFrontendOpenMP.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMLinker.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMObjCARCOpts.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMScalarOpts.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMAggressiveInstCombine.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMInstCombine.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMTarget.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMVectorize.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMInstrumentation.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMTransformUtils.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_llvm_typehierarchy.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_llvm_db.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_llvm_utils.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_config.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMIRReader.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMAsmParser.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_controlflow.a
DataflowEnergyPredictor: /usr/local/lib/libphasar_utils.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMBitWriter.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMAnalysis.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMProfileData.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMSymbolize.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMDebugInfoPDB.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMDebugInfoMSF.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMDebugInfoDWARF.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMObject.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMBitReader.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMMCParser.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMMC.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMDebugInfoCodeView.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMTextAPI.a
DataflowEnergyPredictor: /usr/local/lib/phasar/deps/lib/libnlohmann_json_schema_validator.a
DataflowEnergyPredictor: /usr/lib/x86_64-linux-gnu/libboost_graph.so.1.74.0
DataflowEnergyPredictor: /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.74.0
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMCore.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMBinaryFormat.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMRemarks.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMBitstreamReader.a
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMSupport.a
DataflowEnergyPredictor: /usr/lib/x86_64-linux-gnu/libz.so
DataflowEnergyPredictor: /usr/lib/x86_64-linux-gnu/libtinfo.so
DataflowEnergyPredictor: /usr/lib/llvm-15/lib/libLLVMDemangle.a
DataflowEnergyPredictor: CMakeFiles/DataflowEnergyPredictor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/workspaces/DataflowEnergyPredictor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable DataflowEnergyPredictor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DataflowEnergyPredictor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DataflowEnergyPredictor.dir/build: DataflowEnergyPredictor
.PHONY : CMakeFiles/DataflowEnergyPredictor.dir/build

CMakeFiles/DataflowEnergyPredictor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DataflowEnergyPredictor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DataflowEnergyPredictor.dir/clean

CMakeFiles/DataflowEnergyPredictor.dir/depend:
	cd /workspaces/DataflowEnergyPredictor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspaces/DataflowEnergyPredictor /workspaces/DataflowEnergyPredictor /workspaces/DataflowEnergyPredictor/build /workspaces/DataflowEnergyPredictor/build /workspaces/DataflowEnergyPredictor/build/CMakeFiles/DataflowEnergyPredictor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DataflowEnergyPredictor.dir/depend

