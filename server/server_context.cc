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

#include "dao/dao_manager.h"
#include "request_router.h"

#include "server_context.h"

namespace bustrack {

  const QHostAddress ServerContext::DEFAULT_LISTEN_ADDRESS = QHostAddress::Any;
  const quint16 ServerContext::DEFAULT_LISTEN_PORT = 0;

  ServerContext::ServerContext() :
      address_(DEFAULT_LISTEN_ADDRESS),
      port_(DEFAULT_LISTEN_PORT) {
    // The classes that require the context should not be able to modify the
    // context object itself.
    ServerContext const* context = this;
    request_router_ = std::make_shared<RequestRouter>(context);
    dao_manager_ = std::make_shared<DAOManager>(data_dir_);
  }

  QHostAddress ServerContext::getListenAddress() const {
    return address_;
  }

  void ServerContext::setListenAddress(const QHostAddress& address) {
    address_ = address;
  }

  quint16 ServerContext::getListenPort() const {
    return port_;
  }

  void ServerContext::setListenPort(const quint16& port) {
    port_ = port;
  }

  QDir ServerContext::getDataDir() const {
    return data_dir_;
  }

  void ServerContext::setDataDir(const QDir& data_dir) {
    data_dir_ = data_dir;

    // If the data directory is changed, we should invalidate DAOManager and
    // create a new instance. This will reinitialize all the child DAOs as
    // well.
    dao_manager_.reset();
    dao_manager_ = std::make_shared<DAOManager>(data_dir_);
  }

  std::shared_ptr<RequestRouter> ServerContext::getRequestRouter() const {
    return request_router_;
  }

  std::shared_ptr<DAOManager> ServerContext::getDAOManager() const {
    return dao_manager_;
  }

}

/* vim: set ts=2 sw=2 et: */
