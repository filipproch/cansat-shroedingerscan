
#include <Arduino.h>

#include "base.h"
#include "cansat.h"

void setup() {
    #if DEVTYPE == 0
        Base::setup();
    #elif DEVTYPE == 1
        CanSat::setup();
    #endif
}

void loop() {
    #if DEVTYPE == 0
        Base::loop();
    #elif DEVTYPE == 1
        CanSat::loop();
    #endif
}