
#include <Wire.h>
#include <MyRFM69.h>
#include <SFE_BMP180.h>
#include <SPI.h>

#include "I2Cdev.h"
#include "MPU6050.h"

#include "konstanty.h"

#ifndef CANSAT_CANSAT_H
#define CANSAT_CANSAT_H

class CanSat {
    public:
        static void setup();
        static void loop();

        static bool bmp180_ready;
        static bool mpu6050_ready;
};

#endif //CANSAT_CANSAT_H
