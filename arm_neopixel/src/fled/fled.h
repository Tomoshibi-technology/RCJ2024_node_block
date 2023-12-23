#ifndef _H_FLED_
#define _H_FLED_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class FLED{
	private:
		Adafruit_NeoPixel* NEOPIXEL;
		int NUM;
		int BRIGHTNESS = 255;
	
	public:
		FLED(Adafruit_NeoPixel* ptr_neopixel, int num);
		void init(void);
		void show(void);
		void clear(void);
		void set_color_rgb(int n, int r, int g, int b);
		void set_color_hsv(int n, int h, int s, int v);
};

#endif