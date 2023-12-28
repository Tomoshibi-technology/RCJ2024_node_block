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

void led_test(float piyo);

void setup() {
	for(int i=0; i<5; i++)rawpixel[i].begin();
	for(int i=0; i<6; i++)led[i].init();
	for(int i=0; i<4; i++)circuit_led[i].init();

	Serial.begin(115200);
	ser_ctrl.init();
}


uint32_t hoge = 0;
float piyo = -30.0;

uint32_t loop_time = 0;
void loop(){
	Serial.print("loop_time: ");
	Serial.println(micros()-loop_time);
	loop_time = micros();

	circuit_led[0].clear();
	circuit_led[0].set_color_rgb_all(0, 10, 0);
	circuit_led[0].set_width_hsv(2, 5.0*(sin(piyo)+1.0), 10, 240, 100);
	circuit_led[0].show();

  piyo += 0.1;
	hoge++;
	delay(10);
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

void led_test(float piyo){
	for(int i=0; i<4; i++){
		circuit_led[i].clear();
	}
	for(int i=0; i<6; i++){
		led[i].clear();
	}

	circuit_led[0].set_width_hsv(piyo, 5.5, 0, 240, 100);
	circuit_led[1].set_width_hsv(piyo, 5.5, 63, 240, 100);
	circuit_led[2].set_width_hsv(piyo, 5.5, 126, 240, 100);
	circuit_led[3].set_width_hsv(piyo, 5.5, 189, 240, 100);

	for(int i=0; i<6; i++){
		led[i].set_color_hsv_all(0, 10, 3);
	}
	int i = 0;
	led[(0+i)%6].set_width_hsv( 30 + piyo, 7, 0, 240, 150);
	led[(1+i)%6].set_width_hsv( 10 + piyo, 7, 42, 240, 150);
	led[(2+i)%6].set_width_hsv( 20 + piyo, 7, 84, 240, 150);
	led[(3+i)%6].set_width_hsv( 40 + piyo, 7, 126, 240, 150);
	led[(4+i)%6].set_width_hsv( 30 + piyo, 7, 168, 240, 150);
	led[(5+i)%6].set_width_hsv( 26 + piyo, 7, 210, 240, 150);
	for(int i=0; i<4; i++){
		circuit_led[i].show();
	}
	for(int i=0; i<6; i++){
		led[i].show();
	}	
}


