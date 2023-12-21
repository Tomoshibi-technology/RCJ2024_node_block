#include "./voltage.h"

VOLTAGE::VOLTAGE(int ptr_pin){
	PIN = ptr_pin;
}

void VOLTAGE::init(){
	pinMode(PIN, INPUT);
}

float VOLTAGE::read(){
	voltage = analogRead(PIN)*VolK;
	return voltage;
}

int VOLTAGE::percentage(float max_voltage,float min_voltage){
	//float voltage = read(); //処理がもったいないため省略
	if(voltage > max_voltage){
		return 100;
	}else if(voltage < min_voltage){
		return 0;
	}else{
		return (voltage - min_voltage)/(max_voltage - min_voltage)*100;
	}
}

