#include <ESP8266WiFi.h>

#define SSID "***REMOVED***"
#define KEY "***REMOVED***"

#define TCP_TIMEOUT 3000
#define HOST "emoncms.org"
#define PORT 443
#define APIKEY "***REMOVED***"

#define NODE "estia"


void initWiFi() {
    WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

	gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
			{
			Serial.print(F("Station connected, IP: "));
			Serial.println(WiFi.localIP());
			});

	disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event)
			{
			Serial.println(F("Station disconnected"));
			});

	Serial.printf("Connecting to %s ...\n", "wifi");
	WiFi.begin(SSID, KEY);
}

bool sendPowerUsage(int wattHoursHeater, int wattHoursHeatPump) {
	Serial.print(F("Sending power usage. Heat pump: "));
	Serial.print(wattHoursHeatPump);
    Serial.print(F(" Wh, Heater: "));
	Serial.print(wattHoursHeater);
    Serial.println(F(" Wh."));

	WiFiClientSecure client;
	client.setTimeout(TCP_TIMEOUT);
	Serial.print(F("Connecting to "));
	Serial.println(HOST);
	if (!client.connect(HOST, PORT)) {
		Serial.println(F("connection failed"));
		return false;
	}

	client.print(String("GET ") 
                 + "/input/post?node=" + NODE + "&fulljson={"
                 + "\"wattHoursHeatPump\":" + wattHoursHeatPump + "," 
                 + "\"wattHoursHeater\":" + wattHoursHeater
                 + "}&apikey=" + APIKEY + " HTTP/1.1\r\n"
                 + "Host: " + HOST + "\r\n"
                 + "User-Agent: BuildFailureDetectorESP8266\r\n"
                 + "Connection: close\r\n\r\n");

	int i = 0;
	Serial.println(F("Request sent. Waiting for response..."));
	unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 5000) {
			Serial.println(F(">>> Client Timeout !"));
			client.stop();
			return false;
		}
	}

	Serial.println(F("Reading response..."));
	// Read all the lines of the reply from server and print them to Serial
	bool isSuccess = false;
	while (client.available()) {
		String response = client.readStringUntil('\r');
		// HTTP/1.1 200 OK
		if (response.indexOf("200 OK") >= 0) {
			isSuccess = true;
		}
		Serial.print(response);
		if (response == "\n") {
			break;
		}
	}

    if (!isSuccess) {
        Serial.println(F("Invalid responses."));
    }

	Serial.println();
	Serial.println("Closing connection");

    return isSuccess;
}
