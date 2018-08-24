#include <ESP8266WiFi.h>

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

bool ledState;
volatile int count = 0;

void doCount() {
	count++;
}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(2, INPUT);
	attachInterrupt(2, doCount, RISING);


}

void sendCount() {
	Serial.print("Sending ");
	Serial.println(count);

	WiFiClientSecure client;
	client.setTimeout(3000);
	const char* host = "emoncms.org";
	int httpsPort = 443;
	Serial.print("connecting to ");
	Serial.println(host);
	if (!client.connect(host, httpsPort)) {
		Serial.println("connection failed");
		return;
	}

	int sendCount = count;  
	client.print(String("GET ") + "/input/post?node=test&fulljson={\"counter\":" + sendCount+"}&apikey=***REMOVED***" + " HTTP/1.1\r\n" +
			"Host: " + host + "\r\n" +
			"User-Agent: BuildFailureDetectorESP8266\r\n" +
			"Connection: close\r\n\r\n");

	int i = 0;
	Serial.println("Request sent. Waiting for response...");
	unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 5000) {
			Serial.println(">>> Client Timeout !");
			client.stop();
			return;
		}
	}

	Serial.println("Got it! Reading response...");
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

	Serial.println(String("Success: ") + isSuccess);

	Serial.println();
	Serial.println("closing connection");


	count -= sendCount;
}

void loop() {
	//  digitalWrite(LED_BUILTIN, ledState);
	//  ledState = !ledState;
	//  delay(250);
	Serial.println("ping");
	if (count > 0) {
		sendCount();
	}

	delay(1000);
}
