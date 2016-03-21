
#include "MyRFM69.h"
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>

#include "konstanty.h"

#ifndef CANSAT_BASE_H
#define CANSAT_BASE_H

class Base {
    public:
        void setup();
        void loop();
        Base();
    private:
        bool ready;

        //inicializace objektu komunikujicich s 433Mhz
        RFM69 radio;
        //promena funguje jako buffer pro odesilani dat klientum
        String payloadJson;
        //inicializace WebSocket serveru na portu 81
        WebSocketsServer* webSocket;
};

#endif //CANSAT_BASE_H
