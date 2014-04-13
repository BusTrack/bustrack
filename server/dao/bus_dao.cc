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

#include <fstream>
#include <QDebug>
#include "bus_dao.h"

namespace bustrack {

  const QString BusDAO::DATA_FILE_NAME = "buses.txt";

  BusDAO::BusDAO(const QDir& data_dir) :
      FileBasedDAO<Bus>(data_dir, DATA_FILE_NAME) {
    rollback();
  }

  BusDAO::~BusDAO() {
    commit();
  }

  void BusDAO::commit() {
    using std::shared_ptr;
    using std::fstream;
    using std::pair;
    using std::string;
    using std::endl;

    shared_ptr<fstream> file_stream (prepareCommit());
    if (file_stream != nullptr) {
      // Write out buses, line by line.
      for (pair<string, Bus> bus_pair : items_) {
        Bus bus = bus_pair.second;
        *file_stream << bus.toString() << endl;
      }

      closeCommit();
    }
  }

  void BusDAO::rollback() {
    using std::shared_ptr;
    using std::fstream;

    shared_ptr<fstream> file_stream (prepareRollback());
    if (file_stream != nullptr) {
      // Read in buses, line by line.
      // Each line should have the format: bus_id
      for (std::string line; getline(*file_stream, line);) {
        Bus bus (Bus::fromString(line));
        items_.insert(std::make_pair(bus.getId(), bus));
      }

      closeRollback();
    }
  }

}

/* vim: set ts=2 sw=2 et: */
