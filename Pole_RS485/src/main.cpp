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
	circuit_led[0].set_width_rgb(5, 4, 255, 10, 30);
	// circuit_led[0].debug();

	circuit_led[0].clear();
	circuit_led[0].set_color_rgb_all(5, 10, 7);
	circuit_led[0].show();
	delay(1000);

	circuit_led[0].clear();
	circuit_led[0].set_color_rgb(0, 10, 0, 0);
	circuit_led[0].set_color_rgb(4, 10, 0, 0);
	circuit_led[0].set_color_rgb(5, 10, 0, 0);
	circuit_led[0].set_color_rgb(9, 10, 0, 0);
	circuit_led[0].set_color_rgb(10, 0, 10, 0);
	circuit_led[0].show();
	delay(1000);

	// for(int i=0; i<6; i++){
	// 	led[i].clear();
	// }

	// circuit_led[0].set_width_rgb(0, 5.3, 255, 10, 30);
	// for(int i=0; i<4; i++){
	// 	circuit_led[i].show();
	// }

	// piyo += 0.1;
	// hoge++;

	// delay(50);
}


//ーーーーーーーーーーーーーーーメモ帳ーーーーーーーーーーーーーーーー

	// for(int i=0; i<4; i++){ //虹色ループ
	// 	for(int j=0; j<10; j++){
	// 		circuit_led[i].set_color_hsv(j, 60*i+6*j, 250, 30);
	// 	}
	// }

	// ser_ctrl.read();
	// byte kidou = ser_ctrl.data[0]; //10 起動 2 停止
	// byte phase = ser_ctrl.data[1] - 5; //1-4


