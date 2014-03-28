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

#include <fstream>
#include <QDebug>
#include "bus_dao.h"

namespace bustrack {

  const QString BusDAO::DATA_FILE_NAME = "buses.txt";

  BusDAO::BusDAO(const QDir& data_dir) :
      data_dir_(data_dir) {
    data_file_path_ = data_dir_.filePath(DATA_FILE_NAME);
    rollback();
  }

  BusDAO::~BusDAO() {
    commit();
  }

  void BusDAO::commit() {
    using std::fstream;

    fstream file_stream (data_file_path_.toStdString(), std::ios::out);
    if (file_stream.fail()) {
      qWarning() << "Failed to write bus list to file.";
      return;
    }

    // Write out buses, line by line.
    for (std::pair<std::string, Bus> bus_pair : buses_) {
      Bus bus = bus_pair.second;
      file_stream << bus.getPlateNumber() << std::endl;
    }

    file_stream.close();
  }

  void BusDAO::rollback() {
    buses_.clear();

    // Check if the data file exists.
    if (!data_dir_.exists(DATA_FILE_NAME)) {
      return;
    }

    std::fstream file_stream (data_file_path_.toStdString(), std::ios::in);
    if (file_stream.fail()) {
      throw std::runtime_error("Failed to read bus list from file.");
    }

    // Read in bus stops, line by line.
    // Each line should have the format: stop_id|stop_name|lat|lng
    for (std::string line; getline(file_stream, line);) {
      QString q_line (line.c_str());
      QStringList tokens = q_line.split("|");

      if (tokens.size() != NUM_FIELDS) {
        qWarning() << "One of the buses in file has insufficient number of "
          "fields.";
        continue;
      }

      // Create the bus stop object.
      Bus bus;
      bus.setPlateNumber(tokens[0].toStdString());

      // Stuff the bus stop object into our list.
      buses_.insert(std::make_pair(bus.getPlateNumber(), bus));
    }
  }

  std::vector<Bus> BusDAO::getBuses() const {
    std::vector<Bus> buses;
    for (std::pair<std::string, Bus> bus_pair : buses_) {
      Bus bus = bus_pair.second;
      buses.push_back(bus);
    }

    return buses;
  }

  Bus BusDAO::getBus(const std::string& plate_number) const {
    return buses_.at(plate_number);
  }

  bool BusDAO::busExists(const std::string& plate_number) const {
    return buses_.find(plate_number) != buses_.end();
  }

  bool BusDAO::createBus(Bus bus) {
    if (busExists(bus.getPlateNumber())) {
      return false;
    }

    buses_.insert(std::make_pair(bus.getPlateNumber(), bus));
    return true;
  }

  bool BusDAO::replaceBus(Bus bus) {
    if (!busExists(bus.getPlateNumber())) {
      return false;
    }

    buses_.insert(std::make_pair(bus.getPlateNumber(), bus));
    return true;
  }

  bool BusDAO::removeBus(const std::string& plate_number) {
    if (!busExists(plate_number)) {
      return false;
    }

    buses_.erase(buses_.find(plate_number));
    return true;
  }

}

/* vim: set ts=2 sw=2 et: */
