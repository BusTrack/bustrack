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

#ifndef BUSTRACK_BUSTRACKD_H_
#define BUSTRACK_BUSTRACKD_H_

#include <QHostAddress>
#include <QDir>
#include "server.h"

namespace bustrack {

  /**
   * The BusTrack Daemon.
   */
  class BusTrackDaemon {
  public:
    static const int EXIT_OK = 0;
    static const int EXIT_NOT_ENOUGH_ARGUMENTS = 1;
    static const int EXIT_INVALID_SERVER_ADDRESS = 2;
    static const int EXIT_INVALID_SERVER_PORT = 3;
    static const int EXIT_DATA_DIR_NOT_EXIST = 4;
    static const int EXIT_LISTEN_FAILED = 5;

    static const int ARGS_MIN_LENGTH = 3;

    static int main(int argc, char* argv[]);

  private:
    static Server server_;

    static int startServer(const QHostAddress& address, const int& port,
        const QDir& data_dir);
    static void printHelp();
  };

}

#endif /* BUSTRACK_BUSTRACKD_H_ */

/* vim: set ts=2 sw=2 et: */
