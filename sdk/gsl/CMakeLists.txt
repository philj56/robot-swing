cmake_minimum_required(VERSION 2.8)
project(gsltest)

find_package(qibuild)

include_directories(GSL/include)

set(_srcs
    main.cpp)

# Create an executable named gsltest
qi_create_bin(gsltest ${_srcs})

# Add gsl static libraries
target_link_libraries(gsltest ${CMAKE_SOURCE_DIR}/GSL/lib/libgsl.a ${CMAKE_SOURCE_DIR}/GSL/lib/libgslcblas.a)

qi_use_lib(gsltest QI ALCOMMON ALERROR ALVALUE BOOST)

