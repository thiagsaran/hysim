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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/thiag/priv-git/fmi_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/thiag/priv-git/fmi_test/build

# Include any dependencies generated for this target.
include bin/CMakeFiles/fmi_cosim.dir/depend.make

# Include the progress variables for this target.
include bin/CMakeFiles/fmi_cosim.dir/progress.make

# Include the compile flags for this target's objects.
include bin/CMakeFiles/fmi_cosim.dir/flags.make

bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o: bin/CMakeFiles/fmi_cosim.dir/flags.make
bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o: ../src/main_cs.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/thiag/priv-git/fmi_test/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/fmi_cosim.dir/main_cs.c.o   -c /Users/thiag/priv-git/fmi_test/src/main_cs.c

bin/CMakeFiles/fmi_cosim.dir/main_cs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fmi_cosim.dir/main_cs.c.i"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/thiag/priv-git/fmi_test/src/main_cs.c > CMakeFiles/fmi_cosim.dir/main_cs.c.i

bin/CMakeFiles/fmi_cosim.dir/main_cs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fmi_cosim.dir/main_cs.c.s"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/thiag/priv-git/fmi_test/src/main_cs.c -o CMakeFiles/fmi_cosim.dir/main_cs.c.s

bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o.requires:
.PHONY : bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o.requires

bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o.provides: bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o.requires
	$(MAKE) -f bin/CMakeFiles/fmi_cosim.dir/build.make bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o.provides.build
.PHONY : bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o.provides

bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o.provides.build: bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o

bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o: bin/CMakeFiles/fmi_cosim.dir/flags.make
bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o: ../src/sim_support.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/thiag/priv-git/fmi_test/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/fmi_cosim.dir/sim_support.c.o   -c /Users/thiag/priv-git/fmi_test/src/sim_support.c

bin/CMakeFiles/fmi_cosim.dir/sim_support.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fmi_cosim.dir/sim_support.c.i"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/thiag/priv-git/fmi_test/src/sim_support.c > CMakeFiles/fmi_cosim.dir/sim_support.c.i

bin/CMakeFiles/fmi_cosim.dir/sim_support.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fmi_cosim.dir/sim_support.c.s"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/thiag/priv-git/fmi_test/src/sim_support.c -o CMakeFiles/fmi_cosim.dir/sim_support.c.s

bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o.requires:
.PHONY : bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o.requires

bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o.provides: bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o.requires
	$(MAKE) -f bin/CMakeFiles/fmi_cosim.dir/build.make bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o.provides.build
.PHONY : bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o.provides

bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o.provides.build: bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o

bin/CMakeFiles/fmi_cosim.dir/stack.c.o: bin/CMakeFiles/fmi_cosim.dir/flags.make
bin/CMakeFiles/fmi_cosim.dir/stack.c.o: ../src/stack.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/thiag/priv-git/fmi_test/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object bin/CMakeFiles/fmi_cosim.dir/stack.c.o"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/fmi_cosim.dir/stack.c.o   -c /Users/thiag/priv-git/fmi_test/src/stack.c

bin/CMakeFiles/fmi_cosim.dir/stack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fmi_cosim.dir/stack.c.i"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/thiag/priv-git/fmi_test/src/stack.c > CMakeFiles/fmi_cosim.dir/stack.c.i

bin/CMakeFiles/fmi_cosim.dir/stack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fmi_cosim.dir/stack.c.s"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/thiag/priv-git/fmi_test/src/stack.c -o CMakeFiles/fmi_cosim.dir/stack.c.s

bin/CMakeFiles/fmi_cosim.dir/stack.c.o.requires:
.PHONY : bin/CMakeFiles/fmi_cosim.dir/stack.c.o.requires

bin/CMakeFiles/fmi_cosim.dir/stack.c.o.provides: bin/CMakeFiles/fmi_cosim.dir/stack.c.o.requires
	$(MAKE) -f bin/CMakeFiles/fmi_cosim.dir/build.make bin/CMakeFiles/fmi_cosim.dir/stack.c.o.provides.build
.PHONY : bin/CMakeFiles/fmi_cosim.dir/stack.c.o.provides

bin/CMakeFiles/fmi_cosim.dir/stack.c.o.provides.build: bin/CMakeFiles/fmi_cosim.dir/stack.c.o

bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o: bin/CMakeFiles/fmi_cosim.dir/flags.make
bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o: ../src/xml_parser.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/thiag/priv-git/fmi_test/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/fmi_cosim.dir/xml_parser.c.o   -c /Users/thiag/priv-git/fmi_test/src/xml_parser.c

bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fmi_cosim.dir/xml_parser.c.i"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/thiag/priv-git/fmi_test/src/xml_parser.c > CMakeFiles/fmi_cosim.dir/xml_parser.c.i

bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fmi_cosim.dir/xml_parser.c.s"
	cd /Users/thiag/priv-git/fmi_test/build/bin && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/thiag/priv-git/fmi_test/src/xml_parser.c -o CMakeFiles/fmi_cosim.dir/xml_parser.c.s

bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o.requires:
.PHONY : bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o.requires

bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o.provides: bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o.requires
	$(MAKE) -f bin/CMakeFiles/fmi_cosim.dir/build.make bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o.provides.build
.PHONY : bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o.provides

bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o.provides.build: bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o

# Object files for target fmi_cosim
fmi_cosim_OBJECTS = \
"CMakeFiles/fmi_cosim.dir/main_cs.c.o" \
"CMakeFiles/fmi_cosim.dir/sim_support.c.o" \
"CMakeFiles/fmi_cosim.dir/stack.c.o" \
"CMakeFiles/fmi_cosim.dir/xml_parser.c.o"

# External object files for target fmi_cosim
fmi_cosim_EXTERNAL_OBJECTS =

../bin/fmi_cosim: bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o
../bin/fmi_cosim: bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o
../bin/fmi_cosim: bin/CMakeFiles/fmi_cosim.dir/stack.c.o
../bin/fmi_cosim: bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o
../bin/fmi_cosim: bin/CMakeFiles/fmi_cosim.dir/build.make
../bin/fmi_cosim: bin/CMakeFiles/fmi_cosim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../../bin/fmi_cosim"
	cd /Users/thiag/priv-git/fmi_test/build/bin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fmi_cosim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bin/CMakeFiles/fmi_cosim.dir/build: ../bin/fmi_cosim
.PHONY : bin/CMakeFiles/fmi_cosim.dir/build

bin/CMakeFiles/fmi_cosim.dir/requires: bin/CMakeFiles/fmi_cosim.dir/main_cs.c.o.requires
bin/CMakeFiles/fmi_cosim.dir/requires: bin/CMakeFiles/fmi_cosim.dir/sim_support.c.o.requires
bin/CMakeFiles/fmi_cosim.dir/requires: bin/CMakeFiles/fmi_cosim.dir/stack.c.o.requires
bin/CMakeFiles/fmi_cosim.dir/requires: bin/CMakeFiles/fmi_cosim.dir/xml_parser.c.o.requires
.PHONY : bin/CMakeFiles/fmi_cosim.dir/requires

bin/CMakeFiles/fmi_cosim.dir/clean:
	cd /Users/thiag/priv-git/fmi_test/build/bin && $(CMAKE_COMMAND) -P CMakeFiles/fmi_cosim.dir/cmake_clean.cmake
.PHONY : bin/CMakeFiles/fmi_cosim.dir/clean

bin/CMakeFiles/fmi_cosim.dir/depend:
	cd /Users/thiag/priv-git/fmi_test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/thiag/priv-git/fmi_test /Users/thiag/priv-git/fmi_test/src /Users/thiag/priv-git/fmi_test/build /Users/thiag/priv-git/fmi_test/build/bin /Users/thiag/priv-git/fmi_test/build/bin/CMakeFiles/fmi_cosim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bin/CMakeFiles/fmi_cosim.dir/depend
