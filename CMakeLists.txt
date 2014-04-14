# ============================================================================ #
#  BusTrack Bus Tracking System                                                #
#  CS3249 User Interface Development                                           #
# ============================================================================ #
#  Copyright 2014 Ko Wan Ling, Terence Then, Kek Yan Rong and Wong Yong Jie    #
#                                                                              #
#  Licensed under the Apache License, Version 2.0 (the "License");             #
#  you may not use this file except in compliance with the License.            #
#  You may obtain a copy of the License at                                     #
#                                                                              #
#  http://www.apache.org/licenses/LICENSE-2.0                                  #
#                                                                              #
#  Unless required by applicable law or agreed to in writing, software         #
#  distributed under the License is distributed on an "AS IS" BASIS,           #
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    #
#  See the License for the specific language governing permissions and         #
#  limitations under the License.                                              #
# ============================================================================ #

# The name of our project is "bustrack". CMakeLists files in this project can
# refer to the root source directory of the project as ${bustrack_SOURCE_DIR}
# and to the root binary directory of the project as ${bustrack_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (bustrack)

# Set CMake's module path.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Require C++11 support to build the project.
include (cmake/cmake-cxx11/Modules/CheckCXX11Features.cmake)
if (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_COMPILER_FLAGS}")
endif (CMAKE_COMPILER_IS_GNUCXX)

# Add the bustrack library headers to the global include path.
include_directories (${CMAKE_SOURCE_DIR}/libbustrack/include)

# Recurse into the "libbustrack" subdirectory. This does not actually cause
# another cmake executable to run. The same process will walk through the
# project's entire directory structure.
add_subdirectory (libbustrack)
add_subdirectory (client)
add_subdirectory (server)

# vim: set ts=2 sw=2 et:
