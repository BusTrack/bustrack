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

#ifndef BUSTRACK_BUSES_REQUEST_H_
#define BUSTRACK_BUSES_REQUEST_H_

#include "request.h"

namespace bustrack {

  /**
   * This represents a request for the list of bus stops.
   */
  class BusesRequest : public Request {
  public:
    /**
     * Constructs a BusesRequest instance.
     *
     * @param context The server context.
     */
    BusesRequest(ServerContext const* context) : Request(context) { };

    /**
     * We will always require the server context, so we disable the default
     * constructor.
     */
    BusesRequest() = delete;

    /**
     * Processes the request given by the message.
     *
     * @param request  The message containing the request payload.
     * @param response The message containing the response payload.
     * @param socket  The client socket.
     */
    virtual void process(Message request, Message response, QTcpSocket* socket);
  };

}

#endif /* BUSTRACK_BUSES_REQUEST_H_ */

/* vim: set ts=2 sw=2 et: */
