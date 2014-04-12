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

#ifndef BUSTRACK_SERVER_CONTEXT_H_
#define BUSTRACK_SERVER_CONTEXT_H_

#include <memory>

#include <QHostAddress>
#include <QDir>

namespace bustrack {

  /**
   * This class contains the state data and services associated with a server
   * instance. It is used to share server services with other components that
   * require these services.
   *
   * In particular, ClientHandler can obtain a shared_ptr to the RequestRouter
   * and DAOManager associated with the server. It can then perform actions on
   * these services.
   */
  class DAOManager;
  class RequestRouter;
  class ServerContext {
  public:
    /**
     * Constructs a new ServerContext instance.
     *
     * Sets server parameters to sane default values, and prepares a instance
     * of RequestRouter and DAOManager for subsequent use.
     */
    ServerContext();

    /**
     * ServerContext implements copy-semantics. While the contained DAOManager
     * does not have copy-semantics, it is wrapped within a shared_ptr whose
     * copy-constructor will be called (and internal refcount incremented). In
     * actual fact, there is only one instance of DAOManager.
     *
     * So it's actually perfectly okay to have the default copy-constructor
     * and move-constructors defined for this.
     */

    /**
     * Returns the listening address of the server.
     */
    QHostAddress getListenAddress() const;

    /**
     * Sets the listening address of the server.
     * Once listen() has been called on the Server, this method has no effect.
     *
     * @param address The intended listening address for the server.
     */
    void setListenAddress(const QHostAddress& address);

    /**
     * Returns the listening port of the server.
     */
    quint16 getListenPort() const;

    /**
     * Sets the listening port of the server.
     * Once listen() has been called on the Server, this method has no effect.
     *
     * @param port The intended listening port for the server.
     */
    void setListenPort(const quint16& port);

    /**
     * Returns the data directory used by the server and all its associated
     * services.
     */
    QDir getDataDir() const;

    /**
     * Sets the data directory used by the server and all its associated
     * services. The data directory stores all the essential information
     * required for the operation of the server.
     *
     * This invalidates the existing DAOManager instance contained within the
     * context and creates a new one based on the new data directory.
     *
     * @param data_dir The data directory to be used.
     */
    void setDataDir(const QDir& data_dir);

    /**
     * Returns a shared_ptr to the RequestRouter associated with the server.
     */
    std::shared_ptr<RequestRouter> getRequestRouter() const;

    /**
     * Returns a shared_ptr to the DAOManager associated with the server.
     */
    std::shared_ptr<DAOManager> getDAOManager() const;

  private:
    static const QHostAddress DEFAULT_LISTEN_ADDRESS;
    static const quint16 DEFAULT_LISTEN_PORT;

    QDir data_dir_;
    QHostAddress address_;
    quint16 port_;

    std::shared_ptr<RequestRouter> request_router_;
    std::shared_ptr<DAOManager> dao_manager_;
  };

}

#endif /* BUSTRACK_SERVER_CONTEXT_H_ */

/* vim: set ts=2 sw=2 et: */
