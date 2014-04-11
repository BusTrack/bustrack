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

#include "server_context.h"
#include "request_router.h"

#include "client_handler.h"

namespace bustrack {

  const std::string ClientHandler::TAG ("ClientHandler");

  ClientHandler::ClientHandler(QTcpSocket* socket,
      ServerContext const* context) :
      socket_ (socket),
      socket_id_ (socket->socketDescriptor()),
      context_ (context) {
    // Set up events for this socket.
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));

    qDebug("%s [%d]: Connection from %s:%d received.", TAG.c_str(), socket_id_,
        socket->peerAddress().toString().toStdString().c_str(),
        socket->peerPort());
  }

  void ClientHandler::onReadyRead() {
    // We should read in the command here and decide what it is, then invoke
    // RequestRouter to handle it.
    char buf[1024];
    qint64 line_length = socket_->readLine(buf, sizeof(buf));
    if (line_length != -1) {
      qDebug("%s [%d]: %lld bytes received", TAG.c_str(), socket_id_,
          line_length);
    }

    QString line = QString(buf).trimmed();
    Message message = Message::decodeFromString(line.toStdString());
    qDebug("%s [%d]: Message with tag %s received", TAG.c_str(), socket_id_,
        message.getTag().c_str());

    // Obtain the request router.
    std::shared_ptr<RequestRouter> router = context_->getRequestRouter();
    router->process(message);
  }

  void ClientHandler::onSocketDisconnected() {
    // Disconnect socket events before cleanup.
    disconnect(socket_, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    disconnect(socket_, SIGNAL(disconnected()), this,
        SLOT(onSocketDisconnected()));

    qDebug("%s [%d]: Connection closed.", TAG.c_str(), socket_id_);
    emit jobDone(this);
  }

}

/* vim: set ts=2 sw=2 et: */
