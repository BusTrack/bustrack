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

#ifndef BUSTRACK_BUS_STOP_H_
#define BUSTRACK_BUS_STOP_H_

#include "waypoint.h"

namespace bustrack {

  class BusStop : public Waypoint {
  public:
    std::string getId();
    void setId(const std::string& id);

    std::string getName();
    void setName(const std::string& name);
    
    unsigned int getOccupancy();
    void setOccupancy(unsigned int occupancy);

    static BusStop fromString(const std::string& serialized);
    static BusStop fromStringAll(const std::string& serialized);
    std::string toString();
    std::string toStringAll();

  private:
    static const std::string TAG;
    static const int NUM_SERIALIZED_FIELDS = 4;
    static const int NUM_ALL_FIELDS = 5;

    std::string id_;
    std::string name_;
    unsigned int occupancy_;
  };

}

#endif /* BUSTRACK_BUS_STOP_H_ */

/* vim: set ts=2 sw=2 et: */
