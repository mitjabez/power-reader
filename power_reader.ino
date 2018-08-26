#include "emoncms.h"

#define HEATER_PIN 0
#define HEAT_PUMP_PIN 2

unsigned long lastUpdate;

// send every minute
#define SEND_MILLIS 60000

#define WH(X) (X)*100

volatile unsigned long countHeater = 0;
volatile unsigned long countHeatPump = 0;

void doCountHeater() {
    countHeater++;
	Serial.println("Heater 100 Wh");
}

void doCountHeatPump() {
    countHeatPump++;
	Serial.println("Heat pump 100 Wh");
}

void setup() {
    Serial.begin(115200);

    pinMode(HEATER_PIN, INPUT);
    pinMode(HEAT_PUMP_PIN, INPUT);

    attachInterrupt(HEATER_PIN, doCountHeater, FALLING);
    attachInterrupt(HEAT_PUMP_PIN, doCountHeatPump, FALLING);

    initWiFi();
}

void printState() {
    Serial.print("Heat pump: ");
    Serial.print(countHeatPump);
    Serial.print(" Heater: ");
    Serial.println(countHeater);
}

void loop() {
	printState();
	
	unsigned long now = millis();
	if (now - lastUpdate > SEND_MILLIS) {
		lastUpdate = now;
		sendPowerUsage(countHeater, countHeatPump);
	}

    delay(1000);
}
