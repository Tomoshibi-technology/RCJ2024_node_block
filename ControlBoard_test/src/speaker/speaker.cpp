#include "./speaker.h"

SPEAKER::SPEAKER(uint8_t ptr_pin){
	PIN = ptr_pin;
}

void SPEAKER::init(){
	pinMode(PIN, OUTPUT);
}

void SPEAKER::on(){
	digitalWrite(PIN, HIGH);
}

void SPEAKER::off(){
	digitalWrite(PIN, LOW);
}
