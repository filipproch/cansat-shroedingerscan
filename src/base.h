
#include "MyRFM69.h"
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <FS.h>

#include "konstanty.h"

#ifndef CANSAT_BASE_H
#define CANSAT_BASE_H

class Base: private WebSocketsServer {
    public:
        void setup();
        void run();
        Base();
    private:
        bool ready;

        //inicializace objektu komunikujicich s 433Mhz
        RFM69 radio;
        //promena funguje jako buffer pro odesilani dat klientum
        String payloadJson;

        void runCbEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
};

#endif //CANSAT_BASE_H
