#ifndef _H_SPEAKER_
#define _H_SPEAKER_

#include <Arduino.h>
#include "./speaker_scale.h"

class SPEAKER{
		private:
			uint8_t PIN;

		public:
			SPEAKER(uint8_t ptr_pin);
			void init();
			void on();
			void off();
};

#endif