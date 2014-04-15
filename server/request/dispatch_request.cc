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

#include <QTcpSocket>

#include "../server_context.h"
#include "../dao/dao_manager.h"

#include "dispatch_request.h"

namespace bustrack {

  const std::string DispatchRequest::TAG ("DispatchRequest");

  void DispatchRequest::process(Message request, Message response,
      QTcpSocket* socket) {
    QString decoded_request (request.getPayload());
    decoded_request = decoded_request.trimmed();

    // Obtain the DAO manager.
    std::shared_ptr<DAOManager> dao = getContext()->getDAOManager();

    // Check if the requested service exists.
    std::shared_ptr<BusServiceDAO> service_dao = dao->getBusServiceDAO();
    std::vector<BusService> services = service_dao->getItems();
   
    BusService chosen_service;
    bool service_found = false;
    for (BusService service : services) {
      if (service.getCode() == decoded_request.toStdString()) {
        service_found = true;
        chosen_service = service;
        break;
      }
    }

    if (!service_found) {
      response.setTag("ERROR");
      response.setPayload("Requested service cannot be found.");
      
      std::string response_message = response.toString();
      socket->write(response_message.data(), response_message.size());
      socket->putChar('\n');
      socket->flush();
      return;
    }

    // Now that the service is found, dispatch a bus for it.
    std::shared_ptr<BusDAO> bus_dao = dao->getBusDAO();
    std::vector<Bus> buses = bus_dao->getItems();

    // Pick a random unused bus.
    bool dispatched = false;
    for (Bus bus : buses) {
      if (!bus.isActive()) {
        // Determine the initial location of bus.
        std::shared_ptr<BusStop> waypoint = std::dynamic_pointer_cast<BusStop>(
            chosen_service.getRoute().at(0));

        // Resolve the coordinates.
        std::shared_ptr<BusStopDAO> bus_stop_dao = dao->getBusStopDAO();
        BusStop bus_stop = bus_stop_dao->getItem(waypoint->getId());

        // Set properties of bus.
        bus.setLatitude(bus_stop.getLatitude());
        bus.setLongitude(bus_stop.getLongitude());
        bus.setIsActive(true);
        bus.setDestination(1);
        bus.setService(chosen_service);
        bus_dao->removeItem(bus.getId());
        bus_dao->createItem(bus);

        dispatched = true;
        qDebug("%s: Dispatched bus %s for service %s on user request",
            TAG.c_str(), bus.getId().c_str(), chosen_service.getCode().c_str());
        break;
      }
    }

    if (!dispatched) {
      response.setTag("ERROR");
      response.setPayload("No more spare buses exist.");
    } else {
      response.setTag("OK");
    }
    
    // Send the response message down the wire.
    std::string response_message = response.toString();
    socket->write(response_message.data(), response_message.size());
    socket->putChar('\n');
    socket->flush();
  }

}

/* vim: set ts=2 sw=2 et: */
