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

# Include any dependencies generated for this target.
include src/primer/CMakeFiles/bustub_primer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/primer/CMakeFiles/bustub_primer.dir/compiler_depend.make

# Include the progress variables for this target.
include src/primer/CMakeFiles/bustub_primer.dir/progress.make

# Include the compile flags for this target's objects.
include src/primer/CMakeFiles/bustub_primer.dir/flags.make

src/primer/CMakeFiles/bustub_primer.dir/orset.cpp.o: src/primer/CMakeFiles/bustub_primer.dir/flags.make
src/primer/CMakeFiles/bustub_primer.dir/orset.cpp.o: ../src/primer/orset.cpp
src/primer/CMakeFiles/bustub_primer.dir/orset.cpp.o: src/primer/CMakeFiles/bustub_primer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/woo/bustub/build_rel/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/primer/CMakeFiles/bustub_primer.dir/orset.cpp.o"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/primer/CMakeFiles/bustub_primer.dir/orset.cpp.o -MF CMakeFiles/bustub_primer.dir/orset.cpp.o.d -o CMakeFiles/bustub_primer.dir/orset.cpp.o -c /home/woo/bustub/src/primer/orset.cpp

src/primer/CMakeFiles/bustub_primer.dir/orset.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bustub_primer.dir/orset.cpp.i"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/woo/bustub/src/primer/orset.cpp > CMakeFiles/bustub_primer.dir/orset.cpp.i

src/primer/CMakeFiles/bustub_primer.dir/orset.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bustub_primer.dir/orset.cpp.s"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/woo/bustub/src/primer/orset.cpp -o CMakeFiles/bustub_primer.dir/orset.cpp.s

src/primer/CMakeFiles/bustub_primer.dir/orset_driver.cpp.o: src/primer/CMakeFiles/bustub_primer.dir/flags.make
src/primer/CMakeFiles/bustub_primer.dir/orset_driver.cpp.o: ../src/primer/orset_driver.cpp
src/primer/CMakeFiles/bustub_primer.dir/orset_driver.cpp.o: src/primer/CMakeFiles/bustub_primer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/woo/bustub/build_rel/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/primer/CMakeFiles/bustub_primer.dir/orset_driver.cpp.o"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/primer/CMakeFiles/bustub_primer.dir/orset_driver.cpp.o -MF CMakeFiles/bustub_primer.dir/orset_driver.cpp.o.d -o CMakeFiles/bustub_primer.dir/orset_driver.cpp.o -c /home/woo/bustub/src/primer/orset_driver.cpp

src/primer/CMakeFiles/bustub_primer.dir/orset_driver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bustub_primer.dir/orset_driver.cpp.i"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/woo/bustub/src/primer/orset_driver.cpp > CMakeFiles/bustub_primer.dir/orset_driver.cpp.i

src/primer/CMakeFiles/bustub_primer.dir/orset_driver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bustub_primer.dir/orset_driver.cpp.s"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/woo/bustub/src/primer/orset_driver.cpp -o CMakeFiles/bustub_primer.dir/orset_driver.cpp.s

src/primer/CMakeFiles/bustub_primer.dir/trie.cpp.o: src/primer/CMakeFiles/bustub_primer.dir/flags.make
src/primer/CMakeFiles/bustub_primer.dir/trie.cpp.o: ../src/primer/trie.cpp
src/primer/CMakeFiles/bustub_primer.dir/trie.cpp.o: src/primer/CMakeFiles/bustub_primer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/woo/bustub/build_rel/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/primer/CMakeFiles/bustub_primer.dir/trie.cpp.o"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/primer/CMakeFiles/bustub_primer.dir/trie.cpp.o -MF CMakeFiles/bustub_primer.dir/trie.cpp.o.d -o CMakeFiles/bustub_primer.dir/trie.cpp.o -c /home/woo/bustub/src/primer/trie.cpp

src/primer/CMakeFiles/bustub_primer.dir/trie.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bustub_primer.dir/trie.cpp.i"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/woo/bustub/src/primer/trie.cpp > CMakeFiles/bustub_primer.dir/trie.cpp.i

src/primer/CMakeFiles/bustub_primer.dir/trie.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bustub_primer.dir/trie.cpp.s"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/woo/bustub/src/primer/trie.cpp -o CMakeFiles/bustub_primer.dir/trie.cpp.s

src/primer/CMakeFiles/bustub_primer.dir/trie_store.cpp.o: src/primer/CMakeFiles/bustub_primer.dir/flags.make
src/primer/CMakeFiles/bustub_primer.dir/trie_store.cpp.o: ../src/primer/trie_store.cpp
src/primer/CMakeFiles/bustub_primer.dir/trie_store.cpp.o: src/primer/CMakeFiles/bustub_primer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/woo/bustub/build_rel/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/primer/CMakeFiles/bustub_primer.dir/trie_store.cpp.o"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/primer/CMakeFiles/bustub_primer.dir/trie_store.cpp.o -MF CMakeFiles/bustub_primer.dir/trie_store.cpp.o.d -o CMakeFiles/bustub_primer.dir/trie_store.cpp.o -c /home/woo/bustub/src/primer/trie_store.cpp

src/primer/CMakeFiles/bustub_primer.dir/trie_store.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bustub_primer.dir/trie_store.cpp.i"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/woo/bustub/src/primer/trie_store.cpp > CMakeFiles/bustub_primer.dir/trie_store.cpp.i

src/primer/CMakeFiles/bustub_primer.dir/trie_store.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bustub_primer.dir/trie_store.cpp.s"
	cd /home/woo/bustub/build_rel/src/primer && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/woo/bustub/src/primer/trie_store.cpp -o CMakeFiles/bustub_primer.dir/trie_store.cpp.s

bustub_primer: src/primer/CMakeFiles/bustub_primer.dir/orset.cpp.o
bustub_primer: src/primer/CMakeFiles/bustub_primer.dir/orset_driver.cpp.o
bustub_primer: src/primer/CMakeFiles/bustub_primer.dir/trie.cpp.o
bustub_primer: src/primer/CMakeFiles/bustub_primer.dir/trie_store.cpp.o
bustub_primer: src/primer/CMakeFiles/bustub_primer.dir/build.make
.PHONY : bustub_primer

# Rule to build all files generated by this target.
src/primer/CMakeFiles/bustub_primer.dir/build: bustub_primer
.PHONY : src/primer/CMakeFiles/bustub_primer.dir/build

src/primer/CMakeFiles/bustub_primer.dir/clean:
	cd /home/woo/bustub/build_rel/src/primer && $(CMAKE_COMMAND) -P CMakeFiles/bustub_primer.dir/cmake_clean.cmake
.PHONY : src/primer/CMakeFiles/bustub_primer.dir/clean

src/primer/CMakeFiles/bustub_primer.dir/depend:
	cd /home/woo/bustub/build_rel && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/woo/bustub /home/woo/bustub/src/primer /home/woo/bustub/build_rel /home/woo/bustub/build_rel/src/primer /home/woo/bustub/build_rel/src/primer/CMakeFiles/bustub_primer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/primer/CMakeFiles/bustub_primer.dir/depend

