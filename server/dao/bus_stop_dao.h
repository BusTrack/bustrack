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

#ifndef BUSTRACK_BUS_STOP_DAO_H_
#define BUSTRACK_BUS_STOP_DAO_H_

#include <unordered_map>
#include <vector>

#include <QDir>

#include "bustrack/bus_stop.h"
#include "data_access_object.h"

namespace bustrack {

  /**
   * This is the data access object (DAO) for bus stops.
   * Provides a high level abstraction for retrieval of bus stops in BusTrack.
   *
   * This DAO uses a file-based source to create the data model.
   */
  class BusStopDAO : public DataAccessObject {
  public:
    /**
     * Constructs a new instance of the data access object.
     * The data directory indicates the directory to look for the data file.
     *
     * The constructor invokes the rollback() method to perform an initial
     * population of the internal list of bus stops from a file.
     *
     * @param data_dir A QDir object that points to a data directory.
     */
    BusStopDAO(const QDir& data_dir);
    BusStopDAO() = delete;
    ~BusStopDAO();

    /**
     * Saves the internal list of bus stops to disk.
     */
    virtual void commit();

    /**
     * Restore the internal list of bus stops from disk.
     */
    virtual void rollback();
    
    /**
     * Returns a vector of BusStop instances.
     */
    std::vector<BusStop> getBusStops() const;

    /**
     * Returns a BusStop instance given the ID of the stop.
     * If a bus stop of the given ID does not exist, an exception is thrown.
     */
    BusStop getBusStop(const std::string& stop_id) const;

    /**
     * Returns true if a bus stop of the given ID exists, false otherwise.
     */
    bool busStopExists(const std::string& stop_id) const;
    
    /**
     * Creates a bus stop using the specified instance of BusStop.
     *
     * Returns false if a bus stop with the given ID already exists, true
     * otherwise.
     *
     * @param bus_stop The bus stop to be created.
     */
    bool createBusStop(BusStop bus_stop);

    /**
     * Replaces an existing bus stop using the specified instance of BusStop.
     * Returns false if an existing bus stop does not exist, true otherwise.
     *
     * @param bus_stop The bus stop to be replaced.
     */
    bool replaceBusStop(BusStop bus_stop);

    /**
     * Removes an existing bus stop given by stop_id.
     * Returns true if the bus stop could be found, false otherwise.
     *
     * @param stop_id The stop ID of the bus stop to be removed.
     */
    bool removeBusStop(const std::string& stop_id);

  private:
    static const QString DATA_FILE_NAME;
    static const int NUM_FIELDS = 4;

    std::unordered_map<std::string, BusStop> bus_stops_;
    QDir data_dir_;
    QString data_file_path_;
  };

}

#endif /* BUSTRACK_BUS_STOP_DAO_H_ */

/* vim: set ts=2 sw=2 et: */
