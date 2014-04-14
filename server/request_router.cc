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

#include "request/bus_services_request.h"
#include "request/bus_stops_request.h"
#include "request/buses_request.h"
#include "request/invalid_request.h"

#include "server_context.h"

#include "request_router.h"

namespace bustrack {

  const std::string RequestRouter::TAG ("RequestRouter");
  const std::string RequestRouter::REQUEST_BUS_SERVICES_TAG ("BUS_SERVICES");
  const std::string RequestRouter::REQUEST_BUS_STOPS_TAG ("BUS_STOPS");
  const std::string RequestRouter::REQUEST_BUSES_TAG ("BUSES");

  RequestRouter::RequestRouter(ServerContext const* context) :
    context_(context) {
  }

  void RequestRouter::process(Message request, QTcpSocket* socket) {
    qDebug("%s: routing request with tag %s and id %d", TAG.c_str(),
        request.getTag().c_str(), request.getId());

    // Prepare a response template.
    Message response;
    response.setId(request.getId());

    // Get the actual request.
    std::unique_ptr<Request> actual_request = getActualRequest(request);
    actual_request->process(request, response, socket);
  }

  std::unique_ptr<Request> RequestRouter::getActualRequest(Message request) {
    if (request.getTag() == REQUEST_BUS_SERVICES_TAG) {
      return std::unique_ptr<Request>(new BusServicesRequest(context_));
    } else if (request.getTag() == REQUEST_BUS_STOPS_TAG) {
      return std::unique_ptr<Request>(new BusStopsRequest(context_));
    } else if (request.getTag() == REQUEST_BUSES_TAG) {
      return std::unique_ptr<Request>(new BusesRequest(context_));
    } else {
      return std::unique_ptr<Request>(new InvalidRequest(context_));
    }
  }

}

/* vim: set ts=2 sw=2 et: */
