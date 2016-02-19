cmake_minimum_required(VERSION 2.8)
project(landmarkswing)

find_package(qibuild)

set(_srcs
    main.cpp)

# Create an executable named localcall
qi_create_bin(landmarkswing ${_srcs})

qi_use_lib(landmarkswing QI ALCOMMON ALERROR ALVALUE BOOST)
