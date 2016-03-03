cmake_minimum_required(VERSION 2.8)
project(robotencoder)

find_package(qibuild)

#qi_stage_lib(lpmd1208fs)

#include_directories(/gs/naoqi/doc/examples/new/robot-swing/sdk/robotencoder)

# Create a executable named robotencoder
# with the source file: main.cpp
qi_create_bin(robotencoder "main.cpp")

# Add a simple test:
enable_testing()
qi_create_test(test_robotencoder "test.cpp")

#qi_use_lib(robotencoder libpmd1208fs.so.2.2)

target_link_libraries(robotencoder lpmd1208fs)

