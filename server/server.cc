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
#include <QTimer>

#include "client_handler.h"
#include "server.h"

namespace bustrack {

  Server::Server(QObject*) :
      base_station_timer_(new QTimer(this)) {
    connect(this, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
  }

  ServerContext Server::getContext() const {
    return context_;
  }

  void Server::setContext(const ServerContext& context) {
    context_ = context;
  }

  bool Server::listen() {
    connect(base_station_timer_,
        SIGNAL(timeout()),
        this,
        SLOT(handleBaseStationTimer()));
    base_station_timer_->start(5000);

    return listen(context_.getListenAddress(), context_.getListenPort());
  }

  void Server::handleNewConnection() {
    QTcpSocket* socket = nextPendingConnection();

    // Pass control of the socket to a ClientHandler.
    // There is no need to perform any threading.
    ClientHandler* handler = new ClientHandler(socket,
        reinterpret_cast<ServerContext const*>(&context_));
    connect(handler, SIGNAL(jobDone(ClientHandler*)), this,
        SLOT(clientHandlerComplete(ClientHandler*)));
  }

  void Server::clientHandlerComplete(ClientHandler* handler) {
    delete handler;
  }

  void Server::handleBaseStationTimer() {
    base_station_.simulateStep(context_);
  }

}

/* vim: set ts=2 sw=2 et: */
