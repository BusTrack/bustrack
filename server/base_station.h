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

#ifndef BUSTRACK_BASE_STATION_H_
#define BUSTRACK_BASE_STATION_H_

#include "dao/bus_dao.h"
#include "server_context.h"

namespace bustrack {

  class BaseStation : public QObject {
    Q_OBJECT

  public slots:
    void simulateStep(const ServerContext& context);

  private:
    static const std::string TAG;
    static const int NUM_ACTIVE_BUSES;

    void advanceActiveBuses(std::shared_ptr<DAOManager> dao_manager);
    void randomizeBusOccupancy(std::shared_ptr<DAOManager> dao_manager);
    void maintainActiveBuses(std::shared_ptr<DAOManager> dao_manager);
    void dispatchBusForRandomService(Bus bus,
        std::shared_ptr<DAOManager> dao_manager);
    void destinationCheck(std::shared_ptr<DAOManager> dao_manager);
  };

}

#endif /* BUSTRACK_BASE_STATION_H_ */

/* vim: set ts=2 sw=2 et: */
