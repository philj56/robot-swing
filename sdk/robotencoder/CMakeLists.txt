cmake_minimum_required(VERSION 1.8)
project(robotencoder)

find_package(qibuild)

#qi_stage_lib(lpmd1208fs)

#include_directories(/gs/naoqi/doc/examples/new/robot-swing/sdk/robotencoder)

# Create a executable named robotencoder
# with the source file: main.cpp
qi_create_bin(robotencoder "main.cpp" "encoder.cpp"  "libpmd1208fs.o")

# Add a simple test:
enable_testing()
qi_create_test(test_robotencoder "test.cpp")

#qi_use_lib(robotencoder libpmd1208fs.so.2.2
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lusb-1.0 -L/lib/i386-linux-gnu/" )
#target_link_libraries(robotencoder lpmd1208fs)

