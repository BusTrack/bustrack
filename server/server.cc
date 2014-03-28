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

#include <iostream>
#include <QTcpSocket>
#include "server.h"

namespace bustrack {

  Server::Server(QObject*) :
      bus_stop_dao_(nullptr),
      bus_dao_(nullptr) {
    connect(this, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
  }

  void Server::setDataDir(const QDir& data_dir) {
    data_dir_ = data_dir;
  }

  QDir Server::getDataDir() const {
    return data_dir_;
  }

  bool Server::listen(const QHostAddress& address, quint16 port) {
    // Before we do the actual listening, create the DAOs.
    bus_stop_dao_ = std::unique_ptr<BusStopDAO>(new BusStopDAO(data_dir_));
    bus_dao_ = std::unique_ptr<BusDAO>(new BusDAO(data_dir_));

    // Now we can listen.
    return QTcpServer::listen(address, port);
  }

  void Server::handleNewConnection() {
    QTcpSocket* socket = nextPendingConnection();

    // Test writing some data to the socket then closing it.
    std::vector<Bus> buses = bus_dao_->getItems();
    for (Bus bus : buses) {
      socket->write(bus.getId().c_str(), bus.getId().length());
      socket->write("\r\n", 2);
    }

    socket->close();

    // TODO: We should create a new ClientHandler instance here, then pass
    // control of the socket to this instance. There is no need to perform
    // any threading.
  }

}

/* vim: set ts=2 sw=2 et: */
