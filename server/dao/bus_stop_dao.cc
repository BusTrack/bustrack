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

#include <iostream>
#include <fstream>

#include <QDebug>

#include "bus_stop_dao.h"

namespace bustrack {

  const QString BusStopDAO::DATA_FILE_NAME = "bus_stops.txt";

  BusStopDAO::BusStopDAO(const QDir& data_dir) :
      FileBasedDAO<BusStop>(data_dir, DATA_FILE_NAME) {
    rollback();
  }

  BusStopDAO::~BusStopDAO() {
    commit();
  }

  void BusStopDAO::commit() {
    using std::shared_ptr;
    using std::fstream;
    using std::pair;
    using std::string;
    using std::endl;

    shared_ptr<fstream> file_stream (prepareCommit());
    if (file_stream != nullptr) {
      // Write out bus stops, line by line.
      for (pair<string, BusStop> bus_stop_pair : items_) {
        BusStop bus_stop = bus_stop_pair.second;
        *file_stream << bus_stop.getId() << "|" << bus_stop.getName() << "|" <<
          bus_stop.getLatitude() << "|" << bus_stop.getLongitude() << endl;
      }

      closeCommit();
    }
  }

  void BusStopDAO::rollback() {
    using std::shared_ptr;
    using std::fstream;

    shared_ptr<fstream> file_stream (prepareRollback());
    if (file_stream != nullptr) {
      // Read in bus stops, line by line.
      // Each line should have the format: stop_id|stop_name|lat|lng
      for (std::string line; getline(*file_stream, line);) {
        QString q_line (line.c_str());
        QStringList tokens = q_line.split("|");

        if (tokens.size() != NUM_FIELDS) {
          qWarning() << "One of the bus stops in file has insufficient number "
            "of fields.";
          continue;
        }

        // Create the bus stop object.
        BusStop bus_stop;
        bus_stop.setId(tokens[0].toStdString());
        bus_stop.setName(tokens[1].toStdString());

        // Convert the coordinates.
        bool parsing_ok = false;
        bus_stop.setLatitude(tokens[2].toFloat(&parsing_ok));
        if (!parsing_ok) {
          qWarning() << "Unable to parse latitude for bus stop with ID " << 
            QString(bus_stop.getId().c_str());
          continue;
        }

        bus_stop.setLongitude(tokens[3].toFloat(&parsing_ok));
        if (!parsing_ok) {
          qWarning() << "Unable to parse longitude for bus stop with ID " <<
            QString(bus_stop.getId().c_str());
          continue;
        }

        // Stuff the bus stop object into our list.
        items_.insert(std::make_pair(bus_stop.getId(), bus_stop));
      }

      closeRollback();
    }
  }

}

/* vim: set ts=2 sw=2 et: */
