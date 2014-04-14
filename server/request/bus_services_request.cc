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

#include "bus_services_request.h"

namespace bustrack {

  void BusServicesRequest::process(Message, Message response,
      QTcpSocket* socket) {
    // Obtain the DAO manager.
    std::shared_ptr<DAOManager> dao = getContext()->getDAOManager();
    std::shared_ptr<BusServiceDAO> bus_service_dao = dao->getBusServiceDAO();
    std::vector<BusService> bus_services = bus_service_dao->getItems();
    
    // Create the response payload.
    std::string payload;
    for (BusService bus_service : bus_services) {
      payload.append(bus_service.toString());
      payload.append("\n");
    }

    // Create the response message.
    response.setTag("BUS_SERVICES");
    response.setPayload(QString::fromStdString(payload).toLatin1());

    // Send the response message down the wire.
    std::string response_message = response.toString();
    socket->write(response_message.data(), response_message.size());
    socket->putChar('\n');
    socket->flush();
  }

}

/* vim: set ts=2 sw=2 et: */
