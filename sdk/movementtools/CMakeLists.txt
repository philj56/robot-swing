cmake_minimum_required(VERSION 2.8)
project(movementtools)

find_package(qibuild)

set(_srcs
    movementtools.cpp
    movementtools.h
    main.cpp)

# Create a executable named movementtools
# with the source file: main.cpp
qi_create_lib(movementtools SHARED ${_srcs} SUBFOLDER naoqi)
#qi_create_bin(movementtools ${_srcs})

qi_use_lib(movementtools QI ALCOMMON ALERROR ALVALUE BOOST)
