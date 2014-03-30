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

#include <QCoreApplication>
#include <QStringList>

#include "bustrackd.h"

namespace bustrack {

  Server BusTrackDaemon::server_;

  int BusTrackDaemon::startServer() {
    // Create the server instance and start listening.
    if (!server_.listen()) {
      qCritical() << "Failed to listen on specified address and port.";
      qCritical() << "The error was: " << server_.errorString();
      return EXIT_LISTEN_FAILED;
    }

    // Print information about the server.
    std::cout << "BusTrack Server listening on " <<
      server_.serverAddress().toString().toStdString() << ":" <<
      server_.serverPort() << "..." << std::endl;
    
    return EXIT_OK;
  }

  int BusTrackDaemon::main(int argc, char* argv[]) {
    QCoreApplication app (argc, argv);
    app.setApplicationName("BusTrack Daemon");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("National University of Singapore");
    app.setOrganizationDomain("cs3249.comp.nus.edu.sg");

    // Read daemon settings.
    if (app.arguments().length() < ARGS_MIN_LENGTH) {
      printHelp();
      return EXIT_NOT_ENOUGH_ARGUMENTS;
    }

    // Interpret the arguments.
    QHostAddress server_address;
    if (!server_address.setAddress(app.arguments()[1])) {
      qCritical() << "Invalid server address specified.";
      return EXIT_INVALID_SERVER_ADDRESS;
    }

    bool conversion_ok = false;
    int server_port = app.arguments()[2].toInt(&conversion_ok);
    if (!conversion_ok || server_port <= 0 || server_port > 65535) {
      qCritical() << "Invalid server port specified.";
      return EXIT_INVALID_SERVER_PORT;
    }

    // Check for data directory path.
    QDir data_dir (".");
    if (app.arguments().length() > ARGS_MIN_LENGTH) {
      QString data_dir_name = app.arguments()[ARGS_MIN_LENGTH];
      data_dir.setPath(data_dir_name);

      if (!data_dir.exists() && !data_dir.mkdir(data_dir_name)) {
        qCritical() << "Data directory does not exist or is inaccessible.";
        return EXIT_DATA_DIR_NOT_EXIST;
      }
    }

    // Create the server instance and start listening.
    ServerContext context = server_.getContext();
    context.setListenAddress(server_address);
    context.setListenPort(server_port);
    context.setDataDir(data_dir);
    server_.setContext(context);

    if (startServer() != EXIT_OK) {
      return EXIT_LISTEN_FAILED;
    }

    // Run the event loop.
    return app.exec();
  }

  void BusTrackDaemon::printHelp() {
    using std::cout;
    using std::endl;

    cout << "BusTrack Bus Tracking System (Server)" << endl;
    cout << "Version 1.0" << endl;
    cout << endl;

    cout << "Copyright (C) 2014 Ko Wan Ling, Terence Then, Kek Yan Rong and Wong Yong Jie" << endl;
    cout << "CS3249 User Interface Development, National University of Singapore" << endl;
    cout << endl;

    cout << "Usage: ./bustrackd <address> <port> [data_dir]" << endl;
    cout << endl;

    cout << "    <address>   specifies the address that the server should listen on" << endl;
    cout << "    <port>      specifies the port that the server should listen on" << endl;
    cout << "    [data_dir]  is an optional parameter and determines the directory of the" << endl;
    cout << "                daemon. By default, this is set to the current directory" << endl;
    cout << endl;
  }

}

int main(int argc, char* argv[]) {
  return bustrack::BusTrackDaemon::main(argc, argv);
}

/* vim: set ts=2 sw=2 et: */
