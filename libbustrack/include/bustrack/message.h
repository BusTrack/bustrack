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

#ifndef BUSTRACK_MESSAGE_H_
#define BUSTRACK_MESSAGE_H_

#include <QByteArray>

namespace bustrack {

  /**
   * Represents a message used in the BusTrack protocol.
   *
   * Each message consists of a message type and message content. The message
   * type is an ASCII string no more than 64 bytes long. The message payload
   * is a base64-encoded string.
   *
   * There is no specific limit on the maximum permissible length of the
   * payload. However, in practice, it is safer to limit the length of the
   * entire message.
   *
   * Both parts are separated by one or more space characters. 
   */
  class Message {
  public:
    static const std::string TAG_NOOP;
    static const std::string TAG_INVALID;

    /**
     * Constructs a new empty message.
     *
     * By default, the message tag is set to NOOP. This message carries no
     * payload.
     */
    Message();

    /**
     * Constructs a message with the specified tag and payload.
     *
     * @param tag The tag of the message
     * @param payload The payload of the message
     */
    Message(const std::string& tag, const QByteArray& payload);

    /**
     * Factory method that creates a message by decoding it from a string.
     *
     * If the decoding fails, a warning message is printed via qWarning() and
     * a message of type MESSAGE_INVALID will be returned.
     *
     * @param str The string to construct the message from
     */
    static Message decodeFromString(const std::string& str);

    /**
     * Returns the tag of message.
     */
    std::string getTag() const;

    /**
     * Sets the tag of message.
     * @param tag The tag of the message
     */
    void setTag(const std::string& tag);

    /**
     * Returns the payload of the message.
     */
    QByteArray getPayload() const;

    /**
     * Sets the payload of the message.
     * @param payload The payload of the message.
     */
    void setPayload(const QByteArray& payload);

    /**
     * Encodes this message into a string.
     *
     * @return The encoded message.
     */
    std::string toString() const;

  private:
    std::string tag_;
    QByteArray payload_;
  };

}

#endif /* BUSTRACK_MESSAGE_H_ */

/* vim: set ts=2 sw=2 et: */
