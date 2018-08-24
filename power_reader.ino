#include "emoncms.h"

#define HEATER_PIN 0
#define HEAT_PUMP_PIN 2

#define WH(X) (X)*100

volatile int countHeater = 0;
volatile int countHeatPump = 0;

void doCountHeater() {
    countHeater++;
}

void doCountHeatPump() {
    countHeatPump++;
}

void setup() {
    Serial.begin(115200);

    pinMode(HEATER_PIN, INPUT);
    pinMode(HEAT_PUMP_PIN, INPUT);

    attachInterrupt(HEATER_PIN, doCountHeater, RISING);
    attachInterrupt(HEAT_PUMP_PIN, doCountHeatPump, RISING);

    initWiFi();
}

void loop() {
    if (countHeater > 0 || countHeatPump > 0) {
        int countHeaterSend = countHeater;
        int countHeatPumpSend = countHeatPump;
        if (sendPowerUsage(WH(countHeaterSend), WH(countHeatPumpSend))) {
            countHeater -= countHeaterSend;
            countHeatPump -= countHeatPumpSend;
        }
    }

    delay(1000);
}
