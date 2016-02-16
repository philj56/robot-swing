cmake_minimum_required(VERSION 2.8)
project(motionderivative)

find_package(qibuild)

set(_srcs
    main.cpp)

# Create an executable named localcall
qi_create_bin(motionderivative ${_srcs})

qi_use_lib(motionderivative QI ALCOMMON ALERROR ALVALUE BOOST)
