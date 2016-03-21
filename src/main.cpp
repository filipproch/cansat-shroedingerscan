
#include <Arduino.h>

#if DEVTYPE == 0
#include "base.h"

Base base;
#elif DEVTYPE == 1
#include "cansat.h"

CanSat cansat;
#endif

void setup() {
    #if DEVTYPE == 0
        base.setup();
    #elif DEVTYPE == 1
        cansat.setup();
    #endif
}

void loop() {
    #if DEVTYPE == 0
        base.run();
    #elif DEVTYPE == 1
        cansat.loop();
    #endif
}