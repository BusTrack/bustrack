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
#include <stdexcept>

#include <QString>
#include <QStringList>
#include <QDebug>

#include "bustrack/bus.h"

namespace bustrack {
  
  const std::string Bus::TAG ("Bus");

  Bus::Bus() :
      id_("Unknown ID"), destination_(0), occupancy_(0), longitude_(0),
      latitude_(0), active_(false) {
  }

  std::string Bus::getId() {
    return id_;
  }

  void Bus::setId(const std::string& id) {
    id_ = id;
  }

  BusService Bus::getService() {
    return service_;
  }

  void Bus::setService(BusService service) {
    service_ = service;
  }

  int Bus::getDestination() {
    return destination_;
  }

  void Bus::setDestination(int destination) {
    destination_ = destination;
  }
  
  unsigned int Bus::getOccupancy() {
    return occupancy_;
  }
  
  void Bus::setOccupancy(unsigned int occupancy) {
    occupancy_ = occupancy;
  }
  
  float Bus::getLatitude() {
    return latitude_;
  }
  
  void Bus::setLatitude(float latitude) {
    latitude_ = latitude;
  }
  
  float Bus::getLongitude() {
    return longitude_;
  }
  
  void Bus::setLongitude(float longitude) {
    longitude_ = longitude;
  }

  bool Bus::isActive() {
    return active_;
  }

  void Bus::setIsActive(bool active) {
    active_ = active;
  }

  Bus Bus::fromString(const std::string& serialized) {
    QString q_serialized (serialized.c_str());
    QStringList tokens = q_serialized.split("|");

    if (tokens.size() < NUM_SERIALIZED_FIELDS) {
      qWarning("%s: Insufficient number of fields! (%s)", TAG.c_str(),
          serialized.c_str());
      return Bus();
    }

    Bus bus;
    bus.setId(tokens[0].toStdString());

    return bus;
  }
  
  Bus Bus::fromStringAll(const std::string& serialized) {
    QString q_serialized (serialized.c_str());
    QStringList tokens = q_serialized.split("|");
    Bus bus = fromString(serialized);
    
    if (tokens.size() < NUM_ALL_FIELDS) {
      qWarning("%s: Insufficient number of all fields! (%s)", TAG.c_str(),
          serialized.c_str());
      return bus;
    }
    
    bus.setOccupancy(tokens[1].toUInt());
    
    // Convert the coordinates.
    bool parsing_ok = false;
    bus.setLatitude(tokens[2].toFloat(&parsing_ok));
    if (!parsing_ok) {
      qWarning("%s: Unable to parse latitude for bus with ID %s", TAG.c_str(),
        bus.getId().c_str());
    }

    bus.setLongitude(tokens[3].toFloat(&parsing_ok));
    if (!parsing_ok) {
      qWarning("%s Unable to parse longitude for bus with ID %s", TAG.c_str(),
        bus.getId().c_str());
    }

    // Get the service code.
    BusService service = bus.getService();
    service.setCode(tokens[4].toStdString());
    bus.setService(service);
    
    return bus;
  }

  std::string Bus::toString() {
    std::stringstream serialized_ss;
    serialized_ss << getId();
    return serialized_ss.str();
  }
  
  std::string Bus::toStringAll() {
    std::stringstream serialized_all_ss (toString(),
        std::stringstream::out | std::stringstream::ate);
    serialized_all_ss << "|" << getOccupancy() << "|" << getLatitude() << "|" <<
      getLongitude() << "|" << getService().getCode();
    return serialized_all_ss.str();
  }

}

/* vim: set ts=2 sw=2 et: */
