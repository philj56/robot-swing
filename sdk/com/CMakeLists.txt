cmake_minimum_required(VERSION 2.8)
project(com)

find_package(qibuild)

# Create a executable named com
# with the source file: main.cpp
qi_create_bin(com "main.cpp")

qi_use_lib(com ALCOMMON ALPROXIES)


# Add a simple test:
qi_create_test(test_com "test.cpp")
