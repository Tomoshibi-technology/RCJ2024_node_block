#include <Arduino.h>

#include "./fled/fled.h"
#include <Adafruit_NeoPixel.h>
#define LED_A_PIN 32  // The data pin that the WS2812 strips are connected to.32
#define LED_B_PIN 14
#define LED_C_PIN 13
#define LED_D_PIN 33
#define LED_E_PIN 27
#define LED_NUM 60

Adafruit_NeoPixel rawpixel[5] = {
	Adafruit_NeoPixel(60, LED_A_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(120, LED_B_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(60, LED_C_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(120, LED_D_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(40, LED_E_PIN, NEO_GRB + NEO_KHZ800),
};

FLED led[6] = {
	FLED(&rawpixel[0], 0, 59),
	FLED(&rawpixel[1], 119, 60),
	FLED(&rawpixel[1], 0, 59),
	FLED(&rawpixel[2], 0, 59),
	FLED(&rawpixel[3], 119, 60),
	FLED(&rawpixel[3], 0, 59)
};

FLED circuit_led[4] = {
	FLED(&rawpixel[4], 0, 9), //LED_FROM, LED_TO
	FLED(&rawpixel[4], 10, 19),
	FLED(&rawpixel[4], 20, 29),
	FLED(&rawpixel[4], 30, 39),
};


#include "./ser_ctrl/ser_ctrl.h"

HardwareSerial ctrl(2);
CTRL ser_ctrl(&ctrl);  


void setup() {
	for(int i=0; i<5; i++){
		rawpixel[i].begin();
		rawpixel[i].show();
	}

	for(int i=0; i<6; i++){
		led[i].init();
	}
	ser_ctrl.init();
	Serial.begin(115200);

	for(int i=0; i<4; i++){
		circuit_led[i].init();
	}
}


uint32_t hoge = 0;
float piyo = 0.0;

void loop(){
	for(int i=0; i<6; i++){
		led[i].clear();
	}
	// for(int i=0; i<4; i++){ //虹色ループ
	// 	for(int j=0; j<10; j++){
	// 		circuit_led[i].set_color_hsv(j, 60*i+6*j, 250, 30);
	// 	}
	// }

	circuit_led[0].set_width_rgb(piyo, 5.3, 255, 10, 30);

	piyo += 0.1;

	for(int i=0; i<4; i++){
		circuit_led[i].show();
	}

//----------------------------------------------

	for(int i=0; i<6; i++){
		led[i].clear();
	}
	// ser_ctrl.read();

	// byte kidou = ser_ctrl.data[0]; //10 起動 2 停止
	// byte phase = ser_ctrl.data[1] - 5; //1-4

	// int led_n = j % 6;

	// led[(0+j)%6].set_color_hsv(j-3 ,30, 100, 30);
	// led[(1+j)%6].set_color_hsv(j-2 ,30, 150, 30);
	// led[(2+j)%6].set_color_hsv(j-1 ,30, 200, 30);
	// led[(3+j)%6].set_color_hsv(j ,30, 250, 30);
	// led[(4+j)%6].set_color_hsv(j ,75, 250, 00);
	// led[(5+j)%6].set_color_hsv(j ,90, 250, 00);
	// delay(50);
	

	// if(phase == 3){
	// 	for(int i=0; i<6; i++){
	// 		led[i].set_color_hsv(j	, 0, 150, 100);
	// 		led[i].set_color_hsv(j-1, 0, 100, 100);
	// 		led[i].set_color_hsv(j-2, 0, 70, 100);
	// 		led[i].set_color_hsv(j-3, 0, 40, 100);
	// 	}
	// }
	hoge++;

	for (int i=0; i<6; i++){
		led[i].show();
	}
	delay(50);
}




