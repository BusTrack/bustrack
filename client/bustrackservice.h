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

#ifndef BUSTRACKSERVICE_H
#define BUSTRACKSERVICE_H

#include <unordered_map>
#include <QTcpSocket>

#include "bustrack/message.h"
#include "bustrack/bus.h"
#include "bustrack/bus_stop.h"

namespace bustrack {

class BusTrackService : public QObject
{
    Q_OBJECT

public:
    BusTrackService(QObject* parent = 0);
    
    void getBusStops();
    void getBuses();

signals:
    void connected();
    void error();

    void getBusStopsComplete(std::vector<BusStop> busStops);
    void getBusesComplete(std::vector<Bus> buses);

private slots:
    void handleError(QAbstractSocket::SocketError socketError);
    void handleConnected();
    void handleReadyRead();

private:
    enum RequestType {
        BUS_STOPS_REQUEST,
        BUSES_REQUEST
    };

    static const std::string TAG;
    QTcpSocket* socket_;
    unsigned int nextRequestId_;
    std::unordered_map<unsigned int, RequestType> requestQueue_;

    void connectSignals_();
    void sendRequest_(Message request, RequestType type);
};

}

#endif // BUSTRACKSERVICE_H

// vim: set ts=4 sw=4 et:
