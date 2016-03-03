cmake_minimum_required(VERSION 2.8)
project(machinelearning)

find_package(qibuild)

# Create a executable named machinelearning
# with the source file: main.cpp
qi_create_bin(machinelearning "main.cpp")

# Add a simple test:
enable_testing()
qi_create_test(test_machinelearning "test.cpp")
