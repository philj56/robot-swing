cmake_minimum_required(VERSION 2.8)
project(smoothmove)

find_package(qibuild)

set(_srcs
    smoothmove.cpp
    smoothmove.h
    main.cpp)

# Create a executable named smoothmove
# with the source file: main.cpp
qi_create_lib(smoothmove SHARED ${_srcs} SUBFOLDER naoqi)

qi_use_lib(smoothmove ALCOMMON BOOST)
