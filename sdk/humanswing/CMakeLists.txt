cmake_minimum_required(VERSION 2.8)
project(humanswing)

find_package(qibuild)

set(_srcs
    main.cpp
    encoder.cpp
    createmodule.cpp)

# Create an executable named localcall
qi_create_bin(humanswing ${_srcs})

qi_use_lib(humanswing QI ALCOMMON ALERROR ALVALUE BOOST)
