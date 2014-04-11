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

#ifndef BUSTRACK_CLIENT_HANDLER_H_
#define BUSTRACK_CLIENT_HANDLER_H_

#include <memory>
#include <QTcpSocket>
#include "message.h"

namespace bustrack {

  /**
   * The ClientHandler class is reponsible for managing communication with a
   * single client over the network. An instance is created by Server for each 
   * connection received.
   *
   * Upon receiving data from the network, the data is passed on to
   * RequestRouter, which determines the kind of request. Once the kind of
   * request has been determined, the request is then execute, and the
   * resulting response is sent back to the client.
   */
  class ServerContext;
  class ClientHandler : public QObject {
    Q_OBJECT

  public:
    /**
     * Constructs a ClientHandler instance.
     *
     * @param socket The socket to be managed.
     * @param context The server context.
     */
    ClientHandler(QTcpSocket* socket, ServerContext const* context);

    /**
     * As we need the socket and context to proceed, we should delete the
     * default constructor.
     */
    ClientHandler() = delete;

    /**
     * ClientHandlers should not be copy-constructible nor copy-assignable.
     */
    ClientHandler(const ClientHandler&) = delete;
    ClientHandler& operator=(const ClientHandler&) = delete;

    /**
     * ClientHandlers can be move-constructible and move-assignable.
     */
    ClientHandler(ClientHandler&&) = default;
    ClientHandler& operator=(ClientHandler&&) = default;

  signals:
    /**
     * Emitted when the job of this ClientHandler instance is done and should
     * be cleaned up by its parent.
     *
     * In BusTrack, this signal is processed by Server. As the memory for this
     * instance is allocated using new, the Server makes use of this event
     * to free up memory used by this instance.
     *
     * @param handler A pointer to this instance.
     */
    void jobDone(ClientHandler* handler);

  private slots:
    /**
     * Called when there is data available on the socket to be read.
     */
    void onReadyRead();
  
    /**
     * Called when the socket has been disconnected by either a network
     * condition, or client action.
     */
    void onSocketDisconnected();

  private:
    static const std::string TAG;
    static const int NUM_TOKENS_IN_REQUEST = 3;

    QTcpSocket* socket_;
    int socket_id_;
    ServerContext const* context_;
  };

}

#endif /* BUSTRACK_CLIENT_HANDLER_H_ */

/* vim: set ts=2 sw=2 et: */
