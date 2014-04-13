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
        *file_stream << bus_stop.toString() << endl;
      }

      closeCommit();
    }
  }

  void BusStopDAO::rollback() {
    using std::shared_ptr;
    using std::fstream;

    shared_ptr<fstream> file_stream (prepareRollback());
    if (file_stream != nullptr) {
      for (std::string line; getline(*file_stream, line);) {
        BusStop bus_stop (BusStop::fromString(line));
        items_.insert(std::make_pair(bus_stop.getId(), bus_stop));
      }

      closeRollback();
    }
  }

}

/* vim: set ts=2 sw=2 et: */
