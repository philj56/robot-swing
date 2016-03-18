cmake_minimum_required(VERSION 2.8)
project(landmarkswing)

find_package(qibuild)

set(_srcs
    main.cpp
    createmodule.cpp) 

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=gnu++98 -g -O3 -Wall -Wextra -pedantic")

# Create an executable named localcall
qi_create_bin(landmarkswing ${_srcs})

qi_use_lib(landmarkswing QI ALCOMMON ALERROR ALVALUE BOOST)
