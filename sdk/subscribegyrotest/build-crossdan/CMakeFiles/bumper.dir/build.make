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
CMAKE_SOURCE_DIR = /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/build-crossdan

# Include any dependencies generated for this target.
include CMakeFiles/subscribegyro.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/subscribegyro.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/subscribegyro.dir/flags.make

CMakeFiles/subscribegyro.dir/main.cpp.o: CMakeFiles/subscribegyro.dir/flags.make
CMakeFiles/subscribegyro.dir/main.cpp.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/build-crossdan/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/subscribegyro.dir/main.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -fPIC -o CMakeFiles/subscribegyro.dir/main.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/main.cpp

CMakeFiles/subscribegyro.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribegyro.dir/main.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -fPIC -E /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/main.cpp > CMakeFiles/subscribegyro.dir/main.cpp.i

CMakeFiles/subscribegyro.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribegyro.dir/main.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -fPIC -S /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/main.cpp -o CMakeFiles/subscribegyro.dir/main.cpp.s

CMakeFiles/subscribegyro.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/subscribegyro.dir/main.cpp.o.requires

CMakeFiles/subscribegyro.dir/main.cpp.o.provides: CMakeFiles/subscribegyro.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/subscribegyro.dir/build.make CMakeFiles/subscribegyro.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/subscribegyro.dir/main.cpp.o.provides

CMakeFiles/subscribegyro.dir/main.cpp.o.provides.build: CMakeFiles/subscribegyro.dir/main.cpp.o

CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o: CMakeFiles/subscribegyro.dir/flags.make
CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o: ../subscribegyro.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/build-crossdan/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++   $(CXX_DEFINES) $(CXX_FLAGS) -fPIC -o CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o -c /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/subscribegyro.cpp

CMakeFiles/subscribegyro.dir/subscribegyro.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/subscribegyro.dir/subscribegyro.cpp.i"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -fPIC -E /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/subscribegyro.cpp > CMakeFiles/subscribegyro.dir/subscribegyro.cpp.i

CMakeFiles/subscribegyro.dir/subscribegyro.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/subscribegyro.dir/subscribegyro.cpp.s"
	/gs/cross/cross/bin/i686-aldebaran-linux-gnu-g++  $(CXX_DEFINES) $(CXX_FLAGS) -fPIC -S /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/subscribegyro.cpp -o CMakeFiles/subscribegyro.dir/subscribegyro.cpp.s

CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o.requires:
.PHONY : CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o.requires

CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o.provides: CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o.requires
	$(MAKE) -f CMakeFiles/subscribegyro.dir/build.make CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o.provides.build
.PHONY : CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o.provides

CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o.provides.build: CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o

# Object files for target subscribegyro
subscribegyro_OBJECTS = \
"CMakeFiles/subscribegyro.dir/main.cpp.o" \
"CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o"

# External object files for target subscribegyro
subscribegyro_EXTERNAL_OBJECTS =

sdk/lib/naoqi/libsubscribegyro.so: CMakeFiles/subscribegyro.dir/main.cpp.o
sdk/lib/naoqi/libsubscribegyro.so: CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalproxies.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalsoap.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/librttools.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalthread.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_signals-mt.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_program_options-mt.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalvalue.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libtinyxml.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/librt.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libqi.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalerror.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_filesystem-mt.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_thread-mt.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_system-mt.so
sdk/lib/naoqi/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libdl.so
sdk/lib/naoqi/libsubscribegyro.so: CMakeFiles/subscribegyro.dir/build.make
sdk/lib/naoqi/libsubscribegyro.so: CMakeFiles/subscribegyro.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library sdk/lib/naoqi/libsubscribegyro.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/subscribegyro.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/subscribegyro.dir/build: sdk/lib/naoqi/libsubscribegyro.so
.PHONY : CMakeFiles/subscribegyro.dir/build

# Object files for target subscribegyro
subscribegyro_OBJECTS = \
"CMakeFiles/subscribegyro.dir/main.cpp.o" \
"CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o"

# External object files for target subscribegyro
subscribegyro_EXTERNAL_OBJECTS =

CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: CMakeFiles/subscribegyro.dir/main.cpp.o
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalproxies.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalsoap.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/librttools.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalthread.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_signals-mt.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_program_options-mt.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalvalue.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libtinyxml.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/librt.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libqi.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libalerror.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_filesystem-mt.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_thread-mt.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libboost_system-mt.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: /gs/cross/sysroot/usr/lib/libdl.so
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: CMakeFiles/subscribegyro.dir/build.make
CMakeFiles/CMakeRelink.dir/libsubscribegyro.so: CMakeFiles/subscribegyro.dir/relink.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library CMakeFiles/CMakeRelink.dir/libsubscribegyro.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/subscribegyro.dir/relink.txt --verbose=$(VERBOSE)

# Rule to relink during preinstall.
CMakeFiles/subscribegyro.dir/preinstall: CMakeFiles/CMakeRelink.dir/libsubscribegyro.so
.PHONY : CMakeFiles/subscribegyro.dir/preinstall

CMakeFiles/subscribegyro.dir/requires: CMakeFiles/subscribegyro.dir/main.cpp.o.requires
CMakeFiles/subscribegyro.dir/requires: CMakeFiles/subscribegyro.dir/subscribegyro.cpp.o.requires
.PHONY : CMakeFiles/subscribegyro.dir/requires

CMakeFiles/subscribegyro.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/subscribegyro.dir/cmake_clean.cmake
.PHONY : CMakeFiles/subscribegyro.dir/clean

CMakeFiles/subscribegyro.dir/depend:
	cd /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/build-crossdan && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/build-crossdan /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/build-crossdan /gs/naoqi/doc/examples/new/robot-swing/sdk/subscribegyro/build-crossdan/CMakeFiles/subscribegyro.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/subscribegyro.dir/depend

