#include "./fled.h"

//---------public---------

FLED::FLED(Adafruit_NeoPixel* ptr_neopixel, int led_from, int led_to){
	NEOPIXEL = ptr_neopixel;
	LED_FROM = led_from;
	LED_TO = led_to;
	if(LED_FROM > LED_TO){
		TOTAL = LED_FROM - LED_TO + 1;
	}else{
		TOTAL = LED_TO - LED_FROM + 1;
	}

	pixel_vector = new Vector[TOTAL];
	for(int i=0; i<TOTAL; i++){
		pixel_vector[i].X = cos(i*2*PI/TOTAL);
		pixel_vector[i].Y = sin(i*2*PI/TOTAL);
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
	for(int i=0; i<TOTAL; i++){
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
	float diff = width / 2;
	float from = center - diff;
	float to = center + diff;

	Vector center_vector(cos(center*2.0*PI/float(TOTAL)), sin(center*2.0*PI/float(TOTAL))); //目標のベクトル
	//todo 三角関数の中身がfloatになっているか確認する
	Vector from_vector(cos(from*2.0*PI/float(TOTAL)), sin(from*2.0*PI/float(TOTAL)));
	Vector to_vector(cos(to*2.0*PI/float(TOTAL)), sin(to*2.0*PI/float(TOTAL)));

	float dot_from = (from_vector.X * center_vector.X + from_vector.Y * center_vector.Y +1) /2;
	float dot_to = (to_vector.X * center_vector.X + to_vector.Y * center_vector.Y +1) /2;

	//dot積の値は、0から1までの値をとる
	//1に近いほど、中心に近い

	Serial.print("center:");
	Serial.print(center);
	Serial.print(" diff:");
	Serial.print(diff);
	Serial.print(" from:");
	Serial.print(dot_from);
	Serial.print(" to:");
	Serial.println(dot_to);
	

	Serial.print(" center_vector: ");
	Serial.print(center_vector.X);
	Serial.print(":");
	Serial.println(center_vector.Y);

	Serial.print(" dot: ");
	for(int i=0; i<TOTAL; i++){
		float dot = pixel_vector[i].X * center_vector.X + pixel_vector[i].Y * center_vector.Y;
		Serial.print(i);
		Serial.print(":");
		Serial.print(dot);
		Serial.print("  ");
	}
	Serial.println();
}

void FLED::debug(void){
	Serial.print(" TOTAL: ");
	Serial.print(TOTAL);
	Serial.print("  | ");
	for(int i=0; i<TOTAL; i++){
		Serial.print(pixel_vector[i].X);
		Serial.print(" ");
		Serial.print(pixel_vector[i].Y);
		Serial.print(" | ");
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
	
	if(n >= TOTAL){
		result = n % (TOTAL);
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



