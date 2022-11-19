#ifndef _SENDER_H
#define _SENDER_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

WiFiClient espClient;
PubSubClient pubSub(espClient);

void initWiFi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, KEY);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnect() {
    while (!pubSub.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (pubSub.connect(MQTT_ID, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(pubSub.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void initMqtt() {
    pubSub.setServer(MQTT_SERVER, 1883);
    pubSub.setCallback(callback);
}

void sendPowerUsage(unsigned long countHeater, unsigned long countHeatPump) {
    char json[100];
    sprintf(json,"{\"countHeater\":%d, \"countHeatPump\":%d}", countHeater, countHeatPump);
    Serial.print("Publishing to ");
    Serial.println(TOPIC_ID_HEATPUMP);
    Serial.println(json);
    pubSub.publish(TOPIC_ID_HEATPUMP, json);
}

void loopSender() {
    if (!pubSub.connected()) {
        reconnect();
    } else {
        pubSub.loop();
    }
}

#endif
