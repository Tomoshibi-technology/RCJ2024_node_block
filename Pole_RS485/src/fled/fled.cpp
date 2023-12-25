#include "./fled.h"

//---------public---------

FLED::FLED(Adafruit_NeoPixel* ptr_neopixel, int led_from, int led_to){
	NEOPIXEL = ptr_neopixel;
	LED_FROM = led_from;
	LED_TO = led_to;
	if(LED_FROM > LED_TO){
		TOTAL = LED_FROM - LED_TO;
	}else{
		TOTAL = LED_TO - LED_FROM;
	}
}

void FLED::init(void){
	NEOPIXEL->begin();
	NEOPIXEL->setBrightness(BRIGHTNESS);
	NEOPIXEL->clear();
	NEOPIXEL->show();
}

void FLED::show(void){
	if(timer + interval < millis()){
		NEOPIXEL->show();
	}
}

void FLED::clear(void){
	NEOPIXEL->clear();
}

void FLED::set_color_rgb(int num, int r, int g, int b){
	NEOPIXEL->setPixelColor(get_num(num), r, g, b);
}

void FLED::set_color_rgb_all(int r, int g, int b){
	for(int i=0; TOTAL; i++){
		NEOPIXEL->setPixelColor(i, r, g, b);
	}
}

void FLED::set_color_hsv(int num, int h, int s, int v){
	int r, g, b; get_hsv2rgb(h, s, v, &r, &g, &b);
	NEOPIXEL->setPixelColor(get_num(num), r, g, b);
	
}


void FLED::set_color_hsv_all(int h, int s, int v){
	int r, g, b; get_hsv2rgb(h, s, v, &r, &g, &b);
	for(int i=0; i<TOTAL; i++){
		NEOPIXEL->setPixelColor(i, r, g, b);
	}
}


void FLED::set_width_rgb(float center, float width, int r, int g, int b){

	while(center > TOTAL){
		center -= (float)TOTAL;
	}
	while(center < 0){
		center += (float)TOTAL;
	}

	float diff = width / 2;
	float from = center - diff;
	float to = center + diff;

	for(int i=0; i<TOTAL; i++){
		if(from <= i && i <= to){
			int R=0, G=0, B=0;
			if(r != 0){
				R = r-(abs(i-center)*r/diff);
			}
			if(g != 0){
				G = g-(abs(i-center)*g/diff);
			}
			if(b != 0){
				B = b-(abs(i-center)*b/diff);
			}
			// G=B=0;
			Serial.print(" | ");
			Serial.print(i);
			Serial.print(" ");
			Serial.print(R);
			Serial.print(" ");
			Serial.print(G);
			Serial.print(" ");
			Serial.print(B);
			set_color_rgb(i, R, G, B);
		}
	}
	Serial.println();

}


//---------private---------

void FLED::get_hsv2rgb(int h, int s, int v, int* r, int* g, int* b){
	if (s == 0) {
		*r = *g = *b = v;
	}else{
		h = h * 6; // sector 0 to 5
		int i = h / 256;
		int f = h % 256;
		int p = (v * (256 - s)) / 256;
		int q = (v * (256 - (s * f) / 256)) / 256;
		int t = (v * (256 - (s * (256 - f)) / 256)) / 256;

		switch (i) {
			case 0: *r = v; *g = t; *b = p; break;
			case 1: *r = q; *g = v; *b = p; break;
			case 2: *r = p; *g = v; *b = t; break;
			case 3: *r = p; *g = q; *b = v; break;
			case 4: *r = t; *g = p; *b = v; break;
			default: *r = v; *g = p; *b = q; break;
		}
	}
}

int FLED::get_num(int n){
	int result;
	
	if(n >= TOTAL + 1){
		result = n % (TOTAL+1);
	}else{
		result = n;
	}

	if(LED_FROM > LED_TO){
		result = LED_FROM - result;
	}else{
		result = LED_FROM + result;
	}
	return result;
}