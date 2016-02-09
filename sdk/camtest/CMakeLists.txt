cmake_minimum_required(VERSION 2.8)
project(camtest)

find_package(qibuild)

set(_srcs
    camtest.cpp
    camtest.h
    main.cpp)

# Create a executable named camtest
# with the source file: main.cpp
qi_create_lib(camtest SHARED ${_srcs} SUBFOLDER naoqi)
#qi_create_bin(camtest ${_srcs})

qi_use_lib(camtest QI ALCOMMON ALERROR ALVALUE BOOST)
