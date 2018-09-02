#include "emoncms.h"

#define HEATER_PIN 0
#define HEAT_PUMP_PIN 2

// will not send to emoncms
#define NO_EMONCMS

// how long to wait before a new signal is detected
#define SIGNAL_SENSITIVITY_MS 200

unsigned long lastUpdate;

// send every minute
#define SEND_MILLIS 60000

volatile unsigned long countHeater = 0;
volatile unsigned long countHeatPump = 0;
unsigned long lastHeaterUpdate = 0;
unsigned long lastHeatPumpUpdate = 0;

bool shouldUpdate(unsigned long &lastUpdate) {
    unsigned long now = millis();
    if (now - lastUpdate < SIGNAL_SENSITIVITY_MS)
        return false;

    lastUpdate = now;
    return true;
}

void doCountHeater() {
    if (!shouldUpdate(lastHeaterUpdate))
        return;

    countHeater++;
	Serial.println("Heater 100 Wh");
}

void doCountHeatPump() {
    if (!shouldUpdate(lastHeatPumpUpdate))
        return;

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
        #ifdef NO_EMONCMS
        Serial.println("Would send to emoncms.");
        #else
		sendPowerUsage(countHeater, countHeatPump);
        #endif
	}

    delay(1000);
}
