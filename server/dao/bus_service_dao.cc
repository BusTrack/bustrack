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
#include "bus_service_dao.h"

namespace bustrack {

  const QString BusServiceDAO::DATA_FILE_NAME = "bus_services.txt";

  BusServiceDAO::BusServiceDAO(const QDir& data_dir) :
      FileBasedDAO<BusService>(data_dir, DATA_FILE_NAME) {
    rollback();
  }

  BusServiceDAO::~BusServiceDAO() {
    commit();
  }

  void BusServiceDAO::commit() {
    using std::shared_ptr;
    using std::fstream;
    using std::pair;
    using std::string;
    using std::endl;

    shared_ptr<fstream> file_stream (prepareCommit());
    if (file_stream != nullptr) {
      // Write out bus services, line by line.
      for (pair<string, BusService> service_pair : items_) {
        BusService service = service_pair.second;
        *file_stream << service.toString() << endl;
      }

      closeCommit();
    }
  }

  void BusServiceDAO::rollback() {
    using std::shared_ptr;
    using std::fstream;

    shared_ptr<fstream> file_stream (prepareRollback());
    if (file_stream != nullptr) {
      for (std::string line; getline(*file_stream, line);) {
        BusService service (BusService::fromString(line));
        items_.insert(std::make_pair(service.getCode(), service));
      }

      closeRollback();
    }
  }

}

/* vim: set ts=2 sw=2 et: */
