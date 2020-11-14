#include "sender.h"

#define HEAT_PUMP_PIN 0
#define HEATER_PIN 2

//#define NO_SEND

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

// have to add ICACHE_RAM_ATTR or it will crash with "esp8266 isr not in iram"
// https://arduino-esp8266.readthedocs.io/en/latest/faq/a02-my-esp-crashes.html?highlight=isr%20icache_RAM_attr#other-causes-for-crashes
void ICACHE_RAM_ATTR doCountHeater() {
    if (!shouldUpdate(lastHeaterUpdate))
        return;

    countHeater++;
	Serial.println("Heater 100 Wh");
}

void ICACHE_RAM_ATTR doCountHeatPump() {
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
    initMqtt();
}

void printState() {
    Serial.print("Heat pump: ");
    Serial.print(countHeatPump);
    Serial.print(" Heater: ");
    Serial.println(countHeater);
}

void loop() {
	unsigned long now = millis();
	if (now - lastUpdate > SEND_MILLIS) {
        printState();
		lastUpdate = now;
        #ifdef NO_SEND
        Serial.println("Would send.");
        #else
		sendPowerUsage(countHeater, countHeatPump);
        #endif
	}

    loopSender();
    delay(1000);
}
