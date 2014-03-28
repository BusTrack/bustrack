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

#ifndef BUSTRACK_BUS_SERVICE_DAO_H_
#define BUSTRACK_BUS_SERVICE_DAO_H_

#include <QDir>

#include "bustrack/bus_service.h"
#include "file_based_dao.h"

namespace bustrack {

  /**
   * This is the data access object (DAO) for bus services.
   * Provides a high level abstraction for retrieval of bus services in
   * BusTrack.
   *
   * This DAO uses a file-based source to create the data model.
   */
  class BusServiceDAO : public FileBasedDAO<BusService> {
  public:
    /**
     * The constructor invokes the rollback() method to perform an initial
     * population of the internal list of buses from a file.
     *
     * @param data_dir A QDir object that points to a data directory.
     */
    BusServiceDAO(const QDir& data_dir);
    BusServiceDAO() = delete;
    ~BusServiceDAO();

    /**
     * Saves the internal list of bus services to disk.
     */
    void commit();

    /**
     * Restore the internal list of bus services from disk.
     */
    void rollback();

  private:
    static const QString DATA_FILE_NAME;
    static const int NUM_FIELDS = 1;
  };

}

#endif /* BUSTRACK_BUS_SERVICE_DAO_H_ */

/* vim: set ts=2 sw=2 et: */
