cmake_minimum_required(VERSION 2.8)
project(moduletest)

find_package(qibuild)

set(_srcs
    moduletest.cpp
    moduletest.h
    main.cpp)

option(REMOTE
       "Module is compiled as a remote module (ON or OFF)"
       ON)

# Create a executable named moduletest
# with the source file: main.cpp
if(REMOTE)
	add_definitions(" -DMODULETEST_IS_REMOTE ")
	qi_create_bin(moduletest ${_srcs})
else()
	qi_create_lib(moduletest SHARED ${_srcs} SUBFOLDER naoqi)
endif()

qi_use_lib(moduletest ALCOMMON BOOST)
