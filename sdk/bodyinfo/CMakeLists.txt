cmake_minimum_required(VERSION 2.8)
project(bodyinfo)

find_package(qibuild)

set(_srcs
    bodyinfo.cpp
    bodyinfo.h
    main.cpp)

# Create a executable named bodyinfo
# with the source file: main.cpp
qi_create_lib(bodyinfo SHARED ${_srcs} SUBFOLDER naoqi)
#qi_create_bin(bodyinfo ${_srcs})

qi_use_lib(bodyinfo QI ALCOMMON ALERROR ALVALUE BOOST)
