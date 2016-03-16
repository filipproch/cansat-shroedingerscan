//
// Created by Filip Prochazka (@filipproch).
//

#include <Arduino.h>

#include <Wire.h>
#include <MyRFM69.h>
#include <CanSat.h>
#include <SFE_BMP180.h>
#include <SPI.h>

#include "I2Cdev.h"
#include "MPU6050.h"

#define FREQUENCY     RF69_433MHZ
#define PAYLOAD_SIZE 50
//
//RFM69 radio;
//SFE_BMP180 pressure;
//MPU6050 accelgyro;
//
//int16_t ax, ay, az;
//int16_t gx, gy, gz;
//
//char payload[PAYLOAD_SIZE];
//
//void setup() {
//    //enable serial (debugging only)
//    Serial.begin(9600);
//    while (!Serial) { }
//    Serial.println();
//    Serial.println("Serial communication is up");
//
//    //initialize I2C bus
//    Wire.begin();
//
//    //initialize RFM69 radio
//    Serial.println("Initializing RFM69 radio");
//    bool response = radio.initialize(FREQUENCY, CANSAT_ID, NETWORK_ID);
//    if (response) {
//        radio.setHighPower();
//        radio.encrypt(ENCRYPTKEY);
//        Serial.println("Radio initialized, encryption enabled");
//    } else {
//        Serial.println("Radio init failed");
//        return;
//    }
//
//    //initialize BMP180
//    Serial.println("Initializing BMP180 sensor");
//    if (pressure.begin()) {
//        Serial.println("BMP180 ready");
//    } else {
//        Serial.println("BMP180 failed");
//    }
//
//    //initialize Accel/Gyro
//    Serial.println("Initializing MPU6050 sensor");
//    accelgyro.initialize();
//    if (accelgyro.testConnection()) {
//        Serial.println("MPU6050 ready");
//    } else {
//        Serial.println("MPU6050 failed");
//    }
//
//    //send initial radio ping
//    Serial.println("Sending BOOT to BASE");
//    radio.send(BASE_ID, "BOOT", 4);
//    Serial.println("Initialized, running loop...");
//}
//
//void loop() {
//
//    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//
//    char status;
//    double T = -999, P = -999, p0, a;
//
//    status = pressure.startTemperature();
//    if (status != 0) {
//        //wait for temperature to measure
//        delay(status);
//        //read measured temp
//        status = pressure.getTemperature(T);
//        //print temp
////        Serial.print("Temp: ");
////        Serial.print(T, 2);
////        Serial.println(" C");
//
//        status = pressure.startPressure(3);
//        if (status != 0) {
//            delay(status);
//            status = pressure.getPressure(P, T);
//            if (status != 0) {
////                Serial.print("Absolute pressure: ");
////                Serial.print(P, 2);
////                Serial.println(" mb");
//            }
//        }
//
//        sprintf(payload, "T:%dP:%d", (int) T, (int) P);
//        Serial.println(payload);
//        radio.send(BASE_ID, payload, PAYLOAD_SIZE);
//    }
//
//    delay(1000);
//}
