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

#ifndef BUSTRACK_SERVER_H_
#define BUSTRACK_SERVER_H_

#include <memory>
#include <QTcpServer>

#include "server_context.h"

namespace bustrack {

  /**
   * The BusTrack TCP socket server.
   *
   * This socket server listens for connection requests based on parameters
   * specified by its ServerContext. The ServerContext can be set via
   * the setContext() method.
   *
   * If there is an incoming connection, handleNewConnection() is invoked and
   * a ClientHandler instance is spawned to handle this connection.
   *
   * This socket server does not use threading as it relies on Qt's async
   * programming model and event loop to multiplex connections.
   */
  class ClientHandler;
  class Server : public QTcpServer {
    Q_OBJECT

  public:
    /**
     * Constructs a Server instance.
     * @param parent The QObject parent of this server instance.
     */
    Server(QObject* parent = 0);

    /**
     * Returns the server context in use.
     * 
     * As this method returns a copy of the server context, changes performed
     * to the returned ServerContext instance will not be reflected in this
     * Server instance.
     *
     * To make changes to the server context, obtain an instance of the server
     * contxt using this method, make changes to the instance, then call
     * setContext() to commit the changes.
     *
     * @return A copy of the server context in use.
     */
    ServerContext getContext() const;

    /**
     * Sets the server context to be used.
     *
     * @param context The server context to be used.
     */
    void setContext(const ServerContext& context);

    /**
     * Initiate listening. False is returned on failure. The exact failure
     * condition can be determined by calling serverError() or errorString()
     * from the QTcpServer class.
     *
     * @return true on success, false otherwise
     */
    bool listen();

  private slots:
    /**
     * Handles a new connection. This method spawns a ClientHandler instance
     * and passes the next pending QTcpSocket (in the connected state) to it.
     */
    void handleNewConnection();

    /**
     * Cleans up the ClientHandler instance after it is complete.
     */
    void clientHandlerComplete(ClientHandler* handler);

  private:
    ServerContext context_;

    /**
     * We disable the listen() method from QTcpServer because we do not
     * rely on the two parameters to determine the listening address and port
     * of the server.
     *
     * Instead, the port and address are obtained from the server context.
     */
    bool listen(const QHostAddress& address, quint16 port) {
      return QTcpServer::listen(address, port);
    };
  };

}

#endif /* BUSTRACK_SERVER_H_ */

/* vim: set ts=2 sw=2 et: */
