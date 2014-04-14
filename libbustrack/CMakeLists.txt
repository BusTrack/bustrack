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

# Create a library called "bustrack" which includes the source files below.
# The extension of the library is already known by CMake. Any number of sources
# could be listed here.
add_library (bustrack
  src/bus.cc
  src/bus_route.cc
  src/bus_service.cc
  src/bus_stop.cc
  src/coords.cc
  src/waypoint.cc
  src/message.cc
)

# Add definitions for Qt4.
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

# vim: set ts=2 sw=2 et:
