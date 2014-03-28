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

#include <QHostAddress>
#include "client_handler.h"

namespace bustrack {

  ClientHandler::ClientHandler(QTcpSocket* socket,
      std::shared_ptr<RequestRouter> router) :
      socket_ (socket),
      socket_id_ (socket->socketDescriptor()),
      router_ (router) {
    qDebug("[%d] Connection from %s:%d received.", socket_id_,
        socket->peerAddress().toString().toStdString().c_str(),
        socket->peerPort());

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
  }

  void ClientHandler::onReadyRead() {
    // We should read in the command here and decide what it is, then invoke
    // RequestRouter to handle it.
    char buf[1024];
    qint64 lineLength = socket_->readLine(buf, sizeof(buf));
    if (lineLength != -1) {
      qDebug() << "Data received: " << lineLength;
    }
  }

  void ClientHandler::onSocketDisconnected() {
    qDebug("[%d] Connection closed.", socket_id_);
    emit jobDone(this);
  }

}

/* vim: set ts=2 sw=2 et: */
