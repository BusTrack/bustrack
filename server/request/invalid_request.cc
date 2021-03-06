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

#include <QTcpSocket>
#include "invalid_request.h"

namespace bustrack {

  void InvalidRequest::process(Message request, Message response,
      QTcpSocket* socket) {
    // Create the response payload.
    std::string payload (request.getTag());
    payload.append("_ERROR");

    // Create the response message.
    response.setTag(payload);
    response.setPayload("Invalid request.");

    // Send the response message.
    std::string response_message = response.toString();
    socket->write(response_message.data(), response_message.size());
    socket->putChar('\n');
    socket->flush();
  }

}

/* vim: set ts=2 sw=2 et: */
