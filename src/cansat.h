
#include <Wire.h>
#include <MyRFM69.h>
#include <SFE_BMP180.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

#include "I2Cdev.h"
#include "MPU6050.h"

#include "konstanty.h"

#ifndef CANSAT_CANSAT_H
#define CANSAT_CANSAT_H

#define VELIKOST_BUFFERU_ZRAVY 50

class CanSat {
    public:
        void setup();
        void loop();

        CanSat();
private:

        bool bmp180_ready;
        bool mpu6050_ready;

        // pomocne objekty pro komunikaci se senzory
        // 433Mhz radio
        RFM69 radio;
        // BMP180 tlak + teplota senzor
        SFE_BMP180 pressure;
        // MPU6050 akcelerometr + gyroskop
        MPU6050 accelgyro;

        // pripravene promene pro akcelerometr a gyroskop
        int16_t ax, ay, az;
        int16_t gx, gy, gz;

        // buffer pro odesilanou zpravu
        char payload[VELIKOST_BUFFERU_ZRAVY];
};

#endif //CANSAT_CANSAT_H
