cmake_minimum_required(VERSION 2.8)
project(gyroevents)
find_package(qibuild)

option(EVENTS_IS_REMOTE
  "module is compiled as a remote module (ON or OFF)"
  OFF)

set(_srcs
  main.cpp
  events.h
  events.cpp
)


if(EVENTS_IS_REMOTE)
  add_definitions(" -DEVENTS_IS_REMOTE")
  qi_create_bin(gyroevents ${_srcs})
else()
  qi_create_lib(gyroevents SHARED ${_srcs} SUBFOLDER naoqi)
endif()


qi_use_lib(gyroevents ALCOMMON)
