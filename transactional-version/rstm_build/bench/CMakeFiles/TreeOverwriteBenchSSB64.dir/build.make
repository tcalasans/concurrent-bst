# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build"

# Include any dependencies generated for this target.
include bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/depend.make

# Include the progress variables for this target.
include bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/progress.make

# Include the compile flags for this target's objects.
include bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/flags.make

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o: bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/flags.make
bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o: /home/titocal/Documents/school\ work/2019\ SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm/bench/TreeOverwriteBench.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o"
	cd "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/bench" && g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o -c "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm/bench/TreeOverwriteBench.cpp"

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.i"
	cd "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/bench" && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm/bench/TreeOverwriteBench.cpp" > CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.i

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.s"
	cd "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/bench" && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm/bench/TreeOverwriteBench.cpp" -o CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.s

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o.requires:

.PHONY : bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o.requires

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o.provides: bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o.requires
	$(MAKE) -f bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/build.make bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o.provides.build
.PHONY : bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o.provides

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o.provides.build: bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o


# Object files for target TreeOverwriteBenchSSB64
TreeOverwriteBenchSSB64_OBJECTS = \
"CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o"

# External object files for target TreeOverwriteBenchSSB64
TreeOverwriteBenchSSB64_EXTERNAL_OBJECTS =

bench/TreeOverwriteBenchSSB64: bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o
bench/TreeOverwriteBenchSSB64: bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/build.make
bench/TreeOverwriteBenchSSB64: libstm/libstm64.a
bench/TreeOverwriteBenchSSB64: bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TreeOverwriteBenchSSB64"
	cd "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/bench" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TreeOverwriteBenchSSB64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/build: bench/TreeOverwriteBenchSSB64

.PHONY : bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/build

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/requires: bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/TreeOverwriteBench.cpp.o.requires

.PHONY : bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/requires

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/clean:
	cd "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/bench" && $(CMAKE_COMMAND) -P CMakeFiles/TreeOverwriteBenchSSB64.dir/cmake_clean.cmake
.PHONY : bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/clean

bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/depend:
	cd "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm" "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm/bench" "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build" "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/bench" "/home/titocal/Documents/school work/2019 SPRINGS/MUlticore/research-project/concurrent-bst/transactional-version/rstm_build/bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : bench/CMakeFiles/TreeOverwriteBenchSSB64.dir/depend

