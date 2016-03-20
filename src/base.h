
#include "MyRFM69.h"
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

#include "konstanty.h"

#ifndef CANSAT_BASE_H
#define CANSAT_BASE_H

class Base {
    public:
        static void setup();
        static void loop();

        static bool ready;
    private:
        void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
};

#endif //CANSAT_BASE_H
