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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml

# Include any dependencies generated for this target.
include CMakeFiles/machinelearning.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/machinelearning.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/machinelearning.dir/flags.make

CMakeFiles/machinelearning.dir/Main.cpp.o: CMakeFiles/machinelearning.dir/flags.make
CMakeFiles/machinelearning.dir/Main.cpp.o: ../Main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/machinelearning.dir/Main.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/machinelearning.dir/Main.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/Main.cpp

CMakeFiles/machinelearning.dir/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/machinelearning.dir/Main.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/Main.cpp > CMakeFiles/machinelearning.dir/Main.cpp.i

CMakeFiles/machinelearning.dir/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/machinelearning.dir/Main.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/Main.cpp -o CMakeFiles/machinelearning.dir/Main.cpp.s

CMakeFiles/machinelearning.dir/Main.cpp.o.requires:
.PHONY : CMakeFiles/machinelearning.dir/Main.cpp.o.requires

CMakeFiles/machinelearning.dir/Main.cpp.o.provides: CMakeFiles/machinelearning.dir/Main.cpp.o.requires
	$(MAKE) -f CMakeFiles/machinelearning.dir/build.make CMakeFiles/machinelearning.dir/Main.cpp.o.provides.build
.PHONY : CMakeFiles/machinelearning.dir/Main.cpp.o.provides

CMakeFiles/machinelearning.dir/Main.cpp.o.provides.build: CMakeFiles/machinelearning.dir/Main.cpp.o

CMakeFiles/machinelearning.dir/CreateModule.cpp.o: CMakeFiles/machinelearning.dir/flags.make
CMakeFiles/machinelearning.dir/CreateModule.cpp.o: ../CreateModule.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/machinelearning.dir/CreateModule.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/machinelearning.dir/CreateModule.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/CreateModule.cpp

CMakeFiles/machinelearning.dir/CreateModule.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/machinelearning.dir/CreateModule.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/CreateModule.cpp > CMakeFiles/machinelearning.dir/CreateModule.cpp.i

CMakeFiles/machinelearning.dir/CreateModule.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/machinelearning.dir/CreateModule.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/CreateModule.cpp -o CMakeFiles/machinelearning.dir/CreateModule.cpp.s

CMakeFiles/machinelearning.dir/CreateModule.cpp.o.requires:
.PHONY : CMakeFiles/machinelearning.dir/CreateModule.cpp.o.requires

CMakeFiles/machinelearning.dir/CreateModule.cpp.o.provides: CMakeFiles/machinelearning.dir/CreateModule.cpp.o.requires
	$(MAKE) -f CMakeFiles/machinelearning.dir/build.make CMakeFiles/machinelearning.dir/CreateModule.cpp.o.provides.build
.PHONY : CMakeFiles/machinelearning.dir/CreateModule.cpp.o.provides

CMakeFiles/machinelearning.dir/CreateModule.cpp.o.provides.build: CMakeFiles/machinelearning.dir/CreateModule.cpp.o

CMakeFiles/machinelearning.dir/State.cpp.o: CMakeFiles/machinelearning.dir/flags.make
CMakeFiles/machinelearning.dir/State.cpp.o: ../State.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/machinelearning.dir/State.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/machinelearning.dir/State.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/State.cpp

CMakeFiles/machinelearning.dir/State.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/machinelearning.dir/State.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/State.cpp > CMakeFiles/machinelearning.dir/State.cpp.i

CMakeFiles/machinelearning.dir/State.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/machinelearning.dir/State.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/State.cpp -o CMakeFiles/machinelearning.dir/State.cpp.s

CMakeFiles/machinelearning.dir/State.cpp.o.requires:
.PHONY : CMakeFiles/machinelearning.dir/State.cpp.o.requires

CMakeFiles/machinelearning.dir/State.cpp.o.provides: CMakeFiles/machinelearning.dir/State.cpp.o.requires
	$(MAKE) -f CMakeFiles/machinelearning.dir/build.make CMakeFiles/machinelearning.dir/State.cpp.o.provides.build
.PHONY : CMakeFiles/machinelearning.dir/State.cpp.o.provides

CMakeFiles/machinelearning.dir/State.cpp.o.provides.build: CMakeFiles/machinelearning.dir/State.cpp.o

CMakeFiles/machinelearning.dir/StateSpace.cpp.o: CMakeFiles/machinelearning.dir/flags.make
CMakeFiles/machinelearning.dir/StateSpace.cpp.o: ../StateSpace.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/machinelearning.dir/StateSpace.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/machinelearning.dir/StateSpace.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/StateSpace.cpp

CMakeFiles/machinelearning.dir/StateSpace.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/machinelearning.dir/StateSpace.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/StateSpace.cpp > CMakeFiles/machinelearning.dir/StateSpace.cpp.i

CMakeFiles/machinelearning.dir/StateSpace.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/machinelearning.dir/StateSpace.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/StateSpace.cpp -o CMakeFiles/machinelearning.dir/StateSpace.cpp.s

CMakeFiles/machinelearning.dir/StateSpace.cpp.o.requires:
.PHONY : CMakeFiles/machinelearning.dir/StateSpace.cpp.o.requires

CMakeFiles/machinelearning.dir/StateSpace.cpp.o.provides: CMakeFiles/machinelearning.dir/StateSpace.cpp.o.requires
	$(MAKE) -f CMakeFiles/machinelearning.dir/build.make CMakeFiles/machinelearning.dir/StateSpace.cpp.o.provides.build
.PHONY : CMakeFiles/machinelearning.dir/StateSpace.cpp.o.provides

CMakeFiles/machinelearning.dir/StateSpace.cpp.o.provides.build: CMakeFiles/machinelearning.dir/StateSpace.cpp.o

CMakeFiles/machinelearning.dir/encoder.cpp.o: CMakeFiles/machinelearning.dir/flags.make
CMakeFiles/machinelearning.dir/encoder.cpp.o: ../encoder.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/machinelearning.dir/encoder.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/machinelearning.dir/encoder.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/encoder.cpp

CMakeFiles/machinelearning.dir/encoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/machinelearning.dir/encoder.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/encoder.cpp > CMakeFiles/machinelearning.dir/encoder.cpp.i

CMakeFiles/machinelearning.dir/encoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/machinelearning.dir/encoder.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/encoder.cpp -o CMakeFiles/machinelearning.dir/encoder.cpp.s

CMakeFiles/machinelearning.dir/encoder.cpp.o.requires:
.PHONY : CMakeFiles/machinelearning.dir/encoder.cpp.o.requires

CMakeFiles/machinelearning.dir/encoder.cpp.o.provides: CMakeFiles/machinelearning.dir/encoder.cpp.o.requires
	$(MAKE) -f CMakeFiles/machinelearning.dir/build.make CMakeFiles/machinelearning.dir/encoder.cpp.o.provides.build
.PHONY : CMakeFiles/machinelearning.dir/encoder.cpp.o.provides

CMakeFiles/machinelearning.dir/encoder.cpp.o.provides.build: CMakeFiles/machinelearning.dir/encoder.cpp.o

# Object files for target machinelearning
machinelearning_OBJECTS = \
"CMakeFiles/machinelearning.dir/Main.cpp.o" \
"CMakeFiles/machinelearning.dir/CreateModule.cpp.o" \
"CMakeFiles/machinelearning.dir/State.cpp.o" \
"CMakeFiles/machinelearning.dir/StateSpace.cpp.o" \
"CMakeFiles/machinelearning.dir/encoder.cpp.o"

# External object files for target machinelearning
machinelearning_EXTERNAL_OBJECTS = \
"/gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/libpmd1208fs.o"

sdk/bin/machinelearning: CMakeFiles/machinelearning.dir/Main.cpp.o
sdk/bin/machinelearning: CMakeFiles/machinelearning.dir/CreateModule.cpp.o
sdk/bin/machinelearning: CMakeFiles/machinelearning.dir/State.cpp.o
sdk/bin/machinelearning: CMakeFiles/machinelearning.dir/StateSpace.cpp.o
sdk/bin/machinelearning: CMakeFiles/machinelearning.dir/encoder.cpp.o
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libalproxies.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libalsoap.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/librttools.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libalthread.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libboost_signals-mt.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libboost_program_options-mt.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libalvalue.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libtinyxml.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/librt.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libqi.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libalerror.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libboost_filesystem-mt.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libboost_thread-mt.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libboost_system-mt.so
sdk/bin/machinelearning: /gs/cross/sysroot/usr/lib/libdl.so
sdk/bin/machinelearning: CMakeFiles/machinelearning.dir/build.make
sdk/bin/machinelearning: ../libpmd1208fs.o
sdk/bin/machinelearning: CMakeFiles/machinelearning.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable sdk/bin/machinelearning"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/machinelearning.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/machinelearning.dir/build: sdk/bin/machinelearning
.PHONY : CMakeFiles/machinelearning.dir/build

# Object files for target machinelearning
machinelearning_OBJECTS = \
"CMakeFiles/machinelearning.dir/Main.cpp.o" \
"CMakeFiles/machinelearning.dir/CreateModule.cpp.o" \
"CMakeFiles/machinelearning.dir/State.cpp.o" \
"CMakeFiles/machinelearning.dir/StateSpace.cpp.o" \
"CMakeFiles/machinelearning.dir/encoder.cpp.o"

# External object files for target machinelearning
machinelearning_EXTERNAL_OBJECTS = \
"/gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/libpmd1208fs.o"

CMakeFiles/CMakeRelink.dir/machinelearning: CMakeFiles/machinelearning.dir/Main.cpp.o
CMakeFiles/CMakeRelink.dir/machinelearning: CMakeFiles/machinelearning.dir/CreateModule.cpp.o
CMakeFiles/CMakeRelink.dir/machinelearning: CMakeFiles/machinelearning.dir/State.cpp.o
CMakeFiles/CMakeRelink.dir/machinelearning: CMakeFiles/machinelearning.dir/StateSpace.cpp.o
CMakeFiles/CMakeRelink.dir/machinelearning: CMakeFiles/machinelearning.dir/encoder.cpp.o
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libalproxies.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libalsoap.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/librttools.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libalthread.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libboost_signals-mt.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libboost_program_options-mt.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libalvalue.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libtinyxml.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/librt.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libqi.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libalerror.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libboost_filesystem-mt.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libboost_thread-mt.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libboost_system-mt.so
CMakeFiles/CMakeRelink.dir/machinelearning: /gs/cross/sysroot/usr/lib/libdl.so
CMakeFiles/CMakeRelink.dir/machinelearning: CMakeFiles/machinelearning.dir/build.make
CMakeFiles/CMakeRelink.dir/machinelearning: ../libpmd1208fs.o
CMakeFiles/CMakeRelink.dir/machinelearning: CMakeFiles/machinelearning.dir/relink.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable CMakeFiles/CMakeRelink.dir/machinelearning"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/machinelearning.dir/relink.txt --verbose=$(VERBOSE)

# Rule to relink during preinstall.
CMakeFiles/machinelearning.dir/preinstall: CMakeFiles/CMakeRelink.dir/machinelearning
.PHONY : CMakeFiles/machinelearning.dir/preinstall

CMakeFiles/machinelearning.dir/requires: CMakeFiles/machinelearning.dir/Main.cpp.o.requires
CMakeFiles/machinelearning.dir/requires: CMakeFiles/machinelearning.dir/CreateModule.cpp.o.requires
CMakeFiles/machinelearning.dir/requires: CMakeFiles/machinelearning.dir/State.cpp.o.requires
CMakeFiles/machinelearning.dir/requires: CMakeFiles/machinelearning.dir/StateSpace.cpp.o.requires
CMakeFiles/machinelearning.dir/requires: CMakeFiles/machinelearning.dir/encoder.cpp.o.requires
.PHONY : CMakeFiles/machinelearning.dir/requires

CMakeFiles/machinelearning.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/machinelearning.dir/cmake_clean.cmake
.PHONY : CMakeFiles/machinelearning.dir/clean

CMakeFiles/machinelearning.dir/depend:
	cd /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml /gs/naoqi/doc/examples/new/robot-swing/sdk/machinelearning/build-ml/CMakeFiles/machinelearning.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/machinelearning.dir/depend

