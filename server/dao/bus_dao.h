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

#ifndef BUSTRACK_BUS_DAO_H_
#define BUSTRACK_BUS_DAO_H_

#include <unordered_map>
#include <QDir>

#include "bustrack/bus.h"
#include "data_access_object.h"

namespace bustrack {

  class BusDAO : public DataAccessObject {
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
    BusDAO(const QDir& data_dir);
    BusDAO() = delete;
    ~BusDAO();

    /**
     * Saves the internal list of buses to disk.
     */
    virtual void commit();

    /**
     * Restore the internal list of buses from disk.
     */
    virtual void rollback();

    /**
     * Returns a vector of Bus instances.
     */
    std::vector<Bus> getBuses() const;

    /**
     * Returns a Bus instance given the plate number of the bus.
     * If a bus of the given plate number does not exist, an exception is thrown.
     */
    Bus getBus(const std::string& plate_number) const;

    /**
     * Returns true if a bus of the given plate number exists, false otherwise.
     */
    bool busExists(const std::string& plate_number) const;
    
    /**
     * Creates a bus using the specified instance of Bus.
     *
     * Returns false if a bus with the given plate number already exists, true
     * otherwise.
     *
     * @param bus The bus to be created.
     */
    bool createBus(Bus bus);

    /**
     * Replaces an existing bus using the specified instance of Bus.
     * Returns false if an existing bus does not exist, true otherwise.
     *
     * @param bus The bus to be replaced.
     */
    bool replaceBus(Bus bus);

    /**
     * Removes an existing bus given by plate_number.
     * Returns true if the bus could be found, false otherwise.
     *
     * @param plate_number The plate number of the bus to be removed.
     */
    bool removeBus(const std::string& plate_number);

  private:
    static const QString DATA_FILE_NAME;
    static const int NUM_FIELDS = 1;

    std::unordered_map<std::string, Bus> buses_;
    QDir data_dir_;
    QString data_file_path_;
  };

}

#endif /* BUSTRACK_BUS_DAO_H_ */

/* vim: set ts=2 sw=2 et: */
