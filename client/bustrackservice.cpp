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

#include <QStringList>
#include "bustrackservice.h"

namespace bustrack {

const std::string BusTrackService::TAG ("BusTrackService");

BusTrackService::BusTrackService(QObject*) : nextRequestId_(0)
{
    socket_ = new QTcpSocket();
    connectSignals_();

    // Connect to the socket.
    socket_->connectToHost("127.0.0.1", 8080);
}

void BusTrackService::getBusServices() {
    qDebug("%s: Invoked getBusServices()", TAG.c_str());
    Message request;
    request.setId(nextRequestId_);
    request.setTag("BUS_SERVICES");
    sendRequest_(request, BUS_SERVICES_REQUEST);
}

void BusTrackService::getBusStops() {
    qDebug("%s: Invoked getBusStops()", TAG.c_str());
    Message request;
    request.setId(nextRequestId_);
    request.setTag("BUS_STOPS");
    sendRequest_(request, BUS_STOPS_REQUEST);
}

void BusTrackService::getBuses() {
    qDebug("%s: Invoked getBuses()", TAG.c_str());
    Message request;
    request.setId(nextRequestId_);
    request.setTag("BUSES");
    sendRequest_(request, BUSES_REQUEST);
}

void BusTrackService::handleError(QAbstractSocket::SocketError) {
    qWarning("%s: Error connecting to server: %s", TAG.c_str(),
            socket_->errorString().toStdString().c_str());
    emit error();
}

void BusTrackService::handleConnected() {
    qDebug("%s: Received connect() signal from QTcpSocket", TAG.c_str());
    emit connected();
}

void BusTrackService::handleReadyRead() {
    // We have some response!
    char buf[16384];
    qint64 line_length = socket_->readLine(buf, sizeof(buf));
    if (line_length != -1) {
      qDebug("%s: %lld bytes received", TAG.c_str(), line_length);
    }

    QString line = QString(buf).trimmed();
    Message message = Message::decodeFromString(line.toStdString());
    qDebug("%s: Message with tag %s id %d received", TAG.c_str(),
        message.getTag().c_str(), message.getId());

    // Throw it into the sorting house!
    unsigned int message_id = message.getId();
    if (requestQueue_.find(message_id) == requestQueue_.end()) {
        qWarning("%s: Unsolicited message received, dropping!", TAG.c_str());
        return;
    }

    QString decoded_payload (message.getPayload());
    decoded_payload = decoded_payload.trimmed();
    RequestType message_type = requestQueue_.at(message_id);
    switch (message_type) {
        case BUS_SERVICES_REQUEST: {
            QStringList bus_services_line = decoded_payload.split("\n");
            std::vector<BusService> bus_services;
            for (QString bus_service : bus_services_line) {
                bus_services.push_back(BusService::fromString(
                            bus_service.toStdString()));
            }

            emit getBusServicesComplete(bus_services);
        },
        case BUS_STOPS_REQUEST: {
            QStringList bus_stops_line = decoded_payload.split("\n");
            std::vector<BusStop> bus_stops;
            for (QString bus_stop : bus_stops_line) {
                bus_stops.push_back(BusStop::fromStringAll(bus_stop.toStdString()));
            }

            emit getBusStopsComplete(bus_stops);
            break;
        }
        case BUSES_REQUEST: {
            QStringList buses_line = decoded_payload.split("\n");
            std::vector<Bus> buses;
            for (QString bus : buses_line) {
                buses.push_back(Bus::fromStringAll(bus.toStdString()));
            }

            emit getBusesComplete(buses);
        },
        default:
            break;
    }
}

void BusTrackService::connectSignals_() {
    // Connect the socket events to this class.
    connect(
        socket_,
        SIGNAL(error(QAbstractSocket::SocketError)),
        this,
        SLOT(handleError(QAbstractSocket::SocketError))
    );

    connect(
        socket_,
        SIGNAL(connected()),
        this,
        SLOT(handleConnected())
    );

    connect(
        socket_,
        SIGNAL(readyRead()),
        this,
        SLOT(handleReadyRead())
    );
}

void BusTrackService::sendRequest_(Message request, RequestType type) {
    // Add this request to the request queue.
    requestQueue_.insert(std::make_pair(request.getId(), type));
    
    // Send off!
    std::string request_string = request.toString();
    socket_->write(request_string.c_str(), request_string.length());
    socket_->putChar('\n');
    socket_->flush();

    // Increment our ID (;
    nextRequestId_++;
}

}

// vim: set ts=4 sw=4 et:
