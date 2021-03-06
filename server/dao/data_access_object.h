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

#ifndef BUSTRACK_DATA_ACCESS_OBJECT_H_
#define BUSTRACK_DATA_ACCESS_OBJECT_H_

namespace bustrack {

  /**
   * A base class for data access objects.
   * All data access objects should inherit from this base class.
   */
  class DataAccessObject {
  public:
    /**
     * Commits the internal state of the data access object to disk.
     */
    virtual void commit() = 0;

    /**
     * Restores the internal state of the data access object from disk.
     */
    virtual void rollback() = 0;
  };

}

#endif /* BUSTRACK_DATA_ACCESS_OBJECT_H_ */

/* vim: set ts=2 sw=2 et: */
