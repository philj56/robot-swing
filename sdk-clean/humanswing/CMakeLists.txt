cmake_minimum_required(VERSION 2.8)
project(humanswing)

find_package(qibuild)

set(_srcs
    main.cpp
    encoder.cpp
    createmodule.cpp
    libpmd1208fs.o)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=gnu++98 -g" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lusb-1.0 -L/lib/i386-linux-gnu/" )

# Create an executable named localcall
qi_create_bin(humanswing ${_srcs})

qi_use_lib(humanswing QI ALCOMMON ALERROR ALVALUE BOOST)
