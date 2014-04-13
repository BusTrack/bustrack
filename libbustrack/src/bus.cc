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

#include <stdexcept>

#include <QString>
#include <QStringList>
#include <QDebug>

#include "bustrack/bus.h"

namespace bustrack {
  
  const std::string Bus::TAG ("BusStop");

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

  Bus Bus::fromString(const std::string& serialized) {
    QString q_serialized (serialized.c_str());
    QStringList tokens = q_serialized.split("|");

    if (tokens.size() != NUM_SERIALIZED_FIELDS) {
      qWarning("%s: Insufficient number of fields! (%s)", TAG.c_str(),
          serialized.c_str());
      return Bus();
    }

    Bus bus;
    bus.setId(tokens[0].toStdString());

    return bus;
  }

  std::string Bus::toString() {
    return id_;
  }

}

/* vim: set ts=2 sw=2 et: */
