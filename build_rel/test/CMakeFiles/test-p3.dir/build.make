# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/woo/bustub

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/woo/bustub/build_rel

# Utility rule file for test-p3.

# Include any custom commands dependencies for this target.
include test/CMakeFiles/test-p3.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test-p3.dir/progress.make

test/CMakeFiles/test-p3:
	cd /home/woo/bustub/build_rel/test && /usr/bin/ctest -R SQLLogicTest

test-p3: test/CMakeFiles/test-p3
test-p3: test/CMakeFiles/test-p3.dir/build.make
.PHONY : test-p3

# Rule to build all files generated by this target.
test/CMakeFiles/test-p3.dir/build: test-p3
.PHONY : test/CMakeFiles/test-p3.dir/build

test/CMakeFiles/test-p3.dir/clean:
	cd /home/woo/bustub/build_rel/test && $(CMAKE_COMMAND) -P CMakeFiles/test-p3.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test-p3.dir/clean

test/CMakeFiles/test-p3.dir/depend:
	cd /home/woo/bustub/build_rel && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/woo/bustub /home/woo/bustub/test /home/woo/bustub/build_rel /home/woo/bustub/build_rel/test /home/woo/bustub/build_rel/test/CMakeFiles/test-p3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test-p3.dir/depend

