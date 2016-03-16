//
// Created by Filip Prochazka (@filipproch).
//

#include <Arduino.h>

#include "MyRFM69.h"
#include <CanSat.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <SPI.h> //must be here

#define FREQUENCY     RF69_433MHZ

RFM69 radio(SS, 4, true, 4);
WebSocketsServer webSocket = WebSocketsServer(81);

String payloadJson(100);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            webSocket.sendTXT(num, "{\"result\":true}");
        }
            break;
        case WStype_TEXT:
            //todo
            break;
    }
}

void setup() {
    //enable serial (debugging only)
    Serial.begin(115200);
    while (!Serial) { }
    Serial.println();
    Serial.println("Serial communication is up");

    //initialize WiFi
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ShroedingersCan", "mf123456");
    Serial.println("WiFi AP initialized");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    //initialize RFM69 radio
    Serial.println("Initializing RFM69 radio");
    bool response = radio.initialize(FREQUENCY, BASE_ID, NETWORK_ID);
    Serial.println(response);
    radio.setHighPower();
    radio.promiscuous(true);
    radio.encrypt(ENCRYPTKEY);
    Serial.println("Radio initialized, encryption enabled");
}

void loop() {
    webSocket.loop();
    if (radio.receiveDone()) {

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
        webSocket.broadcastTXT(payloadJson);

        if (radio.ACK_REQUESTED) {
            radio.sendACK();
        }
    }
}
