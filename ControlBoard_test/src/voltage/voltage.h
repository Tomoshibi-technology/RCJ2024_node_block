#ifndef _H_VOLTAGE_
#define _H_VOLTAGE_

#include <Arduino.h>

class VOLTAGE{
	private:
		int PIN;

		float VOL_MAX = 3.3;
		int ADC_RES = 1024;
		int RES1 = 33;
		int RES2 = 10;
		float VolK = VOL_MAX/ADC_RES *(RES1+RES2)/RES2;

		float voltage = 0;

	public:
		VOLTAGE(int ptr_pin);
		void init();
		float read();
		int percentage(float max_voltage,float min_voltage);
};

#endif