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

#include <sstream>
#include "message.h"

namespace bustrack {

  const std::string Message::TAG_NOOP ("NOOP");
  const std::string Message::TAG_INVALID ("INVALID");

  Message::Message() : tag_(TAG_NOOP) { }

  Message::Message(const std::string& tag, const QByteArray& payload) :
      tag_(tag),
      payload_(payload) {
  }

  Message Message::decodeFromString(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream str_ss (str);
    std::string token;
    
    while (std::getline(str_ss, token, ' ')) {
      tokens.push_back(token);
    }

    // Check number of tokens.
    if (tokens.empty() || tokens.size() > 2) {
      qWarning("Message: Unable to decode message, tokens.size() == %lu",
          tokens.size());
      return Message (TAG_INVALID, QByteArray());
    }

    // Take the first token and convert the string to type.
    Message message;
    message.setTag(tokens[0]);
    
    // Is there a payload?
    if (tokens.size() > 1) {
      QByteArray payload_base64 (tokens[1].c_str());
      message.setPayload(QByteArray::fromBase64(payload_base64));
    }

    return message;
  }

  std::string Message::getTag() const {
    return tag_;
  }

  void Message::setTag(const std::string& tag) {
    tag_ = tag;
  }

  QByteArray Message::getPayload() const {
    return payload_;
  }

  void Message::setPayload(const QByteArray& payload) {
    payload_ = payload;
  }

  std::string Message::toString() const {
    std::string encoded;

    encoded.append(tag_);
    encoded.append(" ");

    QByteArray payload_base64 = payload_.toBase64();
    encoded.append(std::string(payload_base64.data()));

    return encoded;
  }

}

/* vim: set ts=2 sw=2 et: */
