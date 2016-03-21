
#include "cansat.h"

CanSat::CanSat() : radio(SS, 0, true, 0), pressure(), accelgyro() {
}

void CanSat::setup() {
    //vychozi stav senzoru
    bmp180_ready = false;
    mpu6050_ready = false;

    //spustime seriovou komunikace
    Serial.begin(115200);
    while (!Serial) { }
    Serial.println();
    Serial.println("[CANSAT] Serial komunikace bezi");

    //vypneme WiFi (setrime energii)
    WiFi.mode(WIFI_OFF);

    //spustime I2C sbernici (jsou na ni pripojene senzory)
    Wire.begin(D2, D1);

    //nastavime 433Mhz radio
    Serial.println("nastavuji RFM69 radio");
    bool response = radio.initialize(FREQUENCY, CANSAT_ID, NETWORK_ID);
    if (response) {
        radio.setHighPower();
        radio.encrypt(ENCRYPTKEY);
        Serial.println("RFM69 nastaveno a pripojeno, sifrovani aktivni");
    } else {
        Serial.println("Komunikace s RFM69 selhala, zkontrolujte zapojeni...");
        return;
    }

    //nastavuji BMP180
    Serial.println("Nastavuji BMP180 sensor");
    if (pressure.begin()) {
        Serial.println("BMP180 pripraven");
        bmp180_ready = true;
    } else {
        Serial.println("BMP180 selhal");
    }

    //nastavuji akcelerometr/gyroskop
    Serial.println("Nastavuji MPU6050 sensor");
    accelgyro.initialize();
    if (accelgyro.testConnection()) {
        Serial.println("MPU6050 pripraven");
        mpu6050_ready = true;
    } else {
        Serial.println("MPU6050 failed");
    }

    //odesilam stav zarizeni
    Serial.println("Odesilam info zakladne");
    char initStav[VELIKOST_BUFFERU_ZRAVY];
    sprintf(initStav, "INIT#TT:%d#AG:%d", bmp180_ready ? 1 : 0, mpu6050_ready ? 1 : 0);
    radio.send(BASE_ID, initStav, VELIKOST_BUFFERU_ZRAVY);

    Serial.println("Spusten a pripraven k mereni");
}

void CanSat::loop() {

    //ziskani dat od akcelerometru / gyroskopu
    if(bmp180_ready)
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    //priprava promenych
    char status;
    double T = -999, P = -999;

    //spusti mereni teploty
    status = pressure.startTemperature();
    if (status != 0) { // mereni teploty zacalo, v promene status je ulozeno jak dlouho mame cekat na dokonceni
        //cekame na dokonceni
        delay(status);
        //teplota je zmerena, precteme a ulozime ji do promene T
        pressure.getTemperature(T);
        //zahajime mereni tlaku
        status = pressure.startPressure(3);
        if (status != 0) {//mereni bylo zahajeno uspesne
            //pockame na dokonceni
            delay(status);
            //ulozime si do promene P zmereny tlak
            pressure.getPressure(P, T);
        }

        //formatovani dat
        sprintf(payload, "DATA#T:%f#P:%f", T, P);
        //odeslani dat do zakladny
        radio.send(BASE_ID, payload, VELIKOST_BUFFERU_ZRAVY);
    }

    //chvilku pockame
    delay(100);
}
