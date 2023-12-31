#include <TWELITE>

const uint8_t LED_PIN[2] = {9,8};
int iLedCounter = 0;

void setup() {
	Serial1.setup(64, 192); // 64byte TX, 192byte RX
	Serial1.begin(115200);
	
	pinMode(LED_PIN[0], OUTPUT);
	digitalWrite(LED_PIN[0], LOW); // TURN DO1 ON

	Timer0.begin(1); // 10Hz Timer 

	Serial << "--- F446 and ZigBee ---" << crlf;
}

/*** loop procedure (called every event) */
byte send = 0;

void loop() {
	Serial1 << send;

	while(Serial1.available()) {
		byte receive = Serial1.read();
		Serial << format("[%d]", receive) << crlf;
	}

	if (Timer0.available()) {
		if (iLedCounter == 0) {
			digitalWrite(LED_PIN[0], HIGH);
			iLedCounter = 1;
			send = 10;
		} else {
			digitalWrite(LED_PIN[0], LOW);
			iLedCounter = 0;
			send = 20;
		}
	}
}