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

#ifndef BUSTRACK_DAO_MANAGER_H_
#define BUSTRACK_DAO_MANAGER_H_

#include "../dao/bus_dao.h"
#include "../dao/bus_service_dao.h"
#include "../dao/bus_stop_dao.h"

namespace bustrack {

  /**
   * Represents the Data Access Object Manager.
   *
   * This class manages the lifecycle of different Data Access Objects
   * implemented in BusTrack.
   */
  class DAOManager {
  public:
    /**
     * Constructs a new DAOManager instance, given the data directory.
     *
     * @param data_dir The data directory to use.
     */
    DAOManager(const QDir& data_dir);

    /**
     * Since we need the data directory, we declare the default constructor
     * as deleted.
     */
    DAOManager() = delete;

    /**
     * Returns a shared_ptr to the Bus DAO.
     */
    std::shared_ptr<BusDAO> getBusDAO();

    /**
     * Returns a shared_ptr to the Bus Service DAO.
     */
    std::shared_ptr<BusServiceDAO> getBusServiceDAO();

    /**
     * Returns a shared_ptr to the Bus Stop DAO.
     */
    std::shared_ptr<BusStopDAO> getBusStopDAO();

  private:
    std::shared_ptr<BusDAO> bus_dao_;
    std::shared_ptr<BusServiceDAO> bus_service_dao_;
    std::shared_ptr<BusStopDAO> bus_stop_dao_;
  };

}

#endif /* BUSTRACK_DAO_MANAGER_H_ */

/* vim: set ts=2 sw=2 et: */
