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
include test/CMakeFiles/disk_scheduler_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/disk_scheduler_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/disk_scheduler_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/disk_scheduler_test.dir/flags.make

test/CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o: test/CMakeFiles/disk_scheduler_test.dir/flags.make
test/CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o: ../test/storage/disk_scheduler_test.cpp
test/CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o: test/CMakeFiles/disk_scheduler_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/woo/bustub/build_rel/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o"
	cd /home/woo/bustub/build_rel/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o -MF CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o.d -o CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o -c /home/woo/bustub/test/storage/disk_scheduler_test.cpp

test/CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.i"
	cd /home/woo/bustub/build_rel/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/woo/bustub/test/storage/disk_scheduler_test.cpp > CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.i

test/CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.s"
	cd /home/woo/bustub/build_rel/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/woo/bustub/test/storage/disk_scheduler_test.cpp -o CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.s

test/CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o: test/CMakeFiles/disk_scheduler_test.dir/flags.make
test/CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o: ../tools/backtrace.cpp
test/CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o: test/CMakeFiles/disk_scheduler_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/woo/bustub/build_rel/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o"
	cd /home/woo/bustub/build_rel/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o -MF CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o.d -o CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o -c /home/woo/bustub/tools/backtrace.cpp

test/CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.i"
	cd /home/woo/bustub/build_rel/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/woo/bustub/tools/backtrace.cpp > CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.i

test/CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.s"
	cd /home/woo/bustub/build_rel/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/woo/bustub/tools/backtrace.cpp -o CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.s

# Object files for target disk_scheduler_test
disk_scheduler_test_OBJECTS = \
"CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o" \
"CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o"

# External object files for target disk_scheduler_test
disk_scheduler_test_EXTERNAL_OBJECTS =

test/disk_scheduler_test: test/CMakeFiles/disk_scheduler_test.dir/storage/disk_scheduler_test.cpp.o
test/disk_scheduler_test: test/CMakeFiles/disk_scheduler_test.dir/__/tools/backtrace.cpp.o
test/disk_scheduler_test: test/CMakeFiles/disk_scheduler_test.dir/build.make
test/disk_scheduler_test: /usr/lib/x86_64-linux-gnu/libdwarf.so
test/disk_scheduler_test: /usr/lib/x86_64-linux-gnu/libelf.so
test/disk_scheduler_test: /usr/lib/x86_64-linux-gnu/libdl.a
test/disk_scheduler_test: lib/libbustub.a
test/disk_scheduler_test: lib/libgtest.a
test/disk_scheduler_test: lib/libgmock_main.a
test/disk_scheduler_test: lib/libbustub_murmur3.a
test/disk_scheduler_test: lib/libduckdb_pg_query.a
test/disk_scheduler_test: lib/libfmtd.a
test/disk_scheduler_test: lib/libfort.a
test/disk_scheduler_test: lib/libgmock.a
test/disk_scheduler_test: lib/libgtest.a
test/disk_scheduler_test: test/CMakeFiles/disk_scheduler_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/woo/bustub/build_rel/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable disk_scheduler_test"
	cd /home/woo/bustub/build_rel/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/disk_scheduler_test.dir/link.txt --verbose=$(VERBOSE)
	cd /home/woo/bustub/build_rel/test && /usr/bin/cmake -D TEST_TARGET=disk_scheduler_test -D TEST_EXECUTABLE=/home/woo/bustub/build_rel/test/disk_scheduler_test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/woo/bustub/build_rel/test -D "TEST_EXTRA_ARGS=--gtest_output=xml:/home/woo/bustub/build_rel/test/disk_scheduler_test.xml;--gtest_catch_exceptions=0" -D "TEST_PROPERTIES=TIMEOUT;120" -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=disk_scheduler_test_TESTS -D CTEST_FILE=/home/woo/bustub/build_rel/test/disk_scheduler_test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=120 -D TEST_XML_OUTPUT_DIR= -P /usr/share/cmake-3.22/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
test/CMakeFiles/disk_scheduler_test.dir/build: test/disk_scheduler_test
.PHONY : test/CMakeFiles/disk_scheduler_test.dir/build

test/CMakeFiles/disk_scheduler_test.dir/clean:
	cd /home/woo/bustub/build_rel/test && $(CMAKE_COMMAND) -P CMakeFiles/disk_scheduler_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/disk_scheduler_test.dir/clean

test/CMakeFiles/disk_scheduler_test.dir/depend:
	cd /home/woo/bustub/build_rel && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/woo/bustub /home/woo/bustub/test /home/woo/bustub/build_rel /home/woo/bustub/build_rel/test /home/woo/bustub/build_rel/test/CMakeFiles/disk_scheduler_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/disk_scheduler_test.dir/depend

