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
CMAKE_SOURCE_DIR = /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/build-ml

# Include any dependencies generated for this target.
include CMakeFiles/landmarkswing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/landmarkswing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/landmarkswing.dir/flags.make

CMakeFiles/landmarkswing.dir/main.cpp.o: CMakeFiles/landmarkswing.dir/flags.make
CMakeFiles/landmarkswing.dir/main.cpp.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/build-ml/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/landmarkswing.dir/main.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/landmarkswing.dir/main.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/main.cpp

CMakeFiles/landmarkswing.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/landmarkswing.dir/main.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/main.cpp > CMakeFiles/landmarkswing.dir/main.cpp.i

CMakeFiles/landmarkswing.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/landmarkswing.dir/main.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/main.cpp -o CMakeFiles/landmarkswing.dir/main.cpp.s

CMakeFiles/landmarkswing.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/landmarkswing.dir/main.cpp.o.requires

CMakeFiles/landmarkswing.dir/main.cpp.o.provides: CMakeFiles/landmarkswing.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/landmarkswing.dir/build.make CMakeFiles/landmarkswing.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/landmarkswing.dir/main.cpp.o.provides

CMakeFiles/landmarkswing.dir/main.cpp.o.provides.build: CMakeFiles/landmarkswing.dir/main.cpp.o

CMakeFiles/landmarkswing.dir/createmodule.cpp.o: CMakeFiles/landmarkswing.dir/flags.make
CMakeFiles/landmarkswing.dir/createmodule.cpp.o: ../createmodule.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/build-ml/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/landmarkswing.dir/createmodule.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/landmarkswing.dir/createmodule.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/createmodule.cpp

CMakeFiles/landmarkswing.dir/createmodule.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/landmarkswing.dir/createmodule.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/createmodule.cpp > CMakeFiles/landmarkswing.dir/createmodule.cpp.i

CMakeFiles/landmarkswing.dir/createmodule.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/landmarkswing.dir/createmodule.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/createmodule.cpp -o CMakeFiles/landmarkswing.dir/createmodule.cpp.s

CMakeFiles/landmarkswing.dir/createmodule.cpp.o.requires:
.PHONY : CMakeFiles/landmarkswing.dir/createmodule.cpp.o.requires

CMakeFiles/landmarkswing.dir/createmodule.cpp.o.provides: CMakeFiles/landmarkswing.dir/createmodule.cpp.o.requires
	$(MAKE) -f CMakeFiles/landmarkswing.dir/build.make CMakeFiles/landmarkswing.dir/createmodule.cpp.o.provides.build
.PHONY : CMakeFiles/landmarkswing.dir/createmodule.cpp.o.provides

CMakeFiles/landmarkswing.dir/createmodule.cpp.o.provides.build: CMakeFiles/landmarkswing.dir/createmodule.cpp.o

# Object files for target landmarkswing
landmarkswing_OBJECTS = \
"CMakeFiles/landmarkswing.dir/main.cpp.o" \
"CMakeFiles/landmarkswing.dir/createmodule.cpp.o"

# External object files for target landmarkswing
landmarkswing_EXTERNAL_OBJECTS =

sdk/bin/landmarkswing: CMakeFiles/landmarkswing.dir/main.cpp.o
sdk/bin/landmarkswing: CMakeFiles/landmarkswing.dir/createmodule.cpp.o
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libalproxies.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libalsoap.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/librttools.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libalthread.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_signals-mt.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_program_options-mt.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libalvalue.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libtinyxml.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/librt.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libqi.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_filesystem-mt.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_thread-mt.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_system-mt.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libdl.so
sdk/bin/landmarkswing: /gs/cross/sysroot/usr/lib/libalerror.so
sdk/bin/landmarkswing: CMakeFiles/landmarkswing.dir/build.make
sdk/bin/landmarkswing: CMakeFiles/landmarkswing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable sdk/bin/landmarkswing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/landmarkswing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/landmarkswing.dir/build: sdk/bin/landmarkswing
.PHONY : CMakeFiles/landmarkswing.dir/build

# Object files for target landmarkswing
landmarkswing_OBJECTS = \
"CMakeFiles/landmarkswing.dir/main.cpp.o" \
"CMakeFiles/landmarkswing.dir/createmodule.cpp.o"

# External object files for target landmarkswing
landmarkswing_EXTERNAL_OBJECTS =

CMakeFiles/CMakeRelink.dir/landmarkswing: CMakeFiles/landmarkswing.dir/main.cpp.o
CMakeFiles/CMakeRelink.dir/landmarkswing: CMakeFiles/landmarkswing.dir/createmodule.cpp.o
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libalproxies.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libalsoap.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/librttools.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libalthread.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_signals-mt.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_program_options-mt.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libalvalue.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libtinyxml.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/librt.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libqi.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_filesystem-mt.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_thread-mt.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libboost_system-mt.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libdl.so
CMakeFiles/CMakeRelink.dir/landmarkswing: /gs/cross/sysroot/usr/lib/libalerror.so
CMakeFiles/CMakeRelink.dir/landmarkswing: CMakeFiles/landmarkswing.dir/build.make
CMakeFiles/CMakeRelink.dir/landmarkswing: CMakeFiles/landmarkswing.dir/relink.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable CMakeFiles/CMakeRelink.dir/landmarkswing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/landmarkswing.dir/relink.txt --verbose=$(VERBOSE)

# Rule to relink during preinstall.
CMakeFiles/landmarkswing.dir/preinstall: CMakeFiles/CMakeRelink.dir/landmarkswing
.PHONY : CMakeFiles/landmarkswing.dir/preinstall

CMakeFiles/landmarkswing.dir/requires: CMakeFiles/landmarkswing.dir/main.cpp.o.requires
CMakeFiles/landmarkswing.dir/requires: CMakeFiles/landmarkswing.dir/createmodule.cpp.o.requires
.PHONY : CMakeFiles/landmarkswing.dir/requires

CMakeFiles/landmarkswing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/landmarkswing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/landmarkswing.dir/clean

CMakeFiles/landmarkswing.dir/depend:
	cd /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/build-ml && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/build-ml /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/build-ml /gs/naoqi/doc/examples/new/robot-swing/sdk/gyroswing/build-ml/CMakeFiles/landmarkswing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/landmarkswing.dir/depend

