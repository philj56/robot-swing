cmake_minimum_required(VERSION 2.8)
project(localcall)

find_package(qibuild)

set(_srcs
    main.cpp)

# Create an executable named localcall
qi_create_bin(localcall ${_srcs})

qi_use_lib(localcall QI ALCOMMON ALERROR ALVALUE BOOST)
