#ifndef _H_FLED_
#define _H_FLED_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../cossim/cossim.h"

class FLED{
	private:
		Adafruit_NeoPixel* NEOPIXEL;
		int BRIGHTNESS = 255;

		int TOTAL;
		int LED_FROM;
		int LED_TO;

		long timer = 0; 
		int interval = 20; //ms

		int get_num(int n);
		void get_hsv2rgb(int h, int s, int v, int* r, int* g, int* b);

	public:
		FLED(Adafruit_NeoPixel* ptr_neopixel, int led_from, int led_to);

		void init(void);
		void show(void);
		void clear(void);

		void set_color_rgb(int num, int r, int g, int b);
		void set_color_hsv(int num, int h, int s, int v);
		void set_color_rgb_all(int r, int g, int b);
		void set_color_hsv_all(int h, int s, int v);

		void set_width_rgb(float center, float width, int r, int g, int b);
		void set_width_hsv(float center, float width, int h, int s, int v);
};

#endif