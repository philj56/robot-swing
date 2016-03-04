cmake_minimum_required(VERSION 2.8)
project(machinelearning)

find_package(qibuild)

# Create a executable named machinelearning
# with the source file: main.cpp
qi_create_bin(machinelearning "Main.cpp" "CreateModule.cpp" "State.cpp" "StateSpace.cpp" "encoder.cpp" "libpmd1208fs.o")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=gnu++98" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lusb-1.0 -L/lib/i386-linux-gnu/" )


# Add a simple test:
#enable_testing()
#qi_create_test(test_machinelearning "test.cpp")

