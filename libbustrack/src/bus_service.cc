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
#include "bustrack/bus_service.h"

namespace bustrack {

  const std::string BusService::TAG ("BusService");

  std::string BusService::getCode() {
    return code_;
  }

  void BusService::setCode(std::string code) {
    code_ = code;
  }

  std::vector<std::shared_ptr<Waypoint>> BusService::getRoute() {
    return route_;
  }

  void BusService::setRoute(std::vector<std::shared_ptr<Waypoint>> route) {
    route_ = route;
  }
  
  BusService BusService::fromString(const std::string& serialized) {
    QString q_serialized (serialized.c_str());
    QStringList tokens = q_serialized.split("|");

    if (tokens.size() < NUM_SERIALIZED_FIELDS) {
      qWarning("%s: Insufficient number of fields! (%s)", TAG.c_str(),
         serialized.c_str());
      return BusService();
    }

    // Create the bus service object.
    BusService service;
    service.setCode(tokens[0].toStdString());

    // Create the routes.
    std::vector<std::shared_ptr<Waypoint>> route;
    for (int i = 1; i < tokens.size(); i++) {
      QString waypoint_str (tokens[i]);
      if (waypoint_str.trimmed().isEmpty()) {
        continue;
      }

      QStringList waypoint_tokens = waypoint_str.split(",");

      if (waypoint_tokens.size() < 1) {
        qWarning("%s: Invalid waypoint found for bus service %s", TAG.c_str(),
            service.getCode().c_str());
        continue;
      }

      // Determine if we have a NORMAL waypoint or BUS_STOP.
      if (waypoint_tokens[0].toStdString() == Waypoint::TYPE_NORMAL) {
        std::shared_ptr<Waypoint> waypoint = std::make_shared<Waypoint>();

        // Parse the other tokens.
        if (waypoint_tokens.size() < 3) {
          qWarning("%s: Normal waypoint does not have sufficient tokens",
              TAG.c_str());
          continue;
        }

        // Convert the coordinates.
        bool parsing_ok = false;
        waypoint->setLatitude(tokens[1].toFloat(&parsing_ok));
        if (!parsing_ok) {
          qWarning("%s: Unable to parse latitude for waypoint", TAG.c_str());
          continue;
        }

        waypoint->setLongitude(tokens[2].toFloat(&parsing_ok));
        if (!parsing_ok) {
          qWarning("%s: Unable to parse longitude for waypoint", TAG.c_str());
          continue;
        }

        route.push_back(waypoint);

      } else if (waypoint_tokens[0].toStdString() == Waypoint::TYPE_BUS_STOP) {
        std::shared_ptr<BusStop> bus_stop = std::make_shared<BusStop>();

        // Parse the other tokens.
        if (waypoint_tokens.size() < 2) {
          qWarning("%s: Bus stop waypoint does not have sufficient tokens",
              TAG.c_str());
          continue;
        }

        bus_stop->setId(waypoint_tokens[1].toStdString());
        route.push_back(bus_stop);

      } else {
        qWarning("%s: Invalid waypoint type found for bus service %s",
            TAG.c_str(), service.getCode().c_str());
        continue;
      }
    }

    service.setRoute(route);
    return service;
  }
 
  std::string BusService::toString() {
    std::stringstream serialized_ss;
    serialized_ss << getCode() << "|";

    // Write the routes too.
    for (std::shared_ptr<Waypoint> waypoint : getRoute()) {
      serialized_ss << waypoint->getType();

      if (waypoint->getType() == Waypoint::TYPE_NORMAL) {
        serialized_ss << "," << waypoint->getLatitude() << "," <<
          waypoint->getLongitude();
      } else if (waypoint->getType() == Waypoint::TYPE_BUS_STOP) {
        std::shared_ptr<BusStop> bus_stop =
          std::dynamic_pointer_cast<BusStop>(waypoint);
        serialized_ss << "," << bus_stop->getId();
      }

      serialized_ss << "|";
    }

    return serialized_ss.str();
  }

}

/* vim: set ts=2 sw=2 et: */
