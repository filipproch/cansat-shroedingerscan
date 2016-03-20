
#include "cansat.h"

#define VELIKOST_BUFFERU_ZRAVY 50

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

void setup() {
    //spustime seriovou komunikace
    Serial.begin(115200);
    while (!Serial) { }
    Serial.println();
    Serial.println("Serial komunikace bezi");

    //spustime I2C sbernici (jsou na ni pripojene senzory)
    Wire.begin();

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
        CanSat::bmp180_ready = true;
    } else {
        Serial.println("BMP180 selhal");
    }

    //nastavuji akcelerometr/gyroskop
    Serial.println("Nastavuji MPU6050 sensor");
    accelgyro.initialize();
    if (accelgyro.testConnection()) {
        Serial.println("MPU6050 pripraven");
        CanSat::mpu6050_ready = true;
    } else {
        Serial.println("MPU6050 failed");
    }

    //odesilam stav zarizeni
    Serial.println("Odesilam info zakladne");
    char initStav[50];
    int velikost = sprintf(initStav, "INIT#TT:%d#AG:%d", CanSat::bmp180_ready ? 1 : 0, CanSat::mpu6050_ready ? 1 : 0);
    radio.send(BASE_ID, initStav, velikost);

    Serial.println("Spusten a pripraven k mereni");
}

void loop() {

    //ziskani dat od akcelerometru / gyroskopu
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
        int delkaDat = sprintf(payload, "DATA#T:%f#P:%f", T, P);
        //odeslani dat do zakladny
        radio.send(BASE_ID, payload, delkaDat);
    }

    //chvilku pockame
    delay(100);
}
