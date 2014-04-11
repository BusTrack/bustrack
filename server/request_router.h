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

#ifndef BUSTRACK_REQUEST_ROUTER_H_
#define BUSTRACK_REQUEST_ROUTER_H_

#include "dao/bus_dao.h"
#include "dao/bus_service_dao.h"
#include "dao/bus_stop_dao.h"

#include "message.h"

namespace bustrack {

  /**
   * The RequestRouter class is responsible for determining the type of
   * request received by the ClientHandler, then routing it to the appropriate
   * logic to process the request.
   */
  class ServerContext;
  class RequestRouter {
  public:
    /**
     * Constructs a RequestRouter instance.
     *
     * @param context The sever context.
     */
    RequestRouter(ServerContext const* context);

    /**
     * We require the server context to proceed, so we disable the default
     * constructor.
     */
    RequestRouter() = delete;

    /**
     * Processes the request given by the message.
     *
     * @param Message request
     */
    void process(Message request);

  private:
    static const std::string TAG;
    ServerContext const* context_;
  };

}

#endif /* BUSTRACK_REQUEST_ROUTER_H_ */

/* vim: set ts=2 sw=2 et: */