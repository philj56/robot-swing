
#############################################
#QIBUILD AUTOGENERATED FILE. DO NOT EDIT.
#############################################

# Add path to CMake framework path if necessary:
set(_qibuild_path "/home/harry/.local/share/cmake")
list(FIND CMAKE_MODULE_PATH "${_qibuild_path}" _found)
if(_found STREQUAL "-1")
  # Prefer cmake files matching  current qibuild installation
  # over cmake files in the cross-toolchain
  list(INSERT CMAKE_MODULE_PATH 0 "${_qibuild_path}")
endif()

# Dependencies:
list(FIND CMAKE_PREFIX_PATH "/media/harry/Windows/scratch/sdk/ubuntu/nao" _found)
if(_found STREQUAL "-1")
  list(APPEND CMAKE_PREFIX_PATH "/media/harry/Windows/scratch/sdk/ubuntu/nao")
endif()

# Store CMAKE_MODULE_PATH and CMAKE_PREFIX_PATH in cache:
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} CACHE INTERNAL ""  FORCE)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} CACHE INTERNAL ""  FORCE)


set(QIBUILD_PYTHON_PATH "/home/harry/.local/lib/python2.7/site-packages" CACHE STRING "" FORCE)
