#define WATT_HR_PER_PULSE 100

#define POWER_PIN_COMPRESSOR 0
#define POWER_PIN_HEATER 2

volatile unsigned int pulsesCompressor = 0;
volatile unsigned int pulsesHeater = 0;

void setup() {
	pinMode(POWER_PIN_COMPRESSOR, INPUT);
	attachInterrupt(digitalPinToInterrupt(POWER_PIN_COMPRESSOR), countPulseCompressor, RISING);
	attachInterrupt(digitalPinToInterrupt(POWER_PIN_HEATER), countPulseHeater, RISING);
}

void countPulseCompressor() {
	pulsesCompressor++;
}

void countPulseCompressor() {
	pulsesHeater++;
}

void loop() {
	unsigned int pulsesSend;

	if (pulsesCompressor > 0) {
		pulsesSend = pulsesCompressor;
		if (powerSender.SendPower("compressorWattHours", pulsesSend)) {
			pulsesCompressor -= pulsesSend;
		}
	}

	if (pulsesHeater > 0) {
		pulsesSend = pulsesHeater;
		if (powerSender.SendPower("heaterWattHours", pulsesSend)) {
			pulsesHeater -= pulsesSend;
		}
	}
	
	delay(500);
}
