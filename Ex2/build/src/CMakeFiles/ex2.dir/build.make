# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/2.8.7/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/2.8.7/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/Cellar/cmake/2.8.7/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ricoh/ETH/RW/CSP/Ex/Ex2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build

# Include any dependencies generated for this target.
include src/CMakeFiles/ex2.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/ex2.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/ex2.dir/flags.make

src/CMakeFiles/ex2.dir/ex2.cpp.o: src/CMakeFiles/ex2.dir/flags.make
src/CMakeFiles/ex2.dir/ex2.cpp.o: ../src/ex2.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/ex2.dir/ex2.cpp.o"
	cd /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ex2.dir/ex2.cpp.o -c /Users/ricoh/ETH/RW/CSP/Ex/Ex2/src/ex2.cpp

src/CMakeFiles/ex2.dir/ex2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex2.dir/ex2.cpp.i"
	cd /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/ricoh/ETH/RW/CSP/Ex/Ex2/src/ex2.cpp > CMakeFiles/ex2.dir/ex2.cpp.i

src/CMakeFiles/ex2.dir/ex2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex2.dir/ex2.cpp.s"
	cd /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/ricoh/ETH/RW/CSP/Ex/Ex2/src/ex2.cpp -o CMakeFiles/ex2.dir/ex2.cpp.s

src/CMakeFiles/ex2.dir/ex2.cpp.o.requires:
.PHONY : src/CMakeFiles/ex2.dir/ex2.cpp.o.requires

src/CMakeFiles/ex2.dir/ex2.cpp.o.provides: src/CMakeFiles/ex2.dir/ex2.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ex2.dir/build.make src/CMakeFiles/ex2.dir/ex2.cpp.o.provides.build
.PHONY : src/CMakeFiles/ex2.dir/ex2.cpp.o.provides

src/CMakeFiles/ex2.dir/ex2.cpp.o.provides.build: src/CMakeFiles/ex2.dir/ex2.cpp.o

# Object files for target ex2
ex2_OBJECTS = \
"CMakeFiles/ex2.dir/ex2.cpp.o"

# External object files for target ex2
ex2_EXTERNAL_OBJECTS =

src/ex2: src/CMakeFiles/ex2.dir/ex2.cpp.o
src/ex2: src/CMakeFiles/ex2.dir/build.make
src/ex2: src/CMakeFiles/ex2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ex2"
	cd /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/ex2.dir/build: src/ex2
.PHONY : src/CMakeFiles/ex2.dir/build

src/CMakeFiles/ex2.dir/requires: src/CMakeFiles/ex2.dir/ex2.cpp.o.requires
.PHONY : src/CMakeFiles/ex2.dir/requires

src/CMakeFiles/ex2.dir/clean:
	cd /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build/src && $(CMAKE_COMMAND) -P CMakeFiles/ex2.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/ex2.dir/clean

src/CMakeFiles/ex2.dir/depend:
	cd /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ricoh/ETH/RW/CSP/Ex/Ex2 /Users/ricoh/ETH/RW/CSP/Ex/Ex2/src /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build/src /Users/ricoh/ETH/RW/CSP/Ex/Ex2/build/src/CMakeFiles/ex2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/ex2.dir/depend
