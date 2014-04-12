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

#ifndef BUSTRACK_INVALID_REQUEST_H_
#define BUSTRACK_INVALID_REQUEST_H_

#include "request.h"

namespace bustrack {

  /**
   * This represents a request for the bus stop list.
   */
  class InvalidRequest : public Request {
  public:
    /**
     * Constructs an InvalidRequest instance.
     *
     * @param context The server context.
     */
    InvalidRequest(ServerContext const* context): Request(context) { };

    /**
     * We will always require the server context, so we disable the default
     * constructor.
     */
    InvalidRequest() = delete;

    /**
     * Processes the request given by the message.
     *
     * @param message The message containing the request payload.
     * @param socket  The client socket.
     */
    virtual void process(Message message, QTcpSocket* socket);
  };

}

#endif /* BUSTRACK_INVALID_REQUEST_H_ */

/* vim: set ts=2 sw=2 et: */
