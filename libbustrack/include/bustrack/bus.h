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

#ifndef BUSTRACK_BUS_H_
#define BUSTRACK_BUS_H_

#include "bus_service.h"
#include "bus_stop.h"

namespace bustrack {

  class Bus {
  public:
    Bus();

    std::string getId();
    void setId(const std::string& id);

    BusService getService();
    void setService(BusService service);
   
    BusStop getDestination();
    void setDestination(BusStop destination);
    
    unsigned int getOccupancy();
    void setOccupancy(unsigned int occupancy);
    
    float getLatitude();
    void setLatitude(float latitude);
    
    float getLongitude();
    void setLongitude(float longitude);

    static Bus fromString(const std::string& serialized);
    static Bus fromStringAll(const std::string& serialized);
    std::string toString();
    std::string toStringAll();

  private:
    static const std::string TAG;
    static const int NUM_SERIALIZED_FIELDS = 1;
    static const int NUM_ALL_FIELDS = 4;

    std::string id_;
    BusService service_;
    BusStop destination_;
    
    unsigned int occupancy_;
    float longitude_;
    float latitude_;
  };

}

#endif /* BUSTRACK_BUS_H_ */

/* vim: set ts=2 sw=2 et: */
