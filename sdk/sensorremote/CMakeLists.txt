cmake_minimum_required(VERSION 2.8)
project(sensorremote)

find_package(qibuild)

# Create a executable named sensorremote
# with the source file: main.cpp
qi_create_bin(sensorremote "main.cpp")

# Add a simple test:
qi_create_test(test_sensorremote "test.cpp")

qi_use_lib(sensorremote ALCOMMON ALPROXIES)
