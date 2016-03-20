
#include <Arduino.h>

#include "base.h"

//inicializace objektu komunikujicich s 433Mhz
RFM69 radio(SS, 4, true, 4);
//inicializace WebSocket serveru na portu 81
WebSocketsServer webSocket = WebSocketsServer(81);

//promena funguje jako buffer pro odesilani dat klientum
String payloadJson(100);

//nasloucha na pripojeni / odpojeni klientu k zakladne
void Base::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Se odpojil!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Se pripojil z adresy %d.%d.%d.%d URL: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            webSocket.sendTXT(num, "{\"result\":true}");
        }
            break;
        case WStype_TEXT:
            //todo
            break;
    }
}

void Base::setup() {
    //spousti seriovou komunikaci, baudrate 115200
    Serial.begin(115200);
    while (!Serial) { }
    Serial.println();
    Serial.println("Serial komunikace bezi");

    //spousti WiFi
    WiFi.mode(WIFI_AP);
    //nastavi nazev site a pristupove heslo
    WiFi.softAP("ShroedingersCan", "mf123456");
    Serial.println("WiFi sit 'ShroedingersCan' bezi");

    //spousti Server pro pripojeni klientu (prijem dat)
    webSocket.begin();
    //nastavuje naslouchani na pripojeni novych klientu
    webSocket.onEvent(webSocketEvent);

    //nastavuje 433 Mhz radio
    Serial.println("Nastavuji RFM69 radio");
    bool response = radio.initialize(FREQUENCY, BASE_ID, NETWORK_ID);
    if (response) {
        radio.setHighPower();
        radio.promiscuous(true);
        radio.encrypt(ENCRYPTKEY);
        Serial.println("RFM69 nastaveno a pripojeno, sifrovani aktivni");
    } else {
        Serial.println("Komunikace s RFM69 selhala, zkontrolujte zapojeni...");
        return;
    }

    //vsechno funguje, zarizeni je pripraveno
    ready = true;
}

void Base::loop() {

    //obslouzi pripojene klienty a vyridi komunikaci
    webSocket.loop();

    //pokud vsechno funguje
    if (ready) {
        //pokud nam dorazila celá zpráva přes 433Mhz
        if (radio.receiveDone()) {

            //pripravime si zpravu pro klienty
            payloadJson = "{\"sender\":";
            payloadJson += (int) radio.SENDERID;
            payloadJson += ",\"rssi\":";
            payloadJson += radio.RSSI;
            payloadJson += ",\"length\":";
            payloadJson += radio.DATALEN;
            payloadJson += ",\"data\":";
            for (byte i = 0; i < radio.DATALEN; i++) {
                payloadJson += (char) radio.DATA[i];
            }
            payloadJson += "}";

            //odesleme zpravu klientum
            webSocket.broadcastTXT(payloadJson);

            //pokud si vysilac vyzadal potvrzeni doruceni, odesleme ho
            if (radio.ACK_REQUESTED) {
                radio.sendACK();
            }
        }
    } else {
        //zarizeni nefunguje, nic nedelame
        Serial.println("Zarizeni neni ve funkcnim stavu, je vyzadovana akce");
        delay(1000);
    }
}
