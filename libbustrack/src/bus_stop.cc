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

#include <sstream>

#include <QString>
#include <QStringList>
#include <QDebug>

#include "bustrack/bus_stop.h"

namespace bustrack {

  const std::string BusStop::TAG ("BusStop");

  BusStop::BusStop() :
      id_("Unknown ID"), name_("Unknown Name"), occupancy_(0) {
    setType(Waypoint::TYPE_BUS_STOP);
  }

  std::string BusStop::getId() {
    return id_;
  }

  void BusStop::setId(const std::string& id) {
    id_ = id;
  }

  std::string BusStop::getName() {
    return name_;
  }

  void BusStop::setName(const std::string& name) {
    name_ = name;
  }
  
  unsigned int BusStop::getOccupancy() {
    return occupancy_;
  }
  
  void BusStop::setOccupancy(unsigned int occupancy) {
    occupancy_ = occupancy;
  }

  BusStop BusStop::fromString(const std::string& serialized) {
    QString q_serialized (serialized.c_str());
    QStringList tokens = q_serialized.split("|");

    if (tokens.size() < NUM_SERIALIZED_FIELDS) {
      qWarning("%s: Insufficient number of fields! (%s)", TAG.c_str(),
         serialized.c_str());
      return BusStop();
    }

    // Create the bus stop object.
    BusStop bus_stop;
    bus_stop.setId(tokens[0].toStdString());
    bus_stop.setName(tokens[1].toStdString());

    // Convert the coordinates.
    bool parsing_ok = false;
    bus_stop.setLatitude(tokens[2].toFloat(&parsing_ok));
    if (!parsing_ok) {
      qWarning("%s: Unable to parse latitude for bus stop with ID %s",
          TAG.c_str(), bus_stop.getId().c_str());
    }

    bus_stop.setLongitude(tokens[3].toFloat(&parsing_ok));
    if (!parsing_ok) {
      qWarning("%s Unable to parse longitude for bus stop with ID %s",
          TAG.c_str(), bus_stop.getId().c_str());
    }

    return bus_stop;
  }
  
  BusStop BusStop::fromStringAll(const std::string& serialized) {
    QString q_serialized (serialized.c_str());
    QStringList tokens = q_serialized.split("|");
    BusStop bus_stop = fromString(serialized);
    
    if (tokens.size() < NUM_ALL_FIELDS) {
      qWarning("%s: Insufficient number of all fields! (%s)", TAG.c_str(),
          serialized.c_str());
      return bus_stop;
    }
    
    bus_stop.setOccupancy(tokens[4].toUInt());
    return bus_stop;
  }
 
  std::string BusStop::toString() {
    std::stringstream serialized_ss;
    serialized_ss << getId() << "|" << getName() << "|" <<
          getLatitude() << "|" << getLongitude();
    return serialized_ss.str();
  }
  
  std::string BusStop::toStringAll() {
    std::stringstream serialized_all_ss (toString(),
        std::stringstream::out | std::stringstream::ate);
    serialized_all_ss << "|" << getOccupancy();
    return serialized_all_ss.str();
  }

}

/* vim: set ts=2 sw=2 et: */
