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

#include <cstdlib>
#include <ctime>
#include <cassert>

#include "dao/dao_manager.h"
#include "dao/bus_dao.h"

#include "base_station.h"

namespace bustrack {

  const std::string BaseStation::TAG ("BaseStation");
  const int BaseStation::NUM_ACTIVE_BUSES = 5;

  void BaseStation::simulateStep(const ServerContext& context) {
    qDebug("%s: Simulation step running...", TAG.c_str());

    // Obtain the data access classes.
    std::shared_ptr<DAOManager> dao_manager = context.getDAOManager();

    // Event loop!
    advanceActiveBuses(dao_manager);
    randomizeBusOccupancy(dao_manager);
    destinationCheck(dao_manager);
    maintainActiveBuses(dao_manager);
  }

  void BaseStation::advanceActiveBuses(std::shared_ptr<DAOManager> dao_manager) {
    qDebug("%s: Running hook: advanceActiveBuses", TAG.c_str());

    std::shared_ptr<BusDAO> bus_dao = dao_manager->getBusDAO();
    std::vector<Bus> buses = bus_dao->getItems();

    for (Bus bus : buses) {
      if (bus.isActive()) {
        // Obtain the service.
        BusService service = bus.getService();
        std::vector<std::shared_ptr<Waypoint>> route = service.getRoute();

        // Get target position on route.
        int destination = bus.getDestination();

        // Check if we have reached the final stop.
        std::shared_ptr<BusStop> waypoint = std::dynamic_pointer_cast<BusStop>(
            service.getRoute().at(destination));
        
        // Resolve the coordinates.
        std::shared_ptr<BusStopDAO> bus_stop_dao = dao_manager->getBusStopDAO();
        BusStop bus_stop = bus_stop_dao->getItem(waypoint->getId());

        // Set bus properties.
        bus.setLatitude(bus_stop.getLatitude());
        bus.setLongitude(bus_stop.getLongitude());
        bus.setDestination(destination + 1);

        bus_dao->removeItem(bus.getId());
        bus_dao->createItem(bus);

        qDebug("%s: Advanced bus %s (service %s) to bus stop %s", TAG.c_str(),
            bus.getId().c_str(), service.getCode().c_str(),
            waypoint->getId().c_str());
      }
    }
  }

  void BaseStation::randomizeBusOccupancy(std::shared_ptr<DAOManager> dao_manager) {
    qDebug("%s: Running hook: randomizeBusOccupancy", TAG.c_str());

    std::shared_ptr<BusDAO> bus_dao = dao_manager->getBusDAO();
    std::vector<Bus> buses = bus_dao->getItems();
    std::srand(std::time(NULL));

    for (Bus bus : buses) {
      if (bus.isActive()) {
        int occupancy = std::rand() % 50;
        bus.setOccupancy(occupancy);
        bus_dao->removeItem(bus.getId());
        bus_dao->createItem(bus);

        qDebug("%s: Bus %s now has occupancy %d", TAG.c_str(),
            bus.getId().c_str(), occupancy);
      }
    }
  }

  void BaseStation::destinationCheck(std::shared_ptr<DAOManager> dao_manager) {
    qDebug("%s: Running hook: destinationCheck", TAG.c_str());

    std::shared_ptr<BusDAO> bus_dao = dao_manager->getBusDAO();
    std::vector<Bus> buses = bus_dao->getItems();

    for (Bus bus : buses) {
      if (bus.isActive()) {
        // Obtain the service.
        BusService service = bus.getService();
        std::vector<std::shared_ptr<Waypoint>> route = service.getRoute();
        int route_length = route.size();

        // Get target position on route.
        int destination = bus.getDestination();
        if (route_length == destination) {
          qDebug("%s: Removing bus %s (service %s) from active service",
              TAG.c_str(), bus.getId().c_str(), service.getCode().c_str());

          bus.setDestination(0);
          bus.setIsActive(false);
          bus.setLatitude(0);
          bus.setLongitude(0);

          bus_dao->removeItem(bus.getId());
          bus_dao->createItem(bus);
        }
      }
    }
  }

  void BaseStation::maintainActiveBuses(
      std::shared_ptr<DAOManager> dao_manager) {
    qDebug("%s: Running hook: maintainActiveBuses", TAG.c_str());

    std::shared_ptr<BusDAO> bus_dao = dao_manager->getBusDAO();
    std::vector<Bus> buses = bus_dao->getItems();

    // Count the number of active buses.
    std::vector<Bus> inactive_buses;
    int num_active_buses = 0;
    for (Bus bus : buses) {
      if (bus.isActive()) {
        num_active_buses++;
      } else {
        inactive_buses.push_back(bus);
      }
    }

    // Are we below the active bus threshold?
    if (num_active_buses < NUM_ACTIVE_BUSES) {
      if (inactive_buses.size() == 0) {
        qWarning("%s: Active bus threshold reached, but no available buses to "
            "dispatch.", TAG.c_str());
        return;
      }

      qDebug("%s: Spawning a bus to maintain the number of active buses...",
          TAG.c_str());
      dispatchBusForRandomService(inactive_buses[0], dao_manager);
    }
  }

  void BaseStation::dispatchBusForRandomService(Bus bus,
      std::shared_ptr<DAOManager> dao_manager) {
    // Pick a random service.
    std::shared_ptr<BusServiceDAO> service_dao = dao_manager->getBusServiceDAO();
    std::vector<BusService> services = service_dao->getItems();
    int num_services = services.size();

    std::srand(std::time(NULL));
    int choice_num = std::rand() % num_services;
    BusService choice = services[choice_num];

    std::shared_ptr<BusDAO> bus_dao = dao_manager->getBusDAO();
    bus.setService(choice);

    // Determine initial location of bus.
    assert(choice.getRoute().at(0)->getType() == Waypoint::TYPE_BUS_STOP);
    std::shared_ptr<BusStop> waypoint = std::dynamic_pointer_cast<BusStop>(
        choice.getRoute().at(0));

    // Resolve the coordinates.
    std::shared_ptr<BusStopDAO> bus_stop_dao = dao_manager->getBusStopDAO();
    BusStop bus_stop = bus_stop_dao->getItem(waypoint->getId());

    // Set properties of bus.
    bus.setLatitude(bus_stop.getLatitude());
    bus.setLongitude(bus_stop.getLongitude());
    bus.setIsActive(true);
    bus.setDestination(1);
    bus_dao->removeItem(bus.getId());
    bus_dao->createItem(bus);

    qDebug("%s: Bus %s dispatched for random service (choice=%s)", TAG.c_str(),
        bus.getId().c_str(), choice.getCode().c_str());
  }

}

/* vim: set ts=2 sw=2 et: */
