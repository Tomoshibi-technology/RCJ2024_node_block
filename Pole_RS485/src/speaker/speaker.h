#ifndef _H_SPEAKER_
#define _H_SPEAKER_

#include <Arduino.h>
#include "./frequency.h"

class SPEAKER{
		private:
			uint8_t PIN;
			int pre_freq = 0;

			const uint8_t CHANNEL = 0;

		public:
			SPEAKER(uint8_t ptr_pin);
			void init();
			void ring(int freq);
			void mute(void);

			void beat(int freq, float beat, int bpm);

			void boot_music(void);
			void boot2_music(void);

			uint16_t keybord[88]{
				A0, A0s, B0,
				C1, C1s, D1, D1s, E1, F1, F1s, G1, G1s, A1, A1s, B1,
				C2, C2s, D2, D2s, E2, F2, F2s, G2, G2s, A2, A2s, B2,
				C3, C3s, D3, D3s, E3, F3, F3s, G3, G3s, A3, A3s, B3,
				C4, C4s, D4, D4s, E4, F4, F4s, G4, G4s, A4, A4s, B4,
				C5, C5s, D5, D5s, E5, F5, F5s, G5, G5s, A5, A5s, B5,
				C6, C6s, D6, D6s, E6, F6, F6s, G6, G6s, A6, A6s, B6,
				C7, C7s, D7, D7s, E7, F7, F7s, G7, G7s, A7, A7s, B7,
				C8
			};

			uint16_t major[12][8]{ //参考 https://soundquest.jp/quest/prerequisite/scale-3/
				{C4, D4, E4, F4, G4, A4, B4, C5}, //C
				{D4f, E4f, F4, G4f, A4f, B4f, C5, D5f}, //Df
				{D4, E4, F4s, G4, A4, B4, C5s, D5}, //D
				{E4f, F4, G4, A4f, B4f, C5, D5, E5f}, //Ef
				{E4, F4s, G4s, A4, B4, C5s, D5s, E5}, //E
				{F4, G4, A4, B4f, C5, D5, E5, F5}, //F
				{G4f, A4f, B4f, C5, D5f, E5f, F5, G5f}, //Gf
				{G4, A4, B4, C5, D5, E5, F5s, G5}, //G
				{A3f, B3f, C4, D4f, E4f, F4, G4, A4f}, //Af
				{A3, B3, C4s, D4, E4, F4s, G4s, A4}, //A
				{B3f, C4, D4, E4f, F4, G4, A4, B4f}, //Bf
				{B3, C4s, D4s, E4, F4s, G4s, A4s, B4} //B
			};


};

#endif