cmake_minimum_required(VERSION 2.8)
project(motiontiming)

find_package(qibuild)

set(_srcs
    main.cpp)

# Create an executable named motiontiming
qi_create_bin(motiontiming ${_srcs})

qi_use_lib(motiontiming QI ALCOMMON ALERROR ALVALUE BOOST)
