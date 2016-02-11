cmake_minimum_required(VERSION 2.8)
project(bodyangles)

find_package(qibuild)

set(_srcs
    bodyangles.cpp
    bodyangles.h
    main.cpp)

# Create a executable named bodyangles
# with the source file: main.cpp
qi_create_lib(bodyangles SHARED ${_srcs} SUBFOLDER naoqi)
#qi_create_bin(bodyangles ${_srcs})

qi_use_lib(bodyangles QI ALCOMMON ALERROR ALVALUE BOOST)
