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

#include "bustrack/bus_service.h"

namespace bustrack {

  const std::string BusService::TAG ("BusService");

  std::string BusService::getCode() {
    return code_;
  }

  void BusService::setCode(std::string code) {
    code_ = code;
  }

  std::vector<Waypoint> BusService::getRoute() {
    return route_;
  }

  void BusService::setRoute(std::vector<Waypoint> route) {
    route_ = route;
  }
  
  BusService BusService::fromString(const std::string& serialized) {
    QString q_serialized (serialized.c_str());
    QStringList tokens = q_serialized.split("|");

    if (tokens.size() != NUM_SERIALIZED_FIELDS) {
      qWarning("%s: Insufficient number of fields! (%s)", TAG.c_str(),
         serialized.c_str());
      return BusService();
    }

    // Create the bus service object.
    BusService service;
    service.setCode(tokens[0].toStdString());

    return service;
  }
 
  std::string BusService::toString() {
    std::stringstream serialized_ss;
    serialized_ss << getCode();

    return serialized_ss.str();
  }

}

/* vim: set ts=2 sw=2 et: */
