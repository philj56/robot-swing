cmake_minimum_required(VERSION 2.8)
project(cameratools)

find_package(qibuild)

set(_srcs
    cameratools.cpp
    cameratools.h
    main.cpp)

# Create a executable named cameratools
# with the source file: main.cpp
qi_create_lib(cameratools SHARED ${_srcs} SUBFOLDER naoqi)
#qi_create_bin(cameratools ${_srcs})

qi_use_lib(cameratools QI ALCOMMON ALERROR ALVALUE BOOST)
