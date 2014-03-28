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

#ifndef BUSTRACK_BUS_SERVICE_H_
#define BUSTRACK_BUS_SERVICE_H_

#include <string>

namespace bustrack {

  class BusService {
  public:
    std::string getCode();
    void setCode(std::string code);

  private:
    std::string code_;
  };

}

#endif /* BUSTRACK_BUS_SERVICE_H_ */

/* vim: set ts=2 sw=2 et: */
