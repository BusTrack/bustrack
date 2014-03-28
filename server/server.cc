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

  Server::Server(QObject* parent) {
    connect(this, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
  }

  void Server::setDataDir(const QDir& data_dir) {
    data_dir_ = data_dir;
  }

  QDir Server::getDataDir() const {
    return data_dir_;
  }

  void Server::handleNewConnection() {
    QTcpSocket* socket = nextPendingConnection();

    // Test writing some data to the socket then closing it.
    socket->write("test\r\n", 7);
    socket->close();

    // TODO: We should create a new ClientHandler instance here, then pass
    // control of the socket to this instance. There is no need to perform
    // any threading.
  }

}

/* vim: set ts=2 sw=2 et: */
