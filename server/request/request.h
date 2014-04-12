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

#ifndef BUSTRACK_REQUEST_H_
#define BUSTRACK_REQUEST_H_

#include <string>
#include "message.h"

class QTcpSocket;
namespace bustrack {

  /**
   * The Request class represents a specific kind of request from the client.
   * This is the base class for all kinds of requests.
   *
   * The collection of Request classes implements the Command pattern so that
   * RequestRouter is not too concerned about what kind of request we have
   * during the execution stage.
   */
  class ServerContext;
  class Request {
  public:
    /**
     * Constructs a Request instance.
     *
     * @param context The server context.
     */
    Request(ServerContext const* context);

    /**
     * We will always require the server context, so we disable the default
     * constructor.
     */
    Request() = delete;

    /**
     * As this class is meant to be inherited and manipulated polymorphically,
     * declare the destructor as virtual.
     */
    virtual ~Request() { };

    /**
     * Processes the request given by message.
     *
     * @param message The message containing the request payload.
     * @param socket  The client socket.
     */
    virtual void process(Message message, QTcpSocket* socket) = 0;

  protected:
    /**
     * Returns the server context associated with this request.
     */
    ServerContext const* getContext();
   
  private:
    ServerContext const* context_;
  };

};

#endif /* BUSTRACK_REQUEST_H */

/* vim: set ts=2 sw=2 et: */
