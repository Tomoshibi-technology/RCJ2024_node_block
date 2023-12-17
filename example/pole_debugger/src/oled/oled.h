#ifndef _H_OLED_
#define _H_OLED_

#include <Arduino.h>
#include <Wire.h>
#include "SSD1306.h"//ディスプレイ用ライブラリを読み込み

class OLED{
		private:
			SSD1306* SSD;

		public:
			OLED(SSD1306* ptr_ssd);
			void init();
			void display_version(int version);
			void clear();
			void show();
			void display_num(String str0, int num0, String str1, int num1);
			void half_display_num(String str0, int num0, String str1, int num1);
			void half_display_3button(bool n0, bool n1, bool n2);
};

#endif