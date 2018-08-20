char[] baseUrl = "https://emoncms.org/input/post?node=estia&apikey=***REMOVED***&fulljson={\""
char[200] destinationUrlBuff;

int PowerSender::Initialize() {
    Serial.println(F("Initializing WiFi:"));
}

void createDestinationUrlBuff(const char* id, unsigned int wattHours) {
    char[6] wattHoursBuff;
    itoa(wattHours, wattHoursBuffer, 10);

    strcpy(destinationUrl, baseUrl);
    strcat(destinationUrl, id);
    strcat(destinationUrl, "\":");
    strcat(destinationUrl, wattHoursBuff)
    strcat(destinationUrl, "}");
}

bool PowerSender::SendPower(const char* id, unsigned int wattHours) {
    createDestinationUrlBuff(id, wattHours);
    Serial.print(F("Sending watt hours update: "));
    Serial.println(destinationUrlBuff);
}
