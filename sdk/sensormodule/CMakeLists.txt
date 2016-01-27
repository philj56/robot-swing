cmake_minimum_required(VERSION 2.8)
project(sensormodule)

find_package(qibuild)

#List of source files
set(_srcs
    testmodule.cpp
    testmodule.h
    main.cpp)

# Create a executable named sensormodule
# with the source file: main.cpp
qi_create_bin(sensormodule ${_srcs})

# Add a simple test:
qi_create_test(test_sensormodule "test.cpp")

#Needed to find SDK header files
qi_use_lib(sensormodule ALCOMMON ALPROXIES)
