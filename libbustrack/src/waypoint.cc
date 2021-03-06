/* ========================================================================= 
 * BusTrack Bus Tracking System
 * CS3249 User Interface Development
 * =========================================================================
 * Copyright 2014 Ko Wan Ling, Terence Then, Kek Yan Rong and Wong Yong Jie
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================================= */

#include "bustrack/waypoint.h"

namespace bustrack {

  const std::string Waypoint::TYPE_NORMAL ("NORMAL");
  const std::string Waypoint::TYPE_BUS_STOP ("BUS_STOP");

  Waypoint::Waypoint() : type_(TYPE_NORMAL) {
  }

  Waypoint::~Waypoint() {
  }

  std::string Waypoint::getType() {
    return type_;
  }

  void Waypoint::setType(const std::string& type) {
    type_ = type;
  }

  float Waypoint::getLatitude() {
    return latitude_;
  }

  void Waypoint::setLatitude(float latitude) {
    latitude_ = latitude;
  }

  float Waypoint::getLongitude() {
    return longitude_;
  }

  void Waypoint::setLongitude(float longitude) {
    longitude_ = longitude;
  }

}

/* vim: set ts=2 sw=2 et: */
