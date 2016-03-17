cmake_minimum_required(VERSION 2.8)
project(subscribegyro)
find_package(qibuild)

option(BUMPER_IS_REMOTE
  "module is compiled as a remote module (ON or OFF)"
  OFF)

set(_srcs
  main.cpp
  subscribegyro.h
  subscribegyro.cpp
  createmodule.h
  createmodule.cpp
)

if(BUMPER_IS_REMOTE)
  add_definitions(" -DBUMPER_IS_REMOTE ")
  qi_create_bin(subscribegyro ${_srcs})
else()
  qi_create_lib(subscribegyro SHARED ${_srcs} SUBFOLDER naoqi)
endif()

qi_use_lib(subscribegyro ALCOMMON)
