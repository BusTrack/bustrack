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

# Add files for the client.
set (client_SOURCES
  bustrackwindow.cpp
  bustrackservice.cpp
  mapview.cpp
  main.cpp
)

# Tell CMake to run MOC when necessary.
set (CMAKE_AUTOMOC ON)

# As MOC files are generated in the binary directory, tell CMake to always
# look for includes here.
set (CMAKE_INCLUDE_CURRENT_DIR ON)

# Include Qt5 dependencies.
find_package (Qt5Widgets REQUIRED)
find_package (Qt5Network REQUIRED)

# Wrap the UI files.
qt5_wrap_ui (client_FORMS
  bustrackwindow.ui
)

# Add the executable and link the libraries.
add_executable (client ${client_SOURCES} ${client_FORMS})

# Link the BusTrack daemon to the common BusTrack library.
target_link_libraries (client Qt5::Widgets Qt5::Network bustrack)

# vim: set ts=2 sw=2 et:
