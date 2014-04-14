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

# Add files for the server.
set (server_SOURCES
  dao/bus_dao.cc
  dao/bus_service_dao.cc
  dao/bus_stop_dao.cc
  dao/dao_manager.cc
  request/request.cc
  request/buses_request.cc
  request/bus_stops_request.cc
  request/invalid_request.cc
  client_handler.cc
  request_router.cc
  server_context.cc
  server.cc
  bustrackd.cc
)

# Tell CMake to run MOC when necessary.
set (CMAKE_AUTOMOC ON)

# As MOC files are generated in the binary directory, tell CMake to always
# look for includes here.
set (CMAKE_INCLUDE_CURRENT_DIR ON)

# Include Qt5 dependencies.
find_package (Qt5Core REQUIRED)
find_package (Qt5Network REQUIRED)

# Create an executable called "bustrackd" which includes the source files below.
# The extension of the library is already known by CMake. Any number of sources
# could be listed here.
add_executable (bustrackd ${server_SOURCES})

# Link the BusTrack daemon to the common BusTrack library.
target_link_libraries (bustrackd Qt5::Core Qt5::Network bustrack)

# vim: set ts=2 sw=2 et:
